/**
 * @file exercises.cpp
 * @brief 第二周综合练习题
 *
 * 练习内容：
 * 1. 智能指针基础练习
 * 2. 移动语义练习
 * 3. RAII资源管理练习
 * 4. 综合应用练习
 */

#include "review/exercises.h"

#include <cstdio>
#include <cstring>
#include <iostream>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

namespace exercises {

// ============================================================
// 练习1: 实现自己的unique_ptr简化版
// ============================================================

/**
 * @brief 简化版unique_ptr实现
 */
template<typename T>
class MyUniquePtr {
public:
    MyUniquePtr() : ptr_(nullptr) {}
    explicit MyUniquePtr(T* ptr) : ptr_(ptr) {}
    ~MyUniquePtr() { delete ptr_; }

    // 禁止拷贝
    MyUniquePtr(const MyUniquePtr&) = delete;
    MyUniquePtr& operator=(const MyUniquePtr&) = delete;

    // 允许移动
    MyUniquePtr(MyUniquePtr&& other) noexcept : ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }

    MyUniquePtr& operator=(MyUniquePtr&& other) noexcept {
        if (this != &other) {
            delete ptr_;
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
        }
        return *this;
    }

    // 解引用
    T& operator*() const { return *ptr_; }
    T* operator->() const { return ptr_; }

    // 其他操作
    T* get() const { return ptr_; }
    T* release() {
        T* temp = ptr_;
        ptr_ = nullptr;
        return temp;
    }
    void reset(T* ptr = nullptr) {
        delete ptr_;
        ptr_ = ptr;
    }

    explicit operator bool() const { return ptr_ != nullptr; }

private:
    T* ptr_;
};

void exercise1_unique_ptr() {
    std::cout << "\n=== 练习1: 自定义unique_ptr ===\n";

    // 基本操作
    MyUniquePtr<int> p1(new int(42));
    std::cout << "p1: " << *p1 << "\n";

    // 移动操作
    MyUniquePtr<int> p2 = std::move(p1);
    std::cout << "p2 after move: " << *p2 << "\n";
    std::cout << "p1 after move is null: " << (p1 ? "no" : "yes") << "\n";

    // reset
    p2.reset(new int(100));
    std::cout << "p2 after reset: " << *p2 << "\n";

    // release
    int* raw = p2.release();
    std::cout << "Released raw ptr: " << *raw << "\n";
    delete raw;

    std::cout << "练习1完成!\n";
}

// ============================================================
// 练习2: 实现引用计数的shared_ptr简化版
// ============================================================

/**
 * @brief 简化版shared_ptr实现
 */
template<typename T>
class MySharedPtr {
public:
    MySharedPtr() : ptr_(nullptr), count_(nullptr) {}

    explicit MySharedPtr(T* ptr) : ptr_(nullptr), count_(nullptr) {
        // 先用临时独占所有者接住调用者交来的资源。若控制块分配失败，
        // guard 会删除 ptr；只有两部分都准备好后才提交对象状态。
        std::unique_ptr<T> guard(ptr);
        if (ptr) {
            auto count = std::make_unique<int>(1);
            ptr_ = guard.release();
            count_ = count.release();
        }
    }

    ~MySharedPtr() {
        if (count_) {
            (*count_)--;
            if (*count_ == 0) {
                delete ptr_;
                delete count_;
            }
        }
    }

    // 拷贝构造
    MySharedPtr(const MySharedPtr& other)
        : ptr_(other.ptr_), count_(other.count_) {
        if (count_) (*count_)++;
    }

    // 拷贝赋值
    MySharedPtr& operator=(const MySharedPtr& other) {
        if (this != &other) {
            // 减少当前引用计数
            if (count_) {
                (*count_)--;
                if (*count_ == 0) {
                    delete ptr_;
                    delete count_;
                }
            }
            // 复制新值
            ptr_ = other.ptr_;
            count_ = other.count_;
            if (count_) (*count_)++;
        }
        return *this;
    }

