/**
 * @file dynamic_array_test.cpp
 * @brief DynamicArray 测试代码
 */

#include "dynamic_array.h"
#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <algorithm>

using namespace cpp_learning;

// 测试辅助宏
#define TEST(name) std::cout << "  [TEST] " << name << "... "
#define PASS() std::cout << "✅ PASS\n"

void test_default_constructor() {
    TEST("默认构造函数");
    
    DynamicArray<int> arr;
    assert(arr.empty());
    assert(arr.size() == 0);
    assert(arr.capacity() == 0);
    assert(arr.data() == nullptr);
    
    PASS();
}

void test_count_constructor() {
    TEST("计数构造函数");
    
    DynamicArray<int> arr(5);
    assert(!arr.empty());
    assert(arr.size() == 5);
    assert(arr.capacity() == 5);
    
    // 默认初始化为0
    for (std::size_t i = 0; i < arr.size(); ++i) {
        assert(arr[i] == 0);
    }
    
    PASS();
}

void test_count_value_constructor() {
    TEST("计数+值构造函数");
    
    DynamicArray<int> arr(3, 42);
    assert(arr.size() == 3);
    
    for (std::size_t i = 0; i < arr.size(); ++i) {
        assert(arr[i] == 42);
    }
    
    PASS();
}

void test_initializer_list_constructor() {
    TEST("初始化列表构造函数");
    
    DynamicArray<int> arr{1, 2, 3, 4, 5};
    assert(arr.size() == 5);
    assert(arr[0] == 1);
    assert(arr[4] == 5);
    
    PASS();
}

void test_copy_constructor() {
    TEST("拷贝构造函数");
    
    DynamicArray<int> arr1{1, 2, 3};
    DynamicArray<int> arr2(arr1);
    
    assert(arr2.size() == 3);
    assert(arr2[0] == 1);
    assert(arr2[1] == 2);
    assert(arr2[2] == 3);
    
    // 修改原数组不影响拷贝
    arr1[0] = 100;
    assert(arr2[0] == 1);
    
    PASS();
}

void test_move_constructor() {
    TEST("移动构造函数");
    
    DynamicArray<int> arr1{1, 2, 3};
    DynamicArray<int> arr2(std::move(arr1));
    
    assert(arr2.size() == 3);
    assert(arr2[0] == 1);
    
    // 移动后原对象为空
    assert(arr1.empty());
    assert(arr1.data() == nullptr);
    
    PASS();
}

void test_push_back() {
    TEST("push_back 操作");
    
    DynamicArray<int> arr;
    
    for (int i = 0; i < 10; ++i) {
        arr.push_back(i);
    }
    
    assert(arr.size() == 10);
    assert(arr.capacity() >= 10);
    
    for (std::size_t i = 0; i < arr.size(); ++i) {
        assert(arr[i] == static_cast<int>(i));
    }
    
    PASS();
}

void test_emplace_back() {
    TEST("emplace_back 操作");
    
    struct Point {
        int x, y;
        Point(int x_, int y_) : x(x_), y(y_) {}
    };
    
    DynamicArray<Point> arr;
    arr.emplace_back(1, 2);
    arr.emplace_back(3, 4);
    
    assert(arr.size() == 2);
    assert(arr[0].x == 1 && arr[0].y == 2);
    assert(arr[1].x == 3 && arr[1].y == 4);
    
    PASS();
}

void test_pop_back() {
    TEST("pop_back 操作");
    
    DynamicArray<int> arr{1, 2, 3};
    arr.pop_back();
    
    assert(arr.size() == 2);
    assert(arr.back() == 2);
    
    PASS();
}

void test_resize() {
    TEST("resize 操作");
    
    DynamicArray<int> arr{1, 2, 3};
    
    // 扩大
    arr.resize(5, 42);
    assert(arr.size() == 5);
    assert(arr[3] == 42);
    assert(arr[4] == 42);
    
    // 缩小
    arr.resize(2);
    assert(arr.size() == 2);
    assert(arr[0] == 1);
    assert(arr[1] == 2);
    
    PASS();
}

