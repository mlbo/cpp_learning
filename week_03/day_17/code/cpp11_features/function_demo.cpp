/**
 * std::function演示
 * 通用的函数包装器
 */

#include "function_demo.h"
#include <iostream>
#include <functional>
#include <map>
#include <string>

// 普通函数
int add(int a, int b) {
    return a + b;
}

// 函数对象
struct Multiplier {
    int factor;
    Multiplier(int f) : factor(f) {}
    int operator()(int x) const { return x * factor; }
};

// 带成员函数的类
struct Calculator {
    int base;
    Calculator(int b) : base(b) {}
    
    int add(int x) { return base + x; }
    int multiply(int x) { return base * x; }
};

void functionDemo() {
    std::cout << "╔══════════════════════════════════════╗" << std::endl;
    std::cout << "║        std::function 函数包装器      ║" << std::endl;
    std::cout << "╚══════════════════════════════════════╝" << std::endl;
    
    // ========== 1. 存储不同类型的可调用对象 ==========
    std::cout << "\n--- 1. 存储不同类型的可调用对象 ---" << std::endl;
    
    // 存储函数指针
    std::function<int(int, int)> f1 = add;
    std::cout << "  函数指针: add(3, 4) = " << f1(3, 4) << std::endl;
    
    // 存储Lambda
    std::function<int(int, int)> f2 = [](int a, int b) { return a * b; };
    std::cout << "  Lambda: 3 * 4 = " << f2(3, 4) << std::endl;
    
    // 存储函数对象
    std::function<int(int)> f3 = Multiplier(3);
    std::cout << "  函数对象: 5 * 3 = " << f3(5) << std::endl;
    
    // ========== 2. 存储成员函数 ==========
    std::cout << "\n--- 2. 存储成员函数 ---" << std::endl;
    
    Calculator calc(10);
    
    // 使用std::bind绑定成员函数
    std::function<int(int)> f4 = std::bind(&Calculator::add, &calc, std::placeholders::_1);
    std::cout << "  成员函数: 10 + 5 = " << f4(5) << std::endl;
    
    // Lambda方式
    std::function<int(int)> f5 = [&calc](int x) { return calc.multiply(x); };
    std::cout << "  Lambda包装: 10 * 5 = " << f5(5) << std::endl;
    
    // ========== 3. 回调函数模式 ==========
    std::cout << "\n--- 3. 回调函数模式 ---" << std::endl;
    
    auto processData = [](const std::vector<int>& data, std::function<void(int)> callback) {
        for (int n : data) {
            callback(n);
        }
    };
    
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    
    // 使用不同的回调
    std::cout << "  打印回调: ";
    processData(numbers, [](int n) { std::cout << n << " "; });
    std::cout << std::endl;
    
    int sum = 0;
    processData(numbers, [&sum](int n) { sum += n; });
    std::cout << "  求和回调: sum = " << sum << std::endl;
    
    // ========== 4. 函数表/命令模式 ==========
    std::cout << "\n--- 4. 函数表/命令模式 ---" << std::endl;
    
    std::map<std::string, std::function<int(int, int)>> operations;
    
    operations["add"] = [](int a, int b) { return a + b; };
    operations["sub"] = [](int a, int b) { return a - b; };
    operations["mul"] = [](int a, int b) { return a * b; };
    operations["div"] = [](int a, int b) { return b != 0 ? a / b : 0; };
    
    std::cout << "  add(10, 5) = " << operations["add"](10, 5) << std::endl;
    std::cout << "  sub(10, 5) = " << operations["sub"](10, 5) << std::endl;
    std::cout << "  mul(10, 5) = " << operations["mul"](10, 5) << std::endl;
    std::cout << "  div(10, 5) = " << operations["div"](10, 5) << std::endl;
    
    // ========== 5. 检查是否可调用 ==========
    std::cout << "\n--- 5. 检查是否可调用 ---" << std::endl;
    
    std::function<void()> f6;  // 空
    std::cout << "  空function: " << (f6 ? "可调用" : "不可调用") << std::endl;
    
    f6 = []() { std::cout << "Hello" << std::endl; };
    std::cout << "  赋值后: " << (f6 ? "可调用" : "不可调用") << std::endl;
    
    // ========== 6. 性能提示 ==========
    std::cout << "\n--- 6. 性能提示 ---" << std::endl;
    std::cout << "  std::function 有一定开销（类型擦除）" << std::endl;
    std::cout << "  对于性能敏感场景，优先使用：" << std::endl;
    std::cout << "    1. 模板参数（编译期多态）" << std::endl;
    std::cout << "    2. 函数指针（如果类型简单）" << std::endl;
    std::cout << "    3. auto + Lambda（避免类型擦除）" << std::endl;
}
