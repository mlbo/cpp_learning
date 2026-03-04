/**
 * @file weak_ptr_demo.cpp
 * @brief weak_ptr 基本用法演示
 * 
 * 涵盖内容：
 * 1. weak_ptr 的创建和初始化
 * 2. lock() 方法的使用
 * 3. expired() 和 use_count()
 * 4. 循环引用的解决
 */

#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Resource {
public:
    Resource(const std::string& name) : name_(name) {
        std::cout << "  📦 Resource '" << name_ << "' 已创建\n";
    }
    ~Resource() {
        std::cout << "  🗑️  Resource '" << name_ << "' 已销毁\n";
    }
    
    void use() const {
        std::cout << "  ✨ 使用 Resource '" << name_ << "'\n";
    }
    
    const std::string& name() const { return name_; }

private:
    std::string name_;
};

// ============================================
// 1. weak_ptr 创建和初始化
// ============================================
void demo_creation() {
    std::cout << "\n【1. weak_ptr 创建和初始化】\n";
    
    // 从 shared_ptr 创建
    std::shared_ptr<Resource> sp = std::make_shared<Resource>("Resource_A");
    std::weak_ptr<Resource> wp1 = sp;
    
    std::cout << "  shared_ptr 引用计数: " << sp.use_count() << "\n";
    std::cout << "  weak_ptr use_count(): " << wp1.use_count() << "\n";
    
    // 空 weak_ptr
    std::weak_ptr<Resource> wp2;
    std::cout << "  空 weak_ptr expired(): " << (wp2.expired() ? "true" : "false") << "\n";
}

// ============================================
// 2. lock() 方法详解
// ============================================
void demo_lock() {
    std::cout << "\n【2. lock() 方法详解】\n";
    
    std::weak_ptr<Resource> wp;
    
    {
        auto sp = std::make_shared<Resource>("Resource_B");
        wp = sp;
        
        std::cout << "  尝试通过 lock() 访问...\n";
        if (auto locked = wp.lock()) {
            locked->use();
            std::cout << "  lock() 成功，引用计数: " << locked.use_count() << "\n";
        }
    }
    
    // Resource_B 已销毁
    std::cout << "  离开作用域后...\n";
    std::cout << "  expired(): " << (wp.expired() ? "true" : "false") << "\n";
    
    if (auto locked = wp.lock()) {
        locked->use();
    } else {
        std::cout << "  ⚠️ lock() 返回空，对象已销毁\n";
    }
}

// ============================================
// 3. 循环引用问题与解决
// ============================================
namespace bad_design {
    struct Node {
        int value;
        std::shared_ptr<Node> next;
        std::shared_ptr<Node> prev;  // 问题：双向都用 shared_ptr
        
        Node(int v) : value(v) {
            std::cout << "    Node(" << value << ") 创建\n";
        }
        ~Node() {
            std::cout << "    Node(" << value << ") 销毁\n";
        }
    };
}

namespace good_design {
    struct Node {
        int value;
        std::shared_ptr<Node> next;
        std::weak_ptr<Node> prev;  // 解决：使用 weak_ptr
        
        Node(int v) : value(v) {
            std::cout << "    Node(" << value << ") 创建\n";
        }
        ~Node() {
            std::cout << "    Node(" << value << ") 销毁\n";
        }
    };
}

void demo_cycle() {
    std::cout << "\n【3. 循环引用问题与解决】\n";
    
    std::cout << "\n  有问题的版本（循环引用）:\n";
    {
        auto node1 = std::make_shared<bad_design::Node>(1);
        auto node2 = std::make_shared<bad_design::Node>(2);
        node1->next = node2;
        node2->prev = node1;  // 循环！
        std::cout << "  node1 引用计数: " << node1.use_count() << "\n";
        std::cout << "  node2 引用计数: " << node2.use_count() << "\n";
        std::cout << "  ⚠️ 需要手动打破循环才能正确释放\n";
        node2->prev.reset();  // 手动打破
    }
    
    std::cout << "\n  正确版本（使用 weak_ptr）:\n";
    {
        auto node1 = std::make_shared<good_design::Node>(1);
        auto node2 = std::make_shared<good_design::Node>(2);
        node1->next = node2;
        node2->prev = node1;  // 不增加引用计数
        std::cout << "  node1 引用计数: " << node1.use_count() << "\n";
        std::cout << "  node2 引用计数: " << node2.use_count() << "\n";
        std::cout << "  ✅ 离开作用域时自动正确销毁\n";
    }
}

// ============================================
// 4. 观察者模式应用
// ============================================
class Observer {
public:
    virtual ~Observer() = default;
    virtual void onNotify(const std::string& message) = 0;
};

class Subject {
public:
    void attach(const std::shared_ptr<Observer>& obs) {
        observers_.push_back(obs);
    }
    
    void notify(const std::string& message) {
        for (auto& wp : observers_) {
            if (auto sp = wp.lock()) {
                sp->onNotify(message);
            }
        }
    }

private:
    std::vector<std::weak_ptr<Observer>> observers_;
};

class ConcreteObserver : public Observer {
public:
    void onNotify(const std::string& message) override {
        std::cout << "    收到消息: " << message << "\n";
    }
};

void demo_observer() {
    std::cout << "\n【4. 观察者模式应用】\n";
    
    Subject subject;
    auto obs1 = std::make_shared<ConcreteObserver>();
    subject.attach(obs1);
    
    {
        auto obs2 = std::make_shared<ConcreteObserver>();
        subject.attach(obs2);
        std::cout << "  发送消息（两个观察者）:\n";
        subject.notify("Hello");
    }
    
    std::cout << "  发送消息（一个观察者已销毁）:\n";
    subject.notify("World");
}

// ============================================
// 主演示函数
// ============================================
void demo_weak_ptr_basic() {
    demo_creation();
    demo_lock();
    demo_cycle();
    demo_observer();
    
    std::cout << "\n💡 weak_ptr 核心要点:\n";
    std::cout << "   1. 不增加引用计数，不控制生命周期\n";
    std::cout << "   2. 通过 lock() 安全获取 shared_ptr\n";
    std::cout << "   3. expired() 检查对象是否已销毁\n";
    std::cout << "   4. 适用于缓存、观察者、打破循环引用\n";
}
