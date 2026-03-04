/**
 * @file solution.cpp
 * @brief LeetCode 206: 反转链表 - 解决方案
 * @author C++ Tutorial
 * @date Day 8
 * 
 * 题目描述：
 * 给你单链表的头节点 head ，请你反转链表，并返回反转后的链表。
 * 
 * 示例 1：
 * 输入：head = [1,2,3,4,5]
 * 输出：[5,4,3,2,1]
 * 
 * 示例 2：
 * 输入：head = [1,2]
 * 输出：[2,1]
 * 
 * 示例 3：
 * 输入：head = []
 * 输出：[]
 */

#include "solution.h"

namespace leetcode {

/**
 * @brief 方法一：迭代法（推荐）
 * 
 * 使用三个指针：prev, curr, next
 * 每次迭代将当前节点的next指向prev
 * 
 * 执行过程：
 * 1. 保存next节点
 * 2. 将curr->next指向prev
 * 3. prev前移到curr
 * 4. curr前移到next
 * 
 * @param head 链表头节点
 * @return 反转后的链表头节点
 */
ListNode* reverseList(ListNode* head) {
    ListNode* prev = nullptr;
    ListNode* curr = head;
    
    while (curr != nullptr) {
        ListNode* next = curr->next;  // 保存下一个节点
        curr->next = prev;            // 反转指针
        prev = curr;                  // prev前移
        curr = next;                  // curr前移
    }
    
    return prev;  // prev是新的头节点
}

/**
 * @brief 方法二：递归法
 * 
 * 递归到链表末尾，然后在回溯过程中反转指针。
 * 
 * 核心思想：
 * 1. 递归到末尾，末尾节点成为新的头节点
 * 2. 回溯时，将当前节点的下一个节点的next指向当前节点
 * 3. 将当前节点的next设为nullptr
 * 
 * @param head 链表头节点
 * @return 反转后的链表头节点
 */
ListNode* reverseListRecursive(ListNode* head) {
    // 基础情况：空链表或只有一个节点
    if (head == nullptr || head->next == nullptr) {
        return head;
    }
    
    // 递归反转剩余部分，newHead是反转后的头节点
    ListNode* newHead = reverseListRecursive(head->next);
    
    // 将下一个节点的next指向当前节点
    head->next->next = head;
    // 当前节点的next设为nullptr（避免循环）
    head->next = nullptr;
    
    return newHead;
}

/**
 * @brief 方法三：头插法
 * 
 * 遍历原链表，依次将每个节点插入到新链表的头部。
 * 
 * @param head 链表头节点
 * @return 反转后的链表头节点
 */
ListNode* reverseListHeadInsert(ListNode* head) {
    ListNode* newHead = nullptr;
    ListNode* curr = head;
    
    while (curr != nullptr) {
        ListNode* next = curr->next;  // 保存下一个节点
        curr->next = newHead;         // 插入到新链表头部
        newHead = curr;               // 更新新链表头节点
        curr = next;                  // 继续遍历
    }
    
    return newHead;
}

/**
 * @brief 方法四：反转前N个节点
 * 
 * 反转链表的前N个节点，剩余部分保持不变。
 * 可用于解决反转链表II等问题。
 * 
 * @param head 链表头节点
 * @param n 要反转的节点数量
 * @return 反转后的链表头节点
 */
ListNode* reverseN(ListNode* head, int n) {
    if (n <= 0 || head == nullptr) {
        return head;
    }
    
    ListNode* prev = nullptr;
    ListNode* curr = head;
    int count = 0;
    
    while (curr != nullptr && count < n) {
        ListNode* next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
        ++count;
    }
    
    // 将原链表的剩余部分接到反转后的尾部
    // head现在是反转部分的最后一个节点
    head->next = curr;
    
    return prev;
}

/**
 * @brief 方法五：反转区间[m,n]的节点
 * 
 * 反转链表中从位置m到n的节点（位置从1开始）。
 * 
 * @param head 链表头节点
 * @param m 起始位置
 * @param n 结束位置
 * @return 反转后的链表头节点
 */
ListNode* reverseBetween(ListNode* head, int m, int n) {
    if (m >= n || head == nullptr) {
        return head;
    }
    
    // 使用虚拟头节点简化边界处理
    ListNode* dummy = new ListNode(0, head);
    ListNode* prev = dummy;
    
    // 找到第m-1个节点
    for (int i = 1; i < m && prev->next != nullptr; ++i) {
        prev = prev->next;
    }
    
    if (prev->next == nullptr) {
        delete dummy;
        return head;
    }
    
    // 反转从m到n的节点
    ListNode* curr = prev->next;
    ListNode* tail = curr;  // 反转后的尾部
    ListNode* newPrev = nullptr;
    
    int count = 0;
    int range = n - m + 1;
    
    while (curr != nullptr && count < range) {
        ListNode* next = curr->next;
        curr->next = newPrev;
        newPrev = curr;
        curr = next;
        ++count;
    }
    
    // 连接反转部分
    prev->next = newPrev;
    tail->next = curr;
    
    ListNode* result = dummy->next;
    delete dummy;
    
    return result;
}

} // namespace leetcode

