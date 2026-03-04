/**
 * @file item19_shared_ptr.cpp
 * @brief EMC++ 条款19：使用 shared_ptr 管理共享所有权的资源
 * 
 * 核心要点：
 * 1. shared_ptr 提供共享所有权语义
 * 2. 引用计数自动管理资源生命周期
 * 3. 自定义删除器支持多种资源类型
 * 4. 避免从同一裸指针创建多个 shared_ptr
 */

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <functional>

// ============================================================
// 演示类：跟踪对象生命周期
// ============================================================

class Widget {
public:
    Widget(const std::string& name) : name_(name) {
        std::cout << "  [Widget 构造] " << name_ << "\n";
    }
    
    ~Widget() {
        std::cout << "  [Widget 析构] " << name_ << "\n";
    }
    
    void doSomething() {
        std::cout << "  [Widget 操作] " << name_ << " 正在工作\n";
    }
    
    const std::string& name() const { return name_; }
    
private:
    std::string name_;
};

// ============================================================
// 要点1：共享所有权语义
// ============================================================

/**
 * shared_ptr 的核心价值：明确表达共享所有权意图
 * 
 * 当多个实体需要共同管理某个资源的生命周期时，
 * shared_ptr 提供了自动、安全的资源管理机制。
 */

void processWidget(std::shared_ptr<Widget> w) {
    std::cout << "  处理 Widget: " << w->name() 
              << ", use_count = " << w.use_count() << "\n";
    // 函数可能存储这个 shared_ptr，资源不会在函数内被释放
}

void demoSharedOwnership() {
    std::cout << "【要点1：共享所有权语义】\n\n";
    
    // 创建 Widget
    auto widget = std::make_shared<Widget>("SharedWidget");
    std::cout << "  创建后 use_count = " << widget.use_count() << "\n";
    
    // 多个地方可以安全地共享所有权
    {
        auto w1 = widget;  // 拷贝
        auto w2 = widget;  // 再次拷贝
        std::cout << "  共享后 use_count = " << widget.use_count() << "\n";
        
        // 即使传递给函数，所有权也是安全的
        processWidget(widget);
    }
    
    std::cout << "  离开作用域后 use_count = " << widget.use_count() << "\n";
    
    std::cout << "\n  共享所有权的优势：\n";
    std::cout << "  • 自动管理：最后一个所有者离开时自动释放\n";
    std::cout << "  • 安全：不会悬空指针\n";
    std::cout << "  • 明确意图：代码清晰表达共享语义\n";
}

// ============================================================
// 要点2：避免从同一裸指针创建多个 shared_ptr
// ============================================================

void demoDoubleDeletion() {
    std::cout << "\n【要点2：避免从同一裸指针创建多个 shared_ptr】\n\n";
    
    std::cout << "❌ 错误示例（注释掉，因为会导致未定义行为）：\n";
    std::cout << R"(
    Widget* raw = new Widget("Danger");
    std::shared_ptr<Widget> sp1(raw);
    std::shared_ptr<Widget> sp2(raw);  // 灾难！
    
    // sp1 和 sp2 有各自独立的控制块
    // 当它们销毁时，raw 被删除两次！
    // 这是未定义行为，通常会崩溃
)";

    std::cout << "\n✓ 正确做法：\n";
    std::cout << R"(
    // 方法1: 直接使用 make_shared
    auto sp1 = std::make_shared<Widget>("Safe");
    
    // 方法2: 如果必须从裸指针创建，确保只创建一次
    Widget* raw = new Widget("Safe2");
    std::shared_ptr<Widget> sp1(raw);
    // 之后使用 sp1 进行拷贝，而不是再次从 raw 创建
    auto sp2 = sp1;  // 正确：共享同一个控制块
)";

    std::cout << "\n  核心规则：一旦将裸指针交给 shared_ptr 管理，\n";
    std::cout << "  就不要再直接使用该裸指针创建另一个 shared_ptr！\n";
}

// ============================================================
// 要点3：自定义删除器
// ============================================================

// 自定义资源类型
struct FileCloser {
    void operator()(FILE* f) const {
        if (f) {
            fclose(f);
            std::cout << "  [文件已关闭]\n";
        }
    }
};

