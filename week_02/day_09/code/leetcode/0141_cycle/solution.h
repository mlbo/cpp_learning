/**
 * @file solution.h
 * @brief LeetCode 141: 环形链表
 * 
 * 给定一个链表，判断链表中是否有环。
 * 
 * 进阶：用 O(1) 内存解决此问题
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
     * @brief 判断链表是否有环 - 快慢指针法
     * 
     * Floyd 判圈算法：
     * - 慢指针每次走1步，快指针每次走2步
     * - 如果有环，快指针最终会追上慢指针
     * - 如果无环，快指针会先到达末尾
     * 
     * 证明：
     * 设环长为 L，当慢指针进入环后：
     * - 快指针相对于慢指针每步多走 1 步
     * - 最多经过 L 步，快指针就会追上慢指针
     * 
     * 时间复杂度：O(n)
     * 空间复杂度：O(1)
     */
    bool hasCycle(ListNode* head);
    
    // 辅助函数
    ListNode* createList(const std::vector<int>& vals);
    ListNode* createListWithCycle(const std::vector<int>& vals, int pos);
    void deleteList(ListNode* head);
};

// 测试函数
void testLinkedListCycle();
