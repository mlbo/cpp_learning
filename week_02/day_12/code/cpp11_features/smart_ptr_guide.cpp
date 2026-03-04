/**
 * @file smart_ptr_guide.cpp
 * @brief 智能指针选择指南 - 全面对比与选择策略
 * 
 * 本文件展示：
 * 1. 三种智能指针的特性对比
 * 2. 不同场景下的最佳选择
 * 3. 性能分析与使用建议
 */

#include <iostream>
#include <memory>
#include <vector>
#include <chrono>
#include <functional>

// ============================================
// 辅助类定义
// ============================================

class Resource {
public:
    explicit Resource(int id) : id_(id) {
        std::cout << "  [Resource] 构造 id=" << id_ << "\n";
    }
    ~Resource() {
        std::cout << "  [Resource] 析构 id=" << id_ << "\n";
    }
    void use() const {
        std::cout << "  [Resource] 使用 id=" << id_ << "\n";
    }
    int id() const { return id_; }
private:
    int id_;
};

// ============================================
// 第一部分：unique_ptr - 独占所有权
// ============================================

void demo_unique_ptr() {
    std::cout << "\n=== 1. unique_ptr 独占所有权 ===\n";
    
    // 1. 基本使用
    std::cout << "\n--- 1.1 基本创建与使用 ---\n";
    {
        std::unique_ptr<Resource> ptr = std::make_unique<Resource>(1);
        ptr->use();
        // 离开作用域自动释放
    }
    std::cout << "  已离开作用域\n";
    
    // 2. 所有权转移
    std::cout << "\n--- 1.2 所有权转移 ---\n";
    {
        std::unique_ptr<Resource> ptr1 = std::make_unique<Resource>(2);
        std::cout << "  ptr1 是否拥有资源: " << (ptr1 ? "是" : "否") << "\n";
        
        // 移动语义转移所有权
        std::unique_ptr<Resource> ptr2 = std::move(ptr1);
        std::cout << "  移动后 ptr1 是否拥有资源: " << (ptr1 ? "是" : "否") << "\n";
        std::cout << "  移动后 ptr2 是否拥有资源: " << (ptr2 ? "是" : "否") << "\n";
        
        // 在函数间转移
        auto transfer = [](std::unique_ptr<Resource> p) {
            p->use();
            return p;
        };
        ptr2 = transfer(std::move(ptr2));
        std::cout << "  函数返回后所有权回归\n";
    }
    
    // 3. 数组支持
    std::cout << "\n--- 1.3 数组支持 ---\n";
    {
        // 使用 unique_ptr<T[]> 管理数组
        std::unique_ptr<int[]> arr = std::make_unique<int[]>(5);
        for (int i = 0; i < 5; ++i) {
            arr[i] = i * 10;
        }
        std::cout << "  数组内容: ";
        for (int i = 0; i < 5; ++i) {
            std::cout << arr[i] << " ";
        }
        std::cout << "\n";
    }
    
    // 4. 适用场景
    std::cout << "\n--- 1.4 适用场景 ---\n";
    std::cout << "  ✓ 工厂函数返回对象\n";
    std::cout << "  ✓ Pimpl 模式\n";
    std::cout << "  ✓ 独占资源管理\n";
    std::cout << "  ✓ 高性能场景（零开销）\n";
}

// ============================================
// 第二部分：shared_ptr - 共享所有权
// ============================================

void demo_shared_ptr() {
    std::cout << "\n=== 2. shared_ptr 共享所有权 ===\n";
    
    // 1. 基本使用与引用计数
    std::cout << "\n--- 2.1 基本使用与引用计数 ---\n";
    {
        std::shared_ptr<Resource> ptr1 = std::make_shared<Resource>(10);
        std::cout << "  ptr1 引用计数: " << ptr1.use_count() << "\n";
        
        {
            std::shared_ptr<Resource> ptr2 = ptr1;  // 共享所有权
            std::cout << "  创建 ptr2 后引用计数: " << ptr1.use_count() << "\n";
            
            std::shared_ptr<Resource> ptr3 = ptr1;  // 再次共享
            std::cout << "  创建 ptr3 后引用计数: " << ptr1.use_count() << "\n";
            
            ptr2->use();
        }  // ptr2, ptr3 离开作用域
        
        std::cout << "  离开内层作用域后引用计数: " << ptr1.use_count() << "\n";
    }
    std::cout << "  ptr1 离开作用域，引用计数归零，资源释放\n";
    
    // 2. 在容器中使用
    std::cout << "\n--- 2.2 在容器中使用 ---\n";
    {
        std::vector<std::shared_ptr<Resource>> resources;
        resources.push_back(std::make_shared<Resource>(20));
        resources.push_back(std::make_shared<Resource>(21));
        resources.push_back(std::make_shared<Resource>(22));
        
        // 取出某个共享
        std::shared_ptr<Resource> shared = resources[0];
        std::cout << "  共享 resources[0]，引用计数: " << shared.use_count() << "\n";
        
        resources.clear();  // 清空容器
        std::cout << "  容器清空后，shared 引用计数: " << shared.use_count() << "\n";
    }
    
    // 3. this 指针共享
    std::cout << "\n--- 2.3 enable_shared_from_this ---\n";
    std::cout << "  在类内部获取自身的 shared_ptr\n";
    
    // 4. 适用场景
    std::cout << "\n--- 2.4 适用场景 ---\n";
    std::cout << "  ✓ 共享资源管理\n";
    std::cout << "  ✓ 缓存系统\n";
    std::cout << "  ✓ 图/树结构\n";
    std::cout << "  ✓ 需要延长对象生命周期\n";
}

// ============================================
// 第三部分：weak_ptr - 非拥有观察者
// ============================================

