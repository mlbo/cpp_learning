/**
 * @file main.cpp
 * @brief Day 9 主程序入口
 * 
 * 今日主题：
 * 1. 链表操作技巧（快慢指针、虚拟头节点）
 * 2. shared_ptr 详解
 * 3. EMC++ 条款 19-20
 * 4. LeetCode 21、141
 */

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// 数据结构模块
void demoFastSlowPointer();
void demoDummyNode();

// C++11特性模块
void demoSharedPtrBasic();
void demoSharedPtrAdvanced();
void demoCircularReference();

// EMC++模块
void demoItem19Item20();

// LeetCode模块
void testMergeLists();
void testLinkedListCycle();

// 辅助函数
void printSection(const std::string& title) {
    std::cout << "\n";
    std::cout << "══════════════════════════════════════════════════════════════\n";
    std::cout << "  " << title << "\n";
    std::cout << "══════════════════════════════════════════════════════════════\n\n";
}

int main() {
    std::cout << R"(
╔══════════════════════════════════════════════════════════════╗
║                    Day 9: 链表与智能指针                      ║
║                                                              ║
║  数据结构：快慢指针、虚拟头节点                                ║
║  C++11特性：shared_ptr详解                                   ║
║  EMC++条款：19-20（shared_ptr资源管理）                       ║
║  LeetCode：21题、141题                                       ║
╚══════════════════════════════════════════════════════════════╝
)";

    // ========== 数据结构演示 ==========
    printSection("Part 1: 快慢指针技巧");
    demoFastSlowPointer();
    
    printSection("Part 2: 虚拟头节点技巧");
    demoDummyNode();
    
    // ========== C++11特性演示 ==========
    printSection("Part 3: shared_ptr 基础用法");
    demoSharedPtrBasic();
    
    printSection("Part 4: shared_ptr 高级特性");
    demoSharedPtrAdvanced();
    
    printSection("Part 5: 循环引用问题预览");
    demoCircularReference();
    
    // ========== EMC++演示 ==========
    printSection("Part 6: EMC++ 条款19-20");
    demoItem19Item20();
    
    // ========== LeetCode题解 ==========
    printSection("Part 7: LeetCode 21 - 合并两个有序链表");
    testMergeLists();
    
    printSection("Part 8: LeetCode 141 - 环形链表");
    testLinkedListCycle();
    
    // ========== 总结 ==========
    printSection("今日总结");
    std::cout << R"(
✓ 快慢指针：找中点、判环、找环入口
✓ 虚拟头节点：统一边界条件处理
✓ shared_ptr：引用计数、控制块、make_shared
✓ EMC++ 19-20：共享所有权管理与性能权衡
✓ LeetCode 21：链表合并（迭代/递归）
✓ LeetCode 141：快慢指针判环

预习 Day 10：
- weak_ptr 与循环引用解决
- EMC++ 条款21-22
- LeetCode 142、234
)";

    return 0;
}
