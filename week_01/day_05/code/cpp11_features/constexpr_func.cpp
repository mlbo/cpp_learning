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
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <string_view>

#include "../constexpr_math.h"

using namespace std;

// ==================== C++11风格constexpr函数 ====================

// 基本算术运算
constexpr int square(int x) {
    return day05::square(x);
}

constexpr int cube(int x) {
    return day05::checked_multiply(day05::square(x), x);
}

constexpr int abs_val(int x) {
    return x == std::numeric_limits<int>::min()
        ? throw std::overflow_error("abs_val result does not fit in int")
        : (x < 0 ? -x : x);
}

// 递归（三元运算符模拟）
constexpr int fibonacci(int n) {
    return (n < 0 || n > 30)
        ? throw std::out_of_range(
              "recursive fibonacci demo requires 0 <= n <= 30; use fibonacci_iter above it")
        : (n <= 1 ? n : day05::checked_add(fibonacci(n - 1), fibonacci(n - 2)));
}

constexpr std::int64_t magnitude(int value) noexcept {
    const auto widened = static_cast<std::int64_t>(value);
    return widened < 0 ? -widened : widened;
}

constexpr std::int64_t gcd_magnitude(int a, int b) noexcept {
    std::int64_t x = magnitude(a);
    std::int64_t y = magnitude(b);
    while (y != 0) {
        const std::int64_t remainder = x % y;
        x = y;
        y = remainder;
    }
    return x;
}

constexpr int gcd(int a, int b) {
    return day05::checked_int(gcd_magnitude(a, b));
}

constexpr int lcm(int a, int b) {
    if (a == 0 || b == 0) return 0;
    const std::int64_t divisor = gcd_magnitude(a, b);
    const std::int64_t result = (magnitude(a) / divisor) * magnitude(b);
    return day05::checked_int(result);
}

// ==================== C++14风格constexpr函数 ====================

// 允许局部变量和循环
constexpr int sum_to_n(int n) {
    return day05::sum_to_n(n);
}

constexpr int power(int base, int exp) {
    if (exp < 0 || exp > 1'000) {
        throw std::out_of_range("integer power demo requires 0 <= exponent <= 1,000");
    }
    int result = 1;
    for (int i = 0; i < exp; ++i) {
        result = day05::checked_multiply(result, base);
    }
    return result;
}

constexpr bool is_prime(int n) {
    if (n < 2) return false;
    for (int i = 2; i <= n / i; ++i) {
        if (n % i == 0) return false;
    }
    return true;
}

constexpr int count_primes(int n) {
    if (n < 0 || n > 1'000'000) {
        throw std::out_of_range("count_primes demo requires 0 <= n <= 1,000,000");
    }
    int count = 0;
    for (int i = 2; i <= n; ++i) {
        if (is_prime(i)) ++count;
    }
    return count;
}

// ==================== constexpr函数的限制 ====================

// 常量求值不能任意修改求值之外已经存在的对象。
// C++14起 constexpr 函数可以修改在本次常量求值内部创建的局部状态，
// 因此不应简化成“constexpr函数不能有任何副作用”。

// 非 constexpr 调用可以写在 constexpr 函数中，但常量求值实际执行的路径
// 不能调用普通函数。若调用只出现在未选择的运行时路径，其他路径仍可常量求值。
// constexpr int bad_sin(double x) {
//     return sin(x);  // 若执行到这里，就不能得到常量表达式（C++26前）
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

void demonstrate_compile_vs_runtime(bool interactive) {
    cout << "【编译期 vs 运行时调用】\n\n";
    
    // 编译期计算
    constexpr int fact5 = day05::factorial(5);   // 编译期
    constexpr int fib10 = fibonacci(10);  // 编译期
    constexpr int sq7 = square(7);        // 编译期
    
    cout << "编译期计算（constexpr参数）:\n";
    cout << "  constexpr int fact5 = factorial(5) = " << fact5 << "\n";
    cout << "  constexpr int fib10 = fibonacci(10) = " << fib10 << "\n";
    cout << "  constexpr int sq7 = square(7) = " << sq7 << "\n\n";
    
    // 运行时计算
    int n = 8;
    cout << "运行时计算（运行时参数）:\n";
    if (interactive) {
        cout << "  请输入 0~" << day05::max_factorial_input << " 的整数 n: ";
        if (!(cin >> n)) {
            cin.clear();
            n = 8;
            cout << "\n  未读到合法整数，使用默认值 8\n";
        }
    } else {
        cout << "  自动演示使用 n = 8（手工输入请加 --interactive）\n";
    }
    if (!day05::factorial_fits_int(n)) {
        cout << "  输入超出 int 阶乘安全范围，使用默认值 8\n";
        n = 8;
    }
    
    int runtime_fact = day05::factorial(n);  // 运行时
    int runtime_fib = fibonacci(n);   // 运行时
    
    cout << "  factorial(" << n << ") = " << runtime_fact << "\n";
    cout << "  fibonacci(" << n << ") = " << runtime_fib << "\n";
    
    // 用作编译期常量
    cout << "\n用作编译期常量:\n";
    cout << "  int arr[factorial(3)];  // 数组大小 = " << day05::factorial(3) << "\n";
    array<int, day05::factorial(4)> arr;  // 编译期计算大小 = 24
    cout << "  array<int, factorial(4)> arr;  // 大小 = " << arr.size() << "\n";
}

// ==================== 实用constexpr函数 ====================

// 前提：str 指向一个有效、以 '\0' 结尾的字符序列。
constexpr size_t str_len(const char* str) {
    if (str == nullptr) {
        throw std::invalid_argument("str_len requires a non-null pointer");
    }
    size_t len = 0;
    while (str[len] != '\0') {
        ++len;
    }
    return len;
}

