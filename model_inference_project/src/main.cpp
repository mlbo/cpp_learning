/**
 * @file main.cpp
 * @brief 模型推理示例主程序
 *
 * 演示：
 * 1. 创建神经网络模型
 * 2. 单线程推理
 * 3. 多线程批量推理
 * 4. 性能对比
 */

#include <chrono>
#include <future>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

#include "tensor.h"
#include "layers.h"
#include "inference_engine.h"

using namespace inference;

unsigned int seedForIndex(size_t index) {
    if (index > static_cast<size_t>(std::numeric_limits<unsigned int>::max())) {
        throw std::overflow_error("Demo seed index does not fit in unsigned int");
    }
    return static_cast<unsigned int>(index);
}

void requireSuccessful(const std::vector<InferenceResult>& results,
                       size_t expected_count,
                       const char* operation) {
    if (results.size() != expected_count) {
        throw std::runtime_error(std::string(operation) +
                                 " returned an unexpected result count");
    }
    for (const auto& result : results) {
        if (!result.success) {
            throw std::runtime_error(std::string(operation) + ": " +
                                     result.error_message);
        }
    }
}

/**
 * @brief 创建一个简单的MLP分类器
 *
 * 结构：Input -> Linear(784, 256) -> ReLU -> Linear(256, 128) -> ReLU -> Linear(128, 10) -> Softmax
 */
Sequential createMLPClassifier(size_t input_size = 784,
                               size_t hidden1 = 256,
                               size_t hidden2 = 128,
                               size_t num_classes = 10) {
    Sequential model;
    model.add(Linear(input_size, hidden1));
    model.add(ReLU());
    model.add(Linear(hidden1, hidden2));
    model.add(ReLU());
    model.add(Linear(hidden2, num_classes));
    model.add(Softmax());

    return model;
}

/**
 * @brief 创建一个用于回归的MLP
 */
Sequential createRegressionModel(size_t input_size = 10,
                                  size_t hidden = 64,
                                  size_t output_size = 1) {
    Sequential model;
    model.add(Linear(input_size, hidden));
    model.add(ReLU());
    model.add(Linear(hidden, hidden));
    model.add(ReLU());
    model.add(Linear(hidden, output_size));

    return model;
}

/**
 * @brief 演示基本的张量操作
 */
void demoTensorOperations() {
    std::cout << "\n========== Tensor Operations Demo ==========\n\n";

    // 创建张量
    auto t1 = Tensor::random({3, 4}, -1.0f, 1.0f);
    t1.print("Random Tensor t1");

    auto t2 = Tensor::ones({3, 4});
    t2.print("Ones Tensor t2");

    // 基本运算
    auto t3 = t1 + t2;
    t3.print("t1 + t2");

    auto t4 = t1 * 2.0f;
    t4.print("t1 * 2.0");

    // 矩阵乘法
    auto t5 = Tensor::random({4, 5}, -1.0f, 1.0f);
    auto t6 = t1.matmul(t5);
    t6.print("t1 @ t5");

    // 激活函数
    auto t7 = t1.relu();
    t7.print("ReLU(t1)");

    auto t8 = t1.sigmoid();
    t8.print("Sigmoid(t1)");

    // Softmax
    auto logits = Tensor::random({5}, -2.0f, 2.0f);
    logits.print("Logits");

    auto probs = logits.softmax();
    probs.print("Softmax Probabilities");
    std::cout << "Sum of probabilities: " << probs.sum() << "\n";

    std::cout << "\n";
}

/**
 * @brief 演示单层操作
 */
void demoLayers() {
    std::cout << "\n========== Layers Demo ==========\n\n";

    // Linear层
    Linear linear(4, 3);
    std::cout << "Linear layer: " << linear.inFeatures() << " -> " << linear.outFeatures() << "\n";
    std::cout << "Parameters: " << linear.numParameters() << "\n";

    auto input = Tensor::random({4}, -1.0f, 1.0f);
    input.print("Input");

    linear.weight().print("Weight");
    linear.bias().print("Bias");

    auto output = linear.forward(input);
    output.print("Linear Output");

    // ReLU层
    ReLU relu;
    auto relu_output = relu.forward(input);
    relu_output.print("ReLU Output");

    // Softmax层
    Softmax softmax;
    auto softmax_output = softmax.forward(output);
    softmax_output.print("Softmax Output");
    std::cout << "Sum: " << softmax_output.sum() << "\n";

    std::cout << "\n";
}

