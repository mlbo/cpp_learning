/**
 * @file test.cpp
 * @brief LeetCode 23 测试文件
 */

#include "solution.h"
#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>

using namespace leetcode_0023;

// 测试用例结构
struct TestCase {
    std::vector<std::vector<int>> input;
    std::vector<int> expected;
    std::string description;
};

// 运行单个测试用例
void runTest(Solution& sol, const TestCase& tc, int testNum) {
    std::cout << "测试 " << testNum << ": " << tc.description << std::endl;

    // 创建输入链表
    std::vector<ListNode*> lists;
    for (const auto& arr : tc.input) {
        lists.push_back(createList(arr));
    }

    // 打印输入
    std::cout << "输入: ";
    for (size_t i = 0; i < lists.size(); ++i) {
        printList(lists[i]);
        if (i < lists.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;

    // 执行合并
    ListNode* result = sol.mergeKLists(lists);
    std::vector<int> actual = listToVector(result);

    // 打印结果
    std::cout << "期望: ";
    for (size_t i = 0; i < tc.expected.size(); ++i) {
        if (i > 0) std::cout << " -> ";
        std::cout << tc.expected[i];
    }
    std::cout << std::endl;

    std::cout << "实际: ";
    printList(result);
    std::cout << std::endl;

    // 验证
    bool passed = (actual == tc.expected);
    std::cout << "结果: " << (passed ? "✓ 通过" : "✗ 失败") << std::endl;
    std::cout << std::endl;

    // 清理
    deleteList(result);

    assert(passed && "测试失败！");
}

// 比较三种方法的性能
void compareMethods() {
    std::cout << "\n=== 三种方法对比 ===\n" << std::endl;

    Solution sol;

    // 创建大量链表进行测试
    std::vector<std::vector<int>> largeInput;
    for (int i = 0; i < 100; ++i) {
        std::vector<int> list;
        for (int j = 0; j < 100; ++j) {
            list.push_back(i * 100 + j);
        }
        largeInput.push_back(list);
    }

    std::cout << "测试规模: " << largeInput.size() << " 个链表，每个 "
              << largeInput[0].size() << " 个节点" << std::endl;
    std::cout << std::endl;

    // 方法1: 分治
    {
        std::vector<ListNode*> lists;
        for (const auto& arr : largeInput) {
            lists.push_back(createList(arr));
        }

        auto start = std::chrono::high_resolution_clock::now();
        ListNode* result = sol.mergeKLists(lists);
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "方法1 (分治合并): " << duration.count() << " μs" << std::endl;

        deleteList(result);
    }

    // 方法2: 优先队列
    {
        std::vector<ListNode*> lists;
        for (const auto& arr : largeInput) {
            lists.push_back(createList(arr));
        }

        auto start = std::chrono::high_resolution_clock::now();
        ListNode* result = sol.mergeKListsPriorityQueue(lists);
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "方法2 (优先队列): " << duration.count() << " μs" << std::endl;

        deleteList(result);
    }

    // 方法3: 顺序合并（对于大规模数据会较慢，这里跳过）
    std::cout << "方法3 (顺序合并): 对于大规模数据效率较低，跳过测试" << std::endl;
}

int main() {
    std::cout << "╔════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║          LeetCode 23: 合并K个升序链表                       ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════════════════╝" << std::endl;
    std::cout << std::endl;

    Solution sol;

    // 测试用例
    std::vector<TestCase> testCases = {
        {
            {{1, 4, 5}, {1, 3, 4}, {2, 6}},
            {1, 1, 2, 3, 4, 4, 5, 6},
            "示例1: 标准情况"
        },
        {
            {},
            {},
            "示例2: 空数组"
        },
        {
            {{}},
            {},
            "示例3: 单个空链表"
        },
        {
            {{1, 2, 3}},
            {1, 2, 3},
            "单个链表"
        },
        {
            {{1, 2}, {3, 4}, {5, 6}},
            {1, 2, 3, 4, 5, 6},
            "多个有序链表"
        },
        {
            {{-10, -5, 0}, {-8, -3, 2}, {-6, 1, 7}},
            {-10, -8, -6, -5, -3, 0, 1, 2, 7},
            "包含负数"
        },
        {
            {{1}, {0}, {-1}},
            {-1, 0, 1},
            "单节点链表"
        },
        {
            {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}},
            {1, 1, 1, 1, 1, 1, 1, 1, 1},
            "相同值链表"
        }
    };

    // 运行测试
    for (size_t i = 0; i < testCases.size(); ++i) {
        runTest(sol, testCases[i], static_cast<int>(i) + 1);
    }

    // 比较三种方法
    compareMethods();

    // 验证两种方法结果一致
    std::cout << "\n=== 验证方法一致性 ===\n" << std::endl;
    {
        std::vector<ListNode*> lists1 = {
            createList({1, 4, 5}),
            createList({1, 3, 4}),
            createList({2, 6})
        };
        std::vector<ListNode*> lists2 = {
            createList({1, 4, 5}),
            createList({1, 3, 4}),
            createList({2, 6})
        };

        ListNode* result1 = sol.mergeKLists(lists1);
        ListNode* result2 = sol.mergeKListsPriorityQueue(lists2);

        std::vector<int> vec1 = listToVector(result1);
        std::vector<int> vec2 = listToVector(result2);

        std::cout << "分治法结果: ";
        printList(result1);
        std::cout << std::endl;

        std::cout << "优先队列结果: ";
        printList(result2);
        std::cout << std::endl;

        bool same = (vec1 == vec2);
        std::cout << "两种方法结果" << (same ? "一致" : "不一致") << std::endl;

        deleteList(result1);
        deleteList(result2);

        assert(same && "两种方法结果不一致！");
    }

    std::cout << "\n╔════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║              所有测试通过！                                 ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════════════════╝" << std::endl;

    return 0;
}
