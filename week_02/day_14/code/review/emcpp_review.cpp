/**
 * @file emcpp_review.cpp
 * @brief Effective Modern C++ 条款17-22 复习代码
 *
 * 本文件涵盖EMC++中关于特殊成员函数和智能指针的关键条款：
 * - Item 17: 理解特殊成员函数的生成规则
 * - Item 18: 使用unique_ptr管理独占资源
 * - Item 19: 使用shared_ptr管理共享资源
 * - Item 20: 使用weak_ptr表示可能悬空的shared_ptr-like指针
 * - Item 21: 优先使用make函数创建智能指针
 * - Item 22: 使用Pimpl模式减少编译依赖
 */

#include "review/emcpp_review.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <mutex>
#include <functional>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace emcpp {

template<typename Writer>
void writeLogNoexcept(Writer&& writer) noexcept {
    try {
        std::forward<Writer>(writer)();
    } catch (...) {
        // 日志失败不能逃出隐式noexcept析构路径。
    }
}

// ============================================================
// Item 17: 理解特殊成员函数的生成规则
// ============================================================

/**
 * @brief 特殊成员函数生成规则演示
 *
 * 关键规则：
 * - 只要声明了任意构造函数，编译器就不再自动生成默认构造函数。
 * - 声明移动构造或移动赋值，会使隐式拷贝操作被定义为deleted。
 * - 只有在没有用户声明的拷贝构造、拷贝赋值、移动操作和析构函数时，
 *   编译器才会隐式生成移动构造和移动赋值。
 * - 用户声明析构、拷贝构造或拷贝赋值都会阻止隐式移动操作生成。
 */

class SpecialMemberDemo {
public:
    std::string name;

    // 为了仍能默认构造，显式写出=default。
    SpecialMemberDemo() = default;

    // 一旦用户声明析构函数，编译器就不会再隐式生成移动操作。
    ~SpecialMemberDemo() {
        writeLogNoexcept([] { std::cout << "SpecialMemberDemo destructor\n"; });
    }

    // 明确保留拷贝；这两个声明也会阻止隐式移动生成。
    SpecialMemberDemo(const SpecialMemberDemo&) = default;
    SpecialMemberDemo& operator=(const SpecialMemberDemo&) = default;
};

class RuleOfFive {
public:
    std::unique_ptr<int[]> data;
    size_t size;

    explicit RuleOfFive(size_t n)
        : data(n == 0U ? nullptr : std::make_unique<int[]>(n)), size(n) {}

    // 五法则是检查清单，不是要求机械地手写五份代码。
    ~RuleOfFive() = default;  // unique_ptr自动管理

    RuleOfFive(const RuleOfFive& other) : RuleOfFive(other.size) {
        if (size != 0U) {
            std::copy_n(other.data.get(), size, data.get());
        }
    }

    RuleOfFive& operator=(const RuleOfFive& other) {
        if (this != &other) {
            RuleOfFive temp(other);
            swap(temp);
        }
        return *this;
    }

    RuleOfFive(RuleOfFive&& other) noexcept
        : data(std::move(other.data)), size(std::exchange(other.size, 0U)) {}

    RuleOfFive& operator=(RuleOfFive&& other) noexcept {
        if (this != &other) {
            data = std::move(other.data);
            size = std::exchange(other.size, 0U);
        }
        return *this;
    }

    void swap(RuleOfFive& other) noexcept {
        using std::swap;
        swap(data, other.data);
        swap(size, other.size);
    }
};

static_assert(std::is_nothrow_move_constructible_v<RuleOfFive>);
static_assert(std::is_nothrow_move_assignable_v<RuleOfFive>);

void demo_item17() {
    std::cout << "\n=== Item 17: 特殊成员函数生成规则 ===\n";

    SpecialMemberDemo obj1;
    obj1.name = "test";
    // std::move只把表达式转为右值；本类没有移动构造，因此这里退化为拷贝。
    SpecialMemberDemo obj2 = std::move(obj1);

    RuleOfFive arr1(10);
    RuleOfFive arr2 = arr1;      // 拷贝构造
    RuleOfFive arr3 = std::move(arr1);  // 移动构造
    if (arr1.data || arr1.size != 0U) {
        throw std::runtime_error("RuleOfFive移动后size与所有权必须同时清空");
    }
    RuleOfFive empty_copy = arr1;
    if (empty_copy.data || empty_copy.size != 0U || !arr3.data || arr3.size != 10U) {
        throw std::runtime_error("moved-from空状态必须仍可安全复制");
    }

    std::cout << "优先Rule of Zero；必须直接管理资源时，再逐项审视Rule of Five\n";
}

