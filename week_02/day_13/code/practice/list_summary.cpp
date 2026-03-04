/**
 * @file list_summary.cpp
 * @brief 链表技巧总结
 * 
 * 涵盖内容：
 * 1. 链表基础操作
 * 2. 双指针技巧
 * 3. 虚拟头节点
 * 4. 常见问题模板
 */

#include <iostream>
#include "list_node.h"

// ============================================
// 技巧一：虚拟头节点
// ============================================

ListNode* removeElements(ListNode* head, int val) {
    // 使用虚拟头节点统一处理删除逻辑
    ListNode dummy(0);
    dummy.next = head;
    ListNode* prev = &dummy;
    
    while (prev->next) {
        if (prev->next->val == val) {
            ListNode* toDelete = prev->next;
            prev->next = prev->next->next;
            delete toDelete;
        } else {
            prev = prev->next;
        }
    }
    
    return dummy.next;
}

// ============================================
// 技巧二：快慢指针找中点
// ============================================

ListNode* findMiddle(ListNode* head) {
    if (!head) return nullptr;
    
    ListNode* slow = head;
    ListNode* fast = head;
    
    // 慢指针走一步，快指针走两步
    while (fast->next && fast->next->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    
    return slow;  // 返回中点（偶数个节点时返回前半部分最后一个）
}

// ============================================
// 技巧三：快慢指针检测环
// ============================================

bool hasCycle(ListNode* head) {
    if (!head || !head->next) return false;
    
    ListNode* slow = head;
    ListNode* fast = head;
    
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        
        if (slow == fast) {
            return true;  // 快慢指针相遇，存在环
        }
    }
    
    return false;
}

// ============================================
// 技巧四：反转链表
// ============================================

// 迭代版本
ListNode* reverseList(ListNode* head) {
    ListNode* prev = nullptr;
    ListNode* curr = head;
    
    while (curr) {
        ListNode* next = curr->next;  // 保存下一个节点
        curr->next = prev;            // 反转指针
        prev = curr;                  // 前移prev
        curr = next;                  // 前移curr
    }
    
    return prev;
}

// 递归版本
ListNode* reverseListRecursive(ListNode* head) {
    if (!head || !head->next) {
        return head;
    }
    
    ListNode* newHead = reverseListRecursive(head->next);
    head->next->next = head;
    head->next = nullptr;
    
    return newHead;
}

// ============================================
// 技巧五：合并两个有序链表
// ============================================

ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
    ListNode dummy(0);
    ListNode* tail = &dummy;
    
    while (l1 && l2) {
        if (l1->val <= l2->val) {
            tail->next = l1;
            l1 = l1->next;
        } else {
            tail->next = l2;
            l2 = l2->next;
        }
        tail = tail->next;
    }
    
    tail->next = l1 ? l1 : l2;
    return dummy.next;
}

// ============================================
// 技巧六：前后指针删除倒数第N个节点
// ============================================

ListNode* removeNthFromEnd(ListNode* head, int n) {
    ListNode dummy(0);
    dummy.next = head;
    
    ListNode* first = &dummy;
    ListNode* second = &dummy;
    
    // first 先走 n 步
    for (int i = 0; i <= n; ++i) {
        first = first->next;
    }
    
    // 同时移动，直到 first 到达末尾
    while (first) {
        first = first->next;
        second = second->next;
    }
    
    // 删除节点
    ListNode* toDelete = second->next;
    second->next = second->next->next;
    delete toDelete;
    
    return dummy.next;
}

// ============================================
// 演示函数
// ============================================

namespace practice {

void demonstrateListSummary() {
    // 1. 虚拟头节点演示
    std::cout << "【虚拟头节点】删除指定值节点\n";
    ListNode* list1 = createList({1, 2, 6, 3, 4, 5, 6});
    std::cout << "原链表: " << listToString(list1) << "\n";
    list1 = removeElements(list1, 6);
    std::cout << "删除6后: " << listToString(list1) << "\n";
    deleteList(list1);
    
    // 2. 快慢指针找中点
    std::cout << "\n【快慢指针】找中点\n";
    ListNode* list2 = createList({1, 2, 3, 4, 5});
    std::cout << "链表: " << listToString(list2) << "\n";
    std::cout << "中点: " << findMiddle(list2)->val << "\n";
    deleteList(list2);
    
    ListNode* list3 = createList({1, 2, 3, 4, 5, 6});
    std::cout << "链表: " << listToString(list3) << "\n";
    std::cout << "中点(偶数): " << findMiddle(list3)->val << "\n";
    deleteList(list3);
    
    // 3. 反转链表
    std::cout << "\n【反转链表】\n";
    ListNode* list4 = createList({1, 2, 3, 4, 5});
    std::cout << "原链表: " << listToString(list4) << "\n";
    ListNode* reversed = reverseList(list4);
    std::cout << "反转后: " << listToString(reversed) << "\n";
    deleteList(reversed);
    
    // 4. 合并有序链表
    std::cout << "\n【合并有序链表】\n";
    ListNode* la = createList({1, 3, 5});
    ListNode* lb = createList({2, 4, 6});
    std::cout << "链表A: " << listToString(la) << "\n";
    std::cout << "链表B: " << listToString(lb) << "\n";
    ListNode* merged = mergeTwoLists(la, lb);
    std::cout << "合并后: " << listToString(merged) << "\n";
    deleteList(merged);
    
    // 5. 删除倒数第N个节点
    std::cout << "\n【删除倒数第N个节点】\n";
    ListNode* list5 = createList({1, 2, 3, 4, 5});
    std::cout << "原链表: " << listToString(list5) << "\n";
    list5 = removeNthFromEnd(list5, 2);
    std::cout << "删除倒数第2个: " << listToString(list5) << "\n";
    deleteList(list5);
}

} // namespace practice
