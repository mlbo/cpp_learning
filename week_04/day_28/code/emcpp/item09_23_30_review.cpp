/**
 * @file item09_23_30_review.cpp
 * @brief EMC++ Item 9, 23-30 条款复习示例
 * 
 * 涵盖内容：
 * - Item 9: 优先使用别名声明而非typedef
 * - Item 23: 理解std::move和std::forward
 * - Item 24: 区分转发引用和右值引用
 * - Item 25: 对右值引用使用std::move，对转发引用使用std::forward
 * - Item 26: 避免在转发引用上重载
 * - Item 27: 熟悉转发引用重载的替代方案
 * - Item 28: 理解引用折叠
 * - Item 29: 假定移动不存在、不便宜、未被使用
 * - Item 30: 熟悉完美转发失败的情况
 */

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <functional>
#include <type_traits>
#include <array>
#include <initializer_list>

namespace emcpp_review {

// ========================================
// Item 9: 优先使用别名声明而非typedef
// ========================================

// 模板别名定义（必须在namespace级别）
template <typename T>
using MyVector = std::vector<T>;

template <typename T>
struct LegacyVectorMeta {
    typedef std::vector<T> type;
};

template <typename T>
using RemoveConstReferenceT = std::remove_const_t<std::remove_reference_t<T>>;

void demonstrateItem09() {
    std::cout << "=== Item 9: 别名声明 vs typedef ===\n\n";

    // 传统typedef
    typedef std::unique_ptr<std::vector<int>> IntVectorPtr_typedef;
    IntVectorPtr_typedef legacyPtr;
    
    // C++11别名声明（推荐）
    using IntVectorPtr = std::unique_ptr<std::vector<int>>;
    
    // 使用上效果相同
    IntVectorPtr ptr1 = std::make_unique<std::vector<int>>();
    ptr1->push_back(1);
    std::cout << "别名声明创建的vector大小: " << ptr1->size() << "\n";
    
    // 别名声明的优势：alias template 可直接产生目标类型；
    // 旧式 typedef 方案需要类模板和嵌套 type。
    MyVector<int> vec = {1, 2, 3};
    std::cout << "模板别名创建的vector: ";
    for (int x : vec) std::cout << x << " ";
    std::cout << "\n";
    
    // 函数指针别名
    using Callback = void(*)(int);
    Callback callback = nullptr;
    (void)legacyPtr;
    (void)callback;
    // typedef void(*Callback_typedef)(int);  // 等价但更难读
    
    std::cout << "\n别名声明的优势:\n";
    std::cout << "  1. 语法更清晰直观\n";
    std::cout << "  2. alias template 可直接产生目标类型；typedef 只能借助类模板的嵌套 type\n";
    std::cout << "  3. 与 C++14 的 _t 风格相同，可少写 typename 和 ::type\n";

    typename LegacyVectorMeta<int>::type legacyVector{4, 5};
    RemoveConstReferenceT<const int&> plain = 6;
    static_assert(std::is_same_v<decltype(plain), int>);
    std::cout << "  旧式元函数结果大小: " << legacyVector.size()
              << "，别名模板去限定结果: " << plain << "\n";
}

// ========================================
// Item 23: 理解std::move和std::forward
// ========================================

void demonstrateItem23() {
    std::cout << "\n=== Item 23: std::move和std::forward ===\n\n";
    
    std::cout << "std::move的本质:\n";
    std::cout << "  - 只是一个类型转换（static_cast<T&&>）\n";
    std::cout << "  - 不生成任何代码\n";
    std::cout << "  - 产生 xvalue，允许后续重载选择移动操作，但不保证一定移动\n\n";
    
    std::string s1 = "Hello";
    std::string s2 = std::move(s1);
    
    std::cout << "std::move后:\n";
    std::cout << "  s1: \"" << s1 << "\" (仍有效，但具体状态未指定)\n";
    std::cout << "  s2: \"" << s2 << "\"\n\n";
    
    std::cout << "std::forward的本质:\n";
    std::cout << "  - 条件转换\n";
    std::cout << "  - 保持参数的原始值类别\n";
    std::cout << "  - 主要用于完美转发\n";
    
    std::cout << "  - 命名的右值引用参数表达式仍是左值，转发时不能直接传 x\n";
}

// ========================================
// Item 24: 区分转发引用和右值引用
// ========================================

void demonstrateItem24() {
    std::cout << "\n=== Item 24: 转发引用 vs 右值引用 ===\n\n";
    
    // 右值引用示例
    auto rvalueRef = [](std::string&& s) {
        std::cout << "  右值引用: " << s << "\n";
    };
    
    // 转发引用示例
    auto forwardingRef = [](auto&& s) {
        std::cout << "  转发引用: " << s << "\n";
    };
    
    std::string str = "Hello";
    
    std::cout << "右值引用只能绑定右值:\n";
    rvalueRef(std::string("Temp"));
    // rvalueRef(str);  // 编译错误！
    
    std::cout << "\n转发引用可以绑定左值和右值:\n";
    forwardingRef(str);                      // 绑定左值
    forwardingRef(std::string("Temp"));      // 绑定右值
    
    std::cout << "\n识别方法:\n";
    std::cout << "  1. 被推导且未加 cv 的模板参数 T 之 T&& 形参 → 转发引用\n";
    std::cout << "  2. auto&& 从普通表达式推导时是转发引用，直接大括号列表是例外\n";
    std::cout << "  3. 类型已知的 X&& → 右值引用\n";
}

// ========================================
// Item 25: std::move vs std::forward的使用
// ========================================

class Widget {
private:
    std::string name_;
    
public:
    // 对右值引用参数使用std::move
    void setName(std::string&& name) {
        name_ = std::move(name);
        std::cout << "  [右值引用] setName: " << name_ << "\n";
    }
    
