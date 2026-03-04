/**
 * Day 15: 栈入门与Lambda表达式
 * 主程序入口
 */

#include <iostream>
#include "data_structure/stack_demo.h"
#include "cpp11_features/lambda_demo.h"
#include "cpp11_features/lambda_capture_demo.h"
#include "emcpp/item31_avoid_default_capture.h"
#include "leetcode/0020_valid_parentheses/solution.h"
#include "leetcode/1047_remove_adjacent_duplicates/solution.h"

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  Day 15: 栈入门与Lambda表达式" << std::endl;
    std::cout << "========================================" << std::endl;

    // 1. 栈演示
    std::cout << "\n【1. 栈数据结构演示】" << std::endl;
    stackDemo();

    // 2. Lambda基础
    std::cout << "\n【2. Lambda表达式基础】" << std::endl;
    lambdaBasicsDemo();

    // 3. Lambda捕获
    std::cout << "\n【3. Lambda捕获方式】" << std::endl;
    lambdaCaptureDemo();

    // 4. EMC++ Item 31
    std::cout << "\n【4. EMC++ Item 31: 避免默认捕获】" << std::endl;
    item31Demo();

    // 5. LeetCode 20 测试
    std::cout << "\n【5. LeetCode 20: 有效的括号】" << std::endl;
    testValidParentheses();

    // 6. LeetCode 1047 测试
    std::cout << "\n【6. LeetCode 1047: 删除相邻重复项】" << std::endl;
    testRemoveDuplicates();

    std::cout << "\n========================================" << std::endl;
    std::cout << "  Day 15 学习完成！" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}
