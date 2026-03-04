/**
 * @file test.cpp
 * @brief LeetCode 234. 回文链表 - 测试
 */

#include "leetcode/0234_palindrome/solution.h"
#include <iostream>
#include <vector>
#include <cassert>

// 辅助函数：创建链表
ListNode* createList(const std::vector<int>& values) {
    if (values.empty()) return nullptr;

    ListNode* head = new ListNode(values[0]);
    ListNode* curr = head;

    for (size_t i = 1; i < values.size(); ++i) {
        curr->next = new ListNode(values[i]);
        curr = curr->next;
    }

    return head;
}

// 辅助函数：释放链表
void deleteList(ListNode* head) {
    while (head) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

// 辅助函数：打印链表
void printList(ListNode* head) {
    std::cout << "[";
    while (head) {
        std::cout << head->val;
        if (head->next) std::cout << " -> ";
        head = head->next;
    }
    std::cout << "]";
}

// 测试用例
struct TestCase {
    std::vector<int> values;
    bool expected;
    std::string description;
};

void runTests() {
    std::vector<TestCase> testCases = {
        {{1, 2, 2, 1}, true, "偶数长度回文链表"},
        {{1, 2, 3, 2, 1}, true, "奇数长度回文链表"},
        {{1, 2}, false, "非回文链表"},
        {{1}, true, "单节点链表"},
        {{}, true, "空链表"},
        {{1, 1, 1, 1}, true, "所有值相同"},
        {{1, 2, 3, 4}, false, "严格递增链表"},
        {{1, 2, 1}, true, "三节点回文"},
        {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, false, "长链表非回文"},
        {{1, 2, 3, 4, 5, 4, 3, 2, 1}, true, "长链表回文"}
    };

    Solution sol;
    int passed = 0;
    int total = static_cast<int>(testCases.size());

    std::cout << "=== LeetCode 234. 回文链表 测试 ===\n\n";

    for (int i = 0; i < total; ++i) {
        const auto& tc = testCases[i];
        ListNode* head = createList(tc.values);

        std::cout << "测试 " << i + 1 << ": " << tc.description << "\n";
        std::cout << "  输入: ";
        printList(head);
        std::cout << "\n";

        // 测试三种方法
        bool result1 = sol.isPalindrome(head);  // 这个方法会恢复链表
        bool result2 = sol.isPalindrome_array(head);
        bool result3 = sol.isPalindrome_recursive(head);

        bool allCorrect = (result1 == tc.expected) &&
                         (result2 == tc.expected) &&
                         (result3 == tc.expected);

        std::cout << "  期望: " << (tc.expected ? "true" : "false") << "\n";
        std::cout << "  方法1(快慢指针): " << (result1 ? "true" : "false") << "\n";
        std::cout << "  方法2(数组): " << (result2 ? "true" : "false") << "\n";
        std::cout << "  方法3(递归): " << (result3 ? "true" : "false") << "\n";

        if (allCorrect) {
            std::cout << "  结果: ✅ 通过\n";
            passed++;
        } else {
            std::cout << "  结果: ❌ 失败\n";
        }
        std::cout << "\n";

        deleteList(head);
    }

    std::cout << "=== 测试总结 ===\n";
    std::cout << "通过: " << passed << "/" << total << "\n";

    if (passed == total) {
        std::cout << "🎉 所有测试用例通过！\n";
    } else {
        std::cout << "⚠️ 有 " << (total - passed) << " 个测试用例失败\n";
    }
}

// 性能测试
void performanceTest() {
    std::cout << "\n=== 性能测试 ===\n";

    // 创建一个很长的回文链表
    std::vector<int> values;
    for (int i = 1; i <= 10000; ++i) {
        values.push_back(i);
    }
    for (int i = 9999; i >= 1; --i) {
        values.push_back(i);
    }

    ListNode* head = createList(values);
    Solution sol;

    std::cout << "链表长度: " << values.size() << "\n";

    auto start = std::chrono::high_resolution_clock::now();
    bool result = sol.isPalindrome(head);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "结果: " << (result ? "true" : "false") << "\n";
    std::cout << "耗时: " << duration.count() << " 微秒\n";

    deleteList(head);
}

int main() {
    runTests();
    performanceTest();
    return 0;
}
