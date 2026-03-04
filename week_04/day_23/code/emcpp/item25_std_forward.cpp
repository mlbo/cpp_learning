/**
 * @file item25_std_forward.cpp
 * @brief EMC++ Item 25: 对右值引用使用 std::move，对通用引用使用 std::forward
 * 
 * 本文件演示：
 * 1. 何时使用 std::move
 * 2. 何时使用 std::forward
 * 3. 为什么不能混用
 * 4. 特殊情况：按值返回
 */

#include <iostream>
#include <utility>
#include <string>
#include <memory>

// ============================================================
// 辅助函数
// ============================================================

void processLValue(const std::string& s) {
    std::cout << "  [左值] " << s << std::endl;
}

void processRValue(std::string&& s) {
    std::cout << "  [右值] " << s << " (可移动)" << std::endl;
}

// ============================================================
// 正确使用 std::move（右值引用参数）
// ============================================================

class StringContainer {
public:
    // 右值引用参数 - 使用 std::move
    void setName(std::string&& name) {
        // 正确：name 是右值引用，可以安全移动
        name_ = std::move(name);
        std::cout << "  [setName&&] 设置名称: " << name_ << std::endl;
    }
    
    // 右值引用参数 - 使用 std::move
    void setData(std::unique_ptr<int[]>&& data, size_t size) {
        // 正确：unique_ptr 只能移动
        data_ = std::move(data);
        size_ = size;
        std::cout << "  [setData&&] 设置数据，大小: " << size_ << std::endl;
    }
    
    const std::string& name() const { return name_; }
    
private:
    std::string name_;
    std::unique_ptr<int[]> data_;
    size_t size_ = 0;
};

void demonstrateStdMoveOnRvalueRef() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "对右值引用使用 std::move" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    std::cout << R"(
当参数是右值引用时，使用 std::move：

原因：
1. 右值引用只绑定右值，我们确定它是临时对象
2. 可以安全地窃取其资源
3. 语义清晰：std::move 明确表示"我要移动它"

示例：
void setName(std::string&& name) {
    name_ = std::move(name);  // 正确且推荐
}
)" << std::endl;
    
    StringContainer container;
    
    std::cout << "\n调用方式：" << std::endl;
    container.setName(std::string("MyContainer"));
    container.setName("Literal");  // 字面量转换为临时对象
    
    // 错误示例
    // std::string name = "Test";
    // container.setName(name);  // 编译错误！右值引用不能绑定左值
}

// ============================================================
// 正确使用 std::forward（通用引用参数）
// ============================================================

class FlexibleContainer {
public:
    // 通用引用参数 - 使用 std::forward
    template<typename T>
    void setName(T&& name) {
        // 正确：使用 forward 保持原有值类别
        name_ = std::forward<T>(name);
        std::cout << "  [setName<T&&>] 设置名称: " << name_ << std::endl;
    }
    
    // 多个通用引用参数
    template<typename T1, typename T2>
    void setBoth(T1&& first, T2&& second) {
        first_ = std::forward<T1>(first);
        second_ = std::forward<T2>(second);
        std::cout << "  [setBoth] 设置: " << first_ << ", " << second_ << std::endl;
    }
    
private:
    std::string name_;
    std::string first_;
    std::string second_;
};

void demonstrateStdForwardOnUniversalRef() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "对通用引用使用 std::forward" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    std::cout << R"(
当参数是通用引用时，使用 std::forward：

原因：
1. 通用引用可绑定左值或右值
2. 我们不知道原始参数的值类别
3. std::forward 有条件地转换，保持原有类别

示例：
template<typename T>
void setName(T&& name) {
    name_ = std::forward<T>(name);  // 正确：完美转发
}
)" << std::endl;
    
    FlexibleContainer container;
    
    std::cout << "\n调用方式：" << std::endl;
    
    // 传入左值
    std::string leftName = "LeftValue";
    container.setName(leftName);
    std::cout << "  原字符串保持: " << leftName << std::endl;
    
    // 传入右值
    container.setName(std::string("RightValue"));
    
    // 传入字面量
    container.setName("Literal");
    
    // 混合调用
    std::string first = "First";
    container.setBoth(first, std::string("Second"));
    std::cout << "  first 保持: " << first << std::endl;
}

// ============================================================
// 为什么不能混用
// ============================================================

// 错误示例：在通用引用上使用 std::move
template<typename T>
void badForward(T&& param) {
    std::cout << "\n  [badForward] 错误使用 std::move" << std::endl;
    // 这会总是移动，即使传入的是左值！
    processLValue(std::move(param));  // 可能调用错误的函数
}

// 正确示例
template<typename T>
void goodForward(T&& param) {
    std::cout << "\n  [goodForward] 正确使用 std::forward" << std::endl;
    if constexpr (std::is_lvalue_reference_v<T>) {
        processLValue(std::forward<T>(param));
    } else {
        processRValue(std::forward<T>(param));
    }
}

void demonstrateWhyNotMix() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "为什么不能混用" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    std::cout << R"(
错误1：在通用引用上使用 std::move
----------------------------------
template<typename T>
void bad(T&& param) {
    target = std::move(param);  // 错误！总是移动
}

问题：
- std::move 无条件转换为右值引用
- 即使传入左值，也会被移动
- 导致意外的数据损坏

错误2：在右值引用上使用 std::forward
------------------------------------
void process(std::string&& s) {
    target = std::forward<std::string>(s);  // 可以工作，但不推荐
}

