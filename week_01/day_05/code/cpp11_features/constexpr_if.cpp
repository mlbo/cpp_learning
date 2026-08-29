/**
 * @file constexpr_if.cpp
 * @brief if constexpr详解（C++17）
 * 
 * if constexpr允许在编译期进行条件判断，
 * 只编译符合条件的分支代码。
 */

#include <iostream>
#include <type_traits>
#include <string>
#include <vector>
#include <array>
#include <limits>
#include <stdexcept>

#include "../constexpr_math.h"

using namespace std;

// ==================== 基本用法 ====================

// 根据类型选择不同处理方式
template <typename T>
auto get_value(T t) {
    if constexpr (is_pointer_v<T>) {
        cout << "  [指针类型] 解引用: ";
        if (t == nullptr) {
            throw invalid_argument("get_value cannot dereference a null pointer");
        }
        return *t;
    } else {
        cout << "  [值类型] 直接返回: ";
        return t;
    }
}

// 根据类型输出信息
template <typename T>
void print_type_info(T value) {
    cout << "  类型: ";
    
    if constexpr (is_integral_v<T>) {
        cout << "整数, 值: " << value;
    } else if constexpr (is_floating_point_v<T>) {
        cout << "浮点数, 值: " << value;
    } else if constexpr (is_same_v<T, string>) {
        cout << "字符串, 值: \"" << value << "\"";
    } else if constexpr (is_pointer_v<T>) {
        cout << "指针, 地址: " << static_cast<void*>(value);
    } else {
        cout << "其他类型";
    }
    
    cout << "\n";
}

// ==================== 类型特征判断 ====================

// 编译期类型检查
template <typename T>
void analyze_type() {
    cout << "  分析类型 " << typeid(T).name() << ":\n";
    
    if constexpr (is_const_v<T>) {
        cout << "    - 是const类型\n";
    } else {
        cout << "    - 不是const类型\n";
    }
    
    if constexpr (is_reference_v<T>) {
        cout << "    - 是引用类型\n";
    } else {
        cout << "    - 不是引用类型\n";
    }
    
    if constexpr (is_pointer_v<T>) {
        cout << "    - 是指针类型\n";
    } else {
        cout << "    - 不是指针类型\n";
    }
    
    if constexpr (is_array_v<T>) {
        cout << "    - 是数组类型\n";
        cout << "    - 数组大小: " << extent_v<T> << "\n";
    }
    
    if constexpr (is_integral_v<T>) {
        cout << "    - 是整数类型\n";
        cout << "    - 字节数: " << sizeof(T) << "\n";
    }
}

// ==================== 编译期算法 ====================

// 编译期斐波那契
template <int N>
constexpr int fibonacci() {
    static_assert(N >= 0 && N <= 30,
                  "recursive fibonacci<int> demo requires 0 <= N <= 30");
    if constexpr (N <= 1) {
        return N;
    } else {
        return day05::checked_add(fibonacci<N - 1>(), fibonacci<N - 2>());
    }
}

// 编译期阶乘
template <int N>
constexpr int factorial() {
    static_assert(N >= 0 && N <= day05::max_factorial_input,
                  "factorial<int> requires 0 <= N <= 12");
    if constexpr (N <= 1) {
        return 1;
    } else {
        return day05::checked_multiply(N, factorial<N - 1>());
    }
}

// 编译期幂运算
template <int Base, int Exp>
constexpr int power() {
    static_assert(Exp >= 0 && Exp <= 64,
                  "this recursive integer power demo requires 0 <= Exp <= 64");
    if constexpr (Exp == 0) {
        return 1;
    } else {
        return day05::checked_multiply(Base, power<Base, Exp - 1>());
    }
}

// ==================== 递归展开 ====================

// 打印任意数量参数
template <typename T>
void print_args(T arg) {
    cout << arg;
}

template <typename T, typename... Args>
void print_args(T first, Args... rest) {
    cout << first << ", ";
    if constexpr (sizeof...(rest) > 0) {
        print_args(rest...);
    }
}

// 求和任意数量参数
template <typename T>
constexpr T sum_all(T value) {
    return value;
}

template <typename T, typename... Args>
constexpr auto sum_all(T first, Args... rest) {
    if constexpr (sizeof...(rest) == 0) {
        return first;
    } else {
        using Result = common_type_t<T, Args...>;
        const auto tail = sum_all(rest...);
        if constexpr (is_integral_v<Result>) {
            static_assert(is_same_v<Result, int>,
                          "the checked integral sum_all demo intentionally supports int only");
            return day05::checked_add(static_cast<int>(first), static_cast<int>(tail));
        } else {
            return static_cast<Result>(first) + static_cast<Result>(tail);
        }
    }
}

