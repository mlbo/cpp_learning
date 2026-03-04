/**
 * @file item08_prefer_nullptr.cpp
 * @brief EMC++条款8: 优先使用nullptr而非0或NULL
 * 
 * Effective Modern C++ by Scott Meyers
 * Item 8: Prefer nullptr to 0 and NULL.
 */

#include <iostream>
#include <memory>
#include <vector>
#include <mutex>
#include <thread>

// ============================================================================
// 条款8核心思想
// ============================================================================

/*
 * 字面常量0是一个int，不是指针！
 * 如果在需要指针的上下文中看到0，虽然C++会将其解释为空指针，
 * 但这只是一个约定，其本质仍是int类型。
 * 
 * NULL的情况更糟，因为其实现不确定（可能是0，可能是0L）
 * 
 * nullptr不是一个int，也不是指针，它是 std::nullptr_t 类型
 * std::nullptr_t 可以隐式转换为任何裸指针类型
 */

// ============================================================================
// 问题1: 函数重载
// ============================================================================

void f(int) {
    std::cout << "  f(int)\n";
}

void f(bool) {
    std::cout << "  f(bool)\n";
}

void f(void*) {
    std::cout << "  f(void*)\n";
}

void demo_problem_overload() {
    std::cout << "═══════════════════════════════════════════════════════════════\n";
    std::cout << "问题1: 函数重载歧义\n";
    std::cout << "═══════════════════════════════════════════════════════════════\n\n";
    
    std::cout << "三个重载版本: f(int), f(bool), f(void*)\n\n";
    
    std::cout << "f(0) 调用: ";
    f(0);  // 调用 f(int)，而非 f(void*)！
    
    std::cout << "f(NULL) 调用: ";
    // f(NULL);  // 编译错误/歧义！NULL可能匹配int/bool/void*
    std::cout << "(编译歧义 - NULL可能匹配多个重载)\n";
    
    std::cout << "f(nullptr) 调用: ";
    f(nullptr);  // 明确调用 f(void*) ✓
    
    std::cout << "\n结论: 只有nullptr能正确表达空指针的语义\n\n";
}

// ============================================================================
// 问题2: 模板类型推导
// ============================================================================

class Widget {
public:
    Widget() : data_(nullptr) {}
    explicit Widget(int* p) : data_(p) { std::cout << "  Widget(int*)\n"; }
    
private:
    int* data_;
};

// 模板函数
template<typename T>
void call_f(T param) {
    // 创建Widget，传入param
    // 如果T是int，这会失败
    // 如果T是int*或nullptr_t，这会成功
    Widget w(param);  // 如果T=int，这里会尝试 Widget(int)，但构造函数需要 int*
}

// 解决方案：使用nullptr
template<typename T>
void call_f_fixed(T param) {
    // 如果需要传递空指针，应该明确使用nullptr
    // C++11写法（注释）:
    // typename std::enable_if<std::is_same<T, std::nullptr_t>::value, void>::type ...
    // typename std::enable_if<!std::is_same<T, std::nullptr_t>::value, void>::type ...
    if constexpr (std::is_null_pointer_v<T>) {
        Widget w(static_cast<int*>(param));  // 明确转换
        std::cout << "  call_f_fixed: 创建Widget(nullptr)\n";
    } else {
        Widget w(param);
        std::cout << "  call_f_fixed: 创建Widget(其他)\n";
    }
}

void demo_problem_template() {
    std::cout << "═══════════════════════════════════════════════════════════════\n";
    std::cout << "问题2: 模板类型推导\n";
    std::cout << "═══════════════════════════════════════════════════════════════\n\n";
    
    // int* p = 0;  // 这在模板中会出问题
    
    std::cout << "使用nullptr:\n";
    call_f_fixed(nullptr);  // T = std::nullptr_t
    
    int x = 42;
    std::cout << "\n使用指针:\n";
    call_f_fixed(&x);  // T = int*
    
    std::cout << "\n问题示例:\n";
    std::cout << "  如果我们写 auto p = 0; 然后 call_f(p);\n";
    std::cout << "  p的类型是int，不是指针！\n";
    std::cout << "  这会导致难以发现的bug\n\n";
}

// ============================================================================
// 问题3: 代码可读性
// ============================================================================

// 辅助函数（前置声明）
int* findValue(int* p);

void demo_readability() {
    std::cout << "═══════════════════════════════════════════════════════════════\n";
    std::cout << "问题3: 代码可读性\n";
    std::cout << "═══════════════════════════════════════════════════════════════\n\n";
    
    std::cout << "比较两种写法:\n\n";
    
    // 不清晰的写法
    auto result1 = findValue(nullptr);  // 返回什么？
    (void)result1;  // 避免未使用变量警告
    
    std::cout << "不清晰:\n";
    std::cout << "  auto result = findValue(0);\n";
    std::cout << "  0是整数还是空指针？需要查看函数签名才能确定\n\n";
    
    // 清晰的写法
    std::cout << "清晰:\n";
    std::cout << "  auto result = findValue(nullptr);\n";
    std::cout << "  nullptr明确表示空指针，无需查看函数签名\n\n";
    
    std::cout << "另一个例子:\n";
    std::cout << "  if (ptr == 0) { ... }    // 令人困惑\n";
    std::cout << "  if (ptr == nullptr) { ... }  // 清晰明了\n\n";
}

