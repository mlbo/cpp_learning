/**
 * @file heap_memory.cpp
 * @brief 堆内存管理详解
 * 
 * 本文件展示：
 * 1. new/delete 与 malloc/free 的区别
 * 2. 内存分配原理
 * 3. 内存对齐与布局
 * 4. placement new
 */

#include <iostream>
#include <cstdlib>
#include <new>
#include <cstring>

// ============================================
// 辅助打印
// ============================================

#define PRINT_SECTION(title) std::cout << "\n=== " << title << " ===\n"

// 自定义 new/delete 追踪（演示用）
// 注意：实际项目中应使用 Valgrind 或 AddressSanitizer

void* tracked_new(size_t size) {
    void* p = std::malloc(size);
    std::cout << "  [分配] " << size << " 字节 @ " << p << "\n";
    return p;
}

void tracked_delete(void* p) {
    std::cout << "  [释放] @ " << p << "\n";
    std::free(p);
}

// ============================================
// 第一部分：new/delete vs malloc/free
// ============================================

class DemoClass {
public:
    DemoClass(int v) : value(v) {
        std::cout << "  DemoClass(" << value << ") 构造函数\n";
    }
    
    ~DemoClass() {
        std::cout << "  DemoClass(" << value << ") 析构函数\n";
    }
    
    int getValue() const { return value; }
    
private:
    int value;
};

void demo_new_vs_malloc() {
    PRINT_SECTION("1. new/delete vs malloc/free");
    
    // 1.1 malloc/free - 仅分配内存
    std::cout << "\n--- 1.1 malloc/free ---\n";
    {
        DemoClass* p = (DemoClass*)std::malloc(sizeof(DemoClass));
        std::cout << "  malloc 分配内存，但未调用构造函数\n";
        std::cout << "  value 值未初始化（随机）: " << p->getValue() << "\n";  // 未定义行为
        
        // 需要手动调用构造函数（placement new）
        new(p) DemoClass(42);  // 调用构造函数
        std::cout << "  手动构造后 value: " << p->getValue() << "\n";
        
        // 需要手动调用析构函数
        p->~DemoClass();
        std::free(p);
        std::cout << "  手动析构并释放内存\n";
    }
    
    // 1.2 new/delete - 自动调用构造/析构
    std::cout << "\n--- 1.2 new/delete ---\n";
    {
        DemoClass* p = new DemoClass(100);
        std::cout << "  new 自动调用构造函数\n";
        std::cout << "  value: " << p->getValue() << "\n";
        
        delete p;
        std::cout << "  delete 自动调用析构函数并释放内存\n";
    }
    
    // 1.3 对比表格
    std::cout << "\n--- 1.3 对比总结 ---\n";
    std::cout << R"(
  ┌─────────────┬─────────────────┬─────────────────┐
  │     特性    │   new/delete    │  malloc/free    │
  ├─────────────┼─────────────────┼─────────────────┤
  │ 类型安全    │       ✓         │       ✗         │
  │ 构造/析构   │   自动调用      │    手动处理     │
  │ 错误处理    │  bad_alloc异常  │  返回 nullptr   │
  │ 内存大小    │   自动计算      │   手动计算      │
  │ 可重载      │       ✓         │       ✗         │
  │ 源头        │   C++运算符     │   C标准库函数   │
  └─────────────┴─────────────────┴─────────────────┘
)";
}

// ============================================
// 第二部分：内存分配原理
// ============================================

