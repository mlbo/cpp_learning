/**
 * 并发综合演示
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <future>
#include <queue>

void concurrencyDemo() {
    std::cout << "=== 并发综合演示 ===" << std::endl;
    
    // 1. mutex + condition_variable
    std::cout << "\n--- 1. mutex + cv ---" << std::endl;
    std::mutex mtx;
    std::condition_variable cv;
    std::queue<int> dataQueue;
    bool finished = false;
    
    std::thread producer([&]() {
        for (int i = 0; i < 5; ++i) {
            {
                std::lock_guard<std::mutex> lock(mtx);
                dataQueue.push(i);
                std::cout << "生产: " << i << std::endl;
            }
            cv.notify_one();
        }
        {
            std::lock_guard<std::mutex> lock(mtx);
            finished = true;
        }
        cv.notify_one();
    });
    
    std::thread consumer([&]() {
        while (true) {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [&]{ return !dataQueue.empty() || finished; });
            
            while (!dataQueue.empty()) {
                int data = dataQueue.front();
                dataQueue.pop();
                std::cout << "消费: " << data << std::endl;
            }
            
            if (finished) break;
        }
    });
    
    producer.join();
    consumer.join();
    
    // 2. atomic
    std::cout << "\n--- 2. atomic ---" << std::endl;
    std::atomic<int> counter{0};
    std::vector<std::thread> threads;
    
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back([&counter]() {
            for (int j = 0; j < 1000; ++j) {
                counter.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }
    
    for (auto& t : threads) t.join();
    std::cout << "atomic计数器: " << counter.load() << std::endl;
    
    // 3. async
    std::cout << "\n--- 3. async ---" << std::endl;
    auto future = std::async(std::launch::async, []() {
        return 42;
    });
    std::cout << "async结果: " << future.get() << std::endl;
}

int main() {
    concurrencyDemo();
    return 0;
}
