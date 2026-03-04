/**
 * EMC++ Item 36-37: async策略与future管理
 */

#include <iostream>
#include <future>
#include <thread>
#include <chrono>

int compute(int n) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    return n * n;
}

void item36_37Demo() {
    std::cout << "=== Item 36-37 演示 ===" << std::endl;
    
    // Item 36: 指定launch::async
    std::cout << "\nItem 36: 指定std::launch::async" << std::endl;
    
    // 默认策略可能延迟执行
    auto f1 = std::async(compute, 10);
    std::cout << "默认策略结果: " << f1.get() << std::endl;
    
    // 强制异步
    auto f2 = std::async(std::launch::async, compute, 20);
    std::cout << "async策略结果: " << f2.get() << std::endl;
    
    // Item 37: 不要忽视future
    std::cout << "\nItem 37: 不要忽视future" << std::endl;
    std::cout << "错误的写法: std::async(func); // future被丢弃" << std::endl;
    std::cout << "正确的写法: auto f = std::async(func); f.get();" << std::endl;
    
    // 正确处理future
    auto f3 = std::async(std::launch::async, compute, 30);
    // 可以做其他事情...
    std::cout << "异步计算中..." << std::endl;
    std::cout << "结果: " << f3.get() << std::endl;
}

int main() {
    item36_37Demo();
    return 0;
}