class Node : public std::enable_shared_from_this<Node> {
public:
    explicit Node(int value) : value_(value) {
        std::cout << "  [Node] 创建 value=" << value_ << "\n";
    }
    ~Node() {
        std::cout << "  [Node] 销毁 value=" << value_ << "\n";
    }
    
    void setNext(std::shared_ptr<Node> next) {
        next_ = next;
    }
    
    void setPrev(std::shared_ptr<Node> prev) {
        // 使用 weak_ptr 防止循环引用
        prev_ = prev;
    }
    
    void setPrevWeak(std::weak_ptr<Node> prev) {
        prev_ = prev;
    }
    
    std::shared_ptr<Node> getNext() const { return next_; }
    std::shared_ptr<Node> getPrev() const { 
        return prev_.lock();  // 尝试获取 shared_ptr
    }
    
    int value() const { return value_; }
    
private:
    int value_;
    std::shared_ptr<Node> next_;    // 强引用
    std::weak_ptr<Node> prev_;      // 弱引用，防止循环引用
};

void demo_weak_ptr() {
    std::cout << "\n=== 3. weak_ptr 非拥有观察者 ===\n";
    
    // 1. 打破循环引用
    std::cout << "\n--- 3.1 打破循环引用 ---\n";
    {
        auto node1 = std::make_shared<Node>(100);
        auto node2 = std::make_shared<Node>(101);
        
        node1->setNext(node2);
        node2->setPrevWeak(node1);  // 使用 weak_ptr
        
        std::cout << "  node1 引用计数: " << node1.use_count() << "\n";
        std::cout << "  node2 引用计数: " << node2.use_count() << "\n";
        std::cout << "  离开作用域...\n";
    }
    std::cout << "  两个节点都被正确释放\n";
    
    // 2. 观察者模式
    std::cout << "\n--- 3.2 观察者模式 ---\n";
    {
        std::weak_ptr<Resource> weak;
        {
            auto strong = std::make_shared<Resource>(30);
            weak = strong;
            
            std::cout << "  强引用存在，weak.lock() 成功\n";
            if (auto locked = weak.lock()) {
                locked->use();
            }
        }
        
        std::cout << "  强引用已销毁，weak.lock() 失败\n";
        if (weak.expired()) {
            std::cout << "  资源已过期\n";
        }
    }
    
    // 3. 适用场景
    std::cout << "\n--- 3.3 适用场景 ---\n";
    std::cout << "  ✓ 打破循环引用\n";
    std::cout << "  ✓ 观察者模式\n";
    std::cout << "  ✓ 缓存临时引用\n";
    std::cout << "  ✓ 条件性访问共享资源\n";
}

// ============================================
// 第四部分：性能对比
// ============================================

void benchmark_smart_pointers() {
    std::cout << "\n=== 4. 性能对比 ===\n";
    
    const int N = 1000000;
    
    // unique_ptr 性能
    auto start = std::chrono::high_resolution_clock::now();
    {
        std::vector<std::unique_ptr<int>> vec;
        vec.reserve(N);
        for (int i = 0; i < N; ++i) {
            vec.push_back(std::make_unique<int>(i));
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "  unique_ptr: " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() 
              << " ms\n";
    
    // shared_ptr 性能
    start = std::chrono::high_resolution_clock::now();
    {
        std::vector<std::shared_ptr<int>> vec;
        vec.reserve(N);
        for (int i = 0; i < N; ++i) {
            vec.push_back(std::make_shared<int>(i));
        }
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "  shared_ptr: " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() 
              << " ms\n";
    
    std::cout << "\n  结论: unique_ptr 零开销，shared_ptr 有原子操作开销\n";
}

// ============================================
// 第五部分：选择决策指南
// ============================================

void print_decision_guide() {
    std::cout << "\n=== 5. 智能指针选择决策指南 ===\n";
    
    std::cout << R"(
  ┌─────────────────────────────────────────────────────────────┐
  │                   智能指针选择决策树                         │
  ├─────────────────────────────────────────────────────────────┤
  │                                                             │
  │   需要管理动态资源？                                        │
  │        │                                                    │
  │        ├── 否 ──→ 使用栈对象/值语义                         │
  │        │                                                    │
  │        └── 是 ──→ 需要共享所有权？                          │
  │                      │                                      │
  │                      ├── 否 ──→ std::unique_ptr            │
  │                      │              │                       │
  │                      │              ├── 需要自定义删除器？  │
  │                      │              │       │               │
  │                      │              │       └── unique_ptr<T, Deleter>
  │                      │              │                       │
  │                      │              └── unique_ptr<T>       │
  │                      │                                      │
  │                      └── 是 ──→ 需要观察但不拥有？          │
  │                                    │                        │
  │                                    ├── 是 ──→ std::weak_ptr │
  │                                    │                        │
  │                                    └── 否 ──→ std::shared_ptr
  │                                                             │
  └─────────────────────────────────────────────────────────────┘

  推荐优先级：
  1. 栈对象 > unique_ptr > shared_ptr > weak_ptr
  2. 能用栈对象就不用指针
  3. 能用 unique_ptr 就不用 shared_ptr
  4. 需要 shared_ptr 时才考虑 weak_ptr
)";
}

// ============================================
// 主函数
// ============================================

void run_smart_ptr_guide() {
    std::cout << "========================================\n";
    std::cout << "  智能指针选择指南\n";
    std::cout << "========================================\n";
    
    demo_unique_ptr();
    demo_shared_ptr();
    demo_weak_ptr();
    benchmark_smart_pointers();
    print_decision_guide();
    
    std::cout << "\n========================================\n";
    std::cout << "  智能指针指南演示完毕\n";
    std::cout << "========================================\n";
}
