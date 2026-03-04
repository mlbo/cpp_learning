/**
 * @file item28_perfect_forward.cpp
 * @brief EMC++ Item 28: 理解引用折叠的上下文
 * 
 * 引用折叠发生在四种上下文中：
 * 1. 模板实例化
 * 2. auto 类型推导
 * 3. typedef/using 别名声明
 * 4. decltype 表达式
 * 
 * 本文件详细演示这四种上下文
 */

#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
#include <memory>

// ==================== 辅助工具 ====================

template<typename T>
const char* getTypeName() {
    if constexpr (std::is_lvalue_reference_v<T>) {
        return "左值引用";
    } else if constexpr (std::is_rvalue_reference_v<T>) {
        return "右值引用";
    } else {
        return "非引用类型";
    }
}

#define SHOW_TYPE(expr) \
    std::cout << "  " << #expr << " → " << getTypeName<decltype(expr)>() << "\n"

// ==================== 上下文 1: 模板实例化 ====================

/**
 * @brief 演示模板实例化中的引用折叠
 * 
 * 这是最常见的引用折叠场景：
 * - 模板参数 T 被推导为引用类型
 * - T&& 触发引用折叠
 */
template<typename T>
struct TemplateContext {
    using Type = T;
    using RefType = T&&;
    
    static void analyze(T&& arg) {
        std::cout << "  T = " << getTypeName<T>() << "\n";
        std::cout << "  T&& = " << getTypeName<T&&>() << "\n";
    }
};

// 特化：显式指定引用类型
template<>
struct TemplateContext<int&> {
    using Type = int&;
    using RefType = int&;  // int& && → int&
    
    static void analyze(int& arg) {
        std::cout << "  特化版本: T = int&\n";
        std::cout << "  T&& = int& (折叠结果)\n";
    }
};

void demonstrateTemplateContext() {
    std::cout << "\n=== 上下文 1: 模板实例化 ===\n";
    
    int x = 42;
    
    std::cout << "\n传入左值 (T 推导为 int&):\n";
    TemplateContext<int&>::analyze(x);
    
    std::cout << "\n传入右值 (T 推导为 int):\n";
    TemplateContext<int>::analyze(42);
    
    std::cout << "\n使用 using 声明观察类型:\n";
    using LValueType = TemplateContext<int&>::RefType;
    using RValueType = TemplateContext<int>::RefType;
    
    std::cout << "  int&::RefType = " << getTypeName<LValueType>() << "\n";
    std::cout << "  int::RefType = " << getTypeName<RValueType>() << "\n";
}

// ==================== 上下文 2: auto 类型推导 ====================

/**
 * @brief 演示 auto 类型推导中的引用折叠
 * 
 * auto&& 是通用引用，遵循与 T&& 相同的推导规则
 */
void demonstrateAutoContext() {
    std::cout << "\n=== 上下文 2: auto 类型推导 ===\n";
    
    int x = 10;
    int& lr = x;
    int&& rr = 20;  // 注意：rr 本身是左值！
    
    std::cout << "\n原始变量类型:\n";
    SHOW_TYPE(x);      // int
    SHOW_TYPE(lr);     // int&
    SHOW_TYPE(rr);     // int&& (变量声明类型)
    
    std::cout << "\nauto&& 通用引用推导:\n";
    
    auto&& ref1 = x;           // 左值 → auto = int&, ref1 = int&
    auto&& ref2 = lr;          // 左值 → auto = int&, ref2 = int&
    auto&& ref3 = 30;          // 右值 → auto = int, ref3 = int&&
    auto&& ref4 = std::move(x); // 右值 → auto = int, ref4 = int&&
    auto&& ref5 = rr;          // rr 是左值！→ auto = int&, ref5 = int&
    
    SHOW_TYPE(ref1);
    SHOW_TYPE(ref2);
    SHOW_TYPE(ref3);
    SHOW_TYPE(ref4);
    SHOW_TYPE(ref5);
    
    std::cout << "\n注意：命名右值引用变量本身是左值!\n";
    std::cout << "  int&& rr = 20; 之后，rr 作为表达式是左值\n";
    std::cout << "  所以 auto&& ref5 = rr; 推导为 int&\n";
    
    // 范围 for 中的 auto&&
    std::cout << "\n范围 for 中的 auto&&:\n";
    std::vector<int> vec = {1, 2, 3};
    
    for (auto&& elem : vec) {
        // elem 类型根据 vec 元素类型推导
        SHOW_TYPE(elem);
        break;  // 只演示第一个
    }
}

