/**
 * @file nullptr_vs_null.cpp
 * @brief nullptr vs NULL vs 0 的详细对比
 */

#include <iostream>
#include <typeinfo>
#include <type_traits>
#include <cstddef>

// ============================================================================
// 问题演示：NULL和0的重载歧义问题
// ============================================================================

// 函数重载
void overloaded_function(int value) {
    std::cout << "  调用 overloaded_function(int): " << value << "\n";
}

void overloaded_function(int* ptr) {
    std::cout << "  调用 overloaded_function(int*): " 
              << (ptr ? "非空指针" : "空指针") << "\n";
}

void overloaded_function(long value) {
    std::cout << "  调用 overloaded_function(long): " << value << "\n";
}

void demo_overload_problem() {
    std::cout << "═══════════════════════════════════════════════════════════════\n";
    std::cout << "问题1: 函数重载歧义\n";
    std::cout << "═══════════════════════════════════════════════════════════════\n\n";
    
    std::cout << "调用 overloaded_function(0):\n";
    overloaded_function(0);  // 调用int版本
    
    std::cout << "调用 overloaded_function(NULL):\n";
    // NULL在C++中通常是0或0L，会调用int或long版本
    overloaded_function(NULL);
    
    std::cout << "调用 overloaded_function(nullptr):\n";
    overloaded_function(nullptr);  // 明确调用指针版本
    
    std::cout << "\n结论: nullptr能正确调用指针版本的重载函数\n\n";
}

// ============================================================================
// 问题演示：模板类型推导问题
// ============================================================================

template<typename T>
void check_template_param(T param) {
    std::cout << "  类型名: " << typeid(T).name() << "\n";
    std::cout << "  是指针类型: " << (std::is_pointer_v<T> ? "是" : "否") << "\n";
    std::cout << "  是整型: " << (std::is_integral_v<T> ? "是" : "否") << "\n";
    std::cout << "  是nullptr_t: " << (std::is_null_pointer_v<T> ? "是" : "否") << "\n";
}

template<typename T>
void forward_to_func(T param) {
    std::cout << "  模板参数类型: " << typeid(T).name() << "\n";
    std::cout << "  尝试调用重载函数: ";
    overloaded_function(param);  // 根据T调用不同版本
}

void demo_template_problem() {
    std::cout << "═══════════════════════════════════════════════════════════════\n";
    std::cout << "问题2: 模板类型推导\n";
    std::cout << "═══════════════════════════════════════════════════════════════\n\n";
    
    std::cout << "传入 0:\n";
    check_template_param(0);
    std::cout << "\n传入 NULL:\n";
    check_template_param(NULL);
    std::cout << "\n传入 nullptr:\n";
    check_template_param(nullptr);
    
    std::cout << "\n───────────────────────────────────────────────────────────────\n";
    std::cout << "实际效果对比:\n\n";
    
    std::cout << "传入 0 到模板函数:\n";
    forward_to_func(0);
    
    std::cout << "\n传入 NULL 到模板函数:\n";
    forward_to_func(NULL);
    
    std::cout << "\n传入 nullptr 到模板函数:\n";
    forward_to_func(nullptr);
    
    std::cout << "\n结论: nullptr能正确保留指针语义\n\n";
}

// ============================================================================
// 演示：类型特性对比
// ============================================================================

void demo_type_properties() {
    std::cout << "═══════════════════════════════════════════════════════════════\n";
    std::cout << "类型特性对比\n";
    std::cout << "═══════════════════════════════════════════════════════════════\n\n";
    
    // 打印表格
    std::cout << "┌──────────────┬────────────┬────────────┬────────────┐\n";
    std::cout << "│   特性       │     0      │    NULL    │  nullptr   │\n";
    std::cout << "├──────────────┼────────────┼────────────┼────────────┤\n";
    
    // 类型
    std::cout << "│ 实际类型     │    int     │";
    std::cout << " int/long   │nullptr_t   │\n";
    
    // 指针安全
    std::cout << "│ 指针安全     │     ❌     │     ❌     │     ✅     │\n";
    
    // 重载正确
    std::cout << "│ 重载正确     │     ❌     │     ❌     │     ✅     │\n";
    
    // 模板友好
    std::cout << "│ 模板友好     │     ❌     │     ❌     │     ✅     │\n";
    
    // 可读性
    std::cout << "│ 可读性       │     差     │     中     │     优     │\n";
    
    std::cout << "└──────────────┴────────────┴────────────┴────────────┘\n\n";
    
    // sizeof对比
    std::cout << "sizeof 对比:\n";
    std::cout << "  sizeof(0) = " << sizeof(0) << " bytes\n";
    std::cout << "  sizeof(NULL) = " << sizeof(NULL) << " bytes\n";
    std::cout << "  sizeof(nullptr) = " << sizeof(nullptr) << " bytes\n\n";
}

