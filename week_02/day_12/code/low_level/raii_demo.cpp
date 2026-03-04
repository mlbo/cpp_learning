/**
 * @file raii_demo.cpp
 * @brief RAII原则演示
 * 
 * 本文件展示：
 * 1. RAII 核心思想
 * 2. 资源管理类实现
 * 3. 异常安全保证
 * 4. 常见 RAII 模式
 */

#include <iostream>
#include <fstream>
#include <mutex>
#include <memory>
#include <vector>
#include <stdexcept>

// ============================================
// 辅助打印
// ============================================

#define PRINT_SECTION(title) std::cout << "\n=== " << title << " ===\n"

// ============================================
// 第一部分：RAII 核心思想
// ============================================

void demo_raii_concept() {
    PRINT_SECTION("1. RAII 核心思想");
    
    std::cout << R"(
  RAII = Resource Acquisition Is Initialization
  
  核心原则：
  ┌─────────────────────────────────────────────────────────┐
  │  1. 资源获取 = 对象构造                                 │
  │  2. 资源释放 = 对象析构                                 │
  │  3. 生命周期绑定 = 资源与对象同生共死                    │
  │  4. 自动管理 = 利用栈展开机制                           │
  └─────────────────────────────────────────────────────────┘
  
  关键优势：
  • 异常安全：即使抛出异常也能正确释放资源
  • 代码简洁：无需手动释放，减少错误
  • 可预测：资源生命周期与作用域一致
)";
}

// ============================================
// 第二部分：资源管理类实现
// ============================================

// 2.1 文件句柄 RAII
class FileHandle {
public:
    explicit FileHandle(const char* filename, const char* mode)
        : file_(std::fopen(filename, mode)) {
        if (!file_) {
            throw std::runtime_error("无法打开文件");
        }
        std::cout << "  [FileHandle] 打开文件\n";
    }
    
    ~FileHandle() {
        if (file_) {
            std::fclose(file_);
            std::cout << "  [FileHandle] 关闭文件\n";
        }
    }
    
    // 禁止拷贝
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;
    
    // 支持移动
    FileHandle(FileHandle&& other) noexcept : file_(other.file_) {
        other.file_ = nullptr;
    }
    
    FileHandle& operator=(FileHandle&& other) noexcept {
        if (this != &other) {
            if (file_) std::fclose(file_);
            file_ = other.file_;
            other.file_ = nullptr;
        }
        return *this;
    }
    
    // 资源访问
    FILE* get() const { return file_; }
    explicit operator bool() const { return file_ != nullptr; }
    
    void write(const std::string& content) {
        if (file_) {
            std::fprintf(file_, "%s", content.c_str());
        }
    }
    
private:
    FILE* file_;
};

// 2.2 动态数组 RAII
template<typename T>
class DynamicArray {
public:
    explicit DynamicArray(size_t size) 
        : data_(new T[size]()), size_(size) {
        std::cout << "  [DynamicArray] 分配 " << size << " 个元素\n";
    }
    
    ~DynamicArray() {
        delete[] data_;
        std::cout << "  [DynamicArray] 释放数组\n";
    }
    
    // 禁止拷贝
    DynamicArray(const DynamicArray&) = delete;
    DynamicArray& operator=(const DynamicArray&) = delete;
    
    // 支持移动
    DynamicArray(DynamicArray&& other) noexcept
        : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;
        other.size_ = 0;
    }
    
    T& operator[](size_t index) { return data_[index]; }
    const T& operator[](size_t index) const { return data_[index]; }
    size_t size() const { return size_; }
    T* data() { return data_; }
    
private:
    T* data_;
    size_t size_;
};

// 2.3 锁 RAII
class ScopedLock {
public:
    explicit ScopedLock(std::mutex& mtx) : mutex_(mtx) {
        mutex_.lock();
        std::cout << "  [ScopedLock] 加锁\n";
    }
    
    ~ScopedLock() {
        mutex_.unlock();
        std::cout << "  [ScopedLock] 解锁\n";
    }
    
    // 禁止拷贝
    ScopedLock(const ScopedLock&) = delete;
    ScopedLock& operator=(const ScopedLock&) = delete;
    
private:
    std::mutex& mutex_;
};

