/**
 * @file nullptr_overload.cpp
 * @brief nullptr在函数重载中的优势演示
 */

#include <iostream>
#include <string>
#include <memory>
#include <cstddef>

// ============================================================================
// 示例1: 基本重载问题解决
// ============================================================================

// 三个重载版本
void process(int value) {
    std::cout << "  process(int): " << value << "\n";
}

void process(const char* str) {
    std::cout << "  process(const char*): " << (str ? str : "null") << "\n";
}

void process(void* ptr) {
    std::cout << "  process(void*): " << ptr << "\n";
}

// 专门处理nullptr的重载
void process(std::nullptr_t) {
    std::cout << "  process(nullptr_t): 空指针\n";
}

void demo_basic_overload() {
    std::cout << "═══════════════════════════════════════════════════════════════\n";
    std::cout << "示例1: 基本重载问题\n";
    std::cout << "═══════════════════════════════════════════════════════════════\n\n";
    
    std::cout << "调用 process(0):\n";
    process(0);  // 调用 int 版本
    
    std::cout << "调用 process(nullptr):\n";
    process(nullptr);  // 调用 void* 版本
    
    std::cout << "调用 process(\"hello\"):\n";
    process("hello");  // 调用 const char* 版本
    
    std::cout << "\n";
}

// ============================================================================
// 示例2: 使用std::nullptr_t进行精确控制
// ============================================================================

class Resource {
public:
    Resource() : ptr_(nullptr) {}
    explicit Resource(int* p) : ptr_(p) {}
    
    // 专门处理nullptr的构造函数
    Resource(std::nullptr_t) : ptr_(nullptr) {
        std::cout << "  调用nullptr_t构造函数\n";
    }
    
    // 专门处理nullptr的赋值运算符
    Resource& operator=(std::nullptr_t) {
        std::cout << "  调用nullptr_t赋值运算符\n";
        ptr_ = nullptr;
        return *this;
    }
    
    bool isNull() const { return ptr_ == nullptr; }
    
private:
    int* ptr_;
};

void demo_nullptr_t_overload() {
    std::cout << "═══════════════════════════════════════════════════════════════\n";
    std::cout << "示例2: 使用std::nullptr_t精确控制\n";
    std::cout << "═══════════════════════════════════════════════════════════════\n\n";
    
    std::cout << "创建 Resource r1(nullptr):\n";
    Resource r1(nullptr);
    
    std::cout << "\n创建 Resource r2:\n";
    Resource r2;
    
    std::cout << "\nr1 = nullptr:\n";
    r1 = nullptr;
    
    std::cout << "\n这种技术可以用来:\n";
    std::cout << "  - 记录空指针赋值\n";
    std::cout << "  - 执行特殊的清理逻辑\n";
    std::cout << "  - 区分'默认构造'和'显式赋空'\n\n";
}

// ============================================================================
// 示例3: 智能指针与nullptr
// ============================================================================

void useResource(std::shared_ptr<int> ptr) {
    if (ptr) {
        std::cout << "  使用资源: " << *ptr << "\n";
    } else {
        std::cout << "  空资源\n";
    }
}

void demo_smart_pointer_overload() {
    std::cout << "═══════════════════════════════════════════════════════════════\n";
    std::cout << "示例3: 智能指针与nullptr\n";
    std::cout << "═══════════════════════════════════════════════════════════════\n\n";
    
    std::cout << "传入 std::make_shared<int>(42):\n";
    useResource(std::make_shared<int>(42));
    
    std::cout << "\n传入 nullptr:\n";
    useResource(nullptr);  // shared_ptr有接受nullptr_t的构造函数
    
    std::cout << "\n传入 shared_ptr():\n";
    useResource(std::shared_ptr<int>());  // 空shared_ptr
    
    std::cout << "\n";
}

// ============================================================================
// 示例4: 模板与nullptr的重载解析
// ============================================================================

// 函数模板
template<typename T>
void handle(T arg) {
    std::cout << "  模板版本: 类型=" << typeid(T).name() << "\n";
}

// nullptr_t的特化
template<>
void handle<std::nullptr_t>(std::nullptr_t) {
    std::cout << "  nullptr_t特化版本\n";
}

// 指针重载
template<typename T>
void handle(T* arg) {
    std::cout << "  指针版本: 类型=" << typeid(T).name() << "*\n";
}

void demo_template_overload() {
    std::cout << "═══════════════════════════════════════════════════════════════\n";
    std::cout << "示例4: 模板与nullptr的重载解析\n";
    std::cout << "═══════════════════════════════════════════════════════════════\n\n";
    
    int value = 10;
    
    std::cout << "handle(42):\n";
    handle(42);
    
    std::cout << "\nhandle(&value):\n";
    handle(&value);
    
    std::cout << "\nhandle(nullptr):\n";
    handle(nullptr);  // 调用nullptr_t特化版本
    
    std::cout << "\nhandle((int*)nullptr):\n";
    handle((int*)nullptr);  // 调用指针版本
    
    std::cout << "\n";
}

