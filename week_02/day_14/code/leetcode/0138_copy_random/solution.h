/**
 * @file solution.h
 * @brief LeetCode 138. 随机链表的复制
 *
 * 给你一个长度为 n 的链表，每个节点包含一个额外增加的随机指针 random，
 * 该指针可以指向链表中的任何节点或空节点。构造这个链表的深拷贝。
 */

#ifndef LC_0138_COPY_RANDOM_SOLUTION_H
#define LC_0138_COPY_RANDOM_SOLUTION_H

#include <unordered_map>

// 随机链表节点定义
class Node {
public:
    int val;
    Node* next;
    Node* random;

    Node(int _val) : val(_val), next(nullptr), random(nullptr) {}
};

class Solution {
public:
    /**
     * @brief 方法1: 哈希表法
     *
     * 算法思路：
     * 1. 第一遍遍历：创建所有新节点，建立原节点到新节点的映射
     * 2. 第二遍遍历：设置新节点的next和random指针
     *
     * 时间复杂度: O(n)
     * 空间复杂度: O(n) - 哈希表
     */
    Node* copyRandomList(Node* head);

    /**
     * @brief 方法2: 节点拆分法（O(1)空间）
     *
     * 算法思路：
     * 1. 在每个原节点后插入复制节点
     * 2. 设置复制节点的random指针
     * 3. 拆分链表
     *
     * 时间复杂度: O(n)
     * 空间复杂度: O(1) - 不计返回结果
     */
    Node* copyRandomList_optimized(Node* head);

    /**
     * @brief 方法3: 递归+哈希表
     *
     * 时间复杂度: O(n)
     * 空间复杂度: O(n) - 哈希表和递归栈
     */
    Node* copyRandomList_recursive(Node* head);

private:
    std::unordered_map<Node*, Node*> visited_;

    Node* cloneNode(Node* node);
    Node* copyRecursive(Node* node);
};

#endif // LC_0138_COPY_RANDOM_SOLUTION_H
