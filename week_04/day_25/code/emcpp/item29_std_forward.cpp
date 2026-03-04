/**
 * @file item29_std_forward_detail.cpp
 * @brief EMC++ Item 29: 理解std::forward的工作机制
 * 
 * 本文件演示：
 * 1. std::forward的本质：条件转换
 * 2. 为什么std::forward只适用于万能引用
 * 3. 类型推导如何保存值类别信息
 * 4. std::forward的典型使用模式
 */

#include <iostream>
#include <string>
#include <utility>
#include <type_traits>
#include <memory>
#include <vector>

// ==================== 辅助输出 ====================

#define LOG(msg) std::cout << msg << std::endl

// ==================== 核心概念演示 ====================

/**
 * std::forward的本质
 * 
 * std::forward是一个条件转换工具：
 * - 如果T是左值引用类型(Type&)，则返回左值引用
 * - 如果T是非引用类型(Type)，则返回右值引用
 * 
 * 简化的实现概念：
 * template<typename T>
 * T&& forward(typename std::remove_reference<T>::type& param) {
 *     return static_cast<T&&>(param);
 * }
 */

// 模拟std::forward的行为（仅用于理解）
template<typename T>
T&& myForward(typename std::remove_reference<T>::type& param) {
    return static_cast<T&&>(param);
}

// ==================== 类型推导演示 ====================

// 展示模板参数T如何保存值类别信息
template<typename T>
void showTypeDeduction(T&& param) {
    std::cout << "\n--- 类型推导分析 ---\n";
    
    // 显示T的类型
    if (std::is_lvalue_reference<T>::value) {
        std::cout << "  T = " << typeid(T).name() << " (左值引用)\n";
        std::cout << "  这意味着传入的是左值\n";
    } else {
        std::cout << "  T = " << typeid(T).name() << " (非引用类型)\n";
        std::cout << "  这意味着传入的是右值\n";
    }
    
    // T&&的实际类型
    typedef T&& ParamType;
    if (std::is_lvalue_reference<ParamType>::value) {
        std::cout << "  T&& = " << typeid(ParamType).name() << " (左值引用，因为引用折叠)\n";
    } else {
        std::cout << "  T&& = " << typeid(ParamType).name() << " (右值引用)\n";
    }
    
    // std::forward<T>的返回类型
    typedef decltype(std::forward<T>(param)) ForwardType;
    if (std::is_lvalue_reference<ForwardType>::value) {
        std::cout << "  std::forward<T>(param) 返回: 左值引用\n";
    } else {
        std::cout << "  std::forward<T>(param) 返回: 右值引用\n";
    }
}

// ==================== 为什么只适用于万能引用 ====================

// 错误示例：对非万能引用使用forward
void wrongForwardDemo() {
    std::cout << "\n【错误示例：对非万能引用使用forward】\n\n";
    
    std::cout << "问题代码:\n";
    std::cout << "  void func(int&& param) {\n";
    std::cout << "      // param在这里是左值！\n";
    std::cout << "      // std::forward<int>(param) 返回 int&&\n";
    std::cout << "      // 但这实际上是多余的转换\n";
    std::cout << "  }\n\n";
    
    std::cout << "解释：\n";
    std::cout << "  1. int&& param 只能接受右值\n";
    std::cout << "  2. 在函数内部，param是有名字的变量，所以是左值\n";
    std::cout << "  3. std::forward<int>(param) 会返回 int&&\n";
    std::cout << "  4. 但没有类型信息来区分原始值类别\n\n";
    
    std::cout << "正确做法：使用std::move\n";
    std::cout << "  void func(int&& param) {\n";
    std::cout << "      doSomething(std::move(param));  // 清晰表明意图\n";
    std::cout << "  }\n";
}

// 正确示例：在万能引用中使用forward
template<typename T>
void correctForwardDemo(T&& param) {
    std::cout << "\n【正确示例：在万能引用中使用forward】\n";
    
    // T保存了值类别信息
    // std::forward根据T来决定返回类型
    
    if (std::is_lvalue_reference<T>::value) {
        std::cout << "  传入左值，std::forward保持为左值\n";
    } else {
        std::cout << "  传入右值，std::forward转换为右值\n";
    }
}

// ==================== 典型使用模式 ====================

