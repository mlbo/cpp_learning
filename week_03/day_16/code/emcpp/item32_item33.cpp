/**
 * EMC++ Item 32-33: Lambda高级用法
 * 
 * Item 32: 使用初始化捕获将对象移入闭包
 * Item 33: 对auto&&参数使用decltype来std::forward
 */

#include "item32_item33.h"
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <utility>
#include <functional>

// 演示Item 32：移动捕获
void demoItem32() {
    std::cout << "\n--- Item 32: 移动捕获 ---" << std::endl;
    
    // ========== 问题：C++11无法移动捕获 ==========
    std::cout << "  问题：传统捕获只能拷贝" << std::endl;
    
    // 假设有一个不可拷贝的对象
    auto uniqueInt = std::make_unique<int>(42);
    
    // C++11: 无法直接捕获unique_ptr
    // auto f = [uniqueInt] {};  // 错误：unique_ptr不可拷贝
    
    // ========== C++14解决方案：初始化捕获 ==========
    std::cout << "  C++14解决方案：初始化捕获" << std::endl;
    
    auto f = [ptr = std::move(uniqueInt)]() {
        std::cout << "  移动捕获的值: " << *ptr << std::endl;
    };
    
    std::cout << "  移动后原指针: " << (uniqueInt ? "有效" : "空") << std::endl;
    f();
    
    // ========== 移动大型容器 ==========
    std::vector<int> bigData(1000, 1);
    
    auto process = [data = std::move(bigData)]() {
        long long sum = 0;
        for (int n : data) sum += n;
        return sum;
    };
    
    std::cout << "  处理结果: " << process() << std::endl;
    std::cout << "  原容器大小: " << bigData.size() << std::endl;
    
    // ========== C++11变通方案：std::bind ==========
    std::cout << "  C++11变通：使用std::bind" << std::endl;
    
    auto movable = std::make_unique<std::string>("Hello");
    
    auto bound = std::bind(
        [](std::unique_ptr<std::string>& s) {
            std::cout << "  bind模拟移动: " << *s << std::endl;
        },
        std::move(movable)
    );
    
    bound();
}

// 演示Item 33：完美转发
void demoItem33() {
    std::cout << "\n--- Item 33: 泛型Lambda完美转发 ---" << std::endl;
    
    // ========== auto&& 转发引用 ==========
    
    // 被调用的函数
    auto processLvalue = [](int& x) {
        std::cout << "  左值引用: " << x << std::endl;
    };
    
    auto processRvalue = [](int&& x) {
        std::cout << "  右值引用: " << x << std::endl;
    };
    
    // 泛型Lambda中的完美转发
    auto forwarder = [](auto&& x) {
        // 使用decltype获取表达式的类型
        // 如果x绑定到左值，decltype(x)是T&
        // 如果x绑定到右值，decltype(x)是T&&
        std::cout << "  转发: " << x << std::endl;
        // 实际转发（需要C++14）
        // process(std::forward<decltype(x)>(x));
    };
    
    int a = 10;
    forwarder(a);      // 左值
    forwarder(20);     // 右值
    
    // ========== 实际应用：包装器 ==========
    std::cout << "  包装器示例:" << std::endl;
    
    // 通用的函数包装器
    auto wrapper = [](auto&&... args) {
        std::cout << "  接收到 " << sizeof...(args) << " 个参数" << std::endl;
        // 可以完美转发给其他函数
        // return func(std::forward<decltype(args)>(args)...);
    };
    
    wrapper(1, 2.0, std::string("three"));
    wrapper('a', 42);
    
    // ========== 区分左值和右值 ==========
    std::cout << "  区分左值右值:" << std::endl;
    
    auto classifier = [](auto&& x) {
        using T = decltype(x);
        if constexpr (std::is_lvalue_reference_v<T>) {
            std::cout << "  左值: " << x << std::endl;
        } else {
            std::cout << "  右值: " << x << std::endl;
        }
    };
    
    int lvalue = 100;
    classifier(lvalue);    // 左值
    classifier(200);       // 右值
}

void item32Item33Demo() {
    std::cout << "╔══════════════════════════════════════╗" << std::endl;
    std::cout << "║  EMC++ Item 32-33: Lambda高级用法    ║" << std::endl;
    std::cout << "╚══════════════════════════════════════╝" << std::endl;
    
    demoItem32();
    demoItem33();
    
    std::cout << "\n总结：" << std::endl;
    std::cout << "  Item 32:" << std::endl;
    std::cout << "    - 使用初始化捕获实现移动语义" << std::endl;
    std::cout << "    - 语法: [var = std::move(obj)]" << std::endl;
    std::cout << "    - 适用于不可拷贝对象和大型对象" << std::endl;
    std::cout << "  Item 33:" << std::endl;
    std::cout << "    - 泛型Lambda使用auto&&参数" << std::endl;
    std::cout << "    - 使用std::forward<decltype(x)>(x)完美转发" << std::endl;
    std::cout << "    - 可区分左值和右值" << std::endl;
}
