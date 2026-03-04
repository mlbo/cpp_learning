/**
 * @file item05_prefer_auto.cpp
 * @brief EMC++条款5：优先使用auto而非显式类型声明
 * @author 35天C++学习计划
 * @date Day 1
 * 
 * auto的优势：
 * 1. 避免未初始化变量
 * 2. 避免类型截断错误
 * 3. 简化复杂类型声明
 * 4. 自动适应类型变更
 */

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <functional>
#include <cstdint>

using namespace std;

// =============================================================================
// 优势一：避免未初始化变量
// =============================================================================

void advantage1_Initialization() {
    cout << "=== 优势一：避免未初始化变量 ===" << endl << endl;
    
    // 问题示例
    int x1;  // 未初始化！可能产生未定义行为
    // auto x2;  // 编译错误！必须初始化
    
    cout << "【问题】" << endl;
    cout << "int x1;  // 未初始化，可能产生未定义行为" << endl;
    cout << "auto x2;  // 编译错误！必须初始化" << endl;
    
    cout << endl << "【正确做法】" << endl;
    int x3 = 0;      // 显式初始化
    auto x4 = 0;     // auto强制初始化
    
    cout << "int x3 = 0;   // 显式初始化" << endl;
    cout << "auto x4 = 0;  // auto强制初始化" << endl;
    
    // 使用变量避免警告
    x1 = 0;
    (void)x3; (void)x4;
    
    cout << endl << "【结论】" << endl;
    cout << "auto变量必须初始化，可以避免使用未定义值" << endl;
    
    cout << endl;
}

// =============================================================================
// 优势二：避免类型截断
// =============================================================================

void advantage2_TypeTruncation() {
    cout << "=== 优势二：避免类型截断 ===" << endl << endl;
    
    // 问题示例：类型截断
    double pi = 3.141592653589793;
    int piInt = pi;  // 隐式转换，精度丢失！
    
    cout << "double pi = 3.141592653589793;" << endl;
    cout << "int piInt = pi;  // 隐式转换" << endl;
    cout << "结果: piInt = " << piInt << " (精度丢失)" << endl;
    
    // 使用auto避免
    auto piAuto = 3.141592653589793;  // 正确推导为double
    cout << endl << "auto piAuto = 3.141592653589793;" << endl;
    cout << "结果: piAuto = " << piAuto << " (保留精度)" << endl;
    
    // vector::size()问题
    cout << endl << "【vector::size()问题】" << endl;
    vector<int> vec(100);
    
    unsigned size1 = vec.size();  // 可能警告
    auto size2 = vec.size();      // 正确类型
    
    cout << "vector<int> vec(100);" << endl;
    cout << "unsigned size1 = vec.size();  // 可能警告" << endl;
    cout << "auto size2 = vec.size();      // 正确：size_type" << endl;
    cout << "size1 = " << size1 << ", size2 = " << size2 << endl;
    
    cout << endl << "【结论】" << endl;
    cout << "auto可以避免隐式类型转换导致的精度丢失" << endl;
    
    cout << endl;
}

// =============================================================================
// 优势三：简化复杂类型
// =============================================================================

void advantage3_SimplifyTypes() {
    cout << "=== 优势三：简化复杂类型声明 ===" << endl << endl;
    
    // 迭代器简化
    map<string, vector<pair<int, double>>> complexMap;
    complexMap["key"] = {{1, 1.1}, {2, 2.2}};
    
    cout << "map<string, vector<pair<int, double>>> complexMap;" << endl << endl;
    
    // 传统写法
    cout << "【传统写法】" << endl;
    cout << "map<string, vector<pair<int, double>>>::iterator it = complexMap.begin();" << endl;
    cout << "真的很长！" << endl;
    
    // auto写法
    cout << endl << "【auto写法】" << endl;
    cout << "auto it = complexMap.begin();" << endl;
    cout << "简洁清晰！" << endl;
    
    // 函数指针
    cout << endl << "【函数指针示例】" << endl;
    
    // 传统写法
    void (*func1)(int) = nullptr;  // 传统函数指针声明
    
    // auto写法
    auto func2 = [](int x) { cout << "x = " << x << endl; };
    
    cout << "传统: void (*func1)(int) = ...;" << endl;
    cout << "auto: auto func2 = [](int x) { ... };" << endl;
    
    func2(42);
    
    // 智能指针
    cout << endl << "【智能指针示例】" << endl;
    
    // 传统写法
    unique_ptr<vector<int>> ptr1(new vector<int>{1, 2, 3});
    
    // auto写法
    auto ptr2 = make_unique<vector<int>>(initializer_list<int>{1, 2, 3});
    
    cout << "传统: unique_ptr<vector<int>> ptr1(new vector<int>{...});" << endl;
    cout << "auto: auto ptr2 = make_unique<vector<int>>({...});" << endl;
    
    (void)func1; (void)ptr1; (void)ptr2;
    
    cout << endl << "【结论】" << endl;
    cout << "auto大幅简化复杂类型的声明" << endl;
    
    cout << endl;
}

