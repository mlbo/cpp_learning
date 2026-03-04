/**
 * @file initializer_list.cpp
 * @brief std::initializer_list 详解
 *
 * 本文件深入讲解：
 * 1. initializer_list 的基本用法
 * 2. 工作原理和实现机制
 * 3. 与构造函数重载的交互
 * 4. 常见陷阱与最佳实践
 */

#include <iostream>
#include <vector>
#include <string>
#include <initializer_list>
#include <algorithm>
#include <numeric>

namespace cpp11_features {

// ============================================================
// 一、initializer_list 基础
// ============================================================

/**
 * @brief 演示 initializer_list 的基本操作
 */
void demo_basic_operations() {
    std::cout << "\n=== initializer_list 基本操作 ===\n";

    // 创建 initializer_list
    std::initializer_list<int> list1 = {1, 2, 3, 4, 5};
    std::initializer_list<int> list2{10, 20, 30};

    // 基本属性
    std::cout << "list1 大小: " << list1.size() << "\n";
    std::cout << "list2 大小: " << list2.size() << "\n";

    // 遍历方式 1：范围 for
    std::cout << "list1 元素（范围for）: ";
    for (int x : list1) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    // 遍历方式 2：迭代器
    std::cout << "list2 元素（迭代器）: ";
    for (auto it = list2.begin(); it != list2.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    // 计算总和
    int sum = std::accumulate(list1.begin(), list1.end(), 0);
    std::cout << "list1 总和: " << sum << "\n";
}

// ============================================================
// 二、作为函数参数
// ============================================================

/**
 * @brief 接受 initializer_list 的函数示例
 */
void print_values(std::initializer_list<int> values) {
    std::cout << "值列表: ";
    for (int v : values) {
        std::cout << v << " ";
    }
    std::cout << "\n";
}

/**
 * @brief 返回最大值的函数
 */
int max_value(std::initializer_list<int> values) {
    if (values.size() == 0) {
        throw std::invalid_argument("空列表");
    }
    return *std::max_element(values.begin(), values.end());
}

/**
 * @brief 可变参数求和
 */
double sum_values(std::initializer_list<double> values) {
    return std::accumulate(values.begin(), values.end(), 0.0);
}

void demo_function_parameter() {
    std::cout << "\n=== initializer_list 作为函数参数 ===\n";

    // 直接传递花括号列表
    print_values({1, 2, 3, 4, 5});
    print_values({10, 20, 30});

    // 计算最大值
    std::cout << "最大值: " << max_value({3, 1, 4, 1, 5, 9, 2, 6}) << "\n";

    // 求和
    std::cout << "总和: " << sum_values({1.1, 2.2, 3.3, 4.4}) << "\n";

    // 也可以先创建变量再传递
    auto list = {100, 200, 300};
    print_values(list);
}

// ============================================================
// 三、自定义类支持 initializer_list
// ============================================================

/**
 * @brief 自定义容器类，支持 initializer_list 构造
 */
class IntContainer {
public:
    // 默认构造
    IntContainer() = default;

    // initializer_list 构造
    IntContainer(std::initializer_list<int> init) : data_(init) {
        std::cout << "initializer_list 构造，元素数: " << data_.size() << "\n";
    }

    // 指定大小和初始值
    IntContainer(size_t count, int value) : data_(count, value) {
        std::cout << "count-value 构造: " << count << " 个 " << value << "\n";
    }

    // 添加元素
    void append(std::initializer_list<int> values) {
        data_.insert(data_.end(), values.begin(), values.end());
    }

    // 打印
    void print() const {
        std::cout << "容器内容: [";
        for (size_t i = 0; i < data_.size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << data_[i];
        }
        std::cout << "]\n";
    }

    size_t size() const { return data_.size(); }

private:
    std::vector<int> data_;
};

/**
 * @brief 字符串容器类
 */
class StringContainer {
public:
    StringContainer(std::initializer_list<std::string> init) {
        for (const auto& s : init) {
            data_.push_back(s);
        }
    }

    void print() const {
        std::cout << "字符串列表: ";
        for (const auto& s : data_) {
            std::cout << "\"" << s << "\" ";
        }
        std::cout << "\n";
    }

private:
    std::vector<std::string> data_;
};

void demo_custom_class() {
    std::cout << "\n=== 自定义类支持 initializer_list ===\n";

    // 使用 initializer_list 构造
    IntContainer c1{1, 2, 3, 4, 5};
    c1.print();

    // 使用 count-value 构造
    IntContainer c2(5, 100);  // 5 个 100
    c2.print();

    // 追加元素
    c1.append({6, 7, 8});
    c1.print();

    // 字符串容器
    StringContainer sc{"hello", "world", "C++11"};
    sc.print();
}

// ============================================================
// 四、构造函数重载决议详解
// ============================================================

/**
 * @brief 用于演示重载决议的类
 */
class OverloadDemo {
public:
    // 构造函数 1：两个 int 参数
    OverloadDemo(int a, int b) {
        std::cout << "调用 (int, int) 构造: " << a << ", " << b << "\n";
    }

    // 构造函数 2：initializer_list<int>
    OverloadDemo(std::initializer_list<int> list) {
        std::cout << "调用 initializer_list 构造，元素数: " << list.size() << "\n";
        std::cout << "  元素: ";
        for (int x : list) {
            std::cout << x << " ";
        }
        std::cout << "\n";
    }

    // 构造函数 3：默认构造
    OverloadDemo() {
        std::cout << "调用默认构造\n";
    }
};

void demo_overload_resolution() {
    std::cout << "\n=== 构造函数重载决议 ===\n";

    // 情况 1：圆括号调用
    std::cout << "OverloadDemo(1, 2): ";
    OverloadDemo od1(1, 2);  // 调用 (int, int) 构造

    // 情况 2：花括号调用（优先 initializer_list）
    std::cout << "OverloadDemo{1, 2}: ";
    OverloadDemo od2{1, 2};  // 调用 initializer_list 构造！

    // 情况 3：空花括号
    std::cout << "OverloadDemo{}: ";
    OverloadDemo od3{};      // 调用默认构造

    // 情况 4：单个参数
    std::cout << "OverloadDemo{42}: ";
    OverloadDemo od4{42};    // 调用 initializer_list 构造

    // 情况 5：三个参数（只有 initializer_list 能接受）
    std::cout << "OverloadDemo{1, 2, 3}: ";
    OverloadDemo od5{1, 2, 3};  // 必须调用 initializer_list 构造

    // 情况 6：空 initializer_list（显式）
    std::cout << "OverloadDemo(std::initializer_list<int>{}): ";
    OverloadDemo od6(std::initializer_list<int>{});  // 显式调用 initializer_list
}

// ============================================================
// 五、vector 的经典陷阱
// ============================================================

void demo_vector_trap() {
    std::cout << "\n=== vector 的初始化陷阱 ===\n";

    // 陷阱：花括号 vs 圆括号
    std::vector<int> v1{10, 20};    // 2 个元素: 10, 20
    std::vector<int> v2(10, 20);    // 10 个元素，每个都是 20

    std::cout << "v1{10, 20} 大小: " << v1.size() << "\n";
    std::cout << "v1 内容: ";
    for (int x : v1) std::cout << x << " ";
    std::cout << "\n";

    std::cout << "v2(10, 20) 大小: " << v2.size() << "\n";
    std::cout << "v2 内容（前5个）: ";
    for (size_t i = 0; i < 5; ++i) std::cout << v2[i] << " ";
    std::cout << "...\n";

    // 更多示例
    std::vector<int> v3{5};     // 1 个元素: 5
    std::vector<int> v4(5);     // 5 个元素，每个都是 0

    std::cout << "v3{5} 大小: " << v3.size() << "\n";
    std::cout << "v4(5) 大小: " << v4.size() << "\n";
}

// ============================================================
// 六、initializer_list 的实现原理
// ============================================================

void demo_implementation_details() {
    std::cout << "\n=== initializer_list 实现原理 ===\n";

    // 当我们写 {1, 2, 3, 4, 5} 时，编译器会：
    // 1. 创建一个临时数组 const int __arr[] = {1, 2, 3, 4, 5}
    // 2. initializer_list 指向这个数组

    auto list = {1, 2, 3, 4, 5};  // 类型是 std::initializer_list<int>

    std::cout << "sizeof(initializer_list<int>): " << sizeof(list) << " 字节\n";
    // 通常是一个指针 + 一个 size_t，所以是 16 字节（64位系统）

    // initializer_list 的元素是 const 的
    // *list.begin() = 10;  // 错误！不能修改

    // 可以复制（低成本，只复制指针和大小）
    auto list2 = list;
    std::cout << "复制后的 list2 大小: " << list2.size() << "\n";
}

// ============================================================
// 七、类型推导与 auto
// ============================================================

void demo_auto_deduction() {
    std::cout << "\n=== auto 与 initializer_list ===\n";

    // auto 推导为 std::initializer_list<int>
    auto list1 = {1, 2, 3, 4, 5};
    std::cout << "auto list1 = {1,2,3,4,5}: initializer_list<int>, size=" << list1.size() << "\n";

    // auto 推导为 std::initializer_list<double>
    auto list2 = {1.0, 2.0, 3.0};
    std::cout << "auto list2 = {1.0,2.0,3.0}: initializer_list<double>, size=" << list2.size() << "\n";

    // 混合类型推导为 common_type
    std::initializer_list<double> list3 = {1, 2.0, 3};  // double
    std::cout << "auto list3 = {1, 2.0, 3}: initializer_list<double>, size=" << list3.size() << "\n";

    // 注意：单个元素推导为值类型，不是 initializer_list
    auto x = {42};  // initializer_list<int>，只有一个元素
    // auto y = 42;   // int，不是 initializer_list！
    std::cout << "auto x = {42}: size=" << x.size() << "\n";

    // 显式指定类型
    std::initializer_list<std::string> strs = {"a", "b", "c"};
    std::cout << "显式 initializer_list<string>: size=" << strs.size() << "\n";
}

// ============================================================
// 主演示函数
// ============================================================

void run_initializer_list_demo() {
    std::cout << "\n╔══════════════════════════════════════════════════════════╗\n";
    std::cout << "║          std::initializer_list 详解                      ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════╝\n";

    demo_basic_operations();
    demo_function_parameter();
    demo_custom_class();
    demo_overload_resolution();
    demo_vector_trap();
    demo_implementation_details();
    demo_auto_deduction();

    std::cout << "\n=== initializer_list 要点总结 ===\n";
    std::cout << "1. 轻量级代理类，只存储指针和大小\n";
    std::cout << "2. 元素是 const 的，不可修改\n";
    std::cout << "3. 构造函数重载时优先匹配 initializer_list\n";
    std::cout << "4. 注意 vector 的 {count, value} vs {a, b} 陷阱\n";
    std::cout << "5. auto + {} 通常推导为 initializer_list\n";
}

} // namespace cpp11_features
