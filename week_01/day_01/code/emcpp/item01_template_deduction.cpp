/**
 * @file item01_template_deduction.cpp
 * @brief EMC++条款1：理解模板类型推导
 * @author 35天C++学习计划
 * @date Day 1
 * 
 * 模板类型推导是理解auto类型推导的基础
 * 本文件详细演示模板类型推导的三种情况
 */

#include <iostream>
#include <vector>
#include <string>
#include <typeinfo>
#include <iomanip>

using namespace std;

// =============================================================================
// 辅助工具：类型显示
// =============================================================================

// 用于在编译期显示类型（产生编译错误来查看类型）
template<typename T>
struct TypeDisplayer;

// 辅助函数：打印类型信息
template<typename T>
void showType(const string& expr, const string& result) {
    cout << "  " << left << setw(30) << expr << " -> " << result << endl;
}

// =============================================================================
// 情况一：ParamType是指针或引用（非万能引用）
// =============================================================================

// 模板函数声明
template<typename T>
void funcRef(T& param);  // param是引用

template<typename T>
void funcPtr(T* param);  // param是指针

template<typename T>
void funcConstRef(const T& param);  // param是const引用

void caseOne_ReferenceOrPointer() {
    cout << "=== 情况一：ParamType是指针或引用（非万能引用）===" << endl;
    cout << "规则：忽略表达式的引用性，然后进行模式匹配" << endl << endl;
    
    int x = 27;
    const int cx = x;
    const int& rx = x;
    
    cout << "初始变量：" << endl;
    cout << "  int x = 27;" << endl;
    cout << "  const int cx = x;" << endl;
    cout << "  const int& rx = x;" << endl << endl;
    
    cout << "模板: template<typename T> void funcRef(T& param);" << endl;
    cout << "推导结果：" << endl;
    
    // funcRef(x);    // T = int, param = int&
    showType("funcRef(x)", "T = int, param = int&");
    
    // funcRef(cx);   // T = const int, param = const int&
    showType("funcRef(cx)", "T = const int, param = const int&");
    
    // funcRef(rx);   // T = const int, param = const int&（引用性被忽略）
    showType("funcRef(rx)", "T = const int, param = const int&");
    
    cout << endl << "【关键点】" << endl;
    cout << "• 传递const对象时，T被推导为const int" << endl;
    cout << "• 传递引用时，引用性在推导前被忽略" << endl;
    cout << "• param的类型包含const（保留表达式的const）" << endl;
    
    // 指针示例
    cout << endl << "指针示例：" << endl;
    cout << "模板: template<typename T> void funcPtr(T* param);" << endl;
    
    int* px = &x;
    const int* pcx = &cx;
    
    showType("funcPtr(&x)", "T = int, param = int*");
    showType("funcPtr(&cx)", "T = const int, param = const int*");
    showType("funcPtr(px)", "T = int, param = int*");
    showType("funcPtr(pcx)", "T = const int, param = const int*");
    
    // const引用参数
    cout << endl << "const引用参数示例：" << endl;
    cout << "模板: template<typename T> void funcConstRef(const T& param);" << endl;
    
    showType("funcConstRef(x)", "T = int, param = const int&");
    showType("funcConstRef(cx)", "T = int, param = const int&");
    showType("funcConstRef(rx)", "T = int, param = const int&");
    
    cout << endl << "【注意】param已经是const T&，所以T不会被推导为const" << endl;
    
    cout << endl;
}

// =============================================================================
// 情况二：ParamType是万能引用
// =============================================================================

template<typename T>
void funcUniversalRef(T&& param);  // 万能引用

void caseTwo_UniversalReference() {
    cout << "=== 情况二：ParamType是万能引用 ===" << endl;
    cout << "规则：根据表达式的值类别决定推导结果" << endl << endl;
    
    int x = 27;
    const int cx = x;
    const int& rx = x;
    
    cout << "初始变量：" << endl;
    cout << "  int x = 27;" << endl;
    cout << "  const int cx = x;" << endl;
    cout << "  const int& rx = x;" << endl << endl;
    
    cout << "模板: template<typename T> void funcUniversalRef(T&& param);" << endl;
    cout << "推导结果：" << endl;
    
    // 左值传递
    showType("funcUniversalRef(x)", "x是左值 -> T = int&, param = int&");
    showType("funcUniversalRef(cx)", "cx是左值 -> T = const int&, param = const int&");
    showType("funcUniversalRef(rx)", "rx是左值 -> T = const int&, param = const int&");
    
    // 右值传递
    showType("funcUniversalRef(27)", "27是右值 -> T = int, param = int&&");
    showType("funcUniversalRef(x+0)", "x+0是右值 -> T = int, param = int&&");
    
    cout << endl << "【引用折叠规则】" << endl;
    cout << "  &  + &  -> &" << endl;
    cout << "  &  + && -> &" << endl;
    cout << "  && + &  -> &" << endl;
    cout << "  && + && -> &&" << endl;
    cout << endl;
    cout << "结论：左值参数导致T被推导为左值引用，右值参数导致T被推导为非引用" << endl;
    
    cout << endl;
}

// =============================================================================
// 情况三：ParamType既非指针也非引用
// =============================================================================

template<typename T>
void funcByValue(T param);  // 按值传递

