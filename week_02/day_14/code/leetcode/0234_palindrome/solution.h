/**
 * @file solution.h
 * @brief LeetCode 234. 回文链表
 *
 * 给你一个单链表的头节点 head，请你判断该链表是否为回文链表。
 * 如果是，返回 true；否则，返回 false。
 *
 * 进阶：你能否用 O(n) 时间复杂度和 O(1) 空间复杂度解决此题？
 */

#ifndef LC_0234_PALINDROME_SOLUTION_H
#define LC_0234_PALINDROME_SOLUTION_H

#include <vector>

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
     * @brief 方法1: 快慢指针 + 反转链表
     *
     * 算法思路：
     * 1. 使用快慢指针找到链表中点
     * 2. 反转后半部分链表
     * 3. 比较前后两半
     * 4. 恢复链表（可选）
     *
     * 时间复杂度: O(n)
     * 空间复杂度: O(1)
     */
    bool isPalindrome(ListNode* head);

    /**
     * @brief 方法2: 使用数组存储值
     *
     * 算法思路：
     * 1. 遍历链表，将值存入数组
     * 2. 使用双指针比较数组两端
     *
     * 时间复杂度: O(n)
     * 空间复杂度: O(n)
     */
    bool isPalindrome_array(ListNode* head);

    /**
     * @brief 方法3: 递归法
     *
     * 算法思路：
     * 利用递归的栈来反向遍历链表
     *
     * 时间复杂度: O(n)
     * 空间复杂度: O(n) 递归栈
     */
    bool isPalindrome_recursive(ListNode* head);

private:
    /**
     * @brief 反转链表
     */
    ListNode* reverseList(ListNode* head);

    /**
     * @brief 找到链表的中间节点（快慢指针）
     * @return 返回前半部分的最后一个节点
     */
    ListNode* findMiddle(ListNode* head);

    /**
     * @brief 递归辅助函数
     */
    bool checkRecursive(ListNode*& front, ListNode* back);

    ListNode* frontPointer_;  // 用于递归方法
};

#endif // LC_0234_PALINDROME_SOLUTION_H
