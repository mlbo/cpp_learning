/**
 * @file decltype_rules.cpp
 * @brief decltype 推导规则详解
 * 
 * 本文件详细讲解 decltype 的三种推导规则：
 * 规则1: 标识符表达式 - 返回声明的类型
 * 规则2: 左值表达式（非标识符）- 返回 T&
 * 规则3: 右值表达式 - 返回 T
 */

#include <iostream>
#include <type_traits>
#include <iomanip>

// 辅助宏
#define PRINT_SEPARATOR() std::cout << "\n" << std::string(60, '=') << "\n"
#define PRINT_TITLE(title) std::cout << "\n【" << title << "】\n"

// 类型判断辅助函数
template<typename T>
void printTypeInfo(const char* expr) {
    std::cout << "  decltype(" << expr << ") 的类型分析:\n";
    
    if (std::is_reference<T>::value) {
        if (std::is_lvalue_reference<T>::value) {
            std::cout << "    - 是左值引用\n";
        }
        if (std::is_rvalue_reference<T>::value) {
            std::cout << "    - 是右值引用\n";
        }
    } else {
        std::cout << "    - 不是引用\n";
    }
    
    if (std::is_const<typename std::remove_reference<T>::type>::value) {
        std::cout << "    - 是 const 类型\n";
    }
    
    std::cout << "    - typeid: " << typeid(T).name() << "\n";
}

// 辅助宏简化类型打印
#define SHOW_TYPE(expr) printTypeInfo<decltype(expr)>(#expr)

/**
 * @brief 规则1：标识符表达式
 * 
 * 如果 expr 是一个标识符（变量名、类成员名），
 * decltype(expr) 返回该标识符的声明类型
 */
void demonstrateRule1() {
    PRINT_TITLE("规则1: 标识符表达式");
    
    std::cout << "规则: 如果 expr 是标识符，返回声明的类型\n";
    std::cout << "适用: 变量名、类成员访问（不涉及表达式）\n\n";
    
    // 基本类型
    int x = 10;
    SHOW_TYPE(x);
    std::cout << "  → int x 的声明类型是 int\n\n";
    
    // const 类型
    const int cx = 20;
    SHOW_TYPE(cx);
    std::cout << "  → const int cx 的声明类型是 const int\n\n";
    
    // 引用类型
    int y = 30;
    int& ref = y;
    SHOW_TYPE(ref);
    std::cout << "  → int& ref 的声明类型是 int&\n\n";
    
    // 指针类型
    int* ptr = &x;
    SHOW_TYPE(ptr);
    std::cout << "  → int* ptr 的声明类型是 int*\n\n";
    
    // 数组类型
    int arr[5] = {1, 2, 3, 4, 5};
    SHOW_TYPE(arr);
    std::cout << "  → int arr[5] 的声明类型是 int[5]\n\n";
    
    std::cout << "【规则1总结】\n";
    std::cout << "  decltype(变量名) 返回变量的声明类型，不做任何修改\n";
}

/**
 * @brief 规则2：左值表达式（非标识符）
 * 
 * 如果 expr 是一个左值表达式（但不是标识符），
 * decltype(expr) 返回 T&
 */
void demonstrateRule2() {
    PRINT_TITLE("规则2: 左值表达式（非标识符）");
    
    std::cout << "规则: 如果 expr 是左值表达式（非标识符），返回 T&\n";
    std::cout << "适用: 带括号的变量名、下标表达式、解引用、前置++等\n\n";
    
    // 双层括号 - 经典陷阱！
    int x = 10;
    SHOW_TYPE((x));  // 注意双层括号
    std::cout << "  → (x) 是左值表达式，返回 int&\n";
    std::cout << "  这是经典的 decltype 陷阱！\n\n";
    
    // 下标操作
    int arr[5] = {1, 2, 3, 4, 5};
    SHOW_TYPE(arr[0]);
    std::cout << "  → arr[0] 是左值表达式，返回 int&\n\n";
    
    // 解引用
    int* ptr = &x;
    SHOW_TYPE(*ptr);
    std::cout << "  → *ptr 是左值表达式，返回 int&\n\n";
    
    // 前置++
    SHOW_TYPE(++x);
    std::cout << "  → ++x 是左值表达式，返回 int&\n";
    std::cout << "  前置++返回修改后的对象（左值）\n\n";
    
    // 赋值表达式
    int y = 0;
    SHOW_TYPE((y = 10));
    std::cout << "  → (y = 10) 是左值表达式，返回 int&\n";
    std::cout << "  赋值表达式返回左值引用\n\n";
    
    // 字符串字面量
    SHOW_TYPE("hello");
    std::cout << "  → \"hello\" 是 const char[6] 类型的左值\n\n";
    
    std::cout << "【规则2总结】\n";
    std::cout << "  对于左值表达式，decltype 返回引用类型\n";
    std::cout << "  常见陷阱：decltype((x)) != decltype(x)\n";
}

