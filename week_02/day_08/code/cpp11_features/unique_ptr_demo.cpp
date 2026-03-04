/**
 * @file unique_ptr_demo.cpp
 * @brief unique_ptr基本用法演示
 * @author C++ Tutorial
 * @date Day 8
 * 
 * 本文件演示unique_ptr的核心特性：
 * 1. 独占所有权
 * 2. 基本创建方式
 * 3. 访问对象
 * 4. 所有权转移
 */

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// 辅助类用于演示
class Widget {
public:
    explicit Widget(const std::string& name) : name_(name) {
        std::cout << "Widget '" << name_ << "' 构造\n";
    }
    
    ~Widget() {
        std::cout << "Widget '" << name_ << "' 析构\n";
    }
    
    void sayHello() const {
        std::cout << "Hello from Widget '" << name_ << "'\n";
    }
    
    const std::string& name() const { return name_; }
    
private:
    std::string name_;
};

// 分隔线
void printSeparator(const std::string& title = "") {
    std::cout << "\n======== " << title << " ========\n";
}

// 演示unique_ptr的基本创建
void demoCreation() {
    printSeparator("unique_ptr的创建方式");
    
    // 方式1：直接构造（不推荐）
    std::unique_ptr<Widget> p1(new Widget("直接构造"));
    
    // 方式2：make_unique（推荐，C++14）
    auto p2 = std::make_unique<Widget>("make_unique");
    
    // 方式3：创建空unique_ptr
    std::unique_ptr<Widget> p3;
    std::unique_ptr<Widget> p4(nullptr);
    
    std::cout << "\n检查是否为空:\n";
    std::cout << "p1 是否为空: " << (p1 ? "否" : "是") << "\n";
    std::cout << "p3 是否为空: " << (p3 ? "否" : "是") << "\n";
    
    // 方式4：创建数组
    std::cout << "\n创建数组unique_ptr:\n";
    std::unique_ptr<int[]> arr = std::make_unique<int[]>(5);
    for (int i = 0; i < 5; ++i) {
        arr[i] = i * 10;
    }
    std::cout << "数组元素: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";
}

// 演示访问对象
void demoAccess() {
    printSeparator("访问被管理对象");
    
    auto widget = std::make_unique<Widget>("访问演示");
    
    // 方式1：解引用
    std::cout << "\n解引用访问:\n";
    widget->sayHello();
    std::cout << "名称: " << (*widget).name() << "\n";
    
    // 方式2：get()获取原生指针
    std::cout << "\nget()获取原生指针:\n";
    Widget* rawPtr = widget.get();
    std::cout << "原生指针地址: " << rawPtr << "\n";
    rawPtr->sayHello();
    
    // 重要：不要用原生指针创建新的unique_ptr！
    // std::unique_ptr<Widget> bad(rawPtr); // 危险！会导致重复删除
    
    std::cout << "\n注意：get()返回的指针不应用于创建新unique_ptr\n";
}

// 演示所有权转移
void demoOwnershipTransfer() {
    printSeparator("所有权转移（移动语义）");
    
    std::cout << "\n【unique_ptr不可拷贝】\n";
    auto p1 = std::make_unique<Widget>("原始主人");
    
    // 以下代码会编译错误：
    // std::unique_ptr<Widget> p2 = p1;  // 错误！拷贝构造被删除
    
    std::cout << "\n【unique_ptr可以移动】\n";
    std::unique_ptr<Widget> p2 = std::move(p1);
    
    std::cout << "转移后:\n";
    std::cout << "  p1 是否为空: " << (p1 ? "否" : "是") << "\n";
    std::cout << "  p2 是否为空: " << (p2 ? "否" : "是") << "\n";
    
    if (p2) {
        std::cout << "  p2 管理的对象: " << p2->name() << "\n";
    }
    
    std::cout << "\n【函数返回unique_ptr】\n";
    auto createWidget = [](const std::string& name) -> std::unique_ptr<Widget> {
        return std::make_unique<Widget>(name);  // 隐式移动或RVO
    };
    
    auto p3 = createWidget("函数创建");
    p3->sayHello();
}

// 演示函数参数传递
void processByValue(std::unique_ptr<Widget> w) {
    std::cout << "processByValue: " << w->name() << "\n";
    // 函数结束时，w被销毁，对象被delete
}

void processByRef(const std::unique_ptr<Widget>& w) {
    std::cout << "processByRef: " << w->name() << "\n";
    // 函数结束时，所有权不变
}

void processRawPtr(Widget* w) {
    if (w) {
        std::cout << "processRawPtr: " << w->name() << "\n";
    }
}

