/**
 * @file uniform_init.cpp
 * @brief C++11 统一初始化（Uniform Initialization）详解
 *
 * 本文件展示15种以上的初始化场景，涵盖：
 * 1. 基本类型初始化
 * 2. 数组初始化
 * 3. 容器初始化
 * 4. 类类型初始化
 * 5. 成员初始化
 * 6. 动态内存初始化
 */

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <atomic>
#include <array>
#include <utility>

namespace cpp11_features {

// ============================================================
// 场景 1-5：基本类型初始化
// ============================================================

void demo_basic_types() {
    std::cout << "\n=== 场景 1-5：基本类型初始化 ===\n";

    // 场景 1：整数初始化的多种方式
    int a1 = 10;        // 拷贝初始化（传统）
    int a2(10);         // 直接初始化（传统）
    int a3 = {10};      // 拷贝列表初始化（C++11）
    int a4{10};         // 直接列表初始化（C++11，推荐）
    std::cout << "int: " << a1 << ", " << a2 << ", " << a3 << ", " << a4 << "\n";

    // 场景 2：浮点数初始化
    double d1 = 3.14;
    double d2(3.14);
    double d3{3.14};    // 推荐方式
    std::cout << "double: " << d1 << ", " << d2 << ", " << d3 << "\n";

    // 场景 3：字符初始化
    char c1 = 'A';
    char c2{'A'};       // 推荐方式
    std::cout << "char: " << c1 << ", " << c2 << "\n";

    // 场景 4：布尔类型初始化
    bool b1 = true;
    bool b2{false};     // 推荐方式
    std::cout << "bool: " << b1 << ", " << b2 << "\n";

    // 场景 5：指针初始化
    int* p1 = nullptr;
    int* p2{nullptr};   // 推荐方式
    int x = 42;
    int* p3{&x};        // 统一风格
    std::cout << "pointer: " << p1 << ", " << p2 << ", " << *p3 << "\n";
}

// ============================================================
// 场景 6-8：数组初始化
// ============================================================

void demo_arrays() {
    std::cout << "\n=== 场景 6-8：数组初始化 ===\n";

    // 场景 6：C 风格数组
    int arr1[] = {1, 2, 3, 4, 5};
    int arr2[]{1, 2, 3, 4, 5};  // 更简洁
    std::cout << "C数组: ";
    for (int i : arr2) std::cout << i << " ";
    std::cout << "\n";

    // 场景 7：std::array（推荐替代C数组）
    std::array<int, 5> stdArr1 = {1, 2, 3, 4, 5};
    std::array<int, 5> stdArr2{1, 2, 3, 4, 5};  // 推荐
    std::cout << "std::array: ";
    for (int i : stdArr2) std::cout << i << " ";
    std::cout << "\n";

    // 场景 8：动态数组初始化
    int* dynamicArr = new int[5]{1, 2, 3, 4, 5};  // C++11 支持
    std::cout << "动态数组: ";
    for (int i = 0; i < 5; ++i) std::cout << dynamicArr[i] << " ";
    std::cout << "\n";
    delete[] dynamicArr;
}

// ============================================================
// 场景 9-11：标准容器初始化
// ============================================================

void demo_containers() {
    std::cout << "\n=== 场景 9-11：标准容器初始化 ===\n";

    // 场景 9：vector 初始化
    std::vector<int> v1;
    v1.push_back(1); v1.push_back(2); v1.push_back(3);  // 传统繁琐方式
    std::vector<int> v2{1, 2, 3, 4, 5};                  // C++11 统一初始化
    std::vector<int> v3 = {1, 2, 3};                     // 拷贝列表初始化
    std::cout << "vector: ";
    for (int i : v2) std::cout << i << " ";
    std::cout << "\n";

    // 场景 10：map 初始化
    std::map<std::string, int> m1{
        {"apple", 1},
        {"banana", 2},
        {"cherry", 3}
    };
    std::cout << "map: ";
    for (const auto& p : m1) {
        std::cout << "[" << p.first << ":" << p.second << "] ";
    }
    std::cout << "\n";

    // 场景 11：嵌套容器初始化
    std::vector<std::pair<std::string, int>> pairs{
        {"one", 1},
        {"two", 2},
        {"three", 3}
    };
    std::cout << "嵌套容器: ";
    for (const auto& p : pairs) {
        std::cout << "(" << p.first << "," << p.second << ") ";
    }
    std::cout << "\n";
}

// ============================================================
// 场景 12-13：类类型初始化
// ============================================================

struct Point {
    int x, y;
};

class Widget {
public:
    Widget() : id_{0}, name_{"default"} {}
    Widget(int id, std::string name) : id_{id}, name_{std::move(name)} {}