void test_reserve() {
    TEST("reserve 操作");
    
    DynamicArray<int> arr;
    arr.reserve(100);
    
    assert(arr.empty());
    assert(arr.capacity() == 100);
    
    // 添加元素不触发重新分配
    for (int i = 0; i < 50; ++i) {
        arr.push_back(i);
    }
    assert(arr.capacity() == 100);
    
    PASS();
}

void test_at_bounds_check() {
    TEST("at 边界检查");
    
    DynamicArray<int> arr{1, 2, 3};
    
    bool caught = false;
    try {
        arr.at(10);
    } catch (const std::out_of_range&) {
        caught = true;
    }
    assert(caught);
    
    PASS();
}

void test_iteration() {
    TEST("迭代器遍历");
    
    DynamicArray<int> arr{1, 2, 3, 4, 5};
    
    int sum = 0;
    for (auto it = arr.begin(); it != arr.end(); ++it) {
        sum += *it;
    }
    assert(sum == 15);
    
    // 范围for
    sum = 0;
    for (const auto& elem : arr) {
        sum += elem;
    }
    assert(sum == 15);
    
    PASS();
}

void test_copy_assignment() {
    TEST("拷贝赋值");
    
    DynamicArray<int> arr1{1, 2, 3};
    DynamicArray<int> arr2;
    
    arr2 = arr1;
    
    assert(arr2.size() == 3);
    assert(arr2[0] == 1);
    
    // 自赋值测试
    arr2 = arr2;
    assert(arr2.size() == 3);
    
    PASS();
}

void test_move_assignment() {
    TEST("移动赋值");
    
    DynamicArray<int> arr1{1, 2, 3};
    DynamicArray<int> arr2;
    
    arr2 = std::move(arr1);
    
    assert(arr2.size() == 3);
    assert(arr1.empty());
    
    PASS();
}

void test_swap() {
    TEST("swap 操作");
    
    DynamicArray<int> arr1{1, 2, 3};
    DynamicArray<int> arr2{4, 5, 6, 7};
    
    arr1.swap(arr2);
    
    assert(arr1.size() == 4);
    assert(arr2.size() == 3);
    assert(arr1[0] == 4);
    assert(arr2[0] == 1);
    
    PASS();
}

void test_string_elements() {
    TEST("string 元素");
    
    DynamicArray<std::string> arr;
    arr.push_back("Hello");
    arr.push_back("World");
    arr.emplace_back("C++17");
    
    assert(arr.size() == 3);
    assert(arr[0] == "Hello");
    assert(arr[1] == "World");
    assert(arr[2] == "C++17");
    
    PASS();
}

void test_performance_comparison() {
    TEST("性能对比");
    
    const int N = 100000;
    
    // DynamicArray
    auto start = std::chrono::high_resolution_clock::now();
    {
        DynamicArray<int> arr;
        for (int i = 0; i < N; ++i) {
            arr.push_back(i);
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto dynamic_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // std::vector
    start = std::chrono::high_resolution_clock::now();
    {
        std::vector<int> vec;
        for (int i = 0; i < N; ++i) {
            vec.push_back(i);
        }
    }
    end = std::chrono::high_resolution_clock::now();
    auto vector_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "\n    DynamicArray: " << dynamic_time.count() << " μs\n";
    std::cout << "    std::vector:  " << vector_time.count() << " μs\n";
    
    PASS();
}

// ========== 入口函数 ==========

void dynamic_array_test() {
    std::cout << "\n【DynamicArray 测试套件】\n";
    std::cout << "===========================================\n";
    
    test_default_constructor();
    test_count_constructor();
    test_count_value_constructor();
    test_initializer_list_constructor();
    test_copy_constructor();
    test_move_constructor();
    test_push_back();
    test_emplace_back();
    test_pop_back();
    test_resize();
    test_reserve();
    test_at_bounds_check();
    test_iteration();
    test_copy_assignment();
    test_move_assignment();
    test_swap();
    test_string_elements();
    test_performance_comparison();
    
    std::cout << "===========================================\n";
    std::cout << "✅ 所有测试通过!\n";
}
