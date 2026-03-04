/**
 * @file raw_vs_smart.cpp
 * @brief 裸指针 vs 智能指针对比演示
 * 
 * 本文件展示：
 * 1. 裸指针的问题与风险
 * 2. 智能指针如何解决这些问题
 * 3. 何时仍需使用裸指针
 */

#include <iostream>
#include <memory>
#include <vector>
#include <cstring>

// ============================================
// 辅助类
// ============================================

class Widget {
public:
    explicit Widget(int id) : id_(id) {
        std::cout << "  Widget(" << id_ << ") 构造\n";
    }
    ~Widget() {
        std::cout << "  Widget(" << id_ << ") 析构\n";
    }
    
    void doWork() {
        std::cout << "  Widget(" << id_ << ") 工作\n";
    }
    
    int id() const { return id_; }
    
private:
    int id_;
};

// ============================================
// 第一部分：裸指针的问题
// ============================================

void demo_raw_pointer_problems() {
    std::cout << "\n=== 1. 裸指针的问题 ===\n";
    
    // 问题1: 忘记释放
    std::cout << "\n--- 1.1 内存泄漏 ---\n";
    std::cout << "  裸指针容易忘记 delete:\n";
    std::cout << "    Widget* p = new Widget(1);\n";
    std::cout << "    // ... 忘记 delete p;\n";
    std::cout << "    // 结果：内存泄漏\n";
    
    // 问题2: 重复释放
    std::cout << "\n--- 1.2 重复释放 ---\n";
    std::cout << "  多个指针指向同一对象:\n";
    std::cout << "    Widget* p1 = new Widget(2);\n";
    std::cout << "    Widget* p2 = p1;\n";
    std::cout << "    delete p1;\n";
    std::cout << "    delete p2;  // 未定义行为！\n";
    
    // 问题3: 悬空指针
    std::cout << "\n--- 1.3 悬空指针 ---\n";
    std::cout << "  使用已释放的内存:\n";
    std::cout << "    Widget* p = new Widget(3);\n";
    std::cout << "    delete p;\n";
    std::cout << "    p->doWork();  // 未定义行为！\n";
    
    // 问题4: 异常安全问题
    std::cout << "\n--- 1.4 异常安全 ---\n";
    std::cout << "  异常发生时资源泄漏:\n";
    std::cout << "    Widget* p = new Widget(4);\n";
    std::cout << "    someFunction();  // 可能抛出异常\n";
    std::cout << "    delete p;  // 如果异常，这行不执行\n";
    
    std::cout << "\n  裸指针总结：需要手动管理，容易出错！\n";
}

// ============================================
// 第二部分：智能指针的解决方案
// ============================================

void demo_smart_pointer_solutions() {
    std::cout << "\n=== 2. 智能指针的解决方案 ===\n";
    
    // 解决1: 自动释放，防止泄漏
    std::cout << "\n--- 2.1 自动释放 ---\n";
    {
        auto p = std::make_unique<Widget>(10);
        p->doWork();
        // 自动释放，无需 delete
    }
    std::cout << "  unique_ptr 自动管理生命周期\n";
    
    // 解决2: 明确所有权，防止重复释放
    std::cout << "\n--- 2.2 明确所有权 ---\n";
    {
        auto p1 = std::make_unique<Widget>(11);
        // auto p2 = p1;  // 编译错误！禁止拷贝
        auto p2 = std::move(p1);  // 必须显式移动
        std::cout << "  所有权转移后 p1 为空\n";
        std::cout << "  p1 是否为空: " << (p1 ? "否" : "是") << "\n";
    }
    
    // 解决3: 共享所有权
    std::cout << "\n--- 2.3 共享所有权 ---\n";
    {
        auto p1 = std::make_shared<Widget>(12);
        auto p2 = p1;  // 共享所有权
        std::cout << "  引用计数: " << p1.use_count() << "\n";
        // 最后一个 shared_ptr 离开作用域时自动释放
    }
    
    // 解决4: 异常安全
    std::cout << "\n--- 2.4 异常安全 ---\n";
    {
        try {
            auto p = std::make_unique<Widget>(13);
            throw std::runtime_error("模拟异常");
            // 即使异常，p 也会正确析构
        } catch (const std::exception& e) {
            std::cout << "  捕获异常: " << e.what() << "\n";
            std::cout << "  资源已自动释放\n";
        }
    }
    
    std::cout << "\n  智能指针总结：自动管理，安全可靠！\n";
}

// ============================================
// 第三部分：代码对比
// ============================================