    void print() const {
        std::cout << "Widget(id=" << id_ << ", name=" << name_ << ")\n";
    }

private:
    int id_;
    std::string name_;
};

void demo_class_types() {
    std::cout << "\n=== 场景 12-13：类类型初始化 ===\n";

    // 场景 12：聚合类型初始化
    Point p1 = {1, 2};
    Point p2{3, 4};  // 推荐
    std::cout << "Point: (" << p1.x << "," << p1.y << ") (" << p2.x << "," << p2.y << ")\n";

    // 场景 13：类类型初始化
    Widget w1;                    // 默认构造
    Widget w2{};                  // 默认构造（避免歧义）
    Widget w3{42, "test"};        // 参数化构造
    Widget w4 = {42, "test2"};    // 拷贝列表初始化

    std::cout << "Widget: ";
    w3.print();
}

// ============================================================
// 场景 14-15：成员初始化与特殊场景
// ============================================================

class Config {
public:
    Config() = default;

    void print() const {
        std::cout << "Config: value=" << value_
                  << ", enabled=" << std::boolalpha << enabled_
                  << ", data=[";
        for (size_t i = 0; i < data_.size(); ++i) {
            if (i > 0) std::cout << ",";
            std::cout << data_[i];
        }
        std::cout << "]\n";
    }

private:
    // 场景 14：类内成员初始化（C++11）
    int value_{100};                      // 花括号初始化
    bool enabled_{true};                  // 花括号初始化
    std::vector<int> data_{1, 2, 3, 4, 5}; // 花括号初始化
    // int test(0);   // 错误！圆括号不支持类内初始化
};

void demo_member_init() {
    std::cout << "\n=== 场景 14：类内成员初始化 ===\n";
    Config config;
    config.print();
}

void demo_special_cases() {
    std::cout << "\n=== 场景 15：特殊场景初始化 ===\n";

    // 场景 15.1：不可拷贝对象的初始化
    std::atomic<int> a1{0};       // OK
    // std::atomic<int> a2 = 0;  // 错误！不可拷贝
    std::cout << "atomic: " << a1.load() << "\n";

    // 场景 15.2：unique_ptr 初始化
    std::unique_ptr<int> up1{new int{42}};
    // C++11写法（注释）: std::unique_ptr<int> up2(new int(42));
    auto up2 = std::make_unique<int>(42);  // 更推荐的方式
    std::cout << "unique_ptr: " << *up1 << ", " << *up2 << "\n";

    // 场景 15.3：pair 和 tuple 初始化
    std::pair<int, std::string> p1{1, "hello"};
    auto t1 = std::make_tuple(1, 2.0, "three");
    std::cout << "pair: (" << p1.first << ", " << p1.second << ")\n";
}

// ============================================================
// 场景 16-18：窄化转换检查
// ============================================================

void demo_narrowing_conversion() {
    std::cout << "\n=== 场景 16-18：窄化转换检查 ===\n";

    // 场景 16：double 到 int 的窄化
    double d = 3.14;
    int i1 = d;      // OK，但有数据丢失（i1 = 3）
    int i2(d);       // OK，但有数据丢失
    // int i3{d};    // 错误！花括号禁止窄化转换
    // int i4 = {d}; // 错误！
    std::cout << "double->int: " << i1 << " (警告：数据丢失)\n";

    // 场景 17：long long 到 int 的窄化
    long long ll = 1000000000000LL;
    int x1 = ll;     // OK，但有数据丢失
    // int x2{ll};    // 错误！窄化转换
    std::cout << "long long->int: 编译器会警告\n";

    // 场景 18：int 到 double（不是窄化，允许）
    int n = 42;
    double d1{static_cast<double>(n)};    // OK
    std::cout << "int->double: " << d1 << " (安全转换)\n";
}

// ============================================================
// 主演示函数
// ============================================================

void run_uniform_init_demo() {
    std::cout << "╔══════════════════════════════════════════════════════════╗\n";
    std::cout << "║      C++11 统一初始化（Uniform Initialization）演示      ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════╝\n";

    demo_basic_types();
    demo_arrays();
    demo_containers();
    demo_class_types();
    demo_member_init();
    demo_special_cases();
    demo_narrowing_conversion();

    std::cout << "\n=== 统一初始化要点总结 ===\n";
    std::cout << "1. 使用 {} 进行初始化风格统一\n";
    std::cout << "2. 花括号初始化禁止窄化转换（安全特性）\n";
    std::cout << "3. 类内成员初始化只支持 {} 或 =\n";
    std::cout << "4. 容器初始化更加简洁直观\n";
    std::cout << "5. 避免'最令人苦恼的解析'问题\n";
}

} // namespace cpp11_features
