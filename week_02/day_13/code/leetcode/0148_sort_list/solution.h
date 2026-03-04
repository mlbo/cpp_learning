/**
 * @file solution.h
 * @brief LeetCode 148: 排序链表
 * 
 * 题目描述：
 * 给你链表的头结点 head ，将其按 升序 排列并返回 排序后的链表 。
 * 
 * 进阶：你可以在 O(n log n) 时间复杂度和常数级空间复杂度下完成吗？
 */

#ifndef LEETCODE_148_SOLUTION_H
#define LEETCODE_148_SOLUTION_H

// 链表节点定义
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* n) : val(x), next(n) {}
};

class Solution148 {
public:
    /**
     * @brief 方法一：归并排序（自顶向下）- 推荐
     * 
     * 核心步骤：
     * 1. 找中点（快慢指针）
     * 2. 断开链表
     * 3. 递归排序两部分
     * 4. 合并有序链表
     * 
     * 时间复杂度：O(n log n)
     * 空间复杂度：O(log n) - 递归栈
     */
    ListNode* sortList(ListNode* head);
    
    /**
     * @brief 方法二：归并排序（自底向上）
     * 
     * 核心思想：
     * 从长度为1的子链表开始，两两合并
     * 逐步增加子链表长度：1, 2, 4, 8, ...
     * 
     * 时间复杂度：O(n log n)
     * 空间复杂度：O(1) - 真正的常数空间
     */
    ListNode* sortListBottomUp(ListNode* head);
    
private:
    /**
     * @brief 合并两个有序链表
     */
    ListNode* merge(ListNode* l1, ListNode* l2);
    
    /**
     * @brief 找到链表的中点（用于分割）
     * @return 返回中点的前一个节点，方便断开链表
     */
    ListNode* findMiddle(ListNode* head);
    
    /**
     * @brief 分割链表，返回后半部分的头节点
     * @param head 链表头节点
     * @param step 分割步长
     * @return 分割后的下一个节点（或nullptr）
     */
    ListNode* split(ListNode* head, int step);
};

#endif // LEETCODE_148_SOLUTION_H
