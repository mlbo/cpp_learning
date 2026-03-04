/**
 * @file solution.h
 * @brief LeetCode 206: 反转链表 - 头文件
 * @author C++ Tutorial
 * @date Day 8
 */

#ifndef LEETCODE_0206_SOLUTION_H
#define LEETCODE_0206_SOLUTION_H

#include "data_structure/list_node.h"

namespace leetcode {

/**
 * @brief 反转单链表（迭代法）
 * 
 * @param head 链表头节点
 * @return 反转后的链表头节点
 * 
 * @complexity
 *   时间复杂度: O(n)
 *   空间复杂度: O(1)
 */
ListNode* reverseList(ListNode* head);

/**
 * @brief 反转单链表（递归法）
 * 
 * @param head 链表头节点
 * @return 反转后的链表头节点
 * 
 * @complexity
 *   时间复杂度: O(n)
 *   空间复杂度: O(n) 递归栈
 */
ListNode* reverseListRecursive(ListNode* head);

} // namespace leetcode

#endif // LEETCODE_0206_SOLUTION_H
