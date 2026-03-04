/**
 * @file solution.cpp
 * @brief LeetCode 160: 相交链表 - 实现
 */

#include "solution.h"
#include <unordered_set>

// ============================================
// 方法一：双指针交叉遍历（推荐）
// ============================================

ListNode* Solution160::getIntersectionNode(ListNode* headA, ListNode* headB) {
    if (headA == nullptr || headB == nullptr) {
        return nullptr;
    }
    
    ListNode* pA = headA;
    ListNode* pB = headB;
    
    // 双指针交叉遍历
    // 如果没有交点，两指针最终都会走到nullptr并相等
    while (pA != pB) {
        // 指针A走完链表A后，跳到链表B
        pA = (pA == nullptr) ? headB : pA->next;
        // 指针B走完链表B后，跳到链表A
        pB = (pB == nullptr) ? headA : pB->next;
    }
    
    // 返回交点（如果没有交点，返回nullptr）
    return pA;
}

// ============================================
// 方法二：长度差法
// ============================================

ListNode* Solution160::getIntersectionNodeByLength(ListNode* headA, ListNode* headB) {
    if (headA == nullptr || headB == nullptr) {
        return nullptr;
    }
    
    // 计算链表长度
    auto getLength = [](ListNode* head) -> int {
        int len = 0;
        while (head) {
            ++len;
            head = head->next;
        }
        return len;
    };
    
    int lenA = getLength(headA);
    int lenB = getLength(headB);
    
    // 让较长的链表先走长度差步
    ListNode* pA = headA;
    ListNode* pB = headB;
    
    if (lenA > lenB) {
        for (int i = 0; i < lenA - lenB; ++i) {
            pA = pA->next;
        }
    } else {
        for (int i = 0; i < lenB - lenA; ++i) {
            pB = pB->next;
        }
    }
    
    // 同时前进，寻找交点
    while (pA && pB) {
        if (pA == pB) {
            return pA;  // 找到交点
        }
        pA = pA->next;
        pB = pB->next;
    }
    
    return nullptr;  // 没有交点
}

// ============================================
// 方法三：哈希集合法
// ============================================

ListNode* Solution160::getIntersectionNodeByHash(ListNode* headA, ListNode* headB) {
    std::unordered_set<ListNode*> visited;
    
    // 将链表A的所有节点地址存入集合
    ListNode* curr = headA;
    while (curr) {
        visited.insert(curr);
        curr = curr->next;
    }
    
    // 遍历链表B，查找第一个存在于集合中的节点
    curr = headB;
    while (curr) {
        if (visited.find(curr) != visited.end()) {
            return curr;  // 找到交点
        }
        curr = curr->next;
    }
    
    return nullptr;  // 没有交点
}
