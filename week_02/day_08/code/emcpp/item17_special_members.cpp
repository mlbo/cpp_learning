/**
 * @file item17_special_members.cpp
 * @brief EMC++条款17：理解特殊成员函数的生成规则
 * @author C++ Tutorial
 * @date Day 8
 * 
 * 条款17核心内容：
 * 1. 特殊成员函数包括：默认构造、析构、拷贝构造、拷贝赋值、移动构造、移动赋值
 * 2. 这些函数的生成规则存在复杂的相互影响
 * 3. 理解规则可以避免意外的行为
 */

#include <iostream>
#include <string>
#include <memory>
#include <utility>

// 辅助打印
#define LOG_FUNC(msg) std::cout << "[" << #msg << "] " << __func__ << "\n"
#define LOG_CALL(obj, msg) std::cout << "[" << #msg << "] " << obj << "\n"

void printSeparator(const std::string& title = "") {
    std::cout << "\n======== " << title << " ========\n";
}

// ============================================================
// 条款17核心规则演示
// ============================================================

/**
 * @brief 特殊成员函数生成规则总览
 * 
 * 1. 默认构造函数：
 *    - 只有在类没有声明任何构造函数时才生成
 * 
 * 2. 析构函数：
 *    - 如果没有声明，自动生成
 *    - 基类析构函数为virtual时，生成的也是virtual
 * 
 * 3. 拷贝操作（拷贝构造、拷贝赋值）：
 *    - 如果没有声明，自动生成
 *    - 声明移动操作会阻止自动生成
 * 
 * 4. 移动操作（移动构造、移动赋值）：
 *    - 只有在类没有声明拷贝操作、移动操作、析构函数时才生成
 */

// ============================================================
// 示例1：什么都不声明的类
// ============================================================

class EmptyClass {
    // 不声明任何特殊成员函数
};

void demoEmptyClass() {
    printSeparator("示例1：什么都不声明");
    
    std::cout << "\n编译器会自动生成所有特殊成员函数：\n";
    std::cout << "  - 默认构造函数\n";
    std::cout << "  - 析构函数\n";
    std::cout << "  - 拷贝构造函数\n";
    std::cout << "  - 拷贝赋值运算符\n";
    std::cout << "  - 移动构造函数 (C++11)\n";
    std::cout << "  - 移动赋值运算符 (C++11)\n\n";
    
    EmptyClass e1;              // 默认构造
    EmptyClass e2(e1);          // 拷贝构造
    EmptyClass e3 = e1;         // 拷贝构造
    EmptyClass e4(std::move(e1)); // 移动构造
    e2 = e3;                    // 拷贝赋值
    e3 = std::move(e4);         // 移动赋值
    // e1.~EmptyClass();        // 析构（自动调用）
    
    std::cout << "所有操作都可以正常使用！\n";
}

// ============================================================
// 示例2：声明析构函数
// ============================================================

class WithDestructor {
public:
    WithDestructor() { LOG_FUNC(默认构造); }
    ~WithDestructor() { LOG_FUNC(析构); }
    
    std::string data = "data";
};

void demoWithDestructor() {
    printSeparator("示例2：声明析构函数");
    
    std::cout << "\n声明析构函数后的影响：\n";
    std::cout << "  - 默认构造：用户已声明 ✓\n";
    std::cout << "  - 析构函数：用户已声明 ✓\n";
    std::cout << "  - 拷贝操作：仍然自动生成 ✓\n";
    std::cout << "  - 移动操作：不再自动生成 ✗\n\n";
    
    WithDestructor w1;
    WithDestructor w2(w1);      // 拷贝构造 ✓
    WithDestructor w3 = w1;     // 拷贝构造 ✓
    
    // WithDestructor w4(std::move(w1)); // 移动构造：会退化为拷贝！
    // 这不会调用移动构造，而是调用拷贝构造
    
    std::cout << "\n注意：移动操作不会生成，但代码仍可编译！\n";
    std::cout << "原因：没有移动构造时，会退化为拷贝构造\n";
    
    WithDestructor w4(std::move(w1)); // 实际调用拷贝构造
    std::cout << "移动退化为拷贝，性能损失！\n";
}

// ============================================================
// 示例3：声明拷贝操作
// ============================================================

class WithCopyOperations {
public:
    WithCopyOperations() : data(new std::string("data")) { LOG_FUNC(默认构造); }
    
    ~WithCopyOperations() { 
        LOG_FUNC(析构); 
        delete data;
    }
    
    // 用户声明的拷贝操作
    WithCopyOperations(const WithCopyOperations& other) 
        : data(new std::string(*other.data)) {
        LOG_FUNC(拷贝构造);
    }
    
    WithCopyOperations& operator=(const WithCopyOperations& other) {
        LOG_FUNC(拷贝赋值);
        if (this != &other) {
            *data = *other.data;
        }
        return *this;
    }
    
private:
    std::string* data;
};

