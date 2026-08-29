/**
 * @file solution.cpp
 * @brief LeetCode 138. 随机链表的复制 - 实现
 */

#include "leetcode/0138_copy_random/solution.h"
#include <memory>
#include <unordered_map>

namespace leetcode_0138 {

// ============================================================
// 方法1: 哈希表法
// ============================================================

Node* Solution::copyRandomList(Node* head) {
    if (!head) return nullptr;

    // 第一步：创建所有节点，建立映射关系
    std::unordered_map<Node*, std::unique_ptr<Node>> mapping;

    Node* curr = head;
    while (curr) {
        mapping.emplace(curr, std::make_unique<Node>(curr->val));
        curr = curr->next;
    }

    // 第二步：设置next和random指针
    curr = head;
    while (curr) {
        // 设置next指针
        if (curr->next) {
            mapping.at(curr)->next = mapping.at(curr->next).get();
        }
        // 设置random指针
        if (curr->random) {
            mapping.at(curr)->random = mapping.at(curr->random).get();
        }
        curr = curr->next;
    }

    Node* result = mapping.at(head).get();
    for (auto& entry : mapping) {
        (void)entry.second.release();
    }
    return result;
}

// ============================================================
// 方法2: 节点拆分法（O(1)空间，推荐）
// ============================================================

Node* Solution::copyRandomList_optimized(Node* head) {
    if (!head) return nullptr;

    // 第一步：在原节点后插入复制节点
    // 原链表: A -> B -> C
    // 变成: A -> A' -> B -> B' -> C -> C'
    Node* curr = head;
    try {
        while (curr) {
            auto copy = std::make_unique<Node>(curr->val);
            copy->next = curr->next;
            curr->next = copy.release();
            curr = curr->next->next;
        }
    } catch (...) {
        // curr指向第一个尚未穿插副本的原节点；逐段恢复并释放已创建副本。
        Node* rollback = head;
        while (rollback != curr) {
            Node* copy = rollback->next;
            rollback->next = copy->next;
            delete copy;
            rollback = rollback->next;
        }
        throw;
    }

    // 第二步：设置复制节点的random指针
    // 如果原节点A的random指向C，则A'的random应指向C'
    curr = head;
    while (curr) {
        if (curr->random) {
            curr->next->random = curr->random->next;
        }
        curr = curr->next->next;
    }

    // 第三步：拆分链表。此时不得再构造节点：若额外哨兵构造抛异常，输入仍处于
    // 穿插状态。直接记住首个副本，并只做不抛的指针重连完成提交。
    // 分离出: A -> B -> C 和 A' -> B' -> C'
    Node* result = head->next;
    curr = head;

    while (curr) {
        Node* copy = curr->next;
        curr->next = copy->next;
        copy->next = curr->next ? curr->next->next : nullptr;
        curr = curr->next;
    }

    return result;
}

// ============================================================
// 方法3: 递归+哈希表
// ============================================================

Node* Solution::copyRandomList_recursive(Node* head) {
    visited_.clear();
    Node* result = nullptr;
    try {
        result = copyRecursive(head);
    } catch (...) {
        visited_.clear();
        throw;
    }
    for (auto& entry : visited_) {
        (void)entry.second.release();
    }
    visited_.clear();
    return result;
}

Node* Solution::copyRecursive(Node* node) {
    if (!node) return nullptr;

    // 如果已经创建过，直接返回
    const auto found = visited_.find(node);
    if (found != visited_.end()) {
        return found->second.get();
    }

    // 创建新节点
    auto owner = std::make_unique<Node>(node->val);
    Node* copy = owner.get();
    visited_.emplace(node, std::move(owner));  // 先记录，避免random环递归

    // 递归设置next和random
    copy->next = copyRecursive(node->next);
    copy->random = copyRecursive(node->random);

    return copy;
}

} // namespace leetcode_0138