// ============================================================
// Item 18: 使用unique_ptr管理独占资源
// ============================================================

/**
 * @brief unique_ptr最佳实践
 */

// 1. 工厂函数返回unique_ptr
class Product {
public:
    virtual ~Product() = default;
    virtual void use() = 0;
};

class ConcreteProduct : public Product {
public:
    void use() override {
        std::cout << "Using ConcreteProduct\n";
    }
};

// 工厂函数 - 返回unique_ptr是最佳选择
std::unique_ptr<Product> createProduct(const std::string& type) {
    if (type == "concrete") {
        return std::make_unique<ConcreteProduct>();
    }
    return nullptr;
}

// 2. 自定义删除器
struct FileDeleter {
    void operator()(FILE* f) const noexcept {
        if (f) {
            fclose(f);
            writeLogNoexcept([] { std::cout << "Closing file via custom deleter\n"; });
        }
    }
};

using UniqueFile = std::unique_ptr<FILE, FileDeleter>;

UniqueFile openFile(const char* path, const char* mode) {
    FILE* f = fopen(path, mode);
    return UniqueFile(f);
}

// 3. Pimpl模式的unique_ptr用法（详见Item 22）

void demo_item18() {
    std::cout << "\n=== Item 18: unique_ptr管理独占资源 ===\n";

    // 工厂函数
    auto product = createProduct("concrete");
    if (product) {
        product->use();
    }

    // 自定义删除器
    {
        auto file = openFile("/dev/null", "w");
        if (file) {
            fprintf(file.get(), "test");
        }
    }  // 自动关闭

    std::cout << "要点:\n";
    std::cout << "- unique_ptr = 独占所有权\n";
    std::cout << "- 工厂函数首选返回类型\n";
    std::cout << "- 支持自定义删除器\n";
    std::cout << "- 可转换为shared_ptr\n";
}

// ============================================================
// Item 19: 使用shared_ptr管理共享资源
// ============================================================

/**
 * @brief shared_ptr使用陷阱和最佳实践
 */

class SharedDemo : public std::enable_shared_from_this<SharedDemo> {
public:
    std::string name;

    SharedDemo(const std::string& n) : name(n) {
        std::cout << "SharedDemo(" << name << ") created\n";
    }
    ~SharedDemo() {
        writeLogNoexcept([this] {
            std::cout << "SharedDemo(" << name << ") destroyed\n";
        });
    }

    // 安全获取自身的shared_ptr
    std::shared_ptr<SharedDemo> getShared() {
        return shared_from_this();
    }

    // 危险：在回调中保持自身
    void registerCallback(std::vector<std::function<void()>>& callbacks) {
        // 错误：捕获shared_from_this()可能导致生命周期问题
        // callbacks.push_back([self = shared_from_this()]() { ... });

        // 正确：使用weak_ptr
        callbacks.push_back([weak_self = std::weak_ptr<SharedDemo>(shared_from_this())]() {
            if (auto self = weak_self.lock()) {
                std::cout << "Callback for " << self->name << "\n";
            }
        });
    }
};

// 别名构造器（aliasing constructor）示例
struct Container {
    int value = 42;
};

void demo_item19() {
    std::cout << "\n=== Item 19: shared_ptr管理共享资源 ===\n";

    // 推荐使用make_shared
    auto sp1 = std::make_shared<SharedDemo>("Demo1");
    std::cout << "use_count: " << sp1.use_count() << "\n";

    // enable_shared_from_this
    auto sp2 = sp1->getShared();
    std::cout << "After shared_from_this, use_count: " << sp1.use_count() << "\n";

    // 别名构造器 - 共享所有权但指向不同对象
    auto container = std::make_shared<Container>();
    std::shared_ptr<int> alias(container, &container->value);
    std::cout << "alias value: " << *alias << "\n";
    std::cout << "container use_count: " << container.use_count() << "\n";

    // 陷阱演示
    std::cout << "\n常见陷阱:\n";
    std::cout << "- 不要从this创建shared_ptr（除非enable_shared_from_this）\n";
    std::cout << "- 不要从同一裸指针创建多个shared_ptr\n";
    std::cout << "- 注意循环引用\n";
    std::cout << "- 控制块计数可并发更新，不代表所指对象自动线程安全\n";
}

