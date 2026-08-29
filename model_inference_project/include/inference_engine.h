/**
 * @file inference_engine.h
 * @brief 单线程与有界队列多线程推理引擎
 */

#ifndef MODEL_INFERENCE_ENGINE_H
#define MODEL_INFERENCE_ENGINE_H

#include "layers.h"
#include "tensor.h"

#include <algorithm>
#include <cassert>
#include <chrono>
#include <condition_variable>
#include <exception>
#include <functional>
#include <future>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>
#include <vector>

namespace inference {

struct InferenceResult {
    Tensor output;
    std::chrono::microseconds latency{0};
    bool success{false};
    std::string error_message;

    InferenceResult() = default;

    InferenceResult(Tensor&& value, std::chrono::microseconds elapsed)
        : output(std::move(value)), latency(elapsed), success(true) {}

    static InferenceResult error(std::string message) {
        InferenceResult result;
        result.error_message = std::move(message);
        return result;
    }
};

struct EngineConfig {
    size_t num_threads = 4;
    size_t batch_size = 32;
    bool enable_profiling = true;
    size_t max_queue_size = 1000;
};

enum class EngineLifecycle {
    accepting,
    draining,
    joined
};

struct EngineStatus {
    EngineLifecycle lifecycle{EngineLifecycle::accepting};
    bool accepting_requests{true};
    size_t queued_requests{0};
    size_t active_requests{0};
    size_t in_flight_calls{0};
};

inline void validateEngineConfig(const EngineConfig& config) {
    if (config.num_threads == 0) {
        throw std::invalid_argument("EngineConfig num_threads must be greater than zero");
    }
    if (config.batch_size == 0) {
        throw std::invalid_argument("EngineConfig batch_size must be greater than zero");
    }
    if (config.max_queue_size == 0) {
        throw std::invalid_argument("EngineConfig max_queue_size must be greater than zero");
    }
}

namespace detail {

inline thread_local size_t inference_callback_depth = 0;

inline bool insideInferenceCallback() noexcept {
    return inference_callback_depth != 0;
}

inline std::string callbackInferenceRejection(const char* operation) {
    return std::string(operation) +
           " rejected: inference callbacks are leaf computations and may not "
           "invoke another inference API";
}

inline void rejectInferenceFromCallback(const char* operation) {
    if (insideInferenceCallback()) {
        throw std::logic_error(callbackInferenceRejection(operation));
    }
}

class InferenceCallbackScope {
public:
    explicit InferenceCallbackScope(bool active = true) noexcept : active_(active) {
        if (active_) {
            ++inference_callback_depth;
        }
    }

    ~InferenceCallbackScope() {
        if (active_) {
            --inference_callback_depth;
        }
    }

    InferenceCallbackScope(const InferenceCallbackScope&) = delete;
    InferenceCallbackScope& operator=(const InferenceCallbackScope&) = delete;

private:
    bool active_;
};

}  // namespace detail

// thread_local 只描述当前 OS 线程，不会自动传播到新线程。这个值对象把
// “当前工作源自模型回调”显式带到 std::thread/std::async：在回调内 capture，
// 在被委托线程最外层 run。外部线程 capture 得到 false，不会被误判为回调；
// 若回调委托者忘记传播，库无法从一个普通 OS 线程反推出它的祖先。
class InferenceExecutionContext {
public:
    static InferenceExecutionContext capture() noexcept {
        return InferenceExecutionContext(detail::insideInferenceCallback());
    }

    [[nodiscard]] bool carriesCallbackContext() const noexcept {
        return callback_context_;
    }

    template <typename Function>
    decltype(auto) run(Function&& function) const {
        detail::InferenceCallbackScope callback(callback_context_);
        return std::invoke(std::forward<Function>(function));
    }

private:
    explicit InferenceExecutionContext(bool callback_context) noexcept
        : callback_context_(callback_context) {}

    bool callback_context_;
};

class InferenceEngine {
public:
    explicit InferenceEngine(const Sequential* model, EngineConfig config = {})
        : model_(model), config_(config) {
        if (model_ == nullptr) {
            throw std::invalid_argument("InferenceEngine requires a non-null model");
        }
        validateEngineConfig(config_);
    }

