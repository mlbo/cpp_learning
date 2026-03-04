/**
 * @file item24_universal_reference.cpp
 * @brief EMC++ Item 24: 区分通用引用和右值引用
 * 
 * 本文件演示：
 * 1. 如何识别通用引用（转发引用）
 * 2. 通用引用与右值引用的关键区别
 * 3. 常见的识别陷阱
 */

#include <iostream>
#include <utility>
#include <string>
#include <vector>
#include <type_traits>

// ============================================================
// 辅助类型特征
// ============================================================

// 打印类型的值类别
template<typename T>
void printValueType(T&& param) {
    std::cout << "  T 的类型: ";
    if constexpr (std::is_lvalue_reference_v<T>) {
        std::cout << "左值引用";
    } else if constexpr (std::is_rvalue_reference_v<T>) {
        std::cout << "右值引用";
    } else {
        std::cout << "非引用类型";
    }
    std::cout << std::endl;
    
    std::cout << "  param 的类型: ";
    using ParamType = decltype(param);
    if constexpr (std::is_lvalue_reference_v<ParamType>) {
        std::cout << "左值引用";
    } else {
        std::cout << "右值引用";
    }
    std::cout << std::endl;
}

// ============================================================
// 通用引用演示
// ============================================================

void demonstrateUniversalReference() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "Item 24: 通用引用（转发引用）" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    std::cout << R"(
什么是通用引用（Universal Reference）？
--------------------------------------
通用引用是 Scott Meyers 提出的概念，标准中称为"转发引用"（Forwarding Reference）。

识别通用引用的两个必要条件：
1. 类型声明形式必须是 T&&（T 是模板参数）
2. 必须发生类型推导

通用引用可以绑定到：
- 左值：T 被推导为左值引用，引用折叠后成为左值引用
- 右值：T 被推导为非引用类型，最终成为右值引用
)" << std::endl;
    
    // -------------------------------------------------------
    // 演示通用引用的行为
    // -------------------------------------------------------
    std::cout << "\n【演示】通用引用绑定左值和右值\n" << std::endl;
    
    auto testUniversalRef = [](auto&& param) {
        std::cout << "传入参数: " << param << std::endl;
        printValueType(param);
    };
    
    int x = 42;
    std::cout << "传入左值 x = 42:" << std::endl;
    testUniversalRef(x);
    
    std::cout << "\n传入右值 100:" << std::endl;
    testUniversalRef(100);
    
    std::cout << "\n传入 std::move(x):" << std::endl;
    testUniversalRef(std::move(x));
}

// ============================================================
// 通用引用 vs 右值引用
// ============================================================

// 这是一个通用引用！
template<typename T>
void universalRef(T&& param) {
    std::cout << "  [universalRef] ";
    if constexpr (std::is_lvalue_reference_v<T>) {
        std::cout << "接收到左值引用" << std::endl;
    } else {
        std::cout << "接收到右值引用" << std::endl;
    }
}

// 这是一个右值引用！
void rvalueRef(int&& param) {
    std::cout << "  [rvalueRef] 接收到右值引用: " << param << std::endl;
}

// 这也是一个右值引用！（非 T&& 形式）
template<typename T>
void notUniversalRef(const std::vector<T>&& param) {
    std::cout << "  [notUniversalRef] 接收到右值引用 vector" << std::endl;
}

// 类成员函数中的右值引用
template<typename T>
class Widget {
public:
    // 这不是通用引用！因为 T 在类实例化时已确定
    void process(T&& param) {
        std::cout << "  [Widget::process] ";
        if constexpr (std::is_rvalue_reference_v<T>) {
            std::cout << "T 是右值引用类型" << std::endl;
        } else if constexpr (std::is_lvalue_reference_v<T>) {
            std::cout << "T 是左值引用类型" << std::endl;
        } else {
            std::cout << "T 是非引用类型，param 是右值引用" << std::endl;
        }
    }
    
    // 这才是通用引用！
    template<typename U>
    void realUniversalRef(U&& param) {
        std::cout << "  [Widget::realUniversalRef] ";
        if constexpr (std::is_lvalue_reference_v<U>) {
            std::cout << "U 被推导为左值引用" << std::endl;
        } else {
            std::cout << "U 被推导为非引用类型" << std::endl;
        }
    }
};

