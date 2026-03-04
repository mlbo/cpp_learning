/**
 * @file reference_collapsing_demo.cpp
 * @brief Day 24: 引用折叠 (Reference Collapsing) 演示
 * 
 * 引用折叠是 C++ 类型系统的重要规则，用于处理"引用的引用"情况。
 * 
 * 四大规则：
 * 1. T& &   → T&   (左值引用 + 左值引用 = 左值引用)
 * 2. T& &&  → T&   (左值引用 + 右值引用 = 左值引用)
 * 3. T&& &  → T&   (右值引用 + 左值引用 = 左值引用)
 * 4. T&& && → T&&  (右值引用 + 右值引用 = 右值引用)
 * 
 * 记忆口诀：有左值引用参与 → 折叠为左值引用；全右值引用 → 保持右值引用
 */

#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

// ==================== 辅助函数 ====================

/**
 * @brief 打印类型特性的辅助函数
 */
template<typename T>
void printTypeTraits(const std::string& typeName) {
    std::cout << "  " << typeName << ":\n";
    std::cout << "    - is_lvalue_reference: " << std::is_lvalue_reference_v<T> << "\n";
    std::cout << "    - is_rvalue_reference: " << std::is_rvalue_reference_v<T> << "\n";
    std::cout << "    - is_reference: " << std::is_reference_v<T> << "\n";
    
    if constexpr (std::is_lvalue_reference_v<T>) {
        std::cout << "    → 折叠结果: 左值引用 (T&)\n";
    } else if constexpr (std::is_rvalue_reference_v<T>) {
        std::cout << "    → 折叠结果: 右值引用 (T&&)\n";
    } else {
        std::cout << "    → 折叠结果: 非引用类型 (T)\n";
    }
}

// ==================== 模板实例化中的引用折叠 ====================

/**
 * @brief 演示模板实例化时的引用折叠
 * 
 * 当模板参数 T 本身是引用类型时，T&& 会触发引用折叠
 */
template<typename T>
void templateCollapseDemo(T&& arg) {
    std::cout << "\n--- 模板实例化中的引用折叠 ---\n";
    
    std::cout << "传入参数后:\n";
    printTypeTraits<T>("模板参数 T");
    printTypeTraits<T&&>("参数类型 T&&");
}

// ==================== using 别名中的引用折叠 ====================

/**
 * @brief 演示 using/typedef 别名声明中的引用折叠
 * 
 * 当使用别名时，如果别名本身是引用类型，再添加 & 会触发折叠
 */
void aliasCollapseDemo() {
    std::cout << "\n=== using 别名中的引用折叠 ===\n";
    
    // 定义基础类型
    using Int = int;           // 非引用类型
    using IntLRef = int&;      // 左值引用
    using IntRRef = int&&;     // 右值引用
    
    std::cout << "\n基础类型:\n";
    printTypeTraits<Int>("int");
    printTypeTraits<IntLRef>("int&");
    printTypeTraits<IntRRef>("int&&");
    
    // 在别名基础上添加引用，观察折叠结果
    std::cout << "\n在别名基础上添加 &:\n";
    
    using Int_And = Int&;          // int & → int&
    using IntLRef_And = IntLRef&;  // int& & → int&
    using IntRRef_And = IntRRef&;  // int&& & → int&
    
    printTypeTraits<Int_And>("Int&");
    printTypeTraits<IntLRef_And>("IntLRef& (int& &)");
    printTypeTraits<IntRRef_And>("IntRRef& (int&& &)");
    
    std::cout << "\n在别名基础上添加 &&:\n";
    
    using Int_AndAnd = Int&&;          // int && → int&&
    using IntLRef_AndAnd = IntLRef&&;  // int& && → int&
    using IntRRef_AndAnd = IntRRef&&;  // int&& && → int&&
    
    printTypeTraits<Int_AndAnd>("Int&&");
    printTypeTraits<IntLRef_AndAnd>("IntLRef&& (int& &&)");
    printTypeTraits<IntRRef_AndAnd>("IntRRef&& (int&& &&)");
}

// ==================== auto 类型推导中的引用折叠 ====================

/**
 * @brief 演示 auto 类型推导中的引用折叠
 * 
 * auto&& 是通用引用，会发生引用折叠
 */
void autoCollapseDemo() {
    std::cout << "\n=== auto 类型推导中的引用折叠 ===\n";
    
    int x = 42;
    int& lr = x;
    int&& rr = 100;
    
    std::cout << "\n原始变量:\n";
    printTypeTraits<decltype(x)>("x (int)");
    printTypeTraits<decltype(lr)>("lr (int&)");
    printTypeTraits<decltype(rr)>("rr (int&&)");
    
    std::cout << "\nauto&& 绑定到不同值类别:\n";
    
    auto&& ref1 = x;          // int& && → int&
    auto&& ref2 = lr;         // int& && → int&
    auto&& ref3 = 100;        // int && → int&&
    auto&& ref4 = std::move(x); // int && → int&&
    
    printTypeTraits<decltype(ref1)>("auto&& ref1 = x");
    printTypeTraits<decltype(ref2)>("auto&& ref2 = lr");
    printTypeTraits<decltype(ref3)>("auto&& ref3 = 100");
    printTypeTraits<decltype(ref4)>("auto&& ref4 = std::move(x)");
}

