/**
 * @file main.cpp
 * @brief Day 11 主程序入口
 *
 * Day 11 主题:
 * - C++11特性：Pimpl模式（编译防火墙）
 * - EMC++条款：22（使用Pimpl模式）
 * - LeetCode：23题（合并K个升序链表）、61题（旋转链表）
 */

#include <iostream>
#include <string>
#include <functional>
#include <map>

// 声明外部演示函数
namespace cpp11_features {
    int runPimplDemo();
}

// ========== 主程序 ==========

void printBanner() {
    std::cout << R"(
╔════════════════════════════════════════════════════════════════════════╗
║                                                                        ║
║     ██████╗  █████╗ ██╗     ██╗                                        ║
║     ██╔══██╗██╔══██╗██║     ██║                                        ║
║     ██████╔╝███████║██║     ██║                                        ║
║     ██╔═══╝ ██╔══██║██║     ██║                                        ║
║     ██║     ██║  ██║███████╗██║                                        ║
║     ╚═╝     ╚═╝  ╚═╝╚══════╝╚═╝                                        ║
║                                                                        ║
║                    Day 11: Pimpl模式与链表专题                          ║
║                                                                        ║
║     主题:                                                              ║
║       • Pimpl模式（编译防火墙）                                         ║
║       • EMC++条款22：使用Pimpl模式                                      ║
║       • LeetCode 23：合并K个升序链表                                    ║
║       • LeetCode 61：旋转链表                                           ║
║                                                                        ║
╚════════════════════════════════════════════════════════════════════════╝
)" << std::endl;
}

void printMenu() {
    std::cout << "\n========== Day 11 学习菜单 ==========\n" << std::endl;
    std::cout << "  1. Pimpl模式演示" << std::endl;
    std::cout << "  2. EMC++条款22示例" << std::endl;
    std::cout << "  3. LeetCode 23: 合并K个升序链表" << std::endl;
    std::cout << "  4. LeetCode 61: 旋转链表" << std::endl;
    std::cout << "  5. 运行所有演示" << std::endl;
    std::cout << "  6. 今日知识点总结" << std::endl;
    std::cout << "  0. 退出" << std::endl;
    std::cout << "\n=====================================" << std::endl;
}

void showKnowledgeSummary() {
    std::cout << R"(
╔════════════════════════════════════════════════════════════════════════╗
║                      Day 11 知识点总结                                  ║
╚════════════════════════════════════════════════════════════════════════╝

【一、Pimpl模式（Pointer to Implementation）】

1. 核心思想：
   - 将类的实现细节从类定义中分离
   - 通过指针访问实现类
   - 形成编译防火墙

2. 主要优势：
   ┌──────────────────┬────────────────────────────────────┐
   │      优势        │              说明                   │
   ├──────────────────┼────────────────────────────────────┤
   │ 编译防火墙       │ 修改实现类无需重新编译使用者代码     │
   │ 减少头文件依赖   │ 实现依赖不会传播给使用者             │
   │ ABI兼容          │ 二进制接口更稳定，便于库升级         │
   │ 更快的编译速度   │ 头文件更简洁，包含更少               │
   └──────────────────┴────────────────────────────────────┘

3. 实现要点：
   - 使用 unique_ptr 管理Impl指针
   - 析构函数必须在.cpp中定义
   - 移动操作必须在.cpp中定义
   - 拷贝操作需要手动实现深拷贝

【二、EMC++条款22】

核心要点：
• 当使用Pimpl模式时，将特殊成员函数定义在实现文件中
• unique_ptr的析构需要完整类型，不能在头文件中=default
• shared_ptr不要求在声明点有完整类型，但性能略低

【三、LeetCode题目】

1. 合并K个升序链表（LeetCode 23）
   ┌──────────────┬────────────────┬─────────────────┐
   │     方法     │   时间复杂度    │    空间复杂度   │
   ├──────────────┼────────────────┼─────────────────┤
   │   分治合并   │   O(N log K)   │    O(log K)     │
   │   优先队列   │   O(N log K)   │    O(K)         │
   │   顺序合并   │   O(K²N)       │    O(1)         │
   └──────────────┴────────────────┴─────────────────┘

2. 旋转链表（LeetCode 61）
   • 关键技巧：成环后断开
   • 时间复杂度：O(N)
   • 空间复杂度：O(1)

【四、最佳实践】

1. Pimpl模式使用场景：
   ✓ 需要隐藏实现细节的库接口
   ✓ 需要减少编译依赖的大型项目
   ✓ 需要稳定ABI的跨模块接口

2. 注意事项：
   • 每个公共接口函数都需要转发调用
   • 增加了一层间接调用，可能有轻微性能开销
   • 需要仔细处理所有特殊成员函数

╔════════════════════════════════════════════════════════════════════════╗
║                      Day 11 学习完成！                                  ║
╚════════════════════════════════════════════════════════════════════════╝
)" << std::endl;
}

