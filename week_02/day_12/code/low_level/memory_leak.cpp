/**
 * @file memory_leak.cpp
 * @brief 内存泄漏检测与预防
 * 
 * 本文件展示：
 * 1. 内存泄漏的常见原因
 * 2. 检测方法与工具
 * 3. 预防最佳实践
 */

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <fstream>

// ============================================
// 辅助打印
// ============================================

#define PRINT_SECTION(title) std::cout << "\n=== " << title << " ===\n"

// ============================================
// 第一部分：内存泄漏的常见原因
// ============================================

class Resource {
public:
    explicit Resource(int id) : id_(id) {
        std::cout << "  Resource(" << id_ << ") 构造\n";
    }
    ~Resource() {
        std::cout << "  Resource(" << id_ << ") 析构\n";
    }
private:
    int id_;
};

// 泄漏原因1：忘记释放
void leak_forget_delete() {
    std::cout << "\n--- 原因1: 忘记 delete ---\n";
    Resource* p = new Resource(1);
    std::cout << "  创建了资源，但忘记释放\n";
    // delete p;  // 注释掉，造成泄漏
    std::cout << "  【泄漏】资源未释放\n";
}

// 泄漏原因2：异常导致跳过释放
void leak_exception() {
    std::cout << "\n--- 原因2: 异常导致跳过 delete ---\n";
    Resource* p = new Resource(2);
    try {
        std::cout << "  执行可能抛出异常的操作...\n";
        throw std::runtime_error("模拟异常");
        delete p;  // 永远不会执行
    } catch (const std::exception& e) {
        std::cout << "  捕获异常: " << e.what() << "\n";
        std::cout << "  【泄漏】delete 被跳过\n";
    }
}

// 泄漏原因3：指针覆盖
void leak_pointer_overwrite() {
    std::cout << "\n--- 原因3: 指针被覆盖 ---\n";
    Resource* p = new Resource(3);
    std::cout << "  分配资源 id=3\n";
    
    p = new Resource(4);  // 覆盖指针，原资源泄漏
    std::cout << "  分配资源 id=4，覆盖了原指针\n";
    std::cout << "  【泄漏】资源 id=3 丢失引用\n";
    
    delete p;  // 只释放了 id=4 的资源
}

// 泄漏原因4：循环引用
class Node {
public:
    explicit Node(int v) : value_(v) {
        std::cout << "  Node(" << value_ << ") 构造\n";
    }
    ~Node() {
        std::cout << "  Node(" << value_ << ") 析构\n";
    }
    
    std::shared_ptr<Node> next;
    std::shared_ptr<Node> prev;  // 强引用，造成循环引用
    
    int value() const { return value_; }
private:
    int value_;
};

void leak_circular_reference() {
    std::cout << "\n--- 原因4: shared_ptr 循环引用 ---\n";
    {
        auto node1 = std::make_shared<Node>(10);
        auto node2 = std::make_shared<Node>(11);
        
        node1->next = node2;
        node2->prev = node1;  // 循环引用！
        
        std::cout << "  离开作用域...\n";
        // 两个节点都不会被释放！
    }
    std::cout << "  【泄漏】循环引用导致无法释放\n";
}

// 正确做法：使用 weak_ptr 打破循环
class SafeNode : public std::enable_shared_from_this<SafeNode> {
public:
    explicit SafeNode(int v) : value_(v) {
        std::cout << "  SafeNode(" << value_ << ") 构造\n";
    }
    ~SafeNode() {
        std::cout << "  SafeNode(" << value_ << ") 析构\n";
    }
    
    std::shared_ptr<SafeNode> next;
    std::weak_ptr<SafeNode> prev;  // 弱引用，打破循环
    
    int value() const { return value_; }
private:
    int value_;
};

void no_leak_weak_ptr() {
    std::cout << "\n--- 正确做法: 使用 weak_ptr ---\n";
    {
        auto node1 = std::make_shared<SafeNode>(20);
        auto node2 = std::make_shared<SafeNode>(21);
        
        node1->next = node2;
        node2->prev = node1;  // weak_ptr，不会增加引用计数
        
        std::cout << "  离开作用域...\n";
        // 正确释放
    }
    std::cout << "  【正确】资源已释放\n";
}

