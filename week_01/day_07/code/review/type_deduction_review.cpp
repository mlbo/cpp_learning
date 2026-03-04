/**
 * @file type_deduction_review.cpp
 * @brief 类型推导复习 - auto, decltype, 模板类型推导
 */

#include <iostream>
#include <type_traits>
#include <vector>
#include <memory>
#include <utility>

// 辅助宏：打印类型信息
#define PRINT_TYPE(expr) \
    std::cout << "  " << #expr << " 的类型: " << typeid(expr).name() << "\n"

#define PRINT_TYPE_TRAIT(expr, trait) \
    std::cout << "  " << #expr << " is_" << #trait << ": " \
              << std::trait##_v<decltype(expr)> << "\n"

// ============================================================================
// 1. auto 类型推导复习
// ============================================================================

void auto_deduction_demo() {
    std::cout << "\n【1. auto 基本推导规则】\n";
    std::cout << "-------------------------------------------\n";

    // 情况1: 指针或引用类型
    int x = 42;
    const int& ref_x = x;
    
    auto a = ref_x;           // int (忽略引用和顶层const)
    auto& b = ref_x;          // const int& (保留const)
    auto* c = &x;             // int*
    const auto d = x;         // const int

    std::cout << "  const int& ref_x = x;\n";
    std::cout << "  auto a = ref_x;     → int (忽略引用和顶层const)\n";
    std::cout << "  auto& b = ref_x;    → const int& (保留const)\n";
    std::cout << "  auto* c = &x;       → int*\n";
    std::cout << "  const auto d = x;   → const int\n";

    // 情况2: 通用引用 (universal reference)
    auto&& r1 = x;            // int& (x是左值)
    auto&& r2 = 42;           // int&& (42是右值)

    std::cout << "\n  auto&& r1 = x;      → int& (左值推导为左值引用)\n";
    std::cout << "  auto&& r2 = 42;     → int&& (右值推导为右值引用)\n";
    std::cout << "  验证: is_lvalue_reference<decltype(r1)> = " 
              << std::is_lvalue_reference_v<decltype(r1)> << "\n";
    std::cout << "  验证: is_rvalue_reference<decltype(r2)> = " 
              << std::is_rvalue_reference_v<decltype(r2)> << "\n";

    // 情况3: 初始化列表
    auto e = {1, 2, 3};       // std::initializer_list<int>
    auto f = {1};             // std::initializer_list<int>
    // auto g{1};             // C++17: int (单个元素)
    
    std::cout << "\n  auto e = {1, 2, 3};  → std::initializer_list<int>\n";
    std::cout << "  auto f = {1};        → std::initializer_list<int>\n";
}

// ============================================================================
// 2. decltype 类型推导复习
// ============================================================================

void decltype_demo() {
    std::cout << "\n【2. decltype 推导规则】\n";
    std::cout << "-------------------------------------------\n";

    int x = 42;
    
    decltype(x) a = x;        // int
    decltype((x)) b = x;      // int& (注意双括号!)
    
    std::cout << "  int x = 42;\n";
    std::cout << "  decltype(x) a = x;    → int\n";
    std::cout << "  decltype((x)) b = x;  → int& (双括号返回引用!)\n";

    // decltype(auto) 示例
    auto get_ref = [&]() -> decltype(auto) {
        return (x);  // 返回 int&
    };
    
    decltype(auto) c = get_ref();
    std::cout << "\n  decltype(auto) c = get_ref(); → int&\n";
    std::cout << "  验证: is_lvalue_reference<decltype(c)> = " 
              << std::is_lvalue_reference_v<decltype(c)> << "\n";
}

// ============================================================================
// 3. 模板类型推导复习
// ============================================================================

template<typename T>
void template_deduction(T param) {
    std::cout << "  T = " << typeid(T).name() << "\n";
    std::cout << "  ParamType = " << typeid(param).name() << "\n";
}

template<typename T>
void template_deduction_ref(T& param) {
    std::cout << "  T = " << typeid(T).name() << "\n";
    std::cout << "  ParamType (T&) = " << typeid(param).name() << "\n";
}

template<typename T>
void template_deduction_universal(T&& param) {
    std::cout << "  T = " << typeid(T).name();
    if constexpr (std::is_lvalue_reference_v<T>) {
        std::cout << " (lvalue ref)";
    }
    std::cout << "\n";
    std::cout << "  ParamType (T&&) = " << typeid(param).name() << "\n";
}

