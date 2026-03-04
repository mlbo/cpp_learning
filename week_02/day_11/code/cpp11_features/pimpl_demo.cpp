/**
 * @file pimpl_demo.cpp
 * @brief Pimpl模式演示程序
 *
 * 演示：
 * 1. Pimpl模式的基本用法
 * 2. 拷贝语义和移动语义
 * 3. 与传统实现的对比
 */

#include "pimpl_widget.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <cassert>

namespace cpp11_features {

// ========== 传统实现（对比用） ==========

/**
 * @brief 传统实现的Widget（不使用Pimpl）
 * 用于对比编译依赖
 */
class WidgetTraditional {
public:
    WidgetTraditional() : name_("traditional"), id_(0) {}

    void setName(const std::string& name) { name_ = name; }
    const std::string& getName() const { return name_; }

private:
    // 问题：任何私有成员的修改都会导致包含此头文件的代码重新编译
    std::string name_;
    int id_;
    std::vector<int> data_;              // 增加成员 -> 重新编译
    std::map<int, std::string> lookup_;  // 增加成员 -> 重新编译
};

} // namespace cpp11_features

// ========== 演示函数 ==========

void demonstrateBasicUsage() {
    std::cout << "\n=== 1. 基本用法演示 ===\n" << std::endl;

    // 创建Widget
    cpp11_features::Widget w1("测试Widget", 100);

    // 使用接口
    w1.addData(10);
    w1.addData(20);
    w1.addData(30);
    w1.setEntry(1, "Hello");
    w1.setEntry(2, "World");

    w1.printInfo();
    w1.doSomething();

    std::cout << "\n获取名称: " << w1.getName() << std::endl;
    std::cout << "获取ID: " << w1.getId() << std::endl;
    std::cout << "获取Entry(1): " << w1.getEntry(1) << std::endl;
}

void demonstrateCopySemantics() {
    std::cout << "\n=== 2. 拷贝语义演示 ===\n" << std::endl;

    cpp11_features::Widget original("原始", 1);
    original.addData(100);
    original.setEntry(10, "value");

    std::cout << "原始对象:" << std::endl;
    original.printInfo();

    // 拷贝构造
    std::cout << "\n--- 拷贝构造 ---" << std::endl;
    cpp11_features::Widget copied(original);
    std::cout << "拷贝后:" << std::endl;
    copied.printInfo();

    // 修改原始对象，验证深拷贝
    original.setName("修改后");
    original.setId(999);

    std::cout << "\n修改原始后:" << std::endl;
    std::cout << "原始: " << original.getName() << std::endl;
    std::cout << "拷贝: " << copied.getName() << std::endl;
    // copied应该保持原值

    // 拷贝赋值
    std::cout << "\n--- 拷贝赋值 ---" << std::endl;
    cpp11_features::Widget assigned;
    assigned = original;
    std::cout << "赋值后:" << std::endl;
    assigned.printInfo();
}

void demonstrateMoveSemantics() {
    std::cout << "\n=== 3. 移动语义演示 ===\n" << std::endl;

    cpp11_features::Widget source("移动源", 42);
    source.addData(1);
    source.addData(2);
    source.addData(3);

    std::cout << "移动前:" << std::endl;
    source.printInfo();

    // 移动构造
    std::cout << "\n--- 移动构造 ---" << std::endl;
    cpp11_features::Widget moved(std::move(source));

    std::cout << "移动后 - 新对象:" << std::endl;
    moved.printInfo();

    // 移动赋值
    std::cout << "\n--- 移动赋值 ---" << std::endl;
    cpp11_features::Widget target;
    target = std::move(moved);
    std::cout << "移动赋值后:" << std::endl;
    target.printInfo();
}

void demonstrateSwap() {
    std::cout << "\n=== 4. 交换操作演示 ===\n" << std::endl;

    cpp11_features::Widget a("WidgetA", 1);
    a.addData(100);

    cpp11_features::Widget b("WidgetB", 2);
    b.addData(200);
    b.addData(300);

    std::cout << "交换前:" << std::endl;
    std::cout << "A: "; a.printInfo();
    std::cout << "B: "; b.printInfo();

    // 交换
    a.swap(b);

    std::cout << "\n交换后:" << std::endl;
    std::cout << "A: "; a.printInfo();
    std::cout << "B: "; b.printInfo();
}

void demonstrateContainerUsage() {
    std::cout << "\n=== 5. 容器中使用 ===\n" << std::endl;

    std::vector<cpp11_features::Widget> widgets;

    std::cout << "--- 添加Widget到vector ---" << std::endl;
    widgets.emplace_back("First", 1);
    widgets.emplace_back("Second", 2);
    widgets.emplace_back("Third", 3);

    std::cout << "\n--- 遍历vector ---" << std::endl;
    for (const auto& w : widgets) {
        std::cout << "Widget: " << w.getName() << ", ID: " << w.getId() << std::endl;
    }

    std::cout << "\n--- 移动vector中的元素 ---" << std::endl;
    cpp11_features::Widget taken = std::move(widgets[0]);
    std::cout << "取出的Widget: " << taken.getName() << std::endl;
}

void demonstrateExceptionSafety() {
    std::cout << "\n=== 6. 异常安全演示 ===\n" << std::endl;

    try {
        cpp11_features::Widget w("异常测试", 0);

        // 模拟可能抛出异常的操作
        w.addData(1);
        w.setEntry(1, "test");

        // 使用RAII，即使抛出异常也会正确析构
        std::cout << "Widget创建成功" << std::endl;

        // 如果这里抛出异常，w会正确析构
        // throw std::runtime_error("模拟异常");

        std::cout << "操作完成" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "捕获异常: " << e.what() << std::endl;
    }

    std::cout << "Widget已正确析构" << std::endl;
}

// ========== 主函数 ==========

int runPimplDemo() {
    std::cout << "╔════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║          Pimpl模式（编译防火墙）演示程序                    ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════════════════╝" << std::endl;

    demonstrateBasicUsage();
    demonstrateCopySemantics();
    demonstrateMoveSemantics();
    demonstrateSwap();
    demonstrateContainerUsage();
    demonstrateExceptionSafety();

    std::cout << "\n╔════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║                   Pimpl模式演示完成                         ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════════════════╝" << std::endl;

    return 0;
}

} // namespace cpp11_features
