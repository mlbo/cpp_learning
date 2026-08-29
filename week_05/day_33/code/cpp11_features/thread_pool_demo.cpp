/**
 * 可测试的 C++17 线程池：提交返回 future，shutdown 采用 drain 策略，停止后拒绝提交。
 */

#include <atomic>
#include <future>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "thread_pool.h"

int main() {
    ThreadPool pool(4);
    std::atomic<int> executions{0};
    std::vector<std::future<int>> results;
    results.reserve(20U);

    for (int value = 1; value <= 20; ++value) {
        results.push_back(pool.submit([value, &executions] {
            executions.fetch_add(1, std::memory_order_relaxed);
            return value * value;
        }));
    }

    std::future<int> exceptional = pool.submit([]() -> int {
        throw std::runtime_error("task failure");
    });

    // 不使用 sleep：shutdown 返回本身就是“队列已排空且工作线程已 join”的同步点。
    pool.shutdown();

    for (std::size_t index = 0; index < results.size(); ++index) {
        const int value = static_cast<int>(index) + 1;
        if (results[index].get() != value * value) {
            std::cerr << "future 结果错误\n";
            return 1;
        }
    }
    if (executions.load(std::memory_order_relaxed) != 20) {
        std::cerr << "drain 未执行全部已接收任务\n";
        return 1;
    }

    bool observed_task_exception = false;
    try {
        static_cast<void>(exceptional.get());
    } catch (const std::runtime_error&) {
        observed_task_exception = true;
    }
    if (!observed_task_exception) {
        std::cerr << "任务异常没有通过 future 传播\n";
        return 1;
    }

    bool rejected_after_stop = false;
    try {
        static_cast<void>(pool.submit([] { return 0; }));
    } catch (const std::runtime_error&) {
        rejected_after_stop = true;
    }
    if (!rejected_after_stop) {
        std::cerr << "停止后的提交没有被拒绝\n";
        return 1;
    }

    std::cout << "线程池测试通过：drain、reject、future 结果与异常传播均符合契约\n";
    return 0;
}
