/**
 * 进程线程模型演示
 */

#include <iostream>
#include <thread>
#include <vector>

void processThreadDemo() {
    std::cout << "=== 进程线程模型演示 ===" << std::endl;
    
    std::cout << "\n进程与线程对比:" << std::endl;
    std::cout << "| 特性     | 进程         | 线程         |" << std::endl;
    std::cout << "|---------|-------------|-------------|" << std::endl;
    std::cout << "| 资源分配 | 独立地址空间 | 共享地址空间 |" << std::endl;
    std::cout << "| 创建开销 | 大          | 小          |" << std::endl;
    std::cout << "| 通信方式 | IPC         | 共享内存    |" << std::endl;
    
    std::cout << "\n硬件并发数: " << std::thread::hardware_concurrency() << std::endl;
    std::cout << "当前线程ID: " << std::this_thread::get_id() << std::endl;
    
    std::vector<std::thread> threads;
    for (int i = 0; i < 3; ++i) {
        threads.emplace_back([i]() {
            std::cout << "线程 " << i << " ID: " 
                      << std::this_thread::get_id() << std::endl;
        });
    }
    for (auto& t : threads) t.join();
}

int main() {
    processThreadDemo();
    return 0;
}
