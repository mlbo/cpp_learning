/**
 * @file auto_demo.cpp
 * @brief auto类型推导演示
 * 
 * 涵盖内容：
 * - auto基本用法
 * - auto推导规则
 * - auto与const/reference的结合
 * - auto的常见陷阱
 */

#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <memory>
#include <type_traits>
#include <initializer_list>

// 辅助函数：打印类型信息
template<typename T>
void print_type(const T& var, const std::string& name) {
    std::cout << "  " << name << " 的类型: ";
    if (std::is_const<T>::value) std::cout << "const ";
    if (std::is_pointer<T>::value) std::cout << "pointer to ";
    if (std::is_reference<T>::value) std::cout << "reference to ";
    if (std::is_array<T>::value) std::cout << "array of ";
    
    // 简化类型名显示
    if (std::is_same<T, int>::value) std::cout << "int";
    else if (std::is_same<T, double>::value) std::cout << "double";
    else if (std::is_same<T, float>::value) std::cout << "float";
    else if (std::is_same<T, char>::value) std::cout << "char";
    else if (std::is_same<T, const char*>::value) std::cout << "const char*";
    else if (std::is_same<T, std::string>::value) std::cout << "std::string";
    else if (std::is_same<T, std::vector<int>>::value) std::cout << "std::vector<int>";
    else std::cout << "other";
    
    std::cout << "\n";
}

// ============================================
// 1. auto 基本用法
// ============================================

void auto_basics() {
    std::cout << "\n=== 1. auto 基本用法 ===\n";
    
    // 基本类型推导
    auto i = 42;              // int
    auto d = 3.14;            // double
    auto f = 3.14f;           // float
    auto c = 'A';             // char
    auto s = "hello";         // const char*
    auto str = std::string("hello");  // std::string
    
    std::cout << "  i = 42 -> int\n";
    std::cout << "  d = 3.14 -> double\n";
    std::cout << "  f = 3.14f -> float\n";
    std::cout << "  c = 'A' -> char\n";
    std::cout << "  s = \"hello\" -> const char*\n";
    std::cout << "  str = std::string(\"hello\") -> std::string\n";
    
    // 表达式推导
    auto sum = i + 10;        // int
    auto div_result = d / 2;  // double
    
    std::cout << "  auto sum = i + 10 -> int\n";
    std::cout << "  auto div_result = d / 2 -> double\n";
}

// ============================================
// 2. auto 与 const/reference 的结合
// ============================================

void auto_with_const_ref() {
    std::cout << "\n=== 2. auto 与 const/reference 结合 ===\n";
    
    int x = 10;
    const int cx = 20;
    int& rx = x;
    const int& crx = cx;
    
    // auto 会忽略引用和顶层const（除非显式指定）
    auto a1 = x;      // int
    auto a2 = cx;     // int（顶层const被忽略）
    auto a3 = rx;     // int（引用被忽略）
    auto a4 = crx;    // int（引用和顶层const都被忽略）
    
    std::cout << "  auto a1 = x -> int\n";
    std::cout << "  auto a2 = cx -> int (顶层const被忽略)\n";
    std::cout << "  auto a3 = rx -> int (引用被忽略)\n";
    std::cout << "  auto a4 = crx -> int (引用和const都被忽略)\n";
    
    // 显式保留const和引用
    const auto& ca1 = x;    // const int&
    auto& ra1 = x;          // int&
    const auto ca2 = cx;    // const int
    
    std::cout << "  const auto& ca1 = x -> const int&\n";
    std::cout << "  auto& ra1 = x -> int&\n";
    std::cout << "  const auto ca2 = cx -> const int\n";
    
    // 修改演示
    ra1 = 100;  // 通过引用修改x
    std::cout << "\n  通过 ra1 修改后，x = " << x << "\n";
}

// ============================================
// 3. auto 与指针
// ============================================

void auto_with_pointers() {
    std::cout << "\n=== 3. auto 与指针 ===\n";
    
    int x = 10;
    int* px = &x;
    const int* pcx = &x;
    
    auto p1 = px;       // int*
    auto p2 = pcx;      // const int*（底层const保留）
    auto* p3 = &x;      // int*
    const auto* p4 = &x; // const int*
    
    std::cout << "  auto p1 = px -> int*\n";
    std::cout << "  auto p2 = pcx -> const int* (底层const保留)\n";
    std::cout << "  auto* p3 = &x -> int*\n";
    std::cout << "  const auto* p4 = &x -> const int*\n";
    
    std::cout << "\n  *p1 = " << *p1 << "\n";
    std::cout << "  *p3 = " << *p3 << "\n";
}

