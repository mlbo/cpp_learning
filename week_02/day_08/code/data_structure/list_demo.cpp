/**
 * @file list_demo.cpp
 * @brief 链表数据结构演示程序
 * @author C++ Tutorial
 * @date Day 8
 * 
 * 本文件演示链表的基本操作，包括创建、插入、删除、反转等。
 * 重点展示虚拟头节点技巧的使用。
 */

#include "list_node.h"
#include <iostream>
#include <iomanip>

// 辅助函数：打印分隔线
void printSeparator(const std::string& title = "") {
    std::cout << "\n";
    if (!title.empty()) {
        std::cout << "======== " << title << " ========\n";
    } else {
        std::cout << std::string(50, '=') << "\n";
    }
}

// 演示链表的创建与基本遍历
void demoCreationAndTraversal() {
    printSeparator("链表创建与遍历");
    
    // 方式1：从vector创建
    std::vector<int> values = {1, 2, 3, 4, 5};
    ListNode* list1 = list_ops::createList(values);
    std::cout << "从vector创建: ";
    list_ops::printList(list1);
    
    // 方式2：从初始化列表创建
    ListNode* list2 = list_ops::createList({10, 20, 30, 40, 50});
    std::cout << "从初始化列表创建: ";
    list_ops::printList(list2);
    
    // 链表长度
    std::cout << "list1长度: " << list_ops::getLength(list1) << "\n";
    
    // 获取特定节点
    ListNode* node2 = list_ops::getNode(list1, 2);
    if (node2) {
        std::cout << "list1第2个节点(索引): " << node2->val << "\n";
    }
    
    // 获取最后一个节点
    ListNode* last = list_ops::getLastNode(list1);
    if (last) {
        std::cout << "list1最后一个节点: " << last->val << "\n";
    }
    
    // 转换为vector
    std::vector<int> vec = list_ops::toVector(list1);
    std::cout << "转换为vector: [";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i];
        if (i < vec.size() - 1) std::cout << ", ";
    }
    std::cout << "]\n";
    
    // 释放内存
    list_ops::deleteList(list1);
    list_ops::deleteList(list2);
}

// 演示链表的插入操作
void demoInsertion() {
    printSeparator("链表插入操作");
    
    ListNode* head = nullptr;
    
    // 头插法
    std::cout << "头插法插入 1, 2, 3:\n";
    list_ops::insertHead(head, 1);
    list_ops::insertHead(head, 2);
    list_ops::insertHead(head, 3);
    list_ops::printList(head);
    std::cout << "结果: 3 -> 2 -> 1 (逆序)\n";
    
    list_ops::deleteList(head);
    head = nullptr;
    
    // 尾插法
    std::cout << "\n尾插法插入 1, 2, 3:\n";
    list_ops::insertTail(head, 1);
    list_ops::insertTail(head, 2);
    list_ops::insertTail(head, 3);
    list_ops::printList(head);
    std::cout << "结果: 1 -> 2 -> 3 (正序)\n";
    
    // 在指定位置插入
    std::cout << "\n在位置2插入 99:\n";
    list_ops::insertAt(head, 2, 99);
    list_ops::printList(head);
    
    // 在节点后插入
    std::cout << "\n在值2的节点后插入 88:\n";
    ListNode* node = list_ops::find(head, 2);
    if (node) {
        list_ops::insertAfter(node, 88);
    }
    list_ops::printList(head);
    
    list_ops::deleteList(head);
}

// 演示链表的删除操作
void demoDeletion() {
    printSeparator("链表删除操作");
    
    ListNode* head = list_ops::createList({1, 2, 3, 4, 5, 3, 6, 3});
    std::cout << "原链表: ";
    list_ops::printList(head);
    
    // 删除头节点
    std::cout << "\n删除头节点:\n";
    list_ops::deleteHead(head);
    list_ops::printList(head);
    
    // 删除尾节点
    std::cout << "\n删除尾节点:\n";
    list_ops::deleteTail(head);
    list_ops::printList(head);
    
    // 删除第一个匹配值
    std::cout << "\n删除第一个值为3的节点:\n";
    list_ops::deleteFirst(head, 3);
    list_ops::printList(head);
    
    // 删除所有匹配值
    std::cout << "\n删除所有值为3的节点:\n";
    int count = list_ops::deleteAll(head, 3);
    std::cout << "删除了 " << count << " 个节点\n";
    list_ops::printList(head);
    
    list_ops::deleteList(head);
}

