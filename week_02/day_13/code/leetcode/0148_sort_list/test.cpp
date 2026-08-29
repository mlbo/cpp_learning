/**
 * @file test.cpp
 * @brief LeetCode 148: 排序链表 - 测试用例
 */

#include "solution.h"
#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using leetcode_0148::ListNode;
using leetcode_0148::Solution148;

// 辅助函数：创建链表
ListNode* createList(const std::vector<int>& values) {
    if (values.empty()) return nullptr;

    const auto delete_chain = [](ListNode* node) {
        while (node) {
            ListNode* next = node->next;
            delete node;
            node = next;
        }
    };
    std::unique_ptr<ListNode, decltype(delete_chain)> owner(
        new ListNode(values[0]), delete_chain);
    ListNode* curr = owner.get();
    for (std::size_t i = 1; i < values.size(); ++i) {
        auto node = std::make_unique<ListNode>(values[i]);
        curr->next = node.release();
        curr = curr->next;
    }
    return owner.release();
}

// 辅助函数：链表转数组
std::vector<int> listToArray(ListNode* head) {
    std::vector<int> result;
    while (head) {
        result.push_back(head->val);
        head = head->next;
    }
    return result;
}

// 辅助函数：打印链表
std::string listToString(ListNode* head) {
    std::string result;
    while (head) {
        result += std::to_string(head->val);
        if (head->next) result += " -> ";
        head = head->next;
    }
    return result.empty() ? "nullptr" : result;
}

// 辅助函数：释放链表
void deleteList(ListNode* head) {
    while (head) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

// 验证链表是否有序
bool isSorted(ListNode* head) {
    while (head && head->next) {
        if (head->val > head->next->val) {
            return false;
        }
        head = head->next;
    }
    return true;
}

// 测试用例
int runTests() {
    Solution148 solution;
    std::cout << "=== LeetCode 148: 排序链表 测试 ===\n\n";

    struct TestCase {
        std::vector<int> input;
        std::vector<int> expected;
        const char* description;
    };

    const std::vector<TestCase> test_cases = {
        {{4, 2, 1, 3}, {1, 2, 3, 4}, "普通乱序"},
        {{-1, 5, 3, 4, 0}, {-1, 0, 3, 4, 5}, "负数与乱序"},
        {{}, {}, "空链表"},
        {{1}, {1}, "单节点"},
        {{1, 2, 3, 4, 5}, {1, 2, 3, 4, 5}, "已排序"},
        {{5, 4, 3, 2, 1}, {1, 2, 3, 4, 5}, "逆序"},
        {{3, 1, 2, 3, 1}, {1, 1, 2, 3, 3}, "重复元素"},
        {{4, 2, 1, 3, 6, 5, 8, 7}, {1, 2, 3, 4, 5, 6, 7, 8}, "偶数长度"},
    };

    int failures = 0;
    for (const auto& test_case : test_cases) {
        ListNode* top_down_input = createList(test_case.input);
        ListNode* bottom_up_input = createList(test_case.input);
        ListNode* top_down = solution.sortList(top_down_input);
        ListNode* bottom_up = solution.sortListBottomUp(bottom_up_input);

        const bool top_down_ok = isSorted(top_down) && listToArray(top_down) == test_case.expected;
        const bool bottom_up_ok =
            isSorted(bottom_up) && listToArray(bottom_up) == test_case.expected;
        std::cout << "  " << test_case.description << ": 自顶向下="
                  << (top_down_ok ? "通过" : "失败") << ", 自底向上="
                  << (bottom_up_ok ? "通过" : "失败") << "\n";
        if (!top_down_ok || !bottom_up_ok) {
            ++failures;
        }

        deleteList(top_down);
        deleteList(bottom_up);
    }

    if (failures == 0) {
        std::cout << "=== 所有测试通过! ===\n";
    }
    return failures;
}

// 演示函数（供主程序调用）
namespace leetcode {

void testSortList() {
    Solution148 solution;
    
    // 演示1：普通排序
    ListNode* head1 = createList({4, 2, 1, 3});
    std::cout << "原链表: " << listToString(head1) << "\n";
    ListNode* sorted1 = solution.sortList(head1);
    std::cout << "排序后: " << listToString(sorted1) << "\n";
    deleteList(sorted1);
    
    // 演示2：较长链表
    std::cout << "\n";
    ListNode* head2 = createList({-1, 5, 3, 4, 0});
    std::cout << "原链表: " << listToString(head2) << "\n";
    ListNode* sorted2 = solution.sortListBottomUp(head2);
    std::cout << "排序后: " << listToString(sorted2) << "\n";
    deleteList(sorted2);
    
    // 算法复杂度说明
    std::cout << "\n复杂度分析:\n";
    std::cout << "自顶向下: O(n log n) 时间, O(log n) 空间(递归栈)\n";
    std::cout << "自底向上: O(n log n) 时间, O(1) 空间\n";
}

} // namespace leetcode

// 主函数
int main() {
    return runTests() == 0 ? 0 : 1;
}
