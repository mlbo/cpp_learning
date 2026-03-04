/**
 * @file circular_reference.cpp
 * @brief 循环引用问题预览（Day 10 详细讲解）
 * 
 * 问题：
 * 当两个对象通过 shared_ptr 相互引用时，引用计数永不归零，导致内存泄漏。
 * 
 * 解决方案：
 * 使用 weak_ptr 打破循环引用。
 */

#include <iostream>
#include <memory>
#include <vector>

// ============================================================
// 问题演示：循环引用导致内存泄漏
// ============================================================

class NodeWithProblem {
public:
    NodeWithProblem(int v) : value(v) {
        std::cout << "  [构造] Node " << value << "\n";
    }
    
    ~NodeWithProblem() {
        std::cout << "  [析构] Node " << value << "\n";
    }
    
    int value;
    std::shared_ptr<NodeWithProblem> next;   // 前向指针
    std::shared_ptr<NodeWithProblem> prev;   // 后向指针（问题所在！）
};

void demoCircularReferenceProblem() {
    std::cout << "【循环引用问题演示】\n\n";
    std::cout << "--- 创建双向链表节点 ---\n";
    
    {
        auto node1 = std::make_shared<NodeWithProblem>(1);
        auto node2 = std::make_shared<NodeWithProblem>(2);
        
        std::cout << "\n初始引用计数:\n";
        std::cout << "  node1.use_count = " << node1.use_count() << "\n";
        std::cout << "  node2.use_count = " << node2.use_count() << "\n";
        
        // 建立双向连接
        std::cout << "\n建立双向连接...\n";
        node1->next = node2;
        node2->prev = node1;  // 这里创建循环引用！
        
        std::cout << "连接后引用计数:\n";
        std::cout << "  node1.use_count = " << node1.use_count() 
                  << " (node1本身 + node2->prev)\n";
        std::cout << "  node2.use_count = " << node2.use_count() 
                  << " (node2本身 + node1->next)\n";
        
        std::cout << "\n离开作用域...\n";
        std::cout << "（注意：析构函数不会被调用！）\n";
    }
    
    std::cout << "\n  结果：内存泄漏！析构函数从未被调用\n";
    
    std::cout << "\n原因分析:\n";
    std::cout << "  1. node1 离开作用域 → 引用计数从2变为1\n";
    std::cout << "  2. node2 离开作用域 → 引用计数从2变为1\n";
    std::cout << "  3. node1 仍被 node2->prev 引用\n";
    std::cout << "  4. node2 仍被 node1->next 引用\n";
    std::cout << "  5. 两者互相持有，都无法释放\n";
}

// ============================================================
// 解决方案：使用 weak_ptr
// ============================================================

class NodeFixed {
public:
    NodeFixed(int v) : value(v) {
        std::cout << "  [构造] Node " << value << "\n";
    }
    
    ~NodeFixed() {
        std::cout << "  [析构] Node " << value << "\n";
    }
    
    int value;
    std::shared_ptr<NodeFixed> next;      // 强引用：拥有下一个节点
    std::weak_ptr<NodeFixed> prev;        // 弱引用：观察上一个节点（不增加引用计数）
};

void demoCircularReferenceFixed() {
    std::cout << "\n\n【解决方案：使用 weak_ptr】\n\n";
    std::cout << "--- 使用 weak_ptr 打破循环 ---\n";
    
    {
        auto node1 = std::make_shared<NodeFixed>(1);
        auto node2 = std::make_shared<NodeFixed>(2);
        
        std::cout << "\n初始引用计数:\n";
        std::cout << "  node1.use_count = " << node1.use_count() << "\n";
        std::cout << "  node2.use_count = " << node2.use_count() << "\n";
        
        // 建立连接
        std::cout << "\n建立连接...\n";
        node1->next = node2;
        node2->prev = node1;  // weak_ptr 不增加引用计数
        
        std::cout << "连接后引用计数:\n";
        std::cout << "  node1.use_count = " << node1.use_count() 
                  << " (只有 node1 本身)\n";
        std::cout << "  node2.use_count = " << node2.use_count() 
                  << " (node2 本身 + node1->next)\n";
        
        std::cout << "\n离开作用域...\n";
    }
    
    std::cout << "\n  结果：正常释放！析构函数被正确调用\n";
}