// 模式1：工厂函数
class Product {
public:
    Product(const std::string& name, int id) 
        : name_(name), id_(id) {
        std::cout << "  [Product] 左值构造\n";
    }
    
    Product(std::string&& name, int id) 
        : name_(std::move(name)), id_(id) {
        std::cout << "  [Product] 右值构造\n";
    }
    
private:
    std::string name_;
    int id_;
};

template<typename T, typename... Args>
std::unique_ptr<T> create(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

// 模式2：包装函数
template<typename Func, typename... Args>
auto call(Func&& func, Args&&... args) 
    -> decltype(std::forward<Func>(func)(std::forward<Args>(args)...))
{
    return std::forward<Func>(func)(std::forward<Args>(args)...);
}

// 模式3：类模板参数转发
template<typename T>
class Container {
public:
    template<typename U>
    void add(U&& item) {
        items_.push_back(std::forward<U>(item));
    }
    
private:
    std::vector<T> items_;
};

// ==================== std::forward的实现解析 ====================

void explainForwardImplementation() {
    std::cout << "\n【std::forward实现解析】\n\n";
    
    std::cout << "标准库中的实现（简化版）：\n\n";
    std::cout << "  template<typename T>\n";
    std::cout << "  constexpr T&& forward(\n";
    std::cout << "      typename std::remove_reference<T>::type& t) noexcept\n";
    std::cout << "  {\n";
    std::cout << "      return static_cast<T&&>(t);\n";
    std::cout << "  }\n\n";
    
    std::cout << "关键点分析：\n";
    std::cout << "  1. 参数类型：remove_reference<T>::type&\n";
    std::cout << "     - 无论T是什么，参数总是左值引用\n";
    std::cout << "     - 这允许接受任何类型的参数\n\n";
    
    std::cout << "  2. 返回类型：T&&\n";
    std::cout << "     - 如果T是 Type&，返回 Type& && → Type& (引用折叠)\n";
    std::cout << "     - 如果T是 Type，返回 Type&&\n\n";
    
    std::cout << "  3. static_cast<T&&>(t)\n";
    std::cout << "     - 根据 T 的类型进行正确的转换\n";
    std::cout << "     - 左值转左值引用，非引用转右值引用\n";
}

// ==================== 主演示函数 ====================

namespace emcpp {

void item29StdForwardDetail() {
    std::cout << "【EMC++ Item 29: 理解std::forward】\n";
    
    // ========== 类型推导演示 ==========
    std::cout << "\n===== 类型推导如何保存值类别信息 =====\n";
    
    int x = 42;
    std::cout << "\n传入左值 x:\n";
    showTypeDeduction(x);
    
    std::cout << "\n\n传入右值 42:\n";
    showTypeDeduction(42);
    
    // ========== 错误用法 ==========
    wrongForwardDemo();
    
    // ========== 正确用法 ==========
    std::cout << "\n===== 正确使用示例 =====\n";
    
    int y = 100;
    correctForwardDemo(y);       // 左值
    correctForwardDemo(200);     // 右值
    
    // ========== 使用模式 ==========
    std::cout << "\n===== 典型使用模式 =====\n\n";
    
    std::cout << "1. 工厂函数模式:\n";
    std::string name = "Widget";
    auto p1 = create<Product>(name, 1);        // 左值
    auto p2 = create<Product>("Gadget", 2);    // 右值
    
    std::cout << "\n2. 包装函数模式:\n";
    auto add = [](int a, int b) { return a + b; };
    int result = call(add, 3, 4);
    std::cout << "  call(add, 3, 4) = " << result << "\n";
    
    // ========== 实现解析 ==========
    explainForwardImplementation();
    
    std::cout << "\n===== Item 29 总结 =====\n";
    std::cout << "  1. std::forward是条件转换，不是无条件转换\n";
    std::cout << "  2. 它依赖于模板参数T中保存的值类别信息\n";
    std::cout << "  3. 只在万能引用参数上使用std::forward\n";
    std::cout << "  4. std::forward<T>(param) 中的T是关键\n";
}

} // namespace emcpp

// 独立测试入口
#ifdef STANDALONE_TEST
int main() {
    emcpp::item29StdForwardDetail();
    return 0;
}
#endif
