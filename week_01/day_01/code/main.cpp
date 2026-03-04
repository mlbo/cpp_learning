/**
 * @file main.cpp
 * @brief Day 1 主程序入口
 * 
 * 学习内容：
 * 1. 复杂度分析
 * 2. auto类型推导
 * 3. EMC++条款1-5
 * 4. LeetCode两数之和
 */

#include <iostream>
#include <vector>

// 声明外部函数
void demonstrate_complexity();
void demonstrate_auto();
void demonstrate_type_deduction();

// LeetCode测试
namespace LC0001 { void test_two_sum(); }
namespace LC0167 { void test_two_sum_ii(); }

int main() {
    std::cout << "========================================\n";
    std::cout << "  Day 1: C++学习之旅开始！\n";
    std::cout << "========================================\n\n";

    // 1. 复杂度分析演示
    std::cout << ">>> 1. 复杂度分析演示 <<<\n";
    demonstrate_complexity();
    std::cout << "\n";

    // 2. auto类型推导演示
    std::cout << ">>> 2. auto类型推导演示 <<<\n";
    demonstrate_auto();
    std::cout << "\n";

    // 3. EMC++类型推导条款演示
    std::cout << ">>> 3. EMC++类型推导条款 <<<\n";
    demonstrate_type_deduction();
    std::cout << "\n";

    // 4. LeetCode题目测试
    std::cout << ">>> 4. LeetCode题目测试 <<<\n";
    LC0001::test_two_sum();
    LC0167::test_two_sum_ii();
    std::cout << "\n";

    std::cout << "========================================\n";
    std::cout << "  Day 1 学习完成！\n";
    std::cout << "========================================\n";

    return 0;
}
