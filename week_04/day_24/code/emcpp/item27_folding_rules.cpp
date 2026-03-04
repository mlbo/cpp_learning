/**
 * @file item27_folding_rules.cpp
 * @brief EMC++ Item 27: 理解引用折叠规则
 * 
 * 引用折叠是实现完美转发的技术基础。
 * 
 * 核心规则：
 * - 如果任一引用是左值引用，结果是左值引用
 * - 只有全部是右值引用，结果才是右值引用
 */

#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include <memory>

// ==================== 辅助函数 ====================

template<typename T>
void printType(const std::string& label) {
    if constexpr (std::is_lvalue_reference_v<T>) {
        std::cout << label << " = 左值引用\n";
    } else if constexpr (std::is_rvalue_reference_v<T>) {
        std::cout << label << " = 右值引用\n";
    } else {
        std::cout << label << " = 非引用类型\n";
    }
}

// ==================== 四种折叠情况演示 ====================

void demonstrateFourRules() {
    std::cout << "\n=== 四种引用折叠规则 ===\n\n";
    
    // 定义基础类型
    using T = int;
    using TRef = int&;
    using TRRef = int&&;
    
    std::cout << "基础类型:\n";
    printType<T>("T");
    printType<TRef>("T&");
    printType<TRRef>("T&&");
    
    std::cout << "\n规则 1: T& & → T&\n";
    using Rule1 = TRef&;  // int& &
    printType<Rule1>("int& &");
    std::cout << "  左值引用 + 左值引用 = 左值引用\n";
    
    std::cout << "\n规则 2: T& && → T&\n";
    using Rule2 = TRef&&;  // int& &&
    printType<Rule2>("int& &&");
    std::cout << "  左值引用 + 右值引用 = 左值引用\n";
    
    std::cout << "\n规则 3: T&& & → T&\n";
    using Rule3 = TRRef&;  // int&& &
    printType<Rule3>("int&& &");
    std::cout << "  右值引用 + 左值引用 = 左值引用\n";
    
    std::cout << "\n规则 4: T&& && → T&&\n";
    using Rule4 = TRRef&&;  // int&& &&
    printType<Rule4>("int&& &&");
    std::cout << "  右值引用 + 右值引用 = 右值引用\n";
    
    std::cout << "\n记忆口诀:\n";
    std::cout << "  \"有左值引用参与 → 折叠为左值引用\"\n";
    std::cout << "  \"全右值引用 → 保持右值引用\"\n";
}

// ==================== 模板实例化中的折叠 ====================

template<typename T>
void analyzeTemplateArg(T&& arg) {
    std::cout << "\n--- 模板参数分析 ---\n";
    
    printType<T>("T");
    printType<T&&>("T&& (参数类型)");
    
    // 展示推导过程
    if constexpr (std::is_lvalue_reference_v<T>) {
        std::cout << "\n推导过程:\n";
        std::cout << "  传入左值 → T 推导为 Type&\n";
        std::cout << "  T&& = Type& && → Type& (规则 2)\n";
    } else {
        std::cout << "\n推导过程:\n";
        std::cout << "  传入右值 → T 推导为 Type\n";
        std::cout << "  T&& = Type && → Type&& (规则 4)\n";
    }
}

void demonstrateTemplateCollapse() {
    std::cout << "\n=== 模板实例化中的引用折叠 ===\n";
    
    int x = 42;
    int& lr = x;
    
    std::cout << "\n传入左值 (int x):\n";
    analyzeTemplateArg(x);
    
    std::cout << "\n传入左值引用 (int& lr):\n";
    analyzeTemplateArg(lr);
    
    std::cout << "\n传入右值 (42):\n";
    analyzeTemplateArg(42);
    
    std::cout << "\n传入 std::move 结果:\n";
    analyzeTemplateArg(std::move(x));
}

// ==================== auto 类型推导中的折叠 ====================

void demonstrateAutoCollapse() {
    std::cout << "\n=== auto 类型推导中的引用折叠 ===\n";
    
    int x = 10;
    int& lr = x;
    
    std::cout << "\nauto&& 推导规则与 T&& 相同:\n";
    
    auto&& r1 = x;          // x 是左值
    auto&& r2 = lr;         // lr 是左值引用
    auto&& r3 = 20;         // 20 是右值
    auto&& r4 = std::move(x); // std::move 产生右值
    
    std::cout << "\nauto&& r1 = x;\n";
    printType<decltype(r1)>("decltype(r1)");
    
    std::cout << "\nauto&& r2 = lr;\n";
    printType<decltype(r2)>("decltype(r2)");
    
    std::cout << "\nauto&& r3 = 20;\n";
    printType<decltype(r3)>("decltype(r3)");
    
    std::cout << "\nauto&& r4 = std::move(x);\n";
    printType<decltype(r4)>("decltype(r4)");
}

