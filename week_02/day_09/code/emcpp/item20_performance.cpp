/**
 * @file item20_performance.cpp
 * @brief EMC++ 条款20：shared_ptr 的性能考虑
 * 
 * 核心要点：
 * 1. shared_ptr 的大小是裸指针的两倍
 * 2. 控制块需要动态分配内存
 * 3. 引用计数操作是原子的（有开销）
 * 4. make_shared 可以减少内存分配次数
 * 5. 移动语义比拷贝更高效
 */

#include <iostream>
#include <memory>
#include <chrono>
#include <vector>
#include <string>
#include <iomanip>

// ============================================================
// 要点1：shared_ptr 的大小
// ============================================================

void demoSizeComparison() {
    std::cout << "【要点1：shared_ptr 的大小】\n\n";
    
    std::cout << "  各类指针的大小比较：\n";
    std::cout << "  ┌─────────────────────────────────────────────────┐\n";
    std::cout << "  │ 裸指针 (int*):              " << sizeof(int*) << " 字节               │\n";
    std::cout << "  │ unique_ptr<int>:            " << sizeof(std::unique_ptr<int>) << " 字节               │\n";
    std::cout << "  │ shared_ptr<int>:            " << sizeof(std::shared_ptr<int>) << " 字节               │\n";
    std::cout << "  │ weak_ptr<int>:              " << sizeof(std::weak_ptr<int>) << " 字节               │\n";
    std::cout << "  └─────────────────────────────────────────────────┘\n";
    
    std::cout << "\n  shared_ptr 内部结构：\n";
    std::cout << "  • 指向对象的指针（1个机器字）\n";
    std::cout << "  • 指向控制块的指针（1个机器字）\n";
    std::cout << "  总计：2个指针大小\n";
    
    std::cout << "\n  对比：unique_ptr 零开销抽象\n";
    std::cout << "  • unique_ptr 大小等于裸指针\n";
    std::cout << "  • 没有额外的控制块\n";
}

// ============================================================
// 要点2：控制块的内存分配
// ============================================================

class TrackedObject {
public:
    TrackedObject(int v) : value(v) {
        // std::cout << "构造 " << value << "\n";
    }
    ~TrackedObject() {
        // std::cout << "析构 " << value << "\n";
    }
    int value;
};

void demoControlBlockAllocation() {
    std::cout << "\n【要点2：控制块的内存分配】\n\n";
    
    std::cout << "--- 方法1：new + shared_ptr（两次分配）---\n";
    std::cout << "  std::shared_ptr<T>(new T(...))\n";
    std::cout << "  分配1: 为 T 分配内存\n";
    std::cout << "  分配2: 为控制块分配内存\n\n";
    
    std::cout << "--- 方法2：make_shared（一次分配）---\n";
    std::cout << "  std::make_shared<T>(...)\n";
    std::cout << "  分配: 一次分配，同时容纳 T 和控制块\n\n";
    
    std::cout << "  内存布局对比：\n";
    std::cout << "  ┌─────────────────────────────────────────┐\n";
    std::cout << "  │ 方法1 (new):                            │\n";
    std::cout << "  │   [对象] ... [控制块]  (两块独立内存)     │\n";
    std::cout << "  ├─────────────────────────────────────────┤\n";
    std::cout << "  │ 方法2 (make_shared):                    │\n";
    std::cout << "  │   [控制块|对象]  (单块连续内存)           │\n";
    std::cout << "  └─────────────────────────────────────────┘\n";
}

// ============================================================
// 要点3：性能基准测试
// ============================================================

// 计时辅助类
class Timer {
public:
    Timer() : start_(std::chrono::high_resolution_clock::now()) {}
    
    double elapsedMs() const {
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(end - start_).count();
    }
    
private:
    std::chrono::high_resolution_clock::time_point start_;
};