// 演示虚拟头节点技巧
void demoDummyHead() {
    printSeparator("虚拟头节点技巧");
    
    // 问题：删除链表中所有值为target的节点
    // 如果不用虚拟头节点，需要特殊处理头节点
    
    std::cout << "【对比演示】删除链表中所有值为2的节点\n\n";
    
    // 方法1：不使用虚拟头节点（需要特殊处理头节点）
    auto removeWithoutDummy = [](ListNode* head, int val) -> ListNode* {
        // 先删除头部的所有目标值节点
        while (head && head->val == val) {
            ListNode* toDelete = head;
            head = head->next;
            delete toDelete;
        }
        
        if (!head) return nullptr;
        
        // 删除中间和尾部的目标值节点
        ListNode* curr = head;
        while (curr->next) {
            if (curr->next->val == val) {
                ListNode* toDelete = curr->next;
                curr->next = toDelete->next;
                delete toDelete;
            } else {
                curr = curr->next;
            }
        }
        
        return head;
    };
    
    // 方法2：使用虚拟头节点（代码更简洁）
    auto removeWithDummy = [](ListNode* head, int val) -> ListNode* {
        ListNode* dummy = new ListNode(0, head);
        ListNode* curr = dummy;
        
        while (curr->next) {
            if (curr->next->val == val) {
                ListNode* toDelete = curr->next;
                curr->next = toDelete->next;
                delete toDelete;
            } else {
                curr = curr->next;
            }
        }
        
        ListNode* result = dummy->next;
        delete dummy;
        return result;
    };
    
    // 测试
    ListNode* list1 = list_ops::createList({2, 1, 2, 3, 2, 4, 2});
    ListNode* list2 = list_ops::createList({2, 1, 2, 3, 2, 4, 2});
    
    std::cout << "原链表: ";
    list_ops::printList(list1);
    
    list1 = removeWithoutDummy(list1, 2);
    std::cout << "不用虚拟头节点删除后: ";
    list_ops::printList(list1);
    
    list2 = removeWithDummy(list2, 2);
    std::cout << "使用虚拟头节点删除后: ";
    list_ops::printList(list2);
    
    std::cout << "\n虚拟头节点优势:\n";
    std::cout << "  1. 统一所有节点的处理逻辑\n";
    std::cout << "  2. 不需要特殊处理头节点\n";
    std::cout << "  3. 代码更简洁、更易理解\n";
    
    list_ops::deleteList(list1);
    list_ops::deleteList(list2);
}

// 演示链表反转
void demoReverse() {
    printSeparator("链表反转");
    
    ListNode* head = list_ops::createList({1, 2, 3, 4, 5});
    std::cout << "原链表: ";
    list_ops::printList(head);
    
    // 迭代法反转
    ListNode* reversed = list_ops::reverse(head);
    std::cout << "\n迭代法反转后: ";
    list_ops::printList(reversed);
    
    // 递归法反转（先反转回来）
    head = list_ops::reverse(reversed);
    ListNode* reversed2 = list_ops::reverseRecursive(head);
    std::cout << "递归法反转后: ";
    list_ops::printList(reversed2);
    
    // 反转前N个节点
    head = list_ops::reverse(reversed2);
    head = list_ops::reverseN(head, 3);
    std::cout << "\n反转前3个节点: ";
    list_ops::printList(head);
    
    // 反转区间
    head = list_ops::reverseBetween(head, 2, 4);
    std::cout << "反转位置2-4: ";
    list_ops::printList(head);
    
    list_ops::deleteList(head);
}

// 演示链表查找与判断
void demoSearchAndCheck() {
    printSeparator("查找与判断");
    
    ListNode* head = list_ops::createList({1, 2, 3, 4, 5});
    list_ops::printList(head);
    
    // 查找节点
    std::cout << "\n查找值为3的节点: ";
    ListNode* node = list_ops::find(head, 3);
    std::cout << (node ? "找到，值为" + std::to_string(node->val) : "未找到") << "\n";
    
    std::cout << "查找值为99的节点: ";
    node = list_ops::find(head, 99);
    std::cout << (node ? "找到" : "未找到") << "\n";
    
    // 检查是否包含
    std::cout << "\n包含值3? " << (list_ops::contains(head, 3) ? "是" : "否") << "\n";
    std::cout << "包含值99? " << (list_ops::contains(head, 99) ? "是" : "否") << "\n";
    
    // 查找前驱
    std::cout << "\n值3的前驱节点: ";
    ListNode* prev = list_ops::findPrev(head, 3);
    std::cout << (prev ? std::to_string(prev->val) : "无") << "\n";
    
    // 中点查找
    ListNode* mid = list_ops::findMiddle(head);
    std::cout << "中点节点值: " << mid->val << "\n";
    
    list_ops::deleteList(head);
}