    // 对转发引用参数使用std::forward
    template<typename T>
    void setNameUniversal(T&& name) {
        name_ = std::forward<T>(name);
        std::cout << "  [转发引用] setName: " << name_ << "\n";
    }
    
    const std::string& name() const { return name_; }
};

enum class ForwardingRoute {
    Lvalue,
    Rvalue
};

ForwardingRoute receiveName(const std::string&) {
    std::cout << "  目标重载收到左值\n";
    return ForwardingRoute::Lvalue;
}

ForwardingRoute receiveName(std::string&&) {
    std::cout << "  目标重载收到右值\n";
    return ForwardingRoute::Rvalue;
}

template<typename T>
ForwardingRoute forwardName(T&& name) {
    return receiveName(std::forward<T>(name));
}

void demonstrateItem25() {
    std::cout << "\n=== Item 25: 正确使用std::move和std::forward ===\n\n";
    
    Widget w;
    
    std::cout << "右值引用参数使用std::move:\n";
    w.setName(std::string("RightValue"));
    
    std::cout << "\n转发引用参数使用std::forward:\n";
    std::string name = "LeftValue";
    w.setNameUniversal(name);
    w.setNameUniversal(std::string("RightValue2"));
    forwardName(name);
    forwardName(std::string("Temporary"));
    
    std::cout << "\n规则总结:\n";
    std::cout << "  - 右值引用参数 → std::move\n";
    std::cout << "  - 转发引用参数 → std::forward\n";
    std::cout << "  - 返回值优化场景 → 都不用\n";
}

// ========================================
// Item 26-27: 避免在转发引用上重载
// ========================================

// 错误示例：转发引用与重载的组合问题
class BadStringSetter {
private:
    std::string name_;
    
public:
    // 重载1：普通参数
    void setName(const std::string& name) {
        name_ = name;
        std::cout << "  [const&] setName: " << name_ << "\n";
    }
    
