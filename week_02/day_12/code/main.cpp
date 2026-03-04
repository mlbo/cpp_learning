/**
 * @file main.cpp
 * @brief Day 12 主入口 - 智能指针总结与RAII原则
 * 
 * 今日主题:
 * 1. 智能指针选择指南
 * 2. RAII原则详解
 * 3. LeetCode 24: 两两交换链表节点
 * 4. LeetCode 25: K个一组翻转链表
 */

#include <iostream>
#include <string>

// 声明各模块的运行函数
extern void run_smart_ptr_guide();
extern void run_raii_demo();

// LeetCode 测试函数声明
namespace leetcode_24 {
    void run_tests();
}

namespace leetcode_25 {
    void run_tests();
}

// ============================================
// 菜单显示
// ============================================

void print_menu() {
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════════════╗\n";
    std::cout << "║          Day 12: 智能指针总结 & RAII原则                  ║\n";
    std::cout << "╠══════════════════════════════════════════════════════════╣\n";
    std::cout << "║  1. 智能指针选择指南                                     ║\n";
    std::cout << "║  2. RAII原则演示                                         ║\n";
    std::cout << "║  3. LeetCode 24: 两两交换链表节点                        ║\n";
    std::cout << "║  4. LeetCode 25: K个一组翻转链表                         ║\n";
    std::cout << "║  5. 运行全部演示                                         ║\n";
    std::cout << "║  0. 退出                                                 ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════╝\n";
    std::cout << "请选择: ";
}

// ============================================
// 主函数
// ============================================

int main() {
    std::cout << "========================================\n";
    std::cout << "  Day 12: 智能指针总结 & RAII原则\n";
    std::cout << "========================================\n";
    
    while (true) {
        print_menu();
        
        int choice;
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                run_smart_ptr_guide();
                break;
                
            case 2:
                run_raii_demo();
                break;
                
            case 3:
                leetcode_24::run_tests();
                break;
                
            case 4:
                leetcode_25::run_tests();
                break;
                
            case 5:
                std::cout << "\n===== 运行全部演示 =====\n";
                run_smart_ptr_guide();
                run_raii_demo();
                leetcode_24::run_tests();
                leetcode_25::run_tests();
                std::cout << "\n===== 全部演示完成 =====\n";
                break;
                
            case 0:
                std::cout << "\n再见！Happy Coding!\n";
                return 0;
                
            default:
                std::cout << "无效选择，请重新输入。\n";
        }
    }
    
    return 0;
}
