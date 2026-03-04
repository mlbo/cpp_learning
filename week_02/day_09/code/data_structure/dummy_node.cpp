/**
 * @file dummy_node.cpp
 * @brief 虚拟头节点技巧详解
 * 
 * 虚拟头节点（Dummy Node / Sentinel Node）的作用：
 * 1. 统一边界条件处理
 * 2. 避免对头节点的特殊判断
 * 3. 简化代码逻辑
 */

#include <iostream>
#include <vector>
#include <memory>

// 链表节点定义（使用智能指针版本）
struct ListNode {
    int val;
    std::shared_ptr<ListNode> next;
    
    ListNode(int x) : val(x), next(nullptr) {}
};

// 使用智能指针创建链表
std::shared_ptr<ListNode> createList(const std::vector<int>& vals) {
    if (vals.empty()) return nullptr;
    
    auto head = std::make_shared<ListNode>(vals[0]);
    auto cur = head;
    for (size_t i = 1; i < vals.size(); ++i) {
        cur->next = std::make_shared<ListNode>(vals[i]);
        cur = cur->next;
    }
    return head;
}

// 打印链表
void printList(const std::shared_ptr<ListNode>& head) {
    auto cur = head;
    while (cur) {
        std::cout << cur->val << " -> ";
        cur = cur->next;
    }
    std::cout << "nullptr\n";
}

// ============================================================
// 示例1：删除链表节点
// ============================================================

/**
 * @brief 不使用虚拟头节点删除节点
 * 
 * 缺点：需要特殊处理删除头节点的情况
 */
std::shared_ptr<ListNode> deleteNodeWithoutDummy(
    std::shared_ptr<ListNode> head, int val) {
    
    // 特殊情况：删除的是头节点
    while (head && head->val == val) {
        head = head->next;
    }
    
    if (!head) return nullptr;
    
    auto cur = head;
    while (cur->next) {
        if (cur->next->val == val) {
            cur->next = cur->next->next;
        } else {
            cur = cur->next;
        }
    }
    
    return head;
}

/**
 * @brief 使用虚拟头节点删除节点
 * 
 * 优点：统一处理，不需要特殊判断
 */
std::shared_ptr<ListNode> deleteNodeWithDummy(
    std::shared_ptr<ListNode> head, int val) {
    
    // 创建虚拟头节点
    auto dummy = std::make_shared<ListNode>(0);
    dummy->next = head;
    
    auto cur = dummy;
    while (cur->next) {
        if (cur->next->val == val) {
            cur->next = cur->next->next;
            // 注意：这里没有break，继续删除后续相同值的节点
        } else {
            cur = cur->next;
        }
    }
    
    return dummy->next;
}

// ============================================================
// 示例2：反转链表
// ============================================================

/**
 * @brief 反转链表（使用虚拟头节点简化）
 */
std::shared_ptr<ListNode> reverseList(
    std::shared_ptr<ListNode> head) {
    
    auto dummy = std::make_shared<ListNode>(0);
    auto cur = head;
    
    while (cur) {
        auto next = cur->next;
        // 头插法
        cur->next = dummy->next;
        dummy->next = cur;
        cur = next;
    }
    
    return dummy->next;
}

// ============================================================
// 示例3：两两交换链表节点
// ============================================================

/**
 * @brief 两两交换相邻节点
 * 
 * 示例：1->2->3->4 变成 2->1->4->3
 * 
 * 虚拟头节点让第一对交换的处理和后续一致
 */
std::shared_ptr<ListNode> swapPairs(
    std::shared_ptr<ListNode> head) {
    
    auto dummy = std::make_shared<ListNode>(0);
    dummy->next = head;
    
    auto prev = dummy;
    
    while (prev->next && prev->next->next) {
        // 要交换的两个节点
        auto first = prev->next;
        auto second = prev->next->next;
        
        // 交换
        first->next = second->next;
        second->next = first;
        prev->next = second;
        
        // 移动到下一对
        prev = first;
    }
    
    return dummy->next;
}

// ============================================================
// 示例4：删除链表倒数第N个节点
// ============================================================

/**
 * @brief 删除链表倒数第N个节点
 * 
 * 虚拟头节点的作用：
 * - 统一处理删除倒数第N个（可能是头节点）的情况
 * - 简化双指针的逻辑
 */
std::shared_ptr<ListNode> removeNthFromEnd(
    std::shared_ptr<ListNode> head, int n) {
    
    auto dummy = std::make_shared<ListNode>(0);
    dummy->next = head;
    
    auto fast = dummy;
    auto slow = dummy;
    
    // 快指针先走n步
    for (int i = 0; i < n; ++i) {
        fast = fast->next;
    }
    
    // 同时移动
    while (fast->next) {
        fast = fast->next;
        slow = slow->next;
    }
    
    // 删除节点
    slow->next = slow->next->next;
    
    return dummy->next;
}

// ============================================================
// 演示函数
// ============================================================

void demoDummyNode() {
    // ----- 演示删除节点 -----
    std::cout << "【演示1：删除链表中值为2的节点】\n\n";
    
    auto list1 = createList({1, 2, 3, 2, 4, 2, 5});
    std::cout << "原链表: ";
    printList(list1);
    
    auto result1 = deleteNodeWithDummy(list1, 2);
    std::cout << "删除后: ";
    printList(result1);
    
    // ----- 演示反转链表 -----
    std::cout << "\n【演示2：反转链表】\n\n";
    
    auto list2 = createList({1, 2, 3, 4, 5});
    std::cout << "原链表: ";
    printList(list2);
    
    auto result2 = reverseList(list2);
    std::cout << "反转后: ";
    printList(result2);
    
    // ----- 演示两两交换 -----
    std::cout << "\n【演示3：两两交换节点】\n\n";
    
    auto list3 = createList({1, 2, 3, 4, 5, 6});
    std::cout << "原链表: ";
    printList(list3);
    
    auto result3 = swapPairs(list3);
    std::cout << "交换后: ";
    printList(result3);
    
    // ----- 演示删除倒数第N个 -----
    std::cout << "\n【演示4：删除倒数第N个节点】\n\n";
    
    auto list4 = createList({1, 2, 3, 4, 5});
    std::cout << "原链表: ";
    printList(list4);
    
    auto result4 = removeNthFromEnd(list4, 2);
    std::cout << "删除倒数第2个后: ";
    printList(result4);
    
    // 特殊情况：删除头节点
    auto list5 = createList({1, 2, 3});
    std::cout << "\n原链表: ";
    printList(list5);
    
    auto result5 = removeNthFromEnd(list5, 3);
    std::cout << "删除倒数第3个（头节点）后: ";
    printList(result5);
    
    std::cout << "\n虚拟头节点优势总结：\n";
    std::cout << "1. 统一处理边界条件（如删除头节点）\n";
    std::cout << "2. 简化代码逻辑，减少特殊判断\n";
    std::cout << "3. 使双指针等技巧更容易实现\n";
    std::cout << "4. 返回 dummy->next 即可，无需额外处理\n";
}
