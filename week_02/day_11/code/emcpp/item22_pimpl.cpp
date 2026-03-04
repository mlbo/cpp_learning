/**
 * @file item22_pimpl.cpp
 * @brief EMC++ 条款22: 当使用Pimpl模式时，将特殊成员函数定义在实现文件中
 *
 * 条款要点：
 * 1. Pimpl模式通过降低类实现和类使用者之间的编译依赖来缩短编译时间
 * 2. 对于std::unique_ptr类型的pImpl指针，需要在类实现的文件中定义特殊成员函数
 * 3. 上述建议也适用于std::shared_ptr
 */

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <chrono>

// ========== 条款22 演示 ==========

namespace item22 {

// ============================================================================
// 错误示例：在头文件中使用=default
// ============================================================================

/**
 * @brief 错误示例 - 会导致编译错误
 *
 * 问题：unique_ptr的析构函数在销毁时会检查是否为完整类型
 * 在头文件中，Impl是不完整类型，会导致编译错误
 */
class WidgetBad {
public:
    WidgetBad();

    // 错误！在头文件中=default会导致编译错误
    // 因为此时Impl是不完整类型
    // ~WidgetBad() = default;  // 取消注释会报错

private:
    class Impl;
    std::unique_ptr<Impl> pImpl_;
};

// ============================================================================
// 正确示例：在.cpp中定义特殊成员函数
// ============================================================================

/**
 * @brief 正确示例 - Widget类声明
 *
 * 正确做法：
 * 1. 在头文件中声明析构函数
 * 2. 在实现文件中定义（或=default）
 */
class WidgetGood {
public:
    // 声明所有需要的特殊成员函数
    WidgetGood();
    explicit WidgetGood(const std::string& name);
    ~WidgetGood();  // 声明，不使用=default

    // 移动操作
    WidgetGood(WidgetGood&& rhs) noexcept;
    WidgetGood& operator=(WidgetGood&& rhs) noexcept;

    // 拷贝操作（需要自定义，因为unique_ptr不可拷贝）
    WidgetGood(const WidgetGood& rhs);
    WidgetGood& operator=(const WidgetGood& rhs);

    // 公共接口
    void setName(const std::string& name);
    std::string getName() const;
    void addData(int value);
    void doSomething();

private:
    class Impl;  // 前向声明
    std::unique_ptr<Impl> pImpl_;
};

// ============================================================================
// 实现类定义（通常在单独的.cpp或impl.h中）
// ============================================================================

class WidgetGood::Impl {
public:
    std::string name_;
    std::vector<int> data_;

    Impl() : name_("default") {
        std::cout << "[Impl] 构造" << std::endl;
    }

    ~Impl() {
        std::cout << "[Impl] 析构: " << name_ << std::endl;
    }

    void doSomething() {
        std::cout << "[Impl] 执行操作, 数据大小: " << data_.size() << std::endl;
    }
};

// ============================================================================
// 特殊成员函数定义（必须在Impl定义之后）
// ============================================================================

WidgetGood::WidgetGood() : pImpl_(std::make_unique<Impl>()) {
    std::cout << "[WidgetGood] 构造" << std::endl;
}

WidgetGood::WidgetGood(const std::string& name) : pImpl_(std::make_unique<Impl>()) {
    pImpl_->name_ = name;
    std::cout << "[WidgetGood] 构造(name)" << std::endl;
}

// 关键！必须在.cpp中定义，因为此时Impl是完整类型
WidgetGood::~WidgetGood() = default;

WidgetGood::WidgetGood(WidgetGood&& rhs) noexcept = default;
WidgetGood& WidgetGood::operator=(WidgetGood&& rhs) noexcept = default;

// 拷贝操作需要手动实现深拷贝
WidgetGood::WidgetGood(const WidgetGood& rhs)
    : pImpl_(std::make_unique<Impl>(*rhs.pImpl_)) {
    std::cout << "[WidgetGood] 拷贝构造" << std::endl;
}

WidgetGood& WidgetGood::operator=(const WidgetGood& rhs) {
    if (this != &rhs) {
        *pImpl_ = *rhs.pImpl_;
    }
    return *this;
}

// 接口实现
void WidgetGood::setName(const std::string& name) { pImpl_->name_ = name; }
std::string WidgetGood::getName() const { return pImpl_->name_; }
void WidgetGood::addData(int value) { pImpl_->data_.push_back(value); }
void WidgetGood::doSomething() { pImpl_->doSomething(); }

// ============================================================================
// shared_ptr vs unique_ptr 对比
// ============================================================================

/**
 * @brief 使用shared_ptr的Pimpl
 *
 * 使用shared_ptr时，析构函数可以在头文件中=default
 * 因为shared_ptr的删除器是在创建时确定的，不依赖完整类型
 */
class WidgetShared {
public:
    WidgetShared() : pImpl_(std::make_shared<Impl>()) {}