// 辅助函数（实现）
int* findValue(int* p) {
    return p;
}

// ============================================================================
// 问题4: 多线程中的0和NULL问题
// ============================================================================

std::mutex m1, m2;

void demo_threading() {
    std::cout << "═══════════════════════════════════════════════════════════════\n";
    std::cout << "问题4: 多线程场景\n";
    std::cout << "═══════════════════════════════════════════════════════════════\n\n";
    
    // C++17: std::scoped_lock
    // 如果使用0或NULL，可能会有歧义
    
    std::cout << "使用nullptr表示'无互斥量':\n";
    std::cout << "  auto lock = std::unique_lock<std::mutex>(nullptr);\n";
    std::cout << "  这是合法的，创建一个不拥有任何互斥量的锁\n\n";
    
    // 实际上unique_lock的构造函数接受 mutex* 或 nullptr_t
    // 使用0或NULL可能导致歧义
}

// ============================================================================
// 问题5: 实际案例分析
// ============================================================================

// 模拟一个实际的函数
void registerCallback(void (*callback)(int), int* userData) {
    if (callback) {
        callback(userData ? *userData : 0);
    }
}

void myCallback(int data) {
    std::cout << "  回调执行，数据: " << data << "\n";
}

void demo_real_case() {
    std::cout << "═══════════════════════════════════════════════════════════════\n";
    std::cout << "问题5: 实际案例分析\n";
    std::cout << "═══════════════════════════════════════════════════════════════\n\n";
    
    int data = 42;
    
    std::cout << "场景: 注册回调函数\n\n";
    
    std::cout << "正确用法:\n";
    registerCallback(myCallback, &data);
    
    std::cout << "\n不使用回调:\n";
    registerCallback(nullptr, nullptr);
    
    std::cout << "\n如果使用0:\n";
    std::cout << "  registerCallback(0, 0);  // 歧义！0是什么？\n";
    std::cout << "  可能被误解为整数参数\n\n";
    
    std::cout << "使用nullptr:\n";
    std::cout << "  registerCallback(nullptr, nullptr);  // 清晰！\n";
    std::cout << "  明确表示空函数指针和空数据指针\n\n";
}

// ============================================================================
// 最佳实践总结
// ============================================================================

void demo_best_practices() {
    std::cout << "═══════════════════════════════════════════════════════════════\n";
    std::cout << "最佳实践总结\n";
    std::cout << "═══════════════════════════════════════════════════════════════\n\n";
    
    std::cout << "╔═════════════════════════════════════════════════════════════╗\n";
    std::cout << "║ 规则: 始终使用nullptr表示空指针                            ║\n";
    std::cout << "╠═════════════════════════════════════════════════════════════╣\n";
    std::cout << "║                                                             ║\n";
    std::cout << "║ ❌ 避免: int* p = 0;                                        ║\n";
    std::cout << "║ ✅ 使用: int* p = nullptr;                                  ║\n";
    std::cout << "║                                                             ║\n";
    std::cout << "║ ❌ 避免: if (ptr == NULL)                                   ║\n";
    std::cout << "║ ✅ 使用: if (ptr == nullptr)                                ║\n";
    std::cout << "║                                                             ║\n";
    std::cout << "║ ❌ 避免: func(0);  // 期望传空指针                          ║\n";
    std::cout << "║ ✅ 使用: func(nullptr);                                     ║\n";
    std::cout << "║                                                             ║\n";
    std::cout << "║ ❌ 避免: return 0;  // 返回指针类型                         ║\n";
    std::cout << "║ ✅ 使用: return nullptr;                                    ║\n";
    std::cout << "║                                                             ║\n";
    std::cout << "╚═════════════════════════════════════════════════════════════╝\n\n";
    
    std::cout << "原因:\n";
    std::cout << "  1. nullptr有明确的类型 (std::nullptr_t)\n";
    std::cout << "  2. 可以正确参与重载决议\n";
    std::cout << "  3. 在模板中正确推导类型\n";
    std::cout << "  4. 代码意图更清晰\n";
    std::cout << "  5. 避免0和NULL的平台差异\n\n";
}

// ============================================================================
// 主函数
// ============================================================================

void run_item08_demo() {
    demo_problem_overload();
    demo_problem_template();
    demo_readability();
    demo_threading();
    demo_real_case();
    demo_best_practices();
    
    std::cout << "═══════════════════════════════════════════════════════════════\n";
    std::cout << "EMC++ 条款8 演示完成\n";
    std::cout << "Prefer nullptr to 0 and NULL\n";
    std::cout << "═══════════════════════════════════════════════════════════════\n";
}
