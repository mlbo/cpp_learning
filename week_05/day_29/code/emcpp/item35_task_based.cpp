/**
 * EMC++ Item 35: 优先使用基于任务的编程而非基于线程
 * 
 * 本示例对比两种方式获取异步计算结果的差异
 */

#include <iostream>
#include <thread>
#include <future>
#include <vector>
#include <chrono>
#include <stdexcept>

// 模拟一个耗时的计算任务
int computeValue(int input) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    if (input < 0) {
        throw std::invalid_argument("输入必须非负");
    }
    
    return input * input;  // 返回平方值
}

// ========== 基于线程的方式 ==========
void threadBasedApproach() {
    std::cout << "\n--- 基于线程的方式 ---" << std::endl;
    
    int result = 0;
    bool exceptionOccurred = false;
    std::string exceptionMessage;
    
    // 必须使用引用或指针来获取结果
    std::thread t([&result, &exceptionOccurred, &exceptionMessage]() {
        try {
            result = computeValue(10);
        } catch (const std::exception& e) {
            exceptionOccurred = true;
            exceptionMessage = e.what();
        }
    });
    
    t.join();  // 必须手动 join
    
    if (exceptionOccurred) {
        std::cout << "异常: " << exceptionMessage << std::endl;
    } else {
        std::cout << "结果: " << result << std::endl;
    }
    
    std::cout << "问题: 需要额外变量、手动join、异常处理复杂" << std::endl;
}

// ========== 基于任务的方式 ==========
void taskBasedApproach() {
    std::cout << "\n--- 基于任务的方式 ---" << std::endl;
    
    // 使用 std::async 启动异步任务
    std::future<int> future = std::async(std::launch::async, computeValue, 10);
    
    std::cout << "等待计算结果..." << std::endl;
    
    try {
        int result = future.get();  // 直接获取结果，异常会自动重新抛出
        std::cout << "结果: " << result << std::endl;
    } catch (const std::exception& e) {
        std::cout << "异常: " << e.what() << std::endl;
    }
    
    std::cout << "优势: 直接获取返回值、无需手动管理线程、异常自动传递" << std::endl;
}

// 演示异常处理的差异
void demonstrateExceptionHandling() {
    std::cout << "\n--- 异常处理对比 ---" << std::endl;
    
    // 基于任务：异常处理简洁
    auto future = std::async(std::launch::async, computeValue, -5);
    
    try {
        future.get();  // 异常在这里重新抛出
    } catch (const std::exception& e) {
        std::cout << "捕获到异常: " << e.what() << std::endl;
    }
}

// 演示多个任务的并行执行
void multipleTasksDemo() {
    std::cout << "\n--- 多任务并行执行 ---" << std::endl;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    // 创建多个异步任务
    std::vector<std::future<int>> futures;
    for (int i = 0; i < 5; ++i) {
        futures.push_back(std::async(std::launch::async, computeValue, i));
    }
    
    // 收集结果
    std::cout << "计算结果: ";
    for (auto& f : futures) {
        std::cout << f.get() << " ";
    }
    std::cout << std::endl;
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "总耗时: " << duration.count() << "ms (并行执行)" << std::endl;
    
    // 对比顺序执行
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 5; ++i) {
        computeValue(i);
    }
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "顺序执行耗时: " << duration.count() << "ms" << std::endl;
}

// 总结对比
void printComparison() {
    std::cout << "\n=== 基于线程 vs 基于任务 对比 ===" << std::endl;
    std::cout << "| 特性         | 基于线程           | 基于任务           |" << std::endl;
    std::cout << "|-------------|-------------------|-------------------|" << std::endl;
    std::cout << "| 获取返回值   | 需要共享变量       | future.get()直接获取|" << std::endl;
    std::cout << "| 异常处理     | 复杂，需手动传递   | 自动传递到调用方   |" << std::endl;
    std::cout << "| 线程管理     | 需手动join/detach | 系统自动管理       |" << std::endl;
    std::cout << "| 资源安全     | 容易泄漏          | 异常安全           |" << std::endl;
    std::cout << "| 推荐程度     | 特定场景使用       | 优先选择           |" << std::endl;
}

int main() {
    std::cout << "=== Item 35: 任务 vs 线程 ===" << std::endl;
    
    threadBasedApproach();
    taskBasedApproach();
    demonstrateExceptionHandling();
    multipleTasksDemo();
    printComparison();
    
    std::cout << "\n结论: 在大多数情况下，优先使用 std::async 和 std::future" << std::endl;
    std::cout << "它们提供了更简洁的语法、更好的异常安全性和更灵活的调度策略。" << std::endl;
    
    return 0;
}
