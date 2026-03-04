/**
 * @file break_cycle.cpp
 * @brief 打破循环引用演示
 * 
 * 涵盖内容：
 * 1. 循环引用问题演示
 * 2. 使用 weak_ptr 解决循环引用
 * 3. 常见循环引用场景分析
 */

#include <iostream>
#include <memory>
#include <vector>

// ============================================
// 1. 循环引用问题演示（故意设计有问题）
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

void demo_memory_leak() {
    std::cout << "\n【1. 循环引用导致内存泄漏】\n";
    std::cout << "  创建双向链表（有问题版本）:\n";
    
    auto node1 = std::make_shared<Node>(1);
    auto node2 = std::make_shared<Node>(2);
    
    // 形成循环引用
    node1->next = node2;
    node2->prev = node1;  // 循环！
    
    std::cout << "  node1 引用计数: " << node1.use_count() << "\n";
    std::cout << "  node2 引用计数: " << node2.use_count() << "\n";
    std::cout << "  ⚠️ 离开作用域时，节点不会被销毁（内存泄漏）\n";
    
    // 手动打破循环才能正确释放
    node2->prev.reset();
    std::cout << "  手动打破循环后...\n";
}

} // namespace bad_design

// ============================================
// 2. 使用 weak_ptr 解决循环引用
// ============================================
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

void demo_no_leak() {
    std::cout << "\n【2. weak_ptr 解决循环引用】\n";
    std::cout << "  创建双向链表（正确版本）:\n";
    
    auto node1 = std::make_shared<Node>(1);
    auto node2 = std::make_shared<Node>(2);
    
    node1->next = node2;
    node2->prev = node1;  // 不增加引用计数
    
    std::cout << "  node1 引用计数: " << node1.use_count() << "\n";
    std::cout << "  node2 引用计数: " << node2.use_count() << "\n";
    std::cout << "  ✅ 离开作用域时，节点会正确销毁\n";
}

} // namespace good_design

// ============================================
// 3. 父子关系中的循环引用
// ============================================
namespace parent_child {

class Child;  // 前向声明

class Parent : public std::enable_shared_from_this<Parent> {
public:
    std::string name;
    std::vector<std::shared_ptr<Child>> children;  // 父拥有子
    
    Parent(const std::string& n) : name(n) {
        std::cout << "    Parent '" << name << "' 创建\n";
    }
    ~Parent() {
        std::cout << "    Parent '" << name << "' 销毁\n";
    }
};

class Child {
public:
    std::string name;
    std::weak_ptr<Parent> parent;  // 子不拥有父，使用 weak_ptr
    
    Child(const std::string& n) : name(n) {
        std::cout << "    Child '" << name << "' 创建\n";
    }
    ~Child() {
        std::cout << "    Child '" << name << "' 销毁\n";
    }
    
    void showParent() const {
        if (auto p = parent.lock()) {
            std::cout << "    " << name << " 的父节点是: " << p->name << "\n";
        } else {
            std::cout << "    " << name << " 没有父节点\n";
        }
    }
};

void demo_parent_child() {
    std::cout << "\n【3. 父子关系中的循环引用】\n";
    std::cout << "  创建父子关系:\n";
    
    auto parent = std::make_shared<Parent>("P1");
    
    auto child1 = std::make_shared<Child>("C1");
    auto child2 = std::make_shared<Child>("C2");
    
    child1->parent = parent;  // weak_ptr 赋值
    child2->parent = parent;
    
    parent->children.push_back(child1);
    parent->children.push_back(child2);
    
    std::cout << "  父节点引用计数: " << parent.use_count() << "\n";
    std::cout << "  子节点1引用计数: " << child1.use_count() << "\n";
    
    child1->showParent();
    
    std::cout << "  ✅ 作用域结束时正确销毁\n";
}

} // namespace parent_child

// ============================================
// 4. 图结构中的循环引用
// ============================================
namespace graph_example {

class GraphNode {
public:
    int id;
    std::vector<std::weak_ptr<GraphNode>> neighbors;  // 图的边用 weak_ptr
    
    GraphNode(int i) : id(i) {
        std::cout << "    GraphNode(" << id << ") 创建\n";
    }
    ~GraphNode() {
        std::cout << "    GraphNode(" << id << ") 销毁\n";
    }
    
    void addNeighbor(std::shared_ptr<GraphNode> node) {
        neighbors.push_back(node);
    }
    
    void showNeighbors() const {
        std::cout << "    Node " << id << " 的邻居: ";
        for (const auto& wp : neighbors) {
            if (auto sp = wp.lock()) {
                std::cout << sp->id << " ";
            }
        }
        std::cout << "\n";
    }
};

void demo_graph() {
    std::cout << "\n【4. 图结构中的循环引用】\n";
    std::cout << "  创建有环图:\n";
    
    auto node1 = std::make_shared<GraphNode>(1);
    auto node2 = std::make_shared<GraphNode>(2);
    auto node3 = std::make_shared<GraphNode>(3);
    
    // 形成环: 1 -> 2 -> 3 -> 1
    node1->addNeighbor(node2);
    node2->addNeighbor(node3);
    node3->addNeighbor(node1);
    
    std::cout << "  各节点引用计数: " << node1.use_count() << ", "
              << node2.use_count() << ", " << node3.use_count() << "\n";
    
    node1->showNeighbors();
    
    std::cout << "  ✅ 使用 weak_ptr 的图结构无内存泄漏\n";
}

} // namespace graph_example

// ============================================
// 5. 循环引用检测和调试技巧
// ============================================
void demo_debug_tips() {
    std::cout << "\n【5. 循环引用调试技巧】\n";
    
    std::cout << "\n  常见循环引用场景:\n";
    std::cout << "    1. 双向链表 (prev/next)\n";
    std::cout << "    2. 父子双向引用\n";
    std::cout << "    3. 观察者模式双向注册\n";
    std::cout << "    4. 图结构的边\n";
    std::cout << "    5. 信号槽机制\n";
    
    std::cout << "\n  解决方案:\n";
    std::cout << "    1. 分析所有权关系\n";
    std::cout << "    2. 拥有方用 shared_ptr\n";
    std::cout << "    3. 被拥有方用 weak_ptr\n";
    std::cout << "    4. 使用 valgrind 检测泄漏\n";
}

// ============================================
// 主演示函数
// ============================================
void demo_break_cycle() {
    // 有问题的版本（需要手动打破）
    bad_design::demo_memory_leak();
    
    // 正确版本
    good_design::demo_no_leak();
    
    // 父子关系
    parent_child::demo_parent_child();
    
    // 图结构
    graph_example::demo_graph();
    
    // 调试技巧
    demo_debug_tips();
    
    std::cout << "\n💡 打破循环引用要点:\n";
    std::cout << "   1. 明确所有权关系\n";
    std::cout << "   2. 弱引用方使用 weak_ptr\n";
    std::cout << "   3. 通过 lock() 安全访问\n";
    std::cout << "   4. 避免不必要的 shared_ptr 互相引用\n";
}