    // 重载2：转发引用（会匹配很多不同类型）
    template<typename T>
    void setName(T&& name) {
        name_ = std::forward<T>(name);
        std::cout << "  [转发引用] setName: " << name_ << "\n";
    }
};

// 正确方案：使用标签分发或限制模板
enum class SetterRoute {
    Name,
    Index
};

class GoodStringSetter {
private:
    std::string name_;
    
public:
    // 只有能构造业务名称、且不是类自身的类型，才允许模板参与候选集。
    template<typename T,
             std::enable_if_t<
                 std::is_constructible_v<std::string, T&&> &&
                 !std::is_same_v<std::decay_t<T>, GoodStringSetter>, int> = 0>
    SetterRoute setName(T&& name) {
        name_ = std::forward<T>(name);
        std::cout << "  [受限转发引用] setName: " << name_ << "\n";
        return SetterRoute::Name;
    }
    
    SetterRoute setName(int index) {
        name_ = "Index_" + std::to_string(index);
        std::cout << "  [int重载] setName: " << name_ << "\n";
        return SetterRoute::Index;
    }
};

void demonstrateItem26_27() {
    std::cout << "\n=== Item 26-27: 转发引用与重载 ===\n\n";
    
    std::cout << "Item 26 问题：转发引用常给出无需 const 转换的精确匹配\n";
    std::cout << "  非 const string 左值和字符串字面量都可能绕过预期的 const string& 重载\n\n";
    
    BadStringSetter bad;
    std::cout << "BadStringSetter:\n";
    std::string nonConstName = "Hello";
    bad.setName(nonConstName);           // 模板可绑定为 string&，比 const string& 少一次限定转换
    bad.setName("World");                // 模板可直接绑定数组类型
    
    std::cout << "\n解决方案:\n";
    std::cout << "  1. 放弃重载（使用不同函数名）\n";
    std::cout << "  2. 使用 const T&，接口简单但不专门利用右值\n";
    std::cout << "  3. 按值接收再移动，适合本来就要保存一份副本的参数\n";
    std::cout << "  4. 标签分发，把分类判断与实际处理分开\n";
    std::cout << "  5. SFINAE/约束模板，只让合法类型进入候选集\n";
    
    std::cout << "\nGoodStringSetter示例:\n";
    GoodStringSetter good;
    std::string s = "Test";
    good.setName(s);
    good.setName(std::string("Move"));
    good.setName(42);
    short shortIndex = 7;
    good.setName(shortIndex);  // 名称模板不可行，short 提升到 int 重载
    std::cout << "  short 与 int 都走索引语义，字符串类型才走名称模板\n";
}

// ========================================
// Item 28: 引用折叠
// ========================================

// 辅助模板函数：类型推导演示
// 必须在namespace级别定义
template <typename T>
const char* deduceType(T&&) {
    if constexpr (std::is_lvalue_reference_v<T>) {
        return "左值引用";
    } else {
        return "右值引用";
    }
}

void demonstrateItem28() {
    std::cout << "\n=== Item 28: 引用折叠规则 ===\n\n";
    
    std::cout << "引用折叠规则:\n";
    std::cout << "  T& &   → T&\n";
    std::cout << "  T& &&  → T&\n";
    std::cout << "  T&& &  → T&\n";
    std::cout << "  T&& && → T&&\n\n";
    
    std::cout << "简化记忆：只有 && && 才会折叠为 &&\n\n";
    
    int x = 10;
    int& lr = x;
    
    std::cout << "类型推导结果:\n";
    std::cout << "  int x = 10; deduceType(x) → " << deduceType(x) << "\n";
    std::cout << "  int& lr = x; deduceType(lr) → " << deduceType(lr) << "\n";
    std::cout << "  deduceType(10) → " << deduceType(10) << "\n";
}

// ========================================
// Item 29: 假定移动不存在、不便宜、未被使用
// ========================================

class CopyOnly {
public:
    CopyOnly() = default;
    CopyOnly(const CopyOnly&) {
        std::cout << "  CopyOnly 从右值初始化时仍调用复制构造\n";
    }
};

class MoveAudit {
public:
    inline static int copies = 0;
    inline static int moves = 0;

