/**
 * @file smart_ptr_summary.cpp
 * @brief 智能指针与链表总结
 * 
 * 涵盖内容：
 * 1. unique_ptr 实现链表
 * 2. shared_ptr 实现链表
 * 3. 智能指针的优缺点
 * 4. 内存泄漏检测
 */

#include <iostream>
#include <memory>
#include <vector>
#include <string>

// ============================================
// 1. unique_ptr 实现链表
// ============================================

template<typename T>
struct UniqueListNode {
    T data;
    std::unique_ptr<UniqueListNode<T>> next;
    
    explicit UniqueListNode(T val) : data(std::move(val)), next(nullptr) {}
};

template<typename T>
class UniquePtrList {
public:
    UniquePtrList() : head_(nullptr), size_(0) {}
    
    // 禁止拷贝
    UniquePtrList(const UniquePtrList&) = delete;
    UniquePtrList& operator=(const UniquePtrList&) = delete;
    
    // 允许移动
    UniquePtrList(UniquePtrList&& other) noexcept 
        : head_(std::move(other.head_)), size_(other.size_) {
        other.size_ = 0;
    }
    
    UniquePtrList& operator=(UniquePtrList&& other) noexcept {
        if (this != &other) {
            head_ = std::move(other.head_);
            size_ = other.size_;
            other.size_ = 0;
        }
        return *this;
    }
    
    void push_front(T val) {
        auto newNode = std::make_unique<UniqueListNode<T>>(std::move(val));
        newNode->next = std::move(head_);
        head_ = std::move(newNode);
        ++size_;
    }
    
    void pop_front() {
        if (head_) {
            head_ = std::move(head_->next);
            --size_;
        }
    }
    
    T* front() const {
        return head_ ? &head_->data : nullptr;
    }
    
    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }
    
    void print(const std::string& name = "") const {
        if (!name.empty()) std::cout << name << ": ";
        UniqueListNode<T>* curr = head_.get();
        while (curr) {
            std::cout << curr->data;
            if (curr->next) std::cout << " -> ";
            curr = curr->next.get();
        }
        std::cout << "\n";
    }
    
private:
    std::unique_ptr<UniqueListNode<T>> head_;
    size_t size_;
};

// ============================================
// 2. shared_ptr 实现链表
// ============================================

template<typename T>
struct SharedListNode {
    T data;
    std::shared_ptr<SharedListNode<T>> next;
    
    explicit SharedListNode(T val) : data(std::move(val)), next(nullptr) {}
};

template<typename T>
class SharedPtrList {
public:
    SharedPtrList() : head_(nullptr), size_(0) {}
    
    void push_front(T val) {
        auto newNode = std::make_shared<SharedListNode<T>>(std::move(val));
        newNode->next = head_;
        head_ = newNode;
        ++size_;
    }
    
    void pop_front() {
        if (head_) {
            head_ = head_->next;
            --size_;
        }
    }
    
    // 创建环（用于演示shared_ptr的循环引用问题）
    void createCycle(int fromIndex) {
        if (!head_) return;
        
        auto curr = head_;
        std::shared_ptr<SharedListNode<T>> target;
        int index = 0;
        
        while (curr->next) {
            if (index == fromIndex) {
                target = curr;
            }
            curr = curr->next;
            ++index;
        }
        
        if (target) {
            curr->next = target;  // 创建环
            std::cout << "注意：创建了循环引用，会导致内存泄漏!\n";
        }
    }
    
    void print(const std::string& name = "", int maxNodes = 20) const {
        if (!name.empty()) std::cout << name << ": ";
        auto curr = head_;
        int count = 0;
        while (curr && count < maxNodes) {
            std::cout << curr->data;
            if (curr->next) std::cout << " -> ";
            curr = curr->next;
            ++count;
        }
        if (count >= maxNodes) std::cout << "...";
        std::cout << "\n";
    }
    
    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }
    std::shared_ptr<SharedListNode<T>> getHead() const { return head_; }
    
private:
    std::shared_ptr<SharedListNode<T>> head_;
    size_t size_;
};

// ============================================
// 3. weak_ptr 解决循环引用
// ============================================

template<typename T>
struct WeakListNode {
    T data;
    std::shared_ptr<WeakListNode<T>> next;
    std::weak_ptr<WeakListNode<T>> prev;  // 使用weak_ptr避免循环引用
    