void template_type_deduction_demo() {
    std::cout << "\n【3. 模板类型推导】\n";
    std::cout << "-------------------------------------------\n";

    int x = 42;
    const int cx = x;
    const int& rx = x;

    std::cout << "情况1: 按值传递 (ParamType = T)\n";
    std::cout << "  template_deduction(x): ";
    template_deduction(x);
    std::cout << "  template_deduction(cx): ";
    template_deduction(cx);
    std::cout << "  template_deduction(rx): ";
    template_deduction(rx);

    std::cout << "\n情况2: 引用传递 (ParamType = T&)\n";
    std::cout << "  template_deduction_ref(x): ";
    template_deduction_ref(x);
    std::cout << "  template_deduction_ref(cx): ";
    template_deduction_ref(cx);

    std::cout << "\n情况3: 通用引用 (ParamType = T&&)\n";
    std::cout << "  template_deduction_universal(x): ";
    template_deduction_universal(x);  // T = int&
    std::cout << "  template_deduction_universal(42): ";
    template_deduction_universal(42); // T = int
}

// ============================================================================
// 4. 实际应用场景
// ============================================================================

void practical_usage_demo() {
    std::cout << "\n【4. 实际应用场景】\n";
    std::cout << "-------------------------------------------\n";

    // 1. 迭代器类型自动推导
    std::vector<int> vec = {1, 2, 3, 4, 5};
    auto it = vec.begin();  // std::vector<int>::iterator
    
    std::cout << "  // 1. 迭代器\n";
    std::cout << "  std::vector<int> vec = {1, 2, 3, 4, 5};\n";
    std::cout << "  auto it = vec.begin();  // 自动推导迭代器类型\n";

    // 2. 智能指针
    auto ptr = std::make_unique<int>(42);
    auto sptr = std::make_shared<int>(100);
    
    std::cout << "\n  // 2. 智能指针\n";
    std::cout << "  auto ptr = std::make_unique<int>(42);\n";
    std::cout << "  auto sptr = std::make_shared<int>(100);\n";

    // 3. 结构化绑定 (C++17)
    std::pair<int, std::string> p{42, "hello"};
    auto [num, str] = p;
    
    std::cout << "\n  // 3. 结构化绑定\n";
    std::cout << "  std::pair<int, std::string> p{42, \"hello\"};\n";
    std::cout << "  auto [num, str] = p;\n";
    std::cout << "  num = " << num << ", str = \"" << str << "\"\n";

    // 4. 范围for循环
    std::cout << "\n  // 4. 范围for循环\n";
    std::cout << "  for (const auto& elem : vec) { /* ... */ }\n";
}

// ============================================================================
// 5. 常见陷阱
// ============================================================================

void common_pitfalls_demo() {
    std::cout << "\n【5. 常见陷阱】\n";
    std::cout << "-------------------------------------------\n";

    // 陷阱1: auto与初始化列表
    std::cout << "  // 陷阱1: auto + 初始化列表\n";
    auto x1 = 42;     // int
    auto x2(42);      // int
    auto x3 = {42};   // initializer_list<int>
    // auto x4{42};   // C++17: int; C++11: initializer_list<int>
    
    std::cout << "  auto x1 = 42;     → int\n";
    std::cout << "  auto x2(42);      → int\n";
    std::cout << "  auto x3 = {42};   → initializer_list<int>\n";

    // 陷阱2: vector<bool>代理类型
    std::cout << "\n  // 陷阱2: vector<bool> 代理类型\n";
    std::vector<bool> bool_vec = {true, false, true};
    auto elem = bool_vec[0];  // 不是bool，是代理类型
    
    std::cout << "  std::vector<bool> bool_vec = {true, false, true};\n";
    std::cout << "  auto elem = bool_vec[0];  // 不是bool!\n";
    std::cout << "  类型: " << typeid(elem).name() << "\n";

    // 陷阱3: 数组退化为指针
    std::cout << "\n  // 陷阱3: 数组退化\n";
    int arr[5] = {1, 2, 3, 4, 5};
    auto arr_copy = arr;  // int* (数组退化为指针)
    
    std::cout << "  int arr[5] = {1, 2, 3, 4, 5};\n";
    std::cout << "  auto arr_copy = arr;  → int* (数组退化为指针)\n";
    std::cout << "  使用 auto& arr_ref = arr; 保留数组类型\n";
}

// ============================================================================

void type_deduction_review() {
    auto_deduction_demo();
    decltype_demo();
    template_type_deduction_demo();
    practical_usage_demo();
    common_pitfalls_demo();
    
    std::cout << "\n✅ 类型推导复习完成!\n";
}
