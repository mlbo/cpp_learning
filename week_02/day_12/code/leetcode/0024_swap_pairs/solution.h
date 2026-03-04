/**
 * @file solution.h
 * @brief LeetCode 24题 - 两两交换链表中的节点
 * 
 * 题目描述：
 * 给定一个链表，两两交换其中相邻的节点，并返回交换后的链表。
 * 你不能只是单纯的改变节点内部的值，而是需要实际的进行节点交换。
 * 
 * 示例:
 * 输入: 1->2->3->4
 * 输出: 2->1->4->3
 * 
 * 输入: 1->2->3
 * 输出: 2->1->3
 */

#ifndef LEETCODE_0024_SWAP_PAIRS_H
#define LEETCODE_0024_SWAP_PAIRS_H

// 链表节点定义
struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};

class Solution {
public:
    /**
     * @brief 方法1：递归法
     * 
     * 思路：
     * 1. 交换前两个节点
     * 2. 递归处理后续链表
     * 3. 连接交换后的部分
     * 
     * 时间复杂度: O(n)
     * 空间复杂度: O(n) - 递归栈深度
     */
    ListNode* swapPairs_recursive(ListNode* head);
    
    /**
     * @brief 方法2：迭代法（推荐）
     * 
     * 思路：
     * 1. 使用虚拟头节点简化边界处理
     * 2. 成对交换节点
     * 3. 更新指针继续处理
     * 
     * 时间复杂度: O(n)
     * 空间复杂度: O(1)
     */
    ListNode* swapPairs_iterative(ListNode* head);
    
    // 主接口
    ListNode* swapPairs(ListNode* head) {
        return swapPairs_iterative(head);
    }
};

#endif // LEETCODE_0024_SWAP_PAIRS_H
