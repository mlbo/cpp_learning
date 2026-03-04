/**
 * @file main.cpp
 * @brief Day 8 主程序入口 - 链表数据结构与unique_ptr智能指针
 * @author C++ Tutorial
 * @date Day 8
 */

#include <iostream>
#include <memory>
#include <vector>

#include "data_structure/list_node.h"
#include "leetcode/0203_remove_elements/solution.h"
#include "leetcode/0206_reverse_list/solution.h"

// 分隔线
void printSeparator(const std::string& title) {
    std::cout << "\n======== " << title << " ========\n";
}

// 演示链表基本操作
void demoListOperations() {
    printSeparator("链表基本操作演示");
    
    using namespace list_ops;
    
    // 创建链表
    std::cout << "\n【创建链表】\n";
    ListNode* head = createList({1, 2, 3, 4, 5});
    printList(head);
    std::cout << "链表长度: " << getLength(head) << "\n";
    
    // 插入操作
    std::cout << "\n【插入操作】\n";
    insertHead(head, 0);
    std::cout << "头部插入0: ";
    printList(head);
    
    insertTail(head, 6);
    std::cout << "尾部插入6: ";
    printList(head);
    
    insertAt(head, 3, 100);
    std::cout << "位置3插入100: ";
    printList(head);
    
    // 删除操作
    std::cout << "\n【删除操作】\n";
    deleteFirst(head, 100);
    std::cout << "删除值100: ";
    printList(head);
    
    deleteTail(head);
    std::cout << "删除尾部: ";
    printList(head);
    
    // 查找操作
    std::cout << "\n【查找操作】\n";
    ListNode* found = find(head, 3);
    if (found) {
        std::cout << "找到值3的节点，地址: " << found << "\n";
    }
    
    std::cout << "值7是否存在: " << (contains(head, 7) ? "是" : "否") << "\n";
    
    // 反转操作
    std::cout << "\n【反转操作】\n";
    ListNode* reversed = reverse(copyList(head));
    std::cout << "反转后的链表: ";
    printList(reversed);
    
    // 清理
    deleteList(head);
    deleteList(reversed);
}

// 演示unique_ptr基本用法
void demoUniquePtr() {
    printSeparator("unique_ptr基本用法");
    
    // 创建
    std::cout << "\n【创建unique_ptr】\n";
    auto p1 = std::make_unique<int>(42);
    std::cout << "p1 = " << *p1 << "\n";
    
    // 移动
    std::cout << "\n【移动所有权】\n";
    auto p2 = std::move(p1);
    std::cout << "p1是否为空: " << (p1 ? "否" : "是") << "\n";
    std::cout << "p2 = " << *p2 << "\n";
    
    // 重置
    std::cout << "\n【reset操作】\n";
    p2.reset(new int(100));
    std::cout << "reset后 p2 = " << *p2 << "\n";
    
    // 数组
    std::cout << "\n【数组unique_ptr】\n";
    auto arr = std::make_unique<int[]>(5);
    for (int i = 0; i < 5; ++i) {
        arr[i] = i * i;
    }
    std::cout << "数组元素: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";
}

// 演示LeetCode 203
void demoLeetCode203() {
    printSeparator("LeetCode 203: 移除链表元素");
    
    using namespace list_ops;
    
    // 测试用例1
    ListNode* head1 = createList({1, 2, 6, 3, 4, 5, 6});
    std::cout << "\n原始链表: ";
    printList(head1);
    
    ListNode* result1 = leetcode::removeElements(head1, 6);
    std::cout << "移除6后: ";
    printList(result1);
    deleteList(result1);
    
    // 测试用例2
    ListNode* head2 = createList({7, 7, 7, 7});
    std::cout << "\n原始链表: ";
    printList(head2);
    
    ListNode* result2 = leetcode::removeElements(head2, 7);
    std::cout << "移除7后: ";
    printList(result2);
    // result2为空，不需要删除
}

// 演示LeetCode 206
void demoLeetCode206() {
    printSeparator("LeetCode 206: 反转链表");
    
    using namespace list_ops;
    
    // 迭代法
    ListNode* head1 = createList({1, 2, 3, 4, 5});
    std::cout << "\n【迭代法】\n";
    std::cout << "原始链表: ";
    printList(head1);
    
    ListNode* reversed1 = leetcode::reverseList(head1);
    std::cout << "反转后: ";
    printList(reversed1);
    deleteList(reversed1);
    
    // 递归法
    ListNode* head2 = createList({1, 2, 3, 4, 5});
    std::cout << "\n【递归法】\n";
    std::cout << "原始链表: ";
    printList(head2);
    
    ListNode* reversed2 = leetcode::reverseListRecursive(head2);
    std::cout << "反转后: ";
    printList(reversed2);
    deleteList(reversed2);
}

int main() {
    std::cout << "╔═════════════════════════════════════════════════════════════╗\n";
    std::cout << "║      Day 8: 链表数据结构与unique_ptr智能指针               ║\n";
    std::cout << "╚═════════════════════════════════════════════════════════════╝\n";
    
    demoListOperations();
    demoUniquePtr();
    demoLeetCode203();
    demoLeetCode206();
    
    printSeparator("今日总结");
    std::cout << "\n链表核心要点:\n";
    std::cout << "  1. 离散内存布局，O(1)插入删除，O(n)访问\n";
    std::cout << "  2. 虚拟头节点简化边界处理\n";
    std::cout << "  3. 快慢指针解决中间值/环检测问题\n";
    
    std::cout << "\nunique_ptr核心要点:\n";
    std::cout << "  1. 独占所有权，不可拷贝，可移动\n";
    std::cout << "  2. 零开销抽象，与原生指针同等性能\n";
    std::cout << "  3. 优先使用make_unique创建\n";
    std::cout << "  4. 支持自定义删除器\n";
    
    std::cout << "\nEMC++条款17-18核心:\n";
    std::cout << "  1. 特殊成员函数生成规则复杂，建议=delete或=default\n";
    std::cout << "  2. Rule of Zero/Three/Five 指导原则\n";
    std::cout << "  3. unique_ptr是独占资源管理的首选\n";
    
    return 0;
}
