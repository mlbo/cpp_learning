/**
 * @file list_tricks.cpp
 * @brief 链表技巧练习
 * 
 * 涵盖高级技巧：
 * 1. 链表分区（类似快排partition）
 * 2. 链表重排
 * 3. 回文链表判断
 * 4. 链表加法
 */

#include <iostream>
#include "list_node.h"

// ============================================
// 技巧一：链表分区（快排思想）
// ============================================

ListNode* partition(ListNode* head, int x) {
    // 使用两个虚拟头节点分别存储小于x和大于等于x的节点
    ListNode lessDummy(0);
    ListNode greaterDummy(0);
    ListNode* lessTail = &lessDummy;
    ListNode* greaterTail = &greaterDummy;
    
    while (head) {
        if (head->val < x) {
            lessTail->next = head;
            lessTail = lessTail->next;
        } else {
            greaterTail->next = head;
            greaterTail = greaterTail->next;
        }
        head = head->next;
    }
    
    // 连接两部分
    greaterTail->next = nullptr;  // 重要：断开尾部
    lessTail->next = greaterDummy.next;
    
    return lessDummy.next;
}

// ============================================
// 技巧二：链表重排
// L0 -> L1 -> L2 -> ... -> Ln
// L0 -> Ln -> L1 -> Ln-1 -> L2 -> Ln-2 -> ...
// ============================================

ListNode* reorderList(ListNode* head) {
    if (!head || !head->next) return head;
    
    // 1. 找中点
    ListNode* slow = head;
    ListNode* fast = head;
    while (fast->next && fast->next->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    
    // 2. 断开并反转后半部分
    ListNode* second = slow->next;
    slow->next = nullptr;
    
    // 反转后半部分
    ListNode* prev = nullptr;
    while (second) {
        ListNode* next = second->next;
        second->next = prev;
        prev = second;
        second = next;
    }
    second = prev;  // 反转后的头
    
    // 3. 交替合并
    ListNode* first = head;
    while (second) {
        ListNode* temp1 = first->next;
        ListNode* temp2 = second->next;
        first->next = second;
        second->next = temp1;
        first = temp1;
        second = temp2;
    }
    
    return head;
}

// ============================================
// 技巧三：判断回文链表
// ============================================

bool isPalindrome(ListNode* head) {
    if (!head || !head->next) return true;
    
    // 方法一：使用栈（O(n)空间）
    // 方法二：反转后半部分（O(1)空间）
    
    // 找中点
    ListNode* slow = head;
    ListNode* fast = head;
    while (fast->next && fast->next->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    
    // 反转后半部分
    ListNode* second = slow->next;
    ListNode* pprev = nullptr;
    while (second) {
        ListNode* next = second->next;
        second->next = pprev;
        pprev = second;
        second = next;
    }
    second = pprev;
    
    // 比较前后两部分
    ListNode* first = head;
    while (second) {
        if (first->val != second->val) {
            return false;
        }
        first = first->next;
        second = second->next;
    }
    
    return true;
}

// ============================================
// 技巧四：链表加法
// ============================================

ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
    ListNode dummy(0);
    ListNode* curr = &dummy;
    int carry = 0;
    
    while (l1 || l2 || carry) {
        int sum = carry;
        if (l1) {
            sum += l1->val;
            l1 = l1->next;
        }
        if (l2) {
            sum += l2->val;
            l2 = l2->next;
        }
        
        carry = sum / 10;
        curr->next = new ListNode(sum % 10);
        curr = curr->next;
    }
    
    return dummy.next;
}

// ============================================
// 技巧五：旋转链表
// ============================================

ListNode* rotateRight(ListNode* head, int k) {
    if (!head || !head->next || k == 0) return head;
    
    // 计算链表长度
    int length = 1;
    ListNode* tail = head;
    while (tail->next) {
        tail = tail->next;
        length++;
    }
    
    // 形成环
    tail->next = head;
    
    // 找到新的尾节点位置
    k = k % length;
    int stepsToNewTail = length - k;
    
    ListNode* newTail = head;
    for (int i = 1; i < stepsToNewTail; ++i) {
        newTail = newTail->next;
    }
    
    // 断开环
    ListNode* newHead = newTail->next;
    newTail->next = nullptr;
    
    return newHead;
}

// ============================================
// 技巧六：K个一组翻转链表
// ============================================

ListNode* reverseKGroup(ListNode* head, int k) {
    // 检查剩余节点是否足够k个
    ListNode* check = head;
    for (int i = 0; i < k; ++i) {
        if (!check) return head;  // 不足k个，不翻转
        check = check->next;
    }
    
    // 翻转k个节点
    ListNode* prevv = nullptr;
    ListNode* curr = head;
    for (int i = 0; i < k; ++i) {
        ListNode* next = curr->next;
        curr->next = prevv;
        prevv = curr;
        curr = next;
    }
    
    // 递归处理剩余部分
    head->next = reverseKGroup(curr, k);
    
    return prevv;  // prev是新的头节点
}

// ============================================
// 演示函数
// ============================================

namespace practice {

void demonstrateListTricks() {
    // 1. 链表分区
    std::cout << "【链表分区】按x=3分区\n";
    ListNode* list1 = createList({1, 4, 3, 2, 5, 2});
    std::cout << "原链表: " << listToString(list1) << "\n";
    list1 = partition(list1, 3);
    std::cout << "分区后: " << listToString(list1) << "\n";
    deleteList(list1);
    
    // 2. 链表重排
    std::cout << "\n【链表重排】\n";
    ListNode* list2 = createList({1, 2, 3, 4, 5});
    std::cout << "原链表: " << listToString(list2) << "\n";
    list2 = reorderList(list2);
    std::cout << "重排后: " << listToString(list2) << "\n";
    deleteList(list2);
    
    // 3. 回文判断
    std::cout << "\n【回文判断】\n";
    ListNode* list3 = createList({1, 2, 2, 1});
    std::cout << "链表: " << listToString(list3) << "\n";
    std::cout << "是否回文: " << (isPalindrome(list3) ? "是" : "否") << "\n";
    deleteList(list3);
    
    ListNode* list4 = createList({1, 2, 3, 2, 1});
    std::cout << "链表: " << listToString(list4) << "\n";
    std::cout << "是否回文: " << (isPalindrome(list4) ? "是" : "否") << "\n";
    deleteList(list4);
    
    // 4. 链表加法
    std::cout << "\n【链表加法】\n";
    ListNode* la = createList({2, 4, 3});  // 342
    ListNode* lb = createList({5, 6, 4});  // 465
    std::cout << "链表A: " << listToString(la) << " (342)\n";
    std::cout << "链表B: " << listToString(lb) << " (465)\n";
    ListNode* sum = addTwoNumbers(la, lb);
    std::cout << "相加: " << listToString(sum) << " (807)\n";
    deleteList(la);
    deleteList(lb);
    deleteList(sum);
    
    // 5. 旋转链表
    std::cout << "\n【旋转链表】\n";
    ListNode* list5 = createList({1, 2, 3, 4, 5});
    std::cout << "原链表: " << listToString(list5) << "\n";
    list5 = rotateRight(list5, 2);
    std::cout << "旋转2位: " << listToString(list5) << "\n";
    deleteList(list5);
    
    // 6. K个一组翻转
    std::cout << "\n【K个一组翻转】\n";
    ListNode* list6 = createList({1, 2, 3, 4, 5});
    std::cout << "原链表: " << listToString(list6) << "\n";
    list6 = reverseKGroup(list6, 2);
    std::cout << "2个一组翻转: " << listToString(list6) << "\n";
    deleteList(list6);
}

} // namespace practice