    // 移动构造
    MySharedPtr(MySharedPtr&& other) noexcept
        : ptr_(other.ptr_), count_(other.count_) {
        other.ptr_ = nullptr;
        other.count_ = nullptr;
    }

    // 移动赋值
    MySharedPtr& operator=(MySharedPtr&& other) noexcept {
        if (this != &other) {
            if (count_) {
                (*count_)--;
                if (*count_ == 0) {
                    delete ptr_;
                    delete count_;
                }
            }
            ptr_ = other.ptr_;
            count_ = other.count_;
            other.ptr_ = nullptr;
            other.count_ = nullptr;
        }
        return *this;
    }

    T& operator*() const { return *ptr_; }
    T* operator->() const { return ptr_; }
    T* get() const { return ptr_; }
    int use_count() const { return count_ ? *count_ : 0; }
    explicit operator bool() const { return ptr_ != nullptr; }

private:
    T* ptr_;
    int* count_;
};

void exercise2_shared_ptr() {
    std::cout << "\n=== 练习2: 自定义shared_ptr ===\n";

    MySharedPtr<int> p1(new int(42));
    std::cout << "p1 use_count: " << p1.use_count() << "\n";

    {
        MySharedPtr<int> p2 = p1;
        std::cout << "After copy, p1 use_count: " << p1.use_count() << "\n";
        std::cout << "p2 value: " << *p2 << "\n";
    }

    std::cout << "After scope exit, p1 use_count: " << p1.use_count() << "\n";

    MySharedPtr<int> p3 = std::move(p1);
    std::cout << "After move, p3 use_count: " << p3.use_count() << "\n";
    std::cout << "p1 is null: " << (p1 ? "no" : "yes") << "\n";

    std::cout << "练习2完成!\n";
}

// ============================================================
// 练习3: 实现移动语义的String类
// ============================================================

/**
 * @brief 具有移动语义的简单String类
 */
class MyString {
public:
    // 默认构造
    MyString() : data_(nullptr), size_(0) {}

    // 从C字符串构造
    MyString(const char* str) : data_(nullptr), size_(0) {
        if (!str) {
            return;
        }
        size_ = std::strlen(str);
        data_ = new char[size_ + 1];
        std::strcpy(data_, str);
    }

    // 析构
    ~MyString() {
        delete[] data_;
    }

    // 拷贝构造
    MyString(const MyString& other) : MyString(other.c_str()) {}

    // 拷贝赋值：先完成临时副本的分配，再交换资源，提供强异常保证。
    MyString& operator=(const MyString& other) {
        if (this != &other) {
            MyString temp(other);
            swap(temp);
        }
        return *this;
    }

    // 移动构造
    MyString(MyString&& other) noexcept
        : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;
        other.size_ = 0;
    }

    // 移动赋值
    MyString& operator=(MyString&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            size_ = other.size_;
            other.data_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    // 操作
    const char* c_str() const { return data_ ? data_ : ""; }
    size_t size() const { return size_; }

    void swap(MyString& other) noexcept {
        using std::swap;
        swap(data_, other.data_);
        swap(size_, other.size_);
    }

private:
    char* data_;
    size_t size_;
};

static_assert(std::is_nothrow_move_constructible_v<MyString>);
static_assert(std::is_nothrow_move_assignable_v<MyString>);

void exercise3_move_string() {
    std::cout << "\n=== 练习3: 移动语义String类 ===\n";

    MyString s1("Hello");
    std::cout << "s1: " << s1.c_str() << "\n";

    // 拷贝
    MyString s2 = s1;
    std::cout << "s2: " << s2.c_str() << "\n";

    // 移动
    MyString s3 = std::move(s1);
    std::cout << "s3: " << s3.c_str() << "\n";
    std::cout << "s1 after move: \"" << s1.c_str() << "\" (size: " << s1.size() << ")\n";

    std::cout << "练习3完成!\n";
}

// ============================================================
// 练习4: 实现线程安全的计数器
// ============================================================

/**
 * @brief 线程安全计数器
 */
class ThreadSafeCounter {
public:
    ThreadSafeCounter() : value_(0) {}