// ============================================================
// Item 20: 使用weak_ptr表示可能悬空的shared_ptr-like指针
// ============================================================

/**
 * @brief weak_ptr先观察控制块，再通过lock临时取得共享所有权
 */

// 有问题的设计：shared_ptr循环引用
class BadNode {
public:
    std::string name;
    std::shared_ptr<BadNode> next;
    std::shared_ptr<BadNode> prev;  // 危险：造成循环引用

    BadNode(const std::string& n) : name(n) {
        std::cout << "BadNode(" << name << ") created\n";
    }
    ~BadNode() {
        writeLogNoexcept([this] { std::cout << "BadNode(" << name << ") destroyed\n"; });
    }
};

// 正确的设计：使用weak_ptr打破循环
class GoodNode {
public:
    std::string name;
    std::shared_ptr<GoodNode> next;
    std::weak_ptr<GoodNode> prev;  // 使用weak_ptr

    GoodNode(const std::string& n) : name(n) {
        std::cout << "GoodNode(" << name << ") created\n";
    }
    ~GoodNode() {
        writeLogNoexcept([this] { std::cout << "GoodNode(" << name << ") destroyed\n"; });
    }
};

// 观察者模式示例
class Subject;

class Observer {
public:
    virtual ~Observer() = default;
    virtual void update() = 0;
};

class ConcreteObserver : public Observer {
public:
    std::weak_ptr<Subject> subject;  // weak_ptr观察

    void update() override {
        if (auto s = subject.lock()) {
            std::cout << "Observer received update from live subject\n";
        }
    }
};

class Subject : public std::enable_shared_from_this<Subject> {
    std::vector<std::weak_ptr<Observer>> observers_;
public:
    void attach(std::shared_ptr<Observer> obs) {
        observers_.push_back(obs);
    }

    void notify() {
        auto it = observers_.begin();
        while (it != observers_.end()) {
            if (auto obs = it->lock()) {
                obs->update();
                ++it;
            } else {
                it = observers_.erase(it);  // 清理已销毁的观察者
            }
        }
    }
};

void demo_item20() {
    std::cout << "\n=== Item 20: weak_ptr观察可能失效的共享对象 ===\n";

    // 正确的双向链表
    {
        auto node1 = std::make_shared<GoodNode>("A");
        auto node2 = std::make_shared<GoodNode>("B");
        node1->next = node2;
        node2->prev = node1;

        std::cout << "node1 use_count: " << node1.use_count() << "\n";
        std::cout << "node2 use_count: " << node2.use_count() << "\n";
    }  // 正确析构

    std::cout << "\nweak_ptr用途:\n";
    std::cout << "- 打破shared_ptr循环引用\n";
    std::cout << "- 实现观察者模式\n";
    std::cout << "- 缓存系统\n";
    std::cout << "- 使用lock在访问前确认对象仍然存活\n";
}

// ============================================================
// Item 21: 优先使用make函数
// ============================================================

class Widget {
public:
    Widget(int x, int y) : x_(x), y_(y) {
        std::cout << "Widget(" << x_ << ", " << y_ << ") created\n";
    }
    ~Widget() {
        writeLogNoexcept([this] {
            std::cout << "Widget(" << x_ << ", " << y_ << ") destroyed\n";
        });
    }
private:
    int x_, y_;
};

void demo_item21() {
    std::cout << "\n=== Item 21: 优先使用make函数 ===\n";

    // 推荐：make_unique / make_shared
    {
        auto up = std::make_unique<Widget>(1, 2);
        auto sp = std::make_shared<Widget>(3, 4);

        std::cout << "sp use_count: " << sp.use_count() << "\n";
    }

    // make的优势
    std::cout << "\nmake函数优势:\n";
    std::cout << "1. 异常安全和表达意图 - 不让裸new暴露在调用表达式中\n";
    std::cout << "2. 性能优化 - make_shared通常将对象与控制块合并为一次分配"
                 "（标准不保证绝对实现方式）\n";
    std::cout << "3. 代码简洁 - 无需重复类型名\n";

    // 不适用make的场景
    std::cout << "\n不适用make的场景:\n";
    std::cout << "1. 需要自定义删除器\n";
    std::cout << "2. 必须使用花括号初始化，而目标构造函数不匹配圆括号参数\n";
    std::cout << "3. make_shared的大对象仍有weak_ptr时，需权衡整块内存延迟释放\n";

    // 异常安全示例
    void process(std::shared_ptr<Widget>, int);
    // 在C++11/14中，第一种写法还存在参数求值交错导致泄漏的历史风险；
    // C++17加强了参数求值顺序，但make写法仍更短、更清楚。
    // process(std::shared_ptr<Widget>(new Widget(1, 2)), computeValue());
    // process(std::make_shared<Widget>(1, 2), computeValue());
}

