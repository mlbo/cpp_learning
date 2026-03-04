/**
 * @file item19_item20.cpp
 * @brief EMC++ 条款19-20：shared_ptr 资源管理与性能考虑
 * 
 * 条款19：使用 shared_ptr 管理共享所有权的资源
 * 条款20：shared_ptr 的性能考虑
 * 
 * 核心要点：
 * 1. shared_ptr 提供共享所有权语义
 * 2. 引用计数自动管理资源生命周期
 * 3. 避免从同一裸指针创建多个 shared_ptr
 * 4. 性能考虑：大小、控制块、原子操作
 * 5. make_shared 的优势
 */

#include <iostream>
#include <memory>
#include <chrono>
#include <vector>
#include <string>
#include <functional>
#include <iomanip>

// ============================================================
// 演示类：跟踪对象生命周期
// ============================================================

class Widget {
public:
    Widget(const std::string& name = "Widget") : name_(name) {
        std::cout << "  [Widget 构造] " << name_ << "\n";
    }
    
    ~Widget() {
        std::cout << "  [Widget 析构] " << name_ << "\n";
    }
    
    void doSomething() const {
        std::cout << "  [Widget 操作] " << name_ << " 正在工作\n";
    }
    
    const std::string& name() const { return name_; }
    
private:
    std::string name_;
};

// ============================================================
// 条款19 要点1：共享所有权语义
// ============================================================

void processWidget(std::shared_ptr<Widget> w) {
    std::cout << "  处理 Widget: " << w->name() 
              << ", use_count = " << w.use_count() << "\n";
}

void demoSharedOwnership() {
    std::cout << "【条款19 要点1：共享所有权语义】\n\n";
    
    // 创建 Widget
    auto widget = std::make_shared<Widget>("SharedWidget");
    std::cout << "  创建后 use_count = " << widget.use_count() << "\n";
    
    // 多个地方可以安全地共享所有权
    {
        auto w1 = widget;  // 拷贝
        auto w2 = widget;  // 再次拷贝
        std::cout << "  共享后 use_count = " << widget.use_count() << "\n";
        
        processWidget(widget);
    }
    
    std::cout << "  离开作用域后 use_count = " << widget.use_count() << "\n";
    
    std::cout << "\n  共享所有权的优势：\n";
    std::cout << "  • 自动管理：最后一个所有者离开时自动释放\n";
    std::cout << "  • 安全：不会悬空指针\n";
    std::cout << "  • 明确意图：代码清晰表达共享语义\n";
}

// ============================================================
// 条款19 要点2：避免从同一裸指针创建多个 shared_ptr
// ============================================================

void demoDoubleDeletion() {
    std::cout << "\n【条款19 要点2：避免从同一裸指针创建多个 shared_ptr】\n\n";
    
    std::cout << "❌ 错误示例（会导致未定义行为）：\n";
    std::cout << R"(
    Widget* raw = new Widget("Danger");
    std::shared_ptr<Widget> sp1(raw);
    std::shared_ptr<Widget> sp2(raw);  // 灾难！两个控制块
    
    // sp1 和 sp2 有各自独立的控制块
    // 当它们销毁时，raw 被删除两次！
)";

    std::cout << "\n✓ 正确做法：\n";
    std::cout << R"(
    // 方法1: 直接使用 make_shared
    auto sp1 = std::make_shared<Widget>("Safe");
    
    // 方法2: 如果必须从裸指针创建，确保只创建一次
    Widget* raw = new Widget("Safe2");
    std::shared_ptr<Widget> sp1(raw);
    auto sp2 = sp1;  // 正确：共享同一个控制块
)";

    std::cout << "\n  核心规则：一旦将裸指针交给 shared_ptr 管理，\n";
    std::cout << "  就不要再直接使用该裸指针创建另一个 shared_ptr！\n";
}

// ============================================================
// 条款19 要点3：自定义删除器
// ============================================================

struct FileCloser {
    void operator()(FILE* f) const {
        if (f) {
            fclose(f);
            std::cout << "  [文件已关闭]\n";
        }
    }
};

static void demoCustomDeleter() {
    std::cout << "\n【条款19 要点3：自定义删除器】\n\n";
    
    std::cout << "--- 管理文件句柄 ---\n";
    {
        auto fileDeleter = [](FILE* f) {
            if (f) {
                fclose(f);
                std::cout << "  [Lambda 删除器：文件已关闭]\n";
            }
        };
        
        std::shared_ptr<FILE> file(
            fopen("/tmp/emcpp_test.txt", "w"),
            fileDeleter
        );
        
        if (file) {
            fprintf(file.get(), "Hello, shared_ptr!");
            std::cout << "  文件写入成功\n";
        }
    }
    
    std::cout << "\n--- 管理数组 ---\n";
    {
        std::shared_ptr<int[]> arr(new int[10]{1,2,3,4,5,6,7,8,9,10});
        arr[0] = 100;
        std::cout << "  arr[0] = " << arr[0] << "\n";
    }
}

// ============================================================
// 条款19 要点4：enable_shared_from_this
// ============================================================

class ProperWidget : public std::enable_shared_from_this<ProperWidget> {
public:
    ProperWidget(const std::string& name) : name_(name) {
        std::cout << "  [ProperWidget 构造] " << name_ << "\n";
    }
    