    // 析构不能抛异常来“拒绝销毁”；回调栈上的销毁属于不可恢复契约违例。
    virtual ~InferenceEngine() {
        if (detail::insideInferenceCallback()) {
            std::terminate();
        }
    }

    Tensor infer(const Tensor& input) const { return inferImpl(input); }

    std::vector<Tensor> inferBatch(const std::vector<Tensor>& inputs) const {
        return inferBatchImpl(inputs);
    }

    void setConfig(const EngineConfig& config) { setConfigImpl(config); }

    EngineConfig config() const {
        std::lock_guard<std::mutex> lock(config_mutex_);
        return config_;
    }

protected:
    virtual Tensor inferImpl(const Tensor& input) const {
        detail::rejectInferenceFromCallback("Synchronous infer");
        const EngineConfig snapshot = config();
        const auto start = std::chrono::steady_clock::now();
        detail::InferenceCallbackScope callback;
        Tensor output = model_->forward(input);
        const auto end = std::chrono::steady_clock::now();
        if (snapshot.enable_profiling) {
            const auto duration =
                std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            std::cout << "[Inference] Latency: " << duration.count() << " us\n";
        }
        return output;
    }

    virtual std::vector<Tensor> inferBatchImpl(
        const std::vector<Tensor>& inputs) const {
        detail::rejectInferenceFromCallback("Synchronous inferBatch");
        const EngineConfig snapshot = config();
        std::vector<Tensor> outputs;
        outputs.reserve(inputs.size());
        if (inputs.empty()) {
            if (snapshot.enable_profiling) {
                std::cout << "[Batch Inference] Samples: 0, Total: 0 us, Per sample: 0 us\n";
            }
            return outputs;
        }

        const auto start = std::chrono::steady_clock::now();
        detail::InferenceCallbackScope callback;
        size_t offset = 0;
        while (offset < inputs.size()) {
            const size_t window_size =
                std::min(snapshot.batch_size, inputs.size() - offset);
            const size_t end = offset + window_size;
            for (size_t i = offset; i < end; ++i) {
                outputs.push_back(model_->forward(inputs[i]));
            }
            offset = end;
        }
        const auto end = std::chrono::steady_clock::now();

        if (snapshot.enable_profiling) {
            const auto duration =
                std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            const double per_sample =
                static_cast<double>(duration.count()) / static_cast<double>(inputs.size());
            std::cout << "[Batch Inference] Samples: " << inputs.size()
                      << ", Total: " << duration.count() << " us"
                      << ", Per sample: " << per_sample << " us\n";
        }
        return outputs;
    }

    virtual void setConfigImpl(const EngineConfig& config) {
        validateEngineConfig(config);
        std::lock_guard<std::mutex> lock(config_mutex_);
        config_ = config;
    }

    const Sequential* model_;

private:
    mutable std::mutex config_mutex_;
    EngineConfig config_;
};

/**
 * @brief 多线程并行推理引擎：有界任务队列 + 可配置的 worker 启动方式。
 *
 * 析构契约（最关键的前置条件，违反会 std::terminate 中止整个进程）：
 * 析构 ParallelInferenceEngine 时，绝不能正处在该引擎的推理回调内部，
 * 也绝不能有任何 inferAsync / inferBatchParallel 调用仍在栈上（在途）。
 * 合法做法是：先 close() 停止接收、join 所有发起推理的线程、等异步 future
 * 都完成，再析构；析构会自动排空已接收的异步任务。
 * 这与 std::thread/std::jthread、boost::asio 的 fail-fast 惯例一致——回调内
 * 销毁或撞上栈上同步调用无法安全组合，没有可恢复的异常路径。Debug 构建下
 * 会先 assert 给出定位信息，Release 构建直接 terminate。
 */
class ParallelInferenceEngine : public InferenceEngine {
public:
    using WorkerTask = std::function<void()>;
    using WorkerLauncher = std::function<std::thread(WorkerTask)>;

private:
    struct Runtime {
        std::queue<WorkerTask> task_queue;
        std::mutex mutex;
        std::condition_variable condition;
        EngineLifecycle lifecycle{EngineLifecycle::accepting};
        size_t active_requests{0};
        size_t in_flight_calls{0};
    };

