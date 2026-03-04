/**
 * @file test.cpp
 * @brief LeetCode 142 测试用例
 */

#include "solution.h"
#include <iostream>
#include <vector>

namespace test_142 {

// 创建链表并形成环
static ListNode* createCycleList(const std::vector<int>& values, int pos) {
    if (values.empty()) return nullptr;
    
    // 创建所有节点
    std::vector<ListNode*> nodes;
    for (int val : values) {
        nodes.push_back(new ListNode(val));
    }
    
    // 连接节点
    for (size_t i = 0; i < nodes.size() - 1; ++i) {
        nodes[i]->next = nodes[i + 1];
    }
    
    // 如果 pos >= 0，形成环
    if (pos >= 0 && pos < static_cast<int>(nodes.size())) {
        nodes.back()->next = nodes[pos];
    }
    
    return nodes[0];
}

// 获取节点索引（用于验证）
static int getNodeIndex(ListNode *head, ListNode *target) {
    ListNode *curr = head;
    int index = 0;
    while (curr != nullptr && curr != target) {
        curr = curr->next;
        ++index;
        if (index > 100) break;  // 防止无限循环
    }
    return (curr == target) ? index : -1;
}

// 释放链表（无环）
static void freeList_142(ListNode *head) {
    while (head != nullptr) {
        ListNode *temp = head;
        head = head->next;
        delete temp;
    }
}

// 释放有环链表
static void freeCycleList(ListNode *head, int cyclePos) {
    if (head == nullptr) return;
    
    // 先打断环
    if (cyclePos >= 0) {
        ListNode *curr = head;
        int count = 0;
        ListNode *cycleNode = nullptr;
        
        while (curr->next != nullptr) {
            if (count == cyclePos) {
                cycleNode = curr;
            }
            curr = curr->next;
            ++count;
            if (count > 100) break;  // 防止无限循环
        }
        
        if (cycleNode != nullptr && curr->next == cycleNode) {
            curr->next = nullptr;  // 打断环
        }
    }
    
    freeList_142(head);
}

static void runTest(const std::string& name, 
             const std::vector<int>& values, 
             int pos, 
             int expected) {
    std::cout << "  测试: " << name << "\n";
    
    ListNode *head = createCycleList(values, pos);
    
    Solution sol;
    ListNode *entrance = sol.detectCycle(head);
    
    int actual = (entrance != nullptr) ? getNodeIndex(head, entrance) : -1;
    
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
    
    freeCycleList(head, pos);
    std::cout << "\n";
}

} // namespace test_142

void test_leetcode_142() {
    using namespace test_142;
    
    std::cout << "\n【LeetCode 142: 环形链表 II】\n";
    std::cout << "\n快慢指针数学推导:\n";
    std::cout << "  设 a = 头到入环点距离\n";
    std::cout << "  设 b = 入环点到相遇点距离\n";
    std::cout << "  设 c = 相遇点到入环点距离\n";
    std::cout << "  慢指针: a + b\n";
    std::cout << "  快指针: a + n(b+c) + b\n";
    std::cout << "  推导: a = (n-1)(b+c) + c\n";
    std::cout << "  结论: 从相遇点和头同时出发，必在入环点相遇\n";
    
    std::cout << "\n-------------------- 测试用例 --------------------\n";
    
    // 测试用例1：标准有环链表
    runTest("标准有环链表", {3, 2, 0, -4}, 1, 1);
    
    // 测试用例2：环在头部
    runTest("环在头部", {1, 2}, 0, 0);
    
    // 测试用例3：无环
    runTest("无环链表", {1, 2, 3, 4}, -1, -1);
    
    // 测试用例4：单节点无环
    runTest("单节点无环", {1}, -1, -1);
    
    // 测试用例5：单节点自环
    runTest("单节点自环", {1}, 0, 0);
    
    // 测试用例6：空链表
    runTest("空链表", {}, -1, -1);
    
    // 测试用例7：大环
    runTest("大环", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 5, 5);
    
    std::cout << "-------------------- 测试完成 --------------------\n";
    
    // 演示哈希表方法
    std::cout << "\n【哈希表方法对比】\n";
    std::cout << "  快慢指针: O(1) 空间\n";
    std::cout << "  哈希表:   O(n) 空间\n";
    std::cout << "  两者时间复杂度都是 O(n)\n";
}
