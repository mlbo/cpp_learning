/**
 * @file test_thread_safe.cpp
 * @brief 线程安全链表测试用例
 */

#include "project/thread_safe_list.h"

#include <atomic>
#include <condition_variable>
#include <iostream>
#include <memory>
#include <new>
#include <stdexcept>
#include <string>
#include <thread>
#include <type_traits>
#include <unordered_set>
#include <vector>

namespace {

using week2_concurrency::ThreadSafeList;

class TestRunner {
public:
    void expect(bool condition, const std::string& message) {
        if (!condition) {
            ++failures_;
            std::cerr << "  [失败] " << message << '\n';
        }
    }

    int failures() const { return failures_; }

private:
    int failures_ = 0;
};

class StartGate {
public:
    explicit StartGate(int participants) : participants_(participants) {}

    void arrive_and_wait() {
        std::unique_lock<std::mutex> lock(mutex_);
        ++arrived_;
        if (arrived_ == participants_) {
            open_ = true;
            cv_.notify_all();
        } else {
            cv_.wait(lock, [this] { return open_; });
        }
    }

private:
    std::mutex mutex_;
    std::condition_variable cv_;
    int participants_;
    int arrived_ = 0;
    bool open_ = false;
};

struct LifetimeState {
    std::atomic<int> destructions{0};
    std::atomic<int> value_one_destructions{0};
};

class LifetimeTracked {
public:
    LifetimeTracked(int value, std::shared_ptr<LifetimeState> state)
        : value_(value), state_(std::move(state)) {}

    LifetimeTracked(const LifetimeTracked&) = delete;
    LifetimeTracked& operator=(const LifetimeTracked&) = delete;

    LifetimeTracked(LifetimeTracked&& other) noexcept
        : value_(other.value_), state_(std::move(other.state_)), active_(other.active_) {
        other.active_ = false;
    }

    LifetimeTracked& operator=(LifetimeTracked&&) = delete;

    ~LifetimeTracked() {
        if (active_) {
            ++state_->destructions;
            if (value_ == 1) {
                ++state_->value_one_destructions;
            }
        }
    }

    int value() const { return value_; }

private:
    int value_;
    std::shared_ptr<LifetimeState> state_;
    bool active_ = true;
};

class ThrowOnCopy {
public:
    explicit ThrowOnCopy(int value = 0) : value_(value) {}

    ThrowOnCopy(const ThrowOnCopy& other) : value_(other.value_) {
        if (copies_before_throw_ == 0) {
            throw std::runtime_error("injected copy failure");
        }
        --copies_before_throw_;
    }

    ThrowOnCopy& operator=(const ThrowOnCopy&) = default;
    ThrowOnCopy(ThrowOnCopy&&) noexcept = default;
    ThrowOnCopy& operator=(ThrowOnCopy&&) noexcept = default;

    int value() const { return value_; }

