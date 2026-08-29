/**
 * @file thread_safe_list.h
 * @brief 线程安全链表实现
 *
 * 设计要点：
 * 1. 细粒度锁：每个节点一个mutex，提高并发度
 * 2. 锁耦合：始终按前驱到后继的方向加锁
 * 3. 生命周期钉住：遍历者持有shared_ptr<Node>，节点即使被链表摘除也不会销毁
 * 4. 异常提交：可能抛出的构造先完成；删除逐节点提交并提供基本保证
 */

#ifndef WEEK_02_DAY_14_THREAD_SAFE_LIST_H
#define WEEK_02_DAY_14_THREAD_SAFE_LIST_H

#include <cstddef>
#include <memory>
#include <mutex>
#include <utility>
#include <vector>

namespace week2_concurrency {

template<typename T>
class ThreadSafeList {
private:
    // 链表节点
    struct Node {
        mutable std::mutex mutex;            // 每个节点一个互斥锁
        std::shared_ptr<T> data;             // 数据（使用shared_ptr便于返回）
        // 链表和正在遍历的线程可能短暂共同拥有节点。单向shared_ptr不会形成环，
        // 局部副本是显式生命周期令牌，避免“mutex仍被访问但Node已析构”。
        std::shared_ptr<Node> next;

        Node() : data(nullptr) {}            // 哨兵节点的构造
        Node(const T& value) : data(std::make_shared<T>(value)) {}
        Node(T&& value) : data(std::make_shared<T>(std::move(value))) {}
    };

    Node head;                               // 哨兵节点（简化边界处理）
public:
    // ============================================================
    // 构造与析构
    // ============================================================

    ThreadSafeList() = default;

    // 析构不加锁；和所有标准容器一样，调用者必须先停止并join所有访问线程。
    // 逐节点移走next可避免长链经shared_ptr成员递归析构；整个路径只做noexcept
    // 的shared_ptr移动/销毁，不调用用户谓词或输出。
    ~ThreadSafeList() noexcept {
        auto current = std::move(head.next);
        while (current) {
            auto next = std::move(current->next);
            current = std::move(next);
        }
    }

    // 禁止拷贝
    ThreadSafeList(const ThreadSafeList&) = delete;
    ThreadSafeList& operator=(const ThreadSafeList&) = delete;

    // mutex本身不可移动。若业务确实需要移动容器，应先定义“移动期间其他线程
    // 是否还能访问”以及跨两个容器的锁顺序；本入门版本明确禁止移动容器本身。
    ThreadSafeList(ThreadSafeList&&) = delete;
    ThreadSafeList& operator=(ThreadSafeList&&) = delete;

    // ============================================================
    // 基本操作
    // ============================================================

    /**
     * @brief 在头部插入元素
     */
    void push_front(const T& value) {
        auto new_node = std::make_shared<Node>(value);

        std::lock_guard<std::mutex> lk(head.mutex);
        new_node->next = head.next;
        head.next = std::move(new_node);
    }

    /**
     * @brief 在头部插入元素（移动语义）
     */
    void push_front(T&& value) {
        auto new_node = std::make_shared<Node>(std::move(value));

        std::lock_guard<std::mutex> lk(head.mutex);
        new_node->next = head.next;
        head.next = std::move(new_node);
    }

    /**
     * @brief 在尾部插入元素
     */
    void push_back(const T& value) {
        auto new_node = std::make_shared<Node>(value);

        append_node(std::move(new_node));
    }

    /**
     * @brief 在尾部插入元素（移动语义）
     */
    void push_back(T&& value) {
        auto new_node = std::make_shared<Node>(std::move(value));

        append_node(std::move(new_node));
    }

private:
    void append_node(std::shared_ptr<Node> new_node) {

        Node* current = &head;
        std::shared_ptr<Node> current_owner;
        std::unique_lock<std::mutex> lk(head.mutex);

        // 遍历到最后一个节点
        while (auto next_owner = current->next) {
            std::unique_lock<std::mutex> next_lk(next_owner->mutex);
            lk.unlock();
            // 先让lk改为引用仍被next_owner钉住的后继mutex，再释放旧owner。
            // 否则旧Node可能已经析构，而一个虽未持锁但仍保存旧mutex地址的
            // unique_lock对象还活着，锁对象和被保护对象的生命周期边界就不清楚。
            lk = std::move(next_lk);
            current_owner = std::move(next_owner);
            current = current_owner.get();
        }

        current->next = std::move(new_node);
    }

public:

    /**
     * @brief 获取并移除头部元素
     * @return 头部元素的值，如果链表为空返回nullptr
     */
    std::shared_ptr<T> try_pop_front() {
        std::unique_lock<std::mutex> head_lk(head.mutex);

        if (!head.next) {
            return nullptr;
        }

        auto old_head = head.next;  // 生命周期令牌在解锁前一直持有节点
        std::unique_lock<std::mutex> node_lk(old_head->mutex);
        auto result = old_head->data;  // shared_ptr复制不分配、不抛异常
        head.next = old_head->next;
        return result;
    }