void demoFunctionParams() {
    printSeparator("函数参数传递");
    
    auto widget = std::make_unique<Widget>("参数演示");
    
    std::cout << "\n【方式1：按值传递】转移所有权\n";
    // processByValue(widget);  // 编译错误！不能拷贝
    processByValue(std::move(widget));
    std::cout << "传递后 widget 是否为空: " << (widget ? "否" : "是") << "\n";
    
    // 重新创建
    widget = std::make_unique<Widget>("参数演示2");
    
    std::cout << "\n【方式2：按const引用传递】借用，不转移所有权\n";
    processByRef(widget);
    std::cout << "传递后 widget 是否为空: " << (widget ? "否" : "是") << "\n";
    
    std::cout << "\n【方式3：传递原生指针】借用，不转移所有权\n";
    processRawPtr(widget.get());
    std::cout << "传递后 widget 是否为空: " << (widget ? "否" : "是") << "\n";
}

// 演示reset和release
void demoResetAndRelease() {
    printSeparator("reset() 和 release()");
    
    auto p = std::make_unique<Widget>("reset演示");
    std::cout << "\n初始状态:\n";
    std::cout << "  p 是否为空: " << (p ? "否" : "是") << "\n";
    
    // reset：释放当前对象，接管新对象
    std::cout << "\n【reset()操作】\n";
    p.reset(new Widget("新对象"));  // 旧对象被删除，接管新对象
    std::cout << "reset新对象后: " << p->name() << "\n";
    
    p.reset();  // 删除对象，变为空
    std::cout << "reset()后 p 是否为空: " << (p ? "否" : "是") << "\n";
    
    // release：放弃所有权，返回原生指针
    std::cout << "\n【release()操作】\n";
    p = std::make_unique<Widget>("release演示");
    Widget* raw = p.release();
    std::cout << "release后 p 是否为空: " << (p ? "否" : "是") << "\n";
    std::cout << "原生指针指向: " << raw->name() << "\n";
    
    // 需要手动管理释放
    delete raw;  // 必须手动删除
    std::cout << "手动删除完成\n";
}

// 演示比较操作
void demoComparison() {
    printSeparator("比较操作");
    
    auto p1 = std::make_unique<int>(42);
    auto p2 = std::make_unique<int>(42);
    std::unique_ptr<int> p3;
    
    std::cout << "\n与nullptr比较:\n";
    std::cout << "  p1 == nullptr: " << (p1 == nullptr ? "是" : "否") << "\n";
    std::cout << "  p3 == nullptr: " << (p3 == nullptr ? "是" : "否") << "\n";
    
    std::cout << "\n与bool转换:\n";
    std::cout << "  if(p1): " << (p1 ? "true" : "false") << "\n";
    std::cout << "  if(p3): " << (p3 ? "true" : "false") << "\n";
    
    std::cout << "\n注意：unique_ptr之间不能比较值是否相等\n";
    std::cout << "  *p1 == *p2: " << (*p1 == *p2 ? "是" : "否") << " (值比较)\n";
}

// 演示容器中的unique_ptr
void demoInContainer() {
    printSeparator("容器中的unique_ptr");
    
    std::cout << "\n创建vector<unique_ptr<Widget>>:\n";
    std::vector<std::unique_ptr<Widget>> widgets;
    
    widgets.push_back(std::make_unique<Widget>("容器1"));
    widgets.push_back(std::make_unique<Widget>("容器2"));
    widgets.push_back(std::make_unique<Widget>("容器3"));
    
    std::cout << "\n遍历容器:\n";
    for (const auto& w : widgets) {
        w->sayHello();
    }
    
    std::cout << "\n移出容器:\n";
    auto extracted = std::move(widgets[1]);
    extracted->sayHello();
    
    std::cout << "\nvector销毁时，剩余unique_ptr自动释放:\n";
}

// 主演示函数
int main() {
    std::cout << "╔═════════════════════════════════════════════════════════╗\n";
    std::cout << "║          unique_ptr基本用法演示                         ║\n";
    std::cout << "╚═════════════════════════════════════════════════════════╝\n";
    
    demoCreation();
    demoAccess();
    demoOwnershipTransfer();
    demoFunctionParams();
    demoResetAndRelease();
    demoComparison();
    demoInContainer();
    
    printSeparator("演示完成");
    std::cout << "unique_ptr核心要点:\n";
    std::cout << "  1. 独占所有权，同一时刻只能有一个unique_ptr指向对象\n";
    std::cout << "  2. 不可拷贝，只能移动\n";
    std::cout << "  3. 离开作用域自动释放对象\n";
    std::cout << "  4. 优先使用make_unique创建\n";
    std::cout << "  5. 传递给函数时注意所有权语义\n";
    
    return 0;
}
