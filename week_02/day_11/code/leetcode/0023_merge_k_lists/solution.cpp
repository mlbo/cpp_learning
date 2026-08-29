/**
 * @file solution.cpp
 * @brief LeetCode 23: 合并K个升序链表 - 实现文件
 */

#include "solution.h"
#include <algorithm>
#include <iostream>
#include <memory>
#include <utility>

namespace leetcode_0023 {

// ========== 方法1: 分治合并 ==========

ListNode* Solution::mergeKLists(std::vector<ListNode*>& lists) {
    if (lists.empty()) {
        return nullptr;
    }
    return merge(lists, 0U, lists.size() - 1U);
}

ListNode* Solution::merge(std::vector<ListNode*>& lists,
                          std::size_t left,
                          std::size_t right) {
    // 基础情况
    if (left > right) {
        return nullptr;
    }
    if (left == right) {
        return lists[left];
    }

    // 分治：分成两半
    const std::size_t mid = left + (right - left) / 2U;

    // 递归合并每一半
    ListNode* l1 = merge(lists, left, mid);
    ListNode* l2 = merge(lists, mid + 1U, right);

    // 合并两个有序链表
    return mergeTwoLists(l1, l2);
}

ListNode* Solution::mergeTwoLists(ListNode* l1, ListNode* l2) {
    // 使用哑节点简化代码
    ListNode dummy(0);
    ListNode* tail = &dummy;

    while (l1 != nullptr && l2 != nullptr) {
        if (l1->val <= l2->val) {
            tail->next = l1;
            l1 = l1->next;
        } else {
            tail->next = l2;
            l2 = l2->next;
        }
        tail = tail->next;
    }

    // 连接剩余部分
    tail->next = (l1 != nullptr) ? l1 : l2;

    return dummy.next;
}

// ========== 方法2: 优先队列 ==========

ListNode* Solution::mergeKListsPriorityQueue(std::vector<ListNode*>& lists) {
    if (lists.empty()) {
        return nullptr;
    }

    // 自定义比较器（最小堆）
    auto cmp = [](ListNode* a, ListNode* b) noexcept {
        return a->val > b->val;  // 小的优先级高
    };

    // 先为最多K个堆元素准备容量。reserve是本算法唯一可能发生的堆分配，
    // 且发生在任何next重连之前；之后每次pop后至多push一个后继，堆大小
    // 永远不超过初始容量，指针移动和noexcept比较器都不会抛出。
    std::vector<ListNode*> storage;
    storage.reserve(lists.size());
    std::priority_queue<ListNode*, std::vector<ListNode*>, decltype(cmp)> pq(
        cmp, std::move(storage));

    // 将所有链表的头节点放入堆
    for (ListNode* list : lists) {
        if (list != nullptr) {
            pq.push(list);
        }
    }

    // 哑节点
    ListNode dummy(0);
    ListNode* tail = &dummy;

    // 逐个取出最小节点
    while (!pq.empty()) {
        // 取出最小节点
        ListNode* node = pq.top();
        pq.pop();

        // 加入结果链表
        tail->next = node;
        tail = tail->next;

        // 如果有下一个节点，放入堆
        if (node->next != nullptr) {
            pq.push(node->next);
        }
    }

    return dummy.next;
}

// ========== 方法3: 顺序合并 ==========

ListNode* Solution::mergeKListsSequential(std::vector<ListNode*>& lists) {
    if (lists.empty()) {
        return nullptr;
    }

    ListNode* result = nullptr;

    for (ListNode* list : lists) {
        result = mergeTwoLists(result, list);
    }

    return result;
}

// ========== 辅助函数实现 ==========

ListNode* createList(const std::vector<int>& values) {
    if (values.empty()) {
        return nullptr;
    }

    const auto delete_chain = [](ListNode* node) { deleteList(node); };
    std::unique_ptr<ListNode, decltype(delete_chain)> owner(
        new ListNode(values[0]), delete_chain);
    ListNode* tail = owner.get();

    for (std::size_t i = 1; i < values.size(); ++i) {
        auto node = std::make_unique<ListNode>(values[i]);
        tail->next = node.release();
        tail = tail->next;
    }

    return owner.release();
}

std::vector<int> listToVector(ListNode* head) {
    std::vector<int> result;
    while (head != nullptr) {
        result.push_back(head->val);
        head = head->next;
    }
    return result;
}

void printList(ListNode* head) {
    std::cout << "[";
    while (head != nullptr) {
        std::cout << head->val;
        if (head->next != nullptr) {
            std::cout << " -> ";
        }
        head = head->next;
    }
    std::cout << "]";
}

void deleteList(ListNode* head) {
    while (head != nullptr) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

} // namespace leetcode_0023