    class InFlightCall {
    public:
        InFlightCall(std::shared_ptr<Runtime> runtime,
                     const char* operation,
                     bool executes_model)
            : runtime_(std::move(runtime)), executes_model_(executes_model) {
            std::lock_guard<std::mutex> lock(runtime_->mutex);
            if (runtime_->lifecycle != EngineLifecycle::accepting) {
                throw std::logic_error(std::string(operation) +
                                       " rejected: inference engine is closed");
            }
            ++runtime_->in_flight_calls;
            if (executes_model_) {
                ++runtime_->active_requests;
            }
        }

        ~InFlightCall() {
            std::lock_guard<std::mutex> lock(runtime_->mutex);
            if (executes_model_) {
                --runtime_->active_requests;
            }
            --runtime_->in_flight_calls;
            runtime_->condition.notify_all();
        }

        InFlightCall(const InFlightCall&) = delete;
        InFlightCall& operator=(const InFlightCall&) = delete;

    private:
        std::shared_ptr<Runtime> runtime_;
        bool executes_model_;
    };

public:
    explicit ParallelInferenceEngine(const Sequential* model, size_t num_threads = 4)
        : ParallelInferenceEngine(model, configForThreads(num_threads)) {}

    ParallelInferenceEngine(const Sequential* model, const EngineConfig& config)
        : ParallelInferenceEngine(model, config, defaultWorkerLauncher()) {}

    // 显式 launcher 是故障注入缝：生产代码使用默认 launcher，测试可确定性模拟
    // 第 N 个 std::thread 创建失败并验证已启动 worker 的有限回滚。
    ParallelInferenceEngine(const Sequential* model,
                            const EngineConfig& config,
                            WorkerLauncher launcher)
        : InferenceEngine(model, config),
          runtime_(std::make_shared<Runtime>()),
          num_threads_(config.num_threads) {
        if (!launcher) {
            throw std::invalid_argument("ParallelInferenceEngine requires a worker launcher");
        }
        startWorkers(std::move(launcher));
    }

    ~ParallelInferenceEngine() override {
        // active 异步任务可由外部析构排空，但回调内销毁或撞上仍在栈上的
        // 同步/阻塞批量调用无法安全组合，只能 fail-fast。
        if (detail::insideInferenceCallback() || hasInFlightCalls()) {
            // Debug 构建下先 assert 给出定位信息，便于学习者排查；Release(NDEBUG)下
            // assert 无效，仍走 terminate，保持 fail-fast 契约。
            assert((!"ParallelInferenceEngine 析构契约违反：析构时正处在推理回调内或仍有在途调用，须先 close()+join 并等异步 future 完成"));
            std::terminate();
        }
        shutdownFromDestructor();
    }

    ParallelInferenceEngine(const ParallelInferenceEngine&) = delete;
    ParallelInferenceEngine& operator=(const ParallelInferenceEngine&) = delete;

protected:
    Tensor inferImpl(const Tensor& input) const override {
        detail::rejectInferenceFromCallback("Synchronous infer");
        InFlightCall call(runtime_, "Synchronous infer", true);
        return InferenceEngine::inferImpl(input);
    }

    std::vector<Tensor> inferBatchImpl(
        const std::vector<Tensor>& inputs) const override {
        detail::rejectInferenceFromCallback("Synchronous inferBatch");
        InFlightCall call(runtime_, "Synchronous inferBatch", true);
        return InferenceEngine::inferBatchImpl(inputs);
    }

