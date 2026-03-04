/**
 * @file universal_reference_demo.cpp
 * @brief Day 24: 通用引用 (Universal Reference) 演示
 * 
 * 通用引用是 Scott Meyers 在《Effective Modern C++》中提出的术语，
 * 也被称为转发引用 (Forwarding Reference)。
 * 
 * 核心要点：
 * 1. 只有在类型推导发生时，T&& 才是通用引用
 * 2. 通用引用可以绑定到左值或右值
 * 3. 引用折叠决定最终类型
 */

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <type_traits>

// ==================== 辅助宏定义 ====================

// 打印类型信息的辅助宏
#define PRINT_TYPE(expr) \
    std::cout << "  " << #expr << " 类型: " << typeid(expr).name() << std::endl

// 使用 type_traits 检查引用类型
template<typename T>
void printRefType(const std::string& varName) {
    std::cout << "  " << varName << ":\n";
    std::cout << "    - is_lvalue_reference: " << std::is_lvalue_reference_v<T> << "\n";
    std::cout << "    - is_rvalue_reference: " << std::is_rvalue_reference_v<T> << "\n";
    std::cout << "    - is_reference: " << std::is_reference_v<T> << "\n";
}

// ==================== 通用引用示例 ====================

/**
 * @brief 演示通用引用的函数
 * 
 * 这是通用引用的典型形式：
 * - T 是模板参数
 * - 参数形式是 T&&（注意：没有 const 修饰）
 * - T 的类型会被推导
 * 
 * 当传入左值时：T = Type&，最终 T&& = Type&（引用折叠）
 * 当传入右值时：T = Type，最终 T&& = Type&&
 */
template<typename T>
void universalRefDemo(T&& arg) {
    std::cout << "\n--- 通用引用函数调用 ---\n";
    
    // 打印 T 的类型信息
    printRefType<T>("模板参数 T");
    
    // 打印 T&& 的类型信息
    using ParamType = T&&;
    printRefType<ParamType>("参数类型 T&&");
    
    // 尝试修改（如果是左值引用，修改会影响原值）
    if constexpr (std::is_lvalue_reference_v<T>) {
        std::cout << "  可以修改原值（左值引用）\n";
    } else {
        std::cout << "  可以移动资源（右值引用）\n";
    }
}

// ==================== 非通用引用示例 ====================

/**
 * @brief 非通用引用示例：固定类型的右值引用
 * 
 * 这不是通用引用，因为：
 * - std::string&& 是固定类型，没有类型推导
 * - 只能接受 std::string 类型的右值
 */
void notUniversalRef(std::string&& arg) {
    std::cout << "  非通用引用（右值引用）: " << arg << "\n";
}

/**
 * @brief 非通用引用示例：带 const 修饰
 * 
 * 这不是通用引用，因为：
 * - 有 const 修饰符
 * - 只能接受右值
 */
template<typename T>
void alsoNotUniversal(const T&& arg) {
    std::cout << "  const T&& 不是通用引用: " << arg << "\n";
}

/**
 * @brief 非通用引用示例：模板参数不参与推导
 * 
 * 这不是通用引用，因为：
 * - std::vector<T>&& 中 T 虽然是模板参数
 * - 但 vector<T> 是一个整体类型，T&& 的推导规则不适用
 */
template<typename T>
void vectorRefDemo(std::vector<T>&& arg) {
    std::cout << "  vector<T>&& 不是通用引用，size = " << arg.size() << "\n";
}

// ==================== auto 与通用引用 ====================

/**
 * @brief 演示 auto&& 作为通用引用
 * 
 * auto&& 也是通用引用，遵循相同的规则：
 * - auto 会进行类型推导
 * - auto&& 可以绑定到左值或右值
 */
void autoUniversalRefDemo() {
    std::cout << "\n=== auto&& 通用引用演示 ===\n";
    
    int x = 42;
    
    // auto&& 绑定到左值
    auto&& ref1 = x;
    std::cout << "auto&& ref1 = x (左值):\n";
    printRefType<decltype(ref1)>("ref1");
    
    // auto&& 绑定到右值
    auto&& ref2 = 100;
    std::cout << "auto&& ref2 = 100 (右值):\n";
    printRefType<decltype(ref2)>("ref2");
    
    // auto&& 绑定到 std::move 的结果
    auto&& ref3 = std::move(x);
    std::cout << "auto&& ref3 = std::move(x):\n";
    printRefType<decltype(ref3)>("ref3");
}

