/**
 * @file item23_std_move.cpp
 * @brief EMC++ Item 23: 理解 std::move 和 std::forward
 * 
 * 本文件演示：
 * 1. std::move 的本质和工作原理
 * 2. std::forward 的本质和工作原理
 * 3. 两者的关键区别和使用场景
 */

#include <iostream>
#include <utility>
#include <string>
#include <type_traits>
#include <vector>

// ============================================================
// 辅助函数：打印类型信息
// ============================================================

// 获取类型的可读名称
template<typename T>
std::string typeName() {
    if constexpr (std::is_lvalue_reference_v<T>) {
        return "lvalue reference";
    } else if constexpr (std::is_rvalue_reference_v<T>) {
        return "rvalue reference";
    } else {
        return "non-reference";
    }
}

// ============================================================
// std::move 的本质演示
// ============================================================

void demonstrateStdMove() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "Item 23: std::move 的本质" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    std::cout << R"(
std::move 的本质：
-----------------
std::move 并不"移动"任何东西，它只是一个类型转换函数！

它的作用是将参数无条件地转换为右值引用。

源码实现（简化版）：
template<typename T>
typename std::remove_reference<T>::type&& move(T&& t) noexcept {
    return static_cast<typename std::remove_reference<T>::type&&>(t);
}

关键点：
1. 如果 T 是 int&，移除引用后变成 int，再加 && 变成 int&&
2. 如果 T 是 int&&，移除引用后变成 int，再加 && 变成 int&&
3. 如果 T 是 int，直接加 && 变成 int&&
)" << std::endl;
    
    // -------------------------------------------------------
    // 演示 std::move 的类型转换
    // -------------------------------------------------------
    std::cout << "\n【演示】std::move 的类型转换效果\n" << std::endl;
    
    int x = 42;
    
    // 使用 std::move
    int&& r1 = std::move(x);  // x 是左值，std::move(x) 返回 int&&
    std::cout << "int x = 42;" << std::endl;
    std::cout << "std::move(x) 的类型: int&&" << std::endl;
    std::cout << "r1 = " << r1 << std::endl;
    
    // 右值引用可直接绑定临时量，并延长这个临时量的生命周期。
    // 不写 int&& r2 = std::move(100)：函数返回的引用不会替调用者延长临时量生命周期。
    int&& r2 = 100;
    std::cout << "\nint&& r2 = 100;（右值不需要再 std::move）" << std::endl;
    std::cout << "r2 = " << r2 << std::endl;
    
    // -------------------------------------------------------
    // 演示 std::move 触发移动语义
    // -------------------------------------------------------
    std::cout << "\n【演示】std::move 触发移动语义\n" << std::endl;
    
    std::string str1 = "Hello";
    std::cout << "原始字符串: \"" << str1 << "\"" << std::endl;
    
    // 不使用 std::move - 拷贝
    std::string str2 = str1;
    std::cout << "拷贝后: str1 = \"" << str1 << "\", str2 = \"" << str2 << "\"" << std::endl;
    
    // 使用 std::move - 移动
    std::string str3 = std::move(str1);
    std::cout << "移动后: str1 = \"" << str1 << "\", str3 = \"" << str3 << "\"" << std::endl;
    
    std::cout << "\n注意：移动后 str1 有效但状态未指定；本次为空不构成可移植保证" << std::endl;
}

// ============================================================
// std::forward 的本质演示
// ============================================================

// 处理左值的函数
void process(const std::string& s) {
    std::cout << "  [左值版本] 处理: \"" << s << "\"" << std::endl;
}

// 处理右值的函数
void process(std::string&& s) {
    std::cout << "  [右值版本] 处理: \"" << s
              << "\" (已进入右值重载，是否移动由实现决定)" << std::endl;
}

// 演示 std::forward 的包装器
template<typename T>
void forwardWrapper(T&& arg) {
    std::cout << "\n调用 forwardWrapper:" << std::endl;
    std::cout << "  T 的类型: " << typeName<T>() << std::endl;
    std::cout << "  使用 std::forward<T>(arg):" << std::endl;
    process(std::forward<T>(arg));  // 完美转发
}

// 演示不使用 std::forward 的问题
template<typename T>
void badForward(T&& arg) {
    std::cout << "\n调用 badForward (不使用 forward):" << std::endl;
    // 命名参数变量的表达式 arg 是左值，因此会丢失调用点的值类别信息。
    process(arg);  // 总是调用左值版本！
}

void demonstrateStdForward() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "Item 23: std::forward 的本质" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    std::cout << R"(
std::forward 的本质：
--------------------
std::forward 是有条件的类型转换：T 记录左值来源时结果仍为左值，否则结果为 xvalue。

源码实现（简化版）：
template<typename T>
constexpr T&& forward(typename std::remove_reference<T>::type& t) noexcept {
    return static_cast<T&&>(t);
}

