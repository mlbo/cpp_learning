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
#include <type_traits>

namespace {

void receive(int& value) {
    std::cout << "  target 收到左值引用: " << value << std::endl;
}

void receive(int&& value) {
    std::cout << "  target 收到右值引用: " << value << std::endl;
}

}  // namespace

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

    // 泛型Lambda中的完美转发
    auto forwarder = [](auto&& x) {
        // 命名变量 x 本身永远是左值表达式；decltype(x) 保留推导出的引用类型。
        receive(std::forward<decltype(x)>(x));
    };

    int a = 10;
    forwarder(a);   // decltype(x) 为 int&，仍转发成左值
    forwarder(20);  // decltype(x) 为 int&&，恢复成右值

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
    std::cout << "    - 命名参数是左值，必须用std::forward恢复调用者的值类别" << std::endl;
    std::cout << "    - 使用std::forward<decltype(x)>(x)完美转发" << std::endl;
    std::cout << "    - 可区分左值和右值" << std::endl;
}
