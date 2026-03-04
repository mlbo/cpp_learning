/**
 * @file solution.cpp
 * @brief LeetCode 141: 环形链表 - 实现
 * 
 * 给定一个链表，判断链表中是否有环。
 * 如果链表中存在环，则返回 true 。 否则，返回 false 。
 * 
 * 进阶：能用 O(1)（即常量）内存解决此问题吗？
 */

#include "solution.h"
#include <iostream>

// ============================================================
// 解法：快慢指针（Floyd 判圈算法）
// ============================================================

bool Solution::hasCycle(ListNode* head) {
    if (!head || !head->next) {
        return false;
    }
    
    ListNode* slow = head;
    ListNode* fast = head;
    
    while (fast && fast->next) {
        slow = slow->next;       // 慢指针走1步
        fast = fast->next->next; // 快指针走2步
        
        if (slow == fast) {
            return true;  // 相遇，有环
        }
    }
    
    return false;  // 快指针到达末尾，无环
}

// ============================================================
// 辅助函数实现
// ============================================================

ListNode* Solution::createList(const std::vector<int>& vals) {
    if (vals.empty()) return nullptr;
    
    ListNode* head = new ListNode(vals[0]);
    ListNode* cur = head;
    for (size_t i = 1; i < vals.size(); ++i) {
        cur->next = new ListNode(vals[i]);
        cur = cur->next;
    }
    return head;
}

ListNode* Solution::createListWithCycle(const std::vector<int>& vals, int pos) {
    if (vals.empty()) return nullptr;
    
    ListNode* head = new ListNode(vals[0]);
    ListNode* cur = head;
    
    for (size_t i = 1; i < vals.size(); ++i) {
        cur->next = new ListNode(vals[i]);
        cur = cur->next;
    }
    
    // 创建环：pos 是环入口的索引（从0开始）
    if (pos >= 0 && pos < static_cast<int>(vals.size())) {
        ListNode* cycleEntry = head;
        for (int i = 0; i < pos; ++i) {
            cycleEntry = cycleEntry->next;
        }
        cur->next = cycleEntry;  // 尾节点指向环入口
    }
    
    return head;
}

void Solution::deleteList(ListNode* head) {
    // 注意：对于有环链表，这个函数会导致无限循环
    // 这里假设链表无环
    while (head) {
        ListNode* tmp = head;
        head = head->next;
        delete tmp;
    }
}

// ============================================================
// 测试函数
// ============================================================

void testLinkedListCycle() {
    Solution sol;
    
    std::cout << "【测试用例1：无环链表】\n";
    ListNode* list1 = sol.createList({1, 2, 3, 4, 5});
    std::cout << "  链表: 1 -> 2 -> 3 -> 4 -> 5 -> nullptr\n";
    std::cout << "  是否有环: " << (sol.hasCycle(list1) ? "是" : "否") << "\n";
    sol.deleteList(list1);
    
    std::cout << "\n【测试用例2：有环链表】\n";
    ListNode* list2 = sol.createListWithCycle({3, 2, 0, -4}, 1);
    std::cout << "  链表: 3 -> 2 -> 0 -> -4 -> 2(环入口)\n";
    std::cout << "  是否有环: " << (sol.hasCycle(list2) ? "是" : "否") << "\n";
    // 注意：list2 有环，不能用 deleteList 清理
    
    std::cout << "\n【测试用例3：单节点无环】\n";
    ListNode* list3 = sol.createList({1});
    std::cout << "  链表: 1 -> nullptr\n";
    std::cout << "  是否有环: " << (sol.hasCycle(list3) ? "是" : "否") << "\n";
    sol.deleteList(list3);
    
    std::cout << "\n【测试用例4：空链表】\n";
    std::cout << "  链表: nullptr\n";
    std::cout << "  是否有环: " << (sol.hasCycle(nullptr) ? "是" : "否") << "\n";
    
    std::cout << "\n【测试用例5：头节点自环】\n";
    ListNode* list5 = sol.createListWithCycle({1}, 0);
    std::cout << "  链表: 1 -> 1(自环)\n";
    std::cout << "  是否有环: " << (sol.hasCycle(list5) ? "是" : "否") << "\n";
    
    std::cout << "\n算法分析：\n";
    std::cout << "  • 时间复杂度：O(n)，最坏情况遍历所有节点\n";
    std::cout << "  • 空间复杂度：O(1)，只使用两个指针\n";
    std::cout << "  • 原理：如果有环，快指针最终会追上慢指针\n";
}
