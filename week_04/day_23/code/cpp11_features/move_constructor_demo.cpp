/**
 * @file move_constructor_demo.cpp
 * @brief 移动构造函数和移动赋值运算符详解
 * 
 * 本文件演示：
 * 1. 规则五（Rule of Five）的实现
 * 2. noexcept 契约及其对容器迁移选择的影响
 * 3. 移动操作的最佳实践
 */

#include <iostream>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <type_traits>
#include <vector>

namespace {

class BufferThrowingStreamBuffer : public std::streambuf {
protected:
    int_type overflow(int_type) override {
        throw std::runtime_error("injected output failure");
    }
};

} // namespace

// ============================================================
// 资源管理类 - 完整实现 Rule of Five
// ============================================================

class Buffer {
private:
    int* data_;         // 动态数组指针
    size_t size_;       // 数组大小
    const char* name_;  // 调试用名称

public:
    // 1. 默认构造函数
    Buffer() : data_(nullptr), size_(0), name_("unnamed") {}
    
    // 带大小的构造函数
    explicit Buffer(size_t size, const char* name = "buffer")
        : data_(size > 0 ? new int[size]{} : nullptr)
        , size_(size)
        , name_(name) {}
    
    // 2. 析构函数
    ~Buffer() {
        delete[] data_;
    }
    
    // 3. 拷贝构造函数
    Buffer(const Buffer& other)
        : data_(other.size_ > 0 ? new int[other.size_] : nullptr)
        , size_(other.size_)
        , name_("copied") {
        if (data_) {
            std::copy(other.data_, other.data_ + size_, data_);
        }
    }
    
    // 4. 拷贝赋值运算符
    Buffer& operator=(const Buffer& other) {
        if (this != &other) {
            // 先分配并复制，再替换旧指针；new 失败时当前对象保持原值。
            int* replacement = other.size_ > 0 ? new int[other.size_] : nullptr;
            if (replacement) {
                std::copy(other.data_, other.data_ + other.size_, replacement);
            }
            delete[] data_;
            data_ = replacement;
            size_ = other.size_;
            name_ = "copy_assigned";
        }
        return *this;
    }
    
    // 5. 移动构造函数 - 核心重点
    // 本实现只转交指针和整数，因此可以诚实承诺 noexcept。
    Buffer(Buffer&& other) noexcept
        : data_(other.data_)
        , size_(other.size_)
        , name_("moved") {
        // 本类型选择用空缓冲区作为 moved-from 契约；标准并不要求所有类型置空。
        other.data_ = nullptr;
        other.size_ = 0;
        other.name_ = "moved_from";
        
    }
    
    // 6. 移动赋值运算符
    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            // 释放原有资源
            delete[] data_;
            
            // 窃取资源
            data_ = other.data_;
            size_ = other.size_;
            name_ = "move_assigned";
            
            // 恢复本类型约定的空缓冲区状态。
            other.data_ = nullptr;
            other.size_ = 0;
            other.name_ = "moved_from";
            
        }
        return *this;
    }
    
    // 辅助方法
    size_t size() const { return size_; }
    bool hasData() const { return data_ != nullptr; }
    const char* name() const { return name_; }
    
    // 设置元素值
    void set(size_t index, int value) {
        if (index < size_) {
            data_[index] = value;
        }
    }
    
    // 获取元素值
    int get(size_t index) const {
        return (index < size_) ? data_[index] : 0;
    }
};

// ============================================================
// 演示函数
// ============================================================

