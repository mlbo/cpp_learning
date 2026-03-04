#ifndef LEETCODE_0061_ROTATE_LIST_H
#define LEETCODE_0061_ROTATE_LIST_H

/**
 * @file solution.h
 * @brief LeetCode 61: 旋转链表
 *
 * 题目描述：
 * 给定链表的头节点 head，旋转链表，将链表每个节点向右移动 k 个位置。
 *
 * 示例 1：
 * 输入：head = [1,2,3,4,5], k = 2
 * 输出：[4,5,1,2,3]
 *
 * 示例 2：
 * 输入：head = [0,1,2], k = 4
 * 输出：[2,0,1]
 */

#include <vector>

namespace leetcode_0061 {

// 链表节点定义
struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};

/**
 * @brief 解决方案类
 */
class Solution {
public:
    /**
     * @brief 方法1: 成环后断开（推荐）
     *
     * 思路：
     * 1. 计算链表长度
     * 2. 将链表首尾相连形成环
     * 3. 找到新的断开点
     * 4. 断开并返回新头节点
     *
     * 时间复杂度: O(N)，遍历链表
     * 空间复杂度: O(1)，常数空间
     */
    ListNode* rotateRight(ListNode* head, int k);

    /**
     * @brief 方法2: 快慢指针
     *
     * 思路：
     * 1. 计算链表长度
     * 2. 使用快慢指针找到断开点
     * 3. 重新连接
     *
     * 时间复杂度: O(N)
     * 空间复杂度: O(1)
     */
    ListNode* rotateRightFastSlow(ListNode* head, int k);

    /**
     * @brief 方法3: 两次遍历
     *
     * 思路：
     * 1. 第一遍计算长度
     * 2. 第二遍找到断开点
     *
     * 时间复杂度: O(N)
     * 空间复杂度: O(1)
     */
    ListNode* rotateRightTwoPass(ListNode* head, int k);
};

// ========== 辅助函数 ==========

/**
 * @brief 从数组创建链表
 */
ListNode* createList(const std::vector<int>& values);

/**
 * @brief 链表转数组
 */
std::vector<int> listToVector(ListNode* head);

/**
 * @brief 打印链表
 */
void printList(ListNode* head);

/**
 * @brief 释放链表内存
 */
void deleteList(ListNode* head);

/**
 * @brief 计算链表长度
 */
int getListLength(ListNode* head);

} // namespace leetcode_0061

#endif // LEETCODE_0061_ROTATE_LIST_H