    /**
     * @brief 获取链表大小
     */
    size_t size() const {
        size_t count = 0;
        for_each([&count](const T&) { ++count; });
        return count;
    }

    /**
     * @brief 判断链表是否为空
     */
    bool empty() const {
        std::lock_guard<std::mutex> lk(head.mutex);
        return !head.next;
    }

    // ============================================================
    // 查找操作
    // ============================================================

    /**
     * @brief 查找第一个满足条件的元素
     * @param pred 判断函数
     * @return 找到值的不可变版本快照，如果没有返回nullptr
     *
     * 返回的shared_ptr会延长这个版本的值的寿命。后续update_first_if会为节点
     * 换入新的shared_ptr，而不会原地修改旧版本，因此该句柄不是节点的实时视图。
     * 谓词在节点锁持有期间执行，不得重入同一个链表。
     */
    template<typename Predicate>
    std::shared_ptr<const T> find_first_if(Predicate pred) const {
        const Node* current = &head;
        std::shared_ptr<Node> current_owner;
        std::unique_lock<std::mutex> lk(head.mutex);

        while (auto next_owner = current->next) {
            std::unique_lock<std::mutex> next_lk(next_owner->mutex);
            lk.unlock();
            lk = std::move(next_lk);
            current_owner = std::move(next_owner);
            current = current_owner.get();

            if (current->data && pred(static_cast<const T&>(*current->data))) {
                return current->data;
            }
        }

        return nullptr;
    }

    /**
     * @brief 遍历链表
     * @param func 对每个元素执行的函数
     */
    template<typename Func>
    void for_each(Func func) const {
        const Node* current = &head;
        std::shared_ptr<Node> current_owner;
        std::unique_lock<std::mutex> lk(head.mutex);

        while (auto next_owner = current->next) {
            std::unique_lock<std::mutex> next_lk(next_owner->mutex);
            lk.unlock();
            lk = std::move(next_lk);
            current_owner = std::move(next_owner);
            current = current_owner.get();

            if (current->data) {
                // 回调在节点锁持有期间执行，以保证引用有效且值不被本类修改。
                // 回调不得重入同一个链表，否则可能再次请求同一把mutex而死锁。
                func(static_cast<const T&>(*current->data));
            }
        }
    }

    // ============================================================
    // 修改操作
    // ============================================================

    /**
     * @brief 移除满足条件的元素
     * @param pred 判断函数
     * @return 移除的元素数量
     * @note 谓词在候选节点锁持有期间执行且不得重入。每个匹配节点都以一次
     *       noexcept的shared_ptr重连独立提交；若后续谓词抛异常，已经提交的
     *       删除保留，未检查后缀仍有效（基本保证），不存在另行维护的计数元数据。
     */
    template<typename Predicate>
    size_t remove_if(Predicate pred) {
        size_t removed = 0;
        Node* current = &head;
        std::shared_ptr<Node> current_owner;
        std::unique_lock<std::mutex> lk(head.mutex);

        while (auto next_owner = current->next) {
            std::unique_lock<std::mutex> next_lk(next_owner->mutex);

            if (next_owner->data &&
                pred(static_cast<const T&>(*next_owner->data))) {
                // next_owner显式钉住候选节点；重连提交后先释放锁，再允许析构。
                current->next = next_owner->next;
                ++removed;
                next_lk.unlock();
            } else {
                lk.unlock();
                lk = std::move(next_lk);
                current_owner = std::move(next_owner);
                current = current_owner.get();
            }
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
        std::shared_ptr<Node> current_owner;
        std::unique_lock<std::mutex> lk(head.mutex);

        while (auto next_owner = current->next) {
            std::unique_lock<std::mutex> next_lk(next_owner->mutex);
            lk.unlock();
            lk = std::move(next_lk);
            current_owner = std::move(next_owner);
            current = current_owner.get();

            if (current->data && pred(static_cast<const T&>(*current->data))) {
                // 命中后先完成可能抛异常的值构造，再以noexcept的shared_ptr移动赋值
                // 提交。失败时节点仍指向旧版本；旧查找句柄也始终只读旧版本。
                auto replacement = std::make_shared<T>(new_value);
                current->data = std::move(replacement);
                return true;
            }
        }

        return false;
    }

    // ============================================================
    // 工具函数
    // ============================================================

    /**
     * @brief 将链表内容转换为vector（用于测试）
     * @note 并发修改时它不是全局原子快照；它只保证所访问节点均在锁保护下读取。
     */
    std::vector<T> to_vector() const {
        std::vector<T> result;
        for_each([&result](const T& value) { result.push_back(value); });
        return result;
    }

    /**
     * @brief 清空链表
     */
    void clear() {
        remove_if([](const T&) { return true; });
    }
};

} // namespace week2_concurrency

#endif // WEEK_02_DAY_14_THREAD_SAFE_LIST_H
