/**
 * @file solution.cpp
 * @brief LeetCode 234. 回文链表 - 实现
 */

#include "leetcode/0234_palindrome/solution.h"
#include <vector>

// ============================================================
// 方法1: 快慢指针 + 反转链表（推荐）
// ============================================================

bool Solution::isPalindrome(ListNode* head) {
    // 边界情况
    if (!head || !head->next) {
        return true;
    }

    // 第一步：找到中间节点
    // 快慢指针，slow走到中间，fast走到末尾
    ListNode* slow = head;
    ListNode* fast = head;

    // 当fast走到末尾时，slow走到中间（或中间偏左）
    while (fast->next && fast->next->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    // 第二步：反转后半部分
    // slow是前半部分的最后一个节点
    ListNode* secondHalfStart = reverseList(slow->next);

    // 第三步：比较前后两半
    ListNode* p1 = head;
    ListNode* p2 = secondHalfStart;
    bool result = true;

    while (p2) {  // 只需要比较后半部分的长度
        if (p1->val != p2->val) {
            result = false;
            break;
        }
        p1 = p1->next;
        p2 = p2->next;
    }

    // 第四步：恢复链表（可选，取决于是否允许修改原链表）
    slow->next = reverseList(secondHalfStart);

    return result;
}

ListNode* Solution::reverseList(ListNode* head) {
    ListNode* prev = nullptr;
    ListNode* curr = head;

    while (curr) {
        ListNode* nextTemp = curr->next;
        curr->next = prev;
        prev = curr;
        curr = nextTemp;
    }

    return prev;
}

ListNode* Solution::findMiddle(ListNode* head) {
    ListNode* slow = head;
    ListNode* fast = head;

    while (fast->next && fast->next->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;
}

// ============================================================
// 方法2: 数组存储法
// ============================================================

bool Solution::isPalindrome_array(ListNode* head) {
    std::vector<int> values;

    // 将链表值存入数组
    ListNode* curr = head;
    while (curr) {
        values.push_back(curr->val);
        curr = curr->next;
    }

    // 双指针比较
    int left = 0;
    int right = static_cast<int>(values.size()) - 1;

    while (left < right) {
        if (values[left] != values[right]) {
            return false;
        }
        left++;
        right--;
    }

    return true;
}

// ============================================================
// 方法3: 递归法
// ============================================================

bool Solution::isPalindrome_recursive(ListNode* head) {
    frontPointer_ = head;
    return checkRecursive(frontPointer_, head);
}

bool Solution::checkRecursive(ListNode*& front, ListNode* back) {
    if (!back) {
        return true;
    }

    // 递归到最后一个节点
    if (!checkRecursive(front, back->next)) {
        return false;
    }

    // 比较前后值
    bool result = (front->val == back->val);
    front = front->next;  // 前指针后移

    return result;
}
