/**
 * @file item02_auto_deduction.cpp
 * @brief EMC++条款2：理解auto类型推导
 * @author 35天C++学习计划
 * @date Day 1
 * 
 * auto类型推导与模板类型推导几乎相同
 * 唯一的例外是统一初始化列表的处理
 */

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <typeinfo>
#include <initializer_list>

using namespace std;

// =============================================================================
// auto推导与模板推导的对应关系
// =============================================================================

void correspondenceWithTemplate() {
    cout << "=== auto推导与模板推导的对应关系 ===" << endl << endl;
    
    // 变量声明与模板参数推导的对应
    // auto x = expr;  对应 template<typename T> void func(T param);
    // auto& x = expr; 对应 template<typename T> void func(T& param);
    // auto&& x = expr; 对应 template<typename T> void func(T&& param);
    
    cout << "对应关系表：" << endl;
    cout << "┌─────────────────────┬─────────────────────────────────────┐" << endl;
    cout << "│ auto声明            │ 对应模板参数                       │" << endl;
    cout << "├─────────────────────┼─────────────────────────────────────┤" << endl;
    cout << "│ auto x = expr       │ T param（值传递）                  │" << endl;
    cout << "│ auto& x = expr      │ T& param（引用）                   │" << endl;
    cout << "│ const auto& x = expr│ const T& param（const引用）        │" << endl;
    cout << "│ auto* x = expr      │ T* param（指针）                   │" << endl;
    cout << "│ auto&& x = expr     │ T&& param（万能引用）              │" << endl;
    cout << "└─────────────────────┴─────────────────────────────────────┘" << endl;
    
    cout << endl;
}

// =============================================================================
// 情况一：auto（值传递）
// =============================================================================

void caseOne_ByValue() {
    cout << "=== 情况一：auto（值传递）===" << endl;
    cout << "规则：忽略引用和顶层const" << endl << endl;
    
    int x = 27;
    const int cx = x;
    const int& rx = x;
    
    auto v1 = x;   // int
    auto v2 = cx;  // int（顶层const被忽略）
    auto v3 = rx;  // int（引用和const都被忽略）
    
    cout << "int x = 27;" << endl;
    cout << "const int cx = x;" << endl;
    cout << "const int& rx = x;" << endl << endl;
    
    cout << "auto v1 = x;   // v1是int" << endl;
    cout << "auto v2 = cx;  // v2是int（顶层const被忽略）" << endl;
    cout << "auto v3 = rx;  // v3是int（引用和const都被忽略）" << endl;
    
    // 验证
    v1 = 100; v2 = 100; v3 = 100;  // 都可以修改
    
    cout << endl << "【验证】所有变量都可以修改，说明不是const" << endl;
    
    // 指针示例
    cout << endl << "指针示例：" << endl;
    const int* const p = &x;
    auto v4 = p;  // const int*（顶层const被忽略，底层const保留）
    
    cout << "const int* const p = &x;" << endl;
    cout << "auto v4 = p;  // v4是const int*" << endl;
    cout << "顶层const（指针本身）被忽略，底层const（指向对象）保留" << endl;
    
    cout << endl;
}

// =============================================================================
// 情况二：auto&（引用）
// =============================================================================

void caseTwo_Reference() {
    cout << "=== 情况二：auto&（引用）===" << endl;
    cout << "规则：忽略表达式引用性，保留const" << endl << endl;
    
    int x = 27;
    const int cx = x;
    const int& rx = x;
    
    auto& r1 = x;   // int&
    auto& r2 = cx;  // const int&
    auto& r3 = rx;  // const int&（rx的引用性被忽略，但const保留）
    
    cout << "int x = 27;" << endl;
    cout << "const int cx = x;" << endl;
    cout << "const int& rx = x;" << endl << endl;
    
    cout << "auto& r1 = x;   // r1是int&" << endl;
    cout << "auto& r2 = cx;  // r2是const int&（保留const）" << endl;
    cout << "auto& r3 = rx;  // r3是const int&（rx的引用性被忽略）" << endl;
    
    // 验证
    r1 = 100;  // OK
    // r2 = 100;  // 编译错误！r2是const int&
    // r3 = 100;  // 编译错误！r3是const int&
    
    cout << endl << "【验证】r1可以修改，r2和r3不能修改（const）" << endl;
    
    cout << endl;
}

// =============================================================================
// 情况三：auto&&（万能引用）
// =============================================================================

