/**
 * @file auto_rules.cpp
 * @brief C++11 auto类型推导规则详解
 * @author 35天C++学习计划
 * @date Day 1
 * 
 * 本文件详细演示auto类型推导的三种情况：
 * 1. 情况一：auto& 或 auto*（引用或指针，非万能引用）
 * 2. 情况二：auto&&（万能引用）
 * 3. 情况三：auto（值传递）
 * 
 * auto推导与模板类型推导几乎完全相同，
 * 只有一个例外：统一初始化列表
 */

#include <iostream>
#include <vector>
#include <typeinfo>
#include <initializer_list>

using namespace std;

// 辅助函数：打印类型信息
template<typename T>
void printType(const string& varName) {
    cout << "  " << varName << " 的类型: " << typeid(T).name() << endl;
}

// =============================================================================
// 情况一：auto& 或 auto*（引用或指针声明，非万能引用）
// =============================================================================

void caseOne_ReferenceOrPointer() {
    cout << "=== 情况一：auto& 或 auto*（引用/指针，非万能引用）===" << endl;
    cout << "规则：忽略表达式的引用性，然后进行模式匹配" << endl << endl;
    
    int x = 27;
    const int cx = x;
    const int& rx = x;
    
    // auto& 推导规则
    cout << "auto& 推导示例：" << endl;
    
    auto& r1 = x;    // x是int，推导T=int，r1是int&
    auto& r2 = cx;   // cx是const int，推导T=const int，r2是const int&
    auto& r3 = rx;   // rx是const int&，忽略引用，T=const int，r3是const int&
    
    cout << "  int x = 27;" << endl;
    cout << "  const int cx = x;" << endl;
    cout << "  const int& rx = x;" << endl << endl;
    
    cout << "  auto& r1 = x;   // r1是int&" << endl;
    cout << "  auto& r2 = cx;  // r2是const int&（保留const）" << endl;
    cout << "  auto& r3 = rx;  // r3是const int&（rx的引用性被忽略）" << endl;
    
    // auto* 推导规则（类似的规则）
    cout << endl << "auto* 推导示例：" << endl;
    
    int* px = &x;
    const int* pcx = &cx;
    
    auto* p1 = &x;   // &x是int*，推导T=int，p1是int*
    auto* p2 = &cx;  // &cx是const int*，推导T=const int，p2是const int*
    auto* p3 = px;   // px是int*，推导T=int，p3是int*
    auto* p4 = pcx;  // pcx是const int*，推导T=const int，p4是const int*
    
    cout << "  auto* p1 = &x;   // p1是int*" << endl;
    cout << "  auto* p2 = &cx;  // p2是const int*（保留const）" << endl;
    cout << "  auto* p3 = px;   // p3是int*" << endl;
    cout << "  auto* p4 = pcx;  // p4是const int*" << endl;
    
    // 重要提示
    cout << endl << "【关键点】" << endl;
    cout << "• 引用和指针声明会保留const" << endl;
    cout << "• 表达式的引用性在匹配前被忽略" << endl;
    
    cout << endl;
}

// =============================================================================
// 情况二：auto&&（万能引用）
// =============================================================================

void caseTwo_UniversalReference() {
    cout << "=== 情况二：auto&&（万能引用）===" << endl;
    cout << "规则：根据初始化表达式的值类别决定推导结果" << endl << endl;
    
    int x = 27;
    const int cx = x;
    const int& rx = x;
    
    cout << "万能引用推导规则：" << endl;
    cout << "• 左值初始化 -> 推导为左值引用" << endl;
    cout << "• 右值初始化 -> 推导为右值引用" << endl << endl;
    
    // 左值初始化
    cout << "左值初始化示例：" << endl;
    
    auto&& ur1 = x;   // x是int左值，T=int&，ur1是int& && -> int&（引用折叠）
    auto&& ur2 = cx;  // cx是const int左值，T=const int&，ur2是const int&
    auto&& ur3 = rx;  // rx是const int&左值，T=const int&，ur3是const int&
    
    cout << "  int x = 27;" << endl;
    cout << "  const int cx = x;" << endl;
    cout << "  const int& rx = x;" << endl << endl;
    
    cout << "  auto&& ur1 = x;   // x是左值，ur1是int&" << endl;
    cout << "  auto&& ur2 = cx;  // cx是左值，ur2是const int&" << endl;
    cout << "  auto&& ur3 = rx;  // rx是左值，ur3是const int&" << endl;
    
    // 右值初始化
    cout << endl << "右值初始化示例：" << endl;
    
    auto&& ur4 = 27;   // 27是int右值，T=int，ur4是int&&
    auto&& ur5 = cx;   // cx是左值！（注意）ur5是const int&
    auto&& ur6 = x+0;  // x+0是右值，ur6是int&&
    
    cout << "  auto&& ur4 = 27;   // 27是右值，ur4是int&&" << endl;
    cout << "  auto&& ur5 = cx;   // cx是左值，ur5是const int&" << endl;
    cout << "  auto&& ur6 = x+0;  // x+0是右值，ur6是int&&" << endl;
    
    // 引用折叠规则
    cout << endl << "【引用折叠规则】" << endl;
    cout << "  &  + &  -> &" << endl;
    cout << "  &  + && -> &" << endl;
    cout << "  && + &  -> &" << endl;
    cout << "  && + && -> &&" << endl;
    cout << "结论：只要有左值引用参与，结果就是左值引用" << endl;
    
    cout << endl;
}

