/**
 * EMC++ Item 31-34 复习
 */

#include "emcpp_review.h"
#include <iostream>
#include <memory>
#include <functional>
#include <vector>

void emcppReview() {
    std::cout << "╔══════════════════════════════════════╗" << std::endl;
    std::cout << "║    EMC++ Item 31-34 复习             ║" << std::endl;
    std::cout << "╚══════════════════════════════════════╝" << std::endl;
    
    // ========== Item 31 ==========
    std::cout << "\n--- Item 31: 避免默认捕获模式 ---" << std::endl;
    
    std::cout << "  问题：默认捕获可能导致意外行为" << std::endl;
    std::cout << "    [=] 值捕获指针 -> 悬空指针风险" << std::endl;
    std::cout << "    [&] 引用捕获 -> 悬空引用风险" << std::endl;
    
    std::cout << "\n  解决方案：显式列出捕获变量" << std::endl;
    
    int a = 10, b = 20;
    // 推荐
    auto good = [a, &b]() {
        b = a + b;
    };
    good();
    std::cout << "    [a, &b]() { ... } - 清晰明确" << std::endl;
    
    // ========== Item 32 ==========
    std::cout << "\n--- Item 32: 使用初始化捕获移动对象 ---" << std::endl;
    
    auto ptr = std::make_unique<int>(42);
    std::cout << "  C++14初始化捕获：" << std::endl;
    std::cout << "    [p = std::move(ptr)]() { ... }" << std::endl;
    
    auto f = [p = std::move(ptr)]() {
        std::cout << "    Lambda内: *p = " << *p << std::endl;
    };
    f();
    std::cout << "  移动后原指针: " << (ptr ? "有效" : "空") << std::endl;
    
    std::cout << "\n  C++11变通方案：std::bind" << std::endl;
    std::cout << "    std::bind(func, std::move(ptr))" << std::endl;
    
    // ========== Item 33 ==========
    std::cout << "\n--- Item 33: auto&& + decltype完美转发 ---" << std::endl;
    
    std::cout << "  泛型Lambda完美转发：" << std::endl;
    std::cout << "    [](auto&& x) {" << std::endl;
    std::cout << "      func(std::forward<decltype(x)>(x));" << std::endl;
    std::cout << "    }" << std::endl;
    
    auto forwarder = [](auto&& x) {
        // x绑定到左值时，decltype(x)是T&
        // x绑定到右值时，decltype(x)是T&&
        std::cout << "    转发值: " << x << std::endl;
    };
    
    int value = 100;
    forwarder(value);   // 左值
    forwarder(200);     // 右值
    
    // ========== Item 34 ==========
    std::cout << "\n--- Item 34: 优先Lambda而非std::bind ---" << std::endl;
    
    std::cout << "  Lambda优势：" << std::endl;
    std::cout << "    1. 更好的可读性" << std::endl;
    std::cout << "    2. 更好的性能（可能内联）" << std::endl;
    std::cout << "    3. 更强的表达能力" << std::endl;
    std::cout << "    4. 支持泛型（C++14）" << std::endl;
    
    // 对比
    auto addLambda = [](int a, int b) { return a + b; };
    auto addBind = std::bind(std::plus<int>(), std::placeholders::_1, std::placeholders::_2);
    
    std::cout << "\n  Lambda: [](int a, int b) { return a + b; }" << std::endl;
    std::cout << "  bind:   std::bind(std::plus<int>(), _1, _2)" << std::endl;
    std::cout << "  结果: " << addLambda(3, 4) << " vs " << addBind(3, 4) << std::endl;
    
    // ========== 总结 ==========
    std::cout << "\n--- 总结 ---" << std::endl;
    std::cout << "  Item 31: 显式捕获，不用默认捕获" << std::endl;
    std::cout << "  Item 32: 初始化捕获实现移动" << std::endl;
    std::cout << "  Item 33: auto&& + decltype完美转发" << std::endl;
    std::cout << "  Item 34: Lambda比bind更好" << std::endl;
}
