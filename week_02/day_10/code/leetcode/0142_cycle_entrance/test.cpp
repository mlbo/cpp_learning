/**
 * @file test.cpp
 * @brief LeetCode 142 测试用例
 */

#include "solution.h"
#include <iostream>
#include <memory>
#include <unordered_set>
#include <vector>

using leetcode_0142::ListNode;
using leetcode_0142::Solution;

namespace test_142 {

// 创建链表并形成环
static ListNode* createCycleList(const std::vector<int>& values, int pos) {
    if (values.empty()) return nullptr;
    
    // 临时所有者先接住全部节点；任一分配失败时，已创建节点会自动释放。
    std::vector<std::unique_ptr<ListNode>> owners;
    owners.reserve(values.size());
    for (int val : values) {
        owners.push_back(std::make_unique<ListNode>(val));
    }
    
    // 连接节点
    for (std::size_t i = 0; i + 1 < owners.size(); ++i) {
        owners[i]->next = owners[i + 1].get();
    }
    
    // 如果 pos >= 0，形成环
    if (pos >= 0 && pos < static_cast<int>(owners.size())) {
        owners.back()->next = owners[static_cast<std::size_t>(pos)].get();
    }

    ListNode* head = owners.front().get();
    for (auto& owner : owners) {
        (void)owner.release();
    }
    return head;
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

// 释放链表：用访问集合同时兼容有环和无环输入。
static void freeCycleList(ListNode *head, int cyclePos) {
    (void)cyclePos;
    std::unordered_set<ListNode*> visited;
    while (head != nullptr && visited.insert(head).second) {
        ListNode *next = head->next;
        delete head;
        head = next;
    }
}

static bool runTest(const std::string& name,
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
    
    const bool passed = actual == expected;
    if (passed) {
        std::cout << "    ✅ 通过\n";
    } else {
        std::cout << "    ❌ 失败\n";
    }
    
    freeCycleList(head, pos);
    std::cout << "\n";
    return passed;
}

} // namespace test_142

int main() {
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
    bool allPassed = true;
    allPassed = test_142::runTest("标准有环链表", {3, 2, 0, -4}, 1, 1) && allPassed;
    
    // 测试用例2：环在头部
    allPassed = test_142::runTest("环在头部", {1, 2}, 0, 0) && allPassed;
    
    // 测试用例3：无环
    allPassed = test_142::runTest("无环链表", {1, 2, 3, 4}, -1, -1) && allPassed;
    
    // 测试用例4：单节点无环
    allPassed = test_142::runTest("单节点无环", {1}, -1, -1) && allPassed;
    
    // 测试用例5：单节点自环
    allPassed = test_142::runTest("单节点自环", {1}, 0, 0) && allPassed;
    
    // 测试用例6：空链表
    allPassed = test_142::runTest("空链表", {}, -1, -1) && allPassed;
    
    // 测试用例7：大环
    allPassed = test_142::runTest("大环", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 5, 5) && allPassed;
    
    std::cout << "-------------------- 测试完成 --------------------\n";
    
    // 演示哈希表方法
    std::cout << "\n【哈希表方法对比】\n";
    std::cout << "  快慢指针: O(1) 空间\n";
    std::cout << "  哈希表:   O(n) 空间\n";
    std::cout << "  两者时间复杂度都是 O(n)\n";

    std::cout << (allPassed ? "\nLeetCode 142 全部测试通过\n"
                            : "\nLeetCode 142 存在失败用例\n");
    return allPassed ? 0 : 1;
}
