/**
 * 线程池演示
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <functional>
#include <vector>
#include <atomic>

class SimpleThreadPool {
public:
    SimpleThreadPool(size_t numThreads) : stop_(false) {
        for (size_t i = 0; i < numThreads; ++i) {
            workers_.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(mtx_);
                        cv_.wait(lock, [this] { return stop_ || !tasks_.empty(); });
                        if (stop_ && tasks_.empty()) return;
                        task = std::move(tasks_.front());
                        tasks_.pop();
                    }
                    task();
                }
            });
        }
    }
    
    ~SimpleThreadPool() {
        {
            std::lock_guard<std::mutex> lock(mtx_);
            stop_ = true;
        }
        cv_.notify_all();
        for (auto& w : workers_) w.join();
    }
    
    void enqueue(std::function<void()> task) {
        {
            std::lock_guard<std::mutex> lock(mtx_);
            tasks_.push(std::move(task));
        }
        cv_.notify_one();
    }

private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex mtx_;
    std::condition_variable cv_;
    bool stop_;
};

void threadPoolDemo() {
    std::cout << "=== 线程池演示 ===" << std::endl;
    
    SimpleThreadPool pool(4);
    std::atomic<int> counter{0};
    
    std::cout << "提交10个任务..." << std::endl;
    for (int i = 0; i < 10; ++i) {
        pool.enqueue([&counter, i]() {
            std::cout << "  任务 " << i << " 在线程 " 
                      << std::this_thread::get_id() << " 执行" << std::endl;
            counter++;
        });
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::cout << "完成任务数: " << counter.load() << std::endl;
}

int main() {
    threadPoolDemo();
    return 0;
}
