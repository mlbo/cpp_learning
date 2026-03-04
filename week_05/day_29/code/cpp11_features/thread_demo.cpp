/**
 * std::thread 基础演示
 * 展示线程创建、管理和同步的基本用法
 */

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <functional>

std::mutex printMutex;

// 线程安全的打印函数
void safePrint(const std::string& message) {
    std::lock_guard<std::mutex> lock(printMutex);
    std::cout << message << std::endl;
}

// 方式1：使用函数指针
void workerFunction(int id, int iterations) {
    for (int i = 0; i < iterations; ++i) {
        safePrint("线程 " + std::to_string(id) + ": 迭代 " + std::to_string(i));
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

// 方式2：函数对象（Functor）
class Task {
public:
    explicit Task(int id) : id_(id) {}
    
    void operator()() const {
        safePrint("Task " + std::to_string(id_) + " 开始执行");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        safePrint("Task " + std::to_string(id_) + " 执行完成");
    }
    
private:
    int id_;
};

// RAII 线程守卫
class ThreadGuard {
public:
    explicit ThreadGuard(std::thread& t) : thread_(t) {}
    
    ~ThreadGuard() {
        if (thread_.joinable()) {
            thread_.join();
        }
    }
    
    // 禁止拷贝
    ThreadGuard(const ThreadGuard&) = delete;
    ThreadGuard& operator=(const ThreadGuard&) = delete;
    
private:
    std::thread& thread_;
};

void threadDemo() {
    std::cout << "=== std::thread 基础演示 ===" << std::endl;
    
    // 1. 使用函数指针
    std::cout << "\n--- 1. 函数指针方式 ---" << std::endl;
    std::thread t1(workerFunction, 1, 3);
    t1.join();
    
    // 2. 使用Lambda表达式
    std::cout << "\n--- 2. Lambda表达式方式 ---" << std::endl;
    int value = 42;
    std::thread t2([value]() {
        safePrint("Lambda线程: value = " + std::to_string(value));
    });
    t2.join();
    
    // 3. 使用函数对象
    std::cout << "\n--- 3. 函数对象方式 ---" << std::endl;
    std::thread t3(Task(3));
    t3.join();
    
    // 4. 多线程并行
    std::cout << "\n--- 4. 多线程并行 ---" << std::endl;
    std::vector<std::thread> threads;
    for (int i = 0; i < 3; ++i) {
        threads.emplace_back([i]() {
            safePrint("并行线程 " + std::to_string(i) + " 执行中");
        });
    }
    for (auto& t : threads) {
        t.join();
    }
    
    // 5. RAII 管理
    std::cout << "\n--- 5. RAII 线程管理 ---" << std::endl;
    {
        std::thread t([]() {
            safePrint("RAII管理的线程运行中...");
        });
        ThreadGuard guard(t);  // 离开作用域自动join
    }
    safePrint("RAII线程已自动join");
    
    // 6. 线程属性
    std::cout << "\n--- 6. 线程属性 ---" << std::endl;
    std::cout << "硬件并发数: " << std::thread::hardware_concurrency() << std::endl;
    
    std::thread tAttr([]() {
        safePrint("获取线程ID...");
    });
    std::cout << "线程ID: " << tAttr.get_id() << std::endl;
    tAttr.join();
    
    std::cout << "\n=== 演示完成 ===" << std::endl;
}

int main() {
    threadDemo();
    return 0;
}
