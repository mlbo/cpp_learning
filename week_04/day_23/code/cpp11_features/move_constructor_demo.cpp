/**
 * @file move_constructor_demo.cpp
 * @brief 移动构造函数和移动赋值运算符详解
 * 
 * 本文件演示：
 * 1. 规则五（Rule of Five）的实现
 * 2. noexcept 关键字的重要性
 * 3. 移动操作的最佳实践
 */

#include <iostream>
#include <utility>
#include <algorithm>

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
    Buffer() : data_(nullptr), size_(0), name_("unnamed") {
        std::cout << "  [默认构造] " << name_ << ": 空缓冲区" << std::endl;
    }
    
    // 带大小的构造函数
    explicit Buffer(size_t size, const char* name = "buffer")
        : data_(size > 0 ? new int[size]{} : nullptr)
        , size_(size)
        , name_(name) {
        std::cout << "  [构造] " << name_ << ": 分配 " << size_ << " 个元素" << std::endl;
    }
    
    // 2. 析构函数
    ~Buffer() {
        if (data_) {
            std::cout << "  [析构] " << name_ << ": 释放 " << size_ << " 个元素" << std::endl;
            delete[] data_;
        } else {
            std::cout << "  [析构] " << name_ << ": 空缓冲区" << std::endl;
        }
    }
    
    // 3. 拷贝构造函数
    Buffer(const Buffer& other)
        : data_(other.size_ > 0 ? new int[other.size_] : nullptr)
        , size_(other.size_)
        , name_("copied") {
        if (data_) {
            std::copy(other.data_, other.data_ + size_, data_);
        }
        std::cout << "  [拷贝构造] 从 " << other.name_ 
                  << " 复制 " << size_ << " 个元素" << std::endl;
    }
    
    // 4. 拷贝赋值运算符
    Buffer& operator=(const Buffer& other) {
        if (this != &other) {
            // 先释放原有资源
            delete[] data_;
            
            // 分配新资源并复制
            size_ = other.size_;
            data_ = size_ > 0 ? new int[size_] : nullptr;
            if (data_) {
                std::copy(other.data_, other.data_ + size_, data_);
            }
            name_ = "copy_assigned";
            
            std::cout << "  [拷贝赋值] 从 " << other.name_
                      << " 复制 " << size_ << " 个元素" << std::endl;
        }
        return *this;
    }
    
    // 5. 移动构造函数 - 核心重点
    // noexcept 声明非常重要！它影响容器的行为
    Buffer(Buffer&& other) noexcept
        : data_(other.data_)
        , size_(other.size_)
        , name_("moved") {
        // 关键：源对象置空
        other.data_ = nullptr;
        other.size_ = 0;
        other.name_ = "moved_from";
        
        std::cout << "  [移动构造] 从 " << other.name_ 
                  << " 移动资源（" << size_ << " 个元素）" << std::endl;
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
            
            // 源对象置空
            other.data_ = nullptr;
            other.size_ = 0;
            other.name_ = "moved_from";
            
            std::cout << "  [移动赋值] 从 " << other.name_
                      << " 移动资源（" << size_ << " 个元素）" << std::endl;
        }
        return *this;
    }
    
    // 辅助方法
    size_t size() const { return size_; }
    bool valid() const { return data_ != nullptr; }
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
    std::cout << "  buf1: " << (buf1.valid() ? "有效" : "无效") 
              << ", 名称: " << buf1.name() << std::endl;
    std::cout << "  buf2: " << (buf2.valid() ? "有效" : "无效")
              << ", 名称: " << buf2.name() << std::endl;
    
    if (buf2.valid()) {
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
    
    std::cout << "  buf5 仍然有效: " << (buf5.valid() ? "是" : "否") << std::endl;
}

// ============================================================
// noexcept 的重要性演示
// ============================================================

void demonstrateNoexcept() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "noexcept 关键字的重要性" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    std::cout << R"(
为什么移动构造函数应该标记为 noexcept？

1. 容器优化：
   - std::vector 扩容时，如果移动构造函数是 noexcept，
     会使用移动而非拷贝
   - 这可以显著提升性能

2. 强异常保证：
   - 如果移动构造抛出异常，可能导致数据丢失
   - noexcept 表示"不会失败"，是重要的契约

3. 标准库要求：
   - 许多标准库算法和容器要求移动操作不抛出异常
   - 例如 std::vector::reserve 对类型的要求

代码示例：
  // 推荐：标记为 noexcept
  Buffer(Buffer&& other) noexcept;
  Buffer& operator=(Buffer&& other) noexcept;
  
  // 不推荐：可能抛出异常的移动操作
  // Buffer(Buffer&& other);  // 缺少 noexcept
)" << std::endl;
    
    // 使用 static_assert 检查
    static_assert(std::is_nothrow_move_constructible<Buffer>::value,
                  "Buffer 应该支持 noexcept 移动构造");
    static_assert(std::is_nothrow_move_assignable<Buffer>::value,
                  "Buffer 应该支持 noexcept 移动赋值");
    
    std::cout << "\n✓ Buffer 类型通过 noexcept 检查！" << std::endl;
}

// ============================================================
// Rule of Five 总结
// ============================================================

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
   - 格式: T(T&& other) noexcept
   - 必须将源对象置空

5. 移动赋值运算符 (Move Assignment Operator)
   - 窃取资源并释放旧资源
   - 格式: T& operator=(T&& other) noexcept
   - 必须将源对象置空

最佳实践：
- 使用 = default 显式要求编译器生成默认实现
- 使用 = delete 禁止拷贝或移动
- 移动操作应标记为 noexcept
- 移动后的对象应处于"有效但未定义"状态
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
    demonstrateMoveConstructor();
    demonstrateNoexcept();
    explainRuleOfFive();
}