// ==================== 容器处理 ====================

// 通用容器打印
template <typename Container>
void print_container(const Container& c) {
    if constexpr (is_array_v<Container>) {
        cout << "  数组: [";
        for (size_t i = 0; i < extent_v<Container>; ++i) {
            if (i > 0) cout << ", ";
            cout << c[i];
        }
    } else {
        cout << "  容器(size=" << c.size() << "): [";
        bool first = true;
        for (const auto& elem : c) {
            if (!first) cout << ", ";
            cout << elem;
            first = false;
        }
    }
    cout << "]\n";
}

// ==================== 条件编译示例 ====================

// 编译期选择实现
template <typename T>
constexpr T abs_value(T x) {
    if constexpr (is_signed_v<T>) {
        if constexpr (is_integral_v<T>) {
            if (x == numeric_limits<T>::min()) {
                throw overflow_error("abs_value result does not fit in its signed type");
            }
        }
        return x < 0 ? -x : x;
    } else {
        return x;  // 无符号类型不需要处理
    }
}

// 编译期字符串处理
template <size_t N>
constexpr bool is_palindrome(const char (&str)[N]) {
    if constexpr (N <= 2) {
        return true;
    } else {
        for (size_t i = 0; i < (N - 1) / 2; ++i) {
            if (str[i] != str[N - 2 - i]) {
                return false;
            }
        }
        return true;
    }
}

// ==================== 演示函数 ====================

void demonstrate_basic_usage() {
    cout << "【基本用法：类型判断】\n\n";
    
    int value = 42;
    int* ptr = &value;
    
    cout << "get_value测试:\n";
    get_value(value);
    cout << value << "\n";
    get_value(ptr);
    cout << *ptr << "\n";
    
    cout << "\nprint_type_info测试:\n";
    print_type_info(42);
    print_type_info(3.14);
    print_type_info(string("hello"));
    print_type_info(&value);
}

void demonstrate_type_traits() {
    cout << "\n【类型特征判断】\n\n";
    
    cout << "analyze_type<int>():\n";
    analyze_type<int>();
    
    cout << "\nanalyze_type<const int&>():\n";
    analyze_type<const int&>();
    
    cout << "\nanalyze_type<int*>():\n";
    analyze_type<int*>();
    
    cout << "\nanalyze_type<int[5]>():\n";
    analyze_type<int[5]>();
}

void demonstrate_compile_time_algorithms() {
    cout << "\n【编译期算法】\n\n";
    
    cout << "编译期斐波那契:\n";
    cout << "  fibonacci<0>() = " << fibonacci<0>() << "\n";
    cout << "  fibonacci<1>() = " << fibonacci<1>() << "\n";
    cout << "  fibonacci<5>() = " << fibonacci<5>() << "\n";
    cout << "  fibonacci<10>() = " << fibonacci<10>() << "\n";
    
    cout << "\n编译期阶乘:\n";
    cout << "  factorial<0>() = " << factorial<0>() << "\n";
    cout << "  factorial<1>() = " << factorial<1>() << "\n";
    cout << "  factorial<5>() = " << factorial<5>() << "\n";
    cout << "  factorial<10>() = " << factorial<10>() << "\n";
    
    cout << "\n编译期幂运算:\n";
    cout << "  power<2, 0>() = " << power<2, 0>() << "\n";
    cout << "  power<2, 10>() = " << power<2, 10>() << "\n";
    cout << "  power<3, 4>() = " << power<3, 4>() << "\n";
}

void demonstrate_variadic() {
    cout << "\n【可变参数模板】\n\n";
    
    cout << "print_args测试:\n";
    cout << "  ";
    print_args(1);
    cout << "\n  ";
    print_args(1, 2, 3);
    cout << "\n  ";
    print_args("hello", 42, 3.14, 'c');
    cout << "\n";
    
    cout << "\nsum_all测试:\n";
    cout << "  sum_all(1) = " << sum_all(1) << "\n";
    cout << "  sum_all(1, 2, 3) = " << sum_all(1, 2, 3) << "\n";
    cout << "  sum_all(1.5, 2.5, 3.0) = " << sum_all(1.5, 2.5, 3.0) << "\n";
}