// =============================================================================
// 情况三：auto（值传递）
// =============================================================================

void caseThree_ByValue() {
    cout << "=== 情况三：auto（值传递）===" << endl;
    cout << "规则：忽略引用和顶层const，保留底层const" << endl << endl;
    
    int x = 27;
    const int cx = x;
    const int& rx = x;
    const int* const p = &x;  // 指向常量的常量指针
    
    cout << "int x = 27;" << endl;
    cout << "const int cx = x;" << endl;
    cout << "const int& rx = x;" << endl;
    cout << "const int* const p = &x;" << endl << endl;
    
    // 基本示例
    auto v1 = x;   // x是int，v1是int
    auto v2 = cx;  // cx是const int，忽略顶层const，v2是int
    auto v3 = rx;  // rx是const int&，忽略引用和const，v3是int
    
    cout << "基本推导：" << endl;
    cout << "  auto v1 = x;   // v1是int" << endl;
    cout << "  auto v2 = cx;  // v2是int（顶层const被忽略）" << endl;
    cout << "  auto v3 = rx;  // v3是int（引用和const都被忽略）" << endl;
    
    // 数组退化
    cout << endl << "数组退化示例：" << endl;
    
    const char name[] = "hello";  // name是const char[6]
    auto arr = name;  // 数组退化为指针，arr是const char*
    
    cout << "  const char name[] = \"hello\";  // name是const char[6]" << endl;
    cout << "  auto arr = name;  // arr是const char*（数组退化为指针）" << endl;
    
    // 指针示例
    cout << endl << "指针示例：" << endl;
    
    auto v4 = p;  // p是const int* const，忽略顶层const，v4是const int*
    
    cout << "  auto v4 = p;  // v4是const int*" << endl;
    cout << "  （顶层const被忽略，底层const保留）" << endl;
    
    // 关键区别
    cout << endl << "【顶层const vs 底层const】" << endl;
    cout << "顶层const：指针本身是const，不能指向其他地址" << endl;
    cout << "底层const：指向的对象是const，不能通过指针修改" << endl;
    cout << endl;
    cout << "int* const p1;       // p1是顶层const（指针本身）" << endl;
    cout << "const int* p2;       // p2是底层const（指向的对象）" << endl;
    cout << "const int* const p3; // 既有顶层又有底层const" << endl;
    
    cout << endl;
}

// =============================================================================
// 例外：统一初始化
// =============================================================================

void exception_InitializerList() {
    cout << "=== 例外情况：统一初始化 ===" << endl;
    cout << "auto推导与模板推导的唯一区别：" << endl;
    cout << "auto可以接受统一初始化，推导为initializer_list" << endl << endl;
    
    // 统一初始化示例
    auto x1 = 42;          // int
    auto x2(42);           // int（直接初始化）
    auto x3 = {42};        // initializer_list<int>
    auto x4 = {1, 2, 3};   // initializer_list<int>
    
    cout << "  auto x1 = 42;        // int" << endl;
    cout << "  auto x2(42);         // int" << endl;
    cout << "  auto x3 = {42};      // initializer_list<int>" << endl;
    cout << "  auto x4 = {1,2,3};   // initializer_list<int>" << endl;
    
    // C++17规则变更
    cout << endl << "【C++17规则变更】" << endl;
    cout << "  auto x5{42};    // C++11: initializer_list<int>" << endl;
    cout << "                  // C++17: int（单个元素）" << endl;
    cout << "  auto x6{1,2};   // C++17编译错误（多元素需要=）" << endl;
    
    // 模板对比
    cout << endl << "【与模板的对比】" << endl;
    cout << "模板不接受统一初始化：" << endl;
    cout << "  template<typename T>" << endl;
    cout << "  void func(T param);" << endl;
    cout << "  func({1,2,3});  // 编译错误！无法推导" << endl;
    cout << endl;
    cout << "但auto可以：" << endl;
    cout << "  auto x = {1,2,3};  // OK，initializer_list<int>" << endl;
    
    cout << endl;
}

// =============================================================================
// 实战示例
// =============================================================================