/**
 * @brief 规则3：右值表达式
 * 
 * 如果 expr 是一个右值表达式（xvalue 或 prvalue），
 * decltype(expr) 返回 T（非引用）
 */
void demonstrateRule3() {
    PRINT_TITLE("规则3: 右值表达式");
    
    std::cout << "规则: 如果 expr 是右值表达式，返回 T\n";
    std::cout << "适用: 算术表达式、后置++、函数返回值等\n\n";
    
    int x = 10, y = 20;
    
    // 算术表达式
    SHOW_TYPE(x + y);
    std::cout << "  → x + y 是右值表达式，返回 int\n";
    std::cout << "  算术运算产生临时值（右值）\n\n";
    
    // 后置++
    SHOW_TYPE(x++);
    std::cout << "  → x++ 是右值表达式，返回 int\n";
    std::cout << "  后置++返回原始值的副本（右值）\n\n";
    
    // 字面量
    SHOW_TYPE(42);
    std::cout << "  → 42 是右值表达式，返回 int\n\n";
    
    SHOW_TYPE(3.14);
    std::cout << "  → 3.14 是右值表达式，返回 double\n\n";
    
    // 取地址
    SHOW_TYPE(&x);
    std::cout << "  → &x 是右值表达式，返回 int*\n";
    std::cout << "  取地址产生指针临时值\n\n";
    
    // 比较表达式
    SHOW_TYPE(x > y);
    std::cout << "  → x > y 是右值表达式，返回 bool\n\n";
    
    std::cout << "【规则3总结】\n";
    std::cout << "  对于右值表达式，decltype 返回非引用类型\n";
    std::cout << "  表达式产生临时值时适用此规则\n";
}

/**
 * @brief 规则对比和判断流程
 */
void demonstrateRuleComparison() {
    PRINT_TITLE("规则判断流程");
    
    std::cout << R"(
┌─────────────────────────────────────────────────────────────┐
│                   decltype(expr) 判断流程                    │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│   输入: expr                                                │
│                    │                                        │
│                    ▼                                        │
│   ┌─────────────────────────────────┐                       │
│   │ expr 是否为标识符？              │                       │
│   │ (变量名/类成员名)               │                       │
│   └────────────┬────────────────────┘                       │
│           是 / │ \ 否                                       │
│             /  │  \                                         │
│            ▼   │   ▼                                        │
│   ┌──────────┐ │ ┌──────────────────────┐                   │
│   │ 规则1    │ │ │ expr 是否为左值？     │                   │
│   │ 返回声明 │ │ │ (非标识符)            │                   │
│   │ 类型     │ │ └──────────┬───────────┘                   │
│   └──────────┘ │      是 /  │ \ 否                          │
│                │        /   │  \                            │
│                │       ▼    │   ▼                           │
│                │ ┌────────┐ │ ┌────────┐                    │
│                │ │ 规则2  │ │ │ 规则3  │                    │
│                │ │ 返回T& │ │ │ 返回T  │                    │
│                │ └────────┘ │ └────────┘                    │
│                └────────────┴───────────┘                   │
│                                                             │
└─────────────────────────────────────────────────────────────┘
)";

    std::cout << "\n典型例子对比:\n";
    
    int x = 10;
    int arr[5] = {1, 2, 3, 4, 5};
    
    std::cout << "\n┌─────────────────┬────────────────┬──────────────────┐\n";
    std::cout << "│     表达式      │    应用规则    │     结果类型     │\n";
    std::cout << "├─────────────────┼────────────────┼──────────────────┤\n";
    std::cout << "│ decltype(x)     │ 规则1: 标识符  │ int              │\n";
    std::cout << "│ decltype((x))   │ 规则2: 左值    │ int&             │\n";
    std::cout << "│ decltype(x+1)   │ 规则3: 右值    │ int              │\n";
    std::cout << "│ decltype(arr[0])│ 规则2: 左值    │ int&             │\n";
    std::cout << "│ decltype(*&x)   │ 规则2: 左值    │ int&             │\n";
    std::cout << "│ decltype(x++)   │ 规则3: 右值    │ int              │\n";
    std::cout << "│ decltype(++x)   │ 规则2: 左值    │ int&             │\n";
    std::cout << "└─────────────────┴────────────────┴──────────────────┘\n";
}

