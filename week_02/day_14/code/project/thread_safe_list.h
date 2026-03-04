/**
 * @file thread_safe_list.h
 * @brief 线程安全链表实现
 *
 * 设计要点：
 * 1. 细粒度锁：每个节点一个mutex，提高并发度
 * 2. 手递手锁：遍历时逐个获取锁，避免死锁
 * 3. 异常安全：使用RAII管理锁
 * 4. 接口设计：提供安全的遍历和修改接口
 */

#ifndef THREAD_SAFE_LIST_H
#define THREAD_SAFE_LIST_H

#include <mutex>
#include <memory>
#include <functional>
#include <condition_variable>
#include <thread>

template<typename T>
class ThreadSafeList {
private:
    // 链表节点
    struct Node {
        std::mutex mutex;                    // 每个节点一个互斥锁
        std::shared_ptr<T> data;             // 数据（使用shared_ptr便于返回）
        std::unique_ptr<Node> next;          // 下一个节点

        Node() : data(nullptr) {}            // 哨兵节点的构造
        Node(const T& value) : data(std::make_shared<T>(value)) {}
    };

    Node head;                               // 哨兵节点（简化边界处理）
    mutable std::mutex size_mutex;           // 用于保护size_
    size_t size_ = 0;

public:
    // ============================================================
    // 构造与析构
    // ============================================================

    ThreadSafeList() = default;

    ~ThreadSafeList() {
        // 清空链表（unique_ptr会自动释放）
        remove_if([](const T&) { return true; });
    }

    // 禁止拷贝
    ThreadSafeList(const ThreadSafeList&) = delete;
    ThreadSafeList& operator=(const ThreadSafeList&) = delete;

    // 允许移动
    ThreadSafeList(ThreadSafeList&& other) noexcept {
        std::lock_guard<std::mutex> lk(other.size_mutex);
        head.next = std::move(other.head.next);
        size_ = other.size_;
        other.size_ = 0;
    }

    ThreadSafeList& operator=(ThreadSafeList&& other) noexcept {
        if (this != &other) {
            std::lock(size_mutex, other.size_mutex);
            std::lock_guard<std::mutex> lk1(size_mutex, std::adopt_lock);
            std::lock_guard<std::mutex> lk2(other.size_mutex, std::adopt_lock);

            remove_if([](const T&) { return true; });
            head.next = std::move(other.head.next);
            size_ = other.size_;
            other.size_ = 0;
        }
        return *this;
    }

    // ============================================================
    // 基本操作
    // ============================================================

    /**
     * @brief 在头部插入元素
     */
    void push_front(const T& value) {
        std::unique_ptr<Node> new_node = std::make_unique<Node>(value);

        std::lock_guard<std::mutex> lk(head.mutex);
        new_node->next = std::move(head.next);
        head.next = std::move(new_node);

        std::lock_guard<std::mutex> size_lk(size_mutex);
        ++size_;
    }

    /**
     * @brief 在头部插入元素（移动语义）
     */
    void push_front(T&& value) {
        std::unique_ptr<Node> new_node = std::make_unique<Node>(std::move(value));

        std::lock_guard<std::mutex> lk(head.mutex);
        new_node->next = std::move(head.next);
        head.next = std::move(new_node);

        std::lock_guard<std::mutex> size_lk(size_mutex);
        ++size_;
    }

    /**
     * @brief 在尾部插入元素
     */
    void push_back(const T& value) {
        std::unique_ptr<Node> new_node = std::make_unique<Node>(value);

        Node* current = &head;
        std::unique_lock<std::mutex> lk(head.mutex);

        // 遍历到最后一个节点
        while (current->next) {
            std::unique_lock<std::mutex> next_lk(current->next->mutex);
            lk.unlock();
            current = current->next.get();
            lk = std::move(next_lk);
        }

        current->next = std::move(new_node);

        std::lock_guard<std::mutex> size_lk(size_mutex);
        ++size_;
    }

