/**
 * @file std_forward_demo.cpp
 * @brief std::forward详解演示
 * 
 * 本文件演示：
 * 1. std::forward的工作原理
 * 2. std::forward与std::move的区别
 * 3. std::forward的正确使用方式
 * 4. 常见陷阱与最佳实践
 */

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <type_traits>

// ==================== 辅助输出 ====================

#define LOG(msg) std::cout << msg << std::endl
#define LOG_TYPE(expr, T) \
    std::cout << expr << ": T = " << typeid(T).name() \
              << (std::is_lvalue_reference<T>::value ? " (左值引用)" : " (非引用或右值引用)") \
              << std::endl

// ==================== 目标函数 ====================

// 模拟一个类，跟踪移动和拷贝操作
class HeavyObject {
public:
    HeavyObject(const std::string& name) : name_(name) {
        std::cout << "  构造: " << name_ << std::endl;
    }
    
    HeavyObject(const HeavyObject& other) : name_(other.name_ + "_copy") {
        std::cout << "  拷贝构造: " << name_ << std::endl;
    }
    
    HeavyObject(HeavyObject&& other) noexcept : name_(other.name_ + "_moved") {
        other.name_ = "invalid";
        std::cout << "  移动构造: " << name_ << std::endl;
    }
    
    ~HeavyObject() {
        std::cout << "  析构: " << name_ << std::endl;
    }
    
    const std::string& name() const { return name_; }
    
private:
    std::string name_;
};

// 接收左值和右值的重载函数
void processObject(const HeavyObject& obj) {
    std::cout << "  [左值版本] 处理: " << obj.name() << std::endl;
}

void processObject(HeavyObject&& obj) {
    std::cout << "  [右值版本] 处理: " << obj.name() << " (可以移动)" << std::endl;
}

// ==================== std::forward工作原理演示 ====================

template<typename T>
void showForwardBehavior(T&& param) {
    std::cout << "\n--- showForwardBehavior ---\n";
    LOG_TYPE("  模板参数", T);
    
    // 不使用forward：param永远是左值
    std::cout << "\n  不使用std::forward:\n";
    processObject(param);  // 总是调用左值版本
    
    // 使用forward：保持原有值类别
    std::cout << "\n  使用std::forward<T>:\n";
    processObject(std::forward<T>(param));
}

// ==================== std::move vs std::forward ====================

void demonstrateMoveVsForward() {
    std::cout << "\n【std::move vs std::forward】\n\n";
    
    std::cout << "1. std::move：无条件转换为右值\n";
    std::cout << "   用途：明确表示要移动对象的所有权\n\n";
    
    std::string str1 = "Hello";
    std::cout << "   原始字符串: \"" << str1 << "\"\n";
    std::string str2 = std::move(str1);
    std::cout << "   std::move后: str1 = \"" << str1 << "\", str2 = \"" << str2 << "\"\n\n";
    
    std::cout << "2. std::forward：条件性转换\n";
    std::cout << "   用途：在模板中保持参数的原始值类别\n\n";
    
    std::cout << "   区别总结:\n";
    std::cout << "   ┌────────────────┬─────────────────┬─────────────────┐\n";
    std::cout << "   │     特性       │   std::move     │  std::forward   │\n";
    std::cout << "   ├────────────────┼─────────────────┼─────────────────┤\n";
    std::cout << "   │     目的       │ 无条件转右值    │ 条件性转换      │\n";
    std::cout << "   │     参数       │ 任意类型        │ 需要模板参数T   │\n";
    std::cout << "   │     返回类型   │ 总是 Type&&     │ 取决于T         │\n";
    std::cout << "   │     使用场景   │ 明确要移动      │ 转发保持原类型  │\n";
    std::cout << "   └────────────────┴─────────────────┴─────────────────┘\n";
}

// ==================== 正确使用示例 ====================

// 示例1：正确使用forward的场景
template<typename T>
void correctUsage(T&& param) {
    // 正确：使用std::forward保持值类别
    processObject(std::forward<T>(param));
}

// 示例2：错误使用
void wrongUsage(int&& param) {
    // 错误：param已经是右值引用，不需要forward
    // std::forward<int>(param); // 这是多余的
    
    // 正确：直接使用std::move
    // 或者直接使用，因为在函数内部param是左值
    std::cout << "  wrongUsage: param是右值引用参数，但在函数内是左值\n";
}

// 示例3：make_unique风格的完美转发
template<typename T, typename... Args>
std::unique_ptr<T> makeUnique(Args&&... args) {
    std::cout << "  makeUnique: 完美转发所有参数\n";
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

// 示例4：容器元素添加
template<typename Container, typename Value>
void addToContainer(Container& c, Value&& v) {
    c.push_back(std::forward<Value>(v));
}

// ==================== 实际应用案例 ====================

// 案例1：可调用对象包装器
template<typename Func, typename... Args>
auto invoke(Func&& func, Args&&... args) 
    -> decltype(std::forward<Func>(func)(std::forward<Args>(args)...)) 
{
    return std::forward<Func>(func)(std::forward<Args>(args)...);
}

// 案例2：延迟执行包装器
template<typename Func>
class DeferredTask {
public:
    template<typename F>
    explicit DeferredTask(F&& func) : func_(std::forward<F>(func)) {}
    
    void execute() { func_(); }
    
private:
    Func func_;
};

template<typename Func>
DeferredTask<Func> makeDeferredTask(Func&& func) {
    return DeferredTask<typename std::decay<Func>::type>(std::forward<Func>(func));
}

// ==================== 主演示函数 ====================

namespace cpp11_features {

void stdForwardDemo() {
    std::cout << "【std::forward详解】\n\n";
    
    // ========== 工作原理 ==========
    std::cout << "===== std::forward工作原理 =====\n";
    
    HeavyObject obj("LeftValue");
    
    std::cout << "\n传入左值:\n";
    showForwardBehavior(obj);
    
    std::cout << "\n\n传入右值:\n";
    showForwardBehavior(HeavyObject("RightValue"));
    
    // ========== move vs forward ==========
    demonstrateMoveVsForward();
    
    // ========== 正确使用示例 ==========
    std::cout << "\n===== 正确使用示例 =====\n\n";
    
    std::cout << "1. 使用完美转发创建对象:\n";
    auto ptr = makeUnique<HeavyObject>("CreatedObject");
    
    std::cout << "\n2. 容器元素添加:\n";
    std::vector<HeavyObject> vec;
    HeavyObject temp("VectorElement");
    
    std::cout << "\n添加左值:\n";
    addToContainer(vec, temp);
    
    std::cout << "\n添加右值:\n";
    addToContainer(vec, HeavyObject("TempElement"));
    
    std::cout << "\n3. 可调用对象包装:\n";
    auto add = [](int a, int b) { return a + b; };
    int result = invoke(add, 3, 4);
    std::cout << "  invoke(add, 3, 4) = " << result << std::endl;
    
    std::cout << "\n4. 延迟执行:\n";
    auto task = makeDeferredTask([]() {
        std::cout << "  延迟任务执行中...\n";
    });
    std::cout << "  任务已创建，稍后执行\n";
    task.execute();
    
    std::cout << "\n===== 演示结束 =====\n";
}

} // namespace cpp11_features

// 独立测试入口
#ifdef STANDALONE_TEST
int main() {
    cpp11_features::stdForwardDemo();
    return 0;
}
#endif