void practicalExamples() {
    cout << "=== 实战示例 ===" << endl;
    
    // 示例1：正确使用auto避免拷贝
    cout << "示例1：避免不必要的拷贝" << endl;
    
    vector<string> names = {"Alice", "Bob", "Charlie"};
    
    // 错误：产生拷贝
    // for (auto name : names) { ... }  // 每次循环都拷贝string
    
    // 正确：使用引用
    cout << "  for (const auto& name : names)  // 推荐：const引用" << endl;
    cout << "  for (auto& name : names)        // 需要修改时用非const引用" << endl;
    
    // 示例2：map遍历陷阱
    cout << endl << "示例2：map遍历的正确姿势" << endl;
    
    map<string, int> scores = {{"Alice", 95}, {"Bob", 87}};
    
    // 错误：pair<string, int>会产生拷贝，因为map的键是const的！
    // for (pair<string, int>& p : scores) { ... }  // 编译错误
    
    // 正确：使用auto
    cout << "  for (const auto& p : scores) {" << endl;
    cout << "      // p是 const pair<const string, int>&" << endl;
    cout << "      cout << p.first << \": \" << p.second << endl;" << endl;
    cout << "  }" << endl;
    
    // 示例3：lambda表达式
    cout << endl << "示例3：lambda与auto" << endl;
    
    auto add = [](int a, int b) { return a + b; };
    auto multiply = [](int a, int b) { return a * b; };
    
    cout << "  auto add = [](int a, int b) { return a + b; };" << endl;
    cout << "  auto multiply = [](int a, int b) { return a * b; };" << endl;
    cout << "  add(3, 4) = " << add(3, 4) << endl;
    cout << "  multiply(3, 4) = " << multiply(3, 4) << endl;
    
    // 示例4：函数返回值
    cout << endl << "示例4：复杂返回类型" << endl;
    
    auto getMap = []() -> map<string, vector<int>> {
        return {{"nums", {1, 2, 3}}};
    };
    
    auto m = getMap();
    cout << "  auto m = getMap();  // m的类型自动推导" << endl;
    cout << "  避免写出冗长的返回类型" << endl;
    
    cout << endl;
}

// =============================================================================
// 推导规则总结表
// =============================================================================

void printSummaryTable() {
    cout << "=== auto推导规则总结表 ===" << endl << endl;
    
    cout << "┌──────────────┬─────────────────────────────────────────────┐" << endl;
    cout << "│ 声明形式     │ 推导规则                                    │" << endl;
    cout << "├──────────────┼─────────────────────────────────────────────┤" << endl;
    cout << "│ auto         │ 忽略引用和顶层const，按值拷贝               │" << endl;
    cout << "│ auto&        │ 忽略引用，保留const，绑定到左值             │" << endl;
    cout << "│ const auto&  │ 忽略引用，const引用，只读访问               │" << endl;
    cout << "│ auto*        │ 指针，保留指向对象的const                   │" << endl;
    cout << "│ auto&&       │ 万能引用，左值推导为左值引用，               │" << endl;
    cout << "│              │ 右值推导为右值引用                          │" << endl;
    cout << "│ auto x={...} │ 推导为std::initializer_list<T>              │" << endl;
    cout << "└──────────────┴─────────────────────────────────────────────┘" << endl;
    
    cout << endl;
    
    cout << "【记忆口诀】" << endl;
    cout << "• 值传递（auto）：剥掉引用和顶层const" << endl;
    cout << "• 引用传递（auto&）：保留const，忽略表达式引用" << endl;
    cout << "• 万能引用（auto&&）：看初始化是左值还是右值" << endl;
    cout << "• 统一初始化：变成initializer_list" << endl;
    
    cout << endl;
}

// =============================================================================
// 主函数
// =============================================================================

int main() {
    cout << "╔════════════════════════════════════════════════════════════╗" << endl;
    cout << "║          C++11 auto 类型推导规则详解                       ║" << endl;
    cout << "║          35天C++学习计划 - Day 1                           ║" << endl;
    cout << "╚════════════════════════════════════════════════════════════╝" << endl;
    cout << endl;
    
    // 三种情况
    caseOne_ReferenceOrPointer();
    caseTwo_UniversalReference();
    caseThree_ByValue();
    
    // 例外情况
    exception_InitializerList();
    
    // 实战示例
    practicalExamples();
    
    // 总结
    printSummaryTable();
    
    // 最终建议
    cout << "=== 最终建议 ===" << endl;
    cout << "1. 理解auto推导就是模板推导（除了统一初始化）" << endl;
    cout << "2. 当需要拷贝时用auto" << endl;
    cout << "3. 当需要引用时用auto&或const auto&" << endl;
    cout << "4. 万能引用用auto&&（转发时使用）" << endl;
    cout << "5. 不确定时，用IDE查看推导结果" << endl;
    cout << endl;
    
    cout << "程序执行完毕！" << endl;
    
    return 0;
}
