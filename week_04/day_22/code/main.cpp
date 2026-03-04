/**
 * Day 22: 哈希表入门
 * 主程序入口
 */

#include <iostream>
#include "data_structure/hash_table_demo.h"
#include "cpp11_features/rvalue_reference_demo.h"
#include "emcpp/item09_type_alias.h"

void testValidAnagram();
void testRansomNote();

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  Day 22: 哈希表入门" << std::endl;
    std::cout << "========================================" << std::endl;

    // 1. 哈希表演示
    std::cout << "\n【1. 哈希表数据结构演示】" << std::endl;
    hashTableDemo();

    // 2. 右值引用演示
    std::cout << "\n【2. 右值引用演示】" << std::endl;
    rvalueReferenceDemo();

    // 3. EMC++ Item 09 演示
    std::cout << "\n【3. EMC++ Item 09: 类型别名】" << std::endl;
    typeAliasDemo();

    // 4. LeetCode 242 测试
    std::cout << "\n【4. LeetCode 242: 有效的字母异位词】" << std::endl;
    testValidAnagram();

    // 5. LeetCode 383 测试
    std::cout << "\n【5. LeetCode 383: 赎金信】" << std::endl;
    testRansomNote();

    std::cout << "\n========================================" << std::endl;
    std::cout << "  Day 22 学习完成！" << std::endl;
    std::cout << "  今日收获：" << std::endl;
    std::cout << "  - 哈希表原理与C++实现" << std::endl;
    std::cout << "  - 右值引用与移动语义基础" << std::endl;
    std::cout << "  - using类型别名优于typedef" << std::endl;
    std::cout << "  - 哈希表解决字符统计问题" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}
