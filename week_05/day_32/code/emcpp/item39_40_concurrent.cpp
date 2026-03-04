/**
 * EMC++ Item 39-40: volatile vs atomic, void future
 */

#include <iostream>
#include <atomic>
#include <future>
#include <thread>

void item39Demo() {
    std::cout << "=== Item 39: atomic用于并发，volatile用于特殊内存 ===" << std::endl;
    
    // 正确：atomic用于并发
    std::atomic<int> atomicCounter(0);
    atomicCounter++;  // 线程安全
    std::cout << "atomic递增: " << atomicCounter.load() << std::endl;
    
    // volatile用于特殊内存（如硬件寄存器）
    // volatile int* hwReg = (volatile int*)0xFFFF0000;
    // *hwReg = 0x01;  // 每次都要写入
    std::cout << "volatile用于特殊内存访问，不保证线程安全" << std::endl;
    
    // 对比
    std::cout << "\n| 特性       | atomic       | volatile     |" << std::endl;
    std::cout << "|-----------|-------------|-------------|" << std::endl;
    std::cout << "| 原子性     | 保证        | 不保证      |" << std::endl;
    std::cout << "| 可见性     | 保证        | 不保证      |" << std::endl;
    std::cout << "| 线程安全   | 是          | 否          |" << std::endl;
    std::cout << "| 用途       | 并发编程    | 特殊内存    |" << std::endl;
}

void item40Demo() {
    std::cout << "\n=== Item 40: void future用于一次性事件通信 ===" << std::endl;
    
    // 使用 promise/future 进行一次性事件通知
    std::promise<void> readyPromise;
    std::future<void> readyFuture = readyPromise.get_future();
    
    std::thread waiter([&readyFuture]() {
        std::cout << "  等待者: 等待事件..." << std::endl;
        readyFuture.wait();
        std::cout << "  等待者: 事件已发生！" << std::endl;
    });
    
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "  通知者: 发送事件通知" << std::endl;
    readyPromise.set_value();
    
    waiter.join();
    
    std::cout << "\n优势：比条件变量更简洁，不易出错" << std::endl;
}

int main() {
    item39Demo();
    item40Demo();
    return 0;
}
