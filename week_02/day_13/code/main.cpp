/**
 * @file main.cpp
 * @brief Day 13: 链表综合练习 - 主程序入口
 * 
 * 本程序涵盖：
 * 1. 链表常见技巧汇总
 * 2. LeetCode 160: 相交链表
 * 3. LeetCode 148: 排序链表
 */

#include <iostream>
#include <vector>

// 练习模块声明
namespace practice {
    void demonstrateListSummary();
    void demonstrateListTricks();
    void demonstrateSmartPtrSummary();
}

int main() {
    std::cout << "========================================\n";
    std::cout << "Day 13: 链表综合练习\n";
    std::cout << "========================================\n\n";
    
    // 1. 链表技巧总结
    std::cout << "【1】链表技巧总结\n";
    std::cout << "----------------------------------------\n";
    practice::demonstrateListSummary();
    std::cout << "\n";
    
    // 2. 链表技巧练习
    std::cout << "【2】链表技巧练习\n";
    std::cout << "----------------------------------------\n";
    practice::demonstrateListTricks();
    std::cout << "\n";
    
    // 3. 智能指针总结
    std::cout << "【3】智能指针与链表\n";
    std::cout << "----------------------------------------\n";
    practice::demonstrateSmartPtrSummary();
    std::cout << "\n";
    
    // 4. LeetCode 160/148
    std::cout << "【4】LeetCode 160: 相交链表\n";
    std::cout << "----------------------------------------\n";
    std::cout << "请运行 test_160_intersection 查看完整测试。\n";
    std::cout << "\n";
    
    std::cout << "【5】LeetCode 148: 排序链表\n";
    std::cout << "----------------------------------------\n";
    std::cout << "请运行 test_148_sort_list 查看完整测试。\n";
    std::cout << "\n";
    
    std::cout << "========================================\n";
    std::cout << "Day 13 学习完成!\n";
    std::cout << "========================================\n";
    
    return 0;
}
