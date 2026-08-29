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
#include <memory>
#include <unordered_set>

namespace leetcode_0141 {

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

    const auto delete_chain = [this](ListNode* node) { deleteList(node); };
    std::unique_ptr<ListNode, decltype(delete_chain)> owner(
        new ListNode(vals[0]), delete_chain);
    ListNode* cur = owner.get();
    for (std::size_t i = 1; i < vals.size(); ++i) {
        auto node = std::make_unique<ListNode>(vals[i]);
        cur->next = node.release();
        cur = cur->next;
    }
    return owner.release();
}

ListNode* Solution::createListWithCycle(const std::vector<int>& vals, int pos) {
    ListNode* head = createList(vals);
    if (!head) return nullptr;

    ListNode* cur = head;
    while (cur->next) {
        cur = cur->next;
    }
    
    // 创建环：pos 是环入口的索引（从0开始）
    if (pos >= 0 && static_cast<std::size_t>(pos) < vals.size()) {
        ListNode* cycleEntry = head;
        for (int i = 0; i < pos; ++i) {
            cycleEntry = cycleEntry->next;
        }
        cur->next = cycleEntry;  // 尾节点指向环入口
    }
    
    return head;
}

void Solution::deleteList(ListNode* head) {
    std::unordered_set<ListNode*> visited;
    while (head && visited.insert(head).second) {
        ListNode* next_node = head->next;
        delete head;
        head = next_node;
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
    sol.deleteList(list2);
    
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
    sol.deleteList(list5);
    
    std::cout << "\n算法分析：\n";
    std::cout << "  • 时间复杂度：O(n)，最坏情况遍历所有节点\n";
    std::cout << "  • 空间复杂度：O(1)，只使用两个指针\n";
    std::cout << "  • 原理：如果有环，快指针最终会追上慢指针\n";
}

} // namespace leetcode_0141
