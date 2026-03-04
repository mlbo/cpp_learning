/**
 * @file solution.h
 * @brief LeetCode 142: 环形链表 II
 * 
 * 给定链表的头节点 head，返回链表开始入环的第一个节点。
 * 如果链表无环，则返回 null。
 * 
 * 难度：中等
 * 标签：链表、双指针、快慢指针
 */

#ifndef LEETCODE_0142_SOLUTION_H
#define LEETCODE_0142_SOLUTION_H

// 链表节点定义
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

class Solution {
public:
    /**
     * @brief 快慢指针法找环入口
     * 
     * 数学推导：
     * 设 a 为头到入环点的距离
     * 设 b 为入环点到相遇点的距离
     * 设 c 为相遇点到入环点的距离
     * 
     * 慢指针走的距离：a + b
     * 快指针走的距离：a + n(b+c) + b = a + (n+1)b + nc
     * 
     * 快指针是慢指针的两倍：
     * 2(a + b) = a + (n+1)b + nc
     * => a = (n-1)(b+c) + c
     * 
     * 结论：从相遇点和头同时出发，会在入环点相遇！
     * 
     * @param head 链表头节点
     * @return 环入口节点，无环返回 nullptr
     * 
     * 时间复杂度：O(n)
     * 空间复杂度：O(1)
     */
    ListNode *detectCycle(ListNode *head);
    
    /**
     * @brief 哈希表法找环入口（简单但用额外空间）
     */
    ListNode *detectCycleHash(ListNode *head);
};

#endif // LEETCODE_0142_SOLUTION_H
