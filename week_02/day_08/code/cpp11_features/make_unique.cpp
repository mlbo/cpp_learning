/**
 * @file make_unique.cpp
 * @brief make_unique用法详解
 * @author C++ Tutorial
 * @date Day 8
 * 
 * 本文件演示make_unique的各种用法：
 * 1. 基本创建
 * 2. 数组创建
 * 3. 与new的对比
 * 4. 异常安全性
 */

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// 辅助打印
void printSeparator(const std::string& title = "") {
    std::cout << "\n======== " << title << " ========\n";
}

// 测试类
class TestClass {
public:
    TestClass(int a, double b, const std::string& c)
        : a_(a), b_(b), c_(c) {
        std::cout << "TestClass构造: a=" << a_ << ", b=" << b_ << ", c=" << c_ << "\n";
    }
    
    ~TestClass() {
        std::cout << "TestClass析构\n";
    }
    
    void print() const {
        std::cout << "TestClass: a=" << a_ << ", b=" << b_ << ", c=" << c_ << "\n";
    }
    
private:
    int a_;
    double b_;
    std::string c_;
};

// ============================================================
// 1. 基本用法
// ============================================================

void demoBasicUsage() {
    printSeparator("make_unique基本用法");
    
    // 创建单个对象
    std::cout << "\n【创建单个对象】\n";
    auto p1 = std::make_unique<int>(42);
    std::cout << "值: " << *p1 << "\n";
    
    // 创建复杂对象
    std::cout << "\n【创建复杂对象】\n";
    auto p2 = std::make_unique<TestClass>(10, 3.14, "hello");
    p2->print();
    
    // 创建自定义类型
    std::cout << "\n【创建自定义类型】\n";
    struct Point { int x, y; };
    auto p3 = std::make_unique<Point>(Point{100, 200});
    std::cout << "Point: (" << p3->x << ", " << p3->y << ")\n";
}

// ============================================================
// 2. 数组创建
// ============================================================

void demoArrayCreation() {
    printSeparator("make_unique数组创建");
    
    // 创建数组（不初始化）
    std::cout << "\n【创建数组（值初始化）】\n";
    auto arr1 = std::make_unique<int[]>(5);
    for (int i = 0; i < 5; ++i) {
        arr1[i] = i * 10;
    }
    std::cout << "数组内容: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << arr1[i] << " ";
    }
    std::cout << "\n";
    
    // 创建数组（指定大小，零初始化）
    std::cout << "\n【创建数组（零初始化）】\n";
    auto arr2 = std::make_unique<double[]>(10);
    std::cout << "零初始化: arr2[0]=" << arr2[0] << ", arr2[5]=" << arr2[5] << "\n";
    
    // 注意：make_unique不支持初始化列表
    std::cout << "\n【限制：不支持初始化列表】\n";
    std::cout << "auto arr = std::make_unique<int[]>(5, {1,2,3,4,5});  // 错误！\n";
    std::cout << "替代方案:\n";
    std::cout << "  1. 创建后逐个赋值\n";
    std::cout << "  2. 使用vector代替\n";
}

// ============================================================
// 3. 与new对比
// ============================================================

void demoCompareWithNew() {
    printSeparator("make_unique vs new");
    
    // 使用new
    std::cout << "\n【使用 new】\n";
    std::unique_ptr<int> p1(new int(42));
    std::cout << "p1 = " << *p1 << "\n";
    
    // 使用make_unique
    std::cout << "\n【使用 make_unique】\n";
    auto p2 = std::make_unique<int>(42);
    std::cout << "p2 = " << *p2 << "\n";
    
    // 对比表格
    std::cout << "\n┌───────────────────────────────────────────────────┐\n";
    std::cout << "│ 对比项       │ new                │ make_unique  │\n";
    std::cout << "├───────────────────────────────────────────────────┤\n";
    std::cout << "│ 代码简洁性   │ 较冗长             │ 简洁         │\n";
    std::cout << "│ 类型重复     │ 需要写两次类型     │ 类型推导     │\n";
    std::cout << "│ 异常安全     │ 可能有泄漏风险     │ 安全         │\n";
    std::cout << "│ 自定义删除器 │ 支持               │ 不直接支持   │\n";
    std::cout << "│ 私有构造函数 │ 可用               │ 需要变通     │\n";
    std::cout << "└───────────────────────────────────────────────────┘\n";
}

// ============================================================
// 4. 异常安全详解
// ============================================================

// 模拟一个函数
void processWidget(const std::unique_ptr<int>& widget, int value) {
    // 使用widget...
    std::cout << "处理 widget，值=" << *widget << ", 参数=" << value << "\n";
}

int getValue() {
    return 42;
}

std::unique_ptr<int> createWidget() {
    return std::make_unique<int>(100);
}