void caseThree_UniversalReference() {
    cout << "=== 情况三：auto&&（万能引用）===" << endl;
    cout << "规则：左值推导为左值引用，右值推导为右值引用" << endl << endl;
    
    int x = 27;
    const int cx = x;
    
    // 左值初始化
    auto&& ur1 = x;   // int&（左值 -> 左值引用）
    auto&& ur2 = cx;  // const int&（左值 -> 左值引用）
    
    // 右值初始化
    auto&& ur3 = 27;  // int&&（右值 -> 右值引用）
    auto&& ur4 = x + 0;  // int&&（右值 -> 右值引用）
    
    cout << "int x = 27;" << endl;
    cout << "const int cx = x;" << endl << endl;
    
    cout << "auto&& ur1 = x;    // x是左值，ur1是int&" << endl;
    cout << "auto&& ur2 = cx;   // cx是左值，ur2是const int&" << endl;
    cout << "auto&& ur3 = 27;   // 27是右值，ur3是int&&" << endl;
    cout << "auto&& ur4 = x+0;  // x+0是右值，ur4是int&&" << endl;
    
    cout << endl << "【应用场景】" << endl;
    cout << "万能引用常用于转发和泛型编程" << endl;
    
    cout << endl;
}

// =============================================================================
// 例外：统一初始化
// =============================================================================

void exception_InitializerList() {
    cout << "=== 例外：统一初始化 ===" << endl;
    cout << "auto推导与模板推导的唯一区别！" << endl << endl;
    
    // 正常情况
    auto x1 = 42;      // int
    auto x2(42);       // int
    
    // 统一初始化
    auto x3 = {42};    // initializer_list<int>
    auto x4 = {1, 2, 3}; // initializer_list<int>
    
    // C++17规则
    auto x5{42};       // C++17: int（单元素）
    // auto x6{1, 2};  // C++17: 编译错误！需要用 = 
    
    cout << "auto x1 = 42;       // int" << endl;
    cout << "auto x2(42);        // int" << endl;
    cout << "auto x3 = {42};     // initializer_list<int>" << endl;
    cout << "auto x4 = {1,2,3};  // initializer_list<int>" << endl;
    cout << "auto x5{42};        // C++17: int（单元素规则变更）" << endl;
    
    // 模板对比
    cout << endl << "【对比：模板无法推导统一初始化】" << endl;
    cout << "template<typename T>" << endl;
    cout << "void func(T param);" << endl;
    cout << "func({1, 2, 3});  // 编译错误！无法推导" << endl;
    cout << endl;
    cout << "但auto可以：" << endl;
    cout << "auto x = {1, 2, 3};  // OK: initializer_list<int>" << endl;
    
    // 实际使用
    cout << endl << "【实际使用】" << endl;
    initializer_list<int> list = {1, 2, 3, 4, 5};
    cout << "initializer_list<int> list = {1, 2, 3, 4, 5};" << endl;
    cout << "元素: ";
    for (int n : list) cout << n << " ";
    cout << endl;
    
    cout << endl;
}

// =============================================================================
// 实战示例
// =============================================================================

void practicalExamples() {
    cout << "=== 实战示例 ===" << endl << endl;
    
    // 示例1：迭代器简化
    cout << "示例1：迭代器简化（最常见的auto用法）" << endl;
    
    map<string, vector<int>> complexData = {
        {"Alice", {95, 87, 92}},
        {"Bob", {88, 91, 85}}
    };
    
    // 传统写法
    cout << "传统写法：" << endl;
    cout << "  map<string, vector<int>>::iterator it = complexData.begin();" << endl;
    
    // auto写法
    cout << "auto写法：" << endl;
    cout << "  auto it = complexData.begin();" << endl;
    
    for (auto it = complexData.begin(); it != complexData.end(); ++it) {
        cout << "    " << it->first << ": ";
        for (int score : it->second) {
            cout << score << " ";
        }
        cout << endl;
    }
    
    // 示例2：范围for循环
    cout << endl << "示例2：范围for循环" << endl;
    vector<string> names = {"Alice", "Bob", "Charlie"};
    
    cout << "避免拷贝（推荐）：" << endl;
    cout << "  for (const auto& name : names)" << endl;
    for (const auto& name : names) {
        cout << "    " << name << endl;
    }
    
    // 示例3：智能指针
    cout << endl << "示例3：智能指针" << endl;
    auto ptr = make_unique<int>(42);
    auto sptr = make_shared<string>("Hello");
    
    cout << "auto ptr = make_unique<int>(42);" << endl;
    cout << "auto sptr = make_shared<string>(\"Hello\");" << endl;
    cout << "*ptr = " << *ptr << ", *sptr = " << *sptr << endl;
    
    // 示例4：Lambda表达式
    cout << endl << "示例4：Lambda表达式" << endl;
    auto add = [](int a, int b) { return a + b; };
    auto multiply = [](int a, int b) { return a * b; };
    
    cout << "auto add = [](int a, int b) { return a + b; };" << endl;
    cout << "add(3, 4) = " << add(3, 4) << endl;
    cout << "multiply(3, 4) = " << multiply(3, 4) << endl;
    
    cout << endl;
}

