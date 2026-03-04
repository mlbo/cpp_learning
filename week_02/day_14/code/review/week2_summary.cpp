/**
 * @file week2_summary.cpp
 * @brief 第二周知识点总结代码
 *
 * 本文件涵盖：
 * 1. 智能指针（unique_ptr, shared_ptr, weak_ptr）
 * 2. 右值引用与移动语义
 * 3. 完美转发
 * 4. RAII资源管理
 */

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <utility>
#include <functional>

namespace week2 {

// ============================================================
// Part 1: unique_ptr - 独占所有权智能指针
// ============================================================

/**
 * @brief unique_ptr使用示例
 *
 * 特点：
 * - 独占所有权，不可拷贝
 * - 可移动，资源转移
 * - 零开销抽象
 * - 支持自定义删除器
 */
void demo_unique_ptr() {
    std::cout << "\n=== unique_ptr 示例 ===\n";

    // 方式1：make_unique（推荐）
    auto p1 = std::make_unique<int>(42);
    std::cout << "p1 value: " << *p1 << "\n";

    // 方式2：直接构造
    std::unique_ptr<std::string> p2(new std::string("Hello"));
    std::cout << "p2 value: " << *p2 << "\n";

    // 移动语义
    auto p3 = std::move(p2);  // p2变为nullptr
    std::cout << "After move, p3: " << *p3 << "\n";
    std::cout << "p2 is null: " << (p2 == nullptr) << "\n";

    // 自定义删除器示例
    auto file_deleter = [](FILE* f) {
        if (f) {
            std::cout << "Closing file...\n";
            fclose(f);
        }
    };

    {
        std::unique_ptr<FILE, decltype(file_deleter)> file(
            fopen("/dev/null", "w"), file_deleter
        );
        if (file) {
            fprintf(file.get(), "test");
            std::cout << "File opened\n";
        }
    }  // 文件在此自动关闭

    // 数组支持
    auto arr = std::make_unique<int[]>(5);
    arr[0] = 1;
    arr[1] = 2;
    std::cout << "Array: " << arr[0] << ", " << arr[1] << "\n";
}

// ============================================================
// Part 2: shared_ptr - 共享所有权智能指针
// ============================================================

/**
 * @brief shared_ptr使用示例
 *
 * 特点：
 * - 引用计数管理共享所有权
 * - 控制块存储引用计数和弱引用计数
 * - 循环引用问题需注意
 */
class SharedResource {
public:
    SharedResource(const std::string& name) : name_(name) {
        std::cout << "SharedResource(" << name_ << ") created\n";
    }
    ~SharedResource() {
        std::cout << "SharedResource(" << name_ << ") destroyed\n";
    }
    const std::string& name() const { return name_; }
private:
    std::string name_;
};

void demo_shared_ptr() {
    std::cout << "\n=== shared_ptr 示例 ===\n";

    // make_shared推荐：一次分配完成对象和控制块
    auto sp1 = std::make_shared<SharedResource>("Resource1");
    std::cout << "use_count: " << sp1.use_count() << "\n";

    {
        auto sp2 = sp1;  // 引用计数+1
        std::cout << "After copy, use_count: " << sp1.use_count() << "\n";
        auto sp3 = sp1;
        std::cout << "After another copy, use_count: " << sp1.use_count() << "\n";
    }  // sp2, sp3离开作用域，引用计数-1

    std::cout << "After scope exit, use_count: " << sp1.use_count() << "\n";

    // 注意：避免从裸指针创建多个shared_ptr
    // SharedResource* raw = new SharedResource("bad");
    // std::shared_ptr<SharedResource> bad1(raw);
    // std::shared_ptr<SharedResource> bad2(raw);  // 危险！double-free
}

// ============================================================
// Part 3: weak_ptr - 打破循环引用
// ============================================================

/**
 * @brief weak_ptr使用示例
 *
 * 特点：
 * - 不增加引用计数
 * - 用于观察共享资源
 * - 打破shared_ptr循环引用
 */
class Node {
public:
    std::string name;
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> prev;  // 使用weak_ptr避免循环

    Node(const std::string& n) : name(n) {
        std::cout << "Node(" << name << ") created\n";
    }
    ~Node() {
        std::cout << "Node(" << name << ") destroyed\n";
    }
};

void demo_weak_ptr() {
    std::cout << "\n=== weak_ptr 示例 ===\n";

    auto node1 = std::make_shared<Node>("A");
    auto node2 = std::make_shared<Node>("B");

    // 构建双向链表
    node1->next = node2;
    node2->prev = node1;  // weak_ptr，不增加引用计数

    std::cout << "node1 use_count: " << node1.use_count() << "\n";
    std::cout << "node2 use_count: " << node2.use_count() << "\n";

    // 通过weak_ptr安全访问
    if (auto locked = node2->prev.lock()) {
        std::cout << "Previous node: " << locked->name << "\n";
    }

    // 检查资源是否已释放
    std::weak_ptr<Node> weak_obs = node1;
    std::cout << "Before reset, expired: " << weak_obs.expired() << "\n";
    node1.reset();
    std::cout << "After reset, expired: " << weak_obs.expired() << "\n";
}

// ============================================================
// Part 4: 右值引用与移动语义
// ============================================================

/**
 * @brief 移动语义示例
 */
class MoveableResource {
public:
    std::string name;
    std::vector<int> data;