void demoWithCopyOperations() {
    printSeparator("示例3：声明拷贝操作");
    
    std::cout << "\n声明拷贝操作后的影响：\n";
    std::cout << "  - 默认构造：仍可用（用户声明了）✓\n";
    std::cout << "  - 析构函数：用户声明 ✓\n";
    std::cout << "  - 拷贝操作：用户声明 ✓\n";
    std::cout << "  - 移动操作：不自动生成 ✗\n\n";
    
    WithCopyOperations w1;
    WithCopyOperations w2(w1);      // 拷贝构造 ✓
    
    // 移动操作不存在，会退化为拷贝
    WithCopyOperations w3(std::move(w1)); // 实际调用拷贝构造
    
    std::cout << "\n问题：声明了拷贝操作后，移动操作不会生成\n";
    std::cout << "移动会退化为拷贝，导致不必要的深拷贝！\n";
}

// ============================================================
// 示例4：声明移动操作
// ============================================================

class WithMoveOperations {
public:
    WithMoveOperations() : data(new std::string("data")) { LOG_FUNC(默认构造); }
    
    ~WithMoveOperations() { 
        LOG_FUNC(析构); 
        delete data;
    }
    
    // 用户声明的移动操作
    WithMoveOperations(WithMoveOperations&& other) noexcept
        : data(other.data) {
        LOG_FUNC(移动构造);
        other.data = nullptr;
    }
    
    WithMoveOperations& operator=(WithMoveOperations&& other) noexcept {
        LOG_FUNC(移动赋值);
        if (this != &other) {
            delete data;
            data = other.data;
            other.data = nullptr;
        }
        return *this;
    }
    
    // 注意：移动操作声明后，拷贝操作不会自动生成！
    // 如果需要拷贝，必须显式声明
    
private:
    std::string* data;
};

void demoWithMoveOperations() {
    printSeparator("示例4：声明移动操作");
    
    std::cout << "\n声明移动操作后的影响：\n";
    std::cout << "  - 默认构造：不生成（因为声明了移动构造）✗\n";
    std::cout << "  - 析构函数：用户声明 ✓\n";
    std::cout << "  - 拷贝操作：不自动生成 ✗\n";
    std::cout << "  - 移动操作：用户声明 ✓\n\n";
    
    WithMoveOperations w1;                  // 默认构造 ✓（用户声明了）
    WithMoveOperations w2(std::move(w1));   // 移动构造 ✓
    
    // WithMoveOperations w3(w2);  // 编译错误！拷贝构造不存在
    // w2 = w3;                    // 编译错误！拷贝赋值不存在
    
    std::cout << "\n问题：声明移动操作后，拷贝操作被禁用\n";
    std::cout << "如果需要拷贝语义，必须显式声明拷贝操作！\n";
}

// ============================================================
// 示例5：Rule of Five
// ============================================================

class RuleOfFive {
public:
    RuleOfFive() : data(new std::string("data")) { LOG_FUNC(默认构造); }
    
    // 1. 析构函数
    ~RuleOfFive() { 
        LOG_FUNC(析构); 
        delete data;
    }
    
    // 2. 拷贝构造函数
    RuleOfFive(const RuleOfFive& other) 
        : data(new std::string(*other.data)) {
        LOG_FUNC(拷贝构造);
    }
    
    // 3. 拷贝赋值运算符
    RuleOfFive& operator=(const RuleOfFive& other) {
        LOG_FUNC(拷贝赋值);
        if (this != &other) {
            *data = *other.data;
        }
        return *this;
    }
    
    // 4. 移动构造函数
    RuleOfFive(RuleOfFive&& other) noexcept
        : data(other.data) {
        LOG_FUNC(移动构造);
        other.data = nullptr;
    }
    
    // 5. 移动赋值运算符
    RuleOfFive& operator=(RuleOfFive&& other) noexcept {
        LOG_FUNC(移动赋值);
        if (this != &other) {
            delete data;
            data = other.data;
            other.data = nullptr;
        }
        return *this;
    }
    
private:
    std::string* data;
};

void demoRuleOfFive() {
    printSeparator("示例5：Rule of Five");
    
    std::cout << "\nRule of Five：如果需要自定义任何一个特殊成员函数，\n";
    std::cout << "通常需要自定义全部五个：\n";
    std::cout << "  1. 析构函数\n";
    std::cout << "  2. 拷贝构造函数\n";
    std::cout << "  3. 拷贝赋值运算符\n";
    std::cout << "  4. 移动构造函数\n";
    std::cout << "  5. 移动赋值运算符\n\n";
    
    RuleOfFive r1;
    RuleOfFive r2(r1);          // 拷贝构造
    RuleOfFive r3(std::move(r1)); // 移动构造
    r2 = r3;                    // 拷贝赋值
    r3 = RuleOfFive();          // 移动赋值
    
    std::cout << "\n所有操作都正确实现！\n";
}

// ============================================================
// 示例6：Rule of Zero（现代C++最佳实践）
// ============================================================

class RuleOfZero {
public:
    explicit RuleOfZero(const std::string& value = "") 
        : data(std::make_unique<std::string>(value)) { 
        LOG_FUNC(构造); 
    }
    
