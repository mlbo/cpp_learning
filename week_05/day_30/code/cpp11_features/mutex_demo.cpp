/**
 * mutex演示
 */

#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

void mutexDemo() {
    std::cout << "=== mutex互斥锁演示 ===" << std::endl;
    
    std::mutex mtx;
    int sharedCounter = 0;
    
    // 不使用锁
    auto unsafeIncrement = [&]() {
        for (int i = 0; i < 10000; ++i) {
            sharedCounter++;  // 数据竞争！
        }
    };
    
    // 使用锁
    auto safeIncrement = [&]() {
        for (int i = 0; i < 10000; ++i) {
            std::lock_guard<std::mutex> lock(mtx);
            sharedCounter++;
        }
    };
    
    std::cout << "\n--- 不使用锁 ---" << std::endl;
    sharedCounter = 0;
    std::thread t1(unsafeIncrement);
    std::thread t2(unsafeIncrement);
    t1.join();
    t2.join();
    std::cout << "预期: 20000, 实际: " << sharedCounter << std::endl;
    
    std::cout << "\n--- 使用lock_guard ---" << std::endl;
    sharedCounter = 0;
    std::thread t3(safeIncrement);
    std::thread t4(safeIncrement);
    t3.join();
    t4.join();
    std::cout << "预期: 20000, 实际: " << sharedCounter << std::endl;
    
    std::cout << "\n--- unique_lock演示 ---" << std::endl;
    {
        std::unique_lock<std::mutex> ul(mtx);
        std::cout << "锁已获取" << std::endl;
        // 可以提前解锁
        ul.unlock();
        std::cout << "锁已释放" << std::endl;
    }
}

int main() {
    mutexDemo();
    return 0;
}