void demoPerformanceBenchmark() {
    std::cout << "\n【要点3：性能基准测试】\n\n";
    
    const int N = 100000;
    
    // 测试1：创建和销毁
    std::cout << "--- 创建/销毁 " << N << " 个对象 ---\n";
    
    // unique_ptr
    {
        Timer t;
        for (int i = 0; i < N; ++i) {
            auto p = std::make_unique<TrackedObject>(i);
        }
        std::cout << "  unique_ptr:      " << std::fixed << std::setprecision(2) 
                  << t.elapsedMs() << " ms\n";
    }
    
    // shared_ptr (new)
    {
        Timer t;
        for (int i = 0; i < N; ++i) {
            std::shared_ptr<TrackedObject> p(new TrackedObject(i));
        }
        std::cout << "  shared_ptr(new): " << std::fixed << std::setprecision(2) 
                  << t.elapsedMs() << " ms\n";
    }
    
    // shared_ptr (make_shared)
    {
        Timer t;
        for (int i = 0; i < N; ++i) {
            auto p = std::make_shared<TrackedObject>(i);
        }
        std::cout << "  make_shared:     " << std::fixed << std::setprecision(2) 
                  << t.elapsedMs() << " ms\n";
    }
    
    // 测试2：拷贝 vs 移动
    std::cout << "\n--- 拷贝 vs 移动 " << N << " 次 ---\n";
    
    auto sp = std::make_shared<TrackedObject>(42);
    
    // 拷贝
    {
        Timer t;
        for (int i = 0; i < N; ++i) {
            auto copy = sp;  // 引用计数原子增
        }
        std::cout << "  拷贝 shared_ptr: " << std::fixed << std::setprecision(2) 
                  << t.elapsedMs() << " ms\n";
    }
    
    // 移动
    {
        Timer t;
        for (int i = 0; i < N; ++i) {
            auto moved = std::move(sp);  // 无原子操作
            sp = std::move(moved);       // 恢复
        }
        std::cout << "  移动 shared_ptr: " << std::fixed << std::setprecision(2) 
                  << t.elapsedMs() << " ms\n";
    }
    
    std::cout << "\n  结论：\n";
    std::cout << "  • make_shared 比 new + shared_ptr 更快\n";
    std::cout << "  • 移动比拷贝更高效（无原子操作）\n";
    std::cout << "  • unique_ptr 是最快的智能指针\n";
}

// ============================================================
// 要点4：原子操作开销
// ============================================================

void demoAtomicOverhead() {
    std::cout << "\n【要点4：原子操作开销】\n\n";
    
    std::cout << "  引用计数操作使用原子指令保证线程安全：\n\n";
    
    std::cout << "  拷贝构造：\n";
    std::cout << "    atomic_fetch_add(&strong_count, 1)  // 原子递增\n\n";
    
    std::cout << "  析构：\n";
    std::cout << "    if (atomic_fetch_sub(&strong_count, 1) == 1) {\n";
    std::cout << "        destroy_object();  // 最后一个所有者\n";
    std::cout << "    }\n\n";
    
    std::cout << "  原子操作的开销：\n";
    std::cout << "  • 通常比普通操作慢 2-10 倍\n";
    std::cout << "  • 涉及内存屏障和缓存同步\n";
    std::cout << "  • 在高度竞争时更明显\n";
}

// ============================================================
// 要点5：函数参数传递优化
// ============================================================

// 不同传递方式
void passByValue(std::shared_ptr<TrackedObject> sp) {
    // 拷贝：引用计数+1（原子操作）
}

void passByConstRef(const std::shared_ptr<TrackedObject>& sp) {
    // 引用：无引用计数变化
}

void passByRvalue(std::shared_ptr<TrackedObject>&& sp) {
    // 移动：无引用计数变化
}

