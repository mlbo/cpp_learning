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
#include <sstream>
#include <string>
#include <string_view>

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
enum class CommandLineResult {
    Interactive,
    Handled,
    Error
};

CommandLineResult handleCommandLineArgs(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        const std::string_view argument(argv[i]);
        if (argument == "--all" || argument == "-a") {
            runAllDemos();
            return CommandLineResult::Handled;
        }
        if (argument == "--help" || argument == "-h") {
            std::cout << "用法: " << argv[0] << " [选项]\n\n";
            std::cout << "选项:\n";
            std::cout << "  --all, -a    运行所有演示\n";
            std::cout << "  --help, -h   显示帮助信息\n";
            return CommandLineResult::Handled;
        }

        std::cerr << "未知选项: " << argument << '\n';
        std::cerr << "请使用 --help 查看可用选项。\n";
        return CommandLineResult::Error;
    }
    return CommandLineResult::Interactive;
}

bool parseChoice(const std::string& line, int& choice) {
    std::istringstream input(line);
    input >> std::ws;
    if (!(input >> choice)) {
        return false;
    }
    input >> std::ws;
    return input.eof();
}

/**
 * @brief 主函数
 */
int main(int argc, char* argv[]) {
    // 处理命令行参数
    const CommandLineResult commandLineResult = handleCommandLineArgs(argc, argv);
    if (commandLineResult == CommandLineResult::Handled) {
        return 0;
    }
    if (commandLineResult == CommandLineResult::Error) {
        return 1;
    }
    
    printWelcome();
    
    // 交互式菜单
    while (true) {
        printMenu();

        std::string line;
        if (!std::getline(std::cin, line)) {
            std::cout << "\n输入结束，退出交互模式。\n";
            return 0;
        }

        int choice = -1;
        if (!parseChoice(line, choice)) {
            std::cout << "\n无效输入，请输入 0 到 8 之间的整数。\n";
            continue;
        }
        
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
                continue;
        }

        std::cout << "\n按 Enter 键继续...";
        if (!std::getline(std::cin, line)) {
            std::cout << "\n输入结束，退出交互模式。\n";
            return 0;
        }
    }
}
