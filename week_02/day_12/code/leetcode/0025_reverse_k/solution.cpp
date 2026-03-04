/**
 * @file solution.cpp
 * @brief LeetCode 25题 - K个一组翻转链表
 * 
 * 题目描述:
 * 给你链表的头节点 head ，每 k 个节点一组进行翻转，请你返回修改后的链表。
 * k 是一个正整数，它的值小于或等于链表的长度。
 * 如果节点总数不是 k 的整数倍，那么请将最后剩余的节点保持原有顺序。
 * 
 * 示例:
 * 输入: head = [1,2,3,4,5], k = 2
 * 输出: [2,1,4,3,5]
 * 
 * 输入: head = [1,2,3,4,5], k = 3
 * 输出: [3,2,1,4,5]
 */

#include <iostream>
#include <vector>

// ============================================
// 链表节点定义
// ============================================

struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};

// ============================================
// 解决方案类
// ============================================

class Solution {
public:
    // 方法1: 迭代法（推荐）
    ListNode* reverseKGroup_iterative(ListNode* head, int k);
    
    // 方法2: 递归法
    ListNode* reverseKGroup_recursive(ListNode* head, int k);
    
private:
    // 辅助函数：翻转区间内的链表
    ListNode* reverse(ListNode* start, ListNode* end);
};

// ============================================
// 辅助函数实现
// ============================================

