/**
 * @file solution.cpp
 * @brief LeetCode 24题 - 两两交换链表中的节点（实现）
 */

#include "solution.h"

// ============================================
// 方法1：递归法
// ============================================

ListNode* Solution::swapPairs_recursive(ListNode* head) {
    // 基本情况：空链表或只有一个节点
    if (head == nullptr || head->next == nullptr) {
        return head;
    }
    
    // 保存第二个节点（将成为新的头节点）
    ListNode* newHead = head->next;
    
    // 递归处理后续链表
    // 当前第一个节点的next指向递归结果
    head->next = swapPairs_recursive(newHead->next);
    
    // 第二个节点指向第一个节点
    newHead->next = head;
    
    return newHead;
}

// ============================================
// 方法2：迭代法（推荐）
// ============================================

ListNode* Solution::swapPairs_iterative(ListNode* head) {
    // 创建虚拟头节点，简化边界处理
    ListNode dummy(0);
    dummy.next = head;
    
    // prev 指向待交换对的前一个节点
    ListNode* prev = &dummy;
    
    while (prev->next != nullptr && prev->next->next != nullptr) {
        // 获取待交换的两个节点
        ListNode* first = prev->next;
        ListNode* second = prev->next->next;
        
        // 开始交换（三步走）
        // 步骤1: prev->next 指向 second
        prev->next = second;
        
        // 步骤2: first->next 指向 second->next
        first->next = second->next;
        
        // 步骤3: second->next 指向 first
        second->next = first;
        
        // 更新 prev，准备下一对
        prev = first;
        
        /*
         * 交换过程图解:
         * 
         * 交换前: prev -> first -> second -> next_pair
         * 
         * 步骤1: prev -> second, first -> second -> next_pair
         *        prev   first   second
         *          |      |       |
         *          v      v       v
         *        [dummy] [1] -> [2] -> [3] -> [4]
         *          |             ^
         *          +-------------+
         * 
         * 步骤2: first->next = second->next
         *        first -> next_pair (跳过 second)
         * 
         * 步骤3: second->next = first
         *        prev -> second -> first -> next_pair
         * 
         * 结果: prev -> [2] -> [1] -> [3] -> [4]
         */
    }
    
    return dummy.next;
}

/*
 * ============================================
 * 复杂度分析
 * ============================================
 * 
 * 递归法：
 * - 时间: O(n)，每个节点处理一次
 * - 空间: O(n)，递归栈深度为 n/2
 * 
 * 迭代法：
 * - 时间: O(n)，遍历链表一次
 * - 空间: O(1)，只使用常量额外空间
 * 
 * 推荐使用迭代法，空间效率更高。
 */
