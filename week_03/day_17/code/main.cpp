/**
 * Day 17: 单调栈与function/bind
 * 主程序入口
 */

#include <iostream>
#include "algorithm/monotonic_stack_demo.h"
#include "cpp11_features/function_demo.h"
#include "cpp11_features/bind_demo.h"
#include "emcpp/item34_lambda_vs_bind.h"
#include "leetcode/0496_next_greater_element/solution.h"
#include "leetcode/0739_daily_temperatures/solution.h"

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  Day 17: 单调栈与function/bind" << std::endl;
    std::cout << "========================================" << std::endl;

    // 1. 单调栈演示
    std::cout << "\n【1. 单调栈算法演示】" << std::endl;
    monotonicStackDemo();

    // 2. std::function演示
    std::cout << "\n【2. std::function演示】" << std::endl;
    functionDemo();

    // 3. std::bind演示
    std::cout << "\n【3. std::bind演示】" << std::endl;
    bindDemo();

    // 4. EMC++ Item 34
    std::cout << "\n【4. EMC++ Item 34: Lambda vs bind】" << std::endl;
    item34Demo();

    // 5. LeetCode 739 测试
    std::cout << "\n【5. LeetCode 739: 每日温度】" << std::endl;
    const bool lc739Passed = lc739::testDailyTemperatures();

    // 6. LeetCode 496 测试
    std::cout << "\n【6. LeetCode 496: 下一个更大元素】" << std::endl;
    const bool lc496Passed = lc496::testNextGreaterElement();

    std::cout << "\n========================================" << std::endl;
    const bool allPassed = lc739Passed && lc496Passed;
    std::cout << "  Day 17 测试" << (allPassed ? "全部通过！" : "存在失败！") << std::endl;
    std::cout << "========================================" << std::endl;

    return allPassed ? 0 : 1;
}
