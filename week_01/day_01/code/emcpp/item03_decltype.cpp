/**
 * @file item03_decltype.cpp
 * @brief EMC++条款3：理解decltype
 * @author 35天C++学习计划
 * @date Day 1
 * 
 * decltype返回表达式或变量的精确类型
 * 包括引用和顶层const
 */

#include <iostream>
#include <vector>
#include <string>
#include <typeinfo>
#include <utility>

#include "safe_index_access.h"

using namespace std;

// =============================================================================
// decltype基本用法
// =============================================================================

void basicUsage() {
    cout << "=== decltype基本用法 ===" << endl;
    cout << "decltype返回精确类型，包括引用和const" << endl << endl;
    
    int x = 0;
    const int cx = 0;
    int& rx = x;
    const int& rcx = cx;
    
    // decltype(变量)
    decltype(x) y1 = 0;      // y1是int
    decltype(cx) y2 = 0;     // y2是const int
    decltype(rx) y3 = x;     // y3是int&
    decltype(rcx) y4 = cx;   // y4是const int&
    
    cout << "int x = 0;" << endl;
    cout << "const int cx = 0;" << endl;
    cout << "int& rx = x;" << endl;
    cout << "const int& rcx = cx;" << endl << endl;
    
    cout << "decltype(x) y1 = 0;     // y1是int" << endl;
    cout << "decltype(cx) y2 = 0;    // y2是const int" << endl;
    cout << "decltype(rx) y3 = x;    // y3是int&" << endl;
    cout << "decltype(rcx) y4 = cx;  // y4是const int&" << endl;
    
    // 验证
    y1 = 100;  // OK
    // y2 = 100;  // 编译错误！y2是const int
    y3 = 100;  // OK，修改x
    
    cout << endl << "【验证】" << endl;
    cout << "y1 = 100;  // OK" << endl;
    cout << "// y2 = 100;  // 编译错误！" << endl;
    cout << "y3 = 100;  // OK，修改x为 " << x << endl;
    (void)y1; (void)y2; (void)y4;
    
    cout << endl;
}

// =============================================================================
// 重要陷阱：decltype((变量))
// =============================================================================

void doubleParentheses() {
    cout << "=== 重要陷阱：decltype((变量)) ===" << endl;
    cout << "双括号会导致结果总是引用！" << endl << endl;
    
    int x = 0;
    
    decltype(x) y = 0;     // y是int
    decltype((x)) z = x;   // z是int&（双括号！）
    
    cout << "int x = 0;" << endl << endl;
    cout << "decltype(x) y = 0;    // y是int" << endl;
    cout << "decltype((x)) z = x;  // z是int&（双括号导致引用！）" << endl;
    
    // 验证
    z = 100;  // 修改x
    cout << endl << "【验证】" << endl;
    cout << "z = 100;  // 修改x为 " << x << endl;
    
    cout << endl << "【原因】" << endl;
    cout << "(x)是一个表达式，而x是左值表达式" << endl;
    cout << "对于左值表达式，decltype返回T&" << endl;
    (void)y;
    
    cout << endl;
}

// =============================================================================
// decltype与表达式
// =============================================================================

int globalVar = 10;
const int constGlobalVar = 20;

int& getRef() { return globalVar; }
int getValue() { return globalVar; }
const int& getConstRef() { return constGlobalVar; }

void expressionsAndDecltype() {
    cout << "=== decltype与表达式 ===" << endl << endl;
    
    // 函数调用
    decltype(getRef()) r1 = globalVar;       // int&
    decltype(getValue()) r2 = 0;             // int
    decltype(getConstRef()) r3 = constGlobalVar;  // const int&
    
    cout << "int& getRef() { ... }" << endl;
    cout << "int getValue() { ... }" << endl;
    cout << "const int& getConstRef() { ... }" << endl << endl;
    
    cout << "decltype(getRef()) r1 = ...;      // int&" << endl;
    cout << "decltype(getValue()) r2 = ...;    // int" << endl;
    cout << "decltype(getConstRef()) r3 = ...; // const int&" << endl;
    
    // 表达式类型
    int x = 0, y = 0;
    
    cout << endl << "表达式类型推导：" << endl;
    cout << "decltype(x + y) -> int（右值表达式）" << endl;
    cout << "decltype((x + y)) -> int（右值表达式）" << endl;
    cout << "decltype(x = 10) -> int&（赋值表达式返回左值引用）" << endl;
    cout << "decltype(++x) -> int&（前置++返回左值引用）" << endl;
    cout << "decltype(x++) -> int（后置++返回右值）" << endl;
    (void)r1; (void)r2; (void)r3; (void)x; (void)y;
    
    cout << endl;
}

// =============================================================================
// C++14：decltype(auto)
// =============================================================================

vector<int> values = {1, 2, 3, 4, 5};

// auto会丢失引用
auto getWithAuto(std::size_t index) {
    return values.at(index);  // 返回int拷贝
}

// decltype(auto)保留引用
decltype(auto) getWithDecltypeAuto(std::size_t index) {
    return values.at(index);  // 返回int&
}

//decltype(auto) getRefOrValue(bool returnRef) {
//    if (returnRef) {
//        return values[0];  // 返回int&
//    }
//    return 0;  // 错误！不能同时返回int&和int
//}