void demo_memory_allocation() {
    PRINT_SECTION("2. 内存分配原理");
    
    // 2.1 new 表达式分解
    std::cout << "\n--- 2.1 new 表达式分解 ---\n";
    std::cout << "  DemoClass* p = new DemoClass(42);\n";
    std::cout << "  等价于:\n";
    std::cout << "    1. void* mem = operator new(sizeof(DemoClass));\n";
    std::cout << "    2. DemoClass* p = static_cast<DemoClass*>(mem);\n";
    std::cout << "    3. p->DemoClass::DemoClass(42);  // 构造函数\n";
    
    // 2.2 delete 表达式分解
    std::cout << "\n--- 2.2 delete 表达式分解 ---\n";
    std::cout << "  delete p;\n";
    std::cout << "  等价于:\n";
    std::cout << "    1. p->~DemoClass();  // 析构函数\n";
    std::cout << "    2. operator delete(p);  // 释放内存\n";
    
    // 2.3 内存布局
    std::cout << "\n--- 2.3 进程内存布局 ---\n";
    std::cout << R"(
  ┌──────────────────────┐ 高地址
  │     命令行参数        │
  ├──────────────────────┤
  │     环境变量          │
  ├──────────────────────┤
  │     栈 (Stack)        │ ↓ 向下增长
  │         ↓            │
  ├──────────────────────┤
  │                      │
  │         ↑            │
  │     堆 (Heap)         │ ↑ 向上增长
  ├──────────────────────┤
  │   BSS段 (未初始化)    │
  ├──────────────────────┤
  │   数据段 (已初始化)   │
  ├──────────────────────┤
  │   代码段 (Text)       │
  └──────────────────────┘ 低地址
)";
    
    // 2.4 堆内存分配器
    std::cout << "\n--- 2.4 堆内存分配器 ---\n";
    std::cout << "  常见实现:\n";
    std::cout << "    • dlmalloc (Doug Lea's malloc)\n";
    std::cout << "    • ptmalloc (glibc 默认)\n";
    std::cout << "    • jemalloc (FreeBSD/Firefox)\n";
    std::cout << "    • tcmalloc (Google)\n";
    std::cout << "    • mimalloc (Microsoft)\n";
}

// ============================================
// 第三部分：内存对齐
// ============================================

#pragma pack(push, 1)
struct PackedStruct {
    char a;     // 1 byte
    int b;      // 4 bytes
    char c;     // 1 byte
};
#pragma pack(pop)

struct AlignedStruct {
    char a;     // 1 byte + 3 bytes padding
    int b;      // 4 bytes
    char c;     // 1 byte + 3 bytes padding
};

void demo_memory_alignment() {
    PRINT_SECTION("3. 内存对齐");
    
    // 3.1 默认对齐 vs 紧凑对齐
    std::cout << "\n--- 3.1 结构体大小对比 ---\n";
    std::cout << "  sizeof(AlignedStruct) = " << sizeof(AlignedStruct) << " bytes\n";
    std::cout << "  sizeof(PackedStruct) = " << sizeof(PackedStruct) << " bytes\n";
    
    // 3.2 对齐原因
    std::cout << "\n--- 3.2 为什么需要内存对齐 ---\n";
    std::cout << "  1. 硬件访问效率：对齐访问更快\n";
    std::cout << "  2. CPU 架构要求：某些 CPU 必须对齐\n";
    std::cout << "  3. 原子操作要求：原子操作通常需要对齐\n";
    
    // 3.3 使用 alignas 和 alignof
    std::cout << "\n--- 3.3 C++11 对齐控制 ---\n";
    struct alignas(16) Aligned16 {
        int data[4];
    };
    
    std::cout << "  alignof(Aligned16) = " << alignof(Aligned16) << "\n";
    std::cout << "  alignof(int) = " << alignof(int) << "\n";
    std::cout << "  alignof(double) = " << alignof(double) << "\n";
    
    // 3.4 内存布局图
    std::cout << "\n--- 3.4 AlignedStruct 内存布局 ---\n";
    std::cout << R"(
  AlignedStruct (12 bytes):
  ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
  │ a │padding │   b (4 bytes)   │ c │padding │
  └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
   0   1   2   3   4   5   6   7   8   9  10  11
)";
}

// ============================================
// 第四部分：placement new
// ============================================