void caseThree_ByValue() {
    cout << "=== 情况三：ParamType既非指针也非引用 ===" << endl;
    cout << "规则：忽略引用和顶层const，保留底层const" << endl << endl;
    
    int x = 27;
    const int cx = x;
    const int& rx = x;
    const int* const p = &x;  // 指向常量的常量指针
    
    cout << "初始变量：" << endl;
    cout << "  int x = 27;" << endl;
    cout << "  const int cx = x;" << endl;
    cout << "  const int& rx = x;" << endl;
    cout << "  const int* const p = &x;" << endl << endl;
    
    cout << "模板: template<typename T> void funcByValue(T param);" << endl;
    cout << "推导结果：" << endl;
    
    showType("funcByValue(x)", "T = int, param = int");
    showType("funcByValue(cx)", "T = int, param = int (顶层const被忽略)");
    showType("funcByValue(rx)", "T = int, param = int (引用和const都被忽略)");
    showType("funcByValue(p)", "T = const int*, param = const int* (顶层const被忽略)");
    
    // 数组退化
    cout << endl << "【数组退化示例】" << endl;
    const char name[] = "Hello";  // name是const char[6]
    showType("funcByValue(name)", "T = const char*, param = const char* (数组退化为指针)");
    
    // 函数退化
    cout << endl << "【函数指针退化示例】" << endl;
    showType("funcByValue(printf)", "T = int(*)(const char*, ...), 函数退化为函数指针");
    
    cout << endl << "【关键点】" << endl;
    cout << "• 顶层const被忽略（指针本身的const）" << endl;
    cout << "• 底层const被保留（指向对象的const）" << endl;
    cout << "• 数组退化为指针" << endl;
    cout << "• 函数退化为函数指针" << endl;
    
    cout << endl;
}

// =============================================================================
// 实战示例
// =============================================================================

// 实际的模板函数实现
template<typename T>
void processRef(T& param) {
    cout << "  T的类型: " << typeid(T).name() << endl;
    cout << "  param的类型: " << typeid(param).name() << endl;
    param = 100;  // 可以修改
}

template<typename T>
void processConstRef(const T& param) {
    cout << "  T的类型: " << typeid(T).name() << endl;
    // param = 100;  // 编译错误！不能修改const引用
}

template<typename T>
void processByValue(T param) {
    cout << "  T的类型: " << typeid(T).name() << endl;
    param = 100;  // 修改的是副本
}

void practicalExamples() {
    cout << "=== 实战示例 ===" << endl << endl;
    
    // 示例1：正确选择参数类型
    cout << "示例1：选择正确的参数类型" << endl;
    int value = 42;
    cout << "需要修改原值时使用T&：" << endl;
    processRef(value);
    cout << "修改后 value = " << value << endl;
    
    cout << endl << "只读访问时使用const T&：" << endl;
    processConstRef(value);
    
    cout << endl << "需要副本时使用T：" << endl;
    processByValue(value);
    cout << "原值不受影响: value = " << value << endl;
    
    // 示例2：数组处理
    cout << endl << "示例2：数组参数推导" << endl;
    int arr[] = {1, 2, 3, 4, 5};
    cout << "传递数组给T&参数：" << endl;
    processRef(arr);  // T = int[5], param = int(&)[5]
    
    cout << endl << "传递数组给T参数（退化）：" << endl;
    processByValue(arr);  // T = int*, param = int*
    
    cout << endl;
}

// =============================================================================
// 总结表
// =============================================================================

void printSummary() {
    cout << "=== 模板类型推导总结表 ===" << endl << endl;
    
    cout << "┌─────────────────────────┬────────────────────────────────────────────┐" << endl;
    cout << "│ ParamType               │ 推导规则                                   │" << endl;
    cout << "├─────────────────────────┼────────────────────────────────────────────┤" << endl;
    cout << "│ T&（引用）              │ 忽略表达式引用性，保留const，模式匹配      │" << endl;
    cout << "│ const T&（const引用）   │ 忽略表达式const和引用，T不会被推导为const  │" << endl;
    cout << "│ T*（指针）              │ 类似引用，保留指向对象的const              │" << endl;
    cout << "│ T&&（万能引用）         │ 左值->左值引用，右值->右值引用             │" << endl;
    cout << "│ T（值传递）             │ 忽略引用和顶层const，数组/函数退化为指针   │" << endl;
    cout << "└─────────────────────────┴────────────────────────────────────────────┘" << endl;
    
    cout << endl;
    cout << "【记忆要点】" << endl;
    cout << "1. 引用参数保留const" << endl;
    cout << "2. 万能引用看值类别（左值/右值）" << endl;
    cout << "3. 值参数忽略顶层const，退化数组/函数" << endl;
    cout << endl;
}

// =============================================================================
// 主函数
// =============================================================================

int main() {
    cout << "╔════════════════════════════════════════════════════════════╗" << endl;
    cout << "║          EMC++条款1：理解模板类型推导                      ║" << endl;
    cout << "║          35天C++学习计划 - Day 1                           ║" << endl;
    cout << "╚════════════════════════════════════════════════════════════╝" << endl;
    cout << endl;
    
    caseOne_ReferenceOrPointer();
    caseTwo_UniversalReference();
    caseThree_ByValue();
    practicalExamples();
    printSummary();
    
    cout << "程序执行完毕！" << endl;
    
    return 0;
}