/**
 * @brief 演示Sequential模型
 */
void demoSequentialModel() {
    std::cout << "\n========== Sequential Model Demo ==========\n\n";

    // 创建MLP模型
    auto model = createMLPClassifier(784, 256, 128, 10);
    model.printStructure();

    // 创建模拟输入（类似于MNIST图像的展平向量）
    auto input = Tensor::random({784}, 0.0f, 1.0f);

    // 前向传播
    auto start = std::chrono::steady_clock::now();
    auto output = model.forward(input);
    auto end = std::chrono::steady_clock::now();

    output.print("Model Output (class probabilities)");

    // 找到预测类别
    size_t predicted_class = output.argmax();
    std::cout << "Predicted class: " << predicted_class
              << ", confidence: " << output[predicted_class] << "\n";

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Inference time: " << duration.count() << " us\n";

    std::cout << "\n";
}

/**
 * @brief 演示单线程批量推理
 */
void demoSingleThreadBatch() {
    std::cout << "\n========== Single-Thread Batch Inference Demo ==========\n\n";

    auto model = createMLPClassifier(784, 256, 128, 10);
    InferenceEngine engine(&model);

    // 生成批量测试数据
    size_t batch_size = 100;
    std::vector<Tensor> inputs;
    inputs.reserve(batch_size);

    for (size_t i = 0; i < batch_size; ++i) {
        inputs.push_back(Tensor::random({784}, 0.0f, 1.0f, seedForIndex(i)));
    }

    // 批量推理
    auto outputs = engine.inferBatch(inputs);

    // 统计结果
    std::vector<size_t> class_counts(10, 0);
    for (const auto& output : outputs) {
        class_counts[output.argmax()]++;
    }

    std::cout << "Classification distribution:\n";
    for (size_t i = 0; i < 10; ++i) {
        std::cout << "  Class " << i << ": " << class_counts[i] << "\n";
    }

    std::cout << "\n";
}

/**
 * @brief 演示多线程并行推理
 */
void demoParallelInference() {
    std::cout << "\n========== Parallel Inference Demo ==========\n\n";

    auto model = createMLPClassifier(784, 256, 128, 10);

    // 测试不同线程数
    std::vector<size_t> thread_counts = {1, 2, 4, 8};
    size_t batch_size = 200;

    // 准备测试数据
    std::vector<Tensor> inputs;
    inputs.reserve(batch_size);
    for (size_t i = 0; i < batch_size; ++i) {
        inputs.push_back(Tensor::random({784}, 0.0f, 1.0f, seedForIndex(i)));
    }

    std::cout << "Testing with batch size: " << batch_size << "\n\n";

    for (size_t num_threads : thread_counts) {
        std::cout << "--- Using " << num_threads << " thread(s) ---\n";

        ParallelInferenceEngine engine(&model, num_threads);

        auto start = std::chrono::steady_clock::now();
        auto results = engine.inferBatchParallel(inputs);
        auto end = std::chrono::steady_clock::now();

        auto total_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        requireSuccessful(results, batch_size, "parallel inference");

        std::cout << "Successful inferences: " << results.size() << "/" << batch_size << "\n";
        const double elapsed_seconds = total_duration.count() > 0
            ? static_cast<double>(total_duration.count()) / 1'000'000.0
            : 0.0;
        const double throughput = elapsed_seconds > 0.0
            ? static_cast<double>(batch_size) / elapsed_seconds
            : 0.0;
        std::cout << "Total time: " << total_duration.count() << " us\n";
        std::cout << "Throughput: " << std::fixed << std::setprecision(1)
                  << throughput << " samples/sec\n\n";

        engine.stop();  // 非阻塞：只关闭新入口
        engine.join();  // 阻塞：排空并回收 worker
    }
}

/**
 * @brief 演示异步推理
 */