// =============================================================================
// 优势四：自动适应类型变更
// =============================================================================

void advantage4_TypeChange() {
    cout << "=== 优势四：自动适应类型变更 ===" << endl << endl;
    
    // 假设函数返回类型可能变化
    auto getValue = []() -> int { return 42; };
    
    // 使用auto
    auto val1 = getValue();  // 如果返回类型变化，自动适应
    
    // 显式类型
    int val2 = getValue();   // 如果返回类型变化，需要修改这里
    
    cout << "auto val1 = getValue();  // 自动适应返回类型" << endl;
    cout << "int val2 = getValue();   // 返回类型变化时需要修改" << endl;
    
    cout << endl << "【重构示例】" << endl;
    cout << "假设getValue()从返回int改为返回int64_t：" << endl;
    cout << "• auto版本：无需修改，自动适应" << endl;
    cout << "• 显式类型：需要修改int为int64_t" << endl;
    
    (void)val1; (void)val2;
    
    cout << endl << "【结论】" << endl;
    cout << "使用auto可以减少代码维护工作" << endl;
    
    cout << endl;
}

// =============================================================================
// 优势五：正确处理复杂模板类型
// =============================================================================

void advantage5_TemplateTypes() {
    cout << "=== 优势五：正确处理复杂模板类型 ===" << endl << endl;
    
    // map遍历陷阱
    map<string, int> scores = {{"Alice", 95}, {"Bob", 87}};
    
    cout << "map<string, int> scores = {{\"Alice\", 95}, {\"Bob\", 87}};" << endl << endl;
    
    // 错误写法
    cout << "【错误写法】" << endl;
    cout << "for (pair<string, int>& p : scores) { ... }" << endl;
    cout << "问题：map的键是const的，实际类型是pair<const string, int>" << endl;
    cout << "这会导致编译错误或创建临时对象！" << endl;
    
    // 正确写法
    cout << endl << "【正确写法】" << endl;
    cout << "for (const auto& p : scores) { ... }" << endl;
    cout << "auto自动推导为pair<const string, int>" << endl;
    
    cout << endl << "演示：" << endl;
    for (const auto& p : scores) {
        cout << "  " << p.first << ": " << p.second << endl;
    }
    
    // lambda与function
    cout << endl << "【lambda与function】" << endl;
    
    // 传统写法
    function<void(int)> func1 = [](int x) { cout << x << endl; };
    
    // auto写法
    auto func2 = [](int x) { cout << x << endl; };
    
    cout << "传统: function<void(int)> func1 = [](int x){...};" << endl;
    cout << "auto: auto func2 = [](int x){...};" << endl;
    cout << "auto版本更高效（避免function的类型擦除开销）" << endl;
    
    func1(1); func2(2);
    
    cout << endl << "【结论】" << endl;
    cout << "auto可以正确处理模板容器的复杂类型" << endl;
    
    cout << endl;
}

// =============================================================================
// 注意事项
// =============================================================================