// 前提：a 和 b 都指向有效、以 '\0' 结尾的字符序列。
constexpr bool str_eq(const char* a, const char* b) {
    if (a == nullptr || b == nullptr) {
        throw std::invalid_argument("str_eq requires non-null pointers");
    }
    while (*a && *b && *a == *b) {
        ++a;
        ++b;
    }
    return *a == *b;
}

// 编译期哈希
constexpr uint32_t hash_str(const char* str) {
    if (str == nullptr) {
        throw std::invalid_argument("hash_str requires a non-null pointer");
    }
    uint32_t hash = 5381;
    while (*str) {
        hash = hash * 33 + static_cast<uint32_t>(static_cast<unsigned char>(*str));
        ++str;
    }
    return hash;
}

// 二分查找
constexpr int binary_search(const int* arr, std::size_t size, int target) {
    if (size > static_cast<std::size_t>(std::numeric_limits<int>::max())) {
        throw std::length_error("binary_search result index would not fit in int");
    }
    if (arr == nullptr && size != 0) {
        throw std::invalid_argument("binary_search requires data for a non-empty range");
    }
    std::size_t left = 0;
    std::size_t right = size;
    while (left < right) {
        const std::size_t mid = left + (right - left) / 2;
        if (arr[mid] == target) {
            return day05::checked_int(static_cast<std::int64_t>(mid));
        }
        if (arr[mid] < target) left = mid + 1;
        else right = mid;
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
constexpr int factorial_tail_impl(int n, int acc) {
    return n <= 1
        ? acc
        : factorial_tail_impl(n - 1, day05::checked_multiply(n, acc));
}

constexpr int factorial_tail(int n) {
    return day05::factorial_fits_int(n)
        ? factorial_tail_impl(n, 1)
        : throw std::out_of_range("factorial_tail(int) requires 0 <= n <= 12");
}

constexpr int fibonacci_iter(int n) {
    if (n < 0 || n > 46) {
        throw std::out_of_range("fibonacci_iter(int) requires 0 <= n <= 46");
    }
    if (n <= 1) return n;
    int a = 0, b = 1;
    for (int i = 2; i <= n; ++i) {
        const int temp = day05::checked_add(a, b);
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

template<typename Exception, typename Operation>
bool expect_exception(Operation operation, const char* name) {
    try {
        operation();
    } catch (const Exception&) {
        cout << "  [PASS] " << name << "\n";
        return true;
    } catch (...) {
    }
    cerr << "  [FAIL] " << name << "\n";
    return false;
}

bool demonstrate_integer_boundaries() {
    cout << "\n【整数与资源边界契约】\n";
    bool passed = true;
    passed = expect_exception<overflow_error>([] { (void)square(46341); },
                                               "square 拒绝 int 溢出") && passed;
    passed = expect_exception<overflow_error>([] {
        (void)abs_val(numeric_limits<int>::min());
    }, "abs_val 拒绝 INT_MIN") && passed;
    passed = expect_exception<out_of_range>([] { (void)fibonacci(31); },
                                            "递归 fibonacci 拒绝超出教学工作量的输入") && passed;
    passed = expect_exception<overflow_error>([] { (void)power(2, 31); },
                                               "power 拒绝中间结果溢出") && passed;
    passed = expect_exception<invalid_argument>([] {
        (void)binary_search(nullptr, 1, 0);
    }, "binary_search 拒绝非空范围的空指针") && passed;
    passed = expect_exception<out_of_range>([] { (void)count_primes(1'000'001); },
                                            "count_primes 拒绝超出教学上限的工作量") && passed;
    return passed;
}

// ==================== main函数 ====================

int main(int argc, char* argv[]) {
    cout << "╔════════════════════════════════════════════════════════════╗\n";
    cout << "║              constexpr函数详解                              ║\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n\n";
    
    // 基本示例
    cout << "【基本constexpr函数】\n\n";
    cout << "  square(5) = " << square(5) << "\n";
    cout << "  cube(3) = " << cube(3) << "\n";
    cout << "  abs_val(-7) = " << abs_val(-7) << "\n";
    cout << "  factorial(6) = " << day05::factorial(6) << "\n";
    cout << "  fibonacci(8) = " << fibonacci(8) << "\n";
    cout << "  gcd(48, 18) = " << gcd(48, 18) << "\n";
    cout << "  lcm(4, 6) = " << lcm(4, 6) << "\n";
    
    // C++14特性
    cout << "\n【C++14风格constexpr函数】\n\n";
    cout << "  sum_to_n(10) = " << sum_to_n(10) << "\n";
    cout << "  power(2, 10) = " << power(2, 10) << "\n";
    cout << "  is_prime(17) = " << (is_prime(17) ? "true" : "false") << "\n";
    cout << "  count_primes(20) = " << count_primes(20) << "\n";
    
    const bool interactive =
        argc == 2 && std::string_view(argv[1]) == "--interactive";
    demonstrate_compile_vs_runtime(interactive);
    demonstrate_utility_functions();
    demonstrate_optimized_recursion();
    const bool boundaries_passed = demonstrate_integer_boundaries();
    
    cout << "\n╔════════════════════════════════════════════════════════════╗\n";
    cout << "║     constexpr函数要点：                                     ║\n";
    cout << "║     1. 编译期常量参数 → 编译期计算                          ║\n";
    cout << "║     2. 运行时参数 → 运行时计算                              ║\n";
    cout << "║     3. C++14起支持循环和局部变量                            ║\n";
    cout << "║     4. 可用于编译期常量场景（数组大小等）                   ║\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n";
    
    return boundaries_passed ? 0 : 1;
}
