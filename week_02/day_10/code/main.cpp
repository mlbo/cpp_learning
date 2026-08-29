/**
 * @file main.cpp
 * @brief Day 10 主入口 - weak_ptr详解与链表双指针技巧
 */

#include <iostream>
#include <iomanip>

// C++11 特性演示
extern void demo_weak_ptr_basic();

// EMC++ Item 20-21 演示
extern void demo_emcpp_items20_21();

// LeetCode 题目演示
namespace leetcode_0142 {
bool test_leetcode_142();
}
namespace leetcode_0019 {
bool test_leetcode_19();
}

void print_section(const std::string& title) {
    std::cout << "\n" << std::string(50, '=') << "\n";
    std::cout << "  " << title << "\n";
    std::cout << std::string(50, '=') << "\n";
}

int main() {
    std::cout << "╔══════════════════════════════════════════════════╗\n";
    std::cout << "║  Day 10: weak_ptr详解与链表双指针技巧            ║\n";
    std::cout << "╚══════════════════════════════════════════════════╝\n";

    // Part 1: weak_ptr 基本用法
    print_section("Part 1: weak_ptr 基本用法");
    demo_weak_ptr_basic();

    // Part 2: EMC++ Item 20-21
    print_section("Part 2: EMC++ Item 20（weak_ptr）与 Item 21（make函数）");
    demo_emcpp_items20_21();

    // Part 3: LeetCode 142
    print_section("Part 3: LeetCode 142 - 环形链表II");
    const bool leetcode142Passed = leetcode_0142::test_leetcode_142();

    // Part 4: LeetCode 19
    print_section("Part 4: LeetCode 19 - 删除链表的倒数第N个节点");
    const bool leetcode19Passed = leetcode_0019::test_leetcode_19();

    std::cout << "\n✅ Day 10 所有演示完成！\n";
    return leetcode142Passed && leetcode19Passed ? 0 : 1;
}
