/**
 * @file constexpr_var.cpp
 * @brief constexpr变量详解
 * 
 * constexpr变量特点：
 * 1. 必须在编译期确定值
 * 2. 必须立即初始化
 * 3. 初始化必须由常量表达式完成
 * 4. 一旦初始化就不能修改
 * 5. 可以用于需要编译期常量的场景
 */

#include <iostream>
#include <array>
#include <vector>

using namespace std;

// ==================== 基本constexpr变量 ====================

// 基本类型
constexpr int max_size = 100;
constexpr double pi = 3.14159265359;
constexpr char newline = '\n';
constexpr bool is_debug = false;

// 编译期计算
constexpr int double_value = max_size * 2;
constexpr double pi_squared = pi * pi;
constexpr int sum = 1 + 2 + 3 + 4 + 5;

// ==================== constexpr与数组 ====================

// 用作数组大小（C风格数组）
int global_arr[max_size];  // ✅ 合法
int global_arr2[double_value];  // ✅ 合法

// 用作std::array大小
array<int, max_size> std_arr;  // ✅ 合法

// ==================== constexpr指针 ====================

// constexpr指针：指针本身是常量
constexpr int* null_ptr = nullptr;
int global_var = 10;
// constexpr int* ptr_to_global = &global_var;  // ❌ 错误！地址不是编译期常量

// constexpr可以指向其他constexpr
constexpr const char* greeting = "Hello, constexpr!";

// ==================== constexpr引用 ====================

// constexpr引用必须绑定到编译期常量
constexpr int& ref_to_constexpr = max_size;  // ❌ 错误！不能对constexpr变量取地址

// ==================== const vs constexpr对比 ====================

void demonstrate_const_vs_constexpr() {
    cout << "【const vs constexpr对比】\n\n";
    
    // const变量
    const int const_runtime = 100;  // 可能运行期初始化
    const int const_from_constexpr = max_size;  // 从constexpr初始化
    
    // constexpr变量
    constexpr int constexpr_val = 100;  // 必须编译期初始化
    
    cout << "const变量:\n";
    cout << "  const int const_runtime = 100;\n";
    cout << "  - 可以延迟初始化（某些情况）\n";
    cout << "  - 不能保证编译期已知\n\n";
    
    cout << "constexpr变量:\n";
    cout << "  constexpr int constexpr_val = 100;\n";
    cout << "  - 必须编译期初始化\n";
    cout << "  - 保证编译期已知\n";
    cout << "  - 可用于模板参数、数组大小等\n\n";
    
    // 数组大小使用
    cout << "数组大小使用:\n";
    cout << "  int arr1[" << const_runtime << "];  // VLA，非标准\n";
    cout << "  int arr2[" << constexpr_val << "];  // 标准合法\n";
}

// ==================== 常见错误示例 ====================

void demonstrate_common_errors() {
    cout << "【常见错误示例】\n\n";
    
    // 错误1：未立即初始化
    // constexpr int uninitialized;  // ❌ 编译错误
    
    // 错误2：用运行时值初始化
    // int runtime_val = 10;
    // constexpr int from_runtime = runtime_val;  // ❌ 编译错误
    
    // 错误3：修改constexpr变量
    // max_size = 200;  // ❌ 编译错误
    
    // 错误4：用函数返回值初始化（非constexpr函数）
    // int get_runtime_value() { return 42; }
    // constexpr int from_func = get_runtime_value();  // ❌ 编译错误
    
    cout << "错误1: constexpr int uninitialized;  // ❌ 必须立即初始化\n";
    cout << "错误2: constexpr int x = runtime_value;  // ❌ 必须用常量初始化\n";
    cout << "错误3: max_size = 200;  // ❌ 不能修改constexpr变量\n";
    cout << "错误4: constexpr int x = non_constexpr_func();  // ❌ 必须用constexpr函数\n";
}

// ==================== 高级用法 ====================

// constexpr数组
constexpr int fibonacci_arr[] = {0, 1, 1, 2, 3, 5, 8, 13, 21, 34};

// constexpr字符串（C++17 string_view）
#include <string_view>
constexpr string_view message = "constexpr is powerful!";

// constexpr结构体聚合初始化
struct Point {
    int x, y;
};

constexpr Point origin = {0, 0};
constexpr Point unit_x = {1, 0};
constexpr Point unit_y = {0, 1};

// C++20: 可以用constexpr声明浮点类型
// constexpr double precise_pi = 3.14159265358979323846;

void demonstrate_advanced_usage() {
    cout << "\n【高级用法】\n\n";
    
    // constexpr数组
    cout << "constexpr数组:\n";
    cout << "  constexpr int fibonacci_arr[] = {0, 1, 1, 2, 3, 5, 8, 13, 21, 34};\n";
    cout << "  fibonacci_arr[5] = " << fibonacci_arr[5] << "\n\n";
    
    // constexpr结构体
    cout << "constexpr结构体:\n";
    cout << "  constexpr Point origin = {0, 0};\n";
    cout << "  origin.x = " << origin.x << ", origin.y = " << origin.y << "\n\n";
    
    // 编译期计算
    cout << "编译期计算:\n";
    cout << "  max_size = " << max_size << "\n";
    cout << "  double_value = max_size * 2 = " << double_value << "\n";
    cout << "  pi = " << pi << "\n";
    cout << "  pi_squared = " << pi_squared << "\n";
}

// ==================== main函数 ====================

int main() {
    cout << "╔════════════════════════════════════════════════════════════╗\n";
    cout << "║              constexpr变量详解                              ║\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n\n";
    
    // 输出基本constexpr变量
    cout << "【基本constexpr变量】\n\n";
    cout << "  max_size = " << max_size << "\n";
    cout << "  pi = " << pi << "\n";
    cout << "  newline = '" << newline << "'\n";
    cout << "  is_debug = " << (is_debug ? "true" : "false") << "\n";
    cout << "  double_value = " << double_value << "\n";
    cout << "  pi_squared = " << pi_squared << "\n";
    cout << "  sum = " << sum << "\n";
    
    cout << "\n";
    demonstrate_const_vs_constexpr();
    demonstrate_common_errors();
    demonstrate_advanced_usage();
    
    cout << "\n╔════════════════════════════════════════════════════════════╗\n";
    cout << "║     constexpr变量要点：                                     ║\n";
    cout << "║     1. 必须编译期确定值                                     ║\n";
    cout << "║     2. 必须立即初始化                                       ║\n";
    cout << "║     3. 初始化必须由常量表达式完成                           ║\n";
    cout << "║     4. 可用于数组大小、模板参数等场景                       ║\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n";
    
    return 0;
}
