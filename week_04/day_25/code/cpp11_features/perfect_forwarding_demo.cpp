/**
 * @file perfect_forwarding_demo.cpp
 * @brief 完美转发演示
 * 
 * 本文件演示：
 * 1. 普通转发的问题
 * 2. 完美转发的解决方案
 * 3. 转发引用成立所需的类型推导与T&&形式
 * 4. 引用折叠规则
 */

#include "perfect_forwarding_demo.h"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <type_traits>

// ==================== 辅助函数 ====================

// 打印类型信息
#define PRINT_TYPE(expr) \
    std::cout << #expr << " 的类型: " << typeid(expr).name() << std::endl

// 目标函数：处理左值和右值。公开包装器与测试都走这两个真实重载。
cpp11_features::ForwardingRoute cpp11_features::detail::forwardingTarget(
    const std::string& s) {
    std::cout << "  [左值版本] 处理: \"" << s << "\"" << std::endl;
    return cpp11_features::ForwardingRoute::Lvalue;
}

cpp11_features::ForwardingRoute cpp11_features::detail::forwardingTarget(
    std::string&& s) {
    std::cout << "  [右值版本] 处理: \"" << s
              << "\" (到达右值重载，资源是否移动由目标实现决定)" << std::endl;
    return cpp11_features::ForwardingRoute::Rvalue;
}

// ==================== 问题演示：普通转发 ====================

namespace bad_forward {

// 问题：普通引用无法区分左值和右值
template<typename T>
cpp11_features::ForwardingRoute wrapperByLvalueReference(T& param) {
    std::cout << "  普通转发(wrapper<T&>): ";
    return cpp11_features::detail::forwardingTarget(param);  // 只能接收左值。
}

// 问题：这里 T 在调用时推导，所以 T&& 是转发引用，不是固定右值引用。
// 它能同时接收左值和右值，但命名形参 param 作为表达式是左值；
// 不使用 std::forward 就会丢掉调用点的值类别信息。
template<typename T>
cpp11_features::ForwardingRoute wrapperWithoutForward(T&& param) {
    std::cout << "  转发引用未恢复值类别(wrapper<T&&>): ";
    return cpp11_features::detail::forwardingTarget(param);  // 左、右值调用都到左值重载。
}

} // namespace bad_forward

// ==================== 解决方案：完美转发 ====================

namespace perfect_forward {

// 完美转发：使用转发引用 + std::forward
template<typename T>
cpp11_features::ForwardingRoute wrapper(T&& param) {
    std::cout << "  完美转发(wrapper<T&&>): ";
    return cpp11_features::forwardingRoute(std::forward<T>(param));
}

} // namespace perfect_forward

// ==================== 引用折叠演示 ====================

void demonstrateReferenceCollapsing() {
    std::cout << "\n【引用折叠规则演示】\n\n";
    
    std::cout << "引用折叠规则：\n";
    std::cout << "  Type&  &  → Type&   (左值引用的引用 → 左值引用)\n";
    std::cout << "  Type&  && → Type&   (左值引用的右值引用 → 左值引用)\n";
    std::cout << "  Type&& &  → Type&   (右值引用的左值引用 → 左值引用)\n";
    std::cout << "  Type&& && → Type&&  (右值引用的右值引用 → 右值引用)\n\n";
    
    // 使用类型推导演示
    int x = 10;
    int& lr = x;    // 左值引用
    int&& rr = 10;  // 右值引用

    std::cout << "  lr 当前值: " << lr << "，rr 当前值: " << rr << "\n";
    
    std::cout << "示例代码：\n";
    std::cout << "  int x = 10;\n";
    std::cout << "  int& lr = x;     // lr 是左值引用\n";
    std::cout << "  int&& rr = 10;   // rr 是右值引用\n\n";
    
    // 演示转发引用的类型推导
    std::cout << "转发引用类型推导（被推导的T&&）：\n";
    std::cout << "  当传入左值时，T推导为左值引用类型\n";
    std::cout << "  当传入右值时，T推导为非引用类型\n";
    std::cout << "  这就是为什么T能保存值类别信息\n";
}