    // 不声明析构函数、拷贝操作、移动操作
    // 使用unique_ptr自动管理资源
    
    const std::string& get() const { return *data; }
    
private:
    std::unique_ptr<std::string> data;
};

// 如果需要拷贝语义，可以显式声明（unique_ptr不可拷贝）
class RuleOfZeroCopyable {
public:
    explicit RuleOfZeroCopyable(const std::string& value = "") 
        : data(std::make_shared<std::string>(value)) {}
    
    // 使用shared_ptr，自动支持拷贝
    
    const std::string& get() const { return *data; }
    
private:
    std::shared_ptr<std::string> data;
};

// 或者使用默认成员
class RuleOfZeroDefault {
public:
    RuleOfZeroDefault() = default;
    RuleOfZeroDefault(const RuleOfZeroDefault&) = default;
    RuleOfZeroDefault(RuleOfZeroDefault&&) = default;
    RuleOfZeroDefault& operator=(const RuleOfZeroDefault&) = default;
    RuleOfZeroDefault& operator=(RuleOfZeroDefault&&) = default;
    ~RuleOfZeroDefault() = default;
    
private:
    std::string data;  // 标准库类型自动管理内存
};

void demoRuleOfZero() {
    printSeparator("示例6：Rule of Zero（最佳实践）");
    
    std::cout << "\nRule of Zero：让编译器自动生成特殊成员函数\n";
    std::cout << "使用智能指针或标准容器管理资源，无需手动实现\n\n";
    
    RuleOfZero z1("hello");
    RuleOfZero z2(std::move(z1));  // 移动构造（unique_ptr支持移动）
    std::cout << "z2.get() = " << z2.get() << "\n";
    
    // RuleOfZero z3(z2);  // 编译错误！unique_ptr不可拷贝
    
    std::cout << "\n如果需要拷贝语义：\n";
    std::cout << "  1. 使用shared_ptr\n";
    std::cout << "  2. 使用标准容器（vector, string等）\n";
    std::cout << "  3. 显式=default声明\n";
}

// ============================================================
// 生成规则对照表
// ============================================================

void printGenerationRules() {
    printSeparator("特殊成员函数生成规则对照表");
    
    std::cout << R"(
┌─────────────────────────────────────────────────────────────────────────┐
│           特殊成员函数生成规则（C++11起）                                │
├─────────────────────────────────────────────────────────────────────────┤
│ 声明的操作          │ 默认 │ 析构 │ 拷贝构造 │ 拷贝赋值 │ 移动构造 │ 移动赋值 │
├─────────────────────────────────────────────────────────────────────────┤
│ 无                  │  ✓   │  ✓   │    ✓     │    ✓     │    ✓     │    ✓     │
│ 任何构造函数        │  ✗   │  ✓   │    ✓     │    ✓     │    ✓     │    ✓     │
│ 析构函数            │  ✓   │  -   │    ✓     │    ✓     │    ✗     │    ✗     │
│ 拷贝构造            │  ✓   │  ✓   │    -     │    ✓     │    ✗     │    ✗     │
│ 拷贝赋值            │  ✓   │  ✓   │    ✓     │    -     │    ✗     │    ✗     │
│ 移动构造            │  ✗   │  ✓   │    ✗     │    ✗     │    -     │    ✗     │
│ 移动赋值            │  ✓   │  ✓   │    ✗     │    ✗     │    ✗     │    -     │
└─────────────────────────────────────────────────────────────────────────┘

关键规则：
  1. 移动操作仅在类完全没有任何拷贝操作时生成
  2. 声明移动操作会阻止拷贝操作的自动生成
  3. 析构函数的存在会阻止移动操作的生成
  4. Rule of Zero 优于 Rule of Five

)";
}

// ============================================================
// 主函数
// ============================================================

int main() {
    std::cout << "╔═════════════════════════════════════════════════════════╗\n";
    std::cout << "║   EMC++条款17：理解特殊成员函数的生成规则               ║\n";
    std::cout << "╚═════════════════════════════════════════════════════════╝\n";
    
    printGenerationRules();
    
    demoEmptyClass();
    demoWithDestructor();
    demoWithCopyOperations();
    demoWithMoveOperations();
    demoRuleOfFive();
    demoRuleOfZero();
    
    printSeparator("条款17总结");
    std::cout << "核心要点：\n";
    std::cout << "  1. 特殊成员函数的生成存在复杂的相互影响\n";
    std::cout << "  2. 声明析构函数会阻止移动操作的生成\n";
    std::cout << "  3. 声明拷贝操作会阻止移动操作的生成\n";
    std::cout << "  4. 声明移动操作会阻止拷贝操作的生成\n";
    std::cout << "  5. 优先遵循 Rule of Zero\n";
    std::cout << "  6. 如需自定义，遵循 Rule of Five\n";
    std::cout << "  7. 显式使用 =default 和 =delete 表达意图\n";
    
    return 0;
}
