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

// 每道题拥有自己的模块边界；这样多个solution.h可以安全组合包含。
namespace leetcode_0021 {

// 链表节点定义
struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next_node) : val(x), next(next_node) {}
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
     *
     * @note 原地重连两个输入链表的既有节点，不分配结果节点。返回后，调用者
     *       只能把返回值视为整条链的唯一释放入口，不能再分别释放list1/list2。
     * @pre 两条输入链均无环、升序且节点集合互不重叠。
     */
    ListNode* mergeTwoListsIterative(ListNode* list1, ListNode* list2);
    
    /**
     * @brief 合并两个有序链表 - 递归解法
     * 
     * 思路：比较两个链表头节点，较小者的next指向合并结果
     * 
     * 时间复杂度：O(n + m)
     * 空间复杂度：O(n + m)，递归调用栈
     * @note 与迭代版相同，会修改输入节点的next关系。
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
bool testMergeLists();

} // namespace leetcode_0021
