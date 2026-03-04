/**
 * @file unique_ptr_advanced.cpp
 * @brief unique_ptr高级用法演示
 * @author C++ Tutorial
 * @date Day 8
 * 
 * 本文件演示unique_ptr的高级特性：
 * 1. 自定义删除器
 * 2. 管理数组
 * 3. Pimpl惯用法
 * 4. 工厂函数
 */

#include <iostream>
#include <memory>
#include <string>
#include <cstdio>
#include <fstream>
#include <functional>

// 辅助打印
void printSeparator(const std::string& title = "") {
    std::cout << "\n======== " << title << " ========\n";
}

// ============================================================
// 1. 自定义删除器
// ============================================================

/**
 * @brief 自定义删除器示例
 * 
 * unique_ptr支持自定义删除器，用于处理特殊的资源释放。
 * 语法：unique_ptr<T, DeleterType>
 */

// 方式1：函数指针作为删除器
void customDelete(int* p) {
    std::cout << "自定义删除器：删除 int*\n";
    delete p;
}

// 方式2：函数对象作为删除器
struct FileDeleter {
    void operator()(FILE* f) const {
        if (f) {
            std::cout << "关闭文件\n";
            fclose(f);
        }
    }
};

// 方式3：Lambda作为删除器
auto arrayDeleter = [](int* p) {
    std::cout << "数组删除器\n";
    delete[] p;
};

// 方式4：通用资源删除器
template<typename T>
struct LoggingDeleter {
    std::string name;
    
    explicit LoggingDeleter(const std::string& n) : name(n) {}
    
    void operator()(T* p) const {
        std::cout << "正在删除 " << name << " (地址: " << p << ")\n";
        delete p;
    }
};

void demoCustomDeleters() {
    printSeparator("自定义删除器");
    
    // 函数指针删除器
    std::cout << "\n【函数指针删除器】\n";
    std::unique_ptr<int, void(*)(int*)> p1(new int(42), customDelete);
    
    // 函数对象删除器
    std::cout << "\n【函数对象删除器】\n";
    std::unique_ptr<FILE, FileDeleter> file(fopen("/tmp/test.txt", "w"));
    if (file) {
        fprintf(file.get(), "Hello, unique_ptr!\n");
    }
    
    // Lambda删除器
    std::cout << "\n【Lambda删除器】\n";
    std::unique_ptr<int, decltype(arrayDeleter)> p2(new int[5]{1,2,3,4,5}, arrayDeleter);
    
    // 日志删除器
    std::cout << "\n【日志删除器】\n";
    std::unique_ptr<int, LoggingDeleter<int>> p3(
        new int(100),
        LoggingDeleter<int>("整数对象")
    );
    
    std::cout << "\n离开作用域，自动调用删除器:\n";
}

// ============================================================
// 2. 数组管理
// ============================================================

void demoArrayManagement() {
    printSeparator("数组管理");
    
    // 方式1：使用unique_ptr<T[]>（推荐）
    std::cout << "\n【unique_ptr<T[]>方式】\n";
    {
        std::unique_ptr<int[]> arr = std::make_unique<int[]>(5);
        for (int i = 0; i < 5; ++i) {
            arr[i] = i * 10;
        }
        
        std::cout << "数组内容: ";
        for (int i = 0; i < 5; ++i) {
            std::cout << arr[i] << " ";
        }
        std::cout << "\n";
        
        // 支持下标访问
        std::cout << "arr[2] = " << arr[2] << "\n";
    }
    
    // 方式2：使用自定义删除器（不推荐，但有时需要）
    std::cout << "\n【自定义删除器方式】\n";
    {
        auto deleter = [](int* p) { delete[] p; };
        std::unique_ptr<int, decltype(deleter)> arr2(new int[3]{1, 2, 3}, deleter);
        
        // 注意：这种方式不支持[]访问
        int* raw = arr2.get();
        std::cout << "数组内容: " << raw[0] << " " << raw[1] << " " << raw[2] << "\n";
    }
    
    std::cout << "\n注意：通常建议使用vector而不是unique_ptr<T[]>\n";
}

