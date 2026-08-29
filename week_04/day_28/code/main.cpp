/**
 * @file main.cpp
 * @brief Day 28: 第四周复习 - 主程序入口
 * 
 * 本周复习内容：
 * 1. 哈希表数据结构
 * 2. 右值引用与移动语义
 * 3. 转发引用与完美转发
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
    bool verify_forwarding_contract();
    bool verify_special_member_stream_contract();
}

namespace emcpp_review {
    void demonstrate();
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
    if (!move_semantics_review::verify_forwarding_contract()) {
        std::cerr << "移动语义转发契约验证失败\n";
        return 1;
    }
    if (!move_semantics_review::verify_special_member_stream_contract()) {
        std::cerr << "资源特殊成员的流异常隔离契约验证失败\n";
        return 1;
    }
    std::cout << "[契约] 左值/右值已到达对应目标重载\n";
    std::cout << "[契约] 构造、复制与移动不依赖观察流\n";

    // ========================================
    // 第三部分：EMC++条款复习
    // ========================================
    printSeparator("第三部分：EMC++条款复习");
    emcpp_review::demonstrate();

    printSeparator("第四部分：缓存算法说明 (LC 146 / LC 460)");
    std::cout << "LRU/LFU 的独立测试请运行 day28_lc0146 和 day28_lc0460 可执行文件。\n";

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
    std::cout << "   - 值类别是表达式属性：左值有身份，右值包含纯右值与将亡值\n";
    std::cout << "   - std::move：产生 xvalue，但不执行或保证资源转移\n";
    std::cout << "   - 移动构造/移动赋值：可按类型契约复用或转移资源\n";
    std::cout << "   - Rule of Zero优先；直接管理资源时再系统检查Rule of Five\n\n";
    
    std::cout << "3. 转发引用与完美转发\n";
    std::cout << "   - 调用点推导、未加 cv 的模板参数 T 之精确 T&& 形参才是转发引用\n";
    std::cout << "   - auto&& 从普通表达式推导时有对应行为，直接大括号列表是例外\n";
    std::cout << "   - 引用折叠规则决定最终类型\n";
    std::cout << "   - 转发引用配合 std::forward，把原值类别送到真实目标重载\n\n";
    
    std::cout << "4. 缓存设计 (LRU/LFU)\n";
    std::cout << "   - LRU：哈希表 + 双向链表，哈希表假设下平均 O(1) 操作\n";
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