void showQuickRef() {
    std::cout << "\n=== Pimpl模式快速参考 ===\n" << std::endl;

    std::cout << "// widget.h" << std::endl;
    std::cout << "class Widget {" << std::endl;
    std::cout << "public:" << std::endl;
    std::cout << "    Widget();" << std::endl;
    std::cout << "    ~Widget();  // 必须声明" << std::endl;
    std::cout << "    Widget(Widget&&) noexcept;" << std::endl;
    std::cout << "    Widget& operator=(Widget&&) noexcept;" << std::endl;
    std::cout << "private:" << std::endl;
    std::cout << "    class Impl;  // 前向声明" << std::endl;
    std::cout << "    std::unique_ptr<Impl> pImpl_;" << std::endl;
    std::cout << "};" << std::endl;

    std::cout << "\n// widget.cpp" << std::endl;
    std::cout << "class Widget::Impl { /* 实现细节 */ };" << std::endl;
    std::cout << "Widget::~Widget() = default;  // 必须定义" << std::endl;
    std::cout << "Widget::Widget(Widget&&) noexcept = default;" << std::endl;

    std::cout << std::endl;
}

int main() {
    printBanner();

    std::map<int, std::function<void()>> actions = {
        {1, []() { cpp11_features::runPimplDemo(); }},
        {2, []() {
            std::cout << "\n请运行: ./build_and_run.sh item22" << std::endl;
            std::cout << "或直接运行: ./build/item22_pimpl" << std::endl;
        }},
        {3, []() {
            std::cout << "\n请运行: ./build_and_run.sh leetcode23" << std::endl;
            std::cout << "或直接运行: ./build/leetcode_23" << std::endl;
        }},
        {4, []() {
            std::cout << "\n请运行: ./build_and_run.sh leetcode61" << std::endl;
            std::cout << "或直接运行: ./build/leetcode_61" << std::endl;
        }},
        {5, []() {
            cpp11_features::runPimplDemo();
            std::cout << "\n其他演示请单独运行:" << std::endl;
            std::cout << "  ./build_and_run.sh item22" << std::endl;
            std::cout << "  ./build_and_run.sh leetcode23" << std::endl;
            std::cout << "  ./build_and_run.sh leetcode61" << std::endl;
        }},
        {6, []() {
            showKnowledgeSummary();
            showQuickRef();
        }}
    };

    while (true) {
        printMenu();

        std::cout << "请选择 (0-6): ";
        int choice;
        std::cin >> choice;

        if (choice == 0) {
            std::cout << "\n感谢学习Day 11内容！再见！" << std::endl;
            break;
        }

        auto it = actions.find(choice);
        if (it != actions.end()) {
            it->second();
        } else {
            std::cout << "无效选择，请重新输入。" << std::endl;
        }

        std::cout << "\n按Enter继续...";
        std::cin.ignore();
        std::cin.get();
    }

    return 0;
}
