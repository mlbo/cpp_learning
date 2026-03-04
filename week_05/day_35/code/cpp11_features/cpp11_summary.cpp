/**
 * C++11特性综合演示
 */

#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
#include <thread>
#include <mutex>
#include <atomic>

void cpp11Summary() {
    std::cout << "=== C++11特性综合演示 ===" << std::endl;
    
    // 1. auto
    std::cout << "\n--- auto类型推导 ---" << std::endl;
    auto i = 42;              // int
    auto d = 3.14;            // double
    auto s = "hello";         // const char*
    std::cout << "auto i = " << i << ", d = " << d << std::endl;
    
    // 2. 智能指针
    std::cout << "\n--- 智能指针 ---" << std::endl;
    auto uptr = std::make_unique<int>(100);
    auto sptr = std::make_shared<int>(200);
    std::cout << "unique_ptr: " << *uptr << std::endl;
    std::cout << "shared_ptr: " << *sptr << ", use_count: " << sptr.use_count() << std::endl;
    
    // 3. Lambda
    std::cout << "\n--- Lambda表达式 ---" << std::endl;
    std::vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6};
    std::sort(v.begin(), v.end(), [](int a, int b) { return a > b; });
    std::cout << "降序排序: ";
    for (int x : v) std::cout << x << " ";
    std::cout << std::endl;
    
    // 4. 并发
    std::cout << "\n--- 并发编程 ---" << std::endl;
    std::atomic<int> counter{0};
    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i) {
        threads.emplace_back([&counter]() {
            for (int j = 0; j < 1000; ++j) counter++;
        });
    }
    for (auto& t : threads) t.join();
    std::cout << "atomic计数器: " << counter.load() << std::endl;
}

int main() {
    cpp11Summary();
    return 0;
}
