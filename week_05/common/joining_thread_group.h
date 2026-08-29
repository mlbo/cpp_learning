#pragma once

#include <cstddef>
#include <exception>
#include <stdexcept>
#include <thread>
#include <utility>
#include <vector>

namespace week5 {

// 拥有一组线程并在作用域退出时 join。若第 N 个 std::thread 构造抛异常，
// 已经成功创建的前 N-1 个线程仍由本对象拥有，栈展开时不会因 joinable 析构而 terminate。
class JoiningThreadGroup final {
public:
    JoiningThreadGroup() = default;

    ~JoiningThreadGroup() noexcept {
        try {
            join_all();
        } catch (...) {
            // 析构时无法向调用者报告所有权协议违反。正常用法必须由
            // 不属于本组的外部 owner 销毁它。
            std::terminate();
        }
    }

    JoiningThreadGroup(const JoiningThreadGroup&) = delete;
    JoiningThreadGroup& operator=(const JoiningThreadGroup&) = delete;
    JoiningThreadGroup(JoiningThreadGroup&&) = delete;
    JoiningThreadGroup& operator=(JoiningThreadGroup&&) = delete;

    void reserve(std::size_t count) {
        threads_.reserve(count);
    }

    template <class Function, class... Args>
    void start(Function&& function, Args&&... args) {
        threads_.emplace_back(
            std::forward<Function>(function), std::forward<Args>(args)...);
    }

    void join_all() {
        const std::thread::id caller = std::this_thread::get_id();
        for (const std::thread& thread : threads_) {
            if (thread.joinable() && thread.get_id() == caller) {
                throw std::logic_error(
                    "JoiningThreadGroup::join_all must be called by an external owner");
            }
        }
        for (std::thread& thread : threads_) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }

    [[nodiscard]] std::size_t size() const noexcept {
        return threads_.size();
    }

private:
    std::vector<std::thread> threads_;
};

}  // namespace week5