/**
 * @brief 实战练习
 */
void demonstratePractice() {
    PRINT_TITLE("实战练习");
    
    int a = 1, b = 2;
    int arr[3] = {10, 20, 30};
    int* p = arr;
    
    std::cout << "给定:\n";
    std::cout << "  int a = 1, b = 2;\n";
    std::cout << "  int arr[3] = {10, 20, 30};\n";
    std::cout << "  int* p = arr;\n\n";
    
    std::cout << "判断以下表达式的类型:\n\n";
    
    // 练习1
    std::cout << "练习1: decltype(a) v1 = a;\n";
    decltype(a) v1 = a;  // int
    std::cout << "  答案: int (规则1: a是标识符)\n\n";
    
    // 练习2
    std::cout << "练习2: decltype((a)) v2 = a;\n";
    decltype((a)) v2 = a;  // int&
    std::cout << "  答案: int& (规则2: (a)是左值表达式)\n\n";
    
    // 练习3
    std::cout << "练习3: decltype(a + b) v3 = a + b;\n";
    decltype(a + b) v3 = a + b;  // int
    std::cout << "  答案: int (规则3: a+b是右值表达式)\n\n";
    
    // 练习4
    std::cout << "练习4: decltype(arr[0]) v4 = arr[0];\n";
    decltype(arr[0]) v4 = arr[0];  // int&
    std::cout << "  答案: int& (规则2: arr[0]是左值表达式)\n\n";
    
    // 练习5
    std::cout << "练习5: decltype(*p) v5 = arr[0];\n";
    decltype(*p) v5 = arr[0];  // int&
    std::cout << "  答案: int& (规则2: *p是左值表达式)\n\n";
    
    // 练习6
    std::cout << "练习6: decltype(p + 1) v6 = p + 1;\n";
    decltype(p + 1) v6 = p + 1;  // int*
    std::cout << "  答案: int* (规则3: p+1是右值表达式)\n\n";
    
    // 练习7 - 经典陷阱
    std::cout << "练习7: decltype(a = b) v7 = a;\n";
    decltype(a = b) v7 = a;  // int&
    std::cout << "  答案: int& (规则2: 赋值表达式是左值表达式)\n";
    std::cout << "  注意: 赋值表达式返回左值引用！\n\n";
    
    // 使用 static_assert 验证
    static_assert(std::is_same<decltype(v1), int>::value, "v1 should be int");
    static_assert(std::is_same<decltype(v2), int&>::value, "v2 should be int&");
    static_assert(std::is_same<decltype(v3), int>::value, "v3 should be int");
    static_assert(std::is_same<decltype(v4), int&>::value, "v4 should be int&");
    static_assert(std::is_same<decltype(v5), int&>::value, "v5 should be int&");
    static_assert(std::is_same<decltype(v6), int*>::value, "v6 should be int*");
    static_assert(std::is_same<decltype(v7), int&>::value, "v7 should be int&");
    
    std::cout << "所有类型判断已通过 static_assert 验证！\n";
}

/**
 * @brief main 函数
 */
int main() {
    PRINT_SEPARATOR();
    std::cout << "      decltype 推导规则详解 - Day 2";
    PRINT_SEPARATOR();
    
    demonstrateRule1();
    demonstrateRule2();
    demonstrateRule3();
    demonstrateRuleComparison();
    demonstratePractice();
    
    PRINT_SEPARATOR();
    std::cout << "规则讲解结束！\n";
    PRINT_SEPARATOR();
    
    return 0;
}
