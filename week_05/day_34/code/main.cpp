#include <condition_variable>
#include <iostream>
#include <mutex>
#include <optional>
#include <queue>
#include <thread>

#include "joining_thread_group.h"

class IntChannel {
public:
    bool push(int value) {
        {
            std::lock_guard<std::mutex> lock{mutex_};
            if (closed_) {
                return false;
            }
            values_.push(value);
        }
        ready_.notify_one();
        return true;
    }

    std::optional<int> pop() {
        std::unique_lock<std::mutex> lock{mutex_};
        ready_.wait(lock, [this] { return closed_ || !values_.empty(); });
        if (values_.empty()) {
            return std::nullopt;
        }
        const int value = values_.front();
        values_.pop();
        return value;
    }

    void close() {
        {
            std::lock_guard<std::mutex> lock{mutex_};
            closed_ = true;
        }
        ready_.notify_all();
    }

private:
    std::mutex mutex_;
    std::condition_variable ready_;
    std::queue<int> values_;
    bool closed_{false};
};

int main() {
    IntChannel channel;
    int sum = 0;

    week5::JoiningThreadGroup threads;
    threads.start([&channel, &sum] {
        while (const std::optional<int> value = channel.pop()) {
            sum += *value;
        }
    });
    threads.start([&channel] {
        for (int value = 1; value <= 5; ++value) {
            if (!channel.push(value)) {
                return;
            }
        }
        channel.close();
    });

    threads.join_all();

    const bool drained_before_stop = sum == 15;
    const bool rejects_after_stop = !channel.push(99);
    std::cout << "停止协议：close 后排空已有数据，拒绝新数据\n";
    std::cout << "消费总和: " << sum << '\n';
    return drained_before_stop && rejects_after_stop ? 0 : 1;
}