// ==================== decltype 中的引用折叠 ====================

/**
 * @brief 演示 decltype 中的引用折叠
 * 
 * decltype 可能产生引用类型，再添加 & 会触发折叠
 */
void decltypeCollapseDemo() {
    std::cout << "\n=== decltype 中的引用折叠 ===\n";
    
    int x = 42;
    int& lr = x;
    
    std::cout << "\ndecltype 推导:\n";
    
    // decltype(x) 是 int
    // decltype((x)) 是 int&（带括号的左值表达式）
    printTypeTraits<decltype(x)>("decltype(x)");
    printTypeTraits<decltype((x))>("decltype((x))");
    
    // 在 decltype 结果上添加 &
    using Type1 = decltype(x)&;       // int & → int&
    using Type2 = decltype(lr)&;      // int& & → int&
    using Type3 = decltype((x))&;     // int& & → int&
    
    std::cout << "\n在 decltype 结果上添加 &:\n";
    printTypeTraits<Type1>("decltype(x)&");
    printTypeTraits<Type2>("decltype(lr)&");
    printTypeTraits<Type3>("decltype((x))&");
}

// ==================== 实际应用：完美转发基础 ====================

/**
 * @brief 模拟 std::forward 的简化实现
 * 
 * std::forward 利用引用折叠实现完美转发
 */
namespace my {
    // 完美转发左值
    template<typename T>
    constexpr T&& forward(typename std::remove_reference<T>::type& t) noexcept {
        return static_cast<T&&>(t);
    }
    
    // 完美转发右值
    template<typename T>
    constexpr T&& forward(typename std::remove_reference<T>::type&& t) noexcept {
        static_assert(!std::is_lvalue_reference<T>::value,
                      "Cannot forward an rvalue as an lvalue");
        return static_cast<T&&>(t);
    }
}

/**
 * @brief 目标函数，演示不同的调用方式
 */
void processValue(int& x) {
    std::cout << "  处理左值引用: " << x << "\n";
}

void processValue(int&& x) {
    std::cout << "  处理右值引用: " << x << "\n";
}

/**
 * @brief 完美转发中间层
 */
template<typename T>
void forwarder(T&& arg) {
    std::cout << "\n--- 完美转发演示 ---\n";
    std::cout << "转发前:\n";
    printTypeTraits<T>("T");
    
    std::cout << "转发后:\n";
    processValue(std::forward<T>(arg));  // 或 my::forward<T>(arg)
}

// ==================== 引用折叠规则总结 ====================

void printFoldingRules() {
    std::cout << "\n========================================\n";
    std::cout << "   引用折叠规则总结\n";
    std::cout << "========================================\n";
    
    std::cout << "\n四大规则（记住：有左值引用 → 折叠为左值引用）:\n\n";
    
    std::cout << "  1. T& &   → T&   (左 + 左 → 左)\n";
    std::cout << "  2. T& &&  → T&   (左 + 右 → 左)\n";
    std::cout << "  3. T&& &  → T&   (右 + 左 → 左)\n";
    std::cout << "  4. T&& && → T&&  (右 + 右 → 右)\n";
    
    std::cout << "\n触发上下文:\n";
    std::cout << "  1. 模板实例化 (T&& 参数推导)\n";
    std::cout << "  2. auto 类型推导 (auto&&)\n";
    std::cout << "  3. typedef/using 别名声明\n";
    std::cout << "  4. decltype 表达式\n";
    
    std::cout << "\n========================================\n";
}

// ==================== 主演示函数 ====================

void runReferenceCollapsingDemo() {
    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "   Day 24: 引用折叠演示\n";
    std::cout << "========================================\n";
    
    // 打印规则总结
    printFoldingRules();
    
    // 1. 模板实例化中的折叠
    std::cout << "\n=== 1. 模板实例化中的引用折叠 ===\n";
    
    int x = 42;
    int& lr = x;
    
    templateCollapseDemo(x);           // T = int&, T&& = int&
    templateCollapseDemo(lr);          // T = int&, T&& = int&
    templateCollapseDemo(100);         // T = int, T&& = int&&
    templateCollapseDemo(std::move(x)); // T = int, T&& = int&&
    
    // 2. using 别名中的折叠
    aliasCollapseDemo();
    
    // 3. auto 类型推导中的折叠
    autoCollapseDemo();
    
    // 4. decltype 中的折叠
    decltypeCollapseDemo();
    
    // 5. 完美转发应用
    std::cout << "\n=== 完美转发应用 ===\n";
    
    int value = 100;
    forwarder(value);          // 左值被转发为左值
    forwarder(200);            // 右值被转发为右值
    forwarder(std::move(value)); // std::move 结果被转发为右值
    
    std::cout << "\n========================================\n";
    std::cout << "   引用折叠演示完成\n";
    std::cout << "========================================\n";
}
