# Effective Modern C++ 教程

> 本教程基于 Scott Meyers 的《Effective Modern C++》，结合中文翻译版，提炼核心要点，帮助开发者掌握现代 C++ 最佳实践。

---

## 目录

1. [类型推导](#1-类型推导)
2. [auto 关键字](#2-auto-关键字)
3. [现代C++特性](#3-现代c特性)
4. [智能指针](#4-智能指针)
5. [右值引用与移动语义](#5-右值引用与移动语义)
6. [Lambda 表达式](#6-lambda-表达式)
7. [并发 API](#7-并发-api)
8. [性能优化](#8-性能优化)

---

## 1. 类型推导

### Item 1: 理解模板类型推导

模板类型推导是理解 `auto` 和 `decltype` 的基础。当调用函数模板时，编译器会根据传入参数推导模板参数类型。

```cpp
template<typename T>
void func(ParamType param);

func(expr);  // 编译器根据 expr 推导 T 和 ParamType
```

#### 情况1：ParamType 是指针或引用（非万能引用）

```cpp
template<typename T>
void func(T& param);  // ParamType 是 T&

void case1() {
    int x = 27;           // x 是 int
    const int cx = x;     // cx 是 const int
    const int& rx = x;    // rx 是 const int&
    
    func(x);   // T = int,       ParamType = int&
    func(cx);  // T = const int, ParamType = const int&
    func(rx);  // T = const int, ParamType = const int&
    // 注意：rx 的引用性被忽略，T 推导时不会保留引用
}
```

**推导规则**：
1. 如果 `expr` 是引用，忽略引用部分
2. 根据 `expr` 的类型匹配 `ParamType`，确定 `T`

#### 情况2：ParamType 是万能引用（Universal Reference）

```cpp
template<typename T>
void func(T&& param);  // 万能引用

void case2() {
    int x = 27;
    const int cx = x;
    
    func(x);   // x 是左值，T = int&,       ParamType = int&
    func(cx);  // cx 是左值，T = const int&, ParamType = const int&
    func(27);  // 27 是右值，T = int,       ParamType = int&&
    
    // 关键区别：
    // - 左值参数：T 推导为左值引用
    // - 右值参数：T 推导为非引用类型
}
```

#### 情况3：ParamType 既非指针也非引用（按值传递）

```cpp
template<typename T>
void func(T param);  // 按值传递

void case3() {
    int x = 27;
    const int cx = x;
    const int& rx = x;
    
    func(x);   // T = int, param = int
    func(cx);  // T = int, param = int  (const 被忽略)
    func(rx);  // T = int, param = int  (引用和 const 都被忽略)
    
    // 注意：param 是全新对象，顶层 const 和引用性都会被忽略
}
```

#### 数组参数的特殊处理

```cpp
template<typename T>
void func(T param);  // 按值传递

template<typename T>
void funcRef(T& param);  // 引用传递

void arrayCase() {
    const char name[] = "J. P. Briggs";
    
    func(name);     // T = const char*, param = const char*
                    // 数组退化为指针
    
    funcRef(name);  // T = const char[13], param = const char (&)[13]
                    // 引用传递保留数组类型
}

// 利用引用保留数组类型的特性，可以编写求数组大小的函数
template<typename T, size_t N>
constexpr size_t arraySize(T (&)[N]) {
    return N;
}
```

---

### Item 2: 理解 auto 类型推导

`auto` 类型推导与模板类型推导基本相同，但有一个例外：初始化列表。

```cpp
void autoDeduction() {
    auto x = 27;      // 情况3：按值传递
    auto& rx = x;     // 情况1：引用
    auto&& rr1 = x;   // 情况2：万能引用，x 是左值，rr1 是 int&
    auto&& rr2 = 27;  // 情况2：万能引用，27 是右值，rr2 是 int&&
    
    // 特殊情况：初始化列表
    auto a1 = {11};     // auto = std::initializer_list<int>
    auto a2 = {11, 22}; // auto = std::initializer_list<int>
    
    // 模板推导不支持初始化列表
    // template<typename T>
    // void f(T param);
    // f({11, 22});  // 错误！无法推导
}
```

#### auto 与模板推导的差异

```cpp
// auto 假设初始化列表为 std::initializer_list
auto x = {1, 2, 3};  // std::initializer_list<int>

// 函数模板不能自动推导初始化列表
template<typename T>
void func(T param);
// func({1, 2, 3});  // 编译错误

// 显式指定类型
void func2(std::initializer_list<int> param);
func2({1, 2, 3});  // OK
```

---

### Item 3: 理解 decltype

`decltype` 返回给定表达式或变量名的精确类型，包括引用和 const 限定符。

```cpp
void decltypeDemo() {
    int x = 0;
    const int cx = 0;
    
    decltype(x) a = x;    // int
    decltype(cx) b = cx;  // const int
    decltype((x)) c = x;  // int& (注意：双层括号返回引用)
    
    // decltype 在返回类型推导中的使用
    auto f = [](int x, int y) -> decltype(x + y) {
        return x + y;
    };
}
```

#### decltype(auto) 的使用

```cpp
// C++14 引入 decltype(auto)
// 结合 auto 的自动推导和 decltype 的精确类型保留

template<typename Container>
decltype(auto) getFirst(Container& c) {
    return c[0];  // 正确返回引用类型
}

void demo() {
    std::vector<int> v = {1, 2, 3};
    getFirst(v) = 100;  // OK: 返回 int&
}
```

---

### Item 4: 学会查看类型推导结果

#### 编译期诊断

```cpp
template<typename T>
class TypeDisplay;

void inspectTypes() {
    auto x = 27;
    // TypeDisplay<decltype(x)> t;  // 编译错误，显示 x 的类型
    
    // 使用 IDE 的类型提示或静态断言
    static_assert(std::is_same_v<decltype(x), int>);
}
```

#### 运行时输出

```cpp
#include <iostream>
#include <typeinfo>
#include <boost/type_index.hpp>

void printTypes() {
    const int x = 42;
    auto& rx = x;
    
    // typeid（不够精确，会忽略引用和 cv 限定符）
    std::cout << typeid(rx).name() << "\n";  // 可能输出 "int"
    
    // Boost.TypeIndex（推荐）
    using boost::typeindex::type_id_with_cvr;
    std::cout << type_id_with_cvr<decltype(rx)>().pretty_name() << "\n";
    // 输出 "int const&"
}
```

---

## 2. auto 关键字

### Item 5: 优先使用 auto 而非显式类型声明

```cpp
void autoBenefits() {
    // 避免类型不匹配
    std::vector<int> v = {1, 2, 3};
    
    // 错误：std::vector<int>::size_type vs unsigned
    // unsigned sz = v.size();  // 可能导致比较错误
    
    auto sz = v.size();  // 正确：std::vector<int>::size_type
    
    // 避免意外构造
    std::unordered_map<std::string, int> m;
    
    // 错误：循环变量类型不对
    // for (const std::pair<std::string, int>& p : m) {
    //     // std::pair<std::string, int> vs std::pair<const std::string, int>
    //     // 导致拷贝构造！
    // }
    
    // 正确
    for (const auto& p : m) {
        // 类型自动匹配为 const std::pair<const std::string, int>&
    }
}
```

#### auto 的优势总结

1. **避免未初始化变量**：`auto x;` 编译错误，必须初始化
2. **避免类型截断**：自动匹配正确类型
3. **减少代码冗余**：简化复杂类型声明
4. **方便重构**：修改变量类型时自动传播

---

### Item 6: auto 推导的意外情况

```cpp
void autoTraps() {
    std::vector<bool> features = {true, false, true};
    
    // 危险：vector<bool> 的代理引用
    // bool& ref = features[0];  // 错误：vector<bool> 不支持引用
    
    auto ref = features[0];  // auto 推导为 std::vector<bool>::reference
    // 这是一个代理对象，不是 bool！
    
    // 潜在问题
    bool* pb = &ref;  // 错误：不能取代理对象的地址
    
    // 解决方案：显式类型转换
    bool value = features[0];  // OK
    
    // 或者使用 auto 后显式转换
    auto value2 = static_cast<bool>(features[0]);
}
```

#### 隐式代理类型

```cpp
// 矩阵类的代理引用示例
class Matrix {
public:
    class RowProxy {
    public:
        double& operator[](size_t col);
    };
    RowProxy operator[](size_t row);
};

void matrixDemo() {
    Matrix m;
    auto row = m[0];  // row 是 RowProxy，不是 double*
    
    // 显式声明类型避免意外
    // double& elem = m[0][1];  // 明确期望 double&
}
```

---

## 3. 现代C++特性

### Item 7: 区分 () 和 {} 初始化语法

```cpp
void initSyntax() {
    // 传统初始化方式
    int x1 = 0;     // 拷贝初始化
    int x2(0);      // 直接初始化
    
    // C++11 统一初始化
    int x3{0};      // 列表初始化
    int x4 = {0};   // 拷贝列表初始化
    
    // 统一初始化的优势
    // 1. 可用于任何类型
    int arr[]{1, 2, 3};
    std::vector<int> v{1, 2, 3};
    
    // 2. 禁止窄化转换
    // int y = 3.14;   // 编译警告或接受
    // int z{3.14};    // 编译错误
    
    // 3. 免受最令人苦恼的解析困扰
    class Widget {
    public:
        Widget() {}
        Widget(int) {}
    };
    
    Widget w1();    // 函数声明！不是对象
    Widget w2{};    // 对象，调用默认构造
    
    // 统一初始化的陷阱
    std::vector<int> v1(10, 5);  // 10 个 5
    std::vector<int> v2{10, 5};  // {10, 5} 两个元素
}
```

#### initializer_list 的行为

```cpp
class Widget {
public:
    Widget() { std::cout << "Default\n"; }
    Widget(int) { std::cout << "Int\n"; }
    Widget(std::initializer_list<int>) { 
        std::cout << "Initializer list\n"; 
    }
};

void widgetDemo() {
    Widget w1;      // Default
    Widget w2(5);   // Int
    Widget w3{};    // Default（空初始化列表不匹配 initializer_list）
    Widget w4{5};   // Initializer list（优先匹配）
    Widget w5(5.0); // Int（窄化转换警告）
    // Widget w6{5.0}; // 编译错误（禁止窄化）
}
```

---

### Item 8: 优先使用 nullptr 而非 0 或 NULL

```cpp
void func(int);
void func(int*);

void nullptrDemo() {
    // 使用 0 或 NULL 的问题
    func(0);     // 调用 func(int)
    func(NULL);  // 调用 func(int)（NULL 是整数）
    
    // nullptr 解决歧义
    func(nullptr);  // 调用 func(int*)
    
    // nullptr 的类型安全
    auto p1 = 0;       // int
    auto p2 = NULL;    // int（通常）
    auto p3 = nullptr; // std::nullptr_t
    
    // 可以隐式转换为任意指针类型
    int* pi = nullptr;
    double* pd = nullptr;
    void (*pf)() = nullptr;
    
    // 模板中的优势
    template<typename T>
    void callFunc(T param) {
        f(param);  // 如果 T 是整数类型，调用错误的 f
    }
    
    callFunc(0);        // 传递 int
    callFunc(nullptr);  // 传递 std::nullptr_t，正确调用指针版本
}
```

---

### Item 9: 优先使用别名声明而非 typedef

```cpp
// typedef 的局限性
typedef void (*FuncPtr)(int, double);
typedef std::unique_ptr<std::unordered_map<std::string, std::vector<int>>> UPtrMap;

// using 别名声明（更清晰）
using FuncPtr = void (*)(int, double);
using UPtrMap = std::unique_ptr<std::unordered_map<std::string, std::vector<int>>>;

// 别名模板（typedef 不支持）
template<typename T>
using MyAllocList = std::list<T, MyAllocator<T>>;

// typedef 等价实现需要包装类
template<typename T>
struct MyAllocListHelper {
    typedef std::list<T, MyAllocator<T>> type;
};

void aliasDemo() {
    MyAllocList<int> list1;  // 使用别名模板
    
    // 在模板中使用
    template<typename T>
    class Widget {
        MyAllocList<T> list;  // OK
        
        // typedef 需要 typename
        typename MyAllocListHelper<T>::type list2;
    };
}
```

---

### Item 10: 优先使用限定作用域的枚举

```cpp
// 无作用域枚举的问题
enum Color { Red, Green, Blue };
enum Size { Small, Medium, Large };

void enumProblems() {
    // 污染命名空间
    // int x = Red;  // OK，隐式转换为 int
    
    // 可能的冲突
    // if (Red == Small) { }  // 编译通过！
}

// 限定作用域枚举（enum class）
enum class Color { Red, Green, Blue };
enum class Size { Small, Medium, Large };

void scopedEnumDemo() {
    Color c = Color::Red;  // 必须使用作用域
    // int x = c;          // 错误，不能隐式转换
    // if (Color::Red == Size::Small) { }  // 错误，类型不匹配
    
    // 显式转换
    int value = static_cast<int>(c);
    
    // 指定底层类型
    enum class Permissions : uint8_t {
        Read = 1,
        Write = 2,
        Execute = 4
    };
    
    // 前向声明（enum class 支持默认底层类型）
    enum class Status;  // OK
    // enum OldStatus;   // 错误，需要知道底层类型才能确定大小
}
```

---

## 4. 智能指针

### Item 18: 使用 std::unique_ptr 管理独占所有权

```cpp
class Investment {
public:
    virtual ~Investment() = default;
};

class Stock : public Investment {};
class Bond : public Investment {};

// 工厂函数返回 unique_ptr
template<typename T, typename... Args>
std::unique_ptr<Investment> makeInvestment(Args&&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

void uniquePtrDemo() {
    // 创建 unique_ptr
    auto p1 = std::make_unique<Stock>();
    
    // 自定义删除器
    auto delInvmt = [](Investment* p) {
        std::cout << "Deleting investment\n";
        delete p;
    };
    
    std::unique_ptr<Investment, decltype(delInvmt)> 
        p2(new Bond(), delInvmt);
    
    // 移动所有权
    auto p3 = std::move(p1);  // p1 变为 nullptr
    
    // 转换为 shared_ptr
    std::shared_ptr<Investment> sp = std::move(p3);
}
```

---

### Item 19: 使用 std::shared_ptr 管理共享所有权

```cpp
void sharedPtrDemo() {
    // 创建 shared_ptr
    auto sp1 = std::make_shared<Stock>();
    std::cout << "use_count: " << sp1.use_count() << "\n";  // 1
    
    // 共享所有权
    auto sp2 = sp1;
    std::cout << "use_count: " << sp1.use_count() << "\n";  // 2
    
    // 控制块结构
    // ┌──────────────────────────────────┐
    // │        Control Block             │
    // ├──────────────────────────────────┤
    // │  Reference count: 2              │
    // │  Weak count: 0                   │
    // │  Deleter: default                │
    // │  Allocator: default              │
    // └──────────────────────────────────┘
    
    // 自定义删除器（删除器类型不影响 shared_ptr 类型）
    auto deleter = [](Investment* p) {
        std::cout << "Custom delete\n";
        delete p;
    };
    
    std::shared_ptr<Investment> sp3(new Bond(), deleter);
    auto sp4 = sp3;  // 共享删除器
}
```

#### 避免循环引用

```cpp
struct Node {
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> prev;  // 使用 weak_ptr 打破循环
};

void cycleDemo() {
    auto n1 = std::make_shared<Node>();
    auto n2 = std::make_shared<Node>();
    
    n1->next = n2;
    n2->prev = n1;  // weak_ptr 不增加引用计数
    
    // 离开作用域时正确释放
}
```

---

### Item 20: 使用 std::weak_ptr 处理悬垂指针

```cpp
class CachedData {
public:
    std::shared_ptr<Data> getData(const std::string& key) {
        auto it = cache_.find(key);
        if (it != cache_.end()) {
            // 检查数据是否还存在
            if (auto sp = it->second.lock()) {
                return sp;  // 数据存在，返回 shared_ptr
            } else {
                cache_.erase(it);  // 数据已销毁，清理缓存
            }
        }
        
        // 加载数据
        auto data = loadData(key);
        cache_[key] = data;  // 存储 weak_ptr
        return data;
    }
    
private:
    std::unordered_map<std::string, std::weak_ptr<Data>> cache_;
};
```

---

### Item 21: 优先使用 std::make_unique 和 std::make_shared

```cpp
void makeFunctions() {
    // 直接使用 new 的问题
    // processWidget(std::shared_ptr<Widget>(new Widget()), 
    //               computePriority());
    // 问题：new Widget 和 computePriority() 的执行顺序不确定
    // 如果 computePriority() 抛出异常，可能内存泄漏
    
    // 使用 make_ 函数
    processWidget(std::make_shared<Widget>(), computePriority());
    // 安全：make_shared 是原子操作
    
    // make_unique (C++14)
    auto p1 = std::make_unique<Widget>();
    
    // make_shared
    auto p2 = std::make_shared<Widget>();
    
    // 优势：
    // 1. 异常安全
    // 2. 更高效（一次分配而非两次）
    // 3. 代码更简洁
    
    // 不能使用 make_ 的情况：
    // 1. 自定义删除器
    auto p3 = std::shared_ptr<Widget>(new Widget(), customDeleter);
    
    // 2. 大括号初始化
    // auto p4 = std::make_unique<Widget>{10, 20};  // 不支持
    auto p4 = std::unique_ptr<Widget>(new Widget{10, 20});
}
```

---

## 5. 右值引用与移动语义

### Item 23: 理解 std::move 和 std::forward

```cpp
// std::move：将左值转换为右值引用
// std::forward：条件转换（完美转发）

void moveForwardDemo() {
    std::string s = "Hello";
    
    // std::move 无条件转换
    std::string&& r1 = std::move(s);  // s 被转换为右值引用
    
    // std::forward 条件转换
    template<typename T>
    void wrapper(T&& arg) {
        // 如果 T 是左值引用，forward 返回左值引用
        // 如果 T 是非引用类型，forward 返回右值引用
        process(std::forward<T>(arg));
    }
    
    // 实现原理
    template<typename T>
    decltype(auto) myMove(T&& x) {
        return static_cast<std::remove_reference_t<T>&&>(x);
    }
    
    template<typename T>
    T&& myForward(std::remove_reference_t<T>& x) {
        return static_cast<T&&>(x);
    }
}
```

---

### Item 24: 区分万能引用与右值引用

```cpp
void universalVsRvalue() {
    // 右值引用
    void rvalueRef(int&& x);  // 只绑定右值
    
    // 万能引用（类型推导 + T&&）
    template<typename T>
    void universalRef(T&& x);  // 绑定左值和右值
    
    // 判断标准：
    // 1. 存在类型推导
    // 2. 形式为 T&& 或 auto&&
    
    // 万能引用示例
    auto&& r1 = 42;          // 右值，auto = int, r1 = int&&
    int x = 10;
    auto&& r2 = x;           // 左值，auto = int&, r2 = int&
    
    // 非万能引用示例
    template<typename T>
    class Widget {
        void process(T&& x);  // 不是万能引用！T 在类实例化时确定
    };
    
    std::vector<int> v;
    auto&& r3 = v[0];        // 万能引用，operator[] 返回左值引用
}
```

---

### Item 25: 对右值引用使用 std::move，对万能引用使用 std::forward

```cpp
class Widget {
public:
    Widget(Widget&& rhs) 
        : name_(std::move(rhs.name_)),
          data_(std::move(rhs.data_)) 
    {}
    
    // 万能引用构造函数
    template<typename T>
    void setName(T&& newName) {
        name_ = std::forward<T>(newName);
    }
    
    // 错误示例：对万能引用使用 std::move
    template<typename T>
    void setNameWrong(T&& newName) {
        name_ = std::move(newName);  // 左值也会被移走！
    }
    
private:
    std::string name_;
    std::vector<int> data_;
};

void demo() {
    Widget w;
    std::string s = "Widget Name";
    
    w.setName(s);            // s 保持有效（复制）
    w.setName(std::move(s)); // s 被移动
}
```

---

### Item 26: 避免在万能引用上重载

```cpp
// 问题代码
class Widget {
public:
    Widget() = default;
    
    void setName(const std::string& name) {  // 左值引用版本
        name_ = name;
    }
    
    template<typename T>
    void setName(T&& name) {  // 万能引用版本
        name_ = std::forward<T>(name);
    }
    
private:
    std::string name_;
};

void problem() {
    Widget w;
    std::string s = "Name";
    
    w.setName(s);             // 调用万能引用版本！（更精确匹配）
    w.setName("Literal");     // 调用万能引用版本
    w.setName(static_cast<const std::string&>(s));  // 强制调用左值版本
    
    // 问题：万能引用版本太"贪婪"
}
```

---

### Item 27: 熟悉万能引用重载的替代方案

```cpp
// 方案1：放弃重载
class Widget1 {
public:
    void setName(const std::string& name) { name_ = name; }
    // 不提供万能引用版本
};

// 方案2：const T& 参数
class Widget2 {
public:
    void setName(const std::string& name) { name_ = name; }
    // 对右值会多一次拷贝，但简单安全
};

// 方案3：值传递
class Widget3 {
public:
    void setName(std::string name) { name_ = std::move(name); }
    // 简洁，性能良好
};

// 方案4：标签分派
class Widget4 {
public:
    template<typename T>
    void setName(T&& name) {
        setNameImpl(std::forward<T>(name), 
                    std::is_lvalue_reference<T>());
    }
    
private:
    template<typename T>
    void setNameImpl(T&& name, std::true_type) {
        name_ = name;  // 左值：复制
    }
    
    template<typename T>
    void setNameImpl(T&& name, std::false_type) {
        name_ = std::move(name);  // 右值：移动
    }
};

// 方案5：SFINAE 约束
class Widget5 {
public:
    template<typename T, 
             typename = std::enable_if_t<!std::is_lvalue_reference_v<T>>>
    void setName(T&& name) {
        name_ = std::forward<T>(name);
    }
};
```

---

## 6. Lambda 表达式

### Item 31: 避免默认捕获模式

```cpp
class Widget {
public:
    void addFilter() {
        // 危险：默认引用捕获
        auto filter1 = [&]() {
            return divisor_ > 0;  // divisor_ 是引用
        };
        // 如果 Widget 对象销毁，filter1 持有悬垂引用！
        
        // 危险：默认值捕获
        auto filter2 = [=]() {
            return divisor_ > 0;  // 捕获的是 this 指针的副本
        };
        // divisor_ 通过 this 访问，仍然悬垂
        
        // 正确做法：显式捕获
        auto filter3 = [this]() {
            return divisor_ > 0;
        };
        
        // 更好：拷贝成员
        auto divisor = divisor_;
        auto filter4 = [divisor]() {
            return divisor > 0;
        };
        
        // C++14：初始化捕获
        auto filter5 = [divisor = divisor_]() {
            return divisor > 0;
        };
    }
    
private:
    int divisor_;
};
```

---

### Item 32: 使用初始化捕获将对象移入闭包

```cpp
void initCapture() {
    std::unique_ptr<Widget> pw = std::make_unique<Widget>();
    
    // C++11：移动捕获的实现
    auto func = std::bind([](const std::unique_ptr<Widget>& p) {
        // 使用 p
    }, std::move(pw));
    
    // C++14：初始化捕获
    auto func2 = [pw = std::move(pw)]() {
        // 使用 pw
    };
    
    // 捕获表达式结果
    auto func3 = [value = computeValue()]() {
        return value;
    };
}
```

---

### Item 33: 对 auto&& 参数使用 decltype

```cpp
// 泛型 lambda（C++14）
auto anyFunc = [](auto&& x) {
    return process(std::forward<decltype(x)>(x));
};

// 等价的模板
struct AnyFunc {
    template<typename T>
    auto operator()(T&& x) const {
        return process(std::forward<T>(x));
    }
};
```

---

### Item 34: 优先使用 lambda 而非 std::bind

```cpp
void lambdaVsBind() {
    // 使用 std::bind
    auto boundFunc = std::bind(&Widget::process, 
                               std::placeholders::_1, 
                               std::placeholders::_2,
                               42);
    
    // 使用 lambda（更清晰）
    auto lambdaFunc = [](Widget& w, int value) {
        w.process(value, 42);
    };
    
    // lambda 的优势：
    // 1. 代码更清晰
    // 2. 可读性更好
    // 3. 性能更优（内联更容易）
    // 4. 支持泛型（C++14）
}
```

---

## 7. 并发 API

### Item 35: 优先使用基于任务的编程

```cpp
// 基于线程的方式
int result;
std::thread t([&result]() {
    result = computeValue();
});
t.join();
std::cout << result << "\n";

// 基于任务的方式（推荐）
auto future = std::async(computeValue);
std::cout << future.get() << "\n";

// 基于任务的优势：
// 1. 自动异常处理
// 2. 简化代码
// 3. 避免线程管理
```

---

### Item 36: 指定 std::async 启动策略

```cpp
void asyncLaunch() {
    // 默认策略：可能异步，可能同步
    auto f1 = std::async(func);
    
    // 强制异步
    auto f2 = std::async(std::launch::async, func);
    
    // 强制同步（延迟执行）
    auto f3 = std::async(std::launch::deferred, func);
    
    // 在 get() 或 wait() 时执行
    f3.get();
}
```

---

### Item 37: 使 std::thread 在所有路径上不可 join

```cpp
class ThreadRAII {
public:
    ThreadRAII(std::thread&& t) : t_(std::move(t)) {}
    
    ~ThreadRAII() {
        if (t_.joinable()) {
            t_.join();  // 或 t_.detach()
        }
    }
    
    // 禁止拷贝
    ThreadRAII(const ThreadRAII&) = delete;
    ThreadRAII& operator=(const ThreadRAII&) = delete;
    
    // 允许移动
    ThreadRAII(ThreadRAII&&) = default;
    ThreadRAII& operator=(ThreadRAII&&) = default;
    
private:
    std::thread t_;
};
```

---

### Item 38: 关注线程句柄的析构行为

```cpp
void handleDestruction() {
    // std::thread: 析构时调用 std::terminate
    // std::future: 析构时可能阻塞
    
    // shared future 的特殊行为
    auto promise = std::promise<int>();
    auto future = promise.get_future().share();
    
    // 多个线程可以等待同一个 future
    std::thread t1([&future]() {
        future.wait();
    });
    // ...
}
```

---

## 8. 性能优化

### Item 41: 考虑使用可变参数模板进行类型安全转发

```cpp
// 类型安全的可变参数模板
template<typename T, typename... Args>
std::unique_ptr<T> make(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

// 对比 C 风格的可变参数（不安全）
void unsafePrintf(const char* fmt, ...);  // 类型不安全
```

---

## 参考资料

1. **Effective Modern C++ 中文版**: https://cntransgroup.github.io/EffectiveModernCppChinese/
2. **C++ 参考手册**: https://en.cppreference.com/
3. **C++ 核心指南**: https://isocpp.github.io/CppCoreGuidelines/

---

## 学习建议

### 阅读顺序

1. **第一阶段**：类型推导（Item 1-4）和 auto（Item 5-6）
2. **第二阶段**：智能指针（Item 18-22）
3. **第三阶段**：移动语义（Item 23-30）
4. **第四阶段**：Lambda 表达式（Item 31-34）
5. **第五阶段**：并发 API（Item 35-41）

### 实践要点

1. **边读边练**：每个条款都要写代码验证
2. **理解原理**：不仅知道"怎么做"，还要知道"为什么"
3. **代码审查**：检查现有代码是否遵循这些准则
4. **持续学习**：C++ 标准在演进，保持关注新特性

---

*本教程基于《Effective Modern C++》中文翻译版提炼，适合有一定 C++ 基础的开发者学习现代 C++ 最佳实践。*
