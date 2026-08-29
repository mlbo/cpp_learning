#pragma once

#include <condition_variable>
#include <cstddef>
#include <exception>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

class ThreadPool final {
public:
    explicit ThreadPool(std::size_t worker_count) {
        if (worker_count == 0U) {
            throw std::invalid_argument("ThreadPool 至少需要一个工作线程");
        }

        try {
            workers_.reserve(worker_count);
            for (std::size_t index = 0; index < worker_count; ++index) {
                static_cast<void>(index);
                workers_.emplace_back([this] { worker_loop(); });
            }
        } catch (...) {
            request_stop();
            join_workers();
            throw;
        }
    }

    ~ThreadPool() {
        // 在线程池自己的任务中销毁池会使 worker 继续访问已销毁的 this，无法安全修复。
        // 生命周期契约要求外部所有者比所有任务活得更久。
        if (active_pool_ == this) {
            std::terminate();
        }
        shutdown();
    }

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;

    template <class Function, class... Args>
    auto submit(Function&& function, Args&&... args)
        -> std::future<std::invoke_result_t<std::decay_t<Function>,
                                            std::decay_t<Args>...>> {
        using StoredFunction = std::decay_t<Function>;
        using StoredArguments = std::tuple<std::decay_t<Args>...>;
        using Result = std::invoke_result_t<StoredFunction, std::decay_t<Args>...>;

        std::future<Result> future;

        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (!accepting_) {
                throw std::runtime_error("线程池已停止接收任务");
            }

            // 先在锁内确认仍接受任务，再衰减并移动调用对象和参数。这样若 shutdown
            // 先取得同一把锁，拒绝提交不会意外消费调用者的 move-only 参数。
            auto invocation =
                [callable = StoredFunction(std::forward<Function>(function)),
                 arguments = StoredArguments(std::forward<Args>(args)...)]() mutable
                -> Result {
                return std::apply(
                    [&callable](auto&&... unpacked) mutable -> Result {
                        return std::invoke(
                            std::move(callable),
                            std::forward<decltype(unpacked)>(unpacked)...);
                    },
                    std::move(arguments));
            };
            auto task = std::make_shared<std::packaged_task<Result()>>(
                std::move(invocation));
            future = task->get_future();
            tasks_.emplace([task] { (*task)(); });
        }
        condition_.notify_one();
        return future;
    }

    // 只有外部控制线程可以等待全部 worker。池内任务调用会抛出并由 future 传播，
    // 避免对当前 worker 执行 join 导致 resource_deadlock 或 noexcept terminate。
    void shutdown() {
        if (active_pool_ == this) {
            throw std::logic_error("ThreadPool::shutdown must be called by an external owner");
        }

        std::call_once(shutdown_once_, [this] {
            request_stop();
            join_workers();
        });
    }

private:
    void request_stop() noexcept {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            accepting_ = false;
            stop_requested_ = true;
        }
        condition_.notify_all();
    }

    void worker_loop() {
        active_pool_ = this;
        while (true) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(mutex_);
                condition_.wait(lock, [this] {
                    return stop_requested_ || !tasks_.empty();
                });

                if (stop_requested_ && tasks_.empty()) {
                    active_pool_ = nullptr;
                    return;
                }

                task = std::move(tasks_.front());
                tasks_.pop();
            }
            task();
        }
    }

    void join_workers() {
        for (std::thread& worker : workers_) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }

    inline static thread_local const ThreadPool* active_pool_{nullptr};

    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex mutex_;
    std::condition_variable condition_;
    std::once_flag shutdown_once_;
    bool accepting_{true};
    bool stop_requested_{false};
};
