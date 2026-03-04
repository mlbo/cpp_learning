/**
 * @file constexpr_func.cpp
 * @brief constexpr函数详解
 * 
 * constexpr函数特点：
 * 1. 可在编译期求值（如果参数是编译期常量）
 * 2. 也可在运行时调用（如果参数是运行时值）
 * 3. C++11限制：函数体必须单一return语句
 * 4. C++14放宽：允许局部变量、循环、条件等
 */

#include <iostream>
#include <array>
#include <cmath>

using namespace std;

// ==================== C++11风格constexpr函数 ====================

// 基本算术运算
constexpr int square(int x) {
    return x * x;
}

constexpr int cube(int x) {
    return x * x * x;
}

constexpr int abs_val(int x) {
    return x < 0 ? -x : x;
}

// 递归（三元运算符模拟）
constexpr int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

constexpr int fibonacci(int n) {
    return n <= 1 ? n : fibonacci(n - 1) + fibonacci(n - 2);
}

constexpr int gcd(int a, int b) {
    return b == 0 ? a : gcd(b, a % b);
}

constexpr int lcm(int a, int b) {
    return a * b / gcd(a, b);
}

// ==================== C++14风格constexpr函数 ====================

// 允许局部变量和循环
constexpr int sum_to_n(int n) {
    int result = 0;
    for (int i = 1; i <= n; ++i) {
        result += i;
    }
    return result;
}

constexpr int power(int base, int exp) {
    int result = 1;
    for (int i = 0; i < exp; ++i) {
        result *= base;
    }
    return result;
}

constexpr bool is_prime(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
}

constexpr int count_primes(int n) {
    int count = 0;
    for (int i = 2; i <= n; ++i) {
        if (is_prime(i)) ++count;
    }
    return count;
}

// ==================== constexpr函数的限制 ====================

// 限制1：不能有副作用
// constexpr int bad_func(int& x) {
//     return ++x;  // ❌ 不能修改参数
// }

// 限制2：不能调用非constexpr函数
// constexpr int bad_sin(double x) {
//     return sin(x);  // ❌ sin不是constexpr（C++26前）
// }

// 限制3：不能有try-catch（C++20前）
// constexpr int bad_with_try(int x) {
//     try {
//         return x;
//     } catch (...) {
//         return 0;
//     }
// }

// ==================== 编译期vs运行时调用 ====================

void demonstrate_compile_vs_runtime() {
    cout << "【编译期 vs 运行时调用】\n\n";
    
    // 编译期计算
    constexpr int fact5 = factorial(5);   // 编译期
    constexpr int fib10 = fibonacci(10);  // 编译期
    constexpr int sq7 = square(7);        // 编译期
    
    cout << "编译期计算（constexpr参数）:\n";
    cout << "  constexpr int fact5 = factorial(5) = " << fact5 << "\n";
    cout << "  constexpr int fib10 = fibonacci(10) = " << fib10 << "\n";
    cout << "  constexpr int sq7 = square(7) = " << sq7 << "\n\n";
    
    // 运行时计算
    int n;
    cout << "运行时计算（运行时参数）:\n";
    cout << "  请输入一个数字n: ";
    cin >> n;
    
    int runtime_fact = factorial(n);  // 运行时
    int runtime_fib = fibonacci(n);   // 运行时
    
    cout << "  factorial(" << n << ") = " << runtime_fact << "\n";
    cout << "  fibonacci(" << n << ") = " << runtime_fib << "\n";
    
    // 用作编译期常量
    cout << "\n用作编译期常量:\n";
    cout << "  int arr[factorial(3)];  // 数组大小 = " << factorial(3) << "\n";
    array<int, factorial(4)> arr;  // 编译期计算大小 = 24
    cout << "  array<int, factorial(4)> arr;  // 大小 = " << arr.size() << "\n";
}

// ==================== 实用constexpr函数 ====================

// 字符串长度
constexpr size_t str_len(const char* str) {
    size_t len = 0;
    while (str[len] != '\0') {
        ++len;
    }
    return len;
}

// 字符串比较
constexpr bool str_eq(const char* a, const char* b) {
    while (*a && *b && *a == *b) {
        ++a;
        ++b;
    }
    return *a == *b;
}

// 编译期哈希
constexpr uint32_t hash_str(const char* str) {
    uint32_t hash = 5381;
    while (*str) {
        hash = hash * 33 + static_cast<uint32_t>(*str);
        ++str;
    }
    return hash;
}

