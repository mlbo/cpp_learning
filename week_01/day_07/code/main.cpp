/**
 * @file main.cpp
 * @brief Day 7 主程序入口 - 第一周复习与综合练习
 */

#include <iostream>
#include <iomanip>
#include <chrono>

// 复习模块
void type_deduction_review();
void init_review();
void pointer_review();

// 项目模块
void dynamic_array_test();

int main() {
    std::cout << R"(
╔══════════════════════════════════════════════════════════════╗
║                   Day 7: 第一周复习与综合练习                  ║
║                                                              ║
║  📚 复习内容: 类型推导 | 初始化 | 指针与智能指针               ║
║  🚀 综合项目: DynamicArray 动态数组类                         ║
║  💻 LeetCode: 42.接雨水 | 189.轮转数组                        ║
╚══════════════════════════════════════════════════════════════╝
)" << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    // 1. 类型推导复习
    std::cout << "\n┌─────────────────────────────────────────────────────┐\n";
    std::cout << "│  📝 第一部分: 类型推导复习                            │\n";
    std::cout << "└─────────────────────────────────────────────────────┘\n";
    type_deduction_review();

    // 2. 初始化复习
    std::cout << "\n┌─────────────────────────────────────────────────────┐\n";
    std::cout << "│  📝 第二部分: 初始化方式复习                          │\n";
    std::cout << "└─────────────────────────────────────────────────────┘\n";
    init_review();

    // 3. 指针复习
    std::cout << "\n┌─────────────────────────────────────────────────────┐\n";
    std::cout << "│  📝 第三部分: 指针与智能指针复习                      │\n";
    std::cout << "└─────────────────────────────────────────────────────┘\n";
    pointer_review();

    // 4. 综合项目测试
    std::cout << "\n┌─────────────────────────────────────────────────────┐\n";
    std::cout << "│  🚀 综合项目: DynamicArray 测试                      │\n";
    std::cout << "└─────────────────────────────────────────────────────┘\n";
    dynamic_array_test();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "\n╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║  ✅ Day 7 练习完成!                                          ║\n";
    std::cout << "║  ⏱️  总耗时: " << std::setw(5) << duration.count() << " ms                                    ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n";

    return 0;
}