void demo_leak_causes() {
    PRINT_SECTION("1. 内存泄漏的常见原因");
    
    std::cout << "\n  注意：以下演示会造成内存泄漏，仅作教学目的\n";
    
    leak_forget_delete();
    leak_exception();
    leak_pointer_overwrite();
    leak_circular_reference();
    no_leak_weak_ptr();
    
    std::cout << R"(
  ┌─────────────────────────────────────────────────────────┐
  │                 内存泄漏常见原因总结                     │
  ├─────────────────────────────────────────────────────────┤
  │  1. 忘记释放：new 后没有对应的 delete                   │
  │  2. 异常跳过：异常发生时跳过了 delete                   │
  │  3. 指针覆盖：指针被新值覆盖，丢失原资源                │
  │  4. 循环引用：shared_ptr 形成循环                       │
  │  5. 错误删除：delete[] vs delete 不匹配                 │
  │  6. 多重释放：同一资源被多次 delete                     │
  └─────────────────────────────────────────────────────────┘
)";
}

// ============================================
// 第二部分：检测方法
// ============================================

// 简单的内存追踪器（演示用）
class MemoryTracker {
public:
    static size_t allocations;
    static size_t deallocations;
    
    static void* trackNew(size_t size) {
        allocations++;
        return std::malloc(size);
    }
    
    static void trackDelete(void* p) {
        deallocations++;
        std::free(p);
    }
    
    static void report() {
        std::cout << "  分配次数: " << allocations << "\n";
        std::cout << "  释放次数: " << deallocations << "\n";
        if (allocations > deallocations) {
            std::cout << "  【警告】可能存在内存泄漏!\n";
        } else {
            std::cout << "  【正常】分配与释放平衡\n";
        }
    }
};

size_t MemoryTracker::allocations = 0;
size_t MemoryTracker::deallocations = 0;

void demo_detection_methods() {
    PRINT_SECTION("2. 内存泄漏检测方法");
    
    // 2.1 手动追踪（简单场景）
    std::cout << "\n--- 2.1 手动追踪 ---\n";
    std::cout << "  原理：重载 new/delete 记录分配/释放次数\n";
    std::cout << "  优点：简单直观\n";
    std::cout << "  缺点：需要修改代码，不适用于复杂场景\n";
    
    // 2.2 工具检测
    std::cout << "\n--- 2.2 常用检测工具 ---\n";
    std::cout << R"(
  ┌─────────────────────────────────────────────────────────────┐
  │                    内存泄漏检测工具                          │
  ├───────────────┬─────────────────────────────────────────────┤
  │ 工具          │ 说明                                        │
  ├───────────────┼─────────────────────────────────────────────┤
  │ Valgrind      │ Linux 下最强大的内存检测工具                │
  │               │ 使用: valgrind --leak-check=full ./prog     │
  ├───────────────┼─────────────────────────────────────────────┤
  │ AddressSanitizer │ 编译器内置，性能开销小                   │
  │ (ASan)        │ 编译: g++ -fsanitize=address -g             │
  ├───────────────┼─────────────────────────────────────────────┤
  │ Dr. Memory    │ Windows 下的内存检测工具                    │
  ├───────────────┼─────────────────────────────────────────────┤
  │ Visual Studio │ MSVC 内置调试器堆检查                       │
  │ Debug Heap    │                                             │
  ├───────────────┼─────────────────────────────────────────────┤
  │ Leaks         │ macOS/iOS 内存泄漏检测                      │
  │ Instruments   │                                             │
  └───────────────┴─────────────────────────────────────────────┘
)";
    
    // 2.3 编译时检测
    std::cout << "\n--- 2.3 编译时检测 ---\n";
    std::cout << "  静态分析工具:\n";
    std::cout << "    • Clang Static Analyzer\n";
    std::cout << "    • Cppcheck\n";
    std::cout << "    • PVS-Studio\n";
    std::cout << "    • Visual Studio Code Analysis\n";
}

// ============================================
// 第三部分：预防最佳实践
// ============================================

