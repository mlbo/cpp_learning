/**
 * @file item18_unique_ptr.cpp
 * @brief EMC++条款18：使用unique_ptr管理独占所有权的资源
 * @author C++ Tutorial
 * @date Day 8
 * 
 * 条款18核心内容：
 * 1. unique_ptr是开销最小的智能指针
 * 2. 它表达独占所有权语义
 * 3. 适用于工厂函数返回值、Pimpl惯用法等场景
 * 4. 可以轻松转换为shared_ptr
 */

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <chrono>
#include <fstream>

// 辅助打印
void printSeparator(const std::string& title = "") {
    std::cout << "\n======== " << title << " ========\n";
}

// ============================================================
// 条款18核心要点
// ============================================================

/**
 * @brief 条款18：使用unique_ptr进行独占所有权的资源管理
 * 
 * 理由：
 * 1. 零开销抽象 - 与原生指针同等性能
 * 2. 明确的所有权语义 - 独占所有权
 * 3. 异常安全 - RAII保证资源释放
 * 4. 可定制 - 支持自定义删除器
 * 
 * 适用场景：
 * - 工厂函数返回值
 * - Pimpl惯用法
 * - 管理独占资源
 */

// ============================================================
// 示例1：工厂函数返回unique_ptr
// ============================================================

// 投资类型层次结构
class Investment {
public:
    virtual ~Investment() = default;
    virtual std::string name() const = 0;
    virtual double value() const = 0;
};

class Stock : public Investment {
public:
    Stock(const std::string& symbol, double price, int shares)
        : symbol_(symbol), price_(price), shares_(shares) {}
    
    std::string name() const override { return "Stock: " + symbol_; }
    double value() const override { return price_ * shares_; }
    
private:
    std::string symbol_;
    double price_;
    int shares_;
};

class Bond : public Investment {
public:
    Bond(const std::string& issuer, double faceValue, double rate)
        : issuer_(issuer), faceValue_(faceValue), rate_(rate) {}
    
    std::string name() const override { return "Bond: " + issuer_; }
    double value() const override { return faceValue_ * (1 + rate_); }
    
private:
    std::string issuer_;
    double faceValue_;
    double rate_;
};

class RealEstate : public Investment {
public:
    RealEstate(const std::string& location, double area, double pricePerSqm)
        : location_(location), area_(area), pricePerSqm_(pricePerSqm) {}
    
    std::string name() const override { return "RealEstate: " + location_; }
    double value() const override { return area_ * pricePerSqm_; }
    
private:
    std::string location_;
    double area_;
    double pricePerSqm_;
};

// 投资类型枚举
enum class InvestmentType { Stock, Bond, RealEstate };

// 工厂函数 - 返回unique_ptr
std::unique_ptr<Investment> makeInvestment(InvestmentType type,
                                           const std::string& param1,
                                           double param2,
                                           double param3) {
    std::unique_ptr<Investment> pInv;
    
    switch (type) {
        case InvestmentType::Stock:
            pInv = std::make_unique<Stock>(param1, param2, static_cast<int>(param3));
            break;
        case InvestmentType::Bond:
            pInv = std::make_unique<Bond>(param1, param2, param3);
            break;
        case InvestmentType::RealEstate:
            pInv = std::make_unique<RealEstate>(param1, param2, param3);
            break;
    }
    
    return pInv;  // 编译器会优化为RVO或移动
}

void demoFactoryFunction() {
    printSeparator("示例1：工厂函数返回unique_ptr");
    
    std::cout << "\n工厂函数使用unique_ptr返回多态对象：\n\n";
    
    // 创建不同类型的投资
    auto investments = std::vector<std::unique_ptr<Investment>>();
    
    investments.push_back(makeInvestment(InvestmentType::Stock, "AAPL", 150.0, 100));
    investments.push_back(makeInvestment(InvestmentType::Bond, "US Treasury", 10000.0, 0.05));
    investments.push_back(makeInvestment(InvestmentType::RealEstate, "Downtown", 200.0, 5000.0));
    
    // 使用多态
    for (const auto& inv : investments) {
        std::cout << "  " << inv->name() << " -> 价值: $" << inv->value() << "\n";
    }
    
    std::cout << "\n优势：\n";
    std::cout << "  1. 工厂函数创建的对象所有权明确归属调用者\n";
    std::cout << "  2. 无需手动delete，内存自动管理\n";
    std::cout << "  3. 可以轻松转移到shared_ptr\n";
}

// ============================================================
// 示例2：Pimpl惯用法
// ============================================================

