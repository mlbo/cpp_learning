/**
 * EMC++ Item 38: 关注未来析构行为
 * 
 * 不同并发句柄的析构行为不同：
 * - std::thread: 如果joinable，会调用std::terminate()
 * - std::future: 会阻塞等待共享状态
 */

#include <iostream>
#include <thread>
#include <future>
#include <chrono>

void item38Demo() {
    std::cout << "=== Item 38: 线程句柄析构行为 ===" << std::endl;
    
    // std::thread 析构行为
    std::cout << "\n--- std::thread 析构 ---" << std::endl;
    std::cout << "如果thread在joinable状态下析构，会调用std::terminate()" << std::endl;
    std::cout << "必须确保thread被join()或detach()" << std::endl;
    
    {
        std::thread t([]() {
            std::cout << "线程执行中..." << std::endl;
        });
        t.join();  // 必须调用，否则析构时程序崩溃
        std::cout << "线程已join，安全析构" << std::endl;
    }
    
    // std::future 析构行为
    std::cout << "\n--- std::future 析构 ---" << std::endl;
    std::cout << "future析构时会等待共享状态就绪" << std::endl;
    
    {
        auto future = std::async(std::launch::async, []() {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            return 42;
        });
        // future析构时会阻塞等待异步任务完成
        std::cout << "future即将析构..." << std::endl;
    }
    std::cout << "future已析构" << std::endl;
    
    // deferred策略的行为
    std::cout << "\n--- deferred策略 ---" << std::endl;
    {
        auto future = std::async(std::launch::deferred, []() {
            std::cout << "deferred任务执行" << std::endl;
            return 100;
        });
        // deferred任务在析构时同步执行
    }
    std::cout << "deferred future已析构" << std::endl;
    
    std::cout << "\n总结：" << std::endl;
    std::cout << "- thread析构：joinable则terminate" << std::endl;
    std::cout << "- future析构：阻塞等待或同步执行" << std::endl;
    std::cout << "- 推荐使用async，避免手动管理thread" << std::endl;
}

int main() {
    item38Demo();
    return 0;
}
