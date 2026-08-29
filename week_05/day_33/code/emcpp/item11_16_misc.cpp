/**
 * Effective Modern C++ Item 11-16：用可执行检查展示机制与边界。
 */

#include <algorithm>
#include <iostream>
#include <mutex>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

#include "joining_thread_group.h"

// Item 11：删除函数在重载决议时仍会被选中，随后产生清晰的编译错误。
class NonCopyable final {
public:
    NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};

static_assert(!std::is_copy_constructible_v<NonCopyable>);
static_assert(!std::is_copy_assignable_v<NonCopyable>);

// Item 12：override 会检查 const、引用限定和 noexcept 等是否与基类契约匹配。
class Base {
public:
    virtual ~Base() = default;
    virtual int id() const noexcept {
        return 1;
    }
};

class Derived final : public Base {
public:
    int id() const noexcept override {
        return 2;
    }
};

// Item 14：只有真实满足“不抛出”保证时才写 noexcept。
void safeSwap(int& left, int& right) noexcept {
    const int temporary = left;
    left = right;
    right = temporary;
}

// Item 15：constexpr 函数既可以参与常量求值，也可以在运行期调用。
constexpr int square(int value) noexcept {
    return value * value;
}

// Item 16：const 只限制通过 this 修改普通成员，不自动提供并发同步。
class ThreadSafeCounter final {
public:
    void increment() {
        std::lock_guard<std::mutex> lock(mutex_);
        ++value_;
    }

    int value() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return value_;
    }

private:
    mutable std::mutex mutex_;
    int value_{0};
};

int main() {
    Derived derived;
    const Base& base = derived;
    if (base.id() != 2) {
        std::cerr << "Item 12 override 测试失败\n";
        return 1;
    }

    std::vector<int> values{1, 2, 3, 4, 5};
    const auto position = std::find(values.cbegin(), values.cend(), 3);
    if (position == values.cend()) {
        std::cerr << "Item 13 const_iterator 查找失败\n";
        return 1;
    }
    values.erase(position); // C++11 起 erase 接受 const_iterator。
    if (values != std::vector<int>({1, 2, 4, 5})) {
        std::cerr << "Item 13 const_iterator 修改位置测试失败\n";
        return 1;
    }

    static_assert(noexcept(safeSwap(std::declval<int&>(), std::declval<int&>())));
    int left = 1;
    int right = 2;
    safeSwap(left, right);
    if (left != 2 || right != 1) {
        std::cerr << "Item 14 noexcept swap 测试失败\n";
        return 1;
    }

    constexpr int compile_time_result = square(5);
    static_assert(compile_time_result == 25);
    const int runtime_input = left + right;
    if (square(runtime_input) != 9) {
        std::cerr << "Item 15 constexpr 运行期调用测试失败\n";
        return 1;
    }

    ThreadSafeCounter counter;
    week5::JoiningThreadGroup threads;
    threads.reserve(4U);
    for (int worker = 0; worker < 4; ++worker) {
        static_cast<void>(worker);
        threads.start([&counter] {
            for (int iteration = 0; iteration < 1'000; ++iteration) {
                counter.increment();
            }
        });
    }
    threads.join_all();
    if (counter.value() != 4'000) {
        std::cerr << "Item 16 const 成员并发同步测试失败\n";
        return 1;
    }

    std::cout << "EMC++ Item 11-16 测试通过\n";
    return 0;
}
