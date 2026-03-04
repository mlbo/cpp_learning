/**
 * @file test.cpp
 * @brief LeetCode 148: 排序链表 - 测试用例
 */

#include "solution.h"
#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <algorithm>

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
std::string listToString(ListNode* head) {
    std::string result;
    while (head) {
        result += std::to_string(head->val);
        if (head->next) result += " -> ";
        head = head->next;
    }
    return result.empty() ? "nullptr" : result;
}

// 辅助函数：释放链表
void deleteList(ListNode* head) {
    while (head) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

// 验证链表是否有序
bool isSorted(ListNode* head) {
    while (head && head->next) {
        if (head->val > head->next->val) {
            return false;
        }
        head = head->next;
    }
    return true;
}

// 测试用例
void runTests() {
    Solution148 solution;
    
    std::cout << "=== LeetCode 148: 排序链表 测试 ===\n\n";
    
    // 测试1：普通乱序链表
    {
        std::cout << "【测试1】普通乱序链表\n";
        ListNode* head = createList({4, 2, 1, 3});
        std::cout << "原链表: " << listToString(head) << "\n";
        
        ListNode* sorted = solution.sortList(head);
        std::cout << "排序后: " << listToString(sorted) << "\n";
        
        assert(isSorted(sorted));
        std::vector<int> expected = {1, 2, 3, 4};
        assert(listToArray(sorted) == expected);
        
        deleteList(sorted);
        std::cout << "测试通过!\n\n";
    }
    
    // 测试2：较长的乱序链表
    {
        std::cout << "【测试2】较长的乱序链表\n";
        ListNode* head = createList({-1, 5, 3, 4, 0});
        std::cout << "原链表: " << listToString(head) << "\n";
        
        ListNode* sorted = solution.sortList(head);
        std::cout << "排序后: " << listToString(sorted) << "\n";
        
        assert(isSorted(sorted));
        std::vector<int> expected = {-1, 0, 3, 4, 5};
        assert(listToArray(sorted) == expected);
        
        deleteList(sorted);
        std::cout << "测试通过!\n\n";
    }
    
    // 测试3：空链表
    {
        std::cout << "【测试3】空链表\n";
        ListNode* head = nullptr;
        ListNode* sorted = solution.sortList(head);
        assert(sorted == nullptr);
        std::cout << "排序后: nullptr\n";
        std::cout << "测试通过!\n\n";
    }
    
    // 测试4：单节点链表
    {
        std::cout << "【测试4】单节点链表\n";
        ListNode* head = createList({1});
        std::cout << "原链表: " << listToString(head) << "\n";
        
        ListNode* sorted = solution.sortList(head);
        std::cout << "排序后: " << listToString(sorted) << "\n";
        
        assert(isSorted(sorted));
        assert(sorted->val == 1);
        
        deleteList(sorted);
        std::cout << "测试通过!\n\n";
    }
    
    // 测试5：已排序链表
    {
        std::cout << "【测试5】已排序链表\n";
        ListNode* head = createList({1, 2, 3, 4, 5});
        std::cout << "原链表: " << listToString(head) << "\n";
        
        ListNode* sorted = solution.sortList(head);
        std::cout << "排序后: " << listToString(sorted) << "\n";
        
        assert(isSorted(sorted));
        
        deleteList(sorted);
        std::cout << "测试通过!\n\n";
    }
    
    // 测试6：逆序链表
    {
        std::cout << "【测试6】逆序链表\n";
        ListNode* head = createList({5, 4, 3, 2, 1});
        std::cout << "原链表: " << listToString(head) << "\n";
        
        ListNode* sorted = solution.sortList(head);
        std::cout << "排序后: " << listToString(sorted) << "\n";
        
        assert(isSorted(sorted));
        
        deleteList(sorted);
        std::cout << "测试通过!\n\n";
    }
    
    // 测试7：有重复元素
    {
        std::cout << "【测试7】有重复元素\n";
        ListNode* head = createList({3, 1, 2, 3, 1});
        std::cout << "原链表: " << listToString(head) << "\n";
        
        ListNode* sorted = solution.sortList(head);
        std::cout << "排序后: " << listToString(sorted) << "\n";
        
        assert(isSorted(sorted));
        std::vector<int> expected = {1, 1, 2, 3, 3};
        assert(listToArray(sorted) == expected);
        
        deleteList(sorted);
        std::cout << "测试通过!\n\n";
    }
    
    // 测试8：自底向上归并排序
    {
        std::cout << "【测试8】自底向上归并排序\n";
        ListNode* head = createList({4, 2, 1, 3, 6, 5, 8, 7});
        std::cout << "原链表: " << listToString(head) << "\n";
        
        ListNode* sorted = solution.sortListBottomUp(head);
        std::cout << "排序后: " << listToString(sorted) << "\n";
        
        assert(isSorted(sorted));
        
        deleteList(sorted);
        std::cout << "测试通过!\n\n";
    }
    
    std::cout << "=== 所有测试通过! ===\n";
}

// 演示函数（供主程序调用）
namespace leetcode {

void testSortList() {
    Solution148 solution;
    
    // 演示1：普通排序
    ListNode* head1 = createList({4, 2, 1, 3});
    std::cout << "原链表: " << listToString(head1) << "\n";
    ListNode* sorted1 = solution.sortList(head1);
    std::cout << "排序后: " << listToString(sorted1) << "\n";
    deleteList(sorted1);
    
    // 演示2：较长链表
    std::cout << "\n";
    ListNode* head2 = createList({-1, 5, 3, 4, 0});
    std::cout << "原链表: " << listToString(head2) << "\n";
    ListNode* sorted2 = solution.sortListBottomUp(head2);
    std::cout << "排序后: " << listToString(sorted2) << "\n";
    deleteList(sorted2);
    
    // 算法复杂度说明
    std::cout << "\n复杂度分析:\n";
    std::cout << "自顶向下: O(n log n) 时间, O(log n) 空间(递归栈)\n";
    std::cout << "自底向上: O(n log n) 时间, O(1) 空间\n";
}

} // namespace leetcode

// 主函数
int main() {
    runTests();
    return 0;
}