// ==================== 实际应用场景 ====================

// 场景1：工厂函数
class Widget {
public:
    Widget(const std::string& name, int value) 
        : name_(name), value_(value) {
        std::cout << "  [Widget] 左值构造: " << name << ", " << value << std::endl;
    }
    
    Widget(std::string&& name, int value) 
        : name_(std::move(name)), value_(value) {
        std::cout << "  [Widget] 右值构造: (moved), " << value << std::endl;
    }
    
private:
    std::string name_;
    int value_;
};

// 使用完美转发的工厂函数
template<typename T, typename... Args>
std::unique_ptr<T> makeWidget(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

// 场景2：可变参数转发
void innerFunction(int a, const std::string& b, double c) {
    std::cout << "  innerFunction(" << a << ", \"" << b << "\", " << c << ")\n";
}

template<typename... Args>
void outerFunction(Args&&... args) {
    std::cout << "  外层函数转发参数到内层函数: ";
    innerFunction(std::forward<Args>(args)...);
}

// ==================== 主演示函数 ====================

namespace cpp11_features {

void perfectForwardingDemo() {
    std::cout << "【完美转发演示】\n\n";
    
    // ========== 问题演示 ==========
    std::cout << "===== 问题：普通转发无法保持值类别 =====\n\n";
    
    std::string str = "Hello";
    
    std::cout << "1. 普通引用转发:\n";
    std::cout << "  传入左值 \"" << str << "\":\n";
    const auto lvalueOnlyRoute = bad_forward::wrapperByLvalueReference(str);
    // bad_forward::wrapperByLvalueReference(std::string("World"));  // 编译错误：T& 不接受右值。
    
    std::cout << "\n2. 转发引用不使用 std::forward:\n";
    std::cout << "  T 在两次调用中都会推导，所以这个 T&& 能接收左值和右值。\n";
    std::cout << "  传入左值 \"" << str << "\":\n";
    const auto lostLvalueRoute = bad_forward::wrapperWithoutForward(str);
    std::cout << "  传入右值(临时对象):\n";
    const auto lostRvalueRoute =
        bad_forward::wrapperWithoutForward(std::string("Temporary"));
    std::cout << "  结论：转发引用可以接收左值和右值，但命名形参是左值，"
                 "不使用 std::forward 时两次都到左值重载。\n";

    if (lvalueOnlyRoute != ForwardingRoute::Lvalue ||
        lostLvalueRoute != ForwardingRoute::Lvalue ||
        lostRvalueRoute != ForwardingRoute::Lvalue) {
        throw std::logic_error("普通转发的左值路由契约失效");
    }
    
    // ========== 解决方案 ==========
    std::cout << "\n===== 解决方案：完美转发 =====\n\n";
    
    std::cout << "3. 完美转发:\n";
    std::cout << "  传入左值 \"" << str << "\":\n";
    const auto forwardedLvalueRoute = perfect_forward::wrapper(str);
    
    std::cout << "\n  传入右值(临时对象):\n";
    const auto forwardedRvalueRoute =
        perfect_forward::wrapper(std::string("World"));
    if (forwardedLvalueRoute != ForwardingRoute::Lvalue ||
        forwardedRvalueRoute != ForwardingRoute::Rvalue) {
        throw std::logic_error("完美转发未保持调用点值类别");
    }
    
    // ========== 引用折叠 ==========
    demonstrateReferenceCollapsing();
    
    // ========== 实际应用 ==========
    std::cout << "\n===== 实际应用场景 =====\n\n";
    
    std::cout << "1. 工厂函数:\n";
    std::string name = "TestWidget";
    auto w1 = makeWidget<Widget>(name, 42);       // 左值构造
    auto w2 = makeWidget<Widget>("TempName", 100); // 右值构造
    
    std::cout << "\n2. 可变参数转发:\n";
    outerFunction(1, std::string("test"), 3.14);
    outerFunction(2, "hello", 2.718);
}

} // namespace cpp11_features

// 独立测试入口
#ifdef STANDALONE_TEST
int main() {
    cpp11_features::perfectForwardingDemo();
    return 0;
}
#endif