// 使用裸指针的危险版本
void dangerous_function_raw() {
    std::cout << "\n--- 危险的裸指针代码 ---\n";
    Widget* widgets[3];
    
    // 分配
    for (int i = 0; i < 3; ++i) {
        widgets[i] = new Widget(20 + i);
    }
    
    // 使用
    for (int i = 0; i < 3; ++i) {
        widgets[i]->doWork();
    }
    
    // 必须记得释放
    for (int i = 0; i < 3; ++i) {
        delete widgets[i];  // 如果中间抛出异常，后面的就泄漏了
    }
}

// 使用智能指针的安全版本
void safe_function_smart() {
    std::cout << "\n--- 安全的智能指针代码 ---\n";
    std::vector<std::unique_ptr<Widget>> widgets;
    
    // 分配
    for (int i = 0; i < 3; ++i) {
        widgets.push_back(std::make_unique<Widget>(30 + i));
    }
    
    // 使用
    for (const auto& w : widgets) {
        w->doWork();
    }
    
    // 无需手动释放，自动管理
}

// ============================================
// 第四部分：何时使用裸指针
// ============================================

void demo_when_to_use_raw() {
    std::cout << "\n=== 3. 何时仍可使用裸指针 ===\n";
    
    // 场景1: 非拥有指针（观察者）
    std::cout << "\n--- 3.1 非拥有观察指针 ---\n";
    {
        auto widget = std::make_unique<Widget>(40);
        Widget* observer = widget.get();  // 裸指针仅作为观察者
        
        std::cout << "  通过观察指针访问: ";
        observer->doWork();
        
        // observer 不拥有资源，不负责释放
    }
    
    // 场景2: 函数参数传递
    std::cout << "\n--- 3.2 函数参数 ---\n";
    auto printWidget = [](Widget* w) {  // 裸指针参数
        std::cout << "  Widget id: " << w->id() << "\n";
    };
    
    {
        auto w = std::make_unique<Widget>(41);
        printWidget(w.get());
    }
    
    // 场景3: 性能关键且所有权明确
    std::cout << "\n--- 3.3 性能关键代码 ---\n";
    std::cout << "  在极少数性能敏感场景，裸指针可以减少开销\n";
    std::cout << "  但必须确保所有权语义清晰\n";
    
    // 场景4: C API 兼容
    std::cout << "\n--- 3.4 C API 兼容 ---\n";
    {
        auto buffer = std::make_unique<char[]>(100);
        // 传递给 C 函数
        std::strcpy(buffer.get(), "Hello, C API!");
        std::cout << "  传递给 C 函数: " << buffer.get() << "\n";
    }
    
    std::cout << "\n  裸指针正确使用原则：\n";
    std::cout << "  1. 只作为非拥有观察者\n";
    std::cout << "  2. 生命周期由智能指针管理\n";
    std::cout << "  3. 不负责内存释放\n";
}

// ============================================
// 第五部分：最佳实践总结
// ============================================

void print_best_practices() {
    std::cout << "\n=== 4. 最佳实践总结 ===\n";
    
    std::cout << R"(
  ┌─────────────────────────────────────────────────────────────┐
  │                    指针使用最佳实践                          │
  ├─────────────────────────────────────────────────────────────┤
  │                                                             │
  │  ✅ 推荐做法：                                              │
  │     • 优先使用栈对象（值语义）                              │
  │     • 需要所有权时用 unique_ptr                            │
  │     • 需要共享时用 shared_ptr                              │
  │     • 打破循环引用用 weak_ptr                              │
  │     • 使用 make_unique/make_shared 创建                    │
  │                                                             │
  │  ❌ 避免做法：                                              │
  │     • 显式 new/delete（用智能指针替代）                     │
  │     • 裸指针持有所有权                                      │
  │     • 共享裸指针（容易重复释放）                            │
  │     • 返回裸指针表示所有权                                  │
  │                                                             │
  │  ⚠️  裸指针正确用法：                                       │
  │     • 作为非拥有观察者                                      │
  │     • 函数参数传递                                          │
  │     • 与 C API 交互                                         │
  │                                                             │
  └─────────────────────────────────────────────────────────────┘
)";
}

// ============================================
// 主函数
// ============================================

void run_raw_vs_smart() {
    std::cout << "\n========================================\n";
    std::cout << "  裸指针 vs 智能指针对比\n";
    std::cout << "========================================\n";
    
    demo_raw_pointer_problems();
    demo_smart_pointer_solutions();
    
    std::cout << "\n--- 代码对比演示 ---\n";
    dangerous_function_raw();
    safe_function_smart();
    
    demo_when_to_use_raw();
    print_best_practices();
    
    std::cout << "\n========================================\n";
    std::cout << "  对比演示完毕\n";
    std::cout << "========================================\n";
}
