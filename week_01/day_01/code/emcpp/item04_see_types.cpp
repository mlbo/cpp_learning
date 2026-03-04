/**
 * @file item04_see_types.cpp
 * @brief EMC++条款4：学会查看推导类型
 * @author 35天C++学习计划
 * @date Day 1
 * 
 * 查看类型推导结果的方法：
 * 1. IDE提示
 * 2. 编译错误信息
 * 3. 运行时输出（typeid）
 * 4. boost::typeindex库
 */

#include <iostream>
#include <vector>
#include <string>
#include <typeinfo>
#include <type_traits>

using namespace std;

// =============================================================================
// 方法一：编译错误信息（最准确）
// =============================================================================

// 只声明不定义的模板类，用于触发编译错误
template<typename T>
struct TypeDisplayer;

// 使用示例：取消注释下面代码会产生编译错误，显示类型
// void showError() {
//     int x = 42;
//     TypeDisplayer<decltype(x)> td;  // 错误信息会显示类型
// }

void methodOne_CompileError() {
    cout << "=== 方法一：编译错误信息 ===" << endl;
    cout << "最准确的类型查看方式" << endl << endl;
    
    cout << "使用方法：" << endl;
    cout << "template<typename T>" << endl;
    cout << "struct TypeDisplayer;  // 只声明不定义" << endl << endl;
    
    cout << "int x = 42;" << endl;
    cout << "TypeDisplayer<decltype(x)> td;  // 编译错误显示类型" << endl;
    cout << "// 错误信息会显示：TypeDisplayer<int>" << endl;
    
    cout << endl << "【优点】" << endl;
    cout << "• 编译期确定，最准确" << endl;
    cout << "• 显示完整的类型信息" << endl;
    
    cout << endl << "【缺点】" << endl;
    cout << "• 需要触发编译错误" << endl;
    cout << "• 不能在运行时使用" << endl;
    
    cout << endl;
}

// =============================================================================
// 方法二：IDE提示（最方便）
// =============================================================================

void methodTwo_IDE() {
    cout << "=== 方法二：IDE提示 ===" << endl;
    cout << "最方便的类型查看方式" << endl << endl;
    
    // 示例代码（在IDE中可以看到提示）
    auto x = 42;  // IDE会显示：int x
    auto d = 3.14;  // IDE会显示：double d
    auto s = "hello";  // IDE会显示：const char* s
    
    vector<int> vec = {1, 2, 3};
    auto it = vec.begin();  // IDE会显示：__gnu_cxx::__normal_iterator...
    
    (void)x; (void)d; (void)s; (void)it;  // 避免未使用警告
    
    cout << "【示例】" << endl;
    cout << "auto x = 42;        // IDE显示：int" << endl;
    cout << "auto d = 3.14;      // IDE显示：double" << endl;
    cout << "auto s = \"hello\";   // IDE显示：const char*" << endl;
    cout << "auto it = vec.begin();  // IDE显示完整迭代器类型" << endl;
    
    cout << endl << "【优点】" << endl;
    cout << "• 无需额外代码" << endl;
    cout << "• 实时显示" << endl;
    cout << "• 不影响编译" << endl;
    
    cout << endl << "【缺点】" << endl;
    cout << "• 依赖IDE功能" << endl;
    cout << "• 不同IDE显示可能不同" << endl;
    
    cout << endl;
}

// =============================================================================
// 方法三：typeid（运行时）
// =============================================================================

void methodThree_Typeid() {
    cout << "=== 方法三：typeid运行时输出 ===" << endl;
    cout << "最简单的运行时查看方式" << endl << endl;
    
    int x = 42;
    const int cx = x;
    const int& rx = x;
    int* px = &x;
    
    auto a1 = x;   // int
    auto a2 = cx;  // int（顶层const被忽略）
    auto a3 = rx;  // int（引用和const被忽略）
    auto a4 = px;  // int*
    
    cout << "int x = 42;" << endl;
    cout << "const int cx = x;" << endl;
    cout << "const int& rx = x;" << endl;
    cout << "int* px = &x;" << endl << endl;
    
    cout << "typeid结果：" << endl;
    cout << "  typeid(x).name() = " << typeid(x).name() << endl;
    cout << "  typeid(cx).name() = " << typeid(cx).name() << endl;
    cout << "  typeid(rx).name() = " << typeid(rx).name() << endl;
    cout << "  typeid(px).name() = " << typeid(px).name() << endl;
    
    cout << endl << "auto推导后的typeid：" << endl;
    cout << "  typeid(a1).name() = " << typeid(a1).name() << " (auto a1 = x)" << endl;
    cout << "  typeid(a2).name() = " << typeid(a2).name() << " (auto a2 = cx)" << endl;
    cout << "  typeid(a3).name() = " << typeid(a3).name() << " (auto a3 = rx)" << endl;
    cout << "  typeid(a4).name() = " << typeid(a4).name() << " (auto a4 = px)" << endl;
    
    cout << endl << "【问题】" << endl;
    cout << "• 可读性差（如i表示int，PKc表示const char*）" << endl;
    cout << "• 不同编译器实现不同" << endl;
    cout << "• 无法区分引用类型" << endl;
    
    cout << endl << "【解码表（GCC/Clang）】" << endl;
    cout << "  i = int" << endl;
    cout << "  d = double" << endl;
    cout << "  f = float" << endl;
    cout << "  c = char" << endl;
    cout << "  PKc = const char*" << endl;
    cout << "  NSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE = std::string" << endl;
    
    cout << endl;
}

