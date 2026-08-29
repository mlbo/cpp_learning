#include <atomic>
#include <future>
#include <iostream>
#include <thread>
#include <vector>

#include "joining_thread_group.h"

int main() {
    std::atomic<int> counter{0};
    week5::JoiningThreadGroup workers;
    for (int worker = 0; worker < 4; ++worker) {
        static_cast<void>(worker);
        workers.start([&counter] {
            for (int iteration = 0; iteration < 1'000; ++iteration) {
                counter.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }
    workers.join_all();

    auto square = std::async(std::launch::async, [] { return 12 * 12; });

    std::promise<void> start_promise;
    std::future<void> start_future = start_promise.get_future();
    std::atomic<bool> event_observed{false};
    std::thread waiter{
        [future = std::move(start_future), &event_observed]() mutable {
            future.wait();
            event_observed.store(true, std::memory_order_release);
        }};
    start_promise.set_value();
    waiter.join();

    const bool passed = counter.load(std::memory_order_relaxed) == 4'000 &&
                        square.get() == 144 &&
                        event_observed.load(std::memory_order_acquire);
    std::cout << "atomic 计数: " << counter.load(std::memory_order_relaxed) << '\n';
    std::cout << "一次性事件已观察: " << std::boolalpha
              << event_observed.load(std::memory_order_acquire) << '\n';
    return passed ? 0 : 1;
}