void demonstrateMoveConstructor() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "移动构造函数演示" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    std::cout << "特殊成员只维护所有权；调用点输出操作前后状态，"
                 "因此流异常不会造成构造泄漏或提交后再抛出。" << std::endl;
    
    // -------------------------------------------------------
    // 演示 1：移动构造
    // -------------------------------------------------------
    std::cout << "\n【演示 1】移动构造函数\n" << std::endl;
    
    Buffer buf1(10, "original");
    for (size_t i = 0; i < buf1.size(); ++i) {
        buf1.set(i, static_cast<int>(i * 10));
    }
    
    std::cout << "\n执行移动构造: Buffer buf2 = std::move(buf1)" << std::endl;
    Buffer buf2 = std::move(buf1);
    
    std::cout << "\n移动后状态:" << std::endl;
    std::cout << "  buf1: " << (buf1.hasData() ? "持有数据" : "空但仍是有效对象")
              << ", 名称: " << buf1.name() << std::endl;
    std::cout << "  buf2: " << (buf2.hasData() ? "持有数据" : "空缓冲区")
              << ", 名称: " << buf2.name() << std::endl;
    
    if (buf2.hasData()) {
        std::cout << "  buf2 数据: ";
        for (size_t i = 0; i < buf2.size(); ++i) {
            std::cout << buf2.get(i) << " ";
        }
        std::cout << std::endl;
    }
    
    // -------------------------------------------------------
    // 演示 2：移动赋值
    // -------------------------------------------------------
    std::cout << "\n【演示 2】移动赋值运算符\n" << std::endl;
    
    Buffer buf3(5, "target");
    Buffer buf4(15, "source");
    for (size_t i = 0; i < buf4.size(); ++i) {
        buf4.set(i, static_cast<int>(i + 1));
    }
    
    std::cout << "\n执行移动赋值: buf3 = std::move(buf4)" << std::endl;
    buf3 = std::move(buf4);
    
    std::cout << "\n移动后状态:" << std::endl;
    std::cout << "  buf3: 大小=" << buf3.size() << ", 名称=" << buf3.name() << std::endl;
    std::cout << "  buf4: 大小=" << buf4.size() << ", 名称=" << buf4.name() << std::endl;
    
    // -------------------------------------------------------
    // 演示 3：自赋值检测
    // -------------------------------------------------------
    std::cout << "\n【演示 3】自移动检测\n" << std::endl;
    
    Buffer buf5(10, "self");
    std::cout << "尝试自移动: buf5 = std::move(buf5)" << std::endl;
    buf5 = std::move(buf5);  // 自赋值，应该被检测并跳过
    
    std::cout << "  buf5 仍持有数据: " << (buf5.hasData() ? "是" : "否") << std::endl;
}

// ============================================================
// noexcept 的重要性演示
// ============================================================

void demonstrateNoexcept() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "noexcept 关键字的重要性" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    std::cout << R"(
什么时候移动操作应该标记为 noexcept？

1. 先保证契约真实：
   - 只有实现及其成员移动都确实不会抛异常时，才能承诺 noexcept
   - 可能抛出却标记 noexcept，会在异常逸出时调用 std::terminate

2. 容器迁移选择：
   - vector 等容器重新分配时需要维护异常保证
   - 当移动可能抛异常且拷贝可用时，容器可能选择拷贝旧元素
   - 若拷贝不可用，容器仍可能使用可抛移动；具体保证取决于操作与类型

3. 性能是正确契约的结果：
   - 正确的 noexcept 信息能帮助标准库选择更合适的迁移路径
   - noexcept(false) 不是“不推荐”，而是对可能抛异常实现的诚实描述

代码示例：
  // Buffer 只转交裸指针，可以诚实承诺不抛异常
  Buffer(Buffer&& other) noexcept;
  Buffer& operator=(Buffer&& other) noexcept;

  // 若成员移动可能抛异常，应省略 noexcept 或使用条件 noexcept
  Wrapper(Wrapper&& other)
      noexcept(std::is_nothrow_move_constructible_v<Member>);
)" << std::endl;
    
    // 使用 static_assert 检查
    static_assert(std::is_nothrow_move_constructible<Buffer>::value,
                  "Buffer 应该支持 noexcept 移动构造");
    static_assert(std::is_nothrow_move_assignable<Buffer>::value,
                  "Buffer 应该支持 noexcept 移动赋值");
    
    std::cout << "\n✓ Buffer 类型通过 noexcept 检查！" << std::endl;
}

// ============================================================
// Rule of Zero 与 Rule of Five 总结
// ============================================================

class ProjectRecord {
public:
    ProjectRecord(std::string name, std::vector<int> samples)
        : name_(std::move(name)), samples_(std::move(samples)) {}

private:
    std::string name_;
    std::vector<int> samples_;
};

