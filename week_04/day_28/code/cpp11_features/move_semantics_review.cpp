/**
 * @file move_semantics_review.cpp
 * @brief 移动语义综合复习示例
 * 
 * 涵盖内容：
 * 1. 左值与右值
 * 2. 右值引用
 * 3. std::move与移动语义
 * 4. 移动构造函数与移动赋值运算符
 * 5. Rule of Five
 * 6. 完美转发基础
 */

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <memory>
#include <cstring>
#include <stdexcept>
#include <streambuf>
#include <type_traits>

namespace move_semantics_review {

// ========================================
// 简单字符串类（演示移动语义）
// ========================================

/**
 * @brief 简单字符串类
 * 演示移动构造、移动赋值和Rule of Five
 */
class MyString {
private:
    char* data_;
    size_t size_;
    
public:
    // 默认构造函数
    MyString() : data_(nullptr), size_(0) {}
    
    // 带参构造函数
    MyString(const char* str) {
        if (str) {
            size_ = std::strlen(str);
            data_ = new char[size_ + 1];
            std::strcpy(data_, str);
        } else {
            data_ = nullptr;
            size_ = 0;
        }
    }
    
    // 析构函数
    ~MyString() {
        delete[] data_;
    }
    
    // 拷贝构造函数
    MyString(const MyString& other) : size_(other.size_) {
        if (other.data_) {
            data_ = new char[size_ + 1];
            std::strcpy(data_, other.data_);
        } else {
            data_ = nullptr;
        }
    }
    
    // 拷贝赋值运算符
    MyString& operator=(const MyString& other) {
        if (this != &other) {
            char* replacement = nullptr;
            if (other.data_) {
                replacement = new char[other.size_ + 1];
                std::memcpy(replacement, other.data_, other.size_ + 1);
            }
            delete[] data_;
            data_ = replacement;
            size_ = other.size_;
        }
        return *this;
    }
    
    // 移动构造函数
    MyString(MyString&& other) noexcept 
        : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;
        other.size_ = 0;
    }
    
    // 移动赋值运算符
    MyString& operator=(MyString&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            size_ = other.size_;
            other.data_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }
    
    // 获取内容
    const char* c_str() const { return data_ ? data_ : ""; }
    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }
    
    // 输出运算符
    friend std::ostream& operator<<(std::ostream& os, const MyString& s) {
        return os << (s.data_ ? s.data_ : "");
    }
};

// ========================================
// 资源管理类
// ========================================

/**
 * @brief 资源管理类
 * 演示Rule of Five和= default/ = delete
 */
class Resource {
private:
    std::string name_;
    int* data_;
    size_t size_;
    
public:
    // 默认构造
    Resource() : data_(nullptr), size_(0) {}
    
    // 带参构造
    Resource(const std::string& name, size_t size) 
        : name_(name), data_(new int[size]), size_(size) {}
    
    // 析构函数
    ~Resource() {
        delete[] data_;
    }
    
    // 禁用拷贝（资源唯一）
    Resource(const Resource&) = delete;
    Resource& operator=(const Resource&) = delete;
    
    // 默认移动操作
    Resource(Resource&& other) noexcept 
        : name_(std::move(other.name_))
        , data_(other.data_)
        , size_(other.size_) {
        other.data_ = nullptr;
        other.size_ = 0;
    }
    
    Resource& operator=(Resource&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            name_ = std::move(other.name_);
            data_ = other.data_;
            size_ = other.size_;
            other.data_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }
    
    const std::string& name() const { return name_; }
};

// ========================================
// 演示函数
// ========================================

void demonstrateLvalueRvalue() {
    std::cout << "=== 左值与右值演示 ===\n\n";
    
    // 值类别是表达式属性；变量名表达式是常见左值。
    int x = 10;           // 表达式 x 是左值
    int& lref = x;        // 左值引用绑定到左值
    
    std::cout << "左值示例:\n";
    std::cout << "  左值是具有身份的表达式，不由对象是否有名字单独决定\n";
    std::cout << "  int x = 10;  // 表达式 x 是左值\n";
    std::cout << "  int& lref = x;  // 左值引用\n";
    std::cout << "  x = " << x << ", lref = " << lref << "\n";
    
    // 右值包括纯右值和将亡值；字面量只是纯右值的常见例子。
    // int& rref1 = 10;   // 错误！左值引用不能绑定右值
    int&& rref = 10;      // 右值引用绑定到右值
    
    std::cout << "\n右值示例:\n";
    std::cout << "  右值是纯右值与将亡值的总称，std::move(x) 产生的 xvalue 仍有身份\n";
    std::cout << "  int&& rref = 10;  // 右值引用绑定字面量\n";
    std::cout << "  命名后的 rref 表达式本身仍是左值\n";
    std::cout << "  rref = " << rref << "\n";
    
    // 临时量直接绑定到这个局部引用变量时，生命周期延长到该变量的作用域末尾。
    // 这种延长不会因返回引用或把引用继续传给别处而自动传播。
    std::cout << "\n局部引用直接绑定临时量时延长生命周期:\n";
    {
        std::string&& temp = std::string("temporary");
        std::cout << "  在作用域内，临时对象仍然有效: \"" << temp << "\"\n";
    }
    std::cout << "  作用域结束，临时对象被销毁\n";
    
    // std::move 只把表达式转换为 xvalue，不执行资源转移。
    std::cout << "\nstd::move转换:\n";
    std::string str = "Hello";
    std::string moved = std::move(str);
    std::cout << "  原字符串: \"" << str << "\" (仍有效，具体状态未指定)\n";
    std::cout << "  新字符串: \"" << moved << "\"\n";
}