// =============================================================================
// 常见陷阱
// =============================================================================

void commonPitfalls() {
    cout << "=== 常见陷阱 ===" << endl << endl;
    
    // 陷阱1：字符串字面量
    cout << "陷阱1：字符串字面量" << endl;
    auto s1 = "hello";  // const char*，不是string！
    auto s2 = string("hello");  // string
    // auto s3 = "hello"s;  // string (C++14)
    
    cout << "  auto s1 = \"hello\";         // const char*（可能不是你想要的）" << endl;
    cout << "  auto s2 = string(\"hello\"); // std::string（明确）" << endl;
    
    // 陷阱2：类型截断
    cout << endl << "陷阱2：避免类型截断" << endl;
    vector<int> v = {1, 2, 3};
    auto size1 = v.size();  // 正确：size_type
    // unsigned size2 = v.size();  // 可能警告
    
    cout << "  auto size = v.size();  // 正确类型：vector<int>::size_type" << endl;
    
    // 陷阱3：vector<bool>特化
    cout << endl << "陷阱3：vector<bool>特化问题" << endl;
    vector<bool> bools = {true, false, true};
    // auto& ref = bools[0];  // 编译错误！vector<bool>不返回引用
    
    cout << "  vector<bool> bools = {true, false, true};" << endl;
    cout << "  auto& ref = bools[0];  // 编译错误！vector<bool>特化" << endl;
    cout << "解决方案：使用 decltype(auto) 或显式类型" << endl;
    
    // 陷阱4：map遍历
    cout << endl << "陷阱4：map遍历类型" << endl;
    map<string, int> scores = {{"Alice", 95}};
    
    // 错误：pair<string, int>不是map的元素类型
    // for (pair<string, int>& p : scores) { ... }  // 编译错误
    
    cout << "  // 错误写法：" << endl;
    cout << "  for (pair<string, int>& p : scores) { ... }  // 编译错误！" << endl;
    cout << "  // 正确写法：" << endl;
    cout << "  for (const auto& p : scores) { ... }  // auto推导正确类型" << endl;
    
    for (const auto& p : scores) {
        cout << "    " << p.first << ": " << p.second << endl;
    }
    
    cout << endl;
}

// =============================================================================
// 总结
// =============================================================================

void printSummary() {
    cout << "=== auto类型推导总结 ===" << endl << endl;
    
    cout << "【核心规则】" << endl;
    cout << "auto推导 = 模板推导 + 统一初始化例外" << endl << endl;
    
    cout << "【三种情况】" << endl;
    cout << "1. auto      -> 值传递，忽略引用和顶层const" << endl;
    cout << "2. auto&     -> 引用，保留const" << endl;
    cout << "3. auto&&    -> 万能引用，看值类别" << endl;
    cout << endl;
    
    cout << "【最佳实践】" << endl;
    cout << "✓ 优先使用auto简化复杂类型" << endl;
    cout << "✓ 使用const auto&避免拷贝" << endl;
    cout << "✓ 注意字符串字面量推导为const char*" << endl;
    cout << "✓ 统一初始化推导为initializer_list" << endl;
    
    cout << endl;
}

// =============================================================================
// 主函数
// =============================================================================

int main() {
    cout << "╔════════════════════════════════════════════════════════════╗" << endl;
    cout << "║          EMC++条款2：理解auto类型推导                      ║" << endl;
    cout << "║          35天C++学习计划 - Day 1                           ║" << endl;
    cout << "╚════════════════════════════════════════════════════════════╝" << endl;
    cout << endl;
    
    correspondenceWithTemplate();
    caseOne_ByValue();
    caseTwo_Reference();
    caseThree_UniversalReference();
    exception_InitializerList();
    practicalExamples();
    commonPitfalls();
    printSummary();
    
    cout << "程序执行完毕！" << endl;
    
    return 0;
}