// Widget.h - 公共接口
class Widget {
public:
    explicit Widget(const std::string& name);
    ~Widget();
    
    Widget(Widget&&) noexcept;  // 移动构造
    Widget& operator=(Widget&&) noexcept;  // 移动赋值
    
    // 禁用拷贝（因为unique_ptr不可拷贝）
    Widget(const Widget&) = delete;
    Widget& operator=(const Widget&) = delete;
    
    void doSomething();
    std::string getName() const;
    int getCounter() const;
    
private:
    struct Impl;  // 前向声明
    std::unique_ptr<Impl> pImpl_;
};

// Widget.cpp - 实现细节
struct Widget::Impl {
    std::string name;
    std::vector<double> data;
    int counter;
    std::chrono::steady_clock::time_point created;
    
    Impl(const std::string& n) 
        : name(n), counter(0), 
          created(std::chrono::steady_clock::now()) {}
};

Widget::Widget(const std::string& name)
    : pImpl_(std::make_unique<Impl>(name)) {
    std::cout << "Widget '" << name << "' 创建\n";
}

Widget::~Widget() {
    if (pImpl_) {
        std::cout << "Widget '" << pImpl_->name << "' 销毁\n";
    }
}

Widget::Widget(Widget&&) noexcept = default;
Widget& Widget::operator=(Widget&&) noexcept = default;

void Widget::doSomething() {
    ++pImpl_->counter;
    pImpl_->data.push_back(static_cast<double>(pImpl_->counter));
}

std::string Widget::getName() const {
    return pImpl_->name;
}

int Widget::getCounter() const {
    return pImpl_->counter;
}

void demoPimpl() {
    printSeparator("示例2：Pimpl惯用法");
    
    std::cout << "\nPimpl（Pointer to Implementation）惯用法：\n\n";
    
    {
        Widget w("Pimpl演示");
        w.doSomething();
        w.doSomething();
        w.doSomething();
        
        std::cout << "  名称: " << w.getName() << "\n";
        std::cout << "  计数: " << w.getCounter() << "\n";
        
        // 移动演示
        Widget w2 = std::move(w);
        std::cout << "  移动后 w2.getName(): " << w2.getName() << "\n";
    }
    
    std::cout << "\nPimpl优势：\n";
    std::cout << "  1. 减少头文件编译依赖（Impl定义在cpp中）\n";
    std::cout << "  2. 稳定的ABI接口\n";
    std::cout << "  3. 更快的编译速度\n";
    std::cout << "  4. 实现细节对用户隐藏\n";
}

// ============================================================
// 示例3：自定义删除器
// ============================================================

// 自定义删除器用于文件句柄
auto fileDeleter = [](FILE* f) {
    if (f) {
        std::cout << "关闭文件\n";
        fclose(f);
    }
};

// 使用别名简化类型
using UniqueFile = std::unique_ptr<FILE, decltype(fileDeleter)>;

// 创建文件的辅助函数
UniqueFile openFile(const char* filename, const char* mode) {
    FILE* f = fopen(filename, mode);
    return UniqueFile(f, fileDeleter);
}

void demoCustomDeleter() {
    printSeparator("示例3：自定义删除器");
    
    std::cout << "\nunique_ptr支持自定义删除器，用于管理特殊资源：\n\n";
    
    // 管理文件句柄
    {
        auto file = openFile("/tmp/unique_ptr_test.txt", "w");
        if (file) {
            fprintf(file.get(), "Hello from unique_ptr!\n");
            std::cout << "  文件写入成功\n";
        }
        // 离开作用域时自动关闭文件
    }
    
    std::cout << "\n其他自定义删除器应用：\n";
    std::cout << "  - 网络套接字：自动close()\n";
    std::cout << "  - 数据库连接：自动disconnect()\n";
    std::cout << "  - 动态数组：delete[] 而非 delete\n";
    std::cout << "  - Windows句柄：CloseHandle()\n";
}

// ============================================================
// 示例4：转换为shared_ptr
// ============================================================