void demonstrateMoveSemantics() {
    std::cout << "\n=== 移动语义演示 ===\n\n";
    std::cout << "特殊成员只管理资源，日志放在调用点；这样流异常不会造成构造泄漏，"
                 "也不会在复制赋值已经提交后再向调用方报失败。\n\n";
    
    std::cout << "创建对象:\n";
    MyString s1("Hello World");
    
    std::cout << "\n拷贝构造:\n";
    MyString s2 = s1;  // 调用拷贝构造
    std::cout << "s1: \"" << s1 << "\", s2: \"" << s2 << "\"\n";
    
    std::cout << "\n移动构造:\n";
    MyString s3 = std::move(s1);  // 调用移动构造
    std::cout << "s1: \"" << s1 << "\", s3: \"" << s3 << "\"\n";
    
    std::cout << "\n移动赋值:\n";
    MyString s4;
    s4 = std::move(s2);  // 调用移动赋值
    std::cout << "s2: \"" << s2 << "\", s4: \"" << s4 << "\"\n";
    
    std::cout << "\n离开作用域，自动析构:\n";
}

void demonstrateStdMove() {
    std::cout << "\n=== std::move详解 ===\n\n";
    
    std::cout << "std::move本质:\n";
    std::cout << "  - 不执行任何移动操作\n";
    std::cout << "  - 把表达式转换为 xvalue\n";
    std::cout << "  - 允许后续重载考虑右值路径，但不保证一定发生移动\n\n";
    
    std::vector<int> v1 = {1, 2, 3, 4, 5};
    std::cout << "原始vector: ";
    for (int x : v1) std::cout << x << " ";
    std::cout << "\n";
    
    // std::move后，标准容器源对象有效但状态未指定
    std::vector<int> v2 = std::move(v1);
    std::cout << "std::move后:\n";
    std::cout << "  v1大小: " << v1.size() << " (有效但状态未指定)\n";
    std::cout << "  v2: ";
    for (int x : v2) std::cout << x << " ";
    std::cout << "\n";
    
    // 本例 int 没有可转交资源；初始化仍复制数值，std::move 只改变表达式类别。
    int a = 10;
    int b = std::move(a);
    std::cout << "\n基本类型std::move:\n";
    std::cout << "  a = " << a << ", b = " << b << " (只是拷贝)\n";
}

void demonstrateRuleOfFive() {
    std::cout << "\n=== Rule of Five 演示 ===\n\n";
    std::cout << "默认先选Rule of Zero：让标准库资源成员自动组合特殊成员。\n";
    std::cout << "只有类型直接拥有裸资源时，才用Rule of Five系统检查以下五项：\n";
    std::cout << "Rule of Five:\n";
    std::cout << "  1. 析构函数\n";
    std::cout << "  2. 拷贝构造函数\n";
    std::cout << "  3. 拷贝赋值运算符\n";
    std::cout << "  4. 移动构造函数\n";
    std::cout << "  5. 移动赋值运算符\n\n";
    
    std::cout << "创建资源:\n";
    Resource r1("Resource1", 100);
    
    std::cout << "\n移动构造:\n";
    Resource r2 = std::move(r1);
    
    std::cout << "\n移动赋值:\n";
    Resource r3;
    r3 = std::move(r2);
    
    std::cout << "\n注意: Resource禁用了拷贝操作\n";
    // Resource r4 = r3;  // 编译错误！
    
    std::cout << "\n离开作用域:\n";
}

void demonstrateNoexcept() {
    std::cout << "\n=== noexcept的重要性 ===\n\n";
    
    std::cout << "什么时候移动操作应该标记为 noexcept?\n";
    std::cout << "  1. 只有实现确实不会抛异常时才能作出该承诺\n";
    std::cout << "  2. 移动可能抛异常且拷贝可用时，容器迁移旧元素可能选择拷贝\n";
    std::cout << "  3. 拷贝不可用时仍可能使用可抛移动，具体保证取决于操作与类型\n\n";
    
    std::vector<MyString> vec;
    vec.reserve(3);
    
    std::cout << "添加元素到vector:\n";
    vec.push_back(MyString("First"));
    vec.push_back(MyString("Second"));
    vec.push_back(MyString("Third"));
    
    std::cout << "\nvector需要扩容时:\n";
    std::cout << "  MyString 的移动确实不抛: "
              << std::boolalpha
              << std::is_nothrow_move_constructible_v<MyString> << "\n";
    std::cout << "  正确的 noexcept 信息会影响容器为异常保证作出的迁移选择\n";
}

enum class ForwardingRoute {
    Lvalue,
    Rvalue
};