// =============================================================================
// 方法四：类型判断（编译期）
// =============================================================================

void methodFour_TypeTraits() {
    cout << "=== 方法四：类型判断（编译期） ===" << endl;
    cout << "使用<type_traits>判断类型特性" << endl << endl;
    
    int x = 0;
    const int cx = 0;
    int& rx = x;
    const int& rcx = cx;
    int* px = &x;
    
    auto a1 = x;
    auto a2 = cx;
    auto& a3 = cx;
    auto* a4 = &cx;
    
    cout << "类型特性判断：" << endl << endl;
    
    cout << "is_const：" << endl;
    cout << "  is_const<decltype(x)>::value = " << is_const<decltype(x)>::value << " (int)" << endl;
    cout << "  is_const<decltype(cx)>::value = " << is_const<decltype(cx)>::value << " (const int)" << endl;
    cout << "  is_const<decltype(a2)>::value = " << is_const<decltype(a2)>::value << " (auto a2 = cx)" << endl;
    
    cout << endl << "is_reference：" << endl;
    cout << "  is_reference<decltype(x)>::value = " << is_reference<decltype(x)>::value << " (int)" << endl;
    cout << "  is_reference<decltype(rx)>::value = " << is_reference<decltype(rx)>::value << " (int&)" << endl;
    cout << "  is_reference<decltype(a3)>::value = " << is_reference<decltype(a3)>::value << " (auto& a3 = cx)" << endl;
    
    cout << endl << "is_pointer：" << endl;
    cout << "  is_pointer<decltype(x)>::value = " << is_pointer<decltype(x)>::value << " (int)" << endl;
    cout << "  is_pointer<decltype(px)>::value = " << is_pointer<decltype(px)>::value << " (int*)" << endl;
    cout << "  is_pointer<decltype(a4)>::value = " << is_pointer<decltype(a4)>::value << " (auto* a4 = &cx)" << endl;
    
    cout << endl << "【常用类型判断】" << endl;
    cout << "• is_const<T> - 是否const" << endl;
    cout << "• is_reference<T> - 是否引用" << endl;
    cout << "• is_pointer<T> - 是否指针" << endl;
    cout << "• is_array<T> - 是否数组" << endl;
    cout << "• is_integral<T> - 是否整型" << endl;
    cout << "• is_floating_point<T> - 是否浮点型" << endl;
    
    cout << endl;
}

// =============================================================================
// 自定义类型显示工具
// =============================================================================

// 简易类型名称转换
template<typename T>
constexpr const char* getTypeName() {
    // 常见类型的简化名称
    if constexpr (is_same_v<T, int>) return "int";
    else if constexpr (is_same_v<T, double>) return "double";
    else if constexpr (is_same_v<T, float>) return "float";
    else if constexpr (is_same_v<T, char>) return "char";
    else if constexpr (is_same_v<T, bool>) return "bool";
    else if constexpr (is_same_v<T, const int>) return "const int";
    else if constexpr (is_same_v<T, const double>) return "const double";
    else if constexpr (is_same_v<T, int&>) return "int&";
    else if constexpr (is_same_v<T, const int&>) return "const int&";
    else if constexpr (is_same_v<T, int*>) return "int*";
    else if constexpr (is_same_v<T, const int*>) return "const int*";
    else if constexpr (is_same_v<T, std::string>) return "std::string";
    else return typeid(T).name();
}

