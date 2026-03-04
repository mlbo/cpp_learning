/**
 * @file type_deduction_items.cpp
 * @brief Effective Modern C++ 条款 1-5 类型推导详解
 * 
 * 条款1：理解模板类型推导
 * 条款2：理解auto类型推导
 * 条款3：理解decltype
 * 条款4：学会查看类型推导结果
 * 条款5：优先使用auto而非显式类型声明
 */

#include <iostream>
#include <memory>
#include <vector>
#include <type_traits>
#include <typeinfo>
#include <memory>
#include <initializer_list>

// ============================================
// 辅助工具
// ============================================

// 打印类型的编译期信息
template<typename T>
struct TypePrinter;

// 打印推导类型的函数
template<typename T>
void print_type_info(const T& param, const std::string& param_name) {
    std::cout << "  " << param_name << ":\n";
    std::cout << "    T = ";
    
    // 使用编译期类型判断
    if (std::is_same<T, int>::value) std::cout << "int";
    else if (std::is_same<T, int&>::value) std::cout << "int&";
    else if (std::is_same<T, const int&>::value) std::cout << "const int&";
    else if (std::is_same<T, int*>::value) std::cout << "int*";
    else if (std::is_same<T, const int*>::value) std::cout << "const int*";
    else if (std::is_same<T, double>::value) std::cout << "double";
    else std::cout << "other type";
    
    std::cout << "\n";
}

// ============================================
// 条款1：理解模板类型推导
// ============================================

// 情况1：ParamType是指针或引用（非万能引用）
template<typename T>
void func_reference(T& param) {
    std::cout << "  func_reference(T& param): ";
    if (std::is_same<T, int>::value) std::cout << "T = int\n";
    else if (std::is_same<T, const int>::value) std::cout << "T = const int\n";
    else std::cout << "T = other\n";
}

template<typename T>
void func_const_reference(const T& param) {
    std::cout << "  func_const_reference(const T& param): ";
    if (std::is_same<T, int>::value) std::cout << "T = int\n";
    else std::cout << "T = other\n";
}

template<typename T>
void func_pointer(T* param) {
    std::cout << "  func_pointer(T* param): ";
    if (std::is_same<T, int>::value) std::cout << "T = int\n";
    else if (std::is_same<T, const int>::value) std::cout << "T = const int\n";
    else std::cout << "T = other\n";
}

// 情况2：ParamType是万能引用
template<typename T>
void func_universal_reference(T&& param) {
    std::cout << "  func_universal_reference(T&& param): ";
    if (std::is_same<T, int>::value) std::cout << "T = int (右值)\n";
    else if (std::is_same<T, int&>::value) std::cout << "T = int& (左值)\n";
    else if (std::is_same<T, const int&>::value) std::cout << "T = const int& (const左值)\n";
    else std::cout << "T = other\n";
}

// 情况3：ParamType非引用
template<typename T>
void func_by_value(T param) {
    std::cout << "  func_by_value(T param): ";
    if (std::is_same<T, int>::value) std::cout << "T = int\n";
    else std::cout << "T = other\n";
}

void item01_template_deduction() {
    std::cout << "\n=== 条款1：模板类型推导 ===\n";
    
    int x = 27;
    const int cx = x;
    const int& rx = x;
    
    std::cout << "\n--- 情况1: T& (引用参数) ---\n";
    func_reference(x);      // T = int
    func_reference(cx);     // T = const int (保留const)
    func_reference(rx);     // T = const int (忽略引用，保留const)
    
    std::cout << "\n--- 情况2: const T& (常量引用参数) ---\n";
    func_const_reference(x);   // T = int (const在ParamType中)
    func_const_reference(cx);  // T = int
    func_const_reference(rx);  // T = int
    
    std::cout << "\n--- 情况3: T* (指针参数) ---\n";
    func_pointer(&x);      // T = int
    func_pointer(&cx);     // T = const int
    
    std::cout << "\n--- 情况4: T&& (万能引用) ---\n";
    func_universal_reference(x);   // T = int& (左值)
    func_universal_reference(cx);  // T = const int& (const左值)
    func_universal_reference(27);  // T = int (右值)
    
    std::cout << "\n--- 情况5: T (按值传递) ---\n";
    func_by_value(x);      // T = int
    func_by_value(cx);     // T = int (忽略const)
    func_by_value(rx);     // T = int (忽略const和引用)
}

// ============================================
// 条款2：理解auto类型推导
// ============================================

void item02_auto_deduction() {
    std::cout << "\n=== 条款2：auto类型推导 ===\n";
    
    // auto推导与模板推导基本相同
    auto x = 27;           // auto -> int, 类似 template<typename T> void f(T param);
    const auto cx = x;     // auto -> int, 类似 template<typename T> void f(const T param);
    const auto& rx = x;    // auto -> int, 类似 template<typename T> void f(const T& param);
    
    std::cout << "  auto x = 27 -> int\n";
    std::cout << "  const auto cx = x -> const int\n";
    std::cout << "  const auto& rx = x -> const int&\n";
    
    // auto与模板推导的唯一区别：初始化列表
    std::cout << "\n  auto与模板推导的区别：初始化列表\n";
    auto x1 = 27;          // int
    auto x2(27);           // int
    auto x3 = {27};        // std::initializer_list<int>
    auto x4{27};           // int (C++17), initializer_list (C++11)
    
    std::cout << "    auto x1 = 27 -> int\n";
    std::cout << "    auto x2(27) -> int\n";
    std::cout << "    auto x3 = {27} -> initializer_list<int>\n";
    std::cout << "    auto x4{27} -> int (C++17)\n";
    
    // 模板无法自动推导初始化列表
    // template<typename T> void f(T param);
    // f({1, 2, 3});  // 错误！无法推导T
    
    // 但可以显式指定
    // f<std::initializer_list<int>>({1, 2, 3});  // 正确
}