    void setConfigImpl(const EngineConfig& config) override {
        validateEngineConfig(config);
        std::lock_guard<std::mutex> lock(runtime_->mutex);
        if (runtime_->lifecycle != EngineLifecycle::accepting) {
            throw std::logic_error("Cannot configure a closed inference engine");
        }
        if (config.num_threads != num_threads_) {
            throw std::invalid_argument(
                "ParallelInferenceEngine worker count is fixed at construction");
        }
        InferenceEngine::setConfigImpl(config);
    }

public:
    std::future<InferenceResult> inferAsync(const Tensor& input) {
        auto promise = std::make_shared<std::promise<InferenceResult>>();
        std::future<InferenceResult> future = promise->get_future();

        if (detail::insideInferenceCallback()) {
            completePromise(
                promise,
                InferenceResult::error(
                    detail::callbackInferenceRejection("Asynchronous infer")));
            return future;
        }

        bool accepted = false;
        std::string rejection;

        try {
            WorkerTask task = [model = model_, input, promise]() noexcept {
                try {
                    InferenceResult result;
                    try {
                        const auto start = std::chrono::steady_clock::now();
                        detail::InferenceCallbackScope callback;
                        Tensor output = model->forward(input);
                        const auto end = std::chrono::steady_clock::now();
                        const auto duration =
                            std::chrono::duration_cast<std::chrono::microseconds>(end - start);
                        result = InferenceResult(std::move(output), duration);
                    } catch (const std::exception& error) {
                        result = InferenceResult::error(error.what());
                    } catch (...) {
                        result = InferenceResult::error(
                            "Inference failed with an unknown exception");
                    }
                    completePromise(promise, std::move(result));
                } catch (...) {
                    completePromiseWithException(promise, std::current_exception());
                }
            };

            // 先单独取 config 快照（config() 内部自加 config_mutex_），避免持
            // runtime_->mutex 的同时再抢 config_mutex_，缩小嵌套临界区。
            const EngineConfig snapshot = config();
            {
                std::lock_guard<std::mutex> lock(runtime_->mutex);
                if (runtime_->lifecycle != EngineLifecycle::accepting) {
                    rejection = "Inference request rejected: engine is closed";
                } else if (runtime_->task_queue.size() >= snapshot.max_queue_size) {
                    rejection = "Inference request rejected: task queue is full";
                } else {
                    runtime_->task_queue.emplace(std::move(task));
                    accepted = true;
                }
            }
        } catch (...) {
            completePromiseWithException(promise, std::current_exception());
            return future;
        }

        if (accepted) {
            runtime_->condition.notify_one();
        } else {
            completePromise(promise, InferenceResult::error(std::move(rejection)));
        }
        return future;
    }

    std::vector<InferenceResult> inferBatchParallel(const std::vector<Tensor>& inputs) {
        detail::rejectInferenceFromCallback("Parallel inferBatch");
        InFlightCall call(runtime_, "Parallel inferBatch", false);

        std::vector<InferenceResult> results;
        results.reserve(inputs.size());
        if (inputs.empty()) {
            if (profilingEnabled()) {
                std::cout << "[Parallel Batch] Samples: 0, Threads: " << num_threads_
                          << ", Total time: 0 us, Throughput: 0 samples/sec\n";
            }
            return results;
        }

        const auto start = std::chrono::steady_clock::now();
        const EngineConfig snapshot = configSnapshot();
        const size_t submission_window =
            std::min(snapshot.batch_size, snapshot.max_queue_size);

        size_t offset = 0;
        while (offset < inputs.size()) {
            const size_t window_size =
                std::min(submission_window, inputs.size() - offset);
            const size_t end = offset + window_size;
            std::vector<std::future<InferenceResult>> futures;
            futures.reserve(end - offset);
            for (size_t i = offset; i < end; ++i) {
                futures.push_back(inferAsync(inputs[i]));
            }
            for (auto& future : futures) {
                results.push_back(future.get());
            }
            offset = end;
        }

        const auto end = std::chrono::steady_clock::now();
        if (snapshot.enable_profiling) {
            const auto duration =
                std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            const double throughput = duration.count() > 0
                ? static_cast<double>(inputs.size()) * 1'000'000.0 /
                      static_cast<double>(duration.count())
                : 0.0;
            std::cout << "[Parallel Batch] Samples: " << inputs.size()
                      << ", Threads: " << num_threads_
                      << ", Total time: " << duration.count() << " us"
                      << ", Throughput: " << throughput << " samples/sec\n";
        }
        return results;
    }

