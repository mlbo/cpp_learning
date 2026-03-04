/**
 * Day 18: 函数调用栈与enum class
 * 主程序入口
 */

#include <iostream>
#include "low_level/call_stack_demo.h"
#include "cpp11_features/enum_class_demo.h"
#include "emcpp/item10_enum_class.h"

void testLargestRectangle();
void testTrap();

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  Day 18: 函数调用栈与enum class" << std::endl;
    std::cout << "========================================" << std::endl;

    // 1. 函数调用栈演示
    std::cout << "\n【1. 函数调用栈演示】" << std::endl;
    callStackDemo();

    // 2. enum class演示
    std::cout << "\n【2. enum class演示】" << std::endl;
    enumClassDemo();

    // 3. EMC++ Item 10
    std::cout << "\n【3. EMC++ Item 10】" << std::endl;
    item10Demo();

    // 4. LeetCode 84 测试
    std::cout << "\n【4. LeetCode 84: 柱状图中最大矩形】" << std::endl;
    testLargestRectangle();

    // 5. LeetCode 42 测试
    std::cout << "\n【5. LeetCode 42: 接雨水】" << std::endl;
    testTrap();

    std::cout << "\n========================================" << std::endl;
    std::cout << "  Day 18 学习完成！" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}
