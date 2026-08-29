/**
 * @file init_comparison.cpp
 * @brief () 与 {} 初始化的完整对比
 *
 * 本文件全面对比两种初始化方式：
 * 1. 语法差异
 * 2. 语义差异
 * 3. 重载决议差异
 * 4. 使用场景建议
 */

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <atomic>

namespace cpp11_features {

// ============================================================
// 一、最令人苦恼的解析（Most Vexing Parse）
// ============================================================

class ComparisonWidget {
public:
    ComparisonWidget() { std::cout << "默认构造\n"; }
    ComparisonWidget(int i) { std::cout << "单参数构造: " << i << "\n"; }
    ComparisonWidget(int i, double d) {
        std::cout << "双参数构造: " << i << ", " << d << "\n";
    }
};

void demo_most_vexing_parse() {
    std::cout << "\n=== 最令人苦恼的解析 ===\n";

    // 问题：以下声明被解析为函数！
    // ComparisonWidget w1();  // 这是一个函数声明，返回 ComparisonWidget！

    // 解决方案 1：使用 {}
    ComparisonWidget w2{};    // 明确是对象初始化

    // 解决方案 2：去掉括号（C++11 之前）
    ComparisonWidget w3;      // 默认构造

    std::cout << "w2 和 w3 都是正确初始化的对象\n";

    // 另一个经典的例子
    // std::unique_ptr<int> up1();  // 函数声明！
    std::unique_ptr<int> up2{};    // 正确
    // C++11写法（注释）: std::unique_ptr<int> up3(new int());
    auto up3 = std::make_unique<int>();  // 更推荐
}

// ============================================================
// 二、窄化转换检查对比
// ============================================================

void demo_narrowing_comparison() {
    std::cout << "\n=== 窄化转换检查对比 ===\n";

    double d = 3.14;
    long long ll = 10000000000LL;

    // 圆括号语法本身允许窄化；本课程启用 -Werror，因此用显式转换表达“我知道会丢数据”。
    int i1(static_cast<int>(d));
    int i2(static_cast<int>(ll));

    std::cout << "圆括号 double->int: " << i1 << " (无编译错误)\n";
    std::cout << "圆括号 long long->int: " << i2 << " (显式接受数据丢失)\n";

    // 花括号：禁止窄化转换
    // int i3{d};     // 错误！窄化转换
    // int i4{ll};    // 错误！窄化转换

    // 显式转换后可以使用花括号
    int i5{static_cast<int>(d)};  // OK: 显式告知编译器
    std::cout << "显式转换后花括号初始化: " << i5 << "\n";

    // 运行期 int -> double：圆括号允许；C++17 列表初始化需要显式转换
    int n = 42;
    double d1(n);   // OK
    double d2{static_cast<double>(n)};   // OK

    std::cout << "int->double（列表形式显式转换）: " << d1 << ", " << d2 << "\n";
}

// ============================================================
// 三、类类型初始化对比
// ============================================================

class Container {
public:
    Container() : data_() {
        std::cout << "默认构造\n";
    }

    Container(std::size_t size, int value) : data_(size, value) {
        std::cout << "size-value 构造: size=" << size << ", value=" << value << "\n";
    }

    Container(std::initializer_list<int> list) : data_(list) {
        std::cout << "initializer_list 构造: " << data_.size() << " 个元素\n";
    }

    void print() const {
        std::cout << "内容: ";
        for (int x : data_) std::cout << x << " ";
        std::cout << "\n";
    }

private:
    std::vector<int> data_;
};

void demo_class_initialization() {
    std::cout << "\n=== 类类型初始化对比 ===\n";

    // 场景 1：默认构造
    std::cout << "圆括号默认构造: ";
    // Container c1(); // 这行如果真正写进程序，它是函数声明而不是对象。
    Container c1b;    // 正确
    std::cout << "花括号默认构造: ";
    Container c2{};   // 正确，调用默认构造

    // 场景 2：size-value 构造
    std::cout << "\n圆括号 size-value: ";
    Container c3(5, 10);  // 5 个元素，每个都是 10
    c3.print();

    std::cout << "花括号 size-value: ";
    Container c4{5, 10};  // 调用 initializer_list！2 个元素：5, 10
    c4.print();

    // 场景 3：显式指定
    std::cout << "\n强制调用 size-value（无 initializer_list 时）:\n";
    // 如果只想用圆括号语义，直接使用圆括号
    Container c5(3, 100);
    c5.print();
}

// ============================================================
// 四、陷阱案例详解
// ============================================================

class Trap {
public:
    Trap(int a, int b, int c) {
        std::cout << "三参数构造: " << a << ", " << b << ", " << c << "\n";
    }

    Trap(std::initializer_list<int> list) {
        std::cout << "initializer_list 构造，元素数: " << list.size() << "\n";
    }
};

class Trap2 {
public:
    Trap2(int a, int b) {
        std::cout << "双参数构造: " << a << ", " << b << "\n";
    }

