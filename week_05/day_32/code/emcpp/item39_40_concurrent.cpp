#include <atomic>
#include <future>
#include <iostream>
#include <system_error>
#include <thread>

int main() {
    int failures = 0;
    const auto expect = [&failures](bool condition, const char* message) {
        if (!condition) {
            std::cerr << "FAIL: " << message << '\n';
            ++failures;
        }
    };

    int payload = 0;
    int observed = 0;
    std::promise<void> event_promise;
    std::future<void> event = event_promise.get_future();
    std::thread waiter([&event, &payload, &observed] {
        event.wait();
        observed = payload;
    });

    payload = 39;
    event_promise.set_value();
    waiter.join();
    expect(observed == 39, "future 等待应与 promise 满足共享状态建立同步");

    bool duplicate_rejected = false;
    try {
        event_promise.set_value();
    } catch (const std::future_error&) {
        duplicate_rejected = true;
    }
    expect(duplicate_rejected, "promise 的一次性事件只能满足一次");

    std::atomic<int> concurrent_value{0};
    concurrent_value.fetch_add(1, std::memory_order_relaxed);
    expect(concurrent_value.load(std::memory_order_relaxed) == 1,
           "Item 40 的并发对象应使用 atomic 而不是 volatile");

    std::cout << "Item 39：void future 一次性事件；Item 40：atomic 用于并发，volatile 用于特殊内存。\n";
    return failures == 0 ? 0 : 1;
}