void demoCustomDeleter() {
    std::cout << "\n【要点3：自定义删除器】\n\n";
    
    std::cout << "--- 管理文件句柄 ---\n";
    {
        // 使用自定义删除器管理 FILE*
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
    std::cout << "  离开作用域，文件自动关闭\n";
    
    std::cout << "\n--- 使用函数对象删除器 ---\n";
    {
        std::shared_ptr<FILE> file(
            fopen("/tmp/emcpp_test2.txt", "w"),
            FileCloser()
        );
        std::cout << "  使用 FileCloser 管理文件\n";
    }
    
    std::cout << "\n--- 管理数组 ---\n";
    {
        // C++17 支持 shared_ptr<T[]>
        std::shared_ptr<int[]> arr(new int[10]{1,2,3,4,5,6,7,8,9,10});
        arr[0] = 100;  // 直接使用 [] 访问
        std::cout << "  arr[0] = " << arr[0] << "\n";
        // 默认使用 delete[]
    }
}

// ============================================================
// 要点4：enable_shared_from_this
// ============================================================

/**
 * 当对象方法需要返回指向自身的 shared_ptr 时，
 * 必须继承 std::enable_shared_from_this
 * 
 * 错误做法：
 *   std::shared_ptr<this>  // 会创建新的控制块！
 * 
 * 正确做法：
 *   shared_from_this()     // 返回共享控制块的 shared_ptr
 */

class ProperWidget : public std::enable_shared_from_this<ProperWidget> {
public:
    ProperWidget(const std::string& name) : name_(name) {
        std::cout << "  [ProperWidget 构造] " << name_ << "\n";
    }
    
    ~ProperWidget() {
        std::cout << "  [ProperWidget 析构] " << name_ << "\n";
    }
    
    // 返回指向自身的 shared_ptr
    std::shared_ptr<ProperWidget> getShared() {
        return shared_from_this();  // 正确！
    }
    
    void process() {
        // 在方法内部需要 shared_ptr 指向自己
        auto self = shared_from_this();
        std::cout << "  处理 " << name_ 
                  << ", use_count = " << self.use_count() << "\n";
    }
    
private:
    std::string name_;
};

void demoEnableSharedFromThis() {
    std::cout << "\n【要点4：enable_shared_from_this】\n\n";
    
    // 必须先通过 shared_ptr 创建对象
    auto widget = std::make_shared<ProperWidget>("SelfRef");
    
    std::cout << "  初始 use_count = " << widget.use_count() << "\n";
    
    // 获取指向自身的 shared_ptr
    auto selfPtr = widget->getShared();
    std::cout << "  获取 selfPtr 后 use_count = " << widget.use_count() << "\n";
    
    // 在方法内使用
    widget->process();
    
    std::cout << "\n  ❌ 错误做法（会导致双重删除）：\n";
    std::cout << R"(
    class BadWidget {
        std::shared_ptr<BadWidget> getShared() {
            // 错误！会创建新的控制块
            return std::shared_ptr<BadWidget>(this);
        }
    };
)";
    
    std::cout << "\n  ✓ 正确做法：继承 enable_shared_from_this\n";
    std::cout << "  并使用 shared_from_this() 获取正确的 shared_ptr\n";
}

// ============================================================
// 要点5：避免不必要的共享
// ============================================================

void demoAvoidUnnecessarySharing() {
    std::cout << "\n【要点5：避免不必要的共享】\n\n";
    
    std::cout << "  原则：优先使用 unique_ptr，只在需要共享时使用 shared_ptr\n\n";
    
    std::cout << "--- 场景分析 ---\n\n";
    
    std::cout << "1. 独占所有权场景 → 使用 unique_ptr\n";
    std::cout << "   • 工厂函数返回对象\n";
    std::cout << "   • 容器中的元素\n";
    std::cout << "   • 类成员变量\n\n";
    
    std::cout << "2. 共享所有权场景 → 使用 shared_ptr\n";
    std::cout << "   • 多个对象共同持有某资源\n";
    std::cout << "   • 异步操作中传递对象所有权\n";
    std::cout << "   • 观察者模式中的主题\n\n";
    
    std::cout << "  性能对比：\n";
    std::cout << "  • unique_ptr: 零开销，和裸指针一样\n";
    std::cout << "  • shared_ptr: 控制块 + 原子操作开销\n";
}

// ============================================================
// 演示函数
// ============================================================

void demoItem19() {
    std::cout << "╔══════════════════════════════════════════════════════════╗\n";
    std::cout << "║  EMC++ 条款19：使用 shared_ptr 管理共享所有权的资源       ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════╝\n\n";
    
    demoSharedOwnership();
    demoDoubleDeletion();
    demoCustomDeleter();
    demoEnableSharedFromThis();
    demoAvoidUnnecessarySharing();
    
    std::cout << "\n\n条款19 总结：\n";
    std::cout << "┌─────────────────────────────────────────────────────────┐\n";
    std::cout << "│  1. shared_ptr 提供自动的共享所有权管理                   │\n";
    std::cout << "│  2. 避免从同一裸指针创建多个 shared_ptr                   │\n";
    std::cout << "│  3. 自定义删除器支持任意资源类型                          │\n";
    std::cout << "│  4. 需要返回 this 的 shared_ptr 时，继承                  │\n";
    std::cout << "│     enable_shared_from_this                              │\n";
    std::cout << "│  5. 优先使用 unique_ptr，只在需要共享时用 shared_ptr      │\n";
    std::cout << "└─────────────────────────────────────────────────────────┘\n";
}