    Trap2(std::initializer_list<bool> list) {
        std::cout << "bool initializer_list 构造，元素数: " << list.size() << "\n";
    }
};

void demo_traps() {
    std::cout << "\n=== 常见陷阱案例 ===\n";

    // 陷阱 1：initializer_list 优先级高于普通构造函数
    std::cout << "陷阱 1：优先级问题\n";
    std::cout << "  圆括号: ";
    Trap t1(1, 2, 3);   // 三参数构造
    std::cout << "  花括号: ";
    Trap t2{1, 2, 3};   // initializer_list 构造！

    // 陷阱 2：类型转换 + 窄化
    std::cout << "\n陷阱 2：类型转换\n";
    std::cout << "  圆括号: ";
    Trap2 t3(10, 5);    // 双参数构造
    // Trap2 t4{10, 5}; // 错误！尝试转换为 bool（窄化）
    std::cout << "  花括号会尝试调用 initializer_list<bool>，导致窄化错误\n";
}

// ============================================================
// 五、vector 的经典对比
// ============================================================

void demo_vector_initialization() {
    std::cout << "\n=== vector 初始化对比 ===\n";

    // 最经典的对比
    std::vector<int> v1{10, 20};  // 2 个元素：10, 20
    std::vector<int> v2(10, 20);  // 10 个元素，每个都是 20

    std::cout << "v1{10, 20}: 大小=" << v1.size() << "\n";
    std::cout << "  内容: ";
    for (int x : v1) std::cout << x << " ";
    std::cout << "\n";

    std::cout << "v2(10, 20): 大小=" << v2.size() << "\n";
    std::cout << "  内容（前5个）: ";
    for (std::size_t i = 0; i < 5; ++i) std::cout << v2[i] << " ";
    std::cout << "...\n";

    // 单参数
    std::vector<int> v3{5};   // 1 个元素：5
    std::vector<int> v4(5);   // 5 个元素，每个都是 0

    std::cout << "v3{5}: 大小=" << v3.size() << ", 元素=" << v3[0] << "\n";
    std::cout << "v4(5): 大小=" << v4.size() << ", 元素全为0\n";
}

// ============================================================
// 六、不可拷贝类型与 C++17 拷贝消除
// ============================================================

void demo_non_copyable() {
    std::cout << "\n=== 不可拷贝类型与 C++17 拷贝消除 ===\n";

    // atomic：不可拷贝
    std::atomic<int> a1{0};      // OK：花括号初始化
    std::atomic<int> a2 = 0;     // C++17：保证拷贝消除，可直接构造目标对象

    // 使用圆括号也可以
    std::atomic<int> a3(0);      // OK

    std::cout << "atomic 不可拷贝，但 C++17 的 = 初始化可依靠保证的拷贝消除\n";
    std::cout << "atomic 值: " << a1.load() << ", " << a2.load()
              << ", " << a3.load() << "\n";
}

// ============================================================
// 七、决策建议表
// ============================================================

void print_recommendation_table() {
    std::cout << "\n=== 初始化方式选择建议 ===\n";
    std::cout << "+--------------------------------+----------+----------+\n";
    std::cout << "| 场景                           | 推荐方式 | 原因     |\n";
    std::cout << "+--------------------------------+----------+----------+\n";
    std::cout << "| 变量初始化                     | {}       | 统一风格 |\n";
    std::cout << "| 类内成员初始化                 | {} 或 =  | () 不支持|\n";
    std::cout << "| 无参构造                       | {}       | 避免歧义 |\n";
    std::cout << "| 容器初始化                     | {}       | 直观     |\n";
    std::cout << "| 数值初始化                     | {}       | 防窄化   |\n";
    std::cout << "| vector(size, value)            | ()       | 避免歧义 |\n";
    std::cout << "| 有 initializer_list 构造的类   | 看情况   | 注意陷阱 |\n";
    std::cout << "+--------------------------------+----------+----------+\n";
}

// ============================================================
// 八、综合测试函数
// ============================================================

void run_init_comparison_demo() {
    std::cout << "\n╔══════════════════════════════════════════════════════════╗\n";
    std::cout << "║        () 与 {} 初始化完整对比                            ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════╝\n";

    demo_most_vexing_parse();
    demo_narrowing_comparison();
    demo_class_initialization();
    demo_traps();
    demo_vector_initialization();
    demo_non_copyable();
    print_recommendation_table();

    std::cout << "\n=== 总结 ===\n";
    std::cout << "花括号 {} 优点:\n";
    std::cout << "  1. 统一语法，适用范围广\n";
    std::cout << "  2. 防止窄化转换\n";
    std::cout << "  3. 避免 Most Vexing Parse\n";
    std::cout << "  4. 支持类内成员初始化\n";
    std::cout << "\n";
    std::cout << "圆括号 () 适用场景:\n";
    std::cout << "  1. vector(size, value) 语义\n";
    std::cout << "  2. 避免 initializer_list 陷阱\n";
    std::cout << "  3. 传统代码风格一致性\n";
}

} // namespace cpp11_features