工作原理：
- 如果 T 是左值引用（如 int&），static_cast<int& &&> 折叠为 int&
- 如果 T 是非引用类型（如 int），static_cast<int&&> 保持为 int&&

关键区别：
- std::move: 无条件产生 xvalue，但不执行或保证资源移动
- std::forward: 有条件转换，保持原有值类别
)" << std::endl;
    
    // -------------------------------------------------------
    // 演示完美转发
    // -------------------------------------------------------
    std::cout << "\n【演示】完美转发\n" << std::endl;
    
    std::string leftValue = "左值字符串";
    
    std::cout << "传递左值:" << std::endl;
    forwardWrapper(leftValue);      // T = std::string&，转发为左值
    
    std::cout << "\n传递右值:" << std::endl;
    forwardWrapper(std::string("右值字符串"));  // T = std::string，转发为右值
    
    std::cout << "\n传递 std::move(左值):" << std::endl;
    forwardWrapper(std::move(leftValue));  // T = std::string，转发为右值
    
    // -------------------------------------------------------
    // 演示不使用 forward 的问题
    // -------------------------------------------------------
    std::cout << "\n【演示】不使用 forward 的问题\n" << std::endl;
    
    badForward(std::string("临时字符串"));  // 即使传入右值，也被当作左值处理
}

// ============================================================
// std::move vs std::forward 对比
// ============================================================

void demonstrateComparison() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "std::move vs std::forward 对比" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    std::cout << R"(
使用场景对比：
-------------

1. std::move 的使用场景：
   - 当你明确要转移所有权时
   - 参数是右值引用时（你确定它是右值）
   
   示例：
   void setName(std::string&& name) {
       name_ = std::move(name);  // OK: name 是右值引用
   }

2. std::forward 的使用场景：
   - 当你需要保持参数原有值类别时
   - 参数是通用引用（T&&）时
   
   示例：
   template<typename T>
   void setName(T&& name) {
       name_ = std::forward<T>(name);  // OK: 保持原有值类别
   }

常见错误：
----------
错误1：在通用引用上使用 std::move
template<typename T>
void bad(T&& arg) {
    process(std::move(arg));  // 错误：左值也被交给右值路径；目标决定是否真的移动
}

错误2：在右值引用上使用 std::forward（可以工作但不推荐）
void process(std::string&& s) {
    store(std::forward<std::string>(s));  // 可以，但 std::move 更清晰
}
)" << std::endl;
    
    // -------------------------------------------------------
    // 实际代码示例
    // -------------------------------------------------------
    std::cout << "\n【实际代码示例】\n" << std::endl;
    
    // 由于局部类不能有模板成员函数，这里使用非模板方式演示
    // 通用引用版本的实际效果
    
    std::string name = "MyWidget";
    std::cout << "通用引用版本:" << std::endl;
    std::cout << "  传入左值: \"" << name << "\" -> 调用拷贝构造" << std::endl;
    std::cout << "  原字符串保持: \"" << name << "\"" << std::endl;
    
    std::cout << "\n传入右值: std::string(\"TempWidget\") -> 调用移动构造" << std::endl;
    
    std::cout << "\n右值引用版本:" << std::endl;
    std::string desc = "A widget";
    std::string storedDesc = std::move(desc);
    std::cout << "  已真实执行 storedDesc = std::move(desc)" << std::endl;
    std::cout << "  目标字符串: \"" << storedDesc << "\"" << std::endl;
    std::cout << "  源字符串当前观察值: \"" << desc
              << "\"（仍有效但状态未指定，不能宣称必为空）" << std::endl;
}

bool verify_item23_move_observation_contract() {
    std::string source = "A widget";
    std::string destination = std::move(source);
    source = "reusable";
    return destination == "A widget" && source == "reusable";
}

// ============================================================
// 最佳实践总结
// ============================================================

void printBestPracticesItem23() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "Item 23 最佳实践总结" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    std::cout << R"(
记住这些规则：

1. std::move 是一个类型转换函数，不移动任何东西
   - 它将参数无条件转换为右值引用
   - 移动操作发生在移动构造函数/赋值运算符中

2. std::forward 是有条件的类型转换
   - 只在原始参数是右值时才转换为右值引用
   - 用于保持参数原有值类别

3. 选择原则：
   - 右值引用参数 -> 使用 std::move
   - 通用引用参数 -> 使用 std::forward

4. 命名约定：
   - std::move 的名字容易误导，它不"移动"
   - 更准确的名字应该是 std::rvalue_cast 或 std::move_cast

5. 移动后的对象状态：
   - 标准库对象移动后通常有效但状态未指定
   - 可以安全析构或重新赋值
   - 不应依赖其值
)" << std::endl;
}

// ============================================================
// 入口函数
// ============================================================

/**
 * @brief Item 23 演示入口函数
 */
void run_item23_demo() {
    demonstrateStdMove();
    demonstrateStdForward();
    demonstrateComparison();
    printBestPracticesItem23();
}
