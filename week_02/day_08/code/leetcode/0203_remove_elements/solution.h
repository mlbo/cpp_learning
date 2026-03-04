/**
 * @file solution.h
 * @brief LeetCode 203: 移除链表元素 - 头文件
 * @author C++ Tutorial
 * @date Day 8
 */

#ifndef LEETCODE_0203_SOLUTION_H
#define LEETCODE_0203_SOLUTION_H

#include "data_structure/list_node.h"

namespace leetcode {

/**
 * @brief 移除链表中所有等于给定值的节点
 * 
 * @param head 链表头节点
 * @param val 要移除的值
 * @return 移除后的链表头节点
 * 
 * @example
 *   输入: head = [1,2,6,3,4,5,6], val = 6
 *   输出: [1,2,3,4,5]
 * 
 * @complexity
 *   时间复杂度: O(n)
 *   空间复杂度: O(1)
 */
ListNode* removeElements(ListNode* head, int val);

} // namespace leetcode

#endif // LEETCODE_0203_SOLUTION_H