    // 可以在头文件中=default！
    // 因为shared_ptr不要求在声明点有完整类型
    ~WidgetShared() = default;

    void setName(const std::string& name) { pImpl_->name_ = name; }
    std::string getName() const { return pImpl_->name_; }

private:
    class Impl {
    public:
        std::string name_;
    };
    std::shared_ptr<Impl> pImpl_;  // 使用shared_ptr
};

// ============================================================================
// 跨DLL边界问题（跨so问题）
// ============================================================================

/**
 * @brief 跨模块安全的Pimpl
 *
 * 当类可能跨DLL边界使用时，需要确保：
 * 1. 使用相同的分配器
 * 2. 导出/导入宏正确配置
 * 3. 考虑使用shared_ptr或自定义删除器
 */
class WidgetDLLSafe {
public:
    // 显式导出（在Windows上）
    // __declspec(dllexport) 或 __attribute__((visibility("default")))

    WidgetDLLSafe();
    ~WidgetDLLSafe();

    // 使用shared_ptr更安全
    // 因为删除器在创建时确定，避免跨DLL的内存问题

private:
    class Impl;
    std::shared_ptr<Impl> pImpl_;  // 跨DLL边界推荐shared_ptr
};

// ============================================================================
// 性能考量
// ============================================================================

void demonstratePerformance() {
    std::cout << "\n=== 性能考量 ===\n" << std::endl;

    const int N = 1000000;

    // unique_ptr的开销
    {
        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < N; ++i) {
            WidgetGood w;
            w.addData(i);
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "unique_ptr: " << duration.count() << "ms" << std::endl;
    }

    // shared_ptr的开销
    {
        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < N; ++i) {
            WidgetShared w;
            // w.addData(i);  // WidgetShared没有此方法
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "shared_ptr: " << duration.count() << "ms (有控制块开销)" << std::endl;
    }

    std::cout << "\n结论：unique_ptr性能更优，shared_ptr有额外的控制块开销" << std::endl;
}

// ============================================================================
// 编译防火墙演示
// ============================================================================

void demonstrateCompilationFirewall() {
    std::cout << "\n=== 编译防火墙演示 ===\n" << std::endl;

    std::cout << "传统实现的问题：" << std::endl;
    std::cout << "1. 任何私有成员的修改都需要重新编译所有使用者" << std::endl;
    std::cout << "2. 私有成员依赖的头文件会传播给使用者" << std::endl;
    std::cout << "3. 头文件包含链导致编译时间增加" << std::endl;

    std::cout << "\nPimpl模式的优势：" << std::endl;
    std::cout << "1. 修改Impl类的成员不会影响使用者" << std::endl;
    std::cout << "2. 依赖的头文件只在.cpp中包含" << std::endl;
    std::cout << "3. 头文件更简洁，编译更快" << std::endl;

    std::cout << "\n示例：" << std::endl;
    std::cout << "// 修改前 Impl 中有 std::vector<int> data_;" << std::endl;
    std::cout << "// 修改后 Impl 中有 std::vector<double> data_;" << std::endl;
    std::cout << "// Pimpl模式：只需重新编译 widget.cpp" << std::endl;
    std::cout << "// 传统模式：需要重新编译所有包含widget.h的文件" << std::endl;
}

} // namespace item22

// ============================================================================
// 主函数
// ============================================================================

int main() {
    std::cout << "╔════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║    EMC++ 条款22: Pimpl模式的特殊成员函数                   ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════════════════╝" << std::endl;

    // 演示正确用法
    std::cout << "\n=== 正确使用Pimpl模式 ===\n" << std::endl;

    item22::WidgetGood w1("测试Widget");
    w1.addData(100);
    w1.addData(200);
    w1.doSomething();

    // 拷贝
    std::cout << "\n--- 拷贝操作 ---" << std::endl;
    item22::WidgetGood w2 = w1;
    std::cout << "w2名称: " << w2.getName() << std::endl;

    // 移动
    std::cout << "\n--- 移动操作 ---" << std::endl;
    item22::WidgetGood w3 = std::move(w1);
    std::cout << "w3名称: " << w3.getName() << std::endl;

    // 性能对比
    item22::demonstratePerformance();

    // 编译防火墙演示
    item22::demonstrateCompilationFirewall();

    std::cout << "\n╔════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║                    条款22演示完成                           ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════════════════╝" << std::endl;

    return 0;
}
