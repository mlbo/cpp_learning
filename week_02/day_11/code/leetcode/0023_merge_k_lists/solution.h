#ifndef LEETCODE_0023_MERGE_K_LISTS_H
#define LEETCODE_0023_MERGE_K_LISTS_H

/**
 * @file solution.h
 * @brief LeetCode 23: 合并K个升序链表
 *
 * 题目描述：
 * 给你一个链表数组，每个链表都已经按升序排列。
 * 请将所有链表合并到一个升序链表中，返回合并后的链表。
 *
 * 示例 1：
 * 输入：lists = [[1,4,5],[1,3,4],[2,6]]
 * 输出：[1,1,2,3,4,4,5,6]
 *
 * 示例 2：
 * 输入：lists = []
 * 输出：[]
 *
 * 示例 3：
 * 输入：lists = [[]]
 * 输出：[]
 */

#include <cstddef>
#include <functional>
#include <queue>
#include <vector>

namespace leetcode_0023 {

// 链表节点定义
struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next_node) : val(x), next(next_node) {}
};

/**
 * @brief 解决方案类
 */
class Solution {
public:
    /**
     * @brief 方法1: 分治合并
     *
     * 思路：
     * 1. 将K个链表分成两半
     * 2. 递归合并每一半
     * 3. 最后合并两个结果
     *
     * 时间复杂度: O(N * log K)，N为所有节点总数，K为链表数量
     * 空间复杂度: O(log K)，递归栈深度
     *
     * @pre 每条链表无环、升序，且不同输入链表不共享节点。
     * @note 原地重连输入节点，不创建结果节点。成功后lists中的旧头指针只是
     *       借用别名；调用者必须只从返回头释放合并后的整条链。
     */
    ListNode* mergeKLists(std::vector<ListNode*>& lists);

    /**
     * @brief 方法2: 优先队列（最小堆）
     *
     * 思路：
     * 1. 将所有链表的头节点放入最小堆
     * 2. 每次取出最小节点，加入结果链表
     * 3. 如果该节点有next，将next放入堆
     *
     * 时间复杂度: O(N * log K)
     * 空间复杂度: O(K)，优先队列大小
     * @note 先为至多K个堆元素reserve容量，再开始改链；容量分配失败时输入完全
     *       不变。提交阶段的堆大小不超过该容量，指针移动、比较和next重连均不抛，
     *       因此本实现对分配失败提供强保证。
     */
    ListNode* mergeKListsPriorityQueue(std::vector<ListNode*>& lists);

    /**
     * @brief 方法3: 顺序合并（简单但较慢）
     *
     * 思路：逐个合并链表
     *
     * 时间复杂度: 最坏O(N * K)，N为所有节点总数
     * 空间复杂度: O(1)
     */
    ListNode* mergeKListsSequential(std::vector<ListNode*>& lists);

private:
    /**
     * @brief 合并两个有序链表
     */
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2);

    /**
     * @brief 分治合并的递归函数
     */
    ListNode* merge(std::vector<ListNode*>& lists,
                    std::size_t left,
                    std::size_t right);
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

} // namespace leetcode_0023

#endif // LEETCODE_0023_MERGE_K_LISTS_H