void demoConvertToShared() {
    printSeparator("示例4：转换为shared_ptr");
    
    std::cout << "\nunique_ptr可以轻松转换为shared_ptr：\n\n";
    
    // 创建unique_ptr
    std::unique_ptr<std::string> unique = std::make_unique<std::string>("Hello");
    std::cout << "  unique_ptr 值: " << *unique << "\n";
    
    // 转换为shared_ptr
    std::shared_ptr<std::string> shared = std::move(unique);
    std::cout << "  转换为 shared_ptr: " << *shared << "\n";
    std::cout << "  unique_ptr 是否为空: " << (unique ? "否" : "是") << "\n";
    
    std::cout << "\n工厂函数直接返回shared_ptr：\n";
    auto factoryReturnShared = []() -> std::shared_ptr<std::string> {
        return std::make_unique<std::string>("Factory Created");
        // unique_ptr隐式转换为shared_ptr
    };
    
    auto sp = factoryReturnShared();
    std::cout << "  值: " << *sp << "\n";
    
    std::cout << "\n注意：shared_ptr不能转换为unique_ptr！\n";
}

// ============================================================
// 示例5：性能对比
// ============================================================

void demoPerformance() {
    printSeparator("示例5：性能考量");
    
    std::cout << "\nunique_ptr的性能特性：\n";
    
    std::cout << R"(
┌───────────────────────────────────────────────────────────┐
│ 操作              │ 性能           │ 说明               │
├───────────────────────────────────────────────────────────┤
│ 构造              │ 与new相同      │ 内联后零开销       │
│ 析构              │ 与delete相同   │ 内联后零开销       │
│ operator*         │ 与原生指针相同 │ 直接解引用         │
│ operator->        │ 与原生指针相同 │ 直接访问成员       │
│ get()             │ 无开销         │ 直接返回内部指针   │
│ 移动操作          │ 指针赋值       │ 非常快             │
└───────────────────────────────────────────────────────────┘

与shared_ptr对比：
  - unique_ptr：单个指针大小（8字节 on 64-bit）
  - shared_ptr：两个指针大小（16字节）+ 控制块开销
  
结论：unique_ptr是"零开销抽象"的典范
)";
    
    // 简单性能测试
    const int N = 1000000;
    
    // 测试原生指针
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        int* p = new int(i);
        delete p;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto rawDuration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    // 测试unique_ptr
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        auto p = std::make_unique<int>(i);
    }
    end = std::chrono::high_resolution_clock::now();
    auto uniqueDuration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "\n简单性能测试（" << N << "次分配/释放）：\n";
    std::cout << "  原生指针:    " << rawDuration.count() << " ms\n";
    std::cout << "  unique_ptr:  " << uniqueDuration.count() << " ms\n";
    std::cout << "  （性能差异主要来自编译器优化级别）\n";
}

// ============================================================
// 示例6：最佳实践
// ============================================================

void demoBestPractices() {
    printSeparator("最佳实践");
    
    std::cout << R"(
┌─────────────────────────────────────────────────────────────────┐
│                    unique_ptr最佳实践                           │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│ 1. 优先使用make_unique创建                                      │
│    auto p = std::make_unique<T>(args...);                      │
│                                                                 │
│ 2. 工厂函数返回unique_ptr                                       │
│    std::unique_ptr<Base> factory() {                           │
│        return std::make_unique<Derived>();                     │
│    }                                                           │
│                                                                 │
│ 3. 函数参数传递                                                 │
│    - 转移所有权：std::unique_ptr<T>                             │
│    - 借用访问：const std::unique_ptr<T>& 或 T*                  │
│                                                                 │
│ 4. 使用Pimpl惯用法时                                            │
│    - 析构函数必须在cpp文件中定义                                 │
│    - 显式声明移动操作                                           │
│                                                                 │
│ 5. 避免的错误                                                   │
│    - 从同一原生指针创建多个unique_ptr                           │
│    - 使用get()返回的指针创建新unique_ptr                        │
│    - 忘记std::move导致拷贝失败                                  │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
)";
}

// ============================================================
// 主函数
// ============================================================

int main() {
    std::cout << "╔═════════════════════════════════════════════════════════╗\n";
    std::cout << "║   EMC++条款18：使用unique_ptr管理独占所有权资源         ║\n";
    std::cout << "╚═════════════════════════════════════════════════════════╝\n";
    
    demoFactoryFunction();
    demoPimpl();
    demoCustomDeleter();
    demoConvertToShared();
    demoPerformance();
    demoBestPractices();
    
    printSeparator("条款18总结");
    std::cout << "核心要点：\n";
    std::cout << "  1. unique_ptr是零开销的智能指针\n";
    std::cout << "  2. 表达独占所有权语义\n";
    std::cout << "  3. 适用于工厂函数返回值\n";
    std::cout << "  4. 完美支持Pimpl惯用法\n";
    std::cout << "  5. 支持自定义删除器\n";
    std::cout << "  6. 可无缝转换为shared_ptr\n";
    std::cout << "  7. 优先使用make_unique创建\n";
    
    return 0;
}
