/**
 * @file inference_engine.h
 * @brief 模型推理引擎
 * 
 * 支持：单线程推理、多线程批量推理、异步推理
 */

#ifndef MODEL_INFERENCE_ENGINE_H
#define MODEL_INFERENCE_ENGINE_H

#include "tensor.h"
#include "layers.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>
#include <future>
#include <vector>
#include <functional>
#include <chrono>

namespace inference {

/**
 * @brief 推理结果
 */
struct InferenceResult {
    Tensor output;
    std::chrono::microseconds latency;
    bool success;
    std::string error_message;
    
    InferenceResult() : success(false) {}
    
    InferenceResult(Tensor&& out, std::chrono::microseconds lat)
        : output(std::move(out)), latency(lat), success(true) {}
    
    static InferenceResult error(const std::string& msg) {
        InferenceResult result;
        result.success = false;
        result.error_message = msg;
        return result;
    }
};

/**
 * @brief 推理引擎配置
 */
struct EngineConfig {
    size_t num_threads = 4;          // 工作线程数
    size_t batch_size = 32;          // 默认批大小
    bool enable_profiling = true;    // 是否启用性能分析
    size_t max_queue_size = 1000;    // 最大队列大小
};

/**
 * @brief 简单推理引擎（单线程）
 */
class InferenceEngine {
public:
    explicit InferenceEngine(Sequential* model) : model_(model) {}
    
    /**
     * @brief 单样本推理
     */
    Tensor infer(const Tensor& input) {
        auto start = std::chrono::high_resolution_clock::now();
        Tensor output = model_->forward(input);
        auto end = std::chrono::high_resolution_clock::now();
        
        if (config_.enable_profiling) {
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            std::cout << "[Inference] Latency: " << duration.count() << " us\n";
        }
        
        return output;
    }
    
    /**
     * @brief 批量推理
     */
    std::vector<Tensor> inferBatch(const std::vector<Tensor>& inputs) {
        std::vector<Tensor> outputs;
        outputs.reserve(inputs.size());
        
        auto start = std::chrono::high_resolution_clock::now();
        
        for (const auto& input : inputs) {
            outputs.push_back(model_->forward(input));
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        
        if (config_.enable_profiling) {
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            std::cout << "[Batch Inference] Total: " << duration.count() << " us, "
                      << "Per sample: " << duration.count() / inputs.size() << " us\n";
        }
        
        return outputs;
    }
    
    void setConfig(const EngineConfig& config) { config_ = config; }

protected:
    Sequential* model_;
    EngineConfig config_;
};

/**
 * @brief 多线程推理引擎
 */
class ParallelInferenceEngine : public InferenceEngine {
public:
    explicit ParallelInferenceEngine(Sequential* model, size_t num_threads = 4)
        : InferenceEngine(model), 
          stop_(false),
          num_threads_(num_threads) {
        startWorkers();
    }
    
    ~ParallelInferenceEngine() {
        stop();
    }
    
    /**
     * @brief 异步推理
     */
    std::future<InferenceResult> inferAsync(const Tensor& input) {
        auto promise = std::make_shared<std::promise<InferenceResult>>();
        auto future = promise->get_future();
        
        {
            std::lock_guard<std::mutex> lock(queue_mutex_);
            task_queue_.emplace([this, input, promise]() {
                auto start = std::chrono::high_resolution_clock::now();
                try {
                    Tensor output = model_->forward(input);
                    auto end = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
                    promise->set_value(InferenceResult(std::move(output), duration));
                } catch (const std::exception& e) {
                    promise->set_value(InferenceResult::error(e.what()));
                }
            });
        }
        
        queue_cv_.notify_one();
        return future;
    }
    
