#include <atomic>
#include <chrono>
#include <future>
#include <iostream>
#include <thread>

class JoiningThread {
public:
    explicit JoiningThread(std::thread thread) : thread_{std::move(thread)} {}
    ~JoiningThread() {
        if (thread_.joinable()) {
            thread_.join();
        }
    }

    JoiningThread(const JoiningThread&) = delete;
    JoiningThread& operator=(const JoiningThread&) = delete;

private:
    std::thread thread_;
};

int main() {
    auto task = std::async(std::launch::async, [] { return 35; });
    auto deferred = std::async(std::launch::deferred, [] { return 36; });
    const bool reports_deferred =
        deferred.wait_for(std::chrono::seconds{0}) == std::future_status::deferred;

    std::atomic<bool> joined_work_finished{false};
    {
        JoiningThread thread{std::thread{[&joined_work_finished] {
            joined_work_finished.store(true, std::memory_order_release);
        }}};
    }

    std::promise<void> event_promise;
    std::future<void> event_future = event_promise.get_future();
    event_promise.set_value();
    event_future.get();

    std::atomic<int> atomic_value{40};
    atomic_value.fetch_add(1, std::memory_order_relaxed);
    const bool passed = task.get() == 35 && reports_deferred && deferred.get() == 36 &&
                        joined_work_finished.load(std::memory_order_acquire) &&
                        atomic_value.load(std::memory_order_relaxed) == 41;

    std::cout << "Item 35-36: task 返回结果，必要异步显式指定 launch::async\n";
    std::cout << "Item 37: RAII 使 thread 在析构前不可 join\n";
    std::cout << "Item 38: deferred 析构不会自动执行任务\n";
    std::cout << "Item 39-40: void future 是一次性事件，atomic 不等于 volatile\n";
    std::cout << "atomic<int> 当前实现 lock-free: " << std::boolalpha
              << atomic_value.is_lock_free() << '\n';
    return passed ? 0 : 1;
}