void decltypeAuto() {
    cout << "=== C++14：decltype(auto) ===" << endl;
    cout << "decltype(auto)用decltype规则推导auto" << endl << endl;
    
    // auto vs decltype(auto)
    cout << "vector<int> values = {1, 2, 3, 4, 5};" << endl << endl;
    
    cout << "auto getWithAuto(int index) {" << endl;
    cout << "    return values[index];  // 返回int拷贝" << endl;
    cout << "}" << endl << endl;
    
    cout << "decltype(auto) getWithDecltypeAuto(int index) {" << endl;
    cout << "    return values[index];  // 返回int&" << endl;
    cout << "}" << endl << endl;
    
    // 测试
    getWithDecltypeAuto(0) = 100;  // 修改values[0]
    // getWithAuto(1) = 200;  // 编译错误！返回的是拷贝
    
    cout << "测试：" << endl;
    cout << "getWithDecltypeAuto(0) = 100;  // OK，修改values[0]" << endl;
    cout << "// getWithAuto(1) = 200;  // 编译错误！" << endl;
    cout << "values[0] = " << values[0] << endl;
    
    // 变量声明
    cout << endl << "变量声明：" << endl;
    int x = 0;
    const int& crx = x;
    
    auto a1 = crx;            // int（auto忽略引用和const）
    decltype(auto) a2 = crx;  // const int&（decltype保留）
    
    cout << "const int& crx = x;" << endl;
    cout << "auto a1 = crx;            // int" << endl;
    cout << "decltype(auto) a2 = crx;  // const int&" << endl;
    (void)a1; (void)a2;
    
    cout << endl;
}

// =============================================================================
// 实战示例：精确保留元素引用并约束容器生命周期
// =============================================================================

void practicalExample() {
    cout << "=== 实战示例：精确保留元素引用 ===" << endl << endl;
    
    vector<int> vec = {10, 20, 30, 40, 50};
    
    cout << "vector<int> vec = {10, 20, 30, 40, 50};" << endl << endl;
    
    cout << "template<typename Container, typename Index>" << endl;
    cout << "decltype(auto) get_element(Container& c, Index i) {" << endl;
    cout << "    return c[i];" << endl;
    cout << "}" << endl << endl;
    
    // 左值容器 -> 返回引用
    cout << "左值容器调用：" << endl;
    get_element(vec, 0) = 100;  // 修改vec[0]
    cout << "get_element(vec, 0) = 100;" << endl;
    cout << "vec[0] = " << vec[0] << endl;
    
    cout << endl << "临时容器调用：" << endl;
    cout << "// get_element(vector<int>{1,2,3}, 1);  // 编译期拒绝" << endl;
    cout << "vector::operator[] 返回元素左值引用；临时 vector 销毁后该引用会悬空。" << endl;
    
    cout << endl;
}

// =============================================================================
// 对比auto和decltype
// =============================================================================

void compareAutoAndDecltype() {
    cout << "=== auto vs decltype对比 ===" << endl << endl;
    
    int x = 0;
    const int& crx = x;
    
    cout << "int x = 0;" << endl;
    cout << "const int& crx = x;" << endl << endl;
    
    cout << "┌─────────────────────┬─────────────────────┬─────────────────────┐" << endl;
    cout << "│ 表达式              │ auto结果            │ decltype结果        │" << endl;
    cout << "├─────────────────────┼─────────────────────┼─────────────────────┤" << endl;
    cout << "│ crx                 │ int                 │ const int&          │" << endl;
    cout << "│ x + 0               │ int                 │ int                 │" << endl;
    cout << "│ (x)                 │ int                 │ int&                │" << endl;
    cout << "│ x = 10              │ int                 │ int&                │" << endl;
    cout << "│ ++x                 │ int                 │ int&                │" << endl;
    cout << "│ x++                 │ int                 │ int                 │" << endl;
    cout << "└─────────────────────┴─────────────────────┴─────────────────────┘" << endl;
    
    cout << endl << "【关键区别】" << endl;
    cout << "• auto：忽略引用和顶层const" << endl;
    cout << "• decltype：保留引用和const" << endl;
    cout << "• decltype((x))：总是返回引用" << endl;
    (void)crx;
    
    cout << endl;
}

// =============================================================================
// 总结
// =============================================================================

void printSummary() {
    cout << "=== decltype总结 ===" << endl << endl;
    
    cout << "【核心规则】" << endl;
    cout << "1. decltype(变量) -> 返回变量的精确类型" << endl;
    cout << "2. decltype((变量)) -> 总是返回引用（陷阱！）" << endl;
    cout << "3. decltype(表达式) -> 表达式的结果类型" << endl;
    cout << endl;
    
    cout << "【C++14 decltype(auto)】" << endl;
    cout << "• 用decltype规则推导auto" << endl;
    cout << "• 保留引用和const" << endl;
    cout << "• 用于函数返回类型推导" << endl;
    cout << endl;
    
    cout << "【最佳实践】" << endl;
    cout << "✓ 需要精确类型时使用decltype" << endl;
    cout << "✓ 函数返回引用时使用decltype(auto)" << endl;
    cout << "✓ 注意双括号的陷阱" << endl;
    cout << "⚠ 不要在decltype(auto)函数中有不一致的返回类型" << endl;
    
    cout << endl;
}

// =============================================================================
// 主函数
// =============================================================================

int main() {
    cout << "╔════════════════════════════════════════════════════════════╗" << endl;
    cout << "║          EMC++条款3：理解decltype                          ║" << endl;
    cout << "║          35天C++学习计划 - Day 1                           ║" << endl;
    cout << "╚════════════════════════════════════════════════════════════╝" << endl;
    cout << endl;
    
    basicUsage();
    doubleParentheses();
    expressionsAndDecltype();
    decltypeAuto();
    practicalExample();
    compareAutoAndDecltype();
    printSummary();
    
    cout << "程序执行完毕！" << endl;
    
    return 0;
}
