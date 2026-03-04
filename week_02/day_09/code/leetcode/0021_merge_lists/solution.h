/**
 * @file solution.h
 * @brief LeetCode 21: 合并两个有序链表
 * 
 * 将两个升序链表合并为一个新的升序链表并返回。
 * 新链表是通过拼接给定的两个链表的所有节点组成的。
 * 
 * 示例：
 * 输入：l1 = [1,2,4], l2 = [1,3,4]
 * 输出：[1,1,2,3,4,4]
 */

#pragma once

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
     * @brief 合并两个有序链表 - 迭代解法
     * 
     * 使用虚拟头节点简化边界处理
     * 
     * 时间复杂度：O(n + m)，n和m分别是两个链表的长度
     * 空间复杂度：O(1)，只使用常量额外空间
     */
    ListNode* mergeTwoListsIterative(ListNode* list1, ListNode* list2);
    
    /**
     * @brief 合并两个有序链表 - 递归解法
     * 
     * 思路：比较两个链表头节点，较小者的next指向合并结果
     * 
     * 时间复杂度：O(n + m)
     * 空间复杂度：O(n + m)，递归调用栈
     */
    ListNode* mergeTwoListsRecursive(ListNode* list1, ListNode* list2);
    
    /**
     * @brief 合并两个有序链表 - 入口函数
     * 
     * 默认使用迭代解法
     */
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2);
    
    // 辅助函数
    ListNode* createList(const std::vector<int>& vals);
    std::vector<int> listToArray(ListNode* head);
    void printList(ListNode* head);
    void deleteList(ListNode* head);
};

// 测试函数
void testMergeLists();