// 二分查找
constexpr int binary_search(const int* arr, int size, int target) {
    int left = 0, right = size - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) return mid;
        if (arr[mid] < target) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

void demonstrate_utility_functions() {
    cout << "\n【实用constexpr函数】\n\n";
    
    // 字符串操作
    constexpr const char* test_str = "Hello, constexpr!";
    constexpr size_t len = str_len(test_str);
    cout << "字符串长度:\n";
    cout << "  str_len(\"" << test_str << "\") = " << len << "\n\n";
    
    // 字符串比较
    constexpr bool eq = str_eq("hello", "hello");
    constexpr bool ne = str_eq("hello", "world");
    cout << "字符串比较:\n";
    cout << "  str_eq(\"hello\", \"hello\") = " << (eq ? "true" : "false") << "\n";
    cout << "  str_eq(\"hello\", \"world\") = " << (ne ? "true" : "false") << "\n\n";
    
    // 编译期哈希
    constexpr uint32_t hash1 = hash_str("test");
    constexpr uint32_t hash2 = hash_str("const");
    cout << "编译期哈希:\n";
    cout << "  hash_str(\"test\") = " << hash1 << "\n";
    cout << "  hash_str(\"const\") = " << hash2 << "\n\n";
    
    // 二分查找
    constexpr int sorted_arr[] = {1, 3, 5, 7, 9, 11, 13, 15};
    constexpr int idx = binary_search(sorted_arr, 8, 7);
    cout << "二分查找:\n";
    cout << "  在 {1,3,5,7,9,11,13,15} 中查找7\n";
    cout << "  结果索引 = " << idx << "\n";
}

// ==================== 递归深度限制 ====================

// 尾递归优化版本
constexpr int factorial_tail(int n, int acc = 1) {
    return n <= 1 ? acc : factorial_tail(n - 1, n * acc);
}

constexpr int fibonacci_iter(int n) {
    if (n <= 1) return n;
    int a = 0, b = 1;
    for (int i = 2; i <= n; ++i) {
        int temp = a + b;
        a = b;
        b = temp;
    }
    return b;
}

void demonstrate_optimized_recursion() {
    cout << "\n【优化递归避免栈溢出】\n\n";
    
    // 尾递归版本
    constexpr int fact10 = factorial_tail(10);
    constexpr int fib15 = fibonacci_iter(15);
    
    cout << "尾递归/迭代版本:\n";
    cout << "  factorial_tail(10) = " << fact10 << "\n";
    cout << "  fibonacci_iter(15) = " << fib15 << "\n";
    cout << "\n  提示：对于大数值，使用迭代版本更安全\n";
}

// ==================== main函数 ====================

int main() {
    cout << "╔════════════════════════════════════════════════════════════╗\n";
    cout << "║              constexpr函数详解                              ║\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n\n";
    
    // 基本示例
    cout << "【基本constexpr函数】\n\n";
    cout << "  square(5) = " << square(5) << "\n";
    cout << "  cube(3) = " << cube(3) << "\n";
    cout << "  abs_val(-7) = " << abs_val(-7) << "\n";
    cout << "  factorial(6) = " << factorial(6) << "\n";
    cout << "  fibonacci(8) = " << fibonacci(8) << "\n";
    cout << "  gcd(48, 18) = " << gcd(48, 18) << "\n";
    cout << "  lcm(4, 6) = " << lcm(4, 6) << "\n";
    
    // C++14特性
    cout << "\n【C++14风格constexpr函数】\n\n";
    cout << "  sum_to_n(10) = " << sum_to_n(10) << "\n";
    cout << "  power(2, 10) = " << power(2, 10) << "\n";
    cout << "  is_prime(17) = " << (is_prime(17) ? "true" : "false") << "\n";
    cout << "  count_primes(20) = " << count_primes(20) << "\n";
    
    demonstrate_compile_vs_runtime();
    demonstrate_utility_functions();
    demonstrate_optimized_recursion();
    
    cout << "\n╔════════════════════════════════════════════════════════════╗\n";
    cout << "║     constexpr函数要点：                                     ║\n";
    cout << "║     1. 编译期常量参数 → 编译期计算                          ║\n";
    cout << "║     2. 运行时参数 → 运行时计算                              ║\n";
    cout << "║     3. C++14起支持循环和局部变量                            ║\n";
    cout << "║     4. 可用于编译期常量场景（数组大小等）                   ║\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n";
    
    return 0;
}