void demo_resource_classes() {
    PRINT_SECTION("2. 资源管理类实现");
    
    // 2.1 文件句柄
    std::cout << "\n--- 2.1 FileHandle ---\n";
    {
        FileHandle file("/tmp/raii_test.txt", "w");
        file.write("Hello, RAII!\n");
        std::cout << "  写入内容到文件\n";
        // 离开作用域自动关闭
    }
    std::cout << "  已离开作用域\n";
    
    // 2.2 动态数组
    std::cout << "\n--- 2.2 DynamicArray ---\n";
    {
        DynamicArray<int> arr(5);
        for (size_t i = 0; i < arr.size(); ++i) {
            arr[i] = static_cast<int>(i * 10);
        }
        std::cout << "  数组元素: ";
        for (size_t i = 0; i < arr.size(); ++i) {
            std::cout << arr[i] << " ";
        }
        std::cout << "\n";
    }
    
    // 2.3 锁
    std::cout << "\n--- 2.3 ScopedLock ---\n";
    {
        std::mutex mtx;
        ScopedLock lock(mtx);
        std::cout << "  临界区代码执行\n";
        // 离开作用域自动解锁
    }
}

// ============================================
// 第三部分：异常安全保证
// ============================================

void riskyOperation(int value) {
    if (value < 0) {
        throw std::invalid_argument("值不能为负");
    }
    std::cout << "  操作成功执行\n";
}

// 不安全的代码（可能泄漏）
void unsafe_code() {
    std::cout << "\n--- 不安全的代码 ---\n";
    int* ptr = new int(42);
    std::cout << "  分配内存: " << ptr << "\n";
    
    riskyOperation(-1);  // 抛出异常
    
    delete ptr;  // 永远不会执行！
    std::cout << "  释放内存（此行不会执行）\n";
}

// 安全的代码（RAII保证）
void safe_code() {
    std::cout << "\n--- 安全的代码 (RAII) ---\n";
    auto ptr = std::make_unique<int>(42);
    std::cout << "  分配内存: " << ptr.get() << "\n";
    
    riskyOperation(-1);  // 抛出异常
    
    // 不需要手动释放，unique_ptr 会自动处理
    std::cout << "  （此行不会执行，但资源已安全释放）\n";
}

void demo_exception_safety() {
    PRINT_SECTION("3. 异常安全保证");
    
    std::cout << "\n--- 异常安全级别 ---\n";
    std::cout << "  1. 无保证：可能泄漏资源\n";
    std::cout << "  2. 基本保证：异常发生时对象仍有效\n";
    std::cout << "  3. 强保证：异常发生时状态回滚\n";
    std::cout << "  4. 无抛保证：承诺不抛出异常\n";
    
    // 演示异常安全
    std::cout << "\n--- 演示异常处理 ---\n";
    
    try {
        unsafe_code();
    } catch (const std::exception& e) {
        std::cout << "  捕获异常: " << e.what() << "\n";
        std::cout << "  注意：上述代码存在内存泄漏！\n";
    }
    
    try {
        safe_code();
    } catch (const std::exception& e) {
        std::cout << "  捕获异常: " << e.what() << "\n";
        std::cout << "  RAII 保证了资源正确释放\n";
    }
}

// ============================================
// 第四部分：常见 RAII 模式
// ============================================

// 4.1 Scope Guard（作用域守卫）
template<typename F>
class ScopeGuard {
public:
    explicit ScopeGuard(F f) : func_(std::move(f)), active_(true) {}
    ~ScopeGuard() { if (active_) func_(); }
    
    void dismiss() { active_ = false; }
    
    // 禁止拷贝
    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;
    
    // 支持移动
    ScopeGuard(ScopeGuard&& other) noexcept
        : func_(std::move(other.func_)), active_(other.active_) {
        other.active_ = false;
    }
    
private:
    F func_;
    bool active_;
};

template<typename F>
ScopeGuard<F> makeScopeGuard(F f) {
    return ScopeGuard<F>(std::move(f));
}

// 4.2 引用计数 RAII
class RefCounted {
public:
    RefCounted() : count_(new size_t(1)), data_(new int(0)) {
        std::cout << "  [RefCounted] 创建资源\n";
    }
    
    ~RefCounted() {
        if (--(*count_) == 0) {
            delete data_;
            delete count_;
            std::cout << "  [RefCounted] 释放资源\n";
        }
    }
    
    // 拷贝增加引用
    RefCounted(const RefCounted& other)
        : count_(other.count_), data_(other.data_) {
        ++(*count_);
        std::cout << "  [RefCounted] 引用计数: " << *count_ << "\n";
    }
    
    RefCounted& operator=(const RefCounted& other) {
        if (this != &other) {
            // 先释放当前资源
            if (--(*count_) == 0) {
                delete data_;
                delete count_;
            }
            // 再共享
            count_ = other.count_;
            data_ = other.data_;
            ++(*count_);
        }
        return *this;
    }
    
