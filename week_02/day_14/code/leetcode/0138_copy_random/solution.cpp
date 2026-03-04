/**
 * @file solution.cpp
 * @brief LeetCode 138. 随机链表的复制 - 实现
 */

#include "leetcode/0138_copy_random/solution.h"
#include <unordered_map>

// ============================================================
// 方法1: 哈希表法
// ============================================================

Node* Solution::copyRandomList(Node* head) {
    if (!head) return nullptr;

    // 第一步：创建所有节点，建立映射关系
    std::unordered_map<Node*, Node*> mapping;

    Node* curr = head;
    while (curr) {
        mapping[curr] = new Node(curr->val);
        curr = curr->next;
    }

    // 第二步：设置next和random指针
    curr = head;
    while (curr) {
        // 设置next指针
        if (curr->next) {
            mapping[curr]->next = mapping[curr->next];
        }
        // 设置random指针
        if (curr->random) {
            mapping[curr]->random = mapping[curr->random];
        }
        curr = curr->next;
    }

    return mapping[head];
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
    while (curr) {
        Node* copy = new Node(curr->val);
        copy->next = curr->next;
        curr->next = copy;
        curr = copy->next;
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

    // 第三步：拆分链表
    // 分离出: A -> B -> C 和 A' -> B' -> C'
    Node* dummy = new Node(0);  // 哨兵节点
    Node* copyCurr = dummy;
    curr = head;

    while (curr) {
        // 取出复制节点
        copyCurr->next = curr->next;
        copyCurr = copyCurr->next;

        // 恢复原链表
        curr->next = curr->next->next;
        curr = curr->next;
    }

    Node* result = dummy->next;
    delete dummy;
    return result;
}

// ============================================================
// 方法3: 递归+哈希表
// ============================================================

Node* Solution::copyRandomList_recursive(Node* head) {
    visited_.clear();
    return copyRecursive(head);
}

Node* Solution::copyRecursive(Node* node) {
    if (!node) return nullptr;

    // 如果已经创建过，直接返回
    if (visited_.find(node) != visited_.end()) {
        return visited_[node];
    }

    // 创建新节点
    Node* copy = new Node(node->val);
    visited_[node] = copy;  // 先记录，避免循环

    // 递归设置next和random
    copy->next = copyRecursive(node->next);
    copy->random = copyRecursive(node->random);

    return copy;
}

Node* Solution::cloneNode(Node* node) {
    if (!node) return nullptr;

    if (visited_.find(node) != visited_.end()) {
        return visited_[node];
    }

    Node* clone = new Node(node->val);
    visited_[node] = clone;
    return clone;
}
