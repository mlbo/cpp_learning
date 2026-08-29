#include <condition_variable>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>
#include <unordered_set>
#include <vector>

#include "joining_thread_group.h"

int main() {
    std::mutex ids_mutex;
    std::condition_variable all_started;
    std::vector<std::thread::id> ids;
    // 声明在 promise 之前：批量创建抛异常时，promise 先析构为
    // broken_promise，解除 worker 的 wait，然后线程组才 join。
    week5::JoiningThreadGroup threads;
    std::promise<void> release_promise;
    const std::shared_future<void> release = release_promise.get_future().share();
    for (int worker = 0; worker < 3; ++worker) {
        static_cast<void>(worker);
        threads.start([&ids_mutex, &all_started, &ids, release] {
            {
                std::lock_guard<std::mutex> lock{ids_mutex};
                ids.push_back(std::this_thread::get_id());
            }
            all_started.notify_one();
            release.wait();
        });
    }

    {
        std::unique_lock<std::mutex> lock{ids_mutex};
        all_started.wait(lock, [&ids] { return ids.size() == 3U; });
    }
    // 三个 worker 此刻都存活且在同一个门闩上等待，ID 不可能因
    // “前一个线程已结束”而被后创建线程复用。
    const std::unordered_set<std::thread::id> unique_ids(ids.begin(), ids.end());
    const bool all_workers_are_distinct = unique_ids.size() == 3U &&
                                          unique_ids.count(std::this_thread::get_id()) == 0U;

    release_promise.set_value();
    threads.join_all();

    std::cout << "进程提供资源与隔离边界，线程是进程内的执行流。\n";
    std::cout << "hardware_concurrency 是提示值: "
              << std::thread::hardware_concurrency() << '\n';
    std::cout << "已完成线程数: " << ids.size() << '\n';
    return all_workers_are_distinct ? 0 : 1;
}
