/**
 * @file solution.cpp
 * @brief LeetCode 21: 合并两个有序链表 - 实现
 * 
 * 将两个升序链表合并为一个新的升序链表并返回。
 */

#include "solution.h"
#include <iostream>
#include <memory>
#include <vector>

namespace leetcode_0021 {

// ============================================================
// 迭代解法：使用虚拟头节点
// ============================================================

ListNode* Solution::mergeTwoListsIterative(ListNode* list1, ListNode* list2) {
    // 虚拟头节点：统一处理边界条件
    ListNode dummy(0);
    ListNode* tail = &dummy;
    
    // 比较并连接较小的节点
    while (list1 && list2) {
        if (list1->val <= list2->val) {
            tail->next = list1;
            list1 = list1->next;
        } else {
            tail->next = list2;
            list2 = list2->next;
        }
        tail = tail->next;
    }
    
    // 连接剩余节点
    tail->next = list1 ? list1 : list2;
    
    return dummy.next;
}

// ============================================================
// 递归解法
// ============================================================

ListNode* Solution::mergeTwoListsRecursive(ListNode* list1, ListNode* list2) {
    // 递归终止条件
    if (!list1) return list2;
    if (!list2) return list1;
    
    // 选择较小的节点作为头，递归处理剩余部分
    if (list1->val <= list2->val) {
        list1->next = mergeTwoListsRecursive(list1->next, list2);
        return list1;
    } else {
        list2->next = mergeTwoListsRecursive(list1, list2->next);
        return list2;
    }
}

// ============================================================
// 入口函数
// ============================================================

ListNode* Solution::mergeTwoLists(ListNode* list1, ListNode* list2) {
    return mergeTwoListsIterative(list1, list2);
}

// ============================================================
// 辅助函数
// ============================================================

ListNode* Solution::createList(const std::vector<int>& vals) {
    if (vals.empty()) return nullptr;

    const auto delete_chain = [this](ListNode* node) { deleteList(node); };
    std::unique_ptr<ListNode, decltype(delete_chain)> owner(
        new ListNode(vals[0]), delete_chain);
    ListNode* cur = owner.get();
    for (std::size_t i = 1; i < vals.size(); ++i) {
        auto node = std::make_unique<ListNode>(vals[i]);
        cur->next = node.release();
        cur = cur->next;
    }
    return owner.release();
}

std::vector<int> Solution::listToArray(ListNode* head) {
    std::vector<int> result;
    while (head) {
        result.push_back(head->val);
        head = head->next;
    }
    return result;
}

void Solution::printList(ListNode* head) {
    std::cout << "[";
    while (head) {
        std::cout << head->val;
        if (head->next) std::cout << ", ";
        head = head->next;
    }
    std::cout << "]";
}

void Solution::deleteList(ListNode* head) {
    while (head) {
        ListNode* tmp = head;
        head = head->next;
        delete tmp;
    }
}

// ============================================================
// 测试函数
// ============================================================

bool testMergeLists() {
    Solution sol;
    bool all_passed = true;
    const auto record = [&all_passed](bool passed, const char* message) {
        all_passed = passed && all_passed;
        if (passed) {
            std::cout << "  ✓ " << message << "\n";
        } else {
            std::cerr << "  ✗ " << message << "\n";
        }
    };
    
    std::cout << "【测试用例】\n\n";
    
    // 测试1：正常情况
    {
        std::cout << "测试1：正常情况\n";
        ListNode* l1 = sol.createList({1, 2, 4});
        ListNode* l2 = sol.createList({1, 3, 4});
        
        std::cout << "  l1: "; sol.printList(l1); std::cout << "\n";
        std::cout << "  l2: "; sol.printList(l2); std::cout << "\n";
        
        ListNode* merged = sol.mergeTwoLists(l1, l2);
        std::vector<int> expected = {1, 1, 2, 3, 4, 4};
        std::vector<int> result = sol.listToArray(merged);
        
        std::cout << "  合并结果: "; sol.printList(merged); std::cout << "\n";
        std::cout << "  期望结果: [1, 1, 2, 3, 4, 4]\n";
        
        record(result == expected, "正常合并结果正确");
        
        sol.deleteList(merged);
    }
    
    // 测试2：一个空链表
    {
        std::cout << "\n测试2：一个空链表\n";
        ListNode* l1 = nullptr;
        ListNode* l2 = sol.createList({1, 3, 5});
        
        ListNode* merged = sol.mergeTwoLists(l1, l2);
        std::vector<int> expected = {1, 3, 5};
        std::vector<int> result = sol.listToArray(merged);
        
        std::cout << "  l1: []\n";
        std::cout << "  l2: [1, 3, 5]\n";
        std::cout << "  合并结果: "; sol.printList(merged); std::cout << "\n";
        
        record(result == expected, "空链表与非空链表合并正确");
        
        sol.deleteList(merged);
    }
    
    // 测试3：两个空链表
    {
        std::cout << "\n测试3：两个空链表\n";
        ListNode* merged = sol.mergeTwoLists(nullptr, nullptr);
        
        std::cout << "  l1: []\n";
        std::cout << "  l2: []\n";
        std::cout << "  合并结果: []\n";
        
        record(merged == nullptr, "两个空链表合并仍为空");
    }
    
    // 测试4：不同长度
    {
        std::cout << "\n测试4：不同长度\n";
        ListNode* l1 = sol.createList({1, 3, 5, 7, 9});
        ListNode* l2 = sol.createList({2, 4});
        
        std::cout << "  l1: "; sol.printList(l1); std::cout << "\n";
        std::cout << "  l2: "; sol.printList(l2); std::cout << "\n";
        
        ListNode* merged = sol.mergeTwoLists(l1, l2);
        std::vector<int> expected = {1, 2, 3, 4, 5, 7, 9};
        std::vector<int> result = sol.listToArray(merged);
        
        std::cout << "  合并结果: "; sol.printList(merged); std::cout << "\n";
        
        record(result == expected, "不同长度链表合并正确");
        
        sol.deleteList(merged);
    }
    
    // 测试5：比较递归和迭代解法
    {
        std::cout << "\n测试5：比较递归和迭代解法\n";
        ListNode* l1 = sol.createList({1, 2, 4});
        ListNode* l2 = sol.createList({1, 3, 4});
        
        ListNode* l1_copy = sol.createList({1, 2, 4});
        ListNode* l2_copy = sol.createList({1, 3, 4});
        
        ListNode* resultIter = sol.mergeTwoListsIterative(l1, l2);
        ListNode* resultRecur = sol.mergeTwoListsRecursive(l1_copy, l2_copy);
        
        std::vector<int> iterArray = sol.listToArray(resultIter);
        std::vector<int> recurArray = sol.listToArray(resultRecur);
        
        std::cout << "  迭代结果: "; sol.printList(resultIter); std::cout << "\n";
        std::cout << "  递归结果: "; sol.printList(resultRecur); std::cout << "\n";
        
        record(iterArray == recurArray, "迭代与递归结果一致");
        
        sol.deleteList(resultIter);
        sol.deleteList(resultRecur);
    }
    
    std::cout << (all_passed ? "\n所有演示检查通过！\n" : "\n存在演示检查失败！\n");
    
    std::cout << "\n【算法分析】\n";
    std::cout << "┌─────────────────────────────────────────────────────┐\n";
    std::cout << "│           迭代解法      │     递归解法             │\n";
    std::cout << "├─────────────────────────────────────────────────────┤\n";
    std::cout << "│ 时间复杂度: O(n+m)     │ 时间复杂度: O(n+m)       │\n";
    std::cout << "│ 空间复杂度: O(1)       │ 空间复杂度: O(n+m)       │\n";
    std::cout << "│ 优点: 空间效率高       │ 优点: 代码简洁           │\n";
    std::cout << "│ 缺点: 代码稍长         │ 缺点: 递归栈开销         │\n";
    std::cout << "└─────────────────────────────────────────────────────┘\n";
    return all_passed;
}

} // namespace leetcode_0021
