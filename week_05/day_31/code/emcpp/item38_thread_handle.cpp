#include <atomic>
#include <future>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <utility>

class JoiningThread {
public:
    explicit JoiningThread(std::thread thread) : thread_(std::move(thread)) {
        if (!thread_.joinable()) {
            throw std::invalid_argument("JoiningThread requires a joinable thread");
        }
    }

    ~JoiningThread() {
        thread_.join();
    }

    JoiningThread(const JoiningThread&) = delete;
    JoiningThread& operator=(const JoiningThread&) = delete;

private:
    std::thread thread_;
};

int main() {
    int failures = 0;
    const auto expect = [&failures](bool condition, const char* message) {
        if (!condition) {
            std::cerr << "FAIL: " << message << '\n';
            ++failures;
        }
    };

    std::atomic<int> joined_result{0};
    {
        JoiningThread worker(std::thread([&joined_result] {
            joined_result.store(42, std::memory_order_relaxed);
        }));
    }
    expect(joined_result.load(std::memory_order_relaxed) == 42,
           "RAII 线程拥有者应在离开作用域前 join");

    std::atomic<int> deferred_runs{0};
    {
        auto deferred = std::async(std::launch::deferred, [&deferred_runs] {
            deferred_runs.fetch_add(1, std::memory_order_relaxed);
        });
        static_cast<void>(deferred.valid());
    }
    expect(deferred_runs.load(std::memory_order_relaxed) == 0,
           "未 wait/get 的 deferred 任务不会因 future 析构而执行");

    std::promise<int> producer;
    {
        std::future<int> consumer = producer.get_future();
        expect(consumer.valid(), "promise 生成的 future 应持有共享状态");
    }
    producer.set_value(7);

    std::promise<void> release_promise;
    std::shared_future<void> release = release_promise.get_future().share();
    {
        std::future<int> async_result = std::async(std::launch::async, [release] {
            release.wait();
            return 9;
        });
        release_promise.set_value();
        expect(async_result.get() == 9, "async 任务结果应通过 get 观察");
    }

    std::cout << "Item 38：joinable thread、async future、promise future 与 deferred 边界测试完成。\n";
    return failures == 0 ? 0 : 1;
}