void demonstrate_condition_compilation() {
    cout << "\n【条件编译示例】\n\n";
    
    cout << "abs_value测试:\n";
    cout << "  abs_value(-42) = " << abs_value(-42) << "\n";
    cout << "  abs_value(42u) = " << abs_value(42u) << "\n";
    
    cout << "\nis_palindrome测试:\n";
    constexpr bool p1 = is_palindrome("a");
    constexpr bool p2 = is_palindrome("aba");
    constexpr bool p3 = is_palindrome("abba");
    constexpr bool p4 = is_palindrome("abc");
    
    cout << "  is_palindrome(\"a\") = " << (p1 ? "true" : "false") << "\n";
    cout << "  is_palindrome(\"aba\") = " << (p2 ? "true" : "false") << "\n";
    cout << "  is_palindrome(\"abba\") = " << (p3 ? "true" : "false") << "\n";
    cout << "  is_palindrome(\"abc\") = " << (p4 ? "true" : "false") << "\n";
}

// ==================== if constexpr vs if 对比 ====================

void demonstrate_if_constexpr_vs_if() {
    cout << "\n【if constexpr vs if 对比】\n\n";
    
    cout << "普通if：\n";
    cout << "  - 条件在运行时计算\n";
    cout << "  - 两个分支都会被编译\n";
    cout << "  - 可能产生无用代码\n\n";
    
    cout << "if constexpr：\n";
    cout << "  - 条件在编译时计算\n";
    cout << "  - 未选中分支在模板实例化时被丢弃\n";
    cout << "  - 可避免实例化与当前类型不匹配的依赖代码\n";
    cout << "  - 但源码仍需可解析，与模板参数无关的错误仍会被诊断\n";
    
    // 示例：普通if无法这样写
    // template <typename T>
    // void bad_example(T t) {
    //     if (is_pointer_v<T>) {
    //         cout << *t;  // 如果T不是指针，这行编译失败！
    //     }
    // }
    
    // if constexpr可以
    cout << "\n示例：只有if constexpr可以这样用:\n";
    cout << "  template <typename T>\n";
    cout << "  void good_example(T t) {\n";
    cout << "      if constexpr (is_pointer_v<T>) {\n";
    cout << "          cout << *t;  // 只有指针类型才编译这行\n";
    cout << "      }\n";
    cout << "  }\n";
}

bool demonstrate_boundary_contracts() {
    cout << "\n【模板边界契约】\n";
    bool passed = true;
    int* null_pointer = nullptr;
    try {
        (void)get_value(null_pointer);
        cerr << "  [FAIL] get_value 未拒绝空指针\n";
        passed = false;
    } catch (const invalid_argument&) {
        cout << "  [PASS] get_value 拒绝空指针\n";
    }

    try {
        (void)abs_value(numeric_limits<int>::min());
        cerr << "  [FAIL] abs_value 未拒绝 INT_MIN\n";
        passed = false;
    } catch (const overflow_error&) {
        cout << "  [PASS] abs_value 拒绝 INT_MIN\n";
    }

    try {
        (void)sum_all(numeric_limits<int>::max(), 1);
        cerr << "  [FAIL] sum_all 未拒绝 int 溢出\n";
        passed = false;
    } catch (const overflow_error&) {
        cout << "  [PASS] sum_all 拒绝 int 溢出\n";
    }
    return passed;
}

// ==================== main函数 ====================

int main() {
    cout << "╔════════════════════════════════════════════════════════════╗\n";
    cout << "║              if constexpr详解（C++17）                      ║\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n\n";
    
    demonstrate_basic_usage();
    demonstrate_type_traits();
    demonstrate_compile_time_algorithms();
    demonstrate_variadic();
    demonstrate_condition_compilation();
    demonstrate_if_constexpr_vs_if();
    const bool boundaries_passed = demonstrate_boundary_contracts();
    
    cout << "\n╔════════════════════════════════════════════════════════════╗\n";
    cout << "║     if constexpr要点：                                      ║\n";
    cout << "║     1. 编译期条件判断（C++17）                              ║\n";
    cout << "║     2. 未选中分支不实例化依赖代码                           ║\n";
    cout << "║     3. 常用于模板元编程                                     ║\n";
    cout << "║     4. 不会屏蔽语法或非依赖错误                             ║\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n";
    
    return boundaries_passed ? 0 : 1;
}