// ============================================================
// 3. Pimpl惯用法
// ============================================================

/**
 * @brief Pimpl惯用法示例
 * 
 * Pimpl (Pointer to Implementation) 是一种减少编译依赖的技术。
 * 将私有成员封装到Impl类中，头文件只需前向声明。
 */

// Widget.h 的内容
class WidgetPimpl {
public:
    explicit WidgetPimpl(const std::string& name);
    ~WidgetPimpl();
    
    void doSomething();
    std::string getName() const;
    
    // 禁用拷贝（unique_ptr不可拷贝）
    WidgetPimpl(const WidgetPimpl&) = delete;
    WidgetPimpl& operator=(const WidgetPimpl&) = delete;
    
    // 允许移动
    WidgetPimpl(WidgetPimpl&&) noexcept = default;
    WidgetPimpl& operator=(WidgetPimpl&&) noexcept = default;
    
private:
    struct Impl;  // 前向声明
    std::unique_ptr<Impl> pImpl_;
};

// Widget.cpp 的内容
struct WidgetPimpl::Impl {
    std::string name;
    std::vector<int> data;
    int counter;
    
    Impl(const std::string& n) : name(n), counter(0) {}
};

WidgetPimpl::WidgetPimpl(const std::string& name)
    : pImpl_(std::make_unique<Impl>(name)) {
    std::cout << "WidgetPimpl '" << name << "' 构造\n";
}

WidgetPimpl::~WidgetPimpl() {
    std::cout << "WidgetPimpl '" << pImpl_->name << "' 析构\n";
}

void WidgetPimpl::doSomething() {
    pImpl_->data.push_back(++pImpl_->counter);
    std::cout << "doSomething: counter = " << pImpl_->counter << "\n";
}

std::string WidgetPimpl::getName() const {
    return pImpl_->name;
}

void demoPimpl() {
    printSeparator("Pimpl惯用法");
    
    std::cout << "\nPimpl优势:\n";
    std::cout << "  1. 减少头文件编译依赖\n";
    std::cout << "  2. 稳定ABI接口\n";
    std::cout << "  3. 缩短编译时间\n\n";
    
    {
        WidgetPimpl w("Pimpl演示");
        w.doSomething();
        w.doSomething();
        std::cout << "名称: " << w.getName() << "\n";
    }
    
    std::cout << "\nWidgetPimpl已销毁\n";
}

// ============================================================
// 4. 工厂函数
// ============================================================

// 基类
class Shape {
public:
    virtual ~Shape() = default;
    virtual void draw() const = 0;
    virtual double area() const = 0;
};

// 派生类
class Circle : public Shape {
public:
    explicit Circle(double r) : radius_(r) {}
    
    void draw() const override {
        std::cout << "绘制圆形 (半径=" << radius_ << ")\n";
    }
    
    double area() const override {
        return 3.14159 * radius_ * radius_;
    }
    
private:
    double radius_;
};

class Rectangle : public Shape {
public:
    Rectangle(double w, double h) : width_(w), height_(h) {}
    
    void draw() const override {
        std::cout << "绘制矩形 (" << width_ << "x" << height_ << ")\n";
    }
    
    double area() const override {
        return width_ * height_;
    }
    
private:
    double width_;
    double height_;
};

// 工厂函数
enum class ShapeType { Circle, Rectangle };

std::unique_ptr<Shape> createShape(ShapeType type, double param1, double param2 = 0) {
    switch (type) {
        case ShapeType::Circle:
            return std::make_unique<Circle>(param1);
        case ShapeType::Rectangle:
            return std::make_unique<Rectangle>(param1, param2);
    }
    return nullptr;
}

