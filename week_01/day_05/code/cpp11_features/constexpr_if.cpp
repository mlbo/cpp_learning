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

using namespace std;

// ==================== 基本用法 ====================

// 根据类型选择不同处理方式
template <typename T>
auto get_value(T t) {
    if constexpr (is_pointer_v<T>) {
        cout << "  [指针类型] 解引用: ";
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
    if constexpr (N <= 1) {
        return N;
    } else {
        return fibonacci<N - 1>() + fibonacci<N - 2>();
    }
}

// 编译期阶乘
template <int N>
constexpr int factorial() {
    if constexpr (N <= 1) {
        return 1;
    } else {
        return N * factorial<N - 1>();
    }
}

// 编译期幂运算
template <int Base, int Exp>
constexpr int power() {
    if constexpr (Exp == 0) {
        return 1;
    } else if constexpr (Exp < 0) {
        return 1.0 / power<Base, -Exp>();
    } else {
        return Base * power<Base, Exp - 1>();
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
        return first + sum_all(rest...);
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
    cout << "  - 只有符合条件的分支被编译\n";
    cout << "  - 可以避免编译错误\n";
    
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
    
    cout << "\n╔════════════════════════════════════════════════════════════╗\n";
    cout << "║     if constexpr要点：                                      ║\n";
    cout << "║     1. 编译期条件判断（C++17）                              ║\n";
    cout << "║     2. 只有符合条件的分支被编译                             ║\n";
    cout << "║     3. 常用于模板元编程                                     ║\n";
    cout << "║     4. 可避免无效分支的编译错误                             ║\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n";
    
    return 0;
}
