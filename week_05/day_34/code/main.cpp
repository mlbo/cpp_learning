/**
 * Day 34: 并发编程综合 - 主程序
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <future>
#include <vector>

void processThreadDemo();
void concurrencyDemo();
void emcppReviewDemo();

int main() {
    std::cout << "=== Day 34: 并发编程综合 ===" << std::endl;
    
    // 1. 进程线程模型
    std::cout << "\n--- 1. 进程线程模型 ---" << std::endl;
    processThreadDemo();
    
    // 2. 并发综合演示
    std::cout << "\n--- 2. 并发综合演示 ---" << std::endl;
    concurrencyDemo();
    
    // 3. EMC++ Item 35-40复习
    std::cout << "\n--- 3. EMC++ Item 35-40 复习 ---" << std::endl;
    emcppReviewDemo();
    
    std::cout << "\n=== Day 34 学习完成 ===" << std::endl;
    return 0;
}

void processThreadDemo() {
    std::cout << "进程: 操作系统资源分配的基本单位" << std::endl;
    std::cout << "线程: CPU调度的基本单位" << std::endl;
    std::cout << "硬件并发数: " << std::thread::hardware_concurrency() << std::endl;
    
    // 线程安全
    std::cout << "\n线程安全要点:" << std::endl;
    std::cout << "  - 使用mutex保护共享资源" << std::endl;
    std::cout << "  - 使用atomic进行轻量同步" << std::endl;
    std::cout << "  - 避免死锁（按顺序加锁）" << std::endl;
}

void concurrencyDemo() {
    std::mutex mtx;
    std::condition_variable cv;
    std::atomic<int> counter{0};
    bool ready = false;
    
    // 生产者-消费者示例
    std::thread producer([&]() {
        for (int i = 0; i < 5; ++i) {
            counter++;
        }
        {
            std::lock_guard<std::mutex> lock(mtx);
            ready = true;
        }
        cv.notify_one();
    });
    
    std::thread consumer([&]() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&]{ return ready; });
        std::cout << "消费者收到通知，counter = " << counter.load() << std::endl;
    });
    
    producer.join();
    consumer.join();
}

void emcppReviewDemo() {
    std::cout << "Item 35: 优先使用async而非thread" << std::endl;
    std::cout << "Item 36: 需要异步时指定launch::async" << std::endl;
    std::cout << "Item 37: 确保thread在所有路径上不可join" << std::endl;
    std::cout << "Item 38: 关注future析构行为" << std::endl;
    std::cout << "Item 39: void future用于一次性事件" << std::endl;
    std::cout << "Item 40: atomic用于并发，volatile用于特殊内存" << std::endl;
}
