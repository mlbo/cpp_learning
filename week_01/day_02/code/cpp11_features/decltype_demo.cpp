/**
 * @file decltype_demo.cpp
 * @brief decltype关键字详解
 * 
 * 内容包括：
 * 1. decltype基本语法
 * 2. decltype推导规则
 * 3. decltype与auto的区别
 * 4. decltype典型应用场景
 */

#include <iostream>
#include <type_traits>
#include <vector>
#include <typeinfo>

namespace cpp11_features {

// ==================== 辅助函数：打印类型信息 ====================

template<typename T>
void print_type(const char* name) {
    std::cout << name << " 的类型: " << typeid(T).name() << std::endl;
}

template<typename T>
void print_type_info(const T& var, const char* var_name) {
    std::cout << var_name << " 的类型: " << typeid(var).name();
    
    if (std::is_reference_v<T>) {
        std::cout << " (引用)";
    }
    if (std::is_const_v<std::remove_reference_t<T>>) {
        std::cout << " (const)";
    }
    if (std::is_pointer_v<T>) {
        std::cout << " (指针)";
    }
    if (std::is_array_v<T>) {
        std::cout << " (数组)";
    }
    std::cout << std::endl;
}

// ==================== 1. decltype基本语法 ====================

/**
 * @brief 演示decltype基本用法
 */
void demonstrate_basic_usage() {
    std::cout << "=== decltype基本语法 ===" << std::endl;
    
    int x = 10;
    
    // decltype(变量名) 返回变量的精确类型
    decltype(x) a = x;           // a 是 int
    decltype((x)) b = x;         // b 是 int& (注意额外的括号!)
    
    std::cout << "decltype(x) = int" << std::endl;
    std::cout << "decltype((x)) = int&" << std::endl;
    
    // 验证类型
    static_assert(std::is_same_v<decltype(a), int>);
    static_assert(std::is_same_v<decltype(b), int&>);
    
    // 指针类型
    int* ptr = &x;
    decltype(ptr) p2 = ptr;      // p2 是 int*
    std::cout << "decltype(ptr) = int*" << std::endl;
    
    // 数组类型
    int arr[5] = {1, 2, 3, 4, 5};
    decltype(arr) arr2;          // arr2 是 int[5]
    std::cout << "decltype(arr) = int[5]" << std::endl;
    
    // 修改b会影响x
    b = 20;
    std::cout << "修改b后, x = " << x << std::endl;
}

// ==================== 2. decltype推导规则 ====================

/**
 * @brief 演示decltype的三种推导规则
 * 
 * 规则1: decltype(变量名) 返回变量的声明类型
 * 规则2: decltype(左值表达式) 返回 T&
 * 规则3: decltype(纯右值) 返回 T
 */
void demonstrate_deduction_rules() {
    std::cout << "\n=== decltype推导规则 ===" << std::endl;
    
    int x = 10;
    int& ref = x;
    
    // 规则1: 变量名 - 返回声明类型
    decltype(x) type1 = x;       // int
    decltype(ref) type2 = x;     // int&
    
    std::cout << "规则1 - 变量名返回声明类型" << std::endl;
    static_assert(std::is_same_v<decltype(type1), int>);
    static_assert(std::is_same_v<decltype(type2), int&>);
    
    // 规则2: 左值表达式 - 返回引用类型
    decltype((x)) type3 = x;     // int& (因为(x)是左值表达式)
    decltype(x + 1) type4 = 0;   // int (x+1是纯右值)
    
    std::cout << "规则2 - 左值表达式返回引用" << std::endl;
    static_assert(std::is_same_v<decltype(type3), int&>);
    static_assert(std::is_same_v<decltype(type4), int>);
    
    // 规则3: 纯右值 - 返回非引用类型
    decltype(42) type5 = 0;      // int
    decltype(x * x) type6 = 0;   // int
    
    std::cout << "规则3 - 纯右值返回非引用类型" << std::endl;
    static_assert(std::is_same_v<decltype(type5), int>);
    static_assert(std::is_same_v<decltype(type6), int>);
}

// ==================== 3. decltype与auto的区别 ====================

/**
 * @brief 演示decltype与auto的区别
 */
void demonstrate_decltype_vs_auto() {
    std::cout << "\n=== decltype vs auto ===" << std::endl;
    
    int x = 10;
    const int cx = 20;
    int& ref = x;
    
    // auto会忽略顶层const和引用
    auto a1 = x;          // int
    auto a2 = cx;         // int (忽略const)
    auto a3 = ref;        // int (忽略引用)
    
    // decltype保留精确类型
    decltype(x) d1 = x;   // int
    decltype(cx) d2 = 20; // const int (保留const)
    decltype(ref) d3 = x; // int& (保留引用)
    
    std::cout << "auto a2 的类型: int (忽略了const)" << std::endl;
    std::cout << "decltype(cx) 的类型: const int (保留const)" << std::endl;
    
    // 验证
    static_assert(std::is_same_v<decltype(a2), int>);
    static_assert(std::is_same_v<decltype(d2), const int>);
    
    // 数组处理差异
    int arr[5] = {1, 2, 3, 4, 5};
    
    auto a4 = arr;        // int* (数组退化为指针)
    decltype(arr) d4;     // int[5] (保留数组类型)
    
    std::cout << "\n数组处理差异:" << std::endl;
    std::cout << "auto数组推导: int* (退化)" << std::endl;
    std::cout << "decltype数组: int[5] (保留)" << std::endl;
    
    static_assert(std::is_same_v<decltype(a4), int*>);
    static_assert(std::is_same_v<decltype(d4), int[5]>);
}

// ==================== 4. decltype典型应用 ====================

/**
 * @brief 使用decltype进行返回类型推导
 */
template<typename T, typename U>
auto add(T t, U u) -> decltype(t + u) {
    return t + u;
}

/**
 * @brief 演示decltype的实际应用
 */
void demonstrate_applications() {
    std::cout << "\n=== decltype实际应用 ===" << std::endl;
    
    // 1. 返回类型尾置语法
    auto result = add(1, 2.5);
    std::cout << "decltype返回类型推导: add(1, 2.5) = " << result << std::endl;
    static_assert(std::is_same_v<decltype(result), double>);
    
    // 2. 用于泛型编程
    std::vector<int> vec = {1, 2, 3, 4, 5};
    
    // 使用decltype获取容器元素类型（注意：vec[0]返回引用）
    // 使用remove_reference获取实际元素类型
    using ValueType = std::remove_reference_t<decltype(vec[0])>;
    ValueType val = 42;
    std::cout << "使用decltype获取元素类型: " << val << std::endl;
    
    // 3. 获取表达式类型
    int x = 10;
    decltype(x * 1.5) computed = x * 1.5;  // double
    std::cout << "decltype(x * 1.5) 推导为 double: " << computed << std::endl;
    static_assert(std::is_same_v<decltype(computed), double>);
    
    // 4. 用于lambda表达式类型
    auto lambda = [](int a, int b) { return a + b; };
    decltype(lambda) lambda2 = lambda;
    std::cout << "lambda复制成功: lambda2(3,4) = " << lambda2(3, 4) << std::endl;
}

// ==================== 5. 常见陷阱 ====================

/**
 * @brief 演示decltype的常见陷阱
 */
void demonstrate_pitfalls() {
    std::cout << "\n=== decltype常见陷阱 ===" << std::endl;
    
    int x = 10;
    
    // 陷阱1: 额外括号的含义
    decltype(x) a = 0;      // int
    decltype((x)) b = x;    // int& (额外的括号改变含义!)
    
    std::cout << "陷阱1: decltype(x) vs decltype((x))" << std::endl;
    std::cout << "  decltype(x) -> int" << std::endl;
    std::cout << "  decltype((x)) -> int& (额外括号!" << std::endl;
    
    // 陷阱2: 对解引用指针的结果
    int* ptr = &x;
    decltype(*ptr) c = x;   // int& (解引用产生左值)
    
    std::cout << "\n陷阱2: decltype(*ptr) -> int&" << std::endl;
    static_assert(std::is_same_v<decltype(c), int&>);
}

// ==================== 主演示函数 ====================

void demonstrate_decltype() {
    demonstrate_basic_usage();
    demonstrate_deduction_rules();
    demonstrate_decltype_vs_auto();
    demonstrate_applications();
    demonstrate_pitfalls();
}

} // namespace cpp11_features