ForwardingRoute forwardingTarget(int&) {
    return ForwardingRoute::Lvalue;
}

ForwardingRoute forwardingTarget(int&&) {
    return ForwardingRoute::Rvalue;
}

template<typename T>
ForwardingRoute forwardToTarget(T&& value) {
    return forwardingTarget(std::forward<T>(value));
}

const char* routeName(ForwardingRoute route) {
    return route == ForwardingRoute::Lvalue ? "int& 左值重载" : "int&& 右值重载";
}

void demonstratePerfectForwarding() {
    std::cout << "\n=== 完美转发基础 ===\n\n";

    std::cout << "固定 int&& 形参是右值引用，不是转发引用；内部继续交付时使用 std::move。\n";
    auto consumeKnownRvalue = [](int&& value) {
        return forwardingTarget(std::move(value));
    };
    std::cout << "  固定右值引用调用目标: " << routeName(consumeKnownRvalue(100)) << "\n\n";

    // generic lambda 的 auto&& 会推导实参类型，并真实调用目标重载。
    auto perfectForward = [](auto&& value) {
        return forwardingTarget(std::forward<decltype(value)>(value));
    };

    std::cout << "转发引用 + std::forward 的目标重载结果:\n";
    int a = 42;
    std::cout << "  传入左值 a -> " << routeName(perfectForward(a)) << "\n";
    std::cout << "  传入右值 100 -> " << routeName(perfectForward(100)) << "\n";
    
    std::cout << "\nstd::forward vs std::move:\n";
    std::cout << "  std::move: 无条件产生 xvalue，但不执行移动\n";
    std::cout << "  std::forward: 条件转换，保持原始值类别\n";
}

void demonstrateCommonPitfalls() {
    std::cout << "\n=== 常见陷阱 ===\n\n";
    
    std::cout << "陷阱1: 移动后的对象状态\n";
    std::cout << "  移动后对象应满足类型契约；标准容器通常有效但状态未指定\n";
    std::cout << "  可以安全地销毁、重新赋值，并调用前置条件仍满足的操作\n";
    std::cout << "  不应依赖其具体值，除非类型额外给出更强承诺\n\n";
    
    std::cout << "陷阱2: 返回局部对象时不要std::move\n";
    std::cout << "  直接返回通常保留复制消除或隐式移动的机会\n";
    std::cout << "  手动std::move可能阻碍复制消除\n\n";
    
    std::cout << "正确示例:\n";
    std::cout << "  std::string create() {\n";
    std::cout << "    std::string s = \"hello\";\n";
    std::cout << "    return s;  // RVO或隐式移动\n";
    std::cout << "  }\n\n";
    
    std::cout << "错误示例:\n";
    std::cout << "  std::string create() {\n";
    std::cout << "    std::string s = \"hello\";\n";
    std::cout << "    return std::move(s);  // 阻碍RVO\n";
    std::cout << "  }\n\n";
    
    std::cout << "陷阱3: 不要把std::move等同于资源转移\n";
    std::cout << "  本例int初始化仍复制数值；std::move只改变表达式类别，也可能影响重载决议\n";
}

bool verify_forwarding_contract() {
    int value = 7;
    return forwardToTarget(value) == ForwardingRoute::Lvalue &&
           forwardToTarget(7) == ForwardingRoute::Rvalue;
}

bool verify_special_member_stream_contract() {
    class ThrowingStreamBuffer : public std::streambuf {
    protected:
        int_type overflow(int_type) override {
            throw std::runtime_error("injected output failure");
        }
    } throwingBuffer;

    std::streambuf* const originalBuffer = std::cout.rdbuf(&throwingBuffer);
    const std::ios::iostate originalExceptions = std::cout.exceptions();
    std::cout.exceptions(std::ios::badbit | std::ios::failbit);

    bool passed = false;
    try {
        MyString source("contract");
        MyString copied(source);
        MyString assigned("old");
        assigned = source;
        MyString moved(std::move(source));

        Resource resource("resource", 2);
        Resource movedResource(std::move(resource));
        Resource assignedResource;
        assignedResource = std::move(movedResource);

        passed = std::string(copied.c_str()) == "contract" &&
                 std::string(assigned.c_str()) == "contract" &&
                 std::string(moved.c_str()) == "contract" &&
                 source.empty() && assignedResource.name() == "resource";
    } catch (...) {
        passed = false;
    }

    std::cout.exceptions(std::ios::goodbit);
    std::cout.clear();
    std::cout.rdbuf(originalBuffer);
    std::cout.exceptions(originalExceptions);
    return passed;
}

void demonstrate() {
    std::cout << "╔════════════════════════════════════════╗\n";
    std::cout << "║        移动语义综合复习                ║\n";
    std::cout << "╚════════════════════════════════════════╝\n\n";
    
    demonstrateLvalueRvalue();
    demonstrateMoveSemantics();
    demonstrateStdMove();
    demonstrateRuleOfFive();
    demonstrateNoexcept();
    demonstratePerfectForwarding();
    demonstrateCommonPitfalls();
    
    std::cout << "\n移动语义复习完成！\n";
}

} // namespace move_semantics_review
