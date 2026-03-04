/**
 * @file test.cpp
 * @brief LeetCode 21 测试代码
 */

#include "solution.cpp"
#include <iostream>
#include <vector>
#include <cassert>

// 辅助函数：从数组创建链表
ListNode* createList(const std::vector<int>& vals) {
    if (vals.empty()) return nullptr;
    
    ListNode* head = new ListNode(vals[0]);
    ListNode* cur = head;
    for (size_t i = 1; i < vals.size(); ++i) {
        cur->next = new ListNode(vals[i]);
        cur = cur->next;
    }
    return head;
}

// 辅助函数：链表转数组
std::vector<int> listToArray(ListNode* head) {
    std::vector<int> result;
    while (head) {
        result.push_back(head->val);
        head = head->next;
    }
    return result;
}

// 辅助函数：打印链表
void printList(ListNode* head) {
    std::cout << "[";
    while (head) {
        std::cout << head->val;
        if (head->next) std::cout << ", ";
        head = head->next;
    }
    std::cout << "]";
}

// 辅助函数：释放链表
void deleteList(ListNode* head) {
    while (head) {
        ListNode* tmp = head;
        head = head->next;
        delete tmp;
    }
}

void testMergeLists() {
    Solution sol;
    
    std::cout << "【测试用例】\n\n";
    
    // 测试1：正常情况
    {
        std::cout << "测试1：正常情况\n";
        ListNode* l1 = createList({1, 2, 4});
        ListNode* l2 = createList({1, 3, 4});
        
        std::cout << "  l1: "; printList(l1); std::cout << "\n";
        std::cout << "  l2: "; printList(l2); std::cout << "\n";
        
        ListNode* merged = sol.mergeTwoLists(l1, l2);
        std::vector<int> expected = {1, 1, 2, 3, 4, 4};
        std::vector<int> result = listToArray(merged);
        
        std::cout << "  合并结果: "; printList(merged); std::cout << "\n";
        std::cout << "  期望结果: [1, 1, 2, 3, 4, 4]\n";
        
        assert(result == expected);
        std::cout << "  ✓ 通过\n";
        
        deleteList(merged);
    }
    
    // 测试2：一个空链表
    {
        std::cout << "\n测试2：一个空链表\n";
        ListNode* l1 = nullptr;
        ListNode* l2 = createList({1, 3, 5});
        
        ListNode* merged = sol.mergeTwoLists(l1, l2);
        std::vector<int> expected = {1, 3, 5};
        std::vector<int> result = listToArray(merged);
        
        std::cout << "  l1: []\n";
        std::cout << "  l2: [1, 3, 5]\n";
        std::cout << "  合并结果: "; printList(merged); std::cout << "\n";
        
        assert(result == expected);
        std::cout << "  ✓ 通过\n";
        
        deleteList(merged);
    }
    
    // 测试3：两个空链表
    {
        std::cout << "\n测试3：两个空链表\n";
        ListNode* merged = sol.mergeTwoLists(nullptr, nullptr);
        
        std::cout << "  l1: []\n";
        std::cout << "  l2: []\n";
        std::cout << "  合并结果: []\n";
        
        assert(merged == nullptr);
        std::cout << "  ✓ 通过\n";
    }
    
    // 测试4：不同长度
    {
        std::cout << "\n测试4：不同长度\n";
        ListNode* l1 = createList({1, 3, 5, 7, 9});
        ListNode* l2 = createList({2, 4});
        
        std::cout << "  l1: "; printList(l1); std::cout << "\n";
        std::cout << "  l2: "; printList(l2); std::cout << "\n";
        
        ListNode* merged = sol.mergeTwoLists(l1, l2);
        std::vector<int> expected = {1, 2, 3, 4, 5, 7, 9};
        std::vector<int> result = listToArray(merged);
        
        std::cout << "  合并结果: "; printList(merged); std::cout << "\n";
        
        assert(result == expected);
        std::cout << "  ✓ 通过\n";
        
        deleteList(merged);
    }
    
    // 测试5：比较递归和迭代解法
    {
        std::cout << "\n测试5：比较递归和迭代解法\n";
        ListNode* l1 = createList({1, 2, 4});
        ListNode* l2 = createList({1, 3, 4});
        
        ListNode* l1_copy = createList({1, 2, 4});
        ListNode* l2_copy = createList({1, 3, 4});
        
        ListNode* resultIter = sol.mergeTwoListsIterative(l1, l2);
        ListNode* resultRecur = sol.mergeTwoListsRecursive(l1_copy, l2_copy);
        
        std::vector<int> iterArray = listToArray(resultIter);
        std::vector<int> recurArray = listToArray(resultRecur);
        
        std::cout << "  迭代结果: "; printList(resultIter); std::cout << "\n";
        std::cout << "  递归结果: "; printList(resultRecur); std::cout << "\n";
        
        assert(iterArray == recurArray);
        std::cout << "  ✓ 两种方法结果一致\n";
        
        deleteList(resultIter);
        deleteList(resultRecur);
    }
    
    std::cout << "\n所有测试通过！\n";
    
    std::cout << "\n【算法分析】\n";
    std::cout << "┌─────────────────────────────────────────────────────┐\n";
    std::cout << "│           迭代解法      │     递归解法             │\n";
    std::cout << "├─────────────────────────────────────────────────────┤\n";
    std::cout << "│ 时间复杂度: O(n+m)     │ 时间复杂度: O(n+m)       │\n";
    std::cout << "│ 空间复杂度: O(1)       │ 空间复杂度: O(n+m)       │\n";
    std::cout << "│ 优点: 空间效率高       │ 优点: 代码简洁           │\n";
    std::cout << "│ 缺点: 代码稍长         │ 缺点: 递归栈开销         │\n";
    std::cout << "└─────────────────────────────────────────────────────┘\n";
}

void testMergeListsMain() {
    std::cout << "╔══════════════════════════════════════════════════════════╗\n";
    std::cout << "║        LeetCode 21: 合并两个有序链表                      ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════╝\n\n";
    
    testMergeLists();
}
