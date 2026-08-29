/**
 * Day 18: 函数调用栈与enum class
 * 主程序入口
 */

#include <iostream>
#include "low_level/call_stack_demo.h"
#include "low_level/stack_frame.h"
#include "cpp11_features/enum_class_demo.h"
#include "cpp11_features/enum_vs_enum_class.h"
#include "emcpp/item10_enum_class.h"
#include "leetcode/0042_trapping_rain_water/solution.h"
#include "leetcode/0084_largest_rectangle/solution.h"

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  Day 18: 函数调用栈与enum class" << std::endl;
    std::cout << "========================================" << std::endl;

    // 1. 函数调用栈演示
    std::cout << "\n【1. 函数调用栈演示】" << std::endl;
    callStackDemo();

    // 2. 栈帧观察
    std::cout << "\n【2. 栈帧观察（实现相关）】" << std::endl;
    run_stack_frame_demo();

    // 3. enum class演示
    std::cout << "\n【3. enum class演示】" << std::endl;
    enumClassDemo();

    // 4. 传统 enum 与 enum class 对比
    std::cout << "\n【4. 传统 enum 与 enum class 对比】" << std::endl;
    run_enum_vs_enum_class_demo();

    // 5. EMC++ Item 10
    std::cout << "\n【5. EMC++ Item 10】" << std::endl;
    item10Demo();

    // 6. LeetCode 84 测试
    std::cout << "\n【6. LeetCode 84: 柱状图中最大矩形】" << std::endl;
    testLargestRectangle();

    // 7. LeetCode 42 测试
    std::cout << "\n【7. LeetCode 42: 接雨水】" << std::endl;
    testTrap();

    std::cout << "\n========================================" << std::endl;
    std::cout << "  Day 18 学习完成！" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}