    ~ProperWidget() {
        std::cout << "  [ProperWidget 析构] " << name_ << "\n";
    }
    
    std::shared_ptr<ProperWidget> getShared() {
        return shared_from_this();  // 正确！
    }
    
    void process() {
        auto self = shared_from_this();
        std::cout << "  处理 " << name_ 
                  << ", use_count = " << self.use_count() << "\n";
    }
    
private:
    std::string name_;
};

void demoEnableSharedFromThis() {
    std::cout << "\n【条款19 要点4：enable_shared_from_this】\n\n";
    
    auto widget = std::make_shared<ProperWidget>("SelfRef");
    std::cout << "  初始 use_count = " << widget.use_count() << "\n";
    
    auto selfPtr = widget->getShared();
    std::cout << "  获取 selfPtr 后 use_count = " << widget.use_count() << "\n";
    
    widget->process();
    
    std::cout << "\n  ❌ 错误做法：return std::shared_ptr<T>(this);\n";
    std::cout << "  ✓ 正确做法：继承 enable_shared_from_this 并使用 shared_from_this()\n";
}

// ============================================================
// 条款20 要点1：shared_ptr 的大小
// ============================================================

void demoSizeComparison() {
    std::cout << "\n【条款20 要点1：shared_ptr 的大小】\n\n";
    
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
}

// ============================================================
// 条款20 要点2：控制块与 make_shared
// ============================================================

void demoControlBlockAllocation() {
    std::cout << "\n【条款20 要点2：控制块与 make_shared】\n\n";
    
    std::cout << "--- 方法1：new + shared_ptr（两次分配）---\n";
    std::cout << "  分配1: 为对象分配内存\n";
    std::cout << "  分配2: 为控制块分配内存\n\n";
    
    std::cout << "--- 方法2：make_shared（一次分配）---\n";
    std::cout << "  一次分配，同时容纳对象和控制块\n\n";
    
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
// 条款20 要点3：性能基准测试
// ============================================================

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

class TrackedObject {
public:
    TrackedObject(int v) : value(v) {}
    ~TrackedObject() {}
    int value;
};

void demoPerformanceBenchmark() {
    std::cout << "\n【条款20 要点3：性能基准测试】\n\n";
    
    const int N = 100000;
    
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
    
    std::cout << "\n--- 拷贝 vs 移动 " << N << " 次 ---\n";
    
    auto sp = std::make_shared<TrackedObject>(42);
    
    // 拷贝
    {
        Timer t;
        for (int i = 0; i < N; ++i) {
            auto copy = sp;
        }
        std::cout << "  拷贝 shared_ptr: " << std::fixed << std::setprecision(2) 
                  << t.elapsedMs() << " ms\n";
    }
    
    // 移动
    {
        Timer t;
        for (int i = 0; i < N; ++i) {
            auto moved = std::move(sp);
            sp = std::move(moved);
        }
        std::cout << "  移动 shared_ptr: " << std::fixed << std::setprecision(2) 
                  << t.elapsedMs() << " ms\n";
    }
}

// ============================================================
// 条款20 要点4：函数参数传递优化
// ============================================================

void passByValue(std::shared_ptr<TrackedObject> sp) {}
void passByConstRef(const std::shared_ptr<TrackedObject>& sp) {}
void passByRvalue(std::shared_ptr<TrackedObject>&& sp) {}

void demoParameterPassing() {
    std::cout << "\n【条款20 要点4：函数参数传递优化】\n\n";
    
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
// 演示函数
// ============================================================

void demoItem19Item20() {
    std::cout << "╔══════════════════════════════════════════════════════════╗\n";
    std::cout << "║     EMC++ 条款19-20：shared_ptr 资源管理与性能            ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════╝\n\n";
    
    // 条款19
    demoSharedOwnership();
    demoDoubleDeletion();
    demoCustomDeleter();
    demoEnableSharedFromThis();
    
    // 条款20
    demoSizeComparison();
    demoControlBlockAllocation();
    demoPerformanceBenchmark();
    demoParameterPassing();
    
    std::cout << "\n\n条款19-20 总结：\n";
    std::cout << "┌─────────────────────────────────────────────────────────┐\n";
    std::cout << "│  【条款19】                                             │\n";
    std::cout << "│  1. shared_ptr 提供自动的共享所有权管理                  │\n";
    std::cout << "│  2. 避免从同一裸指针创建多个 shared_ptr                  │\n";
    std::cout << "│  3. 自定义删除器支持任意资源类型                         │\n";
    std::cout << "│  4. 需要 this 的 shared_ptr 时，继承                     │\n";
    std::cout << "│     enable_shared_from_this                             │\n";
    std::cout << "├─────────────────────────────────────────────────────────┤\n";
    std::cout << "│  【条款20】                                             │\n";
    std::cout << "│  1. shared_ptr 大小是裸指针的两倍                        │\n";
    std::cout << "│  2. 优先使用 make_shared 减少内存分配                    │\n";
    std::cout << "│  3. 引用计数原子操作有开销                               │\n";
    std::cout << "│  4. 移动比拷贝更高效                                    │\n";
    std::cout << "│  5. 函数参数：只读用 const&，转移用值或&&                │\n";
    std::cout << "└─────────────────────────────────────────────────────────┘\n";
}