// ============================================================
// Item 22: Pimpl模式减少编译依赖
// ============================================================

/**
 * @brief Pimpl（Pointer to Implementation）模式
 *
 * 好处：
 * 1. 减少头文件依赖
 * 2. 加快编译速度
 * 3. 有助于维持ABI稳定（并不自动保证所有ABI兼容性）
 *
 * 代价：通常多一次动态分配和一次间接访问；若要值语义，还需自行实现拷贝。
 */

// Widget.h 的内容
class PimplWidget {
public:
    PimplWidget();
    ~PimplWidget();

    // 需要定义移动操作（unique_ptr成员要求）
    PimplWidget(PimplWidget&&) noexcept;
    PimplWidget& operator=(PimplWidget&&) noexcept;

    // 禁止拷贝（或显式定义）
    PimplWidget(const PimplWidget&) = delete;
    PimplWidget& operator=(const PimplWidget&) = delete;

    void doSomething();
    int getValue() const;

private:
    class Impl;  // 前向声明
    std::unique_ptr<Impl> pImpl_;
};

// Widget.cpp 的内容
class PimplWidget::Impl {
public:
    std::string name;
    std::vector<int> data;
    int value;

    Impl() : name("default"), value(0) {
        data.reserve(100);
    }

    void doWork() {
        value++;
        data.push_back(value);
    }
};

PimplWidget::PimplWidget() : pImpl_(std::make_unique<Impl>()) {}
PimplWidget::~PimplWidget() = default;  // 必须在Impl定义之后

PimplWidget::PimplWidget(PimplWidget&&) noexcept = default;
PimplWidget& PimplWidget::operator=(PimplWidget&&) noexcept = default;

void PimplWidget::doSomething() { pImpl_->doWork(); }
int PimplWidget::getValue() const { return pImpl_->value; }

void demo_item22() {
    std::cout << "\n=== Item 22: Pimpl模式 ===\n";

    PimplWidget w;
    w.doSomething();
    std::cout << "Value: " << w.getValue() << "\n";

    PimplWidget w2 = std::move(w);
    w2.doSomething();

    std::cout << "\nPimpl模式要点:\n";
    std::cout << "1. 实现细节放入Impl类\n";
    std::cout << "2. 头文件只保留unique_ptr<Impl>\n";
    std::cout << "3. 析构函数必须在cpp中定义\n";
    std::cout << "4. 移动操作需要在cpp中定义\n";
    std::cout << "5. 公开接口出现的类型仍必须由头文件正确声明或包含\n";
    std::cout << "6. Pimpl以动态分配和间接访问换取依赖隔离\n";
}

// ============================================================
// 主函数
// ============================================================

void run_emcpp_review() {
    std::cout << "╔════════════════════════════════════════════╗\n";
    std::cout << "║   EMC++ 条款17-22 复习                      ║\n";
    std::cout << "╚════════════════════════════════════════════╝\n";

    demo_item17();
    demo_item18();
    demo_item19();
    demo_item20();
    demo_item21();
    demo_item22();

    std::cout << "\n=== EMC++ 核心建议 ===\n";
    std::cout << "Item 17: 理解特殊成员函数的生成规则\n";
    std::cout << "Item 18: 独占资源用unique_ptr\n";
    std::cout << "Item 19: 共享资源用shared_ptr\n";
    std::cout << "Item 20: 可能失效的共享观察关系用weak_ptr\n";
    std::cout << "Item 21: 创建智能指针用make函数\n";
    std::cout << "Item 22: 减少编译依赖用Pimpl\n";
}

} // namespace emcpp
