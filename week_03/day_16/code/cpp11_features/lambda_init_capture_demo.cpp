/**
 * Lambda初始化捕获演示（C++14）
 * 在捕获列表中初始化新变量
 */

#include "lambda_init_capture.h"
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <functional>

void lambdaInitCaptureDemo() {
    std::cout << "╔══════════════════════════════════════╗" << std::endl;
    std::cout << "║     Lambda 初始化捕获 (C++14)        ║" << std::endl;
    std::cout << "╚══════════════════════════════════════╝" << std::endl;
    
    // ========== 1. 初始化捕获基础 ==========
    std::cout << "\n--- 1. 初始化捕获基础 ---" << std::endl;
    
    // 在捕获列表中创建新变量
    auto f1 = [x = 10]() {
        return x * 2;
    };
    std::cout << "  f1() = " << f1() << std::endl;  // 20
    
    // 基于外部变量初始化
    int y = 5;
    auto f2 = [z = y + 10]() {  // z = 15
        return z;
    };
    std::cout << "  f2() = " << f2() << std::endl;  // 15
    std::cout << "  原变量y = " << y << std::endl;  // 5（不受影响）
    
    // ========== 2. 移动捕获 ==========
    std::cout << "\n--- 2. 移动捕获 ---" << std::endl;
    
    auto ptr = std::make_unique<int>(42);
    std::cout << "  移动前 ptr " << (ptr ? "有效" : "无效") << std::endl;
    
    // C++14: 使用std::move进行移动捕获
    auto f3 = [p = std::move(ptr)]() {
        std::cout << "  Lambda内 *p = " << *p << std::endl;
    };
    
    std::cout << "  移动后 ptr " << (ptr ? "有效" : "无效") << std::endl;
    f3();
    
    // ========== 3. 捕获大型对象 ==========
    std::cout << "\n--- 3. 捕获大型对象 ---" << std::endl;
    
    std::vector<int> largeVector = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // 移动捕获避免拷贝
    auto processVector = [v = std::move(largeVector)]() {
        int sum = 0;
        for (int n : v) {
            sum += n;
        }
        return sum;
    };
    
    std::cout << "  向量元素和 = " << processVector() << std::endl;
    std::cout << "  原向量大小 = " << largeVector.size() << " (已移动)" << std::endl;
    
    // ========== 4. 引用捕获 + 初始化 ==========
    std::cout << "\n--- 4. 引用捕获 + 初始化 ---" << std::endl;
    
    int counter = 0;
    
    // 创建一个副本用于计数，同时引用原变量
    auto incrementAndReport = [&counter, times = 0]() mutable {
        ++counter;
        ++times;
        std::cout << "  第" << times << "次调用，counter = " << counter << std::endl;
    };
    
    incrementAndReport();
    incrementAndReport();
    incrementAndReport();
    
    // ========== 5. 复杂表达式初始化 ==========
    std::cout << "\n--- 5. 复杂表达式初始化 ---" << std::endl;
    
    std::string prefix = "Hello";
    std::string suffix = "World";
    
    // 在捕获时组合字符串
    auto greeter = [msg = prefix + ", " + suffix + "!"]() {
        std::cout << "  " << msg << std::endl;
    };
    
    greeter();
    
    // 捕获计算结果
    auto calculator = [result = [](int a, int b) { return a * b; }(3, 4)]() {
        std::cout << "  计算结果 = " << result << std::endl;
    };
    calculator();
    
    // ========== 6. 初始化捕获 vs 传统捕获对比 ==========
    std::cout << "\n--- 6. 初始化捕获 vs 传统捕获 ---" << std::endl;
    
    // 传统：无法移动
    std::shared_ptr<int> sharedP = std::make_shared<int>(100);
    auto oldWay = [sharedP]() {  // 拷贝shared_ptr，引用计数+1
        std::cout << "  传统捕获: " << *sharedP << " (use_count = " 
                  << sharedP.use_count() << ")" << std::endl;
    };
    oldWay();
    
    // 初始化捕获：可以移动
    auto newWay = [p = std::move(sharedP)]() mutable {
        std::cout << "  移动捕获: " << *p << " (use_count = " 
                  << p.use_count() << ")" << std::endl;
    };
    newWay();
    
    // ========== 7. C++11 变通方案 ==========
    std::cout << "\n--- 7. C++11 变通方案（使用std::bind）---" << std::endl;
    
    std::vector<int> data = {1, 2, 3};
    
    // C++11: 使用bind模拟移动捕获
    auto f = std::bind(
        [](const std::vector<int>& v) {
            std::cout << "  向量大小 = " << v.size() << std::endl;
        },
        std::move(data)
    );
    
    f();
    std::cout << "  原数据大小 = " << data.size() << std::endl;
}