    MoveableResource(const std::string& n) : name(n) {
        std::cout << "MoveableResource(" << name << ") constructed\n";
    }

    // 移动构造函数
    MoveableResource(MoveableResource&& other) noexcept
        : name(std::move(other.name)), data(std::move(other.data)) {
        std::cout << "MoveableResource moved\n";
        other.name = "(moved from)";
    }

    // 移动赋值运算符
    MoveableResource& operator=(MoveableResource&& other) noexcept {
        if (this != &other) {
            name = std::move(other.name);
            data = std::move(other.data);
            other.name = "(moved from)";
        }
        return *this;
    }
};

void demo_move_semantics() {
    std::cout << "\n=== 移动语义示例 ===\n";

    MoveableResource res1("Resource1");
    res1.data = {1, 2, 3, 4, 5};

    // 移动构造
    MoveableResource res2 = std::move(res1);
    std::cout << "res1.name after move: " << res1.name << "\n";
    std::cout << "res2.name: " << res2.name << "\n";

    // vector的移动语义
    std::vector<MoveableResource> vec;
    vec.reserve(2);
    vec.push_back(MoveableResource("Temp1"));  // 移动而非拷贝
    vec.emplace_back("Temp2");  // 直接在容器内构造

    // std::move本质是类型转换
    int x = 10;
    int&& rref = std::move(x);  // x的值不变，但可以被移动
    std::cout << "x: " << x << ", rref: " << rref << "\n";
}

// ============================================================
// Part 5: 完美转发
// ============================================================

/**
 * @brief 完美转发示例
 *
 * 使用std::forward保持值类别
 */

// 万能引用（Universal Reference）示例
template<typename T>
void show_type(T&& arg) {
    if constexpr (std::is_lvalue_reference_v<T>) {
        std::cout << "T is lvalue reference\n";
    } else {
        std::cout << "T is rvalue reference (or value type)\n";
    }
}

// 工厂函数示例 - 完美转发参数
class Widget {
public:
    std::string name;
    int value;

    Widget(const std::string& n, int v) : name(n), value(v) {
        std::cout << "Widget(" << name << ", " << value << ") constructed\n";
    }
};

template<typename T, typename U>
std::unique_ptr<Widget> make_widget(T&& name, U&& value) {
    return std::make_unique<Widget>(
        std::forward<T>(name),    // 完美转发
        std::forward<U>(value)
    );
}

void demo_perfect_forwarding() {
    std::cout << "\n=== 完美转发示例 ===\n";

    // 左值
    int x = 42;
    show_type(x);

    // 右值
    show_type(100);

    // 完美转发到工厂函数
    std::string name = "MyWidget";
    auto w1 = make_widget(name, 10);          // name作为左值转发
    auto w2 = make_widget("Another", 20);      // 字符串字面量作为右值转发
}

// ============================================================
// Part 6: RAII资源管理
// ============================================================

/**
 * @brief RAII资源管理示例
 */

class ScopedTimer {
public:
    ScopedTimer(const std::string& name)
        : name_(name), start_(std::chrono::high_resolution_clock::now()) {
        std::cout << "Timer '" << name_ << "' started\n";
    }

    ~ScopedTimer() {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start_);
        std::cout << "Timer '" << name_ << "' stopped: " << duration.count() << " us\n";
    }

    // 禁止拷贝
    ScopedTimer(const ScopedTimer&) = delete;
    ScopedTimer& operator=(const ScopedTimer&) = delete;

private:
    std::string name_;
    std::chrono::high_resolution_clock::time_point start_;
};

class LockGuard {
public:
    explicit LockGuard(std::mutex& m) : mutex_(m) {
        mutex_.lock();
        std::cout << "Lock acquired\n";
    }
    ~LockGuard() {
        mutex_.unlock();
        std::cout << "Lock released\n";
    }
    LockGuard(const LockGuard&) = delete;
    LockGuard& operator=(const LockGuard&) = delete;
private:
    std::mutex& mutex_;
};

void demo_raii() {
    std::cout << "\n=== RAII示例 ===\n";

    {
        ScopedTimer timer("TestBlock");
        // 模拟一些工作
        for (int i = 0; i < 100000; ++i) {
            volatile int x = i * i;
            (void)x;
        }
    }  // timer自动析构

    // 互斥锁RAII
    std::mutex mtx;
    {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "Critical section\n";
    }  // 自动释放锁
}

// ============================================================
// 主函数
// ============================================================

void run_week2_summary() {
    std::cout << "╔════════════════════════════════════════════╗\n";
    std::cout << "║    第二周知识点总结                          ║\n";
    std::cout << "╚════════════════════════════════════════════╝\n";

    demo_unique_ptr();
    demo_shared_ptr();
    demo_weak_ptr();
    demo_move_semantics();
    demo_perfect_forwarding();
    demo_raii();

    std::cout << "\n=== 本周核心要点 ===\n";
    std::cout << "1. 优先使用unique_ptr，只在需要共享时用shared_ptr\n";
    std::cout << "2. 使用make_unique/make_shared创建智能指针\n";
    std::cout << "3. 用weak_ptr打破shared_ptr循环引用\n";
    std::cout << "4. 理解移动语义和std::move\n";
    std::cout << "5. 使用std::forward实现完美转发\n";
    std::cout << "6. RAII是资源管理的基石\n";
}

} // namespace week2
