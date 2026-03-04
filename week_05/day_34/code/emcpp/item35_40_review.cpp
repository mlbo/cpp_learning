/**
 * EMC++ Item 35-40 复习代码
 */

#include <iostream>
#include <thread>
#include <future>
#include <atomic>

void item35_40Review() {
    std::cout << "=== EMC++ Item 35-40 复习 ===" << std::endl;
    
    // Item 35: 优先使用async
    std::cout << "\nItem 35: 基于任务优先于基于线程" << std::endl;
    auto f = std::async([]() { return 42; });
    std::cout << "  async结果: " << f.get() << std::endl;
    
    // Item 36: 指定launch::async
    std::cout << "\nItem 36: 需要异步时指定launch::async" << std::endl;
    auto f2 = std::async(std::launch::async, []() {
        return 100;
    });
    std::cout << "  强制异步结果: " << f2.get() << std::endl;
    
    // Item 37: thread不可join
    std::cout << "\nItem 37: 确保thread在所有路径上不可join" << std::endl;
    std::cout << "  使用RAII包装器或C++20 jthread" << std::endl;
    
    // Item 38: future析构行为
    std::cout << "\nItem 38: future析构会等待共享状态" << std::endl;
    
    // Item 39: void future
    std::cout << "\nItem 39: void future用于一次性事件通信" << std::endl;
    std::promise<void> prom;
    auto fut = prom.get_future();
    prom.set_value();  // 发送通知
    fut.wait();  // 等待通知
    std::cout << "  一次性事件通信完成" << std::endl;
    
    // Item 40: atomic vs volatile
    std::cout << "\nItem 40: atomic用于并发，volatile用于特殊内存" << std::endl;
    std::atomic<int> atomicVal(0);
    atomicVal++;  // 线程安全
    std::cout << "  atomic递增: " << atomicVal.load() << std::endl;
}

int main() {
    item35_40Review();
    return 0;
}