void demo_best_practices() {
    PRINT_SECTION("3. 内存泄漏预防最佳实践");
    
    std::cout << "\n--- 3.1 使用智能指针 ---\n";
    {
        // 安全：使用 unique_ptr
        auto p1 = std::make_unique<Resource>(100);
        auto p2 = std::make_unique<Resource>(101);
        
        std::cout << "  智能指针自动管理内存\n";
        // 无需手动释放
    }
    std::cout << "  离开作用域，自动释放\n";
    
    std::cout << "\n--- 3.2 使用 RAII 封装 ---\n";
    {
        std::vector<int> vec(100);
        std::string str = "Hello";
        
        std::cout << "  标准容器自动管理内存\n";
        std::cout << "  vector 大小: " << vec.size() << "\n";
        std::cout << "  string: " << str << "\n";
        // 无需手动释放
    }
    
    std::cout << "\n--- 3.3 遵循所有权原则 ---\n";
    std::cout << "  1. 明确每个资源的所有者\n";
    std::cout << "  2. 所有者负责资源的释放\n";
    std::cout << "  3. 避免多个指针'拥有'同一资源\n";
    std::cout << "  4. 非拥有指针使用裸指针或 weak_ptr\n";
    
    std::cout << "\n--- 3.4 代码审查检查点 ---\n";
    std::cout << R"(
  □ 每个 new 是否有对应的 delete?
  □ 每个 malloc 是否有对应的 free?
  □ delete[] 与 delete 是否匹配?
  □ 异常路径是否正确释放资源?
  □ 是否存在循环引用?
  □ 是否使用智能指针?
  □ 是否遵循 RAII 原则?
)";
}

// ============================================
// 第四部分：AddressSanitizer 示例
// ============================================

void demo_asan_usage() {
    PRINT_SECTION("4. AddressSanitizer 使用");
    
    std::cout << "\n--- 编译命令 ---\n";
    std::cout << "  g++ -fsanitize=address -g -O1 -fno-omit-frame-pointer \\\n";
    std::cout << "      memory_leak.cpp -o memory_leak\n";
    std::cout << "\n  ./memory_leak\n";
    
    std::cout << "\n--- 检测能力 ---\n";
    std::cout << "  • 堆缓冲区溢出\n";
    std::cout << "  • 栈缓冲区溢出\n";
    std::cout << "  • 全局缓冲区溢出\n";
    std::cout << "  • Use after free\n";
    std::cout << "  • Double free\n";
    std::cout << "  • Memory leaks\n";
    
    std::cout << "\n--- Valgrind 使用 ---\n";
    std::cout << "  valgrind --leak-check=full \\\n";
    std::cout << "           --show-leak-kinds=all \\\n";
    std::cout << "           --track-origins=yes \\\n";
    std::cout << "           ./your_program\n";
}

// ============================================
// 第五部分：内存管理总结
// ============================================

void print_summary() {
    PRINT_SECTION("5. 内存管理总结");
    
    std::cout << R"(
  ┌─────────────────────────────────────────────────────────────┐
  │                    内存管理最佳实践                          │
  ├─────────────────────────────────────────────────────────────┤
  │                                                             │
  │  优先级顺序：                                               │
  │  1. 栈对象 > 智能指针 > 裸指针                             │
  │  2. 标准容器 > 手动数组                                    │
  │  3. RAII > 手动管理                                        │
  │                                                             │
  │  黄金法则：                                                 │
  │  • 谁申请，谁释放                                          │
  │  • 使用 RAII 封装所有资源                                  │
  │  • 优先使用智能指针                                        │
  │  • 避免显式 new/delete                                     │
  │  • 打破循环引用用 weak_ptr                                 │
  │                                                             │
  │  开发流程：                                                 │
  │  • 编码时：遵循 RAII 原则                                  │
  │  • 编译时：启用静态分析                                    │
  │  • 测试时：使用 ASan/Valgrind                              │
  │  • 发布前：代码审查内存相关代码                            │
  │                                                             │
  └─────────────────────────────────────────────────────────────┘
)";
}

// ============================================
// 主函数
// ============================================

void run_memory_leak() {
    std::cout << "\n========================================\n";
    std::cout << "  内存泄漏检测与预防\n";
    std::cout << "========================================\n";
    
    demo_leak_causes();
    demo_detection_methods();
    demo_best_practices();
    demo_asan_usage();
    print_summary();
    
    std::cout << "\n========================================\n";
    std::cout << "  内存泄漏演示完毕\n";
    std::cout << "========================================\n";
}