void demoAsyncInference() {
    std::cout << "\n========== Async Inference Demo ==========\n\n";

    auto model = createMLPClassifier(784, 256, 128, 10);
    ParallelInferenceEngine engine(&model, 4);

    // 提交异步任务
    std::cout << "Submitting async inference requests...\n";

    std::vector<std::future<InferenceResult>> futures;
    for (size_t i = 0; i < 10; ++i) {
        auto input = Tensor::random({784}, 0.0f, 1.0f, seedForIndex(i));
        futures.push_back(engine.inferAsync(input));
        std::cout << "  Submitted request " << i << "\n";
    }

    std::cout << "\nRequests are accepted; future::get establishes completion.\n";

    // 收集结果
    std::cout << "\nCollecting results:\n";
    for (size_t i = 0; i < futures.size(); ++i) {
        auto result = futures[i].get();
        if (result.success) {
            std::cout << "  Request " << i << ": predicted class "
                      << result.output.argmax()
                      << ", latency: " << result.latency.count() << " us\n";
        } else {
            throw std::runtime_error("async inference failed: " + result.error_message);
        }
    }

    engine.stop();
    engine.join();
    std::cout << "\n";
}

/**
 * @brief 性能基准测试
 */
void benchmark() {
    std::cout << "\n========== Performance Benchmark ==========\n\n";

    auto model = createMLPClassifier(784, 512, 256, 10);
    model.printStructure();

    std::cout << "\n";

    // 测试不同批量大小
    std::vector<size_t> batch_sizes = {1, 10, 50, 100, 200, 500};
    size_t num_threads = 4;

    ParallelInferenceEngine engine(&model, num_threads);

    std::cout << "Benchmark with " << num_threads << " threads:\n\n";

    for (size_t batch_size : batch_sizes) {
        // 准备数据
        std::vector<Tensor> inputs;
        inputs.reserve(batch_size);
        for (size_t i = 0; i < batch_size; ++i) {
            inputs.push_back(Tensor::random({784}, 0.0f, 1.0f, seedForIndex(i)));
        }

        // 预热
        const auto warmup_results = engine.inferBatchParallel(inputs);
        requireSuccessful(warmup_results, batch_size, "benchmark warmup");

        // 正式测试（多次取平均）
        const int runs = 5;
        std::chrono::microseconds total_time{0};

        for (int r = 0; r < runs; ++r) {
            auto start = std::chrono::steady_clock::now();
            auto results = engine.inferBatchParallel(inputs);
            auto end = std::chrono::steady_clock::now();
            requireSuccessful(results, batch_size, "benchmark inference");
            total_time +=
                std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        }

        const double avg_time_ms =
            static_cast<double>(total_time.count()) /
            static_cast<double>(runs) / 1000.0;
        double throughput = avg_time_ms > 0.0
            ? static_cast<double>(batch_size) / (avg_time_ms / 1000.0)
            : 0.0;

        std::cout << "Batch size: " << std::setw(4) << batch_size
                  << " | Time: " << std::setw(8) << std::fixed << std::setprecision(2)
                  << avg_time_ms << " ms"
                  << " | Throughput: " << std::setw(8) << std::setprecision(0)
                  << throughput << " samples/sec\n";
    }

    engine.stop();
    engine.join();
    std::cout << "\n";
}

/**
 * @brief 主函数
 */
int main() {
    try {
        std::cout << R"(
╔═══════════════════════════════════════════════════════════════╗
║             C++ Model Inference Demo Project                  ║
║                                                               ║
║  Features:                                                    ║
║  - Custom Tensor library with math operations                 ║
║  - Neural network layers (Linear, ReLU, Softmax, etc.)        ║
║  - Sequential model container                                 ║
║  - Multi-threaded inference engine                            ║
║  - Async inference with futures                               ║
║  - Performance benchmarking                                   ║
╚═══════════════════════════════════════════════════════════════╝
)" << "\n";

        // 运行各项演示
        demoTensorOperations();
        demoLayers();
        demoSequentialModel();
        demoSingleThreadBatch();
        demoParallelInference();
        demoAsyncInference();
        benchmark();

        std::cout << "All demos completed successfully!\n";

        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Demo failed: " << error.what() << '\n';
        return 1;
    }
}
