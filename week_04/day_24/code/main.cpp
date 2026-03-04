/**
 * @file main.cpp
 * @brief Day 24: 通用引用 (Universal Reference) - 主程序入口
 * 
 * 本程序演示：
 * 1. 通用引用的概念和用法
 * 2. 引用折叠规则
 * 3. EMC++ Item 26-28 的实践应用
 * 4. LeetCode 49 和 128 的题解
 * 
 * 编译方法：
 *   mkdir -p build && cd build
 *   cmake ..
 *   make -j$(nproc)
 *   ./day_24_demo
 */

#include <iostream>
#include <string>

// ==================== C++11 特性演示函数声明 ====================

// 通用引用演示
void runUniversalRefDemo();

// 引用折叠演示
void runReferenceCollapsingDemo();

// ==================== EMC++ 演示函数声明 ====================

// Item 26: 避免在通用引用上重载
void runItem26Demo();

// Item 27: 理解引用折叠规则
void runItem27Demo();

// Item 28: 理解引用折叠的上下文
void runItem28Demo();

// ==================== LeetCode 演示函数声明 ====================

// LC 49: 字母异位词分组
void testGroupAnagrams();

// LC 128: 最长连续序列
void testLongestConsecutive();

// ==================== 主程序 ====================

int main() {
    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "   Day 24: 通用引用 (Universal Reference)\n";
    std::cout << "========================================\n";
    std::cout << "\n";
    
    std::cout << "本日学习内容:\n";
    std::cout << "  1. 通用引用 (Universal Reference / 转发引用)\n";
    std::cout << "  2. 引用折叠 (Reference Collapsing)\n";
    std::cout << "  3. EMC++ Item 26-28\n";
    std::cout << "  4. LeetCode 49 & 128\n";
    std::cout << "\n";
    
    // ==================== 第一部分：C++11 特性演示 ====================
    
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║          第一部分：C++11 特性演示                          ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    
    // 1. 通用引用演示
    runUniversalRefDemo();
    
    // 2. 引用折叠演示
    runReferenceCollapsingDemo();
    
    // ==================== 第二部分：EMC++ 条款演示 ====================
    
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║          第二部分：EMC++ Item 26-28 演示                   ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    
    // Item 26: 避免在通用引用上重载
    runItem26Demo();
    
    // Item 27: 理解引用折叠规则
    runItem27Demo();
    
    // Item 28: 理解引用折叠的上下文
    runItem28Demo();
    
    // ==================== 第三部分：LeetCode 刷题 ====================
    
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║          第三部分：LeetCode 刷题                           ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    
    // LC 49: 字母异位词分组
    testGroupAnagrams();
    
    // LC 128: 最长连续序列
    testLongestConsecutive();
    
    // ==================== 总结 ====================
    
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                   今日学习总结                             ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    
    std::cout << "\n核心要点:\n";
    std::cout << "  1. 通用引用 T&& 需要：类型推导 + 精确的 T&& 形式\n";
    std::cout << "  2. 引用折叠规则：有左值引用参与 → 折叠为左值引用\n";
    std::cout << "  3. 避免在通用引用上重载，使用标签分发或 SFINAE\n";
    std::cout << "  4. std::forward 利用引用折叠实现完美转发\n";
    
    std::cout << "\n关键记忆口诀:\n";
    std::cout << "  - 通用引用是「智能快递员」，能接收左值或右值\n";
    std::cout << "  - 引用折叠中，左值引用是「强力胶」，粘住就变左值\n";
    
    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "   Day 24 演示程序运行完成！\n";
    std::cout << "========================================\n";
    std::cout << "\n";
    
    return 0;
}
