/**
 * @file main.cpp
 * @brief Day 10 主入口 - weak_ptr详解与链表双指针技巧
 */

#include <iostream>
#include <iomanip>

// C++11 特性演示
extern void demo_weak_ptr_basic();

// EMC++ 条款演示
extern void demo_item21_weak_ptr();

// LeetCode 题目演示
extern void test_leetcode_142();
extern void test_leetcode_19();

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

    // Part 2: EMC++ 条款21
    print_section("Part 2: EMC++ 条款21 - shared_ptr与weak_ptr配合");
    demo_item21_weak_ptr();

    // Part 3: LeetCode 142
    print_section("Part 3: LeetCode 142 - 环形链表II");
    test_leetcode_142();

    // Part 4: LeetCode 19
    print_section("Part 4: LeetCode 19 - 删除链表的倒数第N个节点");
    test_leetcode_19();

    std::cout << "\n✅ Day 10 所有演示完成！\n";
    return 0;
}
