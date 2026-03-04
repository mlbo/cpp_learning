/**
 * atomic原子操作演示
 */

#include <iostream>
#include <atomic>
#include <thread>
#include <vector>

void atomicDemo() {
    std::cout << "=== std::atomic 原子操作演示 ===" << std::endl;
    
    // 1. 基本原子操作
    std::atomic<int> counter(0);
    
    std::cout << "\n--- 1. 多线程原子递增 ---" << std::endl;
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([&counter]() {
            for (int j = 0; j < 1000; ++j) {
                counter.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }
    
    for (auto& t : threads) t.join();
    std::cout << "预期结果: 10000, 实际结果: " << counter.load() << std::endl;
    
    // 2. CAS操作
    std::cout << "\n--- 2. CAS操作 ---" << std::endl;
    int expected = 10000;
    bool success = counter.compare_exchange_strong(expected, 0);
    std::cout << "CAS结果: " << (success ? "成功" : "失败") << std::endl;
    std::cout << "counter重置为: " << counter.load() << std::endl;
    
    // 3. 内存序
    std::cout << "\n--- 3. 内存序说明 ---" << std::endl;
    std::cout << "memory_order_relaxed: 只保证原子性" << std::endl;
    std::cout << "memory_order_acquire: 获取语义" << std::endl;
    std::cout << "memory_order_release: 释放语义" << std::endl;
    std::cout << "memory_order_seq_cst: 默认，最强约束" << std::endl;
}

int main() {
    atomicDemo();
    return 0;
}