问题：
- 语义不清晰
- std::forward 通常用于通用引用
- 在右值引用上 std::move 更直接

正确做法：
----------
右值引用参数 -> std::move
通用引用参数 -> std::forward
)" << std::endl;
    
    std::cout << "\n【演示】\n" << std::endl;
    
    std::string value = "ImportantData";
    std::cout << "原始值: " << value << std::endl;
    
    goodForward(value);
    std::cout << "goodForward 后: " << value << " (保持不变)" << std::endl;
    
    // badForward 会移动左值！
    // badForward(value);  // 危险操作
}

// ============================================================
// 特殊情况：按值返回
// ============================================================

class Widget {
public:
    Widget(std::string name) : name_(std::move(name)) {}  // 按值传参，内部移动
    
    // 返回局部对象 - 不要用 std::move
    Widget createWidget() {
        Widget w("Local");
        return w;  // RVO 或隐式移动，不要用 std::move(w)
    }
    
    // 返回成员对象 - 可以用 std::move
    std::string&& getName() && {  // ref-qualified
        return std::move(name_);
    }
    
    // 返回通用引用参数
    template<typename T>
    T&& returnForward(T&& param) {
        return std::forward<T>(param);  // 完美转发返回
    }
    
private:
    std::string name_;
};

void demonstrateReturnValue() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "特殊情况：按值返回" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    std::cout << R"(
返回局部对象：
-------------
Widget createWidget() {
    Widget w("Local");
    return w;  // 正确：RVO 或隐式移动
}

错误：
return std::move(w);  // 阻止 RVO！

返回成员对象：
-------------
std::string&& getName() && {
    return std::move(name_);  // 正确：明确移动语义
}

返回通用引用参数：
-----------------
template<typename T>
T&& passThrough(T&& param) {
    return std::forward<T>(param);  // 正确：完美转发
}
)" << std::endl;
    
    Widget w1("Test");
    Widget w2 = w1.createWidget();
}

// ============================================================
// 综合示例
// ============================================================

class ModernWidget {
public:
    // 构造函数：按值传递 + 移动
    explicit ModernWidget(std::string name) 
        : name_(std::move(name)) {}  // 按值传参，内部移动
    
    // 通用引用设置器：完美转发
    template<typename T>
    void setName(T&& name) {
        name_ = std::forward<T>(name);
    }
    
    // 右值引用设置器：移动
    void setDescription(std::string&& desc) {
        description_ = std::move(desc);
    }
    
    // 常量引用设置器：拷贝
    void setTag(const std::string& tag) {
        tag_ = tag;
    }
    
    // 多参数完美转发
    template<typename T1, typename T2, typename T3>
    void setAll(T1&& name, T2&& desc, T3&& tag) {
        name_ = std::forward<T1>(name);
        description_ = std::forward<T2>(desc);
        tag_ = std::forward<T3>(tag);
    }
    
    void print() const {
        std::cout << "  Widget: name=" << name_ 
                  << ", desc=" << description_ 
                  << ", tag=" << tag_ << std::endl;
    }
    
private:
    std::string name_;
    std::string description_;
    std::string tag_;
};

void demonstrateCompleteExample() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "综合示例：正确使用 move 和 forward" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    ModernWidget w("Initial");
    w.print();
    
    // 通用引用：传入左值
    std::string name = "LeftName";
    w.setName(name);
    std::cout << "setName(左值) 后，原字符串: " << name << std::endl;
    
    // 通用引用：传入右值
    w.setName(std::string("RightName"));
    
    // 右值引用
    std::string desc = "Description";
    w.setDescription(std::move(desc));
    std::cout << "setDescription(std::move) 后，原字符串: \"" << desc << "\"" << std::endl;
    
    // 多参数完美转发
    std::string n = "MultiName";
    w.setAll(n, std::string("MultiDesc"), "MultiTag");
    w.print();
}

// ============================================================
// 最佳实践总结
// ============================================================

void printBestPracticesItem25() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "Item 25 最佳实践总结" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    std::cout << R"(
核心规则：
----------
1. 右值引用参数 -> 使用 std::move
2. 通用引用参数 -> 使用 std::forward

完整决策树：
------------
参数类型？
├─ 右值引用 (T&& 无类型推导)
│   └─ 使用 std::move
├─ 通用引用 (T&& 有类型推导)
│   └─ 使用 std::forward<T>
└─ 其他
    └─ 按值传递时可在函数内 std::move

特殊情况：
----------
1. 按值返回局部对象：
   return obj;  // 不要 std::move(obj)，让编译器优化

2. 按值传参的构造/赋值：
   Widget(std::string name) : name_(std::move(name)) {}

3. 多次使用同一参数：
   第一次后通常不能移动（除非使用 std::move）

代码审查检查点：
----------------
□ 右值引用参数是否使用了 std::move？
□ 通用引用参数是否使用了 std::forward？
□ 是否在通用引用上错误地使用了 std::move？
□ 返回局部对象时是否使用了不必要的 std::move？
)" << std::endl;
}

// ============================================================
// 入口函数
// ============================================================

/**
 * @brief Item 25 演示入口函数
 */
void run_item25_demo() {
    demonstrateStdMoveOnRvalueRef();
    demonstrateStdForwardOnUniversalRef();
    demonstrateWhyNotMix();
    demonstrateReturnValue();
    demonstrateCompleteExample();
    printBestPracticesItem25();
}