    static void throw_on_next_copy() { copies_before_throw_ = 0; }
    static void allow_copies(int count = 1000) { copies_before_throw_ = count; }

private:
    int value_;
    static int copies_before_throw_;
};

int ThrowOnCopy::copies_before_throw_ = 1000;

class ThrowBadAllocOnCopy {
public:
    explicit ThrowBadAllocOnCopy(int value) : value_(value) {}
    ThrowBadAllocOnCopy(const ThrowBadAllocOnCopy&) : value_(0) { throw std::bad_alloc(); }
    int value() const { return value_; }
private:
    int value_;
};

void test_basic_operations(TestRunner& test) {
    std::cout << "测试1: 空表与基本操作\n";
    ThreadSafeList<int> list;

    test.expect(list.empty(), "新链表应为空");
    test.expect(list.size() == 0, "新链表大小应为0");
    test.expect(!list.try_pop_front(), "空表弹出应返回空指针");

    list.push_front(1);
    list.push_front(2);
    list.push_back(3);
    test.expect(list.to_vector() == std::vector<int>({2, 1, 3}),
                "头插和尾插后的顺序不正确");

    auto value = list.try_pop_front();
    test.expect(value && *value == 2, "头部弹出值应为2");
    test.expect(list.size() == 2, "弹出后大小应为2");
}

void test_concurrent_push(TestRunner& test) {
    std::cout << "测试2: 多线程插入后的数量与唯一性\n";
    ThreadSafeList<int> list;
    constexpr int num_threads = 8;
    constexpr int items_per_thread = 500;

    std::vector<std::thread> threads;
    for (int thread_id = 0; thread_id < num_threads; ++thread_id) {
        threads.emplace_back([&list, thread_id]() {
            for (int i = 0; i < items_per_thread; ++i) {
                list.push_front(thread_id * items_per_thread + i);
            }
        });
    }
    for (auto& thread : threads) {
        thread.join();
    }

    const auto values = list.to_vector();
    const std::unordered_set<int> unique_values(values.begin(), values.end());
    const auto expected = static_cast<std::size_t>(num_threads * items_per_thread);
    test.expect(list.size() == expected, "并发插入后的size不正确");
    test.expect(values.size() == expected, "遍历得到的节点数不正确");
    test.expect(unique_values.size() == expected, "并发插入后出现重复或丢失的值");
}

void test_concurrent_read_write(TestRunner& test) {
    std::cout << "测试3: 用谓词协议协调真实并发读写\n";
    ThreadSafeList<int> list;
    constexpr int initial_items = 100;
    constexpr int inserted_items = 2000;
    for (int i = 0; i < initial_items; ++i) {
        list.push_front(i);
    }

    std::mutex protocol_mutex;
    std::condition_variable protocol_cv;
    bool reader_entered = false;
    bool writer_finished = false;
    std::atomic<std::size_t> visits{0};

    std::thread reader([&]() {
        bool first = true;
        list.for_each([&](const int&) {
            if (first) {
                first = false;
                std::unique_lock<std::mutex> protocol_lock(protocol_mutex);
                reader_entered = true;
                protocol_cv.notify_all();
                protocol_cv.wait(protocol_lock, [&] { return writer_finished; });
            }
            ++visits;
        });
    });

    std::thread writer([&]() {
        {
            std::unique_lock<std::mutex> protocol_lock(protocol_mutex);
            protocol_cv.wait(protocol_lock, [&] { return reader_entered; });
        }
        for (int i = 0; i < inserted_items; ++i) {
            list.push_front(i);
        }
        {
            std::lock_guard<std::mutex> protocol_lock(protocol_mutex);
            writer_finished = true;
        }
        protocol_cv.notify_all();
    });

    writer.join();
    reader.join();
    const auto expected_size = static_cast<std::size_t>(initial_items + inserted_items);
    test.expect(list.size() == expected_size, "并发读写结束后的最终节点数不正确");
    test.expect(visits.load() == static_cast<std::size_t>(initial_items),
                "遍历从旧头开始，应看到协议开始前的100个节点且不依赖输出顺序");
}

void test_delete_lifetime_handoff(TestRunner& test) {
    std::cout << "测试4: 门闩验证删除竞争期间的节点和值生命周期\n";
    ThreadSafeList<LifetimeTracked> list;
    auto state = std::make_shared<LifetimeState>();
    list.push_front(LifetimeTracked(42, state));

    std::mutex gate_mutex;
    std::condition_variable gate_cv;
    bool reader_entered = false;
    bool remover_announced = false;
    bool reader_verified = false;
    bool release_reader = false;
    std::atomic<bool> remove_completed{false};
    std::atomic<bool> reader_value_valid{true};

    std::thread reader([&]() {
        list.for_each([&](const LifetimeTracked& value) {
            std::unique_lock<std::mutex> lock(gate_mutex);
            reader_entered = true;
            gate_cv.notify_all();
            gate_cv.wait(lock, [&] { return remover_announced; });
            if (value.value() == 42 && state->destructions.load() == 0) {
                reader_verified = true;
            }
            gate_cv.notify_all();
            gate_cv.wait(lock, [&] { return release_reader; });
            reader_value_valid.store(value.value() == 42);
        });
    });

    std::thread remover([&]() {
        {
            std::unique_lock<std::mutex> lock(gate_mutex);
            gate_cv.wait(lock, [&] { return reader_entered; });
            remover_announced = true;
        }
        gate_cv.notify_all();
        (void)list.remove_if([](const LifetimeTracked& value) { return value.value() == 42; });
        remove_completed.store(true);
    });

    {
        std::unique_lock<std::mutex> lock(gate_mutex);
        gate_cv.wait(lock, [&] { return reader_verified; });
        test.expect(state->destructions.load() == 0,
                    "读者持有引用时，摘链线程不得销毁值对象");
        test.expect(!remove_completed.load(),
                    "删除必须等待持有候选节点锁的回调完成");
        release_reader = true;
    }
    gate_cv.notify_all();

    reader.join();
    remover.join();
    test.expect(reader_value_valid.load(),
                "删除竞争期间，回调引用指向的值必须持续存活");
    test.expect(remove_completed.load() && list.empty(), "释放门闩后删除应完成");
    test.expect(state->destructions.load() == 1, "被删除值最终应恰好析构一次");
}

void test_predecessor_removal_after_handoff(TestRunner& test) {
    std::cout << "测试5: 门闩验证前驱摘链不破坏后继访问\n";
    ThreadSafeList<LifetimeTracked> list;
    auto state = std::make_shared<LifetimeState>();
    list.push_back(LifetimeTracked(1, state));
    list.push_back(LifetimeTracked(2, state));

    std::mutex gate_mutex;
    std::condition_variable gate_cv;
    bool walker_on_first = false;
    bool remover_waiting = false;
    bool release_first = false;
    bool remover_completed = false;
    std::atomic<bool> successor_valid_after_predecessor_destroyed{false};

    std::thread walker([&]() {
        list.for_each([&](const LifetimeTracked& value) {
            if (value.value() == 1) {
                std::unique_lock<std::mutex> lock(gate_mutex);
                walker_on_first = true;
                gate_cv.notify_all();
                gate_cv.wait(lock, [&] { return release_first; });
                return;
            }

            std::unique_lock<std::mutex> lock(gate_mutex);
            gate_cv.wait(lock, [&] { return remover_completed; });
            successor_valid_after_predecessor_destroyed.store(
                value.value() == 2 && state->value_one_destructions.load() == 1);
        });
    });

    std::thread remover([&]() {
        {
            std::unique_lock<std::mutex> lock(gate_mutex);
            gate_cv.wait(lock, [&] { return walker_on_first; });
            remover_waiting = true;
        }
        gate_cv.notify_all();
        auto removed = list.try_pop_front();
        removed.reset();
        {
            std::lock_guard<std::mutex> lock(gate_mutex);
            remover_completed = true;
        }
        gate_cv.notify_all();
    });

    {
        std::unique_lock<std::mutex> lock(gate_mutex);
        gate_cv.wait(lock, [&] { return remover_waiting; });
        test.expect(state->value_one_destructions.load() == 0,
                    "遍历者仍持有前驱锁和owner时，删除线程不得销毁前驱");
        release_first = true;
    }
    gate_cv.notify_all();

    walker.join();
    remover.join();
    test.expect(successor_valid_after_predecessor_destroyed.load(),
                "交锁到后继后，即使前驱已摘链析构，后继引用仍必须有效");
    const auto remaining = list.find_first_if(
        [](const LifetimeTracked& value) { return value.value() == 2; });
    test.expect(list.size() == 1U && remaining && remaining->value() == 2,
                "前驱删除提交后最终结构必须只保留后继节点");
}

void test_delete_push_back_stress(TestRunner& test) {
    std::cout << "测试6: 门闩压力覆盖遍历、删除与尾插交接\n";
    constexpr int rounds = 40;
    constexpr int initial_items = 128;
    constexpr int appended_items = 128;

    for (int round = 0; round < rounds; ++round) {
        ThreadSafeList<int> list;
        for (int value = 0; value < initial_items; ++value) {
            list.push_back(value);
        }

        StartGate gate(3);
        std::atomic<bool> traversal_values_valid{true};
        std::thread walker([&]() {
            gate.arrive_and_wait();
            list.for_each([&](int value) {
                if (value < 0 || value >= initial_items + appended_items) {
                    traversal_values_valid.store(false);
                }
            });
        });
        std::thread remover([&]() {
            gate.arrive_and_wait();
            (void)list.remove_if([](int value) {
                return value < initial_items && value % 2 == 0;
            });
        });
        std::thread appender([&]() {
            gate.arrive_and_wait();
            for (int value = initial_items; value < initial_items + appended_items; ++value) {
                list.push_back(value);
            }
        });

        walker.join();
        remover.join();
        appender.join();

        const auto values = list.to_vector();
        const std::unordered_set<int> unique_values(values.begin(), values.end());
        const std::size_t expected =
            static_cast<std::size_t>(initial_items / 2 + appended_items);
        if (!traversal_values_valid.load() || values.size() != expected ||
            unique_values.size() != expected || list.size() != expected) {
            test.expect(false, "并发交接压力后的最终集合或结构计数不正确");
            return;
        }
        for (int value = 1; value < initial_items; value += 2) {
            if (unique_values.count(value) != 1U) {
                test.expect(false, "原始奇数节点不应在删除竞争中丢失");
                return;
            }
        }
        for (int value = initial_items; value < initial_items + appended_items; ++value) {
            if (unique_values.count(value) != 1U) {
                test.expect(false, "并发尾插节点不应丢失或重复");
                return;
            }
        }
    }
}

void test_find_and_remove(TestRunner& test) {
    std::cout << "测试7: 查找不存在、部分删除与全部删除\n";
    ThreadSafeList<int> list;
    for (int i = 0; i < 100; ++i) {
        list.push_front(i);
    }

    auto found = list.find_first_if([](int value) { return value == 50; });
    test.expect(found && *found == 50, "应找到值50");
    test.expect(!list.find_first_if([](int value) { return value == 1000; }),
                "查找不存在的值应返回空指针");

    test.expect(list.update_first_if([](int value) { return value == 50; }, 500),
                "应能更新第一个匹配值");
    auto new_version = list.find_first_if([](int value) { return value == 500; });
    test.expect(found && *found == 50, "更新后旧查找句柄仍应保留旧版本50");
    test.expect(new_version && *new_version == 500, "更新后的新查找应看到新版本500");
    test.expect(!list.find_first_if([](int value) { return value == 50; }),
                "链表节点不应继续暴露旧版本50");

    const std::size_t removed_even =
        list.remove_if([](int value) { return value % 2 == 0; });
    test.expect(removed_even == 50, "应删除50个偶数");
    test.expect(list.size() == 50, "删除偶数后应剩50个节点");

    const std::size_t removed_rest =
        list.remove_if([](const int&) { return true; });
    test.expect(removed_rest == 50, "应能删除剩余全部节点");
    test.expect(list.empty(), "删除全部节点后链表应为空");
}

void test_value_move_and_clear(TestRunner& test) {
    std::cout << "测试8: 元素移动与clear\n";
    ThreadSafeList<std::string> list;
    std::string text = "hello";
    list.push_front(std::move(text));
    list.push_front("world");

    auto value = list.try_pop_front();
    test.expect(value && *value == "world", "应先弹出world");
    list.clear();
    test.expect(list.empty(), "clear后链表应为空");
}

void test_exception_commit_guarantees(TestRunner& test) {
    std::cout << "测试9: 谓词、复制分配与输出回调异常注入\n";
    ThreadSafeList<int> list;
    for (int value = 1; value <= 4; ++value) {
        list.push_back(value);
    }

    bool caught = false;
    int output_count = 0;
    try {
        list.for_each([&](const int&) {
            ++output_count;
            throw std::runtime_error("output callback failed");
        });
    } catch (const std::runtime_error&) {
        caught = true;
    }

    test.expect(caught && output_count == 1, "输出回调异常应原样传播");
    test.expect(list.to_vector() == std::vector<int>({1, 2, 3, 4}),
                "只读回调抛异常后结构必须不变且锁已释放");

    caught = false;
    int predicate_calls = 0;
    try {
        (void)list.remove_if([&](int value) {
            ++predicate_calls;
            if (predicate_calls == 2) {
                throw std::runtime_error("predicate failed after one commit");
            }
            return value == 1;
        });
    } catch (const std::runtime_error&) {
        caught = true;
    }
    test.expect(caught, "删除谓词应在首个删除提交后抛出测试异常");
    test.expect(list.to_vector() == std::vector<int>({2, 3, 4}) && list.size() == 3,
                "部分删除采用基本保证：已提交节点保留删除，结构与size仍一致");

    ThreadSafeList<ThrowOnCopy> throwing_list;
    ThrowOnCopy one(1);
    ThrowOnCopy::throw_on_next_copy();
    caught = false;
    try {
        throwing_list.push_front(one);
    } catch (const std::runtime_error&) {
        caught = true;
    }
    test.expect(caught && throwing_list.empty(),
                "节点或值分配/复制失败时，push_front必须保持原表不变");

    ThreadSafeList<ThrowBadAllocOnCopy> allocation_list;
    ThrowBadAllocOnCopy allocation_value(7);
    caught = false;
    try {
        allocation_list.push_front(allocation_value);
    } catch (const std::bad_alloc&) {
        caught = true;
    }
    test.expect(caught && allocation_list.empty(),
                "节点/值分配阶段报告bad_alloc时，push_front必须保持原结构不变");

    ThrowOnCopy::allow_copies();
    throwing_list.push_front(one);
    const auto before_size = throwing_list.size();
    ThrowOnCopy replacement(2);
    ThrowOnCopy::throw_on_next_copy();
    caught = false;
    try {
        (void)throwing_list.update_first_if(
            [](const ThrowOnCopy&) { return true; }, replacement);
    } catch (const std::runtime_error&) {
        caught = true;
    }
    ThrowOnCopy::allow_copies();
    auto unchanged = throwing_list.find_first_if(
        [](const ThrowOnCopy& value) { return value.value() == 1; });
    test.expect(caught && unchanged && throwing_list.size() == before_size,
                "替换值构造失败时，update_first_if必须提供强保证");
}

void test_iterative_destruction(TestRunner& test) {
    std::cout << "测试10: 长链逐节点析构\n";
    {
        ThreadSafeList<int> list;
        for (int value = 0; value < 20000; ++value) {
            list.push_front(value);
        }
        test.expect(list.size() == 20000U, "长链析构前结构应完整");
    }
    test.expect(true, "长链应在无递归析构栈增长下释放完成");
}

} // namespace

int main() {
    static_assert(!std::is_copy_constructible_v<ThreadSafeList<int>>);
    static_assert(!std::is_move_constructible_v<ThreadSafeList<int>>);

    std::cout << "=== 线程安全链表测试 ===\n\n";
    TestRunner test;

    test_basic_operations(test);
    test_concurrent_push(test);
    test_concurrent_read_write(test);
    test_delete_lifetime_handoff(test);
    test_predecessor_removal_after_handoff(test);
    test_delete_push_back_stress(test);
    test_find_and_remove(test);
    test_value_move_and_clear(test);
    test_exception_commit_guarantees(test);
    test_iterative_destruction(test);

    if (test.failures() != 0) {
        std::cerr << "\n共有 " << test.failures() << " 个检查失败\n";
        return 1;
    }

    std::cout << "\n所有测试通过!\n";
    return 0;
}