// ============================================================================
// 示例5: 可变参数模板中的nullptr处理
// ============================================================================

// 递归终止
void printArgs() {
    std::cout << "  (结束)\n";
}

// 可变参数模板
template<typename T, typename... Args>
void printArgs(T first, Args... rest) {
    // C++11写法（注释）:
    // 用重载 + std::enable_if/std::is_pointer<std::remove_pointer<...>::type> 实现分派
    if constexpr (std::is_same_v<T, std::nullptr_t>) {
        std::cout << "  [nullptr] ";
    } else if constexpr (std::is_pointer_v<T>) {
        // 对于const指针需要特殊处理
        if constexpr (std::is_const_v<std::remove_pointer_t<T>>) {
            std::cout << "  [const指针] ";
        } else {
            std::cout << "  [指针: " << static_cast<void*>(first) << "] ";
        }
    } else {
        std::cout << "  [" << first << "] ";
    }
    printArgs(rest...);
}

void demo_variadic_template() {
    std::cout << "═══════════════════════════════════════════════════════════════\n";
    std::cout << "示例5: 可变参数模板中的nullptr处理\n";
    std::cout << "═══════════════════════════════════════════════════════════════\n\n";
    
    int x = 10;
    
    std::cout << "printArgs(1, 2.0, \"hello\", nullptr, &x):\n";
    printArgs(1, 2.0, "hello", nullptr, &x);
    
    std::cout << "\n";
}

// ============================================================================
// 示例6: 重载决策优先级
// ============================================================================

void func(int) {
    std::cout << "  func(int)\n";
}

void func(long) {
    std::cout << "  func(long)\n";
}

void func(int*) {
    std::cout << "  func(int*)\n";
}

void func(void*) {
    std::cout << "  func(void*)\n";
}

void func(std::nullptr_t) {
    std::cout << "  func(std::nullptr_t)\n";
}

void demo_overload_resolution() {
    std::cout << "═══════════════════════════════════════════════════════════════\n";
    std::cout << "示例6: 重载决策优先级\n";
    std::cout << "═══════════════════════════════════════════════════════════════\n\n";
    
    std::cout << "重载函数集合: func(int), func(long), func(int*), func(void*), func(nullptr_t)\n\n";
    
    std::cout << "func(0):       "; func(0);
    std::cout << "func(0L):      "; func(0L);
    std::cout << "func(nullptr): "; func(nullptr);
    
    std::cout << "\n分析:\n";
    std::cout << "  - 0: 精确匹配int\n";
    std::cout << "  - 0L: 精确匹配long\n";
    std::cout << "  - nullptr: 精确匹配nullptr_t (优先级最高)\n\n";
}

// ============================================================================
// 示例7: 链式调用中的nullptr
// ============================================================================

class Builder {
public:
    Builder& setName(const char* name) {
        name_ = name ? name : "unnamed";
        std::cout << "  设置名称: " << name_ << "\n";
        return *this;
    }
    
    Builder& setData(int* data) {
        data_ = data;
        std::cout << "  设置数据: " << (data ? "有效" : "空") << "\n";
        return *this;
    }
    
    Builder& setNull(std::nullptr_t = nullptr) {
        data_ = nullptr;
        name_ = "null";
        std::cout << "  重置为空\n";
        return *this;
    }
    
private:
    int* data_ = nullptr;
    std::string name_;
};

void demo_chain_call() {
    std::cout << "═══════════════════════════════════════════════════════════════\n";
    std::cout << "示例7: 链式调用中的nullptr\n";
    std::cout << "═══════════════════════════════════════════════════════════════\n\n";
    
    int data = 42;
    
    std::cout << "Builder模式链式调用:\n";
    Builder()
        .setName("Test")
        .setData(&data)
        .setNull()  // 使用默认的nullptr
        .setName(nullptr ? "non-null" : "was-null");
    
    std::cout << "\n";
}

// ============================================================================
// 主函数
// ============================================================================
void run_nullptr_overload() {
    demo_basic_overload();
    demo_nullptr_t_overload();
    demo_smart_pointer_overload();
    demo_template_overload();
    demo_variadic_template();
    demo_overload_resolution();
    demo_chain_call();
    
    std::cout << "═══════════════════════════════════════════════════════════════\n";
    std::cout << "nullptr在函数重载中的优势演示完成\n";
    std::cout << "═══════════════════════════════════════════════════════════════\n";
}