// 演示链表合并
void demoMerge() {
    printSeparator("链表合并");
    
    ListNode* l1 = list_ops::createList({1, 3, 5, 7});
    ListNode* l2 = list_ops::createList({2, 4, 6, 8});
    
    std::cout << "链表1: ";
    list_ops::printList(l1);
    std::cout << "链表2: ";
    list_ops::printList(l2);
    
    ListNode* merged = list_ops::mergeTwoLists(l1, l2);
    std::cout << "\n合并后: ";
    list_ops::printList(merged);
    
    list_ops::deleteList(merged);
}

// 演示环检测
void demoCycleDetection() {
    printSeparator("环检测");
    
    // 创建无环链表
    ListNode* list1 = list_ops::createList({1, 2, 3, 4, 5});
    std::cout << "无环链表: ";
    list_ops::printList(list1);
    std::cout << "是否有环: " << (list_ops::hasCycle(list1) ? "是" : "否") << "\n";
    
    // 创建有环链表
    ListNode* list2 = list_ops::createList({1, 2, 3, 4, 5});
    ListNode* tail = list_ops::getLastNode(list2);
    ListNode* node3 = list_ops::find(list2, 3);
    tail->next = node3;  // 创建环
    
    std::cout << "\n有环链表: 1->2->3->4->5->3(循环)\n";
    std::cout << "是否有环: " << (list_ops::hasCycle(list2) ? "是" : "否") << "\n";
    
    ListNode* entry = list_ops::detectCycle(list2);
    if (entry) {
        std::cout << "环入口节点值: " << entry->val << "\n";
    }
    
    // 断开环以便正常释放
    tail->next = nullptr;
    list_ops::deleteList(list1);
    list_ops::deleteList(list2);
}

// 演示现代C++风格的链表
void demoModernList() {
    printSeparator("现代C++风格链表 (unique_ptr)");
    
    std::cout << "使用unique_ptr管理链表节点:\n";
    std::cout << "优点: 自动内存管理，无需手动delete\n\n";
    
    {
        auto list = modern_list::createUniqueList({10, 20, 30, 40, 50});
        modern_list::printUniqueList(list.get());
        
        std::cout << "离开作用域时自动释放内存\n";
    }
    
    std::cout << "链表已自动销毁\n";
}

// 内存布局可视化演示
void demoMemoryLayout() {
    printSeparator("内存布局对比");
    
    std::cout << "┌─────────────────────────────────────────────────────┐\n";
    std::cout << "│            数组 vs 链表 内存布局对比               │\n";
    std::cout << "├─────────────────────────────────────────────────────┤\n";
    std::cout << "│ 数组 (连续内存):                                    │\n";
    std::cout << "│   ┌───┬───┬───┬───┬───┐                            │\n";
    std::cout << "│   │ 1 │ 2 │ 3 │ 4 │ 5 │  地址连续                 │\n";
    std::cout << "│   └───┴───┴───┴───┴───┘                            │\n";
    std::cout << "│   随机访问 O(1)，插入删除 O(n)                     │\n";
    std::cout << "├─────────────────────────────────────────────────────┤\n";
    std::cout << "│ 链表 (离散内存):                                    │\n";
    std::cout << "│   ┌───┐    ┌───┐    ┌───┐    ┌───┐    ┌───┐      │\n";
    std::cout << "│   │ 1 │───>│ 2 │───>│ 3 │───>│ 4 │───>│ 5 │───>∅ │\n";
    std::cout << "│   └───┘    └───┘    └───┘    └───┘    └───┘      │\n";
    std::cout << "│   地址分散，只能顺序访问                           │\n";
    std::cout << "│   随机访问 O(n)，插入删除 O(1)                     │\n";
    std::cout << "└─────────────────────────────────────────────────────┘\n";
}

// 主演示函数
void runListDemo() {
    std::cout << "\n";
    std::cout << "╔═════════════════════════════════════════════════════════╗\n";
    std::cout << "║          Day 8: 链表数据结构演示                        ║\n";
    std::cout << "╚═════════════════════════════════════════════════════════╝\n";
    
    demoMemoryLayout();
    demoCreationAndTraversal();
    demoInsertion();
    demoDeletion();
    demoDummyHead();
    demoReverse();
    demoSearchAndCheck();
    demoMerge();
    demoCycleDetection();
    demoModernList();
    
    printSeparator("演示完成");
    std::cout << "Day 8 链表数据结构演示结束！\n";
    std::cout << "核心要点回顾:\n";
    std::cout << "  1. 链表内存离散，适合频繁插入删除场景\n";
    std::cout << "  2. 虚拟头节点技巧简化边界处理\n";
    std::cout << "  3. 快慢指针用于中点查找和环检测\n";
    std::cout << "  4. unique_ptr可实现自动内存管理\n";
}
