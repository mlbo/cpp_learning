/**
 * @file test_thread_safe.cpp
 * @brief 线程安全链表测试用例
 */

#include <iostream>
#include <cassert>
#include <thread>
#include <vector>
#include <atomic>
#include "project/thread_safe_list.h"

void test_basic_operations() {
    std::cout << "测试1: 基本操作\n";

    ThreadSafeList<int> list;

    // 测试空链表
    assert(list.empty());
    assert(list.size() == 0);

    // 测试push_front
    list.push_front(1);
    list.push_front(2);
    list.push_front(3);

    assert(list.size() == 3);
    assert(!list.empty());

    // 测试try_pop_front
    auto val = list.try_pop_front();
    assert(val && *val == 3);
    assert(list.size() == 2);

    std::cout << "  通过!\n";
}

void test_concurrent_push() {
    std::cout << "测试2: 并发插入\n";

    ThreadSafeList<int> list;
    const int num_threads = 10;
    const int items_per_thread = 1000;

    std::vector<std::thread> threads;
    for (int t = 0; t < num_threads; ++t) {
        threads.emplace_back([&list, t, items_per_thread]() {
            for (int i = 0; i < items_per_thread; ++i) {
                list.push_front(t * 10000 + i);
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    assert(list.size() == num_threads * items_per_thread);
    std::cout << "  通过! (size=" << list.size() << ")\n";
}

void test_concurrent_read_write() {
    std::cout << "测试3: 并发读写\n";

    ThreadSafeList<int> list;
    std::atomic<bool> done{false};

    // 写入线程
    std::thread writer([&list, &done]() {
        for (int i = 0; i < 10000; ++i) {
            list.push_front(i);
        }
        done = true;
    });

    // 读取线程
    std::thread reader([&list, &done]() {
        int count = 0;
        while (!done) {
            list.for_each([&count](int val) {
                count++;
            });
        }
        std::cout << "  Reader counted " << count << " total visits\n";
    });

    writer.join();
    reader.join();

    std::cout << "  通过!\n";
}

void test_find_and_remove() {
    std::cout << "测试4: 查找和删除\n";

    ThreadSafeList<int> list;

    // 插入一些数据
    for (int i = 0; i < 100; ++i) {
        list.push_front(i);
    }

    // 查找
    auto found = list.find_first_if([](int val) { return val == 50; });
    assert(found && *found == 50);

    // 删除偶数
    size_t removed = list.remove_if([](int val) { return val % 2 == 0; });
    assert(removed == 50);  // 0, 2, 4, ..., 98
    assert(list.size() == 50);

    std::cout << "  通过!\n";
}

void test_move_semantics() {
    std::cout << "测试5: 移动语义\n";

    ThreadSafeList<std::string> list;
    std::string str = "hello";
    list.push_front(std::move(str));
    list.push_front("world");

    assert(list.size() == 2);

    auto val = list.try_pop_front();
    assert(val && *val == "world");

    std::cout << "  通过!\n";
}

void test_clear() {
    std::cout << "测试6: 清空操作\n";

    ThreadSafeList<int> list;
    for (int i = 0; i < 100; ++i) {
        list.push_front(i);
    }

    assert(list.size() == 100);
    list.clear();
    assert(list.empty());
    assert(list.size() == 0);

    std::cout << "  通过!\n";
}

int main() {
    std::cout << "=== 线程安全链表测试 ===\n\n";

    test_basic_operations();
    test_concurrent_push();
    test_concurrent_read_write();
    test_find_and_remove();
    test_move_semantics();
    test_clear();

    std::cout << "\n所有测试通过!\n";
    return 0;
}
