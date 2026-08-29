/**
 * @file solution.cpp
 * @brief LeetCode 148: 排序链表 - 实现
 */

#include "solution.h"

namespace leetcode_0148 {

// ============================================
// 方法一：归并排序（自顶向下）
// ============================================

ListNode* Solution148::sortList(ListNode* head) {
    // 基本情况：空链表或只有一个节点
    if (head == nullptr || head->next == nullptr) {
        return head;
    }
    
    // 1. 找到中点并断开链表
    ListNode* mid = findMiddle(head);
    ListNode* rightHead = mid->next;
    mid->next = nullptr;  // 断开链表
    
    // 2. 递归排序两部分
    ListNode* left = sortList(head);
    ListNode* right = sortList(rightHead);
    
    // 3. 合并两个有序链表
    return merge(left, right);
}

// 找到链表中点的前一个节点
ListNode* Solution148::findMiddle(ListNode* head) {
    if (head == nullptr || head->next == nullptr) {
        return head;
    }
    
    ListNode* slow = head;
    ListNode* fast = head;
    ListNode* prev = nullptr;
    
    // 快慢指针找中点
    while (fast && fast->next) {
        prev = slow;
        slow = slow->next;
        fast = fast->next->next;
    }
    
    // 返回中点的前一个节点（对于偶数个节点）
    return prev;
}

// 合并两个有序链表
ListNode* Solution148::merge(ListNode* l1, ListNode* l2) {
    ListNode dummy(0);
    ListNode* tail = &dummy;
    
    while (l1 && l2) {
        if (l1->val <= l2->val) {
            tail->next = l1;
            l1 = l1->next;
        } else {
            tail->next = l2;
            l2 = l2->next;
        }
        tail = tail->next;
    }
    
    // 连接剩余部分
    tail->next = l1 ? l1 : l2;
    
    return dummy.next;
}

// ============================================
// 方法二：归并排序（自底向上）
// ============================================

ListNode* Solution148::sortListBottomUp(ListNode* head) {
    if (head == nullptr || head->next == nullptr) {
        return head;
    }
    
    // 计算链表长度
    std::size_t length = 0U;
    ListNode* node = head;
    while (node) {
        ++length;
        node = node->next;
    }
    
    ListNode dummy(0);
    dummy.next = head;
    
    // 从长度为1的子链表开始，逐步增大
    for (std::size_t step = 1U; step < length; step *= 2U) {
        ListNode* prev = &dummy;
        ListNode* curr = dummy.next;
        
        while (curr) {
            // 获取第一段链表（长度step）
            ListNode* head1 = curr;
            ListNode* head2 = split(curr, step);
            
            // 获取第二段链表（长度step）
            curr = split(head2, step);
            
            // 合并两段
            ListNode* merged = merge(head1, head2);
            
            // 连接到结果链表
            prev->next = merged;
            while (prev->next) {
                prev = prev->next;
            }
        }
    }
    
    return dummy.next;
}

// 分割链表，返回后半部分的头节点
ListNode* Solution148::split(ListNode* head, std::size_t step) {
    if (head == nullptr) {
        return nullptr;
    }
    
    // 前进step-1步
    for (std::size_t i = 1U; i < step && head->next; ++i) {
        head = head->next;
    }
    
    ListNode* result = head->next;
    head->next = nullptr;  // 断开链表
    
    return result;
}

} // namespace leetcode_0148
