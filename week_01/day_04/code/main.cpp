/**
 * @file main.cpp
 * @brief Day 4: nullptr详解与双指针算法 - 主程序入口
 * 
 * 本程序演示：
 * 1. C++11 nullptr特性
 * 2. EMC++条款8：优先使用nullptr
 * 3. LeetCode双指针算法实现
 */

#include <iostream>
#include <string>
#include <vector>

// 前向声明
void run_nullptr_demo();
void run_nullptr_vs_null();
void run_nullptr_overload();
void run_item08_demo();
void run_leetcode11_tests();
void run_leetcode15_tests();

// 打印分隔线
void print_separator(const std::string& title) {
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║  " << title;
    // 计算需要的空格数
    int padding = 58 - static_cast<int>(title.length());
    for (int i = 0; i < padding; ++i) {
        std::cout << " ";
    }
    std::cout << "║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n\n";
}

int main() {
    std::cout << R"(
╔══════════════════════════════════════════════════════════════════╗
║                                                                  ║
║     Day 4: nullptr详解与双指针算法                                ║
║                                                                  ║
║     - C++11 nullptr特性                                         ║
║     - EMC++ 条款8: 优先使用nullptr                               ║
║     - LeetCode 11: 盛最多水的容器                                ║
║     - LeetCode 15: 三数之和                                     ║
║                                                                  ║
╚══════════════════════════════════════════════════════════════════╝
)" << std::endl;

    // Part 1: nullptr基本用法演示
    print_separator("Part 1: nullptr 基本用法演示");
    run_nullptr_demo();

    // Part 2: nullptr vs NULL对比
    print_separator("Part 2: nullptr vs NULL 对比");
    run_nullptr_vs_null();

    // Part 3: 重载中的优势
    print_separator("Part 3: 函数重载中的优势");
    run_nullptr_overload();

    // Part 4: EMC++ 条款8演示
    print_separator("Part 4: EMC++ 条款8演示");
    run_item08_demo();

    // Part 5: LeetCode 11 测试
    print_separator("Part 5: LeetCode 11 - 盛最多水的容器");
    run_leetcode11_tests();

    // Part 6: LeetCode 15 测试
    print_separator("Part 6: LeetCode 15 - 三数之和");
    run_leetcode15_tests();

    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                    🎉 Day 4 学习完成！                       ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n";
    std::cout << "\n";

    return 0;
}
