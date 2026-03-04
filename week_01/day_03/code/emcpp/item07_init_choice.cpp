/**
 * @file item07_init_choice.cpp
 * @brief Effective Modern C++ 条款 7：区分 () 和 {} 初始化
 *
 * 本文件完整展示条款 7 的核心内容：
 * 1. 为什么需要区分两种初始化方式
 * 2. 各种初始化方式的适用场景
 * 3. 构造函数重载决议的影响
 * 4. 最佳实践指南
 */

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <chrono>
#include <complex>
#include <mutex>

namespace emcpp {

// ============================================================
// 一、C++ 初始化方式的多样性问题
// ============================================================

/**
 * @brief 展示 C++ 初始化方式的混乱历史
 */
void demo_initialization_diversity() {
    std::cout << "\n=== 条款 7：初始化方式的多样性问题 ===\n";

    int x1 = 0;      // 拷贝初始化
    int x2(0);       // 直接初始化
    int x3 = {0};    // 拷贝列表初始化
    int x4{0};       // 直接列表初始化

    std::cout << "同一件事，四种写法：\n";
    std::cout << "  int x1 = 0;   // 拷贝初始化\n";
    std::cout << "  int x2(0);    // 直接初始化\n";
    std::cout << "  int x3 = {0}; // 拷贝列表初始化\n";
    std::cout << "  int x4{0};    // 直接列表初始化\n";
    std::cout << "\nC++11 统一初始化的目标：用 {} 统一所有初始化\n";
}

// ============================================================
// 二、花括号初始化的优势
// ============================================================

// 优势 1：可用于任何地方
class AnywhereInit {
public:
    AnywhereInit(int x, double y) : x_(x), y_(y) {}

    void print() const {
        std::cout << "AnywhereInit(" << x_ << ", " << y_ << ")\n";
    }

private:
    int x_;
    double y_;
};

void demo_anywhere_init() {
    std::cout << "\n=== 优势 1：可用于任何地方 ===\n";

    // 1. 非静态成员初始化
    struct Widget {
        int x{0};        // OK
        int y = 0;       // OK
        // int z(0);     // 错误！
    };
    std::cout << "类内成员初始化：{} 和 = 支持，() 不支持\n";

    // 2. 不可拷贝对象
    std::atomic<int> ai1{0};     // OK
    // std::atomic<int> ai2 = 0; // 错误！
    std::cout << "不可拷贝对象：{} 支持，= 不支持\n";

    // 3. 括号初始化无法表达的场景
    std::cout << "动态数组初始化：new int[5]{1,2,3,4,5}（C++11）\n";
}

// 优势 2：防止窄化转换
void demo_narrowing_prevention() {
    std::cout << "\n=== 优势 2：防止窄化转换 ===\n";

    double d = 1.5;
    int i1 = d;     // OK，但丢失数据
    int i2(d);      // OK，但丢失数据

    // int i3{d};    // 错误！窄化转换
    // int i4 = {d}; // 错误！

    std::cout << "圆括号允许窄化: double " << d << " -> int " << i1 << "\n";
    std::cout << "花括号禁止窄化: 编译时错误（保护性）\n";
}

// 优势 3：避免 Most Vexing Parse
void demo_vexing_parse() {
    std::cout << "\n=== 优势 3：避免 Most Vexing Parse ===\n";

    class Widget {
    public:
        Widget() { std::cout << "Widget 默认构造\n"; }
        Widget(int i) { std::cout << "Widget(" << i << ")\n"; }
    };

    // Widget w1();    // 函数声明！
    Widget w2{};      // 明确是对象初始化
    Widget w3();      // 这不是默认构造！

    std::cout << "w3 声明的是函数，不是对象！\n";
    std::cout << "使用 {} 可以避免这种歧义\n";
}

// ============================================================
// 三、花括号初始化的陷阱
// ============================================================

// 陷阱类定义
class TrapClass {
public:
    // 构造函数 1：普通构造
    TrapClass(int a, int b) {
        std::cout << "  调用 (int, int) 构造: " << a << ", " << b << "\n";
    }

    // 构造函数 2：initializer_list 构造
    TrapClass(std::initializer_list<long double> list) {
        std::cout << "  调用 initializer_list 构造，元素数: " << list.size() << "\n";
        std::cout << "  元素: ";
        for (auto x : list) std::cout << x << " ";
        std::cout << "\n";
    }

    // 隐式转换操作符
    operator float() const {
        std::cout << "  调用 operator float()\n";
        return 0.0f;
    }
};

void demo_initializer_list_trap() {
    std::cout << "\n=== 陷阱：initializer_list 构造函数优先 ===\n";

    std::cout << "场景 1：两个 int 参数\n";
    std::cout << "  TrapClass tc(10, 20):  ";
    TrapClass tc1(10, 20);  // (int, int) 构造

    std::cout << "  TrapClass tc{10, 20}:  ";
    TrapClass tc2{10, 20};  // initializer_list 构造！

    std::cout << "\n场景 2：双精度浮点\n";
    std::cout << "  TrapClass tc(10, 5.0): ";
    TrapClass tc3(10, 5.0);  // (int, int) 构造（5.0 隐式转换）

    std::cout << "  TrapClass tc{10, 5.0}: ";
    TrapClass tc4{10, 5.0};  // initializer_list 构造！
}

// 空花括号的特殊情况
class EmptyBraceTest {
public:
    EmptyBraceTest() {
        std::cout << "  调用默认构造\n";
    }

