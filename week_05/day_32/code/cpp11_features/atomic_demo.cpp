#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

#include "joining_thread_group.h"

int main() {
    int failures = 0;
    const auto expect = [&failures](bool condition, const char* message) {
        if (!condition) {
            std::cerr << "FAIL: " << message << '\n';
            ++failures;
        }
    };

    std::atomic<int> counter{0};
    week5::JoiningThreadGroup workers;
    for (int worker = 0; worker < 8; ++worker) {
        static_cast<void>(worker);
        workers.start([&counter] {
            for (int iteration = 0; iteration < 1000; ++iteration) {
                counter.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }
    workers.join_all();
    expect(counter.load(std::memory_order_relaxed) == 8000,
           "relaxed fetch_add 应保证计数操作本身不丢失");

    int payload = 0;
    int observed = 0;
    std::atomic<bool> ready{false};
    week5::JoiningThreadGroup publication;
    publication.start([&payload, &observed, &ready] {
        while (!ready.load(std::memory_order_acquire)) {
            std::this_thread::yield();
        }
        observed = payload;
    });
    publication.start([&payload, &ready] {
        payload = 42;
        ready.store(true, std::memory_order_release);
    });
    publication.join_all();
    expect(observed == 42, "release/acquire 应发布此前写入的普通数据");

    std::atomic<int> state{1};
    int expected = 1;
    expect(state.compare_exchange_strong(expected, 2), "CAS 应在 expected 匹配时更新状态");
    expected = 1;
    expect(!state.compare_exchange_strong(expected, 3) && expected == 2,
           "CAS 失败时应把实际值写回 expected");

    std::cout << "atomic<int> lock-free on this platform: " << std::boolalpha
              << counter.is_lock_free() << '\n';
    return failures == 0 ? 0 : 1;
}