    int getValue() const { return *data_; }
    void setValue(int v) { *data_ = v; }
    size_t useCount() const { return *count_; }
    
private:
    size_t* count_;
    int* data_;
};

void demo_raii_patterns() {
    PRINT_SECTION("4. 常见 RAII 模式");
    
    // 4.1 Scope Guard
    std::cout << "\n--- 4.1 Scope Guard ---\n";
    {
        std::cout << "  进入作用域\n";
        auto guard = makeScopeGuard([]() {
            std::cout << "  Scope Guard: 退出作用域时执行\n";
        });
        
        std::cout << "  执行某些操作\n";
        
        // 可以取消执行
        // guard.dismiss();
    }
    std::cout << "  已离开作用域\n";
    
    // 4.2 引用计数
    std::cout << "\n--- 4.2 引用计数 ---\n";
    {
        RefCounted obj1;
        obj1.setValue(100);
        
        {
            RefCounted obj2 = obj1;  // 引用计数 +1
            std::cout << "  obj2 引用计数: " << obj2.useCount() << "\n";
            std::cout << "  obj2 值: " << obj2.getValue() << "\n";
        }
        
        std::cout << "  obj1 引用计数: " << obj1.useCount() << "\n";
    }
    
    // 4.3 标准库 RAII 类
    std::cout << "\n--- 4.3 标准库 RAII 类 ---\n";
    std::cout << "  • std::unique_ptr：独占所有权\n";
    std::cout << "  • std::shared_ptr：共享所有权\n";
    std::cout << "  • std::weak_ptr：弱引用\n";
    std::cout << "  • std::lock_guard：互斥锁\n";
    std::cout << "  • std::unique_lock：灵活锁\n";
    std::cout << "  • std::scoped_lock (C++17)：多锁\n";
    std::cout << "  • std::fstream：文件流\n";
    std::cout << "  • std::vector：动态数组\n";
    std::cout << "  • std::string：字符串\n";
}

// ============================================
// 第五部分：RAII 设计原则
// ============================================

void print_raii_guidelines() {
    PRINT_SECTION("5. RAII 设计原则");
    
    std::cout << R"(
  ┌─────────────────────────────────────────────────────────────┐
  │                    RAII 设计原则                            │
  ├─────────────────────────────────────────────────────────────┤
  │                                                             │
  │  1. 构造函数获取资源                                        │
  │     • 在构造函数中分配/获取资源                             │
  │     • 如果获取失败，抛出异常                                │
  │                                                             │
  │  2. 析构函数释放资源                                        │
  │     • 在析构函数中释放资源                                  │
  │     • 析构函数不应抛出异常                                  │
  │                                                             │
  │  3. 禁止拷贝或实现深拷贝                                    │
  │     • 删除拷贝构造/赋值运算符                               │
  │     • 或实现正确的深拷贝语义                                │
  │                                                             │
  │  4. 支持移动语义                                            │
  │     • 实现移动构造/赋值运算符                               │
  │     • 转移资源所有权                                        │
  │                                                             │
  │  5. 提供资源访问接口                                        │
  │     • get() 返回原始资源                                    │
  │     • operator* / operator->                                │
  │                                                             │
  └─────────────────────────────────────────────────────────────┘

  RAII 类模板：
  template<typename T>
  class RAIIWrapper {
  public:
      explicit RAIIWrapper(/* 参数 */) : resource_(/* 获取 */) {}
      
      ~RAIIWrapper() { /* 释放资源 */ }
      
      // 禁止拷贝
      RAIIWrapper(const RAIIWrapper&) = delete;
      RAIIWrapper& operator=(const RAIIWrapper&) = delete;
      
      // 支持移动
      RAIIWrapper(RAIIWrapper&&) noexcept;
      RAIIWrapper& operator=(RAIIWrapper&&) noexcept;
      
      // 资源访问
      T* get() const { return resource_; }
      T& operator*() const { return *resource_; }
      T* operator->() const { return resource_; }
      
  private:
      T* resource_;
  };
)";
}

// ============================================
// 主函数
// ============================================

void run_raii_demo() {
    std::cout << "\n========================================\n";
    std::cout << "  RAII 原则演示\n";
    std::cout << "========================================\n";
    
    demo_raii_concept();
    demo_resource_classes();
    demo_exception_safety();
    demo_raii_patterns();
    print_raii_guidelines();
    
    std::cout << "\n========================================\n";
    std::cout << "  RAII 演示完毕\n";
    std::cout << "========================================\n";
}
