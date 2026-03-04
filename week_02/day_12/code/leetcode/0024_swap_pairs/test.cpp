/**
 * @file test.cpp
 * @brief LeetCode 24题测试用例
 */

#include "solution.h"
#include <iostream>
#include <vector>

// 辅助函数：创建链表
ListNode* createList(const std::vector<int>& vals) {
    if (vals.empty()) return nullptr;
    
    ListNode* head = new ListNode(vals[0]);
    ListNode* curr = head;
    for (size_t i = 1; i < vals.size(); ++i) {
        curr->next = new ListNode(vals[i]);
        curr = curr->next;
    }
    return head;
}

// 辅助函数：打印链表
void printList(ListNode* head) {
    std::cout << "[";
    while (head != nullptr) {
        std::cout << head->val;
        if (head->next != nullptr) {
            std::cout << ",";
        }
        head = head->next;
    }
    std::cout << "]";
}

// 辅助函数：释放链表
void deleteList(ListNode* head) {
    while (head != nullptr) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

// 辅助函数：链表转vector
std::vector<int> listToVector(ListNode* head) {
    std::vector<int> result;
    while (head != nullptr) {
        result.push_back(head->val);
        head = head->next;
    }
    return result;
}

// LeetCode 24 测试命名空间
namespace leetcode_24 {

void run_tests() {
    Solution solution;
    
    std::cout << "\n========================================\n";
    std::cout << "  LeetCode 24: 两两交换链表中的节点\n";
    std::cout << "========================================\n";
    
    // 测试用例1: 偶数个节点
    std::cout << "\n--- 测试用例1: 偶数个节点 ---\n";
    {
        ListNode* head = createList({1, 2, 3, 4});
        std::cout << "输入: ";
        printList(head);
        std::cout << "\n";
        
        ListNode* result = solution.swapPairs(head);
        std::cout << "输出: ";
        printList(result);
        std::cout << "\n";
        std::cout << "预期: [2,1,4,3]\n";
        
        auto vec = listToVector(result);
        if (vec == std::vector<int>{2, 1, 4, 3}) {
            std::cout << "结果: 正确\n";
        } else {
            std::cout << "结果: 错误\n";
        }
        
        deleteList(result);
    }
    
    // 测试用例2: 奇数个节点
    std::cout << "\n--- 测试用例2: 奇数个节点 ---\n";
    {
        ListNode* head = createList({1, 2, 3});
        std::cout << "输入: ";
        printList(head);
        std::cout << "\n";
        
        ListNode* result = solution.swapPairs(head);
        std::cout << "输出: ";
        printList(result);
        std::cout << "\n";
        std::cout << "预期: [2,1,3]\n";
        
        auto vec = listToVector(result);
        if (vec == std::vector<int>{2, 1, 3}) {
            std::cout << "结果: 正确\n";
        } else {
            std::cout << "结果: 错误\n";
        }
        
        deleteList(result);
    }
    
    // 测试用例3: 空链表
    std::cout << "\n--- 测试用例3: 空链表 ---\n";
    {
        ListNode* head = nullptr;
        std::cout << "输入: nullptr\n";
        
        ListNode* result = solution.swapPairs(head);
        std::cout << "输出: " << (result == nullptr ? "nullptr" : "非空") << "\n";
        std::cout << "预期: nullptr\n";
        
        if (result == nullptr) {
            std::cout << "结果: 正确\n";
        } else {
            std::cout << "结果: 错误\n";
        }
    }
    
    // 测试用例4: 单个节点
    std::cout << "\n--- 测试用例4: 单个节点 ---\n";
    {
        ListNode* head = createList({1});
        std::cout << "输入: ";
        printList(head);
        std::cout << "\n";
        
        ListNode* result = solution.swapPairs(head);
        std::cout << "输出: ";
        printList(result);
        std::cout << "\n";
        std::cout << "预期: [1]\n";
        
        auto vec = listToVector(result);
        if (vec == std::vector<int>{1}) {
            std::cout << "结果: 正确\n";
        } else {
            std::cout << "结果: 错误\n";
        }
        
        deleteList(result);
    }
    
    // 比较两种方法
    std::cout << "\n--- 方法比较 ---\n";
    {
        ListNode* head1 = createList({1, 2, 3, 4, 5});
        ListNode* head2 = createList({1, 2, 3, 4, 5});
        
        std::cout << "原始链表: ";
        printList(head1);
        std::cout << "\n";
        
        ListNode* result1 = solution.swapPairs_recursive(head1);
        std::cout << "递归法结果: ";
        printList(result1);
        std::cout << "\n";
        
        ListNode* result2 = solution.swapPairs_iterative(head2);
        std::cout << "迭代法结果: ";
        printList(result2);
        std::cout << "\n";
        
        deleteList(result1);
        deleteList(result2);
    }
    
    // 复杂度分析
    std::cout << "\n--- 复杂度分析 ---\n";
    std::cout << "递归法:\n";
    std::cout << "  时间: O(n) - 每个节点处理一次\n";
    std::cout << "  空间: O(n) - 递归栈深度\n";
    std::cout << "迭代法:\n";
    std::cout << "  时间: O(n) - 遍历链表一次\n";
    std::cout << "  空间: O(1) - 只使用常量额外空间\n";
    
    std::cout << "\n========================================\n";
    std::cout << "  LeetCode 24 测试完成\n";
    std::cout << "========================================\n";
}

} // namespace leetcode_24