// ============================================================================
// 演示：实际编码中的陷阱
// ============================================================================

// 危险的函数
void process_pointer(int* ptr) {
    if (ptr == nullptr) {
        std::cout << "  收到空指针\n";
    } else {
        std::cout << "  处理值: " << *ptr << "\n";
    }
}

void demo_codding_traps() {
    std::cout << "═══════════════════════════════════════════════════════════════\n";
    std::cout << "编码陷阱示例\n";
    std::cout << "═══════════════════════════════════════════════════════════════\n\n";
    
    std::cout << "陷阱1: 用0传递给指针参数\n";
    process_pointer(0);  // 虽然编译通过，但语义不明确
    std::cout << "  → 编译器将0隐式转换为指针，但代码意图不明确\n\n";
    
    std::cout << "正确写法:\n";
    process_pointer(nullptr);  // 明确表示传递空指针
    std::cout << "  → nullptr明确表示空指针，代码意图清晰\n\n";
    
    // 陷阱2: 比较问题
    std::cout << "陷阱2: 指针与整数的比较\n";
    int* ptr = nullptr;
    if (ptr == 0) {
        std::cout << "  ptr == 0: true (编译器允许，但不推荐)\n";
    }
    if (ptr == nullptr) {
        std::cout << "  ptr == nullptr: true (推荐写法)\n";
    }
    std::cout << "\n";
    
    // 陷阱3: NULL的宏定义问题
    std::cout << "陷阱3: NULL的宏定义\n";
    std::cout << "  NULL在不同编译器中可能定义为:\n";
    std::cout << "    - 0\n";
    std::cout << "    - 0L\n";
    std::cout << "    - ((void*)0) (C风格，但C++中不推荐)\n";
    std::cout << "  这导致跨平台行为不一致\n\n";
}

// ============================================================================
// 演示：nullptr的独特特性
// ============================================================================

void demo_nullptr_unique_features() {
    std::cout << "═══════════════════════════════════════════════════════════════\n";
    std::cout << "nullptr的独特特性\n";
    std::cout << "═══════════════════════════════════════════════════════════════\n\n";
    
    // 特性1: 它是关键字
    std::cout << "1. nullptr是关键字，不是宏定义\n";
    std::cout << "   - 不可能被#undef或重定义\n";
    std::cout << "   - 行为完全可预测\n\n";
    
    // 特性2: 它有特定类型
    std::cout << "2. nullptr的类型是std::nullptr_t\n";
    std::nullptr_t n = nullptr;  // 可以声明nullptr_t类型的变量
    std::cout << "   - 可以声明: std::nullptr_t n = nullptr;\n";
    std::cout << "   - n的值仍然是空指针\n\n";
    
    // 特性3: 可以转换为任何指针类型
    std::cout << "3. 隐式转换为任何指针类型\n";
    int* ip = nullptr;
    double* dp = nullptr;
    void (*fp)() = nullptr;
    std::cout << "   int*, double*, 函数指针都可以初始化为nullptr\n\n";
    
    // 特性4: 不能转换为整数
    std::cout << "4. 不能隐式转换为整数类型\n";
    // int n = nullptr;  // 编译错误
    std::cout << "   int n = nullptr; // 编译错误！\n";
    std::cout << "   这防止了指针和整数的混淆\n\n";
    
    // 特性5: 可以用于布尔上下文（通过指针变量）
    std::cout << "5. 可以用于布尔上下文\n";
    int* ptr_from_null = n;  // nullptr隐式转换为int*
    if (!ptr_from_null) {
        std::cout << "   if(!nullptr) 总是true\n\n";
    }
    (void)ptr_from_null;  // 避免未使用变量警告
}

// ============================================================================
// 主函数
// ============================================================================
void run_nullptr_vs_null() {
    demo_overload_problem();
    demo_template_problem();
    demo_type_properties();
    demo_codding_traps();
    demo_nullptr_unique_features();
    
    std::cout << "═══════════════════════════════════════════════════════════════\n";
    std::cout << "nullptr vs NULL vs 0 对比演示完成\n";
    std::cout << "═══════════════════════════════════════════════════════════════\n";
}