// 详细类型信息显示
template<typename T>
void showDetailedType(const char* expr) {
    cout << "  " << expr << endl;
    cout << "    类型名称: " << getTypeName<T>() << endl;
    cout << "    是否const: " << is_const_v<T> << endl;
    cout << "    是否引用: " << is_reference_v<T> << endl;
    cout << "    是否指针: " << is_pointer_v<T> << endl;
}

void customTypeDisplay() {
    cout << "=== 自定义类型显示工具 ===" << endl << endl;
    
    int x = 0;
    const int cx = 0;
    int& rx = x;
    
    auto a1 = x;
    auto& a2 = cx;
    auto&& a3 = 42;
    
    cout << "详细类型信息：" << endl << endl;
    
    showDetailedType<decltype(x)>("decltype(x)");
    cout << endl;
    showDetailedType<decltype(cx)>("decltype(cx)");
    cout << endl;
    showDetailedType<decltype(rx)>("decltype(rx)");
    cout << endl;
    showDetailedType<decltype(a1)>("decltype(a1) (auto a1 = x)");
    cout << endl;
    showDetailedType<decltype(a2)>("decltype(a2) (auto& a2 = cx)");
    cout << endl;
    showDetailedType<decltype(a3)>("decltype(a3) (auto&& a3 = 42)");
    
    cout << endl;
}

// =============================================================================
// 实战示例
// =============================================================================

template<typename T>
void printTypeInfo(const T& param, const string& expr) {
    cout << "【" << expr << "】" << endl;
    
    // 使用type_traits判断
    using ParamType = decltype(param);
    
    cout << "  类型：" << getTypeName<ParamType>() << endl;
    cout << "  const: " << (is_const_v<remove_reference_t<ParamType>> ? "是" : "否") << endl;
    cout << "  引用: " << (is_reference_v<ParamType> ? "是" : "否");
    if (is_lvalue_reference_v<ParamType>) cout << " (左值)";
    if (is_rvalue_reference_v<ParamType>) cout << " (右值)";
    cout << endl;
    cout << "  指针: " << (is_pointer_v<ParamType> ? "是" : "否") << endl;
}

void practicalExample() {
    cout << "=== 实战示例 ===" << endl << endl;
    
    int x = 42;
    const int cx = x;
    const int& rx = x;
    
    printTypeInfo(x, "printTypeInfo(x)");
    cout << endl;
    printTypeInfo(cx, "printTypeInfo(cx)");
    cout << endl;
    printTypeInfo(rx, "printTypeInfo(rx)");
    cout << endl;
    
    // vector迭代器
    vector<int> vec = {1, 2, 3};
    auto it = vec.begin();
    printTypeInfo(it, "vector<int>::iterator");
    
    cout << endl;
}

// =============================================================================
// 总结
// =============================================================================

void printSummary() {
    cout << "=== 查看推导类型的方法总结 ===" << endl << endl;
    
    cout << "┌───────────────────┬─────────────────────────────────────────┐" << endl;
    cout << "│ 方法              │ 说明                                    │" << endl;
    cout << "├───────────────────┼─────────────────────────────────────────┤" << endl;
    cout << "│ IDE提示           │ 最方便，实时显示                        │" << endl;
    cout << "│ 编译错误          │ 最准确，编译期确定                      │" << endl;
    cout << "│ typeid            │ 运行时，可读性差                        │" << endl;
    cout << "│ type_traits       │ 编译期特性判断                          │" << endl;
    cout << "│ boost::typeindex  │ 可读性好，跨平台一致                    │" << endl;
    cout << "└───────────────────┴─────────────────────────────────────────┘" << endl;
    
    cout << endl << "【推荐流程】" << endl;
    cout << "1. 首先尝试IDE提示" << endl;
    cout << "2. 需要确认时用编译错误" << endl;
    cout << "3. 程序中使用type_traits判断" << endl;
    cout << "4. 生产代码考虑boost::typeindex" << endl;
    
    cout << endl;
}

// =============================================================================
// 主函数
// =============================================================================

int main() {
    cout << "╔════════════════════════════════════════════════════════════╗" << endl;
    cout << "║          EMC++条款4：学会查看推导类型                      ║" << endl;
    cout << "║          35天C++学习计划 - Day 1                           ║" << endl;
    cout << "╚════════════════════════════════════════════════════════════╝" << endl;
    cout << endl;
    
    methodOne_CompileError();
    methodTwo_IDE();
    methodThree_Typeid();
    methodFour_TypeTraits();
    customTypeDisplay();
    practicalExample();
    printSummary();
    
    cout << "程序执行完毕！" << endl;
    
    return 0;
}