    EmptyBraceTest(std::initializer_list<int> list) {
        std::cout << "  调用 initializer_list 构造，size: " << list.size() << "\n";
    }
};

void demo_empty_brace() {
    std::cout << "\n=== 空花括号行为 ===\n";

    std::cout << "EmptyBraceTest e1{}: ";
    EmptyBraceTest e1{};  // 默认构造，不是空的 initializer_list！

    std::cout << "EmptyBraceTest e2{{}}: ";
    EmptyBraceTest e2{{}};  // 空的 initializer_list

    std::cout << "EmptyBraceTest e3({}): ";
    EmptyBraceTest e3({});  // 空的 initializer_list（显式）
}

// 窄化转换导致无法调用 initializer_list
class NarrowingTrap {
public:
    NarrowingTrap(int a, int b, double c) {
        std::cout << "  调用 (int, int, double) 构造\n";
    }

    NarrowingTrap(std::initializer_list<bool> list) {
        std::cout << "  调用 initializer_list<bool> 构造\n";
    }
};

void demo_narrowing_trap() {
    std::cout << "\n=== 窄化转换阻止 initializer_list 调用 ===\n";

    std::cout << "NarrowingTrap n1(10, 20, 3.14): ";
    NarrowingTrap n1(10, 20, 3.14);  // OK

    std::cout << "NarrowingTrap n2{10, 20, 3.14}: ";
    // NarrowingTrap n2{10, 20, 3.14};  // 错误！尝试转换为 bool
    std::cout << "  (注释掉，因为会编译错误)\n";
    std::cout << "  原因：double -> bool 是窄化转换\n";
}

// ============================================================
// 四、vector 的经典案例
// ============================================================

void demo_vector_case() {
    std::cout << "\n=== vector 的经典案例 ===\n";

    std::vector<int> v1(10, 20);  // 10 个元素，每个都是 20
    std::vector<int> v2{10, 20};  // 2 个元素：10 和 20

    std::cout << "vector<int> v1(10, 20):\n";
    std::cout << "  含义：10 个元素，每个都是 20\n";
    std::cout << "  实际大小: " << v1.size() << "\n";

    std::cout << "vector<int> v2{10, 20}:\n";
    std::cout << "  含义：元素为 10 和 20\n";
    std::cout << "  实际大小: " << v2.size() << "\n";
    std::cout << "  元素: ";
    for (int x : v2) std::cout << x << " ";
    std::cout << "\n";
}

// ============================================================
// 五、模板中的初始化
// ============================================================

template<typename T, typename... Args>
void create_object(Args&&... args) {
    // 问题：应该使用 () 还是 {}？
    // T obj(std::forward<Args>(args)...);  // 方式 1
    // T obj{std::forward<Args>(args)...};  // 方式 2

    // 条款 30 建议使用 std::make_unique 和 std::make_shared
    // C++11写法（注释）: std::unique_ptr<T> ptr(new T(std::forward<Args>(args)...));
    auto ptr = std::make_unique<T>(std::forward<Args>(args)...);
    std::cout << "创建了对象\n";
}

void demo_template_init() {
    std::cout << "\n=== 模板中的初始化选择 ===\n";

    std::cout << "在模板中，无法确定 T 是否有 initializer_list 构造\n";
    std::cout << "建议：\n";
    std::cout << "  1. 使用 std::make_unique/make_shared\n";
    std::cout << "  2. 如果需要 {} 语义，让调用者显式传入 initializer_list\n";
}

// ============================================================
// 六、最佳实践总结
// ============================================================

void print_best_practices() {
    std::cout << "\n=== 条款 7 最佳实践 ===\n";

    std::cout << "\n【推荐使用花括号 {} 初始化的场景】\n";
    std::cout << "  1. 局部变量初始化\n";
    std::cout << "  2. 类成员初始化（类内）\n";
    std::cout << "  3. 容器初始化\n";
    std::cout << "  4. 需要防止窄化转换时\n";
    std::cout << "  5. 无参对象初始化（避免歧义）\n";

    std::cout << "\n【考虑使用圆括号 () 初始化的场景】\n";
    std::cout << "  1. 类有 initializer_list 构造，但你想调用其他构造函数\n";
    std::cout << "  2. vector(size, value) 语义\n";
    std::cout << "  3. 需要避免 initializer_list 陷阱时\n";

    std::cout << "\n【关键原则】\n";
    std::cout << "  1. 选择一种风格并保持一致\n";
    std::cout << "  2. 了解 initializer_list 的优先级规则\n";
    std::cout << "  3. 在设计 API 时，避免 initializer_list 造成歧义\n";
}

// ============================================================
// 主演示函数
// ============================================================

void run_item07_demo() {
    std::cout << "\n╔══════════════════════════════════════════════════════════╗\n";
    std::cout << "║   EMC++ 条款 7：区分 () 和 {} 初始化                     ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════╝\n";

    demo_initialization_diversity();
    demo_anywhere_init();
    demo_narrowing_prevention();
    demo_vexing_parse();
    demo_initializer_list_trap();
    demo_empty_brace();
    demo_narrowing_trap();
    demo_vector_case();
    demo_template_init();
    print_best_practices();

    std::cout << "\n=== 条款 7 核心要点 ===\n";
    std::cout << "花括号初始化是最广泛适用的初始化语法，\n";
    std::cout << "它禁止窄化转换，并避免 Most Vexing Parse。\n";
    std::cout << "但在构造函数重载时，花括号会优先匹配\n";
    std::cout << "initializer_list 版本，这可能导致意外行为。\n";
}

} // namespace emcpp
