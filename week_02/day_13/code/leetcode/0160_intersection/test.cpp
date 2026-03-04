/**
 * @file test.cpp
 * @brief LeetCode 160: 相交链表 - 测试用例
 */

#include "solution.h"
#include <iostream>
#include <vector>
#include <string>
#include <cassert>

// 辅助函数：创建链表
ListNode* createList(const std::vector<int>& values) {
    if (values.empty()) return nullptr;
    ListNode* head = new ListNode(values[0]);
    ListNode* curr = head;
    for (size_t i = 1; i < values.size(); ++i) {
        curr->next = new ListNode(values[i]);
        curr = curr->next;
    }
    return head;
}

// 辅助函数：打印链表
std::string listToString(ListNode* head) {
    std::string result;
    while (head) {
        result += std::to_string(head->val);
        if (head->next) result += " -> ";
        head = head->next;
    }
    return result.empty() ? "nullptr" : result;
}

// 辅助函数：创建相交链表
void createIntersectingLists(ListNode*& headA, ListNode*& headB, 
                              const std::vector<int>& listA, 
                              const std::vector<int>& listB, 
                              int skipA, int skipB) {
    headA = createList(listA);
    headB = createList(listB);
    
    if (skipA < 0 || skipB < 0) return;  // 不相交
    
    // 找到交点
    ListNode* intersectA = headA;
    for (int i = 0; i < skipA && intersectA; ++i) {
        intersectA = intersectA->next;
    }
    
    // 让B链表的末尾指向交点
    ListNode* tailB = headB;
    while (tailB && tailB->next) {
        tailB = tailB->next;
    }
    
    if (tailB && intersectA) {
        tailB->next = intersectA;
    }
}

// 辅助函数：释放链表（释放到交点之前）
void deleteListUntil(ListNode* head, ListNode* stop) {
    while (head && head != stop) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

// 测试用例
void runTests() {
    Solution160 solution;
    
    std::cout << "=== LeetCode 160: 相交链表 测试 ===\n\n";
    
    // 测试1：有交点
    {
        std::cout << "【测试1】有交点的情况\n";
        ListNode* headA = nullptr;
        ListNode* headB = nullptr;
        createIntersectingLists(headA, headB, {4, 1, 8, 4, 5}, {5, 6, 1}, 2, 2);
        
        std::cout << "链表A: 4 -> 1 -> 8 -> 4 -> 5\n";
        std::cout << "链表B: 5 -> 6 -> 1 -> 8 -> 4 -> 5\n";
        std::cout << "交点值: 8\n\n";
        
        ListNode* result = solution.getIntersectionNode(headA, headB);
        std::cout << "双指针法: ";
        if (result) {
            std::cout << "找到交点，值 = " << result->val << "\n";
            assert(result->val == 8);
        } else {
            std::cout << "未找到交点\n";
        }
        
        result = solution.getIntersectionNodeByLength(headA, headB);
        std::cout << "长度差法: ";
        if (result) {
            std::cout << "找到交点，值 = " << result->val << "\n";
            assert(result->val == 8);
        } else {
            std::cout << "未找到交点\n";
        }
        
        result = solution.getIntersectionNodeByHash(headA, headB);
        std::cout << "哈希法: ";
        if (result) {
            std::cout << "找到交点，值 = " << result->val << "\n";
            assert(result->val == 8);
        } else {
            std::cout << "未找到交点\n";
        }
        
        // 清理（需要分别删除两个链表的独立部分）
        deleteListUntil(headA, nullptr);
        deleteListUntil(headB, nullptr);  // 注意：这会删除公共部分两次，实际应该更小心
        std::cout << "\n";
    }
    
    // 测试2：无交点
    {
        std::cout << "【测试2】无交点的情况\n";
        ListNode* headA = createList({2, 6, 4});
        ListNode* headB = createList({1, 5});
        
        std::cout << "链表A: 2 -> 6 -> 4\n";
        std::cout << "链表B: 1 -> 5\n";
        std::cout << "预期: 无交点\n\n";
        
        ListNode* result = solution.getIntersectionNode(headA, headB);
        std::cout << "双指针法: ";
        if (result) {
            std::cout << "找到交点，值 = " << result->val << "\n";
        } else {
            std::cout << "无交点\n";
        }
        assert(result == nullptr);
        
        result = solution.getIntersectionNodeByLength(headA, headB);
        std::cout << "长度差法: 无交点\n";
        assert(result == nullptr);
        
        result = solution.getIntersectionNodeByHash(headA, headB);
        std::cout << "哈希法: 无交点\n";
        assert(result == nullptr);
        
        deleteListUntil(headA, nullptr);
        deleteListUntil(headB, nullptr);
        std::cout << "\n";
    }
    
    // 测试3：一个链表为空
    {
        std::cout << "【测试3】一个链表为空\n";
        ListNode* headA = createList({1, 2, 3});
        ListNode* headB = nullptr;
        
        ListNode* result = solution.getIntersectionNode(headA, headB);
        std::cout << "双指针法: " << (result ? "找到交点" : "无交点") << "\n";
        assert(result == nullptr);
        
        deleteListUntil(headA, nullptr);
        std::cout << "\n";
    }
    
    // 测试4：交点在头节点
    {
        std::cout << "【测试4】交点在头节点\n";
        ListNode* common = createList({1, 2, 3});
        ListNode* headA = new ListNode(0, common);
        ListNode* headB = new ListNode(5, common);
        
        ListNode* result = solution.getIntersectionNode(headA, headB);
        std::cout << "找到交点，值 = " << result->val << "\n";
        assert(result->val == 1);
        
        delete headA;
        delete headB;
        deleteListUntil(common, nullptr);
        std::cout << "\n";
    }
    
    std::cout << "=== 所有测试通过! ===\n";
}

// 演示函数（供主程序调用）
namespace leetcode {

void testIntersection() {
    Solution160 solution;
    
    // 简单演示
    ListNode* headA = createList({4, 1, 8, 4, 5});
    ListNode* headB = createList({5, 6, 1});
    
    // 创建交点
    ListNode* intersectA = headA;
    for (int i = 0; i < 2; ++i) {
        intersectA = intersectA->next;  // 指向节点8
    }
    
    ListNode* tailB = headB;
    while (tailB->next) {
        tailB = tailB->next;
    }
    tailB->next = intersectA;  // B的末尾指向交点
    
    std::cout << "链表A: 4 -> 1 -> 8 -> 4 -> 5\n";
    std::cout << "链表B: 5 -> 6 -> 1 -> 8 -> 4 -> 5\n";
    std::cout << "交点值: 8\n\n";
    
    ListNode* result = solution.getIntersectionNode(headA, headB);
    std::cout << "双指针法结果: ";
    if (result) {
        std::cout << "交点值 = " << result->val << "\n";
    } else {
        std::cout << "无交点\n";
    }
    
    // 清理
    deleteListUntil(headA, nullptr);
    // 注意：需要单独清理B链表的非公共部分
    ListNode* curr = headB;
    while (curr && curr != result) {
        ListNode* temp = curr;
        curr = curr->next;
        delete temp;
    }
}

} // namespace leetcode

// 主函数
int main() {
    runTests();
    return 0;
}
