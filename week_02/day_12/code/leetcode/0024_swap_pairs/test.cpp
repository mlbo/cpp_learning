/**
 * @file test.cpp
 * @brief LeetCode 24题测试用例
 */

#include "solution.h"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace leetcode_24 {
namespace {

ListNode* createList(const std::vector<int>& values) {
    if (values.empty()) {
        return nullptr;
    }

    const auto delete_chain = [](ListNode* node) {
        while (node) {
            ListNode* next_node = node->next;
            delete node;
            node = next_node;
        }
    };
    std::unique_ptr<ListNode, decltype(delete_chain)> owner(
        new ListNode(values.front()), delete_chain);
    ListNode* tail = owner.get();
    for (std::size_t i = 1U; i < values.size(); ++i) {
        auto node = std::make_unique<ListNode>(values[i]);
        tail->next = node.release();
        tail = tail->next;
    }
    return owner.release();
}

std::vector<int> listToVector(const ListNode* head) {
    std::vector<int> result;
    while (head) {
        result.push_back(head->val);
        head = head->next;
    }
    return result;
}

void deleteList(ListNode* head) {
    while (head) {
        ListNode* old = head;
        head = head->next;
        delete old;
    }
}

struct TestCase {
    std::vector<int> input;
    std::vector<int> expected;
    std::string description;
};

} // namespace

bool run_tests() {
    std::cout << "\n=== LeetCode 24: 两两交换链表中的节点 ===\n";
    const std::vector<TestCase> test_cases = {
        {{1, 2, 3, 4}, {2, 1, 4, 3}, "偶数个节点"},
        {{1, 2, 3}, {2, 1, 3}, "奇数个节点，尾节点保持原位"},
        {{}, {}, "空链表"},
        {{1}, {1}, "单节点"},
        {{1, 1, 2, 2, 3}, {1, 1, 2, 2, 3}, "重复值验证交换节点而非改值"},
    };

    Solution solution;
    int failures = 0;
    for (const auto& test_case : test_cases) {
        ListNode* iterative_input = createList(test_case.input);
        ListNode* recursive_input = createList(test_case.input);
        ListNode* iterative = solution.swapPairs_iterative(iterative_input);
        ListNode* recursive = solution.swapPairs_recursive(recursive_input);

        const bool iterative_ok = listToVector(iterative) == test_case.expected;
        const bool recursive_ok = listToVector(recursive) == test_case.expected;
        std::cout << "  " << test_case.description << ": 迭代="
                  << (iterative_ok ? "通过" : "失败") << ", 递归="
                  << (recursive_ok ? "通过" : "失败") << '\n';
        if (!iterative_ok || !recursive_ok) {
            ++failures;
        }

        deleteList(iterative);
        deleteList(recursive);
    }

    std::cout << "  改链不变量：先保存未处理后继；已处理前缀保持正确；"
                 "奇数尾节点仍可达且保持原序。\n";
    return failures == 0;
}

} // namespace leetcode_24