void demoExceptionSafety() {
    printSeparator("异常安全性");
    
    std::cout << "\n【危险写法】\n";
    std::cout << "processWidget(std::unique_ptr<int>(new int(10)), getValue());\n";
    std::cout << "问题：new和unique_ptr构造之间的顺序不确定\n";
    std::cout << "如果getValue()抛出异常，可能造成内存泄漏\n";
    
    std::cout << "\n【安全写法】\n";
    std::cout << "processWidget(std::make_unique<int>(10), getValue());\n";
    std::cout << "make_unique保证了内存分配和智能指针构造的原子性\n";
    
    // 实际调用演示
    processWidget(std::make_unique<int>(10), getValue());
    
    std::cout << "\n【更安全的写法】先创建，再传递\n";
    auto widget = std::make_unique<int>(20);
    processWidget(widget, getValue());
}

// ============================================================
// 5. 限制与变通
// ============================================================

class PrivateConstructor {
public:
    static std::unique_ptr<PrivateConstructor> create(int value) {
        // 友元方式或辅助函数
        return std::unique_ptr<PrivateConstructor>(new PrivateConstructor(value));
    }
    
    int value() const { return value_; }
    
private:
    explicit PrivateConstructor(int v) : value_(v) {
        std::cout << "PrivateConstructor创建: " << value_ << "\n";
    }
    
    int value_;
};

void demoLimitations() {
    printSeparator("限制与变通");
    
    // 限制1：私有构造函数
    std::cout << "\n【限制1：私有构造函数】\n";
    std::cout << "make_unique需要公有的构造函数\n";
    std::cout << "变通：使用静态工厂函数\n";
    auto obj = PrivateConstructor::create(42);
    std::cout << "创建成功: value=" << obj->value() << "\n";
    
    // 限制2：自定义删除器
    std::cout << "\n【限制2：自定义删除器】\n";
    std::cout << "make_unique不直接支持自定义删除器\n";
    std::cout << "变通：手动构造unique_ptr\n";
    auto customDeleter = [](int* p) {
        std::cout << "自定义删除\n";
        delete p;
    };
    std::unique_ptr<int, decltype(customDeleter)> pCustom(new int(99), customDeleter);
    std::cout << "使用自定义删除器的指针: " << *pCustom << "\n";
}

// ============================================================
// 6. make_unique_for_overwrite (C++20)
// ============================================================

void demoCpp20Features() {
    printSeparator("C++20特性");
    
#if __cplusplus >= 202002L
    std::cout << "\n【make_unique_for_overwrite】\n";
    // 创建数组但不初始化（性能优化）
    auto arr = std::make_unique_for_overwrite<int[]>(1000000);
    std::cout << "创建大数组但不初始化（C++20）\n";
#else
    std::cout << "\n【make_unique_for_overwrite (C++20)】\n";
    std::cout << "创建数组但不进行值初始化，适用于立即覆盖的场景\n";
    std::cout << "示例: auto arr = std::make_unique_for_overwrite<int[]>(1000);\n";
    std::cout << "当前编译器可能不支持\n";
#endif
}

// ============================================================
// 7. 最佳实践总结
// ============================================================

void demoBestPractices() {
    printSeparator("最佳实践");
    
    std::cout << "\n┌───────────────────────────────────────────────────────────┐\n";
    std::cout << "│                 make_unique最佳实践                       │\n";
    std::cout << "├───────────────────────────────────────────────────────────┤\n";
    std::cout << "│ 1. 优先使用make_unique而不是new                          │\n";
    std::cout << "│    auto p = std::make_unique<T>(args...);                │\n";
    std::cout << "│                                                          │\n";
    std::cout << "│ 2. 使用auto避免类型重复                                   │\n";
    std::cout << "│    auto p = std::make_unique<int>(42);  // 推荐          │\n";
    std::cout << "│    std::unique_ptr<int> p(new int(42)); // 不推荐        │\n";
    std::cout << "│                                                          │\n";
    std::cout << "│ 3. 大数组考虑使用vector代替                               │\n";
    std::cout << "│    auto vec = std::vector<int>(1000);                    │\n";
    std::cout << "│                                                          │\n";
    std::cout << "│ 4. 自定义删除器场景使用unique_ptr构造函数                  │\n";
    std::cout << "│    std::unique_ptr<T, Deleter> p(new T, deleter);        │\n";
    std::cout << "└───────────────────────────────────────────────────────────┘\n";
}

// ============================================================
// 主函数
// ============================================================

int main() {
    std::cout << "╔═════════════════════════════════════════════════════════╗\n";
    std::cout << "║              make_unique用法详解                        ║\n";
    std::cout << "╚═════════════════════════════════════════════════════════╝\n";
    
    demoBasicUsage();
    demoArrayCreation();
    demoCompareWithNew();
    demoExceptionSafety();
    demoLimitations();
    demoCpp20Features();
    demoBestPractices();
    
    printSeparator("总结");
    std::cout << "make_unique是创建unique_ptr的首选方式:\n";
    std::cout << "  ✓ 代码简洁（类型推导）\n";
    std::cout << "  ✓ 异常安全（避免内存泄漏）\n";
    std::cout << "  ✓ 性能优异（无额外开销）\n";
    std::cout << "  ✓ 统一风格（与make_shared一致）\n";
    
    return 0;
}