void demoFactoryFunction() {
    printSeparator("工厂函数");
    
    std::cout << "\n使用工厂函数创建多态对象:\n";
    
    auto shapes = std::vector<std::unique_ptr<Shape>>();
    shapes.push_back(createShape(ShapeType::Circle, 5.0));
    shapes.push_back(createShape(ShapeType::Rectangle, 4.0, 3.0));
    shapes.push_back(createShape(ShapeType::Circle, 2.5));
    
    for (const auto& shape : shapes) {
        shape->draw();
        std::cout << "  面积: " << shape->area() << "\n";
    }
    
    std::cout << "\n工厂函数优势:\n";
    std::cout << "  1. 封装对象创建逻辑\n";
    std::cout << "  2. 返回类型为基类指针\n";
    std::cout << "  3. 自动内存管理\n";
}

// ============================================================
// 5. 异常安全
// ============================================================

void functionThatMightThrow(bool shouldThrow) {
    std::cout << "进入函数\n";
    
    // 使用unique_ptr保证异常安全
    auto resource = std::make_unique<int[]>(1000);
    std::cout << "分配资源\n";
    
    if (shouldThrow) {
        throw std::runtime_error("模拟异常");
    }
    
    std::cout << "正常完成\n";
    // 无论是否抛出异常，资源都会被正确释放
}

void demoExceptionSafety() {
    printSeparator("异常安全");
    
    std::cout << "\n【场景1：正常执行】\n";
    try {
        functionThatMightThrow(false);
    } catch (const std::exception& e) {
        std::cout << "捕获异常: " << e.what() << "\n";
    }
    
    std::cout << "\n【场景2：抛出异常】\n";
    try {
        functionThatMightThrow(true);
    } catch (const std::exception& e) {
        std::cout << "捕获异常: " << e.what() << "\n";
    }
    
    std::cout << "\nunique_ptr保证资源正确释放，无内存泄漏\n";
}

// ============================================================
// 6. 性能考量
// ============================================================

void demoPerformance() {
    printSeparator("性能考量");
    
    std::cout << "\nunique_ptr性能特性:\n";
    std::cout << "  ┌────────────────────────────────────────┐\n";
    std::cout << "  │ 特性              │ 性能影响         │\n";
    std::cout << "  ├────────────────────────────────────────┤\n";
    std::cout << "  │ 内存占用          │ 与原生指针相同   │\n";
    std::cout << "  │ 访问速度          │ 与原生指针相同   │\n";
    std::cout << "  │ 构造/析构         │ 内联后零开销     │\n";
    std::cout << "  │ 移动操作          │ 指针赋值         │\n";
    std::cout << "  │ 自定义删除器      │ 可能有额外开销   │\n";
    std::cout << "  └────────────────────────────────────────┘\n";
    
    std::cout << "\n最佳实践:\n";
    std::cout << "  1. 优先使用make_unique (异常安全+性能)\n";
    std::cout << "  2. 用unique_ptr替代原生指针\n";
    std::cout << "  3. 使用移动语义而非拷贝\n";
    std::cout << "  4. 避免不必要的get()调用\n";
}

// ============================================================
// 主函数
// ============================================================

int main() {
    std::cout << "╔═════════════════════════════════════════════════════════╗\n";
    std::cout << "║          unique_ptr高级用法演示                         ║\n";
    std::cout << "╚═════════════════════════════════════════════════════════╝\n";
    
    demoCustomDeleters();
    demoArrayManagement();
    demoPimpl();
    demoFactoryFunction();
    demoExceptionSafety();
    demoPerformance();
    
    printSeparator("总结");
    std::cout << "unique_ptr高级用法要点:\n";
    std::cout << "  1. 自定义删除器：处理特殊资源释放\n";
    std::cout << "  2. 数组管理：unique_ptr<T[]> 支持下标访问\n";
    std::cout << "  3. Pimpl惯用法：减少编译依赖\n";
    std::cout << "  4. 工厂函数：返回多态对象\n";
    std::cout << "  5. 异常安全：RAII保证资源释放\n";
    
    return 0;
}