// ============================================
// 条款3：理解decltype
// ============================================

void item03_decltype() {
    std::cout << "\n=== 条款3：decltype ===\n";
    
    // decltype返回表达式的精确类型
    int x = 0;
    decltype(x) y = 1;        // int
    decltype((x)) z = x;      // int& (注意：双层括号!)
    
    std::cout << "  int x = 0;\n";
    std::cout << "  decltype(x) y = 1; -> int\n";
    std::cout << "  decltype((x)) z = x; -> int& (双层括号)\n";
    
    // decltype(auto) 结合auto和decltype
    // C++11写法（注释）:
    // decltype(x) da1 = x;
    // decltype((x)) da2 = x;
    auto a1 = x;              // int
    decltype(auto) da1 = x;   // int
    
    auto a2 = (x);            // int
    decltype(auto) da2 = (x); // int& (保留引用)
    
    std::cout << "\n  decltype(auto):\n";
    std::cout << "    auto a1 = x; -> int\n";
    std::cout << "    decltype(auto) da1 = x; -> int\n";
    std::cout << "    auto a2 = (x); -> int\n";
    std::cout << "    decltype(auto) da2 = (x); -> int&\n";
    
    // 在返回类型中使用
    std::cout << "\n  在函数返回类型中使用decltype(auto):\n";
    std::cout << "    可以完美转发返回值的类型\n";
}

// 返回值使用decltype(auto)的示例
// C++11写法（注释）:
// template<typename Container, typename Index>
// auto get_element(Container&& c, Index i) -> decltype(std::forward<Container>(c)[i]) {
//     return std::forward<Container>(c)[i];
// }
template<typename Container, typename Index>
decltype(auto) get_element(Container&& c, Index i) {
    return std::forward<Container>(c)[i];
}

// ============================================
// 条款4：学会查看类型推导结果
// ============================================

void item04_see_types() {
    std::cout << "\n=== 条款4：查看类型推导结果 ===\n";
    
    // 方法1：IDE提示（运行时无法演示）
    std::cout << "  方法1：IDE编辑器提示\n";
    
    // 方法2：编译器诊断
    std::cout << "  方法2：编译器诊断\n";
    std::cout << "    template<typename T> class TD;\n";
    std::cout << "    TD<decltype(x)> td;  // 编译错误会显示类型\n";
    
    // 方法3：运行时输出
    std::cout << "  方法3：运行时输出typeid\n";
    auto x = 27;
    const auto& cx = x;
    std::cout << "    typeid(x).name() = " << typeid(x).name() << "\n";
    std::cout << "    typeid(cx).name() = " << typeid(cx).name() << "\n";
    std::cout << "    (注：typeid不保留const和引用)\n";
    
    // 方法4：boost::typeindex（如果可用）
    std::cout << "  方法4：boost::typeindex（精确类型）\n";
}

// 用于编译期诊断的模板
template<typename T>
class TypeDisplay;

// ============================================
// 条款5：优先使用auto而非显式类型声明
// ============================================

void item05_prefer_auto() {
    std::cout << "\n=== 条款5：优先使用auto ===\n";
    
    // 优点1：避免未初始化变量
    int x1;          // 未初始化，危险！
    auto x2 = 0;     // 必须初始化
    std::cout << "  优点1：auto变量必须初始化\n";
    (void)x1; (void)x2;
    
    // 优点2：避免类型不匹配
    std::vector<int> vec;
    // unsigned size = vec.size();  // 32/64位可能不兼容
    auto size = vec.size();         // 总是正确
    std::cout << "  优点2：auto自动匹配正确类型\n";
    
    // 优点3：避免冗长的类型名
    std::unique_ptr<std::vector<std::pair<int, std::string>>> ptr1;
    // C++11写法（注释）:
    // std::unique_ptr<std::vector<std::pair<int, std::string>>> ptr2(
    //     new std::vector<std::pair<int, std::string>>());
    auto ptr2 = std::make_unique<std::vector<std::pair<int, std::string>>>();
    std::cout << "  优点3：简化复杂类型声明\n";
    (void)ptr1; (void)ptr2;
    
    // 优点4：重构友好
    std::cout << "  优点4：重构时减少类型修改\n";
    
    // 注意事项
    std::cout << "\n  注意事项：\n";
    std::cout << "    - 代理类问题：vector<bool>\n";
    std::cout << "    - 需要明确类型意图时不用auto\n";
    std::cout << "    - 简单类型可用可不用\n";
}

// ============================================
// 主演示函数
// ============================================

void demonstrate_type_deduction() {
    item01_template_deduction();
    item02_auto_deduction();
    item03_decltype();
    item04_see_types();
    item05_prefer_auto();
}