void cautions() {
    cout << "=== 使用auto的注意事项 ===" << endl << endl;
    
    // 陷阱1：可读性
    cout << "【陷阱1：可读性】" << endl;
    auto a = computeValue();  // 返回类型不明确
    
    cout << "问题代码：" << endl;
    cout << "auto a = computeValue();  // 返回类型不明确" << endl;
    cout << endl;
    cout << "建议：" << endl;
    cout << "• 对简单类型(int, double)，显式声明可能更清晰" << endl;
    cout << "• 对复杂类型，auto更简洁" << endl;
    
    // 陷阱2：代理对象
    cout << endl << "【陷阱2：代理对象】" << endl;
    vector<bool> bools = {true, false, true};
    
    // auto& ref = bools[0];  // 编译错误！vector<bool>返回代理对象
    
    cout << "vector<bool> bools = {true, false, true};" << endl;
    cout << "auto& ref = bools[0];  // 编译错误！" << endl;
    cout << "原因：vector<bool>特化返回代理对象，不是引用" << endl;
    cout << endl;
    cout << "解决方案：" << endl;
    cout << "bool b = bools[0];  // 显式类型" << endl;
    cout << "或 auto b = bools[0];  // 值拷贝" << endl;
    
    // 陷阱3：统一初始化
    cout << endl << "【陷阱3：统一初始化】" << endl;
    auto x1 = 42;      // int
    auto x2 = {42};    // initializer_list<int>
    
    cout << "auto x1 = 42;    // int" << endl;
    cout << "auto x2 = {42};  // initializer_list<int>（可能不是预期的）" << endl;
    
    // 陷阱4：表达式类型
    cout << endl << "【陷阱4：表达式类型】" << endl;
    int arr[5] = {1, 2, 3, 4, 5};
    auto a1 = arr;   // int*（数组退化）
    auto& a2 = arr;  // int(&)[5]（引用）
    
    cout << "int arr[5] = {1, 2, 3, 4, 5};" << endl;
    cout << "auto a1 = arr;   // int*（数组退化）" << endl;
    cout << "auto& a2 = arr;  // int(&)[5]（保持数组引用）" << endl;
    
    (void)a; (void)x1; (void)x2; (void)a1; (void)a2;
    
    cout << endl;
}

// 辅助函数
int computeValue() { return 42; }

// =============================================================================
// 总结
// =============================================================================

void printSummary() {
    cout << "=== 条款5总结 ===" << endl << endl;
    
    cout << "【auto的五大优势】" << endl;
    cout << "1. 避免未初始化变量（强制初始化）" << endl;
    cout << "2. 避免类型截断（正确推导类型）" << endl;
    cout << "3. 简化复杂类型（代码更清晰）" << endl;
    cout << "4. 自动适应变更（减少维护）" << endl;
    cout << "5. 正确处理模板类型（避免陷阱）" << endl;
    
    cout << endl << "【最佳实践】" << endl;
    cout << "✓ 优先使用auto声明变量" << endl;
    cout << "✓ 复杂类型迭代器、智能指针必用auto" << endl;
    cout << "✓ 使用const auto&避免拷贝" << endl;
    cout << "✓ 注意vector<bool>等代理对象" << endl;
    cout << "⚠ 简单类型可考虑显式声明（可读性）" << endl;
    cout << "⚠ 注意统一初始化的特殊处理" << endl;
    
    cout << endl << "【记住】" << endl;
    cout << "auto本身不会让代码变慢，它只是一个编译期特性。" << endl;
    cout << "关键是理解auto推导规则，正确使用它。" << endl;
    
    cout << endl;
}

// =============================================================================
// 主函数
// =============================================================================

int main() {
    cout << "╔════════════════════════════════════════════════════════════╗" << endl;
    cout << "║          EMC++条款5：优先使用auto                          ║" << endl;
    cout << "║          35天C++学习计划 - Day 1                           ║" << endl;
    cout << "╚════════════════════════════════════════════════════════════╝" << endl;
    cout << endl;
    
    advantage1_Initialization();
    advantage2_TypeTruncation();
    advantage3_SimplifyTypes();
    advantage4_TypeChange();
    advantage5_TemplateTypes();
    cautions();
    printSummary();
    
    cout << "程序执行完毕！" << endl;
    
    return 0;
}