// ============================================
// 4. auto 与容器
// ============================================

void auto_with_containers() {
    std::cout << "\n=== 4. auto 与容器 ===\n";
    
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::map<std::string, int> map = {{"one", 1}, {"two", 2}};
    
    // 迭代器
    auto it = vec.begin();  // std::vector<int>::iterator
    auto mit = map.begin(); // std::map<std::string, int>::iterator
    
    std::cout << "  auto it = vec.begin() -> vector<int>::iterator\n";
    std::cout << "  auto mit = map.begin() -> map<string, int>::iterator\n";
    
    // 范围for循环
    std::cout << "\n  范围for循环遍历:\n    ";
    for (auto& num : vec) {
        std::cout << num << " ";
    }
    std::cout << "\n";
    
    // C++11 初始化
    auto vec2 = std::vector<int>{1, 2, 3};
    std::cout << "  auto vec2 = vector<int>{1,2,3} -> vector<int>\n";
}

// ============================================
// 5. auto 的陷阱
// ============================================

void auto_traps() {
    std::cout << "\n=== 5. auto 的常见陷阱 ===\n";
    
    // 陷阱1：初始化列表推导为 initializer_list
    auto list = {1, 2, 3};  // std::initializer_list<int>
    std::cout << "  auto list = {1,2,3} -> initializer_list<int>\n";
    
    // 陷阱2：vector<bool> 的代理类
    std::vector<bool> bool_vec = {true, false, true};
    auto ref = bool_vec[0];  // 不是bool，而是代理类
    std::cout << "  auto ref = bool_vec[0] -> vector<bool>::reference (代理类)\n";
    
    // 陷阱3：数组退化
    int arr[5] = {1, 2, 3, 4, 5};
    auto arr_copy = arr;  // int*（数组退化为指针）
    std::cout << "  auto arr_copy = arr -> int* (数组退化)\n";
    
    // 正确方式：使用引用
    auto& arr_ref = arr;  // int(&)[5]
    std::cout << "  auto& arr_ref = arr -> int(&)[5]\n";
    
    // 陷阱4：无符号数比较
    std::vector<int> vec = {1, 2, 3};
    // auto size = vec.size();  // size_t (unsigned)
    // int i = -1;
    // if (i < size) { ... }  // 危险！-1会被转为大整数
    
    std::cout << "  vec.size() 类型: size_t (unsigned)\n";
    std::cout << "  警告：有符号与无符号数比较是危险的！\n";
}

// ============================================
// 6. auto 的最佳实践
// ============================================

void auto_best_practices() {
    std::cout << "\n=== 6. auto 最佳实践 ===\n";
    
    std::vector<int> vec = {1, 2, 3, 4, 5};
    
    // 好的使用场景
    std::cout << "  好的使用场景:\n";
    std::cout << "    - 迭代器: for (auto it = vec.begin(); ...)\n";
    std::cout << "    - 复杂类型: auto ptr = std::make_shared<int>(42)\n";
    std::cout << "    - 范围for: for (const auto& item : container)\n";
    
    // 不好的使用场景
    std::cout << "\n  不推荐的使用场景:\n";
    std::cout << "    - 简单类型: auto x = 42 (不如直接写 int x = 42)\n";
    std::cout << "    - 需要明确类型意图时\n";
    
    // auto 配合智能指针
    auto uptr = std::make_unique<int>(42);
    auto sptr = std::make_shared<int>(42);
    std::cout << "\n  智能指针:\n";
    std::cout << "    auto uptr = make_unique<int>(42) -> unique_ptr<int>\n";
    std::cout << "    auto sptr = make_shared<int>(42) -> shared_ptr<int>\n";
}

// ============================================
// 主演示函数
// ============================================

void demonstrate_auto() {
    auto_basics();
    auto_with_const_ref();
    auto_with_pointers();
    auto_with_containers();
    auto_traps();
    auto_best_practices();
}