ListNode* Solution::reverse(ListNode* start, ListNode* end) {
    // 翻转 [start, end) 区间的节点
    ListNode* prev = nullptr;
    ListNode* curr = start;
    
    while (curr != end) {
        ListNode* next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    
    return prev;  // 返回翻转后的头节点
}

// ============================================
// 方法1: 迭代法
// ============================================

ListNode* Solution::reverseKGroup_iterative(ListNode* head, int k) {
    if (head == nullptr || k == 1) {
        return head;
    }
    
    // 创建虚拟头节点
    ListNode dummy(0);
    dummy.next = head;
    
    // 统计链表长度
    int length = 0;
    ListNode* curr = head;
    while (curr != nullptr) {
        length++;
        curr = curr->next;
    }
    
    // 计算需要翻转的组数
    int groups = length / k;
    
    ListNode* prev = &dummy;
    curr = head;
    
    for (int i = 0; i < groups; ++i) {
        // 找到当前组的结束位置
        ListNode* groupStart = curr;
        ListNode* groupEnd = curr;
        for (int j = 1; j < k; ++j) {
            groupEnd = groupEnd->next;
        }
        
        // 保存下一组的起点
        ListNode* nextGroupStart = groupEnd->next;
        
        // 翻转当前组
        ListNode* newHead = reverse(groupStart, nextGroupStart);
        
        // 连接翻转后的链表
        prev->next = newHead;
        groupStart->next = nextGroupStart;  // 原来的头变成尾
        
        // 更新指针，准备下一组
        prev = groupStart;
        curr = nextGroupStart;
    }
    
    return dummy.next;
}

// ============================================
// 方法2: 递归法
// ============================================

ListNode* Solution::reverseKGroup_recursive(ListNode* head, int k) {
    // 检查剩余节点是否足够k个
    ListNode* curr = head;
    for (int i = 0; i < k; ++i) {
        if (curr == nullptr) {
            return head;  // 不足k个，保持原样
        }
        curr = curr->next;
    }
    
    // 翻转当前k个节点
    ListNode* newHead = reverse(head, curr);
    
    // 递归处理后续链表
    head->next = reverseKGroup_recursive(curr, k);
    
    return newHead;
}

// ============================================
// 测试辅助函数（命名空间内）
// ============================================

namespace leetcode_25 {

// 创建链表
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

// 打印链表
void printList(ListNode* head) {
    std::cout << "[";
    while (head) {
        std::cout << head->val;
        if (head->next) std::cout << ",";
        head = head->next;
    }
    std::cout << "]";
}

// 删除链表
void deleteList(ListNode* head) {
    while (head) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

// ============================================
// 测试用例
// ============================================

void run_tests() {
    std::cout << "\n========================================\n";
    std::cout << "  LeetCode 25: K个一组翻转链表\n";
    std::cout << "========================================\n";
    
    Solution sol;
    
    // 测试用例1
    std::cout << "\n--- 测试用例 1 ---\n";
    ListNode* list1 = createList({1, 2, 3, 4, 5});
    std::cout << "输入: ";
    printList(list1);
    std::cout << ", k = 2\n";
    
    ListNode* result1 = sol.reverseKGroup_iterative(list1, 2);
    std::cout << "迭代法输出: ";
    printList(result1);
    std::cout << "\n";
    std::cout << "预期: [2,1,4,3,5]\n";
    deleteList(result1);
    
    // 测试用例2
    std::cout << "\n--- 测试用例 2 ---\n";
    ListNode* list2 = createList({1, 2, 3, 4, 5});
    std::cout << "输入: ";
    printList(list2);
    std::cout << ", k = 3\n";
    
    ListNode* result2 = sol.reverseKGroup_recursive(list2, 3);
    std::cout << "递归法输出: ";
    printList(result2);
    std::cout << "\n";
    std::cout << "预期: [3,2,1,4,5]\n";
    deleteList(result2);
    
    // 测试用例3: 边界情况
    std::cout << "\n--- 测试用例 3 (边界) ---\n";
    ListNode* list3 = createList({1, 2});
    std::cout << "输入: ";
    printList(list3);
    std::cout << ", k = 2\n";
    
    ListNode* result3 = sol.reverseKGroup_iterative(list3, 2);
    std::cout << "输出: ";
    printList(result3);
    std::cout << "\n";
    std::cout << "预期: [2,1]\n";
    deleteList(result3);
    
    // 测试用例4: k=1 不翻转
    std::cout << "\n--- 测试用例 4 (k=1) ---\n";
    ListNode* list4 = createList({1, 2, 3});
    std::cout << "输入: ";
    printList(list4);
    std::cout << ", k = 1\n";
    
    ListNode* result4 = sol.reverseKGroup_iterative(list4, 1);
    std::cout << "输出: ";
    printList(result4);
    std::cout << "\n";
    std::cout << "预期: [1,2,3] (k=1不翻转)\n";
    deleteList(result4);
    
    // 算法复杂度分析
    std::cout << "\n--- 复杂度分析 ---\n";
    std::cout << "迭代法:\n";
    std::cout << "  时间: O(n) - 遍历链表两次\n";
    std::cout << "  空间: O(1) - 只使用常量额外空间\n";
    std::cout << "递归法:\n";
    std::cout << "  时间: O(n) - 每个节点处理一次\n";
    std::cout << "  空间: O(n/k) - 递归栈深度\n";
    
    std::cout << "\n========================================\n";
    std::cout << "  LeetCode 25 测试完成\n";
    std::cout << "========================================\n";
}

} // namespace leetcode_25

/*
 * ============================================
 * 算法图解
 * ============================================
 * 
 * 示例: 1->2->3->4->5, k = 2
 * 
 * 第一组翻转:
 * ┌───┬───┬───┬───┬───┐
 * │ 1 │ 2 │ 3 │ 4 │ 5 │
 * └───┴───┴───┴───┴───┘
 *   ↓ 翻转
 * ┌───┬───┬───┬───┬───┐
 * │ 2 │ 1 │ 3 │ 4 │ 5 │
 * └───┴───┴───┴───┴───┘
 * 
 * 第二组翻转:
 * ┌───┬───┬───┬───┬───┐
 * │ 2 │ 1 │ 4 │ 3 │ 5 │
 * └───┴───┴───┴───┴───┘
 * 
 * 5 不足 k=2，保持原样
 */
