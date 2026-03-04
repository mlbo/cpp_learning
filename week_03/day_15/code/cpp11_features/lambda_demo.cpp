/**
 * Lambda表达式基础演示
 */

#include "lambda_demo.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

void lambdaBasicsDemo() {
    std::cout << "╔══════════════════════════════════════╗" << std::endl;
    std::cout << "║        Lambda 表达式基础             ║" << std::endl;
    std::cout << "╚══════════════════════════════════════╝" << std::endl;
    
    // ========== 1. 最简单的Lambda ==========
    std::cout << "\n--- 1. 最简单的Lambda ---" << std::endl;
    
    auto sayHello = []() {
        std::cout << "  Hello, Lambda!" << std::endl;
    };
    sayHello();
    
    // ========== 2. 带参数的Lambda ==========
    std::cout << "\n--- 2. 带参数的Lambda ---" << std::endl;
    
    auto add = [](int a, int b) {
        return a + b;
    };
    std::cout << "  3 + 4 = " << add(3, 4) << std::endl;
    
    // ========== 3. 指定返回类型 ==========
    std::cout << "\n--- 3. 指定返回类型 ---" << std::endl;
    
    auto divide = [](double a, double b) -> double {
        if (b == 0) return 0;
        return a / b;
    };
    std::cout << "  10.0 / 3.0 = " << divide(10.0, 3.0) << std::endl;
    
    // ========== 4. 在STL算法中使用 ==========
    std::cout << "\n--- 4. 在STL算法中使用 ---" << std::endl;
    
    std::vector<int> nums = {5, 2, 8, 1, 9, 3};
    
    // 使用Lambda排序（降序）
    std::sort(nums.begin(), nums.end(), [](int a, int b) {
        return a > b;
    });
    
    std::cout << "  降序排序: ";
    for (int n : nums) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
    
    // ========== 5. Lambda语法总结 ==========
    std::cout << "\n--- 5. Lambda语法总结 ---" << std::endl;
    std::cout << "  完整语法: [capture](params) -> return_type { body }" << std::endl;
    std::cout << "  示例:     [=](int x) -> double { return x * 1.5; }" << std::endl;
}