// ============================================================
// 测试代码
// ============================================================

#include <iostream>
#include <vector>

using namespace list_ops;

void testReverseList() {
    std::cout << "======== LeetCode 206: 反转链表 测试 ========\n";
    
    // 测试用例1：普通链表
    std::cout << "\n【测试用例1：迭代法】\n";
    ListNode* head1 = createList({1, 2, 3, 4, 5});
    std::cout << "输入: ";
    printList(head1);
    
    ListNode* result1 = leetcode::reverseList(head1);
    std::cout << "输出: ";
    printList(result1);
    std::cout << "预期: 5 -> 4 -> 3 -> 2 -> 1 -> nullptr\n";
    
    std::vector<int> expected1 = {5, 4, 3, 2, 1};
    std::vector<int> actual1 = toVector(result1);
    std::cout << "结果: " << (actual1 == expected1 ? "✓ 通过" : "✗ 失败") << "\n";
    deleteList(result1);
    
    // 测试用例2：递归法
    std::cout << "\n【测试用例2：递归法】\n";
    ListNode* head2 = createList({1, 2, 3, 4, 5});
    std::cout << "输入: ";
    printList(head2);
    
    ListNode* result2 = leetcode::reverseListRecursive(head2);
    std::cout << "输出: ";
    printList(result2);
    std::cout << "预期: 5 -> 4 -> 3 -> 2 -> 1 -> nullptr\n";
    
    std::vector<int> expected2 = {5, 4, 3, 2, 1};
    std::vector<int> actual2 = toVector(result2);
    std::cout << "结果: " << (actual2 == expected2 ? "✓ 通过" : "✗ 失败") << "\n";
    deleteList(result2);
    
    // 测试用例3：两个节点
    std::cout << "\n【测试用例3：两个节点】\n";
    ListNode* head3 = createList({1, 2});
    std::cout << "输入: ";
    printList(head3);
    
    ListNode* result3 = leetcode::reverseList(head3);
    std::cout << "输出: ";
    printList(result3);
    std::cout << "预期: 2 -> 1 -> nullptr\n";
    
    std::vector<int> expected3 = {2, 1};
    std::vector<int> actual3 = toVector(result3);
    std::cout << "结果: " << (actual3 == expected3 ? "✓ 通过" : "✗ 失败") << "\n";
    deleteList(result3);
    
    // 测试用例4：空链表
    std::cout << "\n【测试用例4：空链表】\n";
    ListNode* head4 = nullptr;
    std::cout << "输入: nullptr\n";
    
    ListNode* result4 = leetcode::reverseList(head4);
    std::cout << "输出: ";
    printList(result4);
    std::cout << "预期: nullptr\n";
    std::cout << "结果: " << (result4 == nullptr ? "✓ 通过" : "✗ 失败") << "\n";
    
    // 测试用例5：单个节点
    std::cout << "\n【测试用例5：单个节点】\n";
    ListNode* head5 = createList({1});
    std::cout << "输入: ";
    printList(head5);
    
    ListNode* result5 = leetcode::reverseList(head5);
    std::cout << "输出: ";
    printList(result5);
    std::cout << "预期: 1 -> nullptr\n";
    
    std::vector<int> expected5 = {1};
    std::vector<int> actual5 = toVector(result5);
    std::cout << "结果: " << (actual5 == expected5 ? "✓ 通过" : "✗ 失败") << "\n";
    deleteList(result5);
    
    // 测试用例6：反转前N个节点
    std::cout << "\n【测试用例6：反转前N个节点】\n";
    ListNode* head6 = createList({1, 2, 3, 4, 5});
    std::cout << "输入: ";
    printList(head6);
    std::cout << "反转前3个节点\n";
    
    ListNode* result6 = leetcode::reverseN(head6, 3);
    std::cout << "输出: ";
    printList(result6);
    std::cout << "预期: 3 -> 2 -> 1 -> 4 -> 5 -> nullptr\n";
    
    std::vector<int> expected6 = {3, 2, 1, 4, 5};
    std::vector<int> actual6 = toVector(result6);
    std::cout << "结果: " << (actual6 == expected6 ? "✓ 通过" : "✗ 失败") << "\n";
    deleteList(result6);
    
    std::cout << "\n======== 测试完成 ========\n";
}

int main() {
    testReverseList();
    return 0;
}