    MoveAudit() = default;
    MoveAudit(const MoveAudit&) { ++copies; }
    MoveAudit(MoveAudit&&) { ++moves; } // 故意不标 noexcept
};

void demonstrateItem29() {
    std::cout << "\n=== Item 29: 假定移动不存在、不便宜、未被使用 ===\n\n";
    
    std::cout << "先写出三种反例，再决定是否把性能归功于移动:\n";
    std::cout << "  1. 移动不存在：类型没有移动操作时，右值仍可能绑定 const& 并复制\n";
    std::cout << "  2. 移动不便宜：std::array 必须逐元素处理，短字符串还可能使用 SSO\n";
    std::cout << "  3. 移动未被使用：const 对象不能交给通常接收 T&& 的移动构造；容器迁移还会考虑 noexcept\n\n";

    CopyOnly source;
    CopyOnly copiedFromRvalue(std::move(source));
    (void)copiedFromRvalue;
    
    // 小型数组：移动不比拷贝快
    std::array<int, 5> arr1 = {1, 2, 3, 4, 5};
    auto arr2 = std::move(arr1);  // 实际上是元素级拷贝
    
    std::cout << "std::array的移动:\n";
    std::cout << "  小型array的移动 = 元素级拷贝\n";
    std::cout << "  arr1仍包含: ";
    for (int x : arr1) std::cout << x << " ";
    std::cout << "；目标首元素: " << arr2.front();
    std::cout << "\n\n";

    const std::string constText = "const source";
    std::string copiedText = std::move(constText);
    std::cout << "const string 经 std::move 后目标内容: " << copiedText
              << "（通常选择复制构造）\n";

    MoveAudit::copies = 0;
    MoveAudit::moves = 0;
    std::vector<MoveAudit> audits;
    audits.reserve(1);
    audits.emplace_back();
    audits.emplace_back();
    std::cout << "vector 扩容迁移旧元素: copies=" << MoveAudit::copies
              << ", moves=" << MoveAudit::moves
              << "（可复制且移动可能抛异常时，容器可选择复制）\n\n";
    
    std::cout << "最佳实践:\n";
    std::cout << "  1. 在泛型代码中先按移动可能等同复制来保证复杂度判断可靠\n";
    std::cout << "  2. 对具体类型与具体实现测量，不把 SSO 阈值写成标准保证\n";
    std::cout << "  3. 保持异常安全并让编译器处理返回值优化\n";
}

// ========================================
// Item 30: 完美转发失败的情况
// ========================================

void consumeVector(const std::vector<int>& values) {
    std::cout << "   目标函数收到 vector，元素数=" << values.size() << "\n";
}

void consumePointer(int* pointer) {
    std::cout << "   目标函数收到 " << (pointer == nullptr ? "nullptr" : "非空指针") << "\n";
}

enum class CallbackRoute {
    None,
    Overload,
    FunctionTemplate
};

CallbackRoute lastCallbackRoute = CallbackRoute::None;

void overloaded(int) {
    lastCallbackRoute = CallbackRoute::Overload;
}
void overloaded(double) {}

template<typename T>
void functionTemplate(T) {
    lastCallbackRoute = CallbackRoute::FunctionTemplate;
}

void consumeCallback(void (*callback)(int)) {
    callback(0);
    std::cout << "   目标函数收到已消歧的函数指针\n";
}

template<typename Function, typename... Args>
decltype(auto) fwdCall(Function&& function, Args&&... args) {
    return std::invoke(std::forward<Function>(function),
                       std::forward<Args>(args)...);
}

struct StaticLimit {
    static const int value = 42; // 这里只在类内声明，没有类外定义
};

void consumeNumber(int value) {
    std::cout << "   目标函数收到整数 " << value << "\n";
}

struct ReviewBitField {
    unsigned mode : 3;
};

void demonstrateItem30() {
    std::cout << "\n=== Item 30: 完美转发失败的情况 ===\n\n";
    
    std::cout << "完美转发失败的情况:\n\n";
    
    // 1. 花括号初始化列表
    std::cout << "1. 花括号初始化列表:\n";
    // fwdCall(consumeVector, {1, 2, 3}); // 错误：大括号列表通常没有普通表达式类型
    std::vector<int> v{1, 2, 3};  // 需要先创建对象
    fwdCall(consumeVector, v);
    std::cout << "   auto x = {1,2,3} 推导 initializer_list 是 auto 的特殊规则，不是普通模板推导规则\n\n";
    
    // 2. 0和NULL作为空指针
    std::cout << "2. 0/NULL作为空指针:\n";
    // fwdCall(consumePointer, 0); // 0 被推导成 int，转发后不再是空指针常量
    fwdCall(consumePointer, nullptr);
    std::cout << "   nullptr 的类型是 std::nullptr_t；它不是指针类型，但可安全转换到指针类型\n\n";
    
    // 3. 仅声明的静态常量整型成员
    std::cout << "3. 静态常量成员:\n";
    // fwdCall(consumeNumber, StaticLimit::value); // 绑定引用会 ODR-use，因缺少类外定义而链接失败
    fwdCall(consumeNumber, static_cast<int>(StaticLimit::value));
    std::cout << "   解决：提供类外定义，或先产生不需要绑定该成员地址的值\n\n";

    // 4. 重载函数和函数模板名
    std::cout << "4. 重载函数名与函数模板名:\n";
    // fwdCall(consumeCallback, overloaded); // 错误：模板无法知道选择哪个重载
    using IntCallback = void (*)(int);
    fwdCall(consumeCallback, static_cast<IntCallback>(overloaded));
    // fwdCall(consumeCallback, functionTemplate); // 错误：模板无法选择函数模板实例
    fwdCall(consumeCallback, static_cast<IntCallback>(functionTemplate<int>));
    std::cout << "   解决：用目标函数指针类型消歧，并显式选择所需函数模板实例\n\n";

    // 5. 位域
    std::cout << "5. 位域:\n";
    ReviewBitField bits{5};
    // fwdCall(consumeNumber, bits.mode); // 错误：位域不能绑定到转发引用
    const int copiedMode = static_cast<int>(bits.mode);
    fwdCall(consumeNumber, copiedMode);
    std::cout << "   位域没有可独立取址的对象身份；先复制到普通对象再转发\n";
}

bool verifySemanticContracts() {
    static_assert(std::is_same_v<MyVector<int>, std::vector<int>>);
    static_assert(std::is_same_v<RemoveConstReferenceT<const int&>, int>);

    std::string name = "left";
    const bool forwardingWorks =
        forwardName(name) == ForwardingRoute::Lvalue &&
        forwardName(std::string("right")) == ForwardingRoute::Rvalue;

    GoodStringSetter setter;
    short shortIndex = 7;
    const bool constrainedRoutesWork =
        setter.setName("cache") == SetterRoute::Name &&
        setter.setName(42) == SetterRoute::Index &&
        setter.setName(shortIndex) == SetterRoute::Index;

    ReviewBitField bits{5};
    const int copiedMode = static_cast<int>(bits.mode);
    fwdCall(consumeNumber, copiedMode);
    using IntCallback = void (*)(int);
    fwdCall(consumeCallback, static_cast<IntCallback>(overloaded));
    const bool overloadResolved = lastCallbackRoute == CallbackRoute::Overload;
    fwdCall(consumeCallback, static_cast<IntCallback>(functionTemplate<int>));
    const bool functionTemplateResolved =
        lastCallbackRoute == CallbackRoute::FunctionTemplate;

    return forwardingWorks && constrainedRoutesWork && copiedMode == 5 &&
           overloadResolved && functionTemplateResolved;
}

// ========================================
// 主演示函数
// ========================================

void demonstrate() {
    std::cout << "╔════════════════════════════════════════╗\n";
    std::cout << "║     EMC++ Item 9, 23-30 复习           ║\n";
    std::cout << "╚════════════════════════════════════════╝\n\n";
    
    demonstrateItem09();
    demonstrateItem23();
    demonstrateItem24();
    demonstrateItem25();
    demonstrateItem26_27();
    demonstrateItem28();
    demonstrateItem29();
    demonstrateItem30();
    
    std::cout << "\nEMC++条款复习完成！\n";
}

} // namespace emcpp_review

bool verifyEMCPPReviewContract() {
    return emcpp_review::verifySemanticContracts();
}