void demo_placement_new() {
    PRINT_SECTION("4. Placement New");
    
    // 4.1 基本用法
    std::cout << "\n--- 4.1 基本用法 ---\n";
    {
        // 预分配内存
        alignas(DemoClass) char buffer[sizeof(DemoClass)];
        
        std::cout << "  在栈上预分配缓冲区\n";
        
        // placement new：在指定位置构造对象
        DemoClass* p = new(buffer) DemoClass(999);
        
        std::cout << "  使用 placement new 构造对象\n";
        std::cout << "  value: " << p->getValue() << "\n";
        
        // 手动调用析构函数
        p->~DemoClass();
        
        std::cout << "  手动析构（不释放内存）\n";
        // buffer 是栈变量，自动释放
    }
    
    // 4.2 应用场景
    std::cout << "\n--- 4.2 应用场景 ---\n";
    std::cout << "  1. 内存池：预分配大块内存，减少分配次数\n";
    std::cout << "  2. 自定义内存管理：控制对象位置\n";
    std::cout << "  3. 共享内存：在固定地址构造对象\n";
    std::cout << "  4. 性能优化：避免动态分配开销\n";
    
    // 4.3 简单内存池示例
    std::cout << "\n--- 4.3 简单内存池示例 ---\n";
    {
        class MemoryPool {
        public:
            MemoryPool(size_t size) : buffer_(new char[size]), used_(0) {}
            ~MemoryPool() { delete[] buffer_; }
            
            void* allocate(size_t size) {
                if (used_ + size > 1024) return nullptr;
                void* p = buffer_ + used_;
                used_ += size;
                return p;
            }
            
        private:
            char* buffer_;
            size_t used_;
        };
        
        MemoryPool pool(1024);
        
        // 在内存池中构造对象
        void* mem = pool.allocate(sizeof(DemoClass));
        DemoClass* obj = new(mem) DemoClass(777);
        
        std::cout << "  在内存池中构造对象 value=" << obj->getValue() << "\n";
        
        obj->~DemoClass();
        // 内存池析构时统一释放所有内存
    }
}

// ============================================
// 第五部分：operator new/delete 重载
// ============================================

// 全局 operator new 重载（演示用，实际项目中谨慎使用）
/*
void* operator new(size_t size) {
    std::cout << "  [全局 new] 分配 " << size << " 字节\n";
    return std::malloc(size);
}

void operator delete(void* p) noexcept {
    std::cout << "  [全局 delete] 释放 @ " << p << "\n";
    std::free(p);
}
*/

class TrackedObject {
public:
    static size_t allocationCount;
    static size_t deallocationCount;
    
    void* operator new(size_t size) {
        allocationCount++;
        std::cout << "  [类重载 new] 分配 " << size << " 字节\n";
        return ::operator new(size);
    }
    
    void operator delete(void* p) {
        deallocationCount++;
        std::cout << "  [类重载 delete] 释放 @ " << p << "\n";
        ::operator delete(p);
    }
    
    int data = 42;
};

size_t TrackedObject::allocationCount = 0;
size_t TrackedObject::deallocationCount = 0;

void demo_operator_overload() {
    PRINT_SECTION("5. operator new/delete 重载");
    
    // 5.1 类级别重载
    std::cout << "\n--- 5.1 类级别重载 ---\n";
    {
        TrackedObject* obj = new TrackedObject;
        std::cout << "  obj->data = " << obj->data << "\n";
        delete obj;
    }
    
    std::cout << "\n  分配次数: " << TrackedObject::allocationCount << "\n";
    std::cout << "  释放次数: " << TrackedObject::deallocationCount << "\n";
    
    // 5.2 注意事项
    std::cout << "\n--- 5.2 重载注意事项 ---\n";
    std::cout << "  1. 全局重载影响整个程序，谨慎使用\n";
    std::cout << "  2. 类重载只影响该类的对象\n";
    std::cout << "  3. 需要同时重载 new 和 delete\n";
    std::cout << "  4. 重载版本应调用默认版本或 malloc/free\n";
}

// ============================================
// 主函数
// ============================================

void run_heap_memory() {
    std::cout << "\n========================================\n";
    std::cout << "  堆内存管理详解\n";
    std::cout << "========================================\n";
    
    demo_new_vs_malloc();
    demo_memory_allocation();
    demo_memory_alignment();
    demo_placement_new();
    demo_operator_overload();
    
    std::cout << "\n========================================\n";
    std::cout << "  堆内存管理演示完毕\n";
    std::cout << "========================================\n";
}
