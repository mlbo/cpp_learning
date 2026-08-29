#include <condition_variable>
#include <iostream>
#include <mutex>
#include <optional>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

#include "joining_thread_group.h"

template <typename T>
class CloseableQueue {
public:
    bool push(T value) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (closed_) {
                return false;
            }
            queue_.push(std::move(value));
        }
        ready_.notify_one();
        return true;
    }

    std::optional<T> wait_pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        ready_.wait(lock, [this] { return closed_ || !queue_.empty(); });

        if (queue_.empty()) {
            return std::nullopt;
        }
        T value = std::move(queue_.front());
        queue_.pop();
        return value;
    }

    void close() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            closed_ = true;
        }
        ready_.notify_all();
    }

private:
    std::mutex mutex_;
    std::condition_variable ready_;
    std::queue<T> queue_;
    bool closed_{false};
};

int main() {
    int failures = 0;
    const auto expect = [&failures](bool condition, const char* message) {
        if (!condition) {
            std::cerr << "FAIL: " << message << '\n';
            ++failures;
        }
    };

    CloseableQueue<int> queue;
    std::vector<int> consumed;

    week5::JoiningThreadGroup threads;
    threads.start([&queue, &consumed] {
        while (const std::optional<int> value = queue.wait_pop()) {
            consumed.push_back(*value);
        }
    });

    threads.start([&queue] {
        for (int value = 1; value <= 5; ++value) {
            static_cast<void>(queue.push(value));
        }
        queue.close();
    });

    threads.join_all();

    expect(consumed == std::vector<int>({1, 2, 3, 4, 5}),
           "消费者应按 FIFO 顺序读完关闭前的数据");
    expect(!queue.push(6), "关闭后的 push 应按接口契约失败");
    expect(!queue.wait_pop().has_value(), "关闭且排空后 wait_pop 应返回结束信号");

    std::cout << "条件变量谓词、先通知后等待兼容性与关闭协议测试完成。\n";
    return failures == 0 ? 0 : 1;
}
