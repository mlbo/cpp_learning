/**
 * @file solution.cpp
 * @brief LeetCode 203: 移除链表元素 - 解决方案
 * @author C++ Tutorial
 * @date Day 8
 * 
 * 题目描述：
 * 给你一个链表的头节点 head 和一个整数 val ，
 * 请你删除链表中所有满足 Node.val == val 的节点，并返回 新的头节点 。
 * 
 * 示例 1：
 * 输入：head = [1,2,6,3,4,5,6], val = 6
 * 输出：[1,2,3,4,5]
 * 
 * 示例 2：
 * 输入：head = [], val = 1
 * 输出：[]
 * 
 * 示例 3：
 * 输入：head = [7,7,7,7], val = 7
 * 输出：[]
 */

#include "solution.h"

namespace leetcode {

/**
 * @brief 方法一：虚拟头节点法（推荐）
 * 
 * 使用虚拟头节点简化边界处理，统一所有节点的删除逻辑。
 * 
 * @param head 链表头节点
 * @param val 要移除的值
 * @return 移除后的链表头节点
 */
ListNode* removeElements(ListNode* head, int val) {
    // 创建虚拟头节点，简化头节点删除的特殊情况
    ListNode* dummy = new ListNode(0, head);
    ListNode* prev = dummy;
    
    // 遍历链表
    while (prev->next != nullptr) {
        if (prev->next->val == val) {
            // 找到目标节点，删除
            ListNode* toDelete = prev->next;
            prev->next = toDelete->next;
            delete toDelete;  // 释放内存
        } else {
            // 不是目标节点，继续前进
            prev = prev->next;
        }
    }
    
    // 返回新的头节点
    ListNode* result = dummy->next;
    delete dummy;  // 释放虚拟头节点
    return result;
}

/**
 * @brief 方法二：原地删除法（不使用虚拟头节点）
 * 
 * 单独处理头节点，然后处理后续节点。
 * 
 * @param head 链表头节点
 * @param val 要移除的值
 * @return 移除后的链表头节点
 */
ListNode* removeElementsInPlace(ListNode* head, int val) {
    // 先删除头部所有等于val的节点
    while (head != nullptr && head->val == val) {
        ListNode* toDelete = head;
        head = head->next;
        delete toDelete;
    }
    
    // 如果链表为空，直接返回
    if (head == nullptr) {
        return nullptr;
    }
    
    // 处理后续节点
    ListNode* curr = head;
    while (curr->next != nullptr) {
        if (curr->next->val == val) {
            ListNode* toDelete = curr->next;
            curr->next = toDelete->next;
            delete toDelete;
        } else {
            curr = curr->next;
        }
    }
    
    return head;
}

/**
 * @brief 方法三：递归法
 * 
 * 递归处理后续节点，根据当前节点值决定是否保留。
 * 
 * @param head 链表头节点
 * @param val 要移除的值
 * @return 移除后的链表头节点
 */
ListNode* removeElementsRecursive(ListNode* head, int val) {
    // 基础情况：空链表
    if (head == nullptr) {
        return nullptr;
    }
    
    // 递归处理后续节点
    head->next = removeElementsRecursive(head->next, val);
    
    // 决定当前节点是否保留
    if (head->val == val) {
        ListNode* next = head->next;
        delete head;
        return next;
    } else {
        return head;
    }
}

} // namespace leetcode

// ============================================================
// 测试代码
// ============================================================

#include <iostream>
#include <vector>

using namespace list_ops;

void testRemoveElements() {
    std::cout << "======== LeetCode 203: 移除链表元素 测试 ========\n";
    
    // 测试用例1
    std::cout << "\n【测试用例1】\n";
    ListNode* head1 = createList({1, 2, 6, 3, 4, 5, 6});
    std::cout << "输入: ";
    printList(head1);
    std::cout << "val = 6\n";
    
    ListNode* result1 = leetcode::removeElements(head1, 6);
    std::cout << "输出: ";
    printList(result1);
    std::cout << "预期: 1 -> 2 -> 3 -> 4 -> 5 -> nullptr\n";
    
    std::vector<int> expected1 = {1, 2, 3, 4, 5};
    std::vector<int> actual1 = toVector(result1);
    std::cout << "结果: " << (actual1 == expected1 ? "✓ 通过" : "✗ 失败") << "\n";
    deleteList(result1);
    
    // 测试用例2：空链表
    std::cout << "\n【测试用例2：空链表】\n";
    ListNode* head2 = nullptr;
    std::cout << "输入: []\nval = 1\n";
    
    ListNode* result2 = leetcode::removeElements(head2, 1);
    std::cout << "输出: ";
    printList(result2);
    std::cout << "预期: nullptr\n";
    std::cout << "结果: " << (result2 == nullptr ? "✓ 通过" : "✗ 失败") << "\n";
    
    // 测试用例3：所有节点都删除
    std::cout << "\n【测试用例3：所有节点都删除】\n";
    ListNode* head3 = createList({7, 7, 7, 7});
    std::cout << "输入: ";
    printList(head3);
    std::cout << "val = 7\n";
    
    ListNode* result3 = leetcode::removeElements(head3, 7);
    std::cout << "输出: ";
    printList(result3);
    std::cout << "预期: nullptr\n";
    std::cout << "结果: " << (result3 == nullptr ? "✓ 通过" : "✗ 失败") << "\n";
    
    // 测试用例4：头节点需要删除
    std::cout << "\n【测试用例4：头节点需要删除】\n";
    ListNode* head4 = createList({1, 2, 3});
    std::cout << "输入: ";
    printList(head4);
    std::cout << "val = 1\n";
    
    ListNode* result4 = leetcode::removeElements(head4, 1);
    std::cout << "输出: ";
    printList(result4);
    std::cout << "预期: 2 -> 3 -> nullptr\n";
    
    std::vector<int> expected4 = {2, 3};
    std::vector<int> actual4 = toVector(result4);
    std::cout << "结果: " << (actual4 == expected4 ? "✓ 通过" : "✗ 失败") << "\n";
    deleteList(result4);
    
    // 测试用例5：没有节点需要删除
    std::cout << "\n【测试用例5：没有节点需要删除】\n";
    ListNode* head5 = createList({1, 2, 3});
    std::cout << "输入: ";
    printList(head5);
    std::cout << "val = 4\n";
    
    ListNode* result5 = leetcode::removeElements(head5, 4);
    std::cout << "输出: ";
    printList(result5);
    std::cout << "预期: 1 -> 2 -> 3 -> nullptr\n";
    
    std::vector<int> expected5 = {1, 2, 3};
    std::vector<int> actual5 = toVector(result5);
    std::cout << "结果: " << (actual5 == expected5 ? "✓ 通过" : "✗ 失败") << "\n";
    deleteList(result5);
    
    std::cout << "\n======== 测试完成 ========\n";
}

int main() {
    testRemoveElements();
    return 0;
}
