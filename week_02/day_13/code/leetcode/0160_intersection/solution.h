/**
 * @file solution.h
 * @brief LeetCode 160: 相交链表
 * 
 * 题目描述：
 * 给你两个单链表的头节点 headA 和 headB ，请你找出并返回两个单链表相交的起始节点。
 * 如果两个链表不存在相交节点，返回 null 。
 * 
 * 题目保证在整个链表中不存在环。
 * 函数返回结果后，链表必须保持其原始结构。
 * 
 * 进阶：你能否设计一个时间复杂度 O(m + n) 、仅用 O(1) 内存的解决方案？
 */

#ifndef LEETCODE_160_SOLUTION_H
#define LEETCODE_160_SOLUTION_H

// 链表节点定义
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* n) : val(x), next(n) {}
};

class Solution160 {
public:
    /**
     * @brief 方法一：双指针交叉遍历（推荐）
     * 
     * 核心思想：
     * 指针A遍历完链表A后，跳到链表B继续
     * 指针B遍历完链表B后，跳到链表A继续
     * 两指针最终会在相交点相遇（或同时到达末尾nullptr）
     * 
     * 数学原理：
     * 设链表A独立部分长度为a，链表B独立部分长度为b，公共部分长度为c
     * 指针A走的距离：a + c + b
     * 指针B走的距离：b + c + a
     * 两者相等，必会在交点相遇
     * 
     * 时间复杂度：O(m + n)
     * 空间复杂度：O(1)
     */
    ListNode* getIntersectionNode(ListNode* headA, ListNode* headB);
    
    /**
     * @brief 方法二：长度差法
     * 
     * 核心思想：
     * 1. 先计算两个链表的长度
     * 2. 让较长的链表先走长度差步
     * 3. 然后两个指针同时前进，相遇即为交点
     * 
     * 时间复杂度：O(m + n)
     * 空间复杂度：O(1)
     */
    ListNode* getIntersectionNodeByLength(ListNode* headA, ListNode* headB);
    
    /**
     * @brief 方法三：哈希集合法
     * 
     * 核心思想：
     * 将链表A的所有节点地址存入哈希集合
     * 然后遍历链表B，第一个存在于集合中的节点即为交点
     * 
     * 时间复杂度：O(m + n)
     * 空间复杂度：O(m) 或 O(n)
     */
    ListNode* getIntersectionNodeByHash(ListNode* headA, ListNode* headB);
};

#endif // LEETCODE_160_SOLUTION_H