    // close 同时关闭同步和异步入口；已接收工作继续排空。
    void close() { requestClose(); }

    // stop 始终只是非阻塞关闭请求。它不依赖回调标记，所以即使委托者忘记
    // 传播执行上下文，stop 本身也不会 join worker 并形成等待环。
    void stop() { close(); }

    // join 是显式阻塞边界。库能强制拒绝同线程回调和显式传播后的回调；
    // 对忘记传播的回调子线程，调用者前置条件是不得在其中调用 join。
    // 正常外部并发不携带回调上下文，仍可调用 join。先检查再 close，使被
    // 拒绝的 join 不会偷偷改变目标引擎状态。
    void join() {
        if (detail::insideInferenceCallback()) {
            throw std::logic_error(
                "Blocking join rejected: inference callback context must not wait for workers");
        }
        close();
        joinWorkers();

        std::unique_lock<std::mutex> lock(runtime_->mutex);
        runtime_->condition.wait(lock, [this]() {
            return runtime_->task_queue.empty() && runtime_->active_requests == 0 &&
                   runtime_->in_flight_calls == 0;
        });
        runtime_->lifecycle = EngineLifecycle::joined;
        runtime_->condition.notify_all();
    }

    EngineStatus status() const {
        std::lock_guard<std::mutex> lock(runtime_->mutex);
        return EngineStatus{
            runtime_->lifecycle,
            runtime_->lifecycle == EngineLifecycle::accepting,
            runtime_->task_queue.size(),
            runtime_->active_requests,
            runtime_->in_flight_calls};
    }

    bool closed() const noexcept {
        try {
            return !status().accepting_requests;
        } catch (...) {
            return true;
        }
    }

    bool stopped() const noexcept {
        try {
            return status().lifecycle == EngineLifecycle::joined;
        } catch (...) {
            return false;
        }
    }

private:
    std::shared_ptr<Runtime> runtime_;
    std::vector<std::thread> workers_;
    std::mutex join_mutex_;
    const size_t num_threads_;

    static EngineConfig configForThreads(size_t num_threads) {
        EngineConfig config;
        config.num_threads = num_threads;
        return config;
    }

    static WorkerLauncher defaultWorkerLauncher() {
        return [](WorkerTask task) { return std::thread(std::move(task)); };
    }

    EngineConfig configSnapshot() const { return config(); }

    bool profilingEnabled() const { return config().enable_profiling; }

    bool hasInFlightCalls() const noexcept {
        try {
            std::lock_guard<std::mutex> lock(runtime_->mutex);
            return runtime_->in_flight_calls != 0;
        } catch (...) {
            std::terminate();
        }
    }

    static void completePromise(const std::shared_ptr<std::promise<InferenceResult>>& promise,
                                InferenceResult result) noexcept {
        try {
            promise->set_value(std::move(result));
        } catch (...) {
            // future 已放弃或已完成时，不允许异常逃出 worker 并触发 terminate。
        }
    }

    static void completePromiseWithException(
        const std::shared_ptr<std::promise<InferenceResult>>& promise,
        std::exception_ptr error) noexcept {
        try {
            promise->set_exception(std::move(error));
        } catch (...) {
        }
    }

    void requestClose() {
        {
            std::lock_guard<std::mutex> lock(runtime_->mutex);
            if (runtime_->lifecycle == EngineLifecycle::accepting) {
                runtime_->lifecycle = EngineLifecycle::draining;
            }
        }
        runtime_->condition.notify_all();
    }

    void requestCloseNoexcept() noexcept {
        try {
            requestClose();
        } catch (...) {
        }
    }

