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
    MyString() : data_(nullptr), size_(0) {
        std::cout << "  [默认构造] 空字符串\n";
    }
    
    // 带参构造函数
    MyString(const char* str) {
        if (str) {
            size_ = std::strlen(str);
            data_ = new char[size_ + 1];
            std::strcpy(data_, str);
            std::cout << "  [构造] \"" << data_ << "\" (size=" << size_ << ")\n";
        } else {
            data_ = nullptr;
            size_ = 0;
            std::cout << "  [构造] 空字符串\n";
        }
    }
    
    // 析构函数
    ~MyString() {
        if (data_) {
            std::cout << "  [析构] \"" << data_ << "\"\n";
            delete[] data_;
        } else {
            std::cout << "  [析构] 空字符串\n";
        }
    }
    
    // 拷贝构造函数
    MyString(const MyString& other) : size_(other.size_) {
        if (other.data_) {
            data_ = new char[size_ + 1];
            std::strcpy(data_, other.data_);
            std::cout << "  [拷贝构造] \"" << data_ << "\"\n";
        } else {
            data_ = nullptr;
            std::cout << "  [拷贝构造] 空字符串\n";
        }
    }
    
    // 拷贝赋值运算符
    MyString& operator=(const MyString& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            if (other.data_) {
                data_ = new char[size_ + 1];
                std::strcpy(data_, other.data_);
                std::cout << "  [拷贝赋值] \"" << data_ << "\"\n";
            } else {
                data_ = nullptr;
                std::cout << "  [拷贝赋值] 空字符串\n";
            }
        }
        return *this;
    }
    
    // 移动构造函数
    MyString(MyString&& other) noexcept 
        : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;
        other.size_ = 0;
        if (data_) {
            std::cout << "  [移动构造] \"" << data_ << "\" (资源转移)\n";
        } else {
            std::cout << "  [移动构造] 空字符串\n";
        }
    }
    
    // 移动赋值运算符
    MyString& operator=(MyString&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            size_ = other.size_;
            other.data_ = nullptr;
            other.size_ = 0;
            if (data_) {
                std::cout << "  [移动赋值] \"" << data_ << "\" (资源转移)\n";
            } else {
                std::cout << "  [移动赋值] 空字符串\n";
            }
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
    Resource() : data_(nullptr), size_(0) {
        std::cout << "  [Resource] 默认构造\n";
    }
    
    // 带参构造
    Resource(const std::string& name, size_t size) 
        : name_(name), data_(new int[size]), size_(size) {
        std::cout << "  [Resource] 构造: " << name_ << " (" << size_ << " 个int)\n";
    }
    
    // 析构函数
    ~Resource() {
        delete[] data_;
        std::cout << "  [Resource] 析构: " << name_ << "\n";
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
        std::cout << "  [Resource] 移动构造: " << name_ << "\n";
    }
    
    Resource& operator=(Resource&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            name_ = std::move(other.name_);
            data_ = other.data_;
            size_ = other.size_;
            other.data_ = nullptr;
            other.size_ = 0;
            std::cout << "  [Resource] 移动赋值: " << name_ << "\n";
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
    
    // 左值：有名字、有地址的对象
    int x = 10;           // x是左值
    int& lref = x;        // 左值引用绑定到左值
    
    std::cout << "左值示例:\n";
    std::cout << "  int x = 10;  // x是左值\n";
    std::cout << "  int& lref = x;  // 左值引用\n";
    std::cout << "  x = " << x << ", lref = " << lref << "\n";
    
    // 右值：临时对象或字面量
    // int& rref1 = 10;   // 错误！左值引用不能绑定右值
    int&& rref = 10;      // 右值引用绑定到右值
    
    std::cout << "\n右值示例:\n";
    std::cout << "  int&& rref = 10;  // 右值引用绑定字面量\n";
    std::cout << "  rref = " << rref << "\n";
    
    // 右值引用可以延长临时对象的生命周期
    std::cout << "\n右值引用延长生命周期:\n";
    {
        std::string&& temp = std::string("temporary");
        std::cout << "  在作用域内，临时对象仍然有效: \"" << temp << "\"\n";
    }
    std::cout << "  作用域结束，临时对象被销毁\n";
    
    // std::move将左值转换为右值
    std::cout << "\nstd::move转换:\n";
    std::string str = "Hello";
    std::string moved = std::move(str);
    std::cout << "  原字符串: \"" << str << "\" (移动后可能为空)\n";
    std::cout << "  新字符串: \"" << moved << "\"\n";
}

void demonstrateMoveSemantics() {
    std::cout << "\n=== 移动语义演示 ===\n\n";
    
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
    std::cout << "  - 只是将左值转换为右值引用\n";
    std::cout << "  - 告知编译器\"这个对象可以被移动\"\n\n";
    
    std::vector<int> v1 = {1, 2, 3, 4, 5};
    std::cout << "原始vector: ";
    for (int x : v1) std::cout << x << " ";
    std::cout << "\n";
    
    // std::move后，源对象处于有效但未定义状态
    std::vector<int> v2 = std::move(v1);
    std::cout << "std::move后:\n";
    std::cout << "  v1大小: " << v1.size() << " (有效但未定义)\n";
    std::cout << "  v2: ";
    for (int x : v2) std::cout << x << " ";
    std::cout << "\n";
    
    // 对基本类型，std::move只是拷贝
    int a = 10;
    int b = std::move(a);  // 对int来说只是拷贝
    std::cout << "\n基本类型std::move:\n";
    std::cout << "  a = " << a << ", b = " << b << " (只是拷贝)\n";
}

void demonstrateRuleOfFive() {
    std::cout << "\n=== Rule of Five 演示 ===\n\n";
    
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
    
    std::cout << "为什么移动操作要标记为noexcept?\n";
    std::cout << "  1. 标准容器在重新分配时会优先使用移动\n";
    std::cout << "  2. 如果移动不是noexcept，容器会使用拷贝\n";
    std::cout << "  3. 异常安全的移动操作更可靠\n\n";
    
    std::vector<MyString> vec;
    vec.reserve(3);
    
    std::cout << "添加元素到vector:\n";
    vec.push_back(MyString("First"));
    vec.push_back(MyString("Second"));
    vec.push_back(MyString("Third"));
    
    std::cout << "\nvector需要扩容时:\n";
    std::cout << "  如果移动构造是noexcept，使用移动\n";
    std::cout << "  否则使用拷贝（更安全但更慢）\n";
}

void demonstratePerfectForwarding() {
    std::cout << "\n=== 完美转发基础 ===\n\n";
    
    // 辅助函数：识别值类别
    auto identify = [](int& x) { std::cout << "  左值: " << x << "\n"; };
    auto identifyRval = [](int&& x) { std::cout << "  右值: " << x << "\n"; };
    
    // 完美转发包装器
    auto wrapper = [&identify, &identifyRval](int&& x) {
        std::cout << "  转发前: " << x << "\n";
        identifyRval(std::forward<int>(x));  // 保持右值属性
    };
    
    std::cout << "完美转发保持值类别:\n";
    int a = 42;
    
    // 通过std::forward保持参数的原始属性
    auto perfectForward = [](auto&& x) {
        std::cout << "  参数值: " << x << "\n";
        // std::forward保持x的原始值类别
    };
    
    std::cout << "传入左值:\n";
    perfectForward(a);
    
    std::cout << "传入右值:\n";
    perfectForward(100);
    
    std::cout << "\nstd::forward vs std::move:\n";
    std::cout << "  std::move: 无条件转换为右值\n";
    std::cout << "  std::forward: 条件转换，保持原始值类别\n";
}

void demonstrateCommonPitfalls() {
    std::cout << "\n=== 常见陷阱 ===\n\n";
    
    std::cout << "陷阱1: 移动后的对象状态\n";
    std::cout << "  移动后对象处于\"有效但未定义\"状态\n";
    std::cout << "  可以安全地销毁或重新赋值\n";
    std::cout << "  但不应访问其值\n\n";
    
    std::cout << "陷阱2: 返回局部对象时不要std::move\n";
    std::cout << "  编译器会自动进行RVO优化\n";
    std::cout << "  手动std::move反而阻碍优化\n\n";
    
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
    
    std::cout << "陷阱3: 对基本类型std::move无意义\n";
    std::cout << "  int, double等基本类型的移动等于拷贝\n";
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
