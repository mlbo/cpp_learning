#include <algorithm>
#include <atomic>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

#include "joining_thread_group.h"

int main() {
    const auto answer = 42;
    auto owner = std::make_shared<int>(200);
    std::weak_ptr<int> observer = owner;

    std::vector<int> values{3, 1, 4, 1, 5};
    std::sort(values.begin(), values.end(), std::greater<int>{});

    std::atomic<int> counter{0};
    week5::JoiningThreadGroup workers;
    for (int worker_index = 0; worker_index < 4; ++worker_index) {
        static_cast<void>(worker_index);
        workers.start([&counter] {
            for (int iteration = 0; iteration < 1'000; ++iteration) {
                counter.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }
    workers.join_all();

    const bool owner_visible = !observer.expired() && *observer.lock() == 200;
    owner.reset();
    const bool observer_expires = observer.expired();
    const bool passed = answer == 42 && values.front() == 5 && owner_visible &&
                        observer_expires &&
                        counter.load(std::memory_order_relaxed) == 4'000;

    std::cout << "auto 保留可读性，智能指针表达所有权，lambda 表达策略。\n";
    std::cout << "atomic 计数结果: " << counter.load(std::memory_order_relaxed) << '\n';
    return passed ? 0 : 1;
}