    static void workerLoop(const std::shared_ptr<Runtime>& runtime) noexcept {
        while (true) {
            WorkerTask task;
            try {
                std::unique_lock<std::mutex> lock(runtime->mutex);
                runtime->condition.wait(lock, [&runtime]() {
                    return runtime->lifecycle != EngineLifecycle::accepting ||
                           !runtime->task_queue.empty();
                });
                if (runtime->lifecycle != EngineLifecycle::accepting &&
                    runtime->task_queue.empty()) {
                    break;
                }
                task = std::move(runtime->task_queue.front());
                runtime->task_queue.pop();
                ++runtime->active_requests;
            } catch (...) {
                break;
            }

            try {
                task();
            } catch (...) {
                // 队列边界保证任何任务异常都不会越过 worker 入口。
            }

            try {
                std::lock_guard<std::mutex> lock(runtime->mutex);
                --runtime->active_requests;
                runtime->condition.notify_all();
            } catch (...) {
                break;
            }
        }
    }

    void startWorkers(WorkerLauncher launcher) {
        workers_.reserve(num_threads_);
        try {
            for (size_t i = 0; i < num_threads_; ++i) {
                std::thread worker = launcher([runtime = runtime_]() noexcept {
                    workerLoop(runtime);
                });
                if (!worker.joinable()) {
                    throw std::runtime_error("Worker launcher returned a non-joinable thread");
                }
                workers_.emplace_back(std::move(worker));
            }
        } catch (...) {
            const std::exception_ptr startup_error = std::current_exception();
            requestCloseNoexcept();
            joinWorkersNoexcept();
            std::rethrow_exception(startup_error);
        }
    }

    void joinWorkers() {
        std::lock_guard<std::mutex> lock(join_mutex_);
        for (auto& worker : workers_) {
            if (worker.joinable()) {
                worker.join();
            }
        }
        workers_.clear();
    }

    static void joinNoexcept(std::thread& worker) noexcept {
        if (!worker.joinable()) {
            return;
        }
        try {
            worker.join();
        } catch (...) {
            std::terminate();
        }
    }

    void joinWorkersNoexcept() noexcept {
        try {
            std::lock_guard<std::mutex> lock(join_mutex_);
            for (auto& worker : workers_) {
                joinNoexcept(worker);
            }
            workers_.clear();
        } catch (...) {
            std::terminate();
        }
    }

    void shutdownFromDestructor() noexcept {
        requestCloseNoexcept();
        joinWorkersNoexcept();
    }
};

struct InferenceStats {
    size_t total_requests = 0;
    size_t successful_requests = 0;
    size_t failed_requests = 0;
    double total_latency_ms = 0.0;
    double avg_latency_ms = 0.0;
    double min_latency_ms = 0.0;
    double max_latency_ms = 0.0;
    double throughput = 0.0;

    void update(const InferenceResult& result, double elapsed_sec) {
        ++total_requests;
        if (result.success) {
            ++successful_requests;
            const double latency = static_cast<double>(result.latency.count()) / 1000.0;
            total_latency_ms += latency;
            if (successful_requests == 1) {
                min_latency_ms = latency;
                max_latency_ms = latency;
            } else {
                min_latency_ms = std::min(min_latency_ms, latency);
                max_latency_ms = std::max(max_latency_ms, latency);
            }
            avg_latency_ms =
                total_latency_ms / static_cast<double>(successful_requests);
        } else {
            ++failed_requests;
        }
        throughput = elapsed_sec > 0.0
            ? static_cast<double>(total_requests) / elapsed_sec
            : 0.0;
    }

    void print() const {
        std::cout << "\n========== Inference Statistics ==========\n"
                  << "Total requests:    " << total_requests << '\n'
                  << "Successful:        " << successful_requests << '\n'
                  << "Failed:            " << failed_requests << '\n'
                  << "Avg latency:       " << std::fixed << std::setprecision(3)
                  << avg_latency_ms << " ms\n"
                  << "Min latency:       " << min_latency_ms << " ms\n"
                  << "Max latency:       " << max_latency_ms << " ms\n"
                  << "Throughput:        " << std::setprecision(2) << throughput
                  << " req/sec\n"
                  << "==========================================\n";
    }
};

}  // namespace inference

#endif  // MODEL_INFERENCE_ENGINE_H
