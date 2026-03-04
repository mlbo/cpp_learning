/**
 * @file thread_safe_list.cpp
 * @brief 线程安全链表实现文件
 *
 * 由于ThreadSafeList是模板类，大部分实现在头文件中。
 * 本文件包含一些辅助函数和非模板实现。
 */

#include "project/thread_safe_list.h"
#include <iostream>
#include <sstream>
#include <iomanip>

namespace thread_safe_list_utils {

/**
 * @brief 打印链表内容（用于调试）
 */
template<typename T>
std::string list_to_string(const ThreadSafeList<T>& list) {
    std::ostringstream oss;
    oss << "[";

    bool first = true;
    list.for_each([&](const T& value) {
        if (!first) oss << ", ";
        oss << value;
        first = false;
    });

    oss << "]";
    return oss.str();
}

/**
 * @brief 验证链表内容是否与预期一致
 */
template<typename T>
bool verify_content(const ThreadSafeList<T>& list, const std::vector<T>& expected) {
    std::vector<T> actual;
    list.for_each([&](const T& value) {
        actual.push_back(value);
    });

    if (actual.size() != expected.size()) {
        return false;
    }

    for (size_t i = 0; i < actual.size(); ++i) {
        if (actual[i] != expected[i]) {
            return false;
        }
    }

    return true;
}

// 显式实例化常用类型
template std::string list_to_string<int>(const ThreadSafeList<int>&);
template bool verify_content<int>(const ThreadSafeList<int>&, const std::vector<int>&);

} // namespace thread_safe_list_utils

// ============================================================
// 示例使用
// ============================================================

void demo_thread_safe_list() {
    std::cout << "=== 线程安全链表示例 ===\n\n";

    ThreadSafeList<int> list;

    // 单线程测试基本操作
    std::cout << "1. 基本操作:\n";

    list.push_front(1);
    list.push_front(2);
    list.push_front(3);
    std::cout << "   push_front(3, 2, 1): size = " << list.size() << "\n";

    list.push_back(4);
    list.push_back(5);
    std::cout << "   push_back(4, 5): size = " << list.size() << "\n";

    std::cout << "   遍历: ";
    list.for_each([](int val) {
        std::cout << val << " ";
    });
    std::cout << "\n";

    // 查找
    std::cout << "\n2. 查找操作:\n";
    auto found = list.find_first_if([](int val) { return val == 3; });
    std::cout << "   find_first_if(==3): " << (found ? "found" : "not found") << "\n";

    found = list.find_first_if([](int val) { return val == 100; });
    std::cout << "   find_first_if(==100): " << (found ? "found" : "not found") << "\n";

    // 删除
    std::cout << "\n3. 删除操作:\n";
    size_t removed = list.remove_if([](int val) { return val % 2 == 0; });
    std::cout << "   remove_if(偶数): removed = " << removed << "\n";
    std::cout << "   剩余: ";
    list.for_each([](int val) {
        std::cout << val << " ";
    });
    std::cout << "\n";

    // 弹出
    std::cout << "\n4. 弹出操作:\n";
    auto popped = list.try_pop_front();
    std::cout << "   try_pop_front: " << (popped ? std::to_string(*popped) : "empty") << "\n";
    std::cout << "   size = " << list.size() << "\n";

    std::cout << "\n示例完成!\n";
}
