/**
 * @file main.cpp
 * @brief Day 23: 移动语义 - 主程序入口
 * 
 * 本程序演示 C++11 移动语义的核心概念：
 * 1. 移动语义基础 (std::move, 移动构造函数, 移动赋值)
 * 2. EMC++ Item 23-25 (std::move/std::forward, 通用引用)
 * 3. LeetCode 1 和 454 (哈希表应用)
 */

#include <iostream>
#include <string>
#include <cstring>

// C++11 特性演示
extern void run_move_semantics_demo();
extern void run_move_constructor_demo();

// EMC++ 条款演示
extern void run_item23_demo();
extern void run_item24_demo();
extern void run_item25_demo();

// LeetCode 题解演示
extern void run_two_sum_test();
extern void run_four_sum_ii_test();

/**
 * @brief 打印欢迎信息
 */
void printWelcome() {
    std::cout << R"(
╔═══════════════════════════════════════════════════════════════════╗
║                                                                   ║
║              Day 23: 移动语义 - 让 C++ 代码飞起来                  ║
║                                                                   ║
║  📚 今日主题:                                                     ║
║     • 移动语义 (std::move, 移动构造, 移动赋值)                     ║
║     • EMC++ Item 23-25 (std::move/std::forward)                   ║
║     • 通用引用 vs 右值引用                                        ║
║     • LeetCode 1, 454 (哈希表优化)                                ║
║                                                                   ║
╚═══════════════════════════════════════════════════════════════════╝
)" << std::endl;
}

/**
 * @brief 打印菜单
 */
void printMenu() {
    std::cout << "\n";
    std::cout << "┌─────────────────────────────────────────────────────────────────┐\n";
    std::cout << "│                         选择演示模块                            │\n";
    std::cout << "├─────────────────────────────────────────────────────────────────┤\n";
    std::cout << "│  1. 移动语义基础演示                                            │\n";
    std::cout << "│  2. 移动构造函数演示                                            │\n";
    std::cout << "│  3. EMC++ Item 23: std::move 和 std::forward                    │\n";
    std::cout << "│  4. EMC++ Item 24: 通用引用 vs 右值引用                          │\n";
    std::cout << "│  5. EMC++ Item 25: 正确使用 std::move/std::forward              │\n";
    std::cout << "│  6. LeetCode 1: 两数之和                                        │\n";
    std::cout << "│  7. LeetCode 454: 四数相加 II                                   │\n";
    std::cout << "│  8. 运行所有演示                                                │\n";
    std::cout << "│  0. 退出                                                        │\n";
    std::cout << "└─────────────────────────────────────────────────────────────────┘\n";
    std::cout << "请输入选择 (0-8): ";
}

/**
 * @brief 运行所有演示
 */
void runAllDemos() {
    std::cout << "\n";
    std::cout << "═══════════════════════════════════════════════════════════════════\n";
    std::cout << "                    运行所有演示模块                               \n";
    std::cout << "═══════════════════════════════════════════════════════════════════\n";
    
    std::cout << "\n\n";
    run_move_semantics_demo();
    
    std::cout << "\n\n";
    run_move_constructor_demo();
    
    std::cout << "\n\n";
    run_item23_demo();
    
    std::cout << "\n\n";
    run_item24_demo();
    
    std::cout << "\n\n";
    run_item25_demo();
    
    std::cout << "\n\n";
    run_two_sum_test();
    
    std::cout << "\n\n";
    run_four_sum_ii_test();
}

/**
 * @brief 处理命令行参数
 */
bool handleCommandLineArgs(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--all") == 0 || strcmp(argv[i], "-a") == 0) {
            runAllDemos();
            return true;
        }
        else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            std::cout << "用法: day_23_demo [选项]\n\n";
            std::cout << "选项:\n";
            std::cout << "  --all, -a    运行所有演示\n";
            std::cout << "  --help, -h   显示帮助信息\n";
            return true;
        }
    }
    return false;
}

/**
 * @brief 主函数
 */
int main(int argc, char* argv[]) {
    // 处理命令行参数
    if (handleCommandLineArgs(argc, argv)) {
        return 0;
    }
    
    printWelcome();
    
    // 交互式菜单
    while (true) {
        printMenu();
        
        int choice;
        std::cin >> choice;
        
        std::cout << "\n";
        
        switch (choice) {
            case 0:
                std::cout << "感谢学习！再见！\n";
                return 0;
                
            case 1:
                run_move_semantics_demo();
                break;
                
            case 2:
                run_move_constructor_demo();
                break;
                
            case 3:
                run_item23_demo();
                break;
                
            case 4:
                run_item24_demo();
                break;
                
            case 5:
                run_item25_demo();
                break;
                
            case 6:
                run_two_sum_test();
                break;
                
            case 7:
                run_four_sum_ii_test();
                break;
                
            case 8:
                runAllDemos();
                break;
                
            default:
                std::cout << "无效选择，请重新输入。\n";
        }
        
        std::cout << "\n按 Enter 键继续...";
        std::cin.ignore();
        std::cin.get();
    }
    
    return 0;
}
