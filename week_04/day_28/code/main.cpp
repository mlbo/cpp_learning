/**
 * @file main.cpp
 * @brief Day 28: 第四周复习 - 主程序入口
 * 
 * 本周复习内容：
 * 1. 哈希表数据结构
 * 2. 右值引用与移动语义
 * 3. 通用引用与完美转发
 * 4. EMC++ Item 9, 23-30
 * 5. LRU/LFU缓存设计
 */

#include <iostream>
#include <string>
#include <vector>

// 声明各模块的演示函数
namespace hash_table_review {
    void demonstrate();
}

namespace move_semantics_review {
    void demonstrate();
}

namespace emcpp_review {
    void demonstrate();
}

namespace lru_cache_test {
    void test();
}

namespace lfu_cache_test {
    void test();
}

// 打印分隔线
void printSeparator(const std::string& title) {
    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "  " << title << "\n";
    std::cout << "========================================\n\n";
}

// 打印子标题
void printSubTitle(const std::string& title) {
    std::cout << "\n--- " << title << " ---\n\n";
}

int main() {
    std::cout << "╔════════════════════════════════════════╗\n";
    std::cout << "║   Day 28: 第四周复习                   ║\n";
    std::cout << "║   主题：哈希表 + 移动语义              ║\n";
    std::cout << "╚════════════════════════════════════════╝\n";

    // ========================================
    // 第一部分：哈希表复习
    // ========================================
    printSeparator("第一部分：哈希表复习");
    hash_table_review::demonstrate();

    // ========================================
    // 第二部分：移动语义复习
    // ========================================
    printSeparator("第二部分：移动语义复习");
    move_semantics_review::demonstrate();

    // ========================================
    // 第三部分：EMC++条款复习
    // ========================================
    printSeparator("第三部分：EMC++条款复习");
    emcpp_review::demonstrate();

    // ========================================
    // 第四部分：LRU缓存测试
    // ========================================
    printSeparator("第四部分：LRU缓存测试 (LC 146)");
    lru_cache_test::test();

    // ========================================
    // 第五部分：LFU缓存测试
    // ========================================
    printSeparator("第五部分：LFU缓存测试 (LC 460)");
    lfu_cache_test::test();

    // ========================================
    // 本周总结
    // ========================================
    printSeparator("本周学习总结");
    
    std::cout << "【本周核心知识点】\n\n";
    
    std::cout << "1. 哈希表\n";
    std::cout << "   - 平均O(1)时间复杂度的查找、插入、删除\n";
    std::cout << "   - 哈希函数设计是关键\n";
    std::cout << "   - 冲突解决：链地址法 vs 开放寻址法\n";
    std::cout << "   - 装载因子影响性能，需要动态扩容\n\n";
    
    std::cout << "2. 移动语义\n";
    std::cout << "   - 左值 vs 右值：有名字/无名字，可取地址/不可取地址\n";
    std::cout << "   - std::move：将左值转换为右值引用\n";
    std::cout << "   - 移动构造/移动赋值：资源所有权转移\n";
    std::cout << "   - Rule of Five：析构、拷贝、移动五大函数\n\n";
    
    std::cout << "3. 通用引用与完美转发\n";
    std::cout << "   - T&& 在类型推导上下文中是通用引用\n";
    std::cout << "   - 引用折叠规则决定最终类型\n";
    std::cout << "   - std::forward 实现完美转发\n\n";
    
    std::cout << "4. 缓存设计 (LRU/LFU)\n";
    std::cout << "   - LRU：哈希表 + 双向链表，O(1)操作\n";
    std::cout << "   - LFU：多层频率结构，复杂但高效\n";
    std::cout << "   - 面试高频题，需要熟练掌握\n\n";

    std::cout << "【下周预告】\n";
    std::cout << "第五周将学习：\n";
    std::cout << "- 二叉树数据结构\n";
    std::cout << "- 树的遍历（前序/中序/后序）\n";
    std::cout << "- DFS算法\n";
    std::cout << "- 并发编程基础\n";

    std::cout << "\n========================================\n";
    std::cout << "Day 28 学习完成！\n";
    std::cout << "========================================\n";

    return 0;
}