// ==================== 上下文 3: typedef/using 别名声明 ====================

/**
 * @brief 演示 using/typedef 别名中的引用折叠
 */
void demonstrateAliasContext() {
    std::cout << "\n=== 上下文 3: typedef/using 别名声明 ===\n";
    
    // 基础类型
    using Int = int;
    using IntLRef = int&;
    using IntRRef = int&&;
    
    std::cout << "\n基础别名:\n";
    std::cout << "  Int = " << getTypeName<Int>() << "\n";
    std::cout << "  IntLRef = " << getTypeName<IntLRef>() << "\n";
    std::cout << "  IntRRef = " << getTypeName<IntRRef>() << "\n";
    
    // 在别名上添加引用
    std::cout << "\n在别名上添加 &:\n";
    using Int_AddL = Int&;           // int & → int&
    using IntLRef_AddL = IntLRef&;   // int& & → int&
    using IntRRef_AddL = IntRRef&;   // int&& & → int&
    
    std::cout << "  Int& = " << getTypeName<Int_AddL>() << "\n";
    std::cout << "  IntLRef& (int& &) = " << getTypeName<IntLRef_AddL>() << "\n";
    std::cout << "  IntRRef& (int&& &) = " << getTypeName<IntRRef_AddL>() << "\n";
    
    std::cout << "\n在别名上添加 &&:\n";
    using Int_AddR = Int&&;          // int && → int&&
    using IntLRef_AddR = IntLRef&&;  // int& && → int&
    using IntRRef_AddR = IntRRef&&;  // int&& && → int&&
    
    std::cout << "  Int&& = " << getTypeName<Int_AddR>() << "\n";
    std::cout << "  IntLRef&& (int& &&) = " << getTypeName<IntLRef_AddR>() << "\n";
    std::cout << "  IntRRef&& (int&& &&) = " << getTypeName<IntRRef_AddR>() << "\n";
    
    // 模板别名中的折叠
    std::cout << "\n模板别名中的折叠:\n";
    
    // 直接演示引用折叠效果（模板别名需在全局定义）
    // AddLRef<int&> = int& & → int&
    // AddRRef<int&> = int& && → int&
    // AddLRef<int&&> = int&& & → int&
    // AddRRef<int&&> = int&& && → int&&
    
    std::cout << "  AddLRef<int&> = " << "左值引用 (int& & → int&)" << "\n";
    std::cout << "  AddRRef<int&> = " << "左值引用 (int& && → int&)" << "\n";
    std::cout << "  AddLRef<int&&> = " << "左值引用 (int&& & → int&)" << "\n";
    std::cout << "  AddRRef<int&&> = " << "右值引用 (int&& && → int&&)" << "\n";
}

// ==================== 上下文 4: decltype 表达式 ====================

/**
 * @brief 演示 decltype 中的引用折叠
 */