// ==================== 实际应用：理解 std::forward ====================

namespace my {

/**
 * @brief std::forward 的简化实现
 * 
 * 关键：利用引用折叠实现完美转发
 * 
 * 当 T = Type& 时：
 *   static_cast<Type& &&>(t) → Type& (规则 2)
 * 
 * 当 T = Type 时：
 *   static_cast<Type&&>(t) → Type&& (规则 4)
 */
template<typename T>
T&& forward(typename std::remove_reference<T>::type& t) noexcept {
    return static_cast<T&&>(t);
}

template<typename T>
T&& forward(typename std::remove_reference<T>::type&& t) noexcept {
    static_assert(!std::is_lvalue_reference<T>::value,
                  "Cannot forward an rvalue as an lvalue");
    return static_cast<T&&>(t);
}

} // namespace my

void process(int& x) {
    std::cout << "  处理左值: " << x << "\n";
}

void process(int&& x) {
    std::cout << "  处理右值: " << x << "\n";
}

template<typename T>
void perfectForwarder(T&& arg) {
    std::cout << "\n转发参数...\n";
    std::cout << "T 的类型: ";
    printType<T>("");
    
    // 使用 std::forward 或自定义的 my::forward
    process(std::forward<T>(arg));
}

void demonstrateForward() {
    std::cout << "\n=== std::forward 与引用折叠 ===\n";
    
    int value = 100;
    
    std::cout << "\n转发左值:\n";
    perfectForwarder(value);
    
    std::cout << "\n转发右值:\n";
    perfectForwarder(200);
    
    std::cout << "\nstd::forward 原理:\n";
    std::cout << "  forward<T>(arg) 实际上是 static_cast<T&&>(arg)\n";
    std::cout << "  当 T = int& 时:  static_cast<int& &&> → int&\n";
    std::cout << "  当 T = int 时:   static_cast<int&&>  → int&&\n";
}

// ==================== 实用技巧 ====================

/**
 * @brief 工厂函数：使用通用引用和完美转发
 */
template<typename T, typename... Args>
std::unique_ptr<T> makeUnique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

class Widget {
public:
    Widget(int id, std::string name) 
        : id_(id), name_(std::move(name)) {
        std::cout << "  Widget 构造: " << name_ << " (id=" << id_ << ")\n";
    }
    
    void print() const {
        std::cout << "  Widget[" << name_ << ", id=" << id_ << "]\n";
    }
    
private:
    int id_;
    std::string name_;
};

void demonstrateFactory() {
    std::cout << "\n=== 工厂函数与完美转发 ===\n";
    
    // 传入左值
    std::string name = "MyWidget";
    auto w1 = makeUnique<Widget>(1, name);
    w1->print();
    
    // 传入右值
    auto w2 = makeUnique<Widget>(2, std::string("TempWidget"));
    w2->print();
    
    // 混合参数
    auto w3 = makeUnique<Widget>(3, "LiteralWidget");
    w3->print();
}

// ==================== 主演示函数 ====================

void runItem27Demo() {
    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "   EMC++ Item 27: 理解引用折叠规则\n";
    std::cout << "========================================\n";
    
    // 四种规则
    demonstrateFourRules();
    
    // 模板实例化
    demonstrateTemplateCollapse();
    
    // auto 推导
    demonstrateAutoCollapse();
    
    // std::forward 原理
    demonstrateForward();
    
    // 工厂函数应用
    demonstrateFactory();
    
    std::cout << "\n========================================\n";
    std::cout << "   Item 27 演示完成\n";
    std::cout << "========================================\n";
    
    std::cout << "\n关键要点:\n";
    std::cout << "  1. 引用折叠只产生 T& 或 T&& 两种结果\n";
    std::cout << "  2. 有左值引用参与 → 结果是左值引用\n";
    std::cout << "  3. std::forward 利用引用折叠实现完美转发\n";
    std::cout << "  4. auto&& 也是通用引用，遵循相同规则\n";
}