void demoParameterPassing() {
    std::cout << "\n【要点5：函数参数传递优化】\n\n";
    
    auto sp = std::make_shared<TrackedObject>(42);
    const int N = 1000000;
    
    // 传值
    {
        Timer t;
        for (int i = 0; i < N; ++i) {
            passByValue(sp);
        }
        std::cout << "  传值:        " << std::fixed << std::setprecision(2) 
                  << t.elapsedMs() << " ms\n";
    }
    
    // 传引用
    {
        Timer t;
        for (int i = 0; i < N; ++i) {
            passByConstRef(sp);
        }
        std::cout << "  传 const&:   " << std::fixed << std::setprecision(2) 
                  << t.elapsedMs() << " ms\n";
    }
    
    // 传右值
    {
        Timer t;
        for (int i = 0; i < N; ++i) {
            passByRvalue(std::move(sp));
            sp = std::make_shared<TrackedObject>(42);  // 恢复
        }
        std::cout << "  传右值:      " << std::fixed << std::setprecision(2) 
                  << t.elapsedMs() << " ms (含重建)\n";
    }
    
    std::cout << "\n  最佳实践：\n";
    std::cout << "  ┌─────────────────────────────────────────────────────┐\n";
    std::cout << "  │ 场景                    │ 参数类型                 │\n";
    std::cout << "  ├─────────────────────────────────────────────────────┤\n";
    std::cout << "  │ 只读访问，不涉及所有权   │ const shared_ptr<T>&    │\n";
    std::cout << "  │ 需要存储/转移所有权      │ shared_ptr<T> (值传递)  │\n";
    std::cout << "  │ 明确转移所有权           │ shared_ptr<T>&&         │\n";
    std::cout << "  └─────────────────────────────────────────────────────┘\n";
}

// ============================================================
// 要点6：容器中的性能考虑
// ============================================================

void demoContainerPerformance() {
    std::cout << "\n【要点6：容器中的性能考虑】\n\n";
    
    const int N = 100000;
    
    // 填充容器
    std::cout << "--- 填充容器 (" << N << " 元素) ---\n";
    
    std::vector<std::shared_ptr<TrackedObject>> vec1;
    {
        Timer t;
        for (int i = 0; i < N; ++i) {
            vec1.push_back(std::make_shared<TrackedObject>(i));
        }
        std::cout << "  push_back:    " << std::fixed << std::setprecision(2) 
                  << t.elapsedMs() << " ms\n";
    }
    
    std::vector<std::shared_ptr<TrackedObject>> vec2;
    {
        Timer t;
        for (int i = 0; i < N; ++i) {
            vec2.emplace_back(std::make_shared<TrackedObject>(i));
        }
        std::cout << "  emplace_back: " << std::fixed << std::setprecision(2) 
                  << t.elapsedMs() << " ms\n";
    }
    
    // 遍历
    std::cout << "\n--- 遍历容器 ---\n";
    {
        Timer t;
        int sum = 0;
        for (const auto& sp : vec1) {
            sum += sp->value;
        }
        std::cout << "  范围for:      " << std::fixed << std::setprecision(2) 
                  << t.elapsedMs() << " ms\n";
    }
    
    std::cout << "\n  建议：\n";
    std::cout << "  • 容器中存储 shared_ptr 是安全的\n";
    std::cout << "  • 遍历时使用 const auto& 避免拷贝\n";
    std::cout << "  • 考虑使用 unique_ptr + 移动语义（如果可行）\n";
}

// ============================================================
// 演示函数
// ============================================================

void demoItem20() {
    std::cout << "╔══════════════════════════════════════════════════════════╗\n";
    std::cout << "║         EMC++ 条款20：shared_ptr 的性能考虑               ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════╝\n\n";
    
    demoSizeComparison();
    demoControlBlockAllocation();
    demoPerformanceBenchmark();
    demoAtomicOverhead();
    demoParameterPassing();
    demoContainerPerformance();
    
    std::cout << "\n\n条款20 总结：\n";
    std::cout << "┌─────────────────────────────────────────────────────────┐\n";
    std::cout << "│  1. shared_ptr 大小是裸指针的两倍                        │\n";
    std::cout << "│  2. 优先使用 make_shared 减少内存分配                    │\n";
    std::cout << "│  3. 引用计数原子操作有开销                               │\n";
    std::cout << "│  4. 移动比拷贝更高效                                    │\n";
    std::cout << "│  5. 函数参数：只读用 const&，转移用值或&&                │\n";
    std::cout << "│  6. 真正需要共享所有权时才使用 shared_ptr                │\n";
    std::cout << "└─────────────────────────────────────────────────────────┘\n";
}
