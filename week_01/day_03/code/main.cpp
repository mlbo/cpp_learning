/**
 * @file main.cpp
 * @brief Day 3: C++11 统一初始化与初始化列表 - 主程序入口
 *
 * 本程序演示：
 * 1. C++11 统一初始化语法
 * 2. std::initializer_list 详解
 * 3. () 与 {} 初始化对比
 * 4. EMC++ 条款 7
 * 5. LeetCode 88 题和 283 题
 */

#include <iostream>
#include <string>

namespace cpp11_features {
void run_uniform_init_demo();
void run_initializer_list_demo();
void run_init_comparison_demo();
}

namespace emcpp {
void run_item07_demo();
}

namespace leetcode_0088 {
void run_tests();
}

namespace leetcode_0283 {
void run_tests();
}

void print_banner() {
    std::cout << R"(
╔══════════════════════════════════════════════════════════════════════╗
║                                                                      ║
║           Day 3: C++11 统一初始化与初始化列表                        ║
║                                                                      ║
║  学习目标:                                                           ║
║  1. 掌握统一初始化语法（花括号 {}）                                  ║
║  2. 深入理解 std::initializer_list                                  ║
║  3. 区分 () 和 {} 初始化的使用场景                                   ║
║  4. 掌握逆向双指针技巧（LeetCode 88、283）                           ║
║                                                                      ║
╚══════════════════════════════════════════════════════════════════════╝
)" << std::endl;
}

void print_menu() {
    std::cout << "\n";
    std::cout << "┌──────────────────────────────────────────────────────────────┐\n";
    std::cout << "│                       学习模块选择                            │\n";
    std::cout << "├──────────────────────────────────────────────────────────────┤\n";
    std::cout << "│  1. 统一初始化（Uniform Initialization）                     │\n";
    std::cout << "│  2. initializer_list 详解                                    │\n";
    std::cout << "│  3. () 与 {} 初始化对比                                       │\n";
    std::cout << "│  4. EMC++ 条款 7：区分 () 和 {} 初始化                        │\n";
    std::cout << "│  5. LeetCode 88：合并两个有序数组                            │\n";
    std::cout << "│  6. LeetCode 283：移动零                                     │\n";
    std::cout << "│  7. 运行所有模块                                             │\n";
    std::cout << "│  0. 退出                                                     │\n";
    std::cout << "└──────────────────────────────────────────────────────────────┘\n";
    std::cout << "请选择 (0-7): ";
}

void run_all_demos() {
    std::cout << "\n══════════════════════════════════════════════════════════════\n";
    std::cout << "                    运行所有演示模块                            \n";
    std::cout << "══════════════════════════════════════════════════════════════\n";

    cpp11_features::run_uniform_init_demo();
    cpp11_features::run_initializer_list_demo();
    cpp11_features::run_init_comparison_demo();
    emcpp::run_item07_demo();
    leetcode_0088::run_tests();
    leetcode_0283::run_tests();
}

int main(int argc, char* argv[]) {
    print_banner();

    // 如果有命令行参数，直接运行所有模块
    if (argc > 1 && std::string(argv[1]) == "--all") {
        run_all_demos();
        return 0;
    }

    // 交互式菜单
    while (true) {
        print_menu();

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1:
                cpp11_features::run_uniform_init_demo();
                break;
            case 2:
                cpp11_features::run_initializer_list_demo();
                break;
            case 3:
                cpp11_features::run_init_comparison_demo();
                break;
            case 4:
                emcpp::run_item07_demo();
                break;
            case 5:
                leetcode_0088::run_tests();
                break;
            case 6:
                leetcode_0283::run_tests();
                break;
            case 7:
                run_all_demos();
                break;
            case 0:
                std::cout << "\n感谢学习！再见！\n";
                return 0;
            default:
                std::cout << "无效选择，请重新输入。\n";
        }

        std::cout << "\n按 Enter 键继续...";
        std::cin.ignore();
        std::cin.get();
    }

    return 0;
}
