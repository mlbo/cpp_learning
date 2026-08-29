/**
 * @file solution.cpp
 * @brief LeetCode 142: 环形链表 II 实现
 * 
 * 给定链表的头节点 head，返回链表开始入环的第一个节点。
 * 如果链表无环，则返回 null。
 * 
 * 难度：中等
 * 标签：链表、双指针、快慢指针
 */

#include "solution.h"

#include <iostream>
#include <memory>
#include <unordered_set>
#include <vector>

namespace leetcode_0142 {

ListNode *Solution::detectCycle(ListNode *head) {
    if (head == nullptr || head->next == nullptr) {
        return nullptr;
    }

    ListNode *slow = head;
    ListNode *fast = head;

    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast) {
            ListNode *fromHead = head;
            ListNode *fromMeeting = slow;
            while (fromHead != fromMeeting) {
                fromHead = fromHead->next;
                fromMeeting = fromMeeting->next;
            }
            return fromHead;
        }
    }

    return nullptr;
}

ListNode *Solution::detectCycleHash(ListNode *head) {
    std::unordered_set<ListNode*> visited;
    ListNode *current = head;

    while (current != nullptr) {
        if (!visited.insert(current).second) {
            return current;
        }
        current = current->next;
    }

    return nullptr;
}

// ============================================
// 测试辅助函数
// ============================================
namespace {

// 简单测试：只创建无环链表用于测试
ListNode* createList(const std::vector<int>& values) {
    if (values.empty()) return nullptr;

    const auto delete_chain = [](ListNode* node) {
        while (node) {
            ListNode* next_node = node->next;
            delete node;
            node = next_node;
        }
    };
    std::unique_ptr<ListNode, decltype(delete_chain)> owner(
        new ListNode(values[0]), delete_chain);
    ListNode *curr = owner.get();

    for (std::size_t i = 1; i < values.size(); ++i) {
        auto node = std::make_unique<ListNode>(values[i]);
        curr->next = node.release();
        curr = curr->next;
    }

    return owner.release();
}

void freeList(ListNode* head) {
    while (head != nullptr) {
        ListNode *temp = head;
        head = head->next;
        delete temp;
    }
}

bool runTest(const std::string& name,
             const std::vector<int>& values, 
             int pos, 
             int expected) {
    std::cout << "  测试: " << name << "\n";
    
    ListNode *head = createList(values);
    
    Solution sol;
    ListNode *entrance = sol.detectCycle(head);
    
    // 无环链表的测试
    int actual = (entrance != nullptr) ? -2 : -1;  // -2 表示找到环(不应该), -1 表示无环
    
    std::cout << "    输入: 链表 [";
    for (size_t i = 0; i < values.size(); ++i) {
        std::cout << values[i];
        if (i < values.size() - 1) std::cout << "->";
    }
    std::cout << "], pos = " << pos << "\n";
    
    std::cout << "    期望入口索引: " << expected << "\n";
    std::cout << "    实际入口索引: " << actual << "\n";
    
    if (actual == expected) {
        std::cout << "    ✅ 通过\n";
    } else {
        std::cout << "    ❌ 失败\n";
    }
    
    freeList(head);
    std::cout << "\n";
    return actual == expected;
}

} // anonymous namespace

// ============================================
// 主演示函数
// ============================================
bool test_leetcode_142() {
    std::cout << "\n【LeetCode 142: 环形链表 II】\n";
    std::cout << "\n快慢指针数学推导:\n";
    std::cout << "  设 a = 头到入环点距离\n";
    std::cout << "  设 b = 入环点到相遇点距离\n";
    std::cout << "  设 c = 相遇点到入环点距离\n";
    std::cout << "  慢指针: a + b\n";
    std::cout << "  快指针: a + n(b+c) + b\n";
    std::cout << "  推导: a = (n-1)(b+c) + c\n";
    std::cout << "  结论: 从相遇点和头同时出发，必在入环点相遇\n";
    
    std::cout << "\n-------------------- 测试用例 (无环链表) --------------------\n";
    
    // 测试无环链表用例
    bool all_passed = true;
    all_passed = runTest("无环链表", {1, 2, 3, 4}, -1, -1) && all_passed;
    all_passed = runTest("单节点无环", {1}, -1, -1) && all_passed;
    all_passed = runTest("空链表", {}, -1, -1) && all_passed;
    
    std::cout << "-------------------- 测试完成 --------------------\n";
    
    // 构造有环链表演示；使用后会先打断环，再释放全部节点。
    std::cout << "\n【有环链表示例】\n";
    ListNode* n1 = new ListNode(3);
    ListNode* n2 = new ListNode(2);
    ListNode* n3 = new ListNode(0);
    ListNode* n4 = new ListNode(-4);
    n1->next = n2;
    n2->next = n3;
    n3->next = n4;
    n4->next = n2;  // 形成环
    
    Solution sol;
    ListNode* entrance = sol.detectCycle(n1);
    if (entrance == n2 && sol.detectCycleHash(n1) == n2) {
        std::cout << "  链表 [3->2->0->-4], 环在索引1 (节点值为2)\n";
        std::cout << "  检测到环入口值: " << entrance->val << "\n";
        std::cout << "  ✅ 环检测正确\n";
    } else {
        std::cout << "  ❌ 环入口检测错误\n";
        all_passed = false;
    }
    
    // 打断环后释放
    n4->next = nullptr;
    freeList(n1);
    
    std::cout << "\n【方法对比】\n";
    std::cout << "  快慢指针: O(1) 空间，O(n) 时间\n";
    std::cout << "  哈希表:   O(n) 空间，O(n) 时间\n";
    return all_passed;
}

} // namespace leetcode_0142