void demonstrateDecltypeContext() {
    std::cout << "\n=== 上下文 4: decltype 表达式 ===\n";
    
    int x = 42;
    int& lr = x;
    
    std::cout << "\ndecltype 基本推导:\n";
    
    // decltype(x) 推导为 int
    // decltype((x)) 推导为 int&（带括号的左值表达式）
    using Type1 = decltype(x);
    using Type2 = decltype((x));
    using Type3 = decltype(lr);
    using Type4 = decltype(std::move(x));
    
    std::cout << "  decltype(x) = " << getTypeName<Type1>() << "\n";
    std::cout << "  decltype((x)) = " << getTypeName<Type2>() << "\n";
    std::cout << "  decltype(lr) = " << getTypeName<Type3>() << "\n";
    std::cout << "  decltype(std::move(x)) = " << getTypeName<Type4>() << "\n";
    
    std::cout << "\n在 decltype 结果上添加引用:\n";
    
    using DT1 = decltype(x)&;        // int & → int&
    using DT2 = decltype((x))&;      // int& & → int&
    using DT3 = decltype(lr)&;       // int& & → int&
    using DT4 = decltype(std::move(x))&;  // int&& & → int&
    
    std::cout << "  decltype(x)& = " << getTypeName<DT1>() << "\n";
    std::cout << "  decltype((x))& = " << getTypeName<DT2>() << "\n";
    std::cout << "  decltype(lr)& = " << getTypeName<DT3>() << "\n";
    std::cout << "  decltype(std::move(x))& = " << getTypeName<DT4>() << "\n";
    
    // 实际应用：返回类型推导
    std::cout << "\n返回类型推导中的应用:\n";
    
    auto getRef = [&]() -> decltype((x)) {
        return x;  // 返回 int&
    };
    
    auto getVal = [&]() -> decltype(x) {
        return x;  // 返回 int
    };
    
    auto getRRef = [&]() -> decltype(std::move(x)) {
        return std::move(x);  // 返回 int&&
    };
    
    SHOW_TYPE(getRef());
    SHOW_TYPE(getVal());
    SHOW_TYPE(getRRef());
}

// ==================== 综合应用：完美转发工厂 ====================

/**
 * @brief 完美转发工厂示例
 * 
 * 展示引用折叠在实际编程中的应用
 */
class Resource {
public:
    Resource() : name_("default"), data_(0) {
        std::cout << "  Resource 默认构造\n";
    }
    
    Resource(std::string name, int data) 
        : name_(std::move(name)), data_(data) {
        std::cout << "  Resource 构造: " << name_ << "\n";
    }
    
    Resource(const Resource& other) 
        : name_(other.name_), data_(other.data_) {
        std::cout << "  Resource 拷贝构造\n";
    }
    
    Resource(Resource&& other) noexcept
        : name_(std::move(other.name_)), data_(other.data_) {
        other.data_ = 0;
        std::cout << "  Resource 移动构造\n";
    }
    
    void print() const {
        std::cout << "  Resource[" << name_ << ", data=" << data_ << "]\n";
    }
    
private:
    std::string name_;
    int data_;
};

/**
 * @brief 完美转发工厂函数
 * 
 * 使用通用引用 + std::forward 实现高效构造
 */
template<typename T, typename... Args>
std::unique_ptr<T> create(Args&&... args) {
    std::cout << "  创建对象...\n";
    return std::make_unique<T>(std::forward<Args>(args)...);
}

void demonstratePerfectForward() {
    std::cout << "\n=== 综合应用：完美转发工厂 ===\n";
    
    // 使用左值参数
    std::string name = "MyResource";
    auto r1 = create<Resource>(name, 100);
    r1->print();
    
    // 使用右值参数
    auto r2 = create<Resource>(std::string("TempResource"), 200);
    r2->print();
    
    // 拷贝构造
    Resource original("Original", 300);
    auto r3 = create<Resource>(original);
    r3->print();
    
    // 移动构造
    auto r4 = create<Resource>(std::move(original));
    r4->print();
}

// ==================== 主演示函数 ====================

void runItem28Demo() {
    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "   EMC++ Item 28: 理解引用折叠的上下文\n";
    std::cout << "========================================\n";
    
    // 四种上下文
    demonstrateTemplateContext();
    demonstrateAutoContext();
    demonstrateAliasContext();
    demonstrateDecltypeContext();
    
    // 综合应用
    demonstratePerfectForward();
    
    std::cout << "\n========================================\n";
    std::cout << "   Item 28 演示完成\n";
    std::cout << "========================================\n";
    
    std::cout << "\n关键要点:\n";
    std::cout << "  1. 引用折叠发生在四种上下文\n";
    std::cout << "     - 模板实例化 (T&&)\n";
    std::cout << "     - auto 类型推导 (auto&&)\n";
    std::cout << "     - typedef/using 别名\n";
    std::cout << "     - decltype 表达式\n";
    std::cout << "  2. 规则始终一致：有左值引用 → 左值引用\n";
    std::cout << "  3. 理解这些上下文是掌握完美转发的关键\n";
}