    explicit WeakListNode(T val) : data(std::move(val)), next(nullptr) {}
};

template<typename T>
class DoublyLinkedList {
public:
    void push_front(T val) {
        auto newNode = std::make_shared<WeakListNode<T>>(std::move(val));
        if (head_) {
            newNode->next = head_;
            head_->prev = newNode;
        }
        head_ = newNode;
        ++size_;
    }
    
    void print(const std::string& name = "") const {
        if (!name.empty()) std::cout << name << ": ";
        auto curr = head_;
        while (curr) {
            std::cout << curr->data;
            if (curr->next) std::cout << " <-> ";
            curr = curr->next;
        }
        std::cout << "\n";
    }
    
private:
    std::shared_ptr<WeakListNode<T>> head_;
    size_t size_ = 0;
};

// ============================================
// 4. 智能指针 vs 原始指针对比
// ============================================

void comparePointerTypes() {
    std::cout << "【智能指针 vs 原始指针对比】\n\n";
    
    std::cout << "┌─────────────────┬──────────────┬──────────────┬──────────────┐\n";
    std::cout << "│     特性        │  原始指针    │  unique_ptr  │  shared_ptr  │\n";
    std::cout << "├─────────────────┼──────────────┼──────────────┼──────────────┤\n";
    std::cout << "│ 自动释放内存    │      否      │      是      │      是      │\n";
    std::cout << "│ 可拷贝          │      是      │      否      │      是      │\n";
    std::cout << "│ 可移动          │      是      │      是      │      是      │\n";
    std::cout << "│ 额外开销        │      无      │     极小     │     有       │\n";
    std::cout << "│ 循环引用问题    │   内存泄漏   │   不会发生   │  需weak_ptr  │\n";
    std::cout << "│ 适用场景        │   非拥有     │   独占所有权 │   共享所有权 │\n";
    std::cout << "└─────────────────┴──────────────┴──────────────┴──────────────┘\n";
}

// ============================================
// 演示函数
// ============================================

namespace practice {

void demonstrateSmartPtrSummary() {
    // 1. unique_ptr 链表演示
    std::cout << "【unique_ptr 链表】\n";
    {
        UniquePtrList<int> list;
        list.push_front(3);
        list.push_front(2);
        list.push_front(1);
        list.print("链表内容");
        
        std::cout << "front: " << *list.front() << "\n";
        std::cout << "size: " << list.size() << "\n";
        
        list.pop_front();
        list.print("pop_front后");
        
        std::cout << "unique_ptr 自动管理内存，离开作用域自动释放\n";
    }
    std::cout << "已离开作用域，内存已释放\n\n";
    
    // 2. shared_ptr 链表演示
    std::cout << "【shared_ptr 链表】\n";
    {
        SharedPtrList<int> list;
        list.push_front(3);
        list.push_front(2);
        list.push_front(1);
        list.print("链表内容");
        
        // 共享所有权示例
        auto head = list.getHead();
        std::cout << "head 引用计数: " << head.use_count() << "\n";
        
        {
            auto copy = head;
            std::cout << "复制后引用计数: " << head.use_count() << "\n";
        }
        std::cout << "离开作用域后引用计数: " << head.use_count() << "\n";
    }
    std::cout << "\n";
    
    // 3. 双向链表（weak_ptr解决循环引用）
    std::cout << "【双向链表 - weak_ptr解决循环引用】\n";
    {
        DoublyLinkedList<int> dlist;
        dlist.push_front(3);
        dlist.push_front(2);
        dlist.push_front(1);
        dlist.print("双向链表内容");
        std::cout << "使用weak_ptr作为prev指针，避免循环引用\n";
    }
    std::cout << "\n";
    
    // 4. 对比表格
    comparePointerTypes();
    
    // 5. 最佳实践建议
    std::cout << "\n【最佳实践建议】\n";
    std::cout << "1. 优先使用 unique_ptr（独占所有权）\n";
    std::cout << "2. 需要共享时使用 shared_ptr\n";
    std::cout << "3. 打破循环引用使用 weak_ptr\n";
    std::cout << "4. 链表节点通常用 unique_ptr 或 shared_ptr\n";
    std::cout << "5. 对于算法题，通常使用原始指针更简洁\n";
}

} // namespace practice