// ============================================================
// weak_ptr 的使用方法
// ============================================================

void demoWeakPtrUsage() {
    std::cout << "\n\n【weak_ptr 使用方法】\n\n";
    
    // 创建 weak_ptr
    auto shared = std::make_shared<int>(42);
    std::weak_ptr<int> weak = shared;
    
    std::cout << "--- weak_ptr 特性 ---\n";
    std::cout << "  shared.use_count = " << shared.use_count() << "\n";
    std::cout << "  weak.expired = " << std::boolalpha << weak.expired() << "\n";
    std::cout << "  weak.use_count = " << weak.use_count() << "\n";
    
    // 访问对象：需要 lock() 获取 shared_ptr
    std::cout << "\n--- 访问对象 ---\n";
    if (auto locked = weak.lock()) {
        std::cout << "  成功获取 shared_ptr: *locked = " << *locked << "\n";
        std::cout << "  临时 use_count = " << locked.use_count() << "\n";
    }
    
    // 资源释放后
    std::cout << "\n--- 资源释放后 ---\n";
    shared.reset();
    std::cout << "  shared 已 reset\n";
    std::cout << "  weak.expired = " << weak.expired() << "\n";
    
    if (auto locked = weak.lock()) {
        std::cout << "  （不会执行）*locked = " << *locked << "\n";
    } else {
        std::cout << "  无法获取对象，资源已释放\n";
    }
}

// ============================================================
// 其他循环引用场景
// ============================================================

class Observer;  // 前向声明

class Subject {
public:
    std::vector<std::weak_ptr<Observer>> observers;  // 使用 weak_ptr
    
    void notify() {
        for (auto& weakObs : observers) {
            if (auto obs = weakObs.lock()) {
                // 通知观察者
            }
        }
    }
};

class Observer {
public:
    std::shared_ptr<Subject> subject;  // 强引用
    
    void observe(std::shared_ptr<Subject> s) {
        subject = s;
    }
};

void demoObserverPattern() {
    std::cout << "\n\n【观察者模式中的循环引用】\n\n";
    
    auto subject = std::make_shared<Subject>();
    auto observer = std::make_shared<Observer>();
    
    observer->observe(subject);
    subject->observers.push_back(observer);  // weak_ptr，不增加引用计数
    
    std::cout << "  Subject use_count = " << subject.use_count() << "\n";
    std::cout << "  Observer use_count = " << observer.use_count() << "\n";
    std::cout << "  （使用 weak_ptr 存储观察者，避免循环引用）\n";
}

// ============================================================
// 演示函数
// ============================================================

void demoCircularReference() {
    std::cout << "╔══════════════════════════════════════════════════════════╗\n";
    std::cout << "║           循环引用问题预览                                 ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════╝\n\n";
    
    std::cout << "⚠️  警告：接下来的演示会导致内存泄漏（仅用于教学目的）\n\n";
    
    demoCircularReferenceProblem();
    demoCircularReferenceFixed();
    demoWeakPtrUsage();
    demoObserverPattern();
    
    std::cout << "\n\n要点总结：\n";
    std::cout << "1. 双向关联时，一边用 shared_ptr，另一边用 weak_ptr\n";
    std::cout << "2. weak_ptr 不增加引用计数，不影响对象生命周期\n";
    std::cout << "3. 使用 weak_ptr::lock() 安全访问对象\n";
    std::cout << "4. 观察者模式中，主题用 weak_ptr 持有观察者\n";
    std::cout << "\nDay 10 将深入讲解 weak_ptr 和完整解决方案。\n";
}
