/**
 * Lambda表达式复习
 */

#include "lambda_summary.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

void lambdaSummary() {
    std::cout << "╔══════════════════════════════════════╗" << std::endl;
    std::cout << "║        Lambda表达式总结              ║" << std::endl;
    std::cout << "╚══════════════════════════════════════╝" << std::endl;
    
    // ========== 语法回顾 ==========
    std::cout << "\n--- 语法回顾 ---" << std::endl;
    std::cout << "  完整语法: [capture](params) -> return_type { body }" << std::endl;
    
    // 示例
    auto add = [](int a, int b) { return a + b; };
    std::cout << "  [](int a, int b) { return a + b; }" << std::endl;
    std::cout << "  add(3, 4) = " << add(3, 4) << std::endl;
    
    // ========== 捕获方式 ==========
    std::cout << "\n--- 捕获方式 ---" << std::endl;
    
    int x = 10;
    int y = 20;
    
    // 值捕获
    auto valueCapture = [x]() { return x; };
    std::cout << "  值捕获 [x]: " << valueCapture() << std::endl;
    
    // 引用捕获
    auto refCapture = [&x]() { x = 100; };
    refCapture();
    std::cout << "  引用捕获 [&x]: x变为 " << x << std::endl;
    
    // 混合捕获
    auto mixed = [=, &y]() { y = x + y; };
    x = 10;  // 重置
    mixed();
    std::cout << "  混合捕获 [=, &y]: y变为 " << y << std::endl;
    
    std::cout << "\n  捕获规则：" << std::endl;
    std::cout << "    [x]     值捕获x" << std::endl;
    std::cout << "    [&x]    引用捕获x" << std::endl;
    std::cout << "    [=]     值捕获所有（不推荐）" << std::endl;
    std::cout << "    [&]     引用捕获所有（不推荐）" << std::endl;
    std::cout << "    [=, &x] 值捕获所有，x引用捕获" << std::endl;
    
    // ========== C++14特性 ==========
    std::cout << "\n--- C++14特性 ---" << std::endl;
    
    // 泛型Lambda
    auto genericAdd = [](auto a, auto b) { return a + b; };
    std::cout << "  泛型Lambda:" << std::endl;
    std::cout << "    add(1, 2) = " << genericAdd(1, 2) << std::endl;
    std::cout << "    add(1.5, 2.5) = " << genericAdd(1.5, 2.5) << std::endl;
    
    // 初始化捕获
    auto initCapture = [z = 30]() { return z; };
    std::cout << "  初始化捕获 [z = 30]: " << initCapture() << std::endl;
    
    // ========== 实际应用 ==========
    std::cout << "\n--- 实际应用 ---" << std::endl;
    
    std::vector<int> nums = {5, 2, 8, 1, 9};
    
    // 自定义排序
    std::sort(nums.begin(), nums.end(), [](int a, int b) { return a > b; });
    std::cout << "  降序排序: [";
    for (int n : nums) std::cout << n << " ";
    std::cout << "]" << std::endl;
    
    // 查找
    auto it = std::find_if(nums.begin(), nums.end(), [](int n) { return n < 3; });
    if (it != nums.end()) {
        std::cout << "  找第一个<3的数: " << *it << std::endl;
    }
    
    // ========== 最佳实践 ==========
    std::cout << "\n--- 最佳实践 ---" << std::endl;
    std::cout << "  1. 显式列出要捕获的变量" << std::endl;
    std::cout << "  2. 避免默认捕获 [=] 或 [&]" << std::endl;
    std::cout << "  3. 注意引用捕获的生命周期" << std::endl;
    std::cout << "  4. 需要修改捕获的值时使用 mutable" << std::endl;
    std::cout << "  5. C++14优先使用泛型Lambda" << std::endl;
}