// ==================== 实际应用示例 ====================

/**
 * @brief 一个简单的 Widget 类，用于演示
 */
class Widget {
public:
    Widget() : name_("default"), id_(0) {
        std::cout << "  Widget 默认构造\n";
    }
    
    Widget(std::string name, int id) : name_(std::move(name)), id_(id) {
        std::cout << "  Widget 构造: " << name_ << ", id=" << id_ << "\n";
    }
    
    // 拷贝构造
    Widget(const Widget& other) : name_(other.name_), id_(other.id_) {
        std::cout << "  Widget 拷贝构造\n";
    }
    
    // 移动构造
    Widget(Widget&& other) noexcept 
        : name_(std::move(other.name_)), id_(other.id_) {
        other.id_ = 0;
        std::cout << "  Widget 移动构造\n";
    }
    
    ~Widget() {
        std::cout << "  Widget 析构: " << name_ << "\n";
    }
    
    void print() const {
        std::cout << "  Widget[" << name_ << ", id=" << id_ << "]\n";
    }
    
private:
    std::string name_;
    int id_;
};

/**
 * @brief 工厂函数：使用通用引用接收参数
 * 
 * 这是一个典型的通用引用应用场景：
 * - 可以接受左值（拷贝）
 * - 可以接受右值（移动）
 * - 避免多次重载
 */
template<typename T>
Widget createWidget(T&& name, int id) {
    std::cout << "  创建 Widget...\n";
    return Widget(std::forward<T>(name), id);
}

// ==================== 主演示函数 ====================

void runUniversalRefDemo() {
    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "   Day 24: 通用引用演示\n";
    std::cout << "========================================\n";
    
    // 1. 基本通用引用演示
    std::cout << "\n=== 1. 通用引用基本演示 ===\n";
    
    int lvalue = 42;
    const int constLvalue = 100;
    
    std::cout << "\n传入左值:\n";
    universalRefDemo(lvalue);
    
    std::cout << "\n传入 const 左值:\n";
    universalRefDemo(constLvalue);
    
    std::cout << "\n传入右值:\n";
    universalRefDemo(42);
    
    std::cout << "\n传入 std::move 结果:\n";
    universalRefDemo(std::move(lvalue));
    
    // 2. 字符串类型演示
    std::cout << "\n=== 2. 字符串类型演示 ===\n";
    
    std::string str = "Hello";
    
    std::cout << "\n传入字符串左值:\n";
    universalRefDemo(str);
    
    std::cout << "\n传入字符串右值:\n";
    universalRefDemo(std::string("World"));
    
    std::cout << "\n传入字符串字面量:\n";
    universalRefDemo("Literal");
    
    // 3. 非通用引用对比
    std::cout << "\n=== 3. 非通用引用对比 ===\n";
    
    std::cout << "\n非通用引用 - 右值引用:\n";
    // notUniversalRef(str);  // 编译错误！不能绑定左值
    notUniversalRef(std::string("right"));  // OK
    notUniversalRef("string literal");  // OK（隐式转换）
    
    std::cout << "\n非通用引用 - const T&&:\n";
    // alsoNotUniversal(lvalue);  // 编译错误！
    alsoNotUniversal(42);  // OK
    
    std::cout << "\n非通用引用 - vector<T>&&:\n";
    std::vector<int> vec = {1, 2, 3};
    // vectorRefDemo(vec);  // 编译错误！
    vectorRefDemo(std::vector<int>{4, 5, 6});  // OK
    
    // 4. auto&& 演示
    autoUniversalRefDemo();
    
    // 5. 实际应用演示
    std::cout << "\n=== 5. 实际应用演示 ===\n";
    
    std::cout << "\n使用左值创建 Widget:\n";
    std::string widgetName = "MyWidget";
    auto w1 = createWidget(widgetName, 1);
    w1.print();
    
    std::cout << "\n使用右值创建 Widget:\n";
    auto w2 = createWidget(std::string("TempWidget"), 2);
    w2.print();
    
    std::cout << "\n使用字面量创建 Widget:\n";
    auto w3 = createWidget("LiteralWidget", 3);
    w3.print();
    
    std::cout << "\n========================================\n";
    std::cout << "   通用引用演示完成\n";
    std::cout << "========================================\n";
}