    /**
     * @brief 获取并移除头部元素
     * @return 头部元素的值，如果链表为空返回nullptr
     */
    std::shared_ptr<T> try_pop_front() {
        std::lock_guard<std::mutex> lk(head.mutex);

        if (!head.next) {
            return nullptr;
        }

        std::unique_ptr<Node> old_head = std::move(head.next);
        head.next = std::move(old_head->next);

        std::lock_guard<std::mutex> size_lk(size_mutex);
        --size_;

        return old_head->data;
    }

    /**
     * @brief 获取链表大小
     */
    size_t size() const {
        std::lock_guard<std::mutex> lk(size_mutex);
        return size_;
    }

    /**
     * @brief 判断链表是否为空
     */
    bool empty() const {
        std::lock_guard<std::mutex> lk(size_mutex);
        return size_ == 0;
    }

    // ============================================================
    // 查找操作
    // ============================================================

    /**
     * @brief 查找第一个满足条件的元素
     * @param pred 判断函数
     * @return 找到的元素，如果没有返回nullptr
     */
    template<typename Predicate>
    std::shared_ptr<T> find_first_if(Predicate pred) {
        Node* current = &head;
        std::unique_lock<std::mutex> lk(head.mutex);

        while (Node* next = current->next.get()) {
            std::unique_lock<std::mutex> next_lk(next->mutex);
            lk.unlock();

            if (next->data && pred(*next->data)) {
                return next->data;
            }

            current = next;
            lk = std::move(next_lk);
        }

        return nullptr;
    }

    /**
     * @brief 遍历链表
     * @param func 对每个元素执行的函数
     */
    template<typename Func>
    void for_each(Func func) {
        Node* current = &head;
        std::unique_lock<std::mutex> lk(head.mutex);

        while (Node* next = current->next.get()) {
            std::unique_lock<std::mutex> next_lk(next->mutex);
            lk.unlock();

            if (next->data) {
                func(*next->data);
            }

            current = next;
            lk = std::move(next_lk);
        }
    }

    // ============================================================
    // 修改操作
    // ============================================================

    /**
     * @brief 移除满足条件的元素
     * @param pred 判断函数
     * @return 移除的元素数量
     */
    template<typename Predicate>
    size_t remove_if(Predicate pred) {
        size_t removed = 0;
        Node* current = &head;
        std::unique_lock<std::mutex> lk(head.mutex);

        while (Node* next = current->next.get()) {
            std::unique_lock<std::mutex> next_lk(next->mutex);

            if (next->data && pred(*next->data)) {
                // 移除节点
                std::unique_ptr<Node> old_node = std::move(current->next);
                current->next = std::move(old_node->next);
                ++removed;
                // 不释放next_lk，继续检查新的next
            } else {
                lk.unlock();
                current = next;
                lk = std::move(next_lk);
            }
        }

        if (removed > 0) {
            std::lock_guard<std::mutex> size_lk(size_mutex);
            size_ -= removed;
        }

        return removed;
    }

    /**
     * @brief 更新满足条件的第一个元素
     * @param pred 判断函数
     * @param new_value 新值
     * @return 是否更新成功
     */
    template<typename Predicate>
    bool update_first_if(Predicate pred, const T& new_value) {
        Node* current = &head;
        std::unique_lock<std::mutex> lk(head.mutex);

        while (Node* next = current->next.get()) {
            std::unique_lock<std::mutex> next_lk(next->mutex);
            lk.unlock();

            if (next->data && pred(*next->data)) {
                *next->data = new_value;
                return true;
            }

            current = next;
            lk = std::move(next_lk);
        }

        return false;
    }

    // ============================================================
    // 工具函数
    // ============================================================

    /**
     * @brief 将链表内容转换为vector（用于测试）
     */
    std::vector<T> to_vector() const {
        std::vector<T> result;
        const Node* current = &head;

        while (current->next) {
            if (current->next->data) {
                result.push_back(*current->next->data);
            }
            current = current->next.get();
        }

        return result;
    }

    /**
     * @brief 清空链表
     */
    void clear() {
        remove_if([](const T&) { return true; });
    }
};

#endif // THREAD_SAFE_LIST_H
