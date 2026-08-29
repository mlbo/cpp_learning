/**
 * @file solution.h
 * @brief LeetCode 138. 随机链表的复制
 *
 * 给你一个长度为 n 的链表，每个节点包含一个额外增加的随机指针 random，
 * 该指针可以指向链表中的任何节点或空节点。构造这个链表的深拷贝。
 */

#ifndef LC_0138_COPY_RANDOM_SOLUTION_H
#define LC_0138_COPY_RANDOM_SOLUTION_H

#include <memory>
#include <new>
#include <unordered_map>

namespace leetcode_0138 {

// 随机链表节点定义
class Node {
public:
    int val;
    Node* next;
    Node* random;

    explicit Node(int value) : val(value), next(nullptr), random(nullptr) {
        if (constructions_before_failure_ == 0) {
            throw std::bad_alloc();
        }
        if (constructions_before_failure_ > 0) {
            --constructions_before_failure_;
        }
    }

    // 仅供异常路径测试：允许在成功构造指定数量的新节点后注入bad_alloc。
    static void failAfterConstructionsForTest(int successful_constructions) noexcept {
        constructions_before_failure_ = successful_constructions;
    }

    static void disableConstructionFailureForTest() noexcept {
        constructions_before_failure_ = -1;
    }

private:
    inline static int constructions_before_failure_ = -1;
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
     * @note 不修改输入；分配或建表失败时，临时副本由RAII释放。
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
     * @note 会在执行中临时穿插复制节点，但成功后恢复原链；若节点分配失败，
     *       会回滚已经穿插的部分后再抛出，原链仍保持不变。
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
    std::unordered_map<Node*, std::unique_ptr<Node>> visited_;

    Node* copyRecursive(Node* node);
};

} // namespace leetcode_0138

#endif // LC_0138_COPY_RANDOM_SOLUTION_H