void demonstrateDifference() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "通用引用 vs 右值引用的区别" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    // -------------------------------------------------------
    // 演示通用引用
    // -------------------------------------------------------
    std::cout << "\n【演示 1】通用引用可以接受左值和右值\n" << std::endl;
    
    int x = 10;
    universalRef(x);            // 接受左值
    universalRef(20);           // 接受右值
    universalRef(std::move(x)); // 接受右值
    
    // -------------------------------------------------------
    // 演示右值引用只能接受右值
    // -------------------------------------------------------
    std::cout << "\n【演示 2】右值引用只能接受右值\n" << std::endl;
    
    // rvalueRef(x);  // 编译错误！不能绑定左值
    rvalueRef(30);            // OK
    rvalueRef(std::move(x));  // OK
    
    // -------------------------------------------------------
    // 演示类成员函数的情况
    // -------------------------------------------------------
    std::cout << "\n【演示 3】类模板中的成员函数\n" << std::endl;
    
    Widget<int> w1;  // T = int
    std::cout << "Widget<int> w1:" << std::endl;
    w1.process(100);      // param 类型是 int&&（右值引用）
    // w1.process(x);     // 编译错误！int&& 不能绑定左值
    
    Widget<int&> w2;  // T = int&
    std::cout << "\nWidget<int&> w2:" << std::endl;
    int y = 200;
    w2.process(y);        // param 类型是 int& && -> int&（左值引用）
    
    // 真正的通用引用
    std::cout << "\n真正的通用引用成员函数:" << std::endl;
    w1.realUniversalRef(y);          // 左值
    w1.realUniversalRef(300);        // 右值
    w1.realUniversalRef(std::move(y));  // 右值
}

// ============================================================
// 常见陷阱
// ============================================================

void demonstratePitfalls() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "常见识别陷阱" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    std::cout << R"(
陷阱 1：const 修饰符破坏通用引用
---------------------------------
template<typename T>
void notUniversal(const T&& param);  // 这是右值引用！

const 使 T&& 不再是通用引用，因为通用引用需要能够绑定到可修改的值。

陷阱 2：非模板参数
------------------
template<typename T>
class Container {
    void push_back(T&& item);   // 这是右值引用！T 在类实例化时已确定
};

陷阱 3：嵌套模板参数
--------------------
template<typename T>
void process(std::vector<T>&& vec);  // 这是右值引用！不是 T&& 形式

陷阱 4：auto&& 是通用引用
-------------------------
auto&& x = expr;  // 这是通用引用！

auto&& 遵循相同的规则，可以绑定左值或右值。
)" << std::endl;
    
    // auto&& 示例
    std::cout << "\n【auto&& 示例】\n" << std::endl;
    
    int value = 100;
    
    auto&& r1 = value;       // 绑定左值，r1 类型为 int&
    auto&& r2 = 200;         // 绑定右值，r2 类型为 int&&
    auto&& r3 = std::move(value);  // 绑定右值，r3 类型为 int&&
    
    std::cout << "auto&& r1 = value;  // r1 是左值引用" << std::endl;
    std::cout << "auto&& r2 = 200;    // r2 是右值引用" << std::endl;
    std::cout << "auto&& r3 = std::move(value);  // r3 是右值引用" << std::endl;
}

// ============================================================
// 引用折叠规则
// ============================================================

void explainReferenceCollapsing() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "引用折叠规则" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    std::cout << R"(
引用折叠规则：
-------------
当形成多重引用时，编译器会进行折叠：

1. T& &   -> T&    (左值引用 + 左值引用 = 左值引用)
2. T& &&  -> T&    (左值引用 + 右值引用 = 左值引用)
3. T&& &  -> T&    (右值引用 + 左值引用 = 左值引用)
4. T&& && -> T&&   (右值引用 + 右值引用 = 右值引用)

简记：只有两个右值引用折叠成右值引用，其他都是左值引用。

这解释了为什么通用引用能工作：

当传入左值 int x：
  - T 被推导为 int&
  - T&& 变成 int& &&
  - 折叠后为 int&（左值引用）

当传入右值 int 42：
  - T 被推导为 int
  - T&& 变成 int&&（右值引用）
)" << std::endl;
    
    // 静态断言验证
    static_assert(std::is_same_v<int&, std::add_lvalue_reference_t<int&>>);
    static_assert(std::is_same_v<int&, std::add_rvalue_reference_t<int&>>);
    static_assert(std::is_same_v<int&, std::add_lvalue_reference_t<int&&>>);
    static_assert(std::is_same_v<int&&, std::add_rvalue_reference_t<int>>);
    
    std::cout << "\n✓ 引用折叠规则验证通过！" << std::endl;
}

// ============================================================
// 最佳实践
// ============================================================

void printBestPracticesItem24() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "Item 24 最佳实践总结" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    std::cout << R"(
记住这些规则：

1. 识别通用引用的两个条件：
   - 形式必须是 T&&（模板参数 + &&）
   - 必须发生类型推导

2. 通用引用的行为：
   - 绑定左值时，T 推导为左值引用
   - 绑定右值时，T 推导为非引用类型

3. 右值引用的限制：
   - 只能绑定右值
   - 形式不是 T&& 时一定是右值引用

4. 常见陷阱：
   - const T&& 不是通用引用
   - 类成员 T&& 如果 T 在实例化时确定，不是通用引用
   - std::vector<T>&& 不是通用引用

5. auto&& 是通用引用：
   - 范围 for 循环中常用
   - 可以安全地绑定任何表达式
)" << std::endl;
}

// ============================================================
// 入口函数
// ============================================================

/**
 * @brief Item 24 演示入口函数
 */
void run_item24_demo() {
    demonstrateUniversalReference();
    demonstrateDifference();
    demonstratePitfalls();
    explainReferenceCollapsing();
    printBestPracticesItem24();
}