    void increment() {
        std::lock_guard<std::mutex> lock(mutex_);
        ++value_;
    }

    void decrement() {
        std::lock_guard<std::mutex> lock(mutex_);
        --value_;
    }

    int get() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return value_;
    }

    void reset() {
        std::lock_guard<std::mutex> lock(mutex_);
        value_ = 0;
    }

private:
    mutable std::mutex mutex_;
    int value_;
};

void exercise4_threadsafe_counter() {
    std::cout << "\n=== 练习4: 线程安全计数器 ===\n";

    ThreadSafeCounter counter;

    const int num_threads = 4;
    const int increments_per_thread = 10000;

    std::vector<std::thread> threads;

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&counter, increments_per_thread]() {
            for (int j = 0; j < increments_per_thread; ++j) {
                counter.increment();
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Expected value: " << num_threads * increments_per_thread << "\n";
    std::cout << "Actual value: " << counter.get() << "\n";

    if (counter.get() != num_threads * increments_per_thread) {
        throw std::runtime_error("线程安全计数器结果不正确");
    }
    std::cout << "练习4完成!\n";
}

// ============================================================
// 练习5: 实现对象池
// ============================================================

/**
 * @brief 对象池模板
 */
template<typename T>
class ObjectPool {
private:
    struct State {
        std::mutex mutex;
        std::vector<std::unique_ptr<T>> available;
    };

public:
    ObjectPool() : state_(std::make_shared<State>()) {}

    std::shared_ptr<T> acquire() {
        std::unique_ptr<T> object;
        {
            std::lock_guard<std::mutex> lock(state_->mutex);
            if (state_->available.empty()) {
                object = std::make_unique<T>();
            } else {
                object = std::move(state_->available.back());
                state_->available.pop_back();
            }
        }

        T* raw = object.release();
        auto state = state_;
        auto return_to_pool = [state](T* ptr) noexcept {
            if (!ptr) {
                return;
            }

            // shared_ptr析构会调用删除器，因此删除器绝不能让异常逃逸。
            // 加锁和vector扩容都可能抛异常：先reserve，确保随后的emplace_back
            // 不再分配；任何失败都直接delete，至少保证对象不会泄漏或terminate。
            try {
                std::lock_guard<std::mutex> lock(state->mutex);
                state->available.reserve(state->available.size() + 1);
                state->available.emplace_back(ptr);
                ptr = nullptr;  // 所有权已交给池
            } catch (...) {
                delete ptr;
            }
        };
        return std::shared_ptr<T>(raw, std::move(return_to_pool));
    }

private:
    // 归还器捕获共享状态而不是this，因此借出的对象可以安全地晚于ObjectPool析构。
    std::shared_ptr<State> state_;
};

void exercise5_object_pool() {
    std::cout << "\n=== 练习5: 对象池 ===\n";

    struct Data {
        int id;
        Data() : id(0) {}
        explicit Data(int i) : id(i) {}
    };

    ObjectPool<Data> pool;

    {
        auto obj1 = pool.acquire();
        obj1->id = 1;
        std::cout << "Acquired object with id: " << obj1->id << "\n";

        auto obj2 = pool.acquire();
        obj2->id = 2;
        std::cout << "Acquired object with id: " << obj2->id << "\n";
    }  // 对象归还到池中

    auto obj3 = pool.acquire();  // 可能复用之前的对象
    std::cout << "Re-acquired object with id: " << obj3->id << "\n";

    std::cout << "练习5完成!\n";
}

// ============================================================
// 练习6: 链表操作综合练习
// ============================================================

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// 反转链表
ListNode* reverseList(ListNode* head) {
    ListNode* prev = nullptr;
    ListNode* curr = head;
    while (curr) {
        ListNode* next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    return prev;
}

// 快慢指针找中点
ListNode* findMiddle(ListNode* head) {
    ListNode* slow = head;
    ListNode* fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

// 合并两个有序链表
ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
    ListNode dummy(0);
    ListNode* tail = &dummy;

    while (l1 && l2) {
        if (l1->val < l2->val) {
            tail->next = l1;
            l1 = l1->next;
        } else {
            tail->next = l2;
            l2 = l2->next;
        }
        tail = tail->next;
    }

    tail->next = l1 ? l1 : l2;
    return dummy.next;
}

// 打印链表
void printList(ListNode* head) {
    while (head) {
        std::cout << head->val;
        if (head->next) std::cout << " -> ";
        head = head->next;
    }
    std::cout << "\n";
}

// 释放链表
void deleteList(ListNode* head) {
    while (head) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

ListNode* createList(const std::vector<int>& values) {
    if (values.empty()) {
        return nullptr;
    }

    std::unique_ptr<ListNode, void (*)(ListNode*)> owner(
        new ListNode(values[0]), &deleteList);
    ListNode* tail = owner.get();
    for (std::size_t i = 1; i < values.size(); ++i) {
        auto node = std::make_unique<ListNode>(values[i]);
        tail->next = node.release();
        tail = tail->next;
    }
    return owner.release();
}

void exercise6_list_operations() {
    std::cout << "\n=== 练习6: 链表操作 ===\n";

    // 创建链表 1->2->3->4->5
    ListNode* head = createList({1, 2, 3, 4, 5});

    std::cout << "Original list: ";
    printList(head);

    // 反转
    head = reverseList(head);
    std::cout << "After reverse: ";
    printList(head);

    // 找中点
    ListNode* mid = findMiddle(head);
    std::cout << "Middle node: " << mid->val << "\n";

    // 创建另一个有序链表并合并
    ListNode* l2 = createList({0, 2, 4, 6});

    std::cout << "Second list: ";
    printList(l2);

    head = reverseList(head);  // 恢复顺序
    ListNode* merged = mergeTwoLists(head, l2);
    std::cout << "After merge: ";
    printList(merged);

    deleteList(merged);

    std::cout << "练习6完成!\n";
}

// ============================================================
// 练习7: RAII文件句柄
// ============================================================

class FileHandle {
public:
    explicit FileHandle(const char* filename, const char* mode)
        : file_(fopen(filename, mode)) {
        if (!file_) {
            throw std::runtime_error("Failed to open file");
        }
    }

    ~FileHandle() {
        if (file_) {
            fclose(file_);
        }
    }

    // 禁止拷贝
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;

    // 允许移动
    FileHandle(FileHandle&& other) noexcept : file_(other.file_) {
        other.file_ = nullptr;
    }

    FileHandle& operator=(FileHandle&& other) noexcept {
        if (this != &other) {
            if (file_) fclose(file_);
            file_ = other.file_;
            other.file_ = nullptr;
        }
        return *this;
    }

    FILE* get() const { return file_; }
    explicit operator bool() const { return file_ != nullptr; }

private:
    FILE* file_;
};

void exercise7_raii_file() {
    std::cout << "\n=== 练习7: RAII文件句柄 ===\n";

    {
        FileHandle file("/dev/null", "w");
        if (file) {
            fprintf(file.get(), "test");
            std::cout << "File opened successfully\n";
        }
    }  // 自动关闭

    std::cout << "File closed automatically (RAII)\n";
    std::cout << "练习7完成!\n";
}

// ============================================================
// 主函数
// ============================================================

void run_exercises() {
    std::cout << "╔════════════════════════════════════════════╗\n";
    std::cout << "║      第二周综合练习题                         ║\n";
    std::cout << "╚════════════════════════════════════════════╝\n";

    exercise1_unique_ptr();
    exercise2_shared_ptr();
    exercise3_move_string();
    exercise4_threadsafe_counter();
    exercise5_object_pool();
    exercise6_list_operations();
    exercise7_raii_file();

    std::cout << "\n=== 所有练习完成! ===\n";
    std::cout << "核心收获:\n";
    std::cout << "1. 理解智能指针的内部实现\n";
    std::cout << "2. 掌握移动语义的正确实现\n";
    std::cout << "3. 学会使用RAII管理资源\n";
    std::cout << "4. 理解线程安全的编程模式\n";
}

} // namespace exercises
