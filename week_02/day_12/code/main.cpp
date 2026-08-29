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
#include <limits>
#include <string>

// 声明各模块的运行函数
extern void run_smart_ptr_guide();
extern void run_raw_vs_smart();
extern void run_custom_deleter();
extern void run_raii_demo();
extern bool run_raii_contract_tests();
extern void run_heap_memory();
extern bool run_memory_pool_contract_tests();
extern void run_memory_leak();

// LeetCode 测试函数声明
namespace leetcode_24 {
    bool run_tests();
}

namespace leetcode_25 {
    bool run_tests();
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

bool run_all() {
    std::cout << "\n===== 运行全部演示 =====\n";
    run_smart_ptr_guide();
    run_raw_vs_smart();
    run_custom_deleter();
    run_raii_demo();
    run_heap_memory();
    run_memory_leak();
    const bool day24_ok = leetcode_24::run_tests();
    const bool day25_ok = leetcode_25::run_tests();
    std::cout << "\n===== 全部演示完成 =====\n";
    return day24_ok && day25_ok;
}

int main(int argc, char* argv[]) {
    std::cout << "========================================\n";
    std::cout << "  Day 12: 智能指针总结 & RAII原则\n";
    std::cout << "========================================\n";
    
    if (argc > 1 && std::string(argv[1]) == "--all") {
        return run_all() ? 0 : 1;
    }
    if (argc > 1 && std::string(argv[1]) == "--resource-contracts") {
        const bool raii_ok = run_raii_contract_tests();
        const bool memory_pool_ok = run_memory_pool_contract_tests();
        return raii_ok && memory_pool_ok ? 0 : 1;
    }
    if (argc > 1) {
        std::cerr << "未知参数: " << argv[1]
                  << "（可用参数: --all, --resource-contracts）\n";
        return 2;
    }

    while (true) {
        print_menu();
        
        int choice = 0;
        if (!(std::cin >> choice)) {
            if (std::cin.eof()) {
                std::cout << "\n输入结束，退出菜单。\n";
                return 0;
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "请输入0到5之间的整数。\n";
            continue;
        }
        
        switch (choice) {
            case 1:
                run_smart_ptr_guide();
                break;
                
            case 2:
                run_raii_demo();
                break;
                
            case 3:
                (void)leetcode_24::run_tests();
                break;
                
            case 4:
                (void)leetcode_25::run_tests();
                break;
                
            case 5:
                (void)run_all();
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
