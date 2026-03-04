/**
 * @file solution.cpp
 * @brief LeetCode 61: 旋转链表 - 实现文件
 */

#include "solution.h"
#include <iostream>

namespace leetcode_0061 {

// ========== 方法1: 成环后断开（推荐） ==========

ListNode* Solution::rotateRight(ListNode* head, int k) {
    // 边界情况处理
    if (head == nullptr || head->next == nullptr || k == 0) {
        return head;
    }

    // 步骤1: 计算链表长度并找到尾节点
    int length = 1;
    ListNode* tail = head;
    while (tail->next != nullptr) {
        tail = tail->next;
        length++;
    }

    // 步骤2: 计算有效旋转次数
    // 向右移动k次 = 向右移动 k % n 次
    k = k % length;
    if (k == 0) {
        return head;  // 不需要旋转
    }

    // 步骤3: 将链表首尾相连形成环
    tail->next = head;

    // 步骤4: 找到新的断开点
    // 需要向右移动k个位置，即新的头节点是倒数第k个节点
    // 新的尾节点是倒数第k+1个节点，即第 n-k 个节点（从0开始计数）
    int steps = length - k;
    ListNode* newTail = head;

    // 移动 steps-1 步到达新的尾节点
    for (int i = 1; i < steps; i++) {
        newTail = newTail->next;
    }

    // 步骤5: 断开环并返回新的头节点
    ListNode* newHead = newTail->next;
    newTail->next = nullptr;

    return newHead;
}

// ========== 方法2: 快慢指针 ==========

ListNode* Solution::rotateRightFastSlow(ListNode* head, int k) {
    if (head == nullptr || head->next == nullptr || k == 0) {
        return head;
    }

    // 计算长度
    int length = getListLength(head);
    k = k % length;
    if (k == 0) {
        return head;
    }

    // 快指针先走k步
    ListNode* fast = head;
    for (int i = 0; i < k; i++) {
        fast = fast->next;
    }

    // 快慢指针同时走
    ListNode* slow = head;
    while (fast->next != nullptr) {
        fast = fast->next;
        slow = slow->next;
    }

    // 此时slow指向新的尾节点
    // fast指向原来的尾节点
    ListNode* newHead = slow->next;
    slow->next = nullptr;  // 断开
    fast->next = head;     // 连接到原来的头

    return newHead;
}

// ========== 方法3: 两次遍历 ==========

ListNode* Solution::rotateRightTwoPass(ListNode* head, int k) {
    if (head == nullptr || head->next == nullptr || k == 0) {
        return head;
    }

    // 第一次遍历：计算长度
    int length = 0;
    ListNode* current = head;
    while (current != nullptr) {
        length++;
        current = current->next;
    }

    // 计算有效旋转次数
    k = k % length;
    if (k == 0) {
        return head;
    }

    // 第二次遍历：找到断开点
    int steps = length - k;
    current = head;
    for (int i = 1; i < steps; i++) {
        current = current->next;
    }

    // 断开并重新连接
    ListNode* newHead = current->next;
    current->next = nullptr;

    // 找到原链表的尾节点
    ListNode* tail = newHead;
    while (tail->next != nullptr) {
        tail = tail->next;
    }

    // 将尾部连接到原来的头
    tail->next = head;

    return newHead;
}

// ========== 辅助函数实现 ==========

ListNode* createList(const std::vector<int>& values) {
    if (values.empty()) {
        return nullptr;
    }

    ListNode* head = new ListNode(values[0]);
    ListNode* tail = head;

    for (size_t i = 1; i < values.size(); ++i) {
        tail->next = new ListNode(values[i]);
        tail = tail->next;
    }

    return head;
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

int getListLength(ListNode* head) {
    int length = 0;
    while (head != nullptr) {
        length++;
        head = head->next;
    }
    return length;
}

} // namespace leetcode_0061
