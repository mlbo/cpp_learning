/**
 * @file test.cpp
 * @brief LeetCode 61 测试文件
 */

#include "solution.h"
#include <iostream>
#include <vector>
#include <cassert>
#include <chrono>

using namespace leetcode_0061;

// 测试用例结构
struct TestCase {
    std::vector<int> input;
    int k;
    std::vector<int> expected;
    std::string description;
};

// 运行单个测试用例
void runTest(Solution& sol, const TestCase& tc, int testNum) {
    std::cout << "测试 " << testNum << ": " << tc.description << std::endl;

    // 创建输入链表
    ListNode* head = createList(tc.input);

    // 打印输入
    std::cout << "输入: ";
    printList(head);
    std::cout << ", k = " << tc.k << std::endl;

    // 执行旋转
    ListNode* result = sol.rotateRight(head, tc.k);
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

// 演示旋转过程
void demonstrateRotation() {
    std::cout << "\n=== 旋转过程演示 ===\n" << std::endl;

    ListNode* head = createList({1, 2, 3, 4, 5});
    std::cout << "原始链表: ";
    printList(head);
    std::cout << std::endl;

    // 手动演示旋转过程
    std::cout << "\n旋转步骤（k=2）:" << std::endl;
    std::cout << "1. 计算链表长度: 5" << std::endl;
    std::cout << "2. 有效旋转次数: 2 % 5 = 2" << std::endl;
    std::cout << "3. 将链表成环: 1->2->3->4->5->1 (环)" << std::endl;
    std::cout << "4. 新断开点: 从头移动 (5-2)=3 步" << std::endl;
    std::cout << "   到达节点3（新尾节点）" << std::endl;
    std::cout << "5. 断开: 3->next = nullptr" << std::endl;
    std::cout << "6. 新头节点: 4" << std::endl;

    Solution sol;
    ListNode* result = sol.rotateRight(head, 2);

    std::cout << "\n旋转结果: ";
    printList(result);
    std::cout << std::endl;

    deleteList(result);
}

// 比较三种方法
void compareMethods() {
    std::cout << "\n=== 三种方法对比 ===\n" << std::endl;

    Solution sol;

    // 创建长链表
    std::vector<int> largeInput;
    for (int i = 1; i <= 10000; ++i) {
        largeInput.push_back(i);
    }

    std::cout << "测试规模: " << largeInput.size() << " 个节点" << std::endl;
    std::cout << "k = 1000" << std::endl;
    std::cout << std::endl;

    // 方法1: 成环后断开
    {
        ListNode* head = createList(largeInput);

        auto start = std::chrono::high_resolution_clock::now();
        ListNode* result = sol.rotateRight(head, 1000);
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "方法1 (成环后断开): " << duration.count() << " μs" << std::endl;

        deleteList(result);
    }

    // 方法2: 快慢指针
    {
        ListNode* head = createList(largeInput);

        auto start = std::chrono::high_resolution_clock::now();
        ListNode* result = sol.rotateRightFastSlow(head, 1000);
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "方法2 (快慢指针): " << duration.count() << " μs" << std::endl;

        deleteList(result);
    }

    // 方法3: 两次遍历
    {
        ListNode* head = createList(largeInput);

        auto start = std::chrono::high_resolution_clock::now();
        ListNode* result = sol.rotateRightTwoPass(head, 1000);
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "方法3 (两次遍历): " << duration.count() << " μs" << std::endl;

        deleteList(result);
    }
}

// 边界情况测试
void testEdgeCases() {
    std::cout << "\n=== 边界情况测试 ===\n" << std::endl;

    Solution sol;

    // 空链表
    {
        ListNode* result = sol.rotateRight(nullptr, 1);
        std::cout << "空链表: " << (result == nullptr ? "nullptr ✓" : "失败 ✗") << std::endl;
    }

    // 单节点
    {
        ListNode* head = createList({1});
        ListNode* result = sol.rotateRight(head, 100);
        std::cout << "单节点: " << ((result != nullptr && result->val == 1) ? "✓" : "✗") << std::endl;
        deleteList(result);
    }

    // k = 0
    {
        ListNode* head = createList({1, 2, 3});
        ListNode* result = sol.rotateRight(head, 0);
        std::vector<int> expected = {1, 2, 3};
        std::vector<int> actual = listToVector(result);
        std::cout << "k=0: " << (actual == expected ? "✓" : "✗") << std::endl;
        deleteList(result);
    }

    // k = n（相当于不旋转）
    {
        ListNode* head = createList({1, 2, 3});
        ListNode* result = sol.rotateRight(head, 3);
        std::vector<int> expected = {1, 2, 3};
        std::vector<int> actual = listToVector(result);
        std::cout << "k=n: " << (actual == expected ? "✓" : "✗") << std::endl;
        deleteList(result);
    }

    // k > n
    {
        ListNode* head = createList({1, 2, 3});
        ListNode* result = sol.rotateRight(head, 5);  // 5 % 3 = 2
        std::vector<int> expected = {2, 3, 1};
        std::vector<int> actual = listToVector(result);
        std::cout << "k>n (k=5,n=3): " << (actual == expected ? "✓" : "✗") << std::endl;
        deleteList(result);
    }
}

// 可视化旋转
void visualizeRotation() {
    std::cout << "\n=== 可视化旋转过程 ===\n" << std::endl;

    auto visualize = [](const std::vector<int>& arr, int k) {
        std::cout << "原数组: [";
        for (size_t i = 0; i < arr.size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << arr[i];
        }
        std::cout << "]" << std::endl;

        std::cout << "向右旋转 " << k << " 步:" << std::endl;

        // 显示每一步
        for (int step = 1; step <= k && step <= 3; ++step) {
            std::cout << "  步骤" << step << ": [";
            // 简单显示旋转后的结果
            size_t newStart = arr.size() - step;
            bool first = true;
            for (size_t i = newStart; i < arr.size(); ++i) {
                if (!first) std::cout << ", ";
                std::cout << arr[i];
                first = false;
            }
            for (size_t i = 0; i < newStart; ++i) {
                if (!first) std::cout << ", ";
                std::cout << arr[i];
                first = false;
            }
            std::cout << "]" << std::endl;
        }
        std::cout << std::endl;
    };

    visualize({1, 2, 3, 4, 5}, 2);
    visualize({0, 1, 2}, 4);
}

int main() {
    std::cout << "╔════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║          LeetCode 61: 旋转链表                              ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════════════════╝" << std::endl;
    std::cout << std::endl;

    Solution sol;

    // 测试用例
    std::vector<TestCase> testCases = {
        {{1, 2, 3, 4, 5}, 2, {4, 5, 1, 2, 3}, "示例1: 标准情况"},
        {{0, 1, 2}, 4, {2, 0, 1}, "示例2: k大于链表长度"},
        {{1}, 1, {1}, "单节点"},
        {{1, 2}, 1, {2, 1}, "两节点，旋转1次"},
        {{1, 2}, 2, {1, 2}, "两节点，旋转2次"},
        {{1, 2, 3, 4, 5}, 0, {1, 2, 3, 4, 5}, "k=0"},
        {{1, 2, 3, 4, 5}, 5, {1, 2, 3, 4, 5}, "k等于长度"},
        {{1, 2, 3, 4, 5}, 10, {1, 2, 3, 4, 5}, "k是长度的倍数"},
        {{1, 2, 3, 4, 5, 6}, 3, {4, 5, 6, 1, 2, 3}, "旋转一半"},
        {{1, 2, 3}, 100, {3, 1, 2}, "大k值"}
    };

    // 运行测试
    for (size_t i = 0; i < testCases.size(); ++i) {
        runTest(sol, testCases[i], static_cast<int>(i) + 1);
    }

    // 演示旋转过程
    demonstrateRotation();

    // 可视化
    visualizeRotation();

    // 边界情况
    testEdgeCases();

    // 比较方法
    compareMethods();

    std::cout << "\n╔════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║              所有测试通过！                                 ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════════════════╝" << std::endl;

    return 0;
}