    /**
     * @brief 批量并行推理
     */
    std::vector<InferenceResult> inferBatchParallel(
            const std::vector<Tensor>& inputs) {
        std::vector<std::future<InferenceResult>> futures;
        futures.reserve(inputs.size());
        
        auto total_start = std::chrono::high_resolution_clock::now();
        
        // 提交所有任务
        for (const auto& input : inputs) {
            futures.push_back(inferAsync(input));
        }
        
        // 收集结果
        std::vector<InferenceResult> results;
        results.reserve(inputs.size());
        
        for (auto& future : futures) {
            results.push_back(future.get());
        }
        
        auto total_end = std::chrono::high_resolution_clock::now();
        
        if (config_.enable_profiling) {
            auto total_duration = std::chrono::duration_cast<std::chrono::microseconds>(
                total_end - total_start);
            
            double total_latency = 0;
            for (const auto& result : results) {
                if (result.success) {
                    total_latency += result.latency.count();
                }
            }
            
            std::cout << "[Parallel Batch] Samples: " << inputs.size() 
                      << ", Threads: " << num_threads_
                      << ", Total time: " << total_duration.count() << " us"
                      << ", Throughput: " << (inputs.size() * 1000000.0 / total_duration.count())
                      << " samples/sec\n";
        }
        
        return results;
    }
    
    /**
     * @brief 停止引擎
     */
    void stop() {
        {
            std::lock_guard<std::mutex> lock(queue_mutex_);
            stop_ = true;
        }
        queue_cv_.notify_all();
        
        for (auto& worker : workers_) {
            if (worker.joinable()) {
                worker.join();
            }
        }
        workers_.clear();
    }

private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> task_queue_;
    std::mutex queue_mutex_;
    std::condition_variable queue_cv_;
    std::atomic<bool> stop_;
    size_t num_threads_;
    
    void startWorkers() {
        for (size_t i = 0; i < num_threads_; ++i) {
            workers_.emplace_back([this]() {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(queue_mutex_);
                        queue_cv_.wait(lock, [this] { 
                            return stop_ || !task_queue_.empty(); 
                        });
                        
                        if (stop_ && task_queue_.empty()) {
                            return;
                        }
                        
                        task = std::move(task_queue_.front());
                        task_queue_.pop();
                    }
                    
                    task();
                }
            });
        }
    }
};

/**
 * @brief 推理统计信息
 */
struct InferenceStats {
    size_t total_requests = 0;
    size_t successful_requests = 0;
    size_t failed_requests = 0;
    double total_latency_ms = 0;
    double avg_latency_ms = 0;
    double min_latency_ms = std::numeric_limits<double>::max();
    double max_latency_ms = 0;
    double throughput = 0;  // requests per second
    
    void update(const InferenceResult& result, double elapsed_sec) {
        total_requests++;
        if (result.success) {
            successful_requests++;
            double latency = result.latency.count() / 1000.0;  // to ms
            total_latency_ms += latency;
            min_latency_ms = std::min(min_latency_ms, latency);
            max_latency_ms = std::max(max_latency_ms, latency);
        } else {
            failed_requests++;
        }
        
        if (total_requests > 0) {
            avg_latency_ms = total_latency_ms / successful_requests;
            throughput = total_requests / elapsed_sec;
        }
    }
    
    void print() const {
        std::cout << "\n========== Inference Statistics ==========\n";
        std::cout << "Total requests:    " << total_requests << "\n";
        std::cout << "Successful:        " << successful_requests << "\n";
        std::cout << "Failed:            " << failed_requests << "\n";
        std::cout << "Avg latency:       " << std::fixed << std::setprecision(3) 
                  << avg_latency_ms << " ms\n";
        std::cout << "Min latency:       " << min_latency_ms << " ms\n";
        std::cout << "Max latency:       " << max_latency_ms << " ms\n";
        std::cout << "Throughput:        " << std::setprecision(2) 
                  << throughput << " req/sec\n";
        std::cout << "==========================================\n";
    }
};

} // namespace inference

#endif // MODEL_INFERENCE_ENGINE_H