void explainRuleOfZero() {
    static_assert(std::is_copy_constructible_v<ProjectRecord>);
    static_assert(std::is_move_constructible_v<ProjectRecord>);
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "先学 Rule of Zero（零规则）" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    std::cout << R"(
普通业务类优先把资源交给 string、vector、unique_ptr 等成员管理。
这样通常无需手写析构、复制或移动，编译器生成的特殊成员会组合成员的正确行为。
只有像下面的 Buffer 一样直接拥有裸资源并定义释放策略时，才进入 Rule of Five。
)" << std::endl;
}

void explainRuleOfFive() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "Rule of Five（规则五）总结" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    std::cout << R"(
如果你需要自定义以下任何一个特殊成员函数，
通常需要自定义全部五个：

1. 析构函数 (Destructor)
   - 释放资源
   - 如果有动态内存，必须定义

2. 拷贝构造函数 (Copy Constructor)
   - 深拷贝资源
   - 格式: T(const T& other)

3. 拷贝赋值运算符 (Copy Assignment Operator)
   - 深拷贝并释放旧资源
   - 格式: T& operator=(const T& other)

4. 移动构造函数 (Move Constructor)
   - 窃取资源
   - 格式: T(T&& other)，只有确实不抛时才加 noexcept
   - 必须让源对象满足该类型的 moved-from 契约；置空只是常见选择

5. 移动赋值运算符 (Move Assignment Operator)
   - 窃取资源并释放旧资源
   - 格式: T& operator=(T&& other)，只有确实不抛时才加 noexcept
   - 必须让源对象保持有效；不要求所有类型都为空

最佳实践：
- 使用 = default 显式要求编译器生成默认实现
- 使用 = delete 禁止拷贝或移动
- 只有确实不会抛异常时，移动操作才应标记为 noexcept
- 标准库对象移动后通常有效但状态未指定；类型可额外承诺更具体状态
)" << std::endl;
}

// ============================================================
// 禁止拷贝/移动的示例
// ============================================================

class NonCopyable {
public:
    NonCopyable() = default;
    
    // 禁止拷贝
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
    
    // 允许移动（可以同时禁止）
    NonCopyable(NonCopyable&&) = default;
    NonCopyable& operator=(NonCopyable&&) = default;
};

class NonMovable {
public:
    NonMovable() = default;
    
    // 允许拷贝
    NonMovable(const NonMovable&) = default;
    NonMovable& operator=(const NonMovable&) = default;
    
    // 禁止移动
    NonMovable(NonMovable&&) = delete;
    NonMovable& operator=(NonMovable&&) = delete;
};

class Singleton {
public:
    static Singleton& instance() {
        static Singleton inst;
        return inst;
    }
    
    // 单例模式：禁止拷贝和移动
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;
    
private:
    Singleton() = default;
};

// ============================================================
// 入口函数
// ============================================================

/**
 * @brief 移动构造函数演示入口函数
 */
void run_move_constructor_demo() {
    explainRuleOfZero();
    demonstrateMoveConstructor();
    demonstrateNoexcept();
    explainRuleOfFive();
}

bool verify_buffer_contract() {
    static_assert(std::is_nothrow_move_constructible_v<Buffer>);
    BufferThrowingStreamBuffer throwingBuffer;
    std::streambuf* const originalBuffer = std::cout.rdbuf(&throwingBuffer);
    const std::ios::iostate originalExceptions = std::cout.exceptions();
    std::cout.exceptions(std::ios::badbit | std::ios::failbit);

    bool passed = false;
    try {
        Buffer source(3, "test_source");
        source.set(0, 7);
        source.set(1, 8);
        source.set(2, 9);

        Buffer copied(1, "test_target");
        copied = source;
        const bool copiedData = copied.size() == 3 && copied.get(0) == 7 &&
                                copied.get(2) == 9;

        Buffer moved(std::move(source));
        const bool movedData = moved.size() == 3 && moved.get(1) == 8;
        const bool sourceContract = source.size() == 0 && !source.hasData();
        source = Buffer(1, "reused_source");
        passed = copiedData && movedData && sourceContract && source.size() == 1;
    } catch (...) {
        passed = false;
    }

    std::cout.exceptions(std::ios::goodbit);
    std::cout.clear();
    std::cout.rdbuf(originalBuffer);
    std::cout.exceptions(originalExceptions);
    return passed;
}
