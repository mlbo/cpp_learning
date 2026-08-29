/**
 * @file test.cpp
 * @brief LeetCode 160: 相交链表 - 测试用例
 */

#include "solution.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using leetcode_0160::ListNode;
using leetcode_0160::Solution160;

namespace {

void deleteChain(ListNode* node) {
    while (node) {
        ListNode* next = node->next;
        delete node;
        node = next;
    }
}

}  // namespace

// 辅助函数：创建链表
ListNode* createList(const std::vector<int>& values) {
    if (values.empty()) return nullptr;

    std::unique_ptr<ListNode, void (*)(ListNode*)> owner(
        new ListNode(values[0]), &deleteChain);
    ListNode* curr = owner.get();
    for (std::size_t i = 1; i < values.size(); ++i) {
        auto node = std::make_unique<ListNode>(values[i]);
        curr->next = node.release();
        curr = curr->next;
    }
    return owner.release();
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
                              int skipA) {
    std::unique_ptr<ListNode, void (*)(ListNode*)> ownerA(
        createList(listA), &deleteChain);
    std::unique_ptr<ListNode, void (*)(ListNode*)> ownerB(
        createList(listB), &deleteChain);
    
    if (skipA < 0) {
        headA = ownerA.release();
        headB = ownerB.release();
        return;  // 不相交
    }
    
    // 找到交点
    ListNode* intersectA = ownerA.get();
    for (int i = 0; i < skipA && intersectA; ++i) {
        intersectA = intersectA->next;
    }
    
    // 让B链表的末尾指向交点
    ListNode* tailB = ownerB.get();
    while (tailB && tailB->next) {
        tailB = tailB->next;
    }
    
    if (tailB && intersectA) {
        tailB->next = intersectA;
    }

    headA = ownerA.release();
    headB = ownerB.release();
}

// 辅助函数：释放链表（释放到交点之前）
void deleteListUntil(ListNode* head, ListNode* stop) {
    while (head && head != stop) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

ListNode* findIntersectionByAddress(ListNode* headA, ListNode* headB) {
    ListNode* pa = headA;
    ListNode* pb = headB;
    while (pa != pb) {
        pa = pa ? pa->next : headB;
        pb = pb ? pb->next : headA;
    }
    return pa;
}

void deleteIntersectingLists(ListNode* headA, ListNode* headB) {
    ListNode* intersection = findIntersectionByAddress(headA, headB);
    deleteListUntil(headA, intersection);
    deleteListUntil(headB, intersection);
    deleteListUntil(intersection, nullptr);
}

// 测试用例
int runTests() {
    Solution160 solution;
    int failures = 0;
    auto expect = [&failures](bool condition, const char* message) {
        if (!condition) {
            ++failures;
            std::cerr << "  [失败] " << message << "\n";
        }
    };
    
    std::cout << "=== LeetCode 160: 相交链表 测试 ===\n\n";
    
    // 测试1：有交点
    {
        std::cout << "【测试1】有交点的情况\n";
        ListNode* headA = nullptr;
        ListNode* headB = nullptr;
        createIntersectingLists(headA, headB, {4, 1, 8, 4, 5}, {5, 6, 1}, 2);
        ListNode* expected = headA->next->next;
        
        std::cout << "链表A: 4 -> 1 -> 8 -> 4 -> 5\n";
        std::cout << "链表B: 5 -> 6 -> 1 -> 8 -> 4 -> 5\n";
        std::cout << "交点值: 8\n\n";
        
        ListNode* result = solution.getIntersectionNode(headA, headB);
        std::cout << "双指针法: ";
        if (result) {
            std::cout << "找到交点，值 = " << result->val << "\n";
            expect(result == expected, "双指针法必须返回同一节点地址，而不只是相同值");
        } else {
            std::cout << "未找到交点\n";
            expect(false, "双指针法未找到交点");
        }
        
        result = solution.getIntersectionNodeByLength(headA, headB);
        std::cout << "长度差法: ";
        if (result) {
            std::cout << "找到交点，值 = " << result->val << "\n";
            expect(result == expected, "长度差法返回的节点地址错误");
        } else {
            std::cout << "未找到交点\n";
            expect(false, "长度差法未找到交点");
        }
        
        result = solution.getIntersectionNodeByHash(headA, headB);
        std::cout << "哈希法: ";
        if (result) {
            std::cout << "找到交点，值 = " << result->val << "\n";
            expect(result == expected, "哈希法返回的节点地址错误");
        } else {
            std::cout << "未找到交点\n";
            expect(false, "哈希法未找到交点");
        }
        
        // 清理（避免公共尾部重复释放）
        deleteIntersectingLists(headA, headB);
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
        expect(result == nullptr, "无交点时双指针法应返回nullptr");
        
        result = solution.getIntersectionNodeByLength(headA, headB);
        std::cout << "长度差法: 无交点\n";
        expect(result == nullptr, "无交点时长度差法应返回nullptr");
        
        result = solution.getIntersectionNodeByHash(headA, headB);
        std::cout << "哈希法: 无交点\n";
        expect(result == nullptr, "无交点时哈希法应返回nullptr");
        
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
        expect(result == nullptr, "任一链表为空时应返回nullptr");
        
        deleteListUntil(headA, nullptr);
        std::cout << "\n";
    }
    
    // 测试4：两个独立前缀后共享整个尾部
    {
        std::cout << "【测试4】两个前缀后共享整个尾部\n";
        ListNode* common = createList({1, 2, 3});
        ListNode* headA = new ListNode(0, common);
        ListNode* headB = new ListNode(5, common);
        
        ListNode* result = solution.getIntersectionNode(headA, headB);
        std::cout << "找到交点，值 = " << (result ? result->val : -1) << "\n";
        expect(result == common, "公共尾部起点必须按节点地址识别");
        
        delete headA;
        delete headB;
        deleteListUntil(common, nullptr);
        std::cout << "\n";
    }
    
    if (failures == 0) {
        std::cout << "=== 所有测试通过! ===\n";
    }
    return failures;
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
    
    // 清理（避免公共尾部重复释放）
    deleteIntersectingLists(headA, headB);
}

} // namespace leetcode

// 主函数
int main() {
    return runTests() == 0 ? 0 : 1;
}
