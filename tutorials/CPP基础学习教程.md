# C++ 基础学习教程

> 本教程面向C++初学者，从零开始系统讲解C++11核心特性，结合实战代码，帮助新手快速入门。

---

## 目录

1. [C++11 新特性概述](#1-c11-新特性概述)
2. [类型推导：auto 与 decltype](#2-类型推导auto-与-decltype)
3. [智能指针：现代内存管理](#3-智能指针现代内存管理)
4. [右值引用与移动语义](#4-右值引用与移动语义)
5. [Lambda 表达式](#5-lambda-表达式)
6. [统一初始化与 initializer_list](#6-统一初始化与-initializer_list)
7. [constexpr：编译期计算](#7-constexpr编译期计算)
8. [nullptr：空指针的新写法](#8-nullptr空指针的新写法)
9. [强类型枚举 enum class](#9-强类型枚举-enum-class)
10. [范围 for 循环](#10-范围-for-循环)
11. [标准容器增强](#11-标准容器增强)
12. [实战项目：动态数组类](#12-实战项目动态数组类)

---

## 1. C++11 新特性概述

### 1.1 为什么学习 C++11

C++11 是C++语言的重大更新，引入了许多现代编程特性。根据 cppreference 文档，C++11 标准于2011年发布，被视为"新C++"的起点。掌握C++11特性是现代C++开发的基础，也是面试的重点考察内容。

### 1.2 C++11 核心特性一览

```
C++11 主要特性分类:

┌─────────────────────────────────────────────────────────────┐
│                    C++11 核心特性                            │
├─────────────────────────────────────────────────────────────┤
│  类型系统        │ auto, decltype, nullptr, enum class      │
├─────────────────────────────────────────────────────────────┤
│  内存管理        │ unique_ptr, shared_ptr, weak_ptr         │
├─────────────────────────────────────────────────────────────┤
│  移动语义        │ 右值引用, std::move, std::forward         │
├─────────────────────────────────────────────────────────────┤
│  函数增强        │ lambda, std::function, std::bind         │
├─────────────────────────────────────────────────────────────┤
│  初始化增强      │ 统一初始化, initializer_list, constexpr  │
├─────────────────────────────────────────────────────────────┤
│  并发支持        │ thread, mutex, atomic, condition_variable│
├─────────────────────────────────────────────────────────────┤
│  容器增强        │ array, forward_list, unordered_map/set   │
└─────────────────────────────────────────────────────────────┘
```

### 1.3 编译器支持

主流编译器对C++11的支持情况：

| 编译器 | 最低版本 | 完整支持 |
|--------|----------|----------|
| GCC | 4.8.1 | GCC 5.0+ |
| Clang | 3.3 | Clang 3.5+ |
| MSVC | 2012 | MSVC 2015+ |

编译选项：`g++ -std=c++11 your_code.cpp`

---

## 2. 类型推导：auto 与 decltype

### 2.1 auto 关键字详解

`auto` 让编译器自动推导变量类型，简化代码书写，特别适用于类型名称冗长的场景。

#### 基本用法

```cpp
#include <iostream>
#include <vector>
#include <map>

int main() {
    // 基本类型推导
    auto i = 42;          // int
    auto d = 3.14;        // double
    auto s = "hello";     // const char*
    auto c = 'A';         // char

    // 复杂类型推导（auto最常用的场景）
    std::map<std::string, std::vector<int>> data;
    
    // 没有auto时的写法（非常繁琐）
    std::map<std::string, std::vector<int>>::iterator it1 = data.begin();
    
    // 使用auto（简洁清晰）
    auto it2 = data.begin();  // 类型自动推导

    // 范围for循环中auto的典型应用
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    for (auto num : numbers) {
        std::cout << num << " ";
    }
    
    return 0;
}
```

#### auto 的推导规则

```cpp
#include <iostream>

void autoRules() {
    int x = 10;
    int& ref = x;
    const int cx = x;
    
    // 规则1: auto会忽略引用
    auto a = ref;       // a 是 int，不是 int&
    
    // 规则2: auto会忽略顶层const
    auto b = cx;        // b 是 int，不是 const int
    
    // 规则3: 想保留引用和const，需要显式声明
    auto& c = ref;      // c 是 int&
    const auto& d = cx; // d 是 const int&
    
    // 规则4: 数组名会退化为指针
    int arr[5] = {1, 2, 3, 4, 5};
    auto e = arr;       // e 是 int*
    
    // 但使用引用可以保留数组类型
    auto& f = arr;      // f 是 int(&)[5]
}
```

#### auto 使用建议

```cpp
// ✅ 推荐使用的场景
auto iter = container.begin();           // 迭代器类型复杂
auto lambda = [](int x) { return x*2; }; // lambda类型难写
auto ptr = std::make_unique<int>(42);    // 智能指针

// ❌ 不推荐使用的场景
auto x = 0;       // 不如直接写 int x = 0; 清晰
auto y = func();  // 不明确返回类型，可读性差

// ✅ 显式写法更好的情况
int count = 0;           // 清晰表明意图
double ratio = 0.5;      // 数值类型最好显式声明
```

### 2.2 decltype 关键字详解

`decltype` 用于获取表达式的类型，不计算表达式的值。它在模板元编程和泛型编程中非常有用。

#### 基本用法

```cpp
#include <iostream>
#include <type_traits>

int main() {
    int x = 10;
    const int& cx = x;
    
    // decltype 保留完整的类型信息
    decltype(x) a = x;      // int
    decltype(cx) b = x;     // const int&
    decltype((x)) c = x;    // int& (注意：双层括号表示引用)
    
    // 用于声明返回类型
    auto add = [](int a, int b) -> decltype(a + b) {
        return a + b;
    };
    
    // 验证类型
    static_assert(std::is_same_v<decltype(a), int>);
    static_assert(std::is_same_v<decltype(b), const int&>);
    
    return 0;
}
```

#### decltype 与 auto 配合使用

```cpp
#include <vector>
#include <type_traits>

// 尾置返回类型（C++11）
template<typename Container>
auto getFirst(Container& c) -> decltype(c.front()) {
    return c.front();
}

// C++14 简化写法
template<typename Container>
decltype(auto) getFirstCpp14(Container& c) {
    return c.front();
}

int main() {
    std::vector<int> v = {1, 2, 3};
    
    // 返回类型自动推导为 int&
    auto& first = getFirst(v);
    first = 100;  // 修改vector的第一个元素
    
    return 0;
}
```

#### decltype(auto) 的妙用

```cpp
#include <iostream>

// 正确返回引用
decltype(auto) getValue(int& x) {
    return (x);  // 返回 int&
}

// 错误：返回值而非引用
auto getValueWrong(int& x) {
    return (x);  // 返回 int，丢失了引用
}

int main() {
    int value = 42;
    
    decltype(auto) ref = getValue(value);
    ref = 100;  // OK: 可以修改原值
    
    auto wrong = getValueWrong(value);
    wrong = 200;  // 只修改了副本
    
    std::cout << value << std::endl;  // 输出 100
    
    return 0;
}
```

---

## 3. 智能指针：现代内存管理

### 3.1 为什么需要智能指针

传统C++使用原始指针进行内存管理，容易导致内存泄漏、悬垂指针等问题。智能指针通过RAII（资源获取即初始化）机制自动管理内存生命周期。

```
传统指针的问题:

┌──────────────────────────────────────────────────────┐
│  int* ptr = new int(42);                             │
│  // ... 中间可能抛出异常                              │
│  delete ptr;  // 可能忘记，可能执行不到               │
└──────────────────────────────────────────────────────┘
                    ↓ 内存泄漏！

智能指针的解决方案:

┌──────────────────────────────────────────────────────┐
│  auto ptr = std::make_unique<int>(42);              │
│  // 自动管理，离开作用域自动释放                      │
└──────────────────────────────────────────────────────┘
                    ↓ 安全！
```

### 3.2 unique_ptr：独占所有权

`unique_ptr` 拥有它所指向对象的独占所有权，不可复制，只能移动。

```cpp
#include <iostream>
#include <memory>

class Resource {
public:
    Resource(int id) : id_(id) {
        std::cout << "Resource " << id_ << " created\n";
    }
    ~Resource() {
        std::cout << "Resource " << id_ << " destroyed\n";
    }
    void use() { std::cout << "Using resource " << id_ << "\n"; }
private:
    int id_;
};

void uniquePtrDemo() {
    // 创建 unique_ptr（推荐使用 make_unique）
    auto ptr1 = std::make_unique<Resource>(1);
    ptr1->use();
    
    // 移动所有权（允许）
    auto ptr2 = std::move(ptr1);
    // ptr1 现在为 nullptr
    if (!ptr1) {
        std::cout << "ptr1 is now empty\n";
    }
    ptr2->use();
    
    // 自定义删除器
    auto deleter = [](Resource* p) {
        std::cout << "Custom deleter called\n";
        delete p;
    };
    std::unique_ptr<Resource, decltype(deleter)> ptr3(
        new Resource(3), deleter
    );
    
}  // 离开作用域，自动释放

int main() {
    uniquePtrDemo();
    std::cout << "After uniquePtrDemo\n";
    return 0;
}
```

输出：
```
Resource 1 created
Using resource 1
ptr1 is now empty
Using resource 1
Resource 3 created
Custom deleter called
Resource 3 destroyed
Resource 1 destroyed
After uniquePtrDemo
```

### 3.3 shared_ptr：共享所有权

`shared_ptr` 使用引用计数管理共享资源，当引用计数归零时自动释放。

```cpp
#include <iostream>
#include <memory>

class SharedResource {
public:
    SharedResource(const std::string& name) : name_(name) {
        std::cout << name_ << " created\n";
    }
    ~SharedResource() {
        std::cout << name_ << " destroyed\n";
    }
private:
    std::string name_;
};

void sharedPtrDemo() {
    // 创建 shared_ptr
    auto ptr1 = std::make_shared<SharedResource>("Resource A");
    std::cout << "use_count: " << ptr1.use_count() << "\n";  // 1
    
    {
        auto ptr2 = ptr1;  // 共享所有权
        std::cout << "use_count: " << ptr1.use_count() << "\n";  // 2
        
        auto ptr3 = ptr1;
        std::cout << "use_count: " << ptr1.use_count() << "\n";  // 3
    }  // ptr2, ptr3 离开作用域
    
    std::cout << "use_count: " << ptr1.use_count() << "\n";  // 1
    
    // 循环引用问题演示
    struct Node {
        std::shared_ptr<Node> next;
        std::weak_ptr<Node> prev;  // 使用 weak_ptr 打破循环
    };
    
    auto node1 = std::make_shared<Node>();
    auto node2 = std::make_shared<Node>();
    
    node1->next = node2;
    node2->prev = node1;  // weak_ptr 不增加引用计数
    
}  // Resource A 在这里被销毁

int main() {
    sharedPtrDemo();
    std::cout << "After sharedPtrDemo\n";
    return 0;
}
```

### 3.4 weak_ptr：打破循环引用

`weak_ptr` 是一种不控制对象生命周期的智能指针，它指向一个由 `shared_ptr` 管理的对象。

```cpp
#include <iostream>
#include <memory>

class Observer {
public:
    void observe(std::weak_ptr<int> data) {
        if (auto locked = data.lock()) {
            std::cout << "Data value: " << *locked << "\n";
        } else {
            std::cout << "Data no longer exists\n";
        }
    }
};

void weakPtrDemo() {
    Observer obs;
    
    {
        auto shared = std::make_shared<int>(42);
        std::weak_ptr<int> weak = shared;
        
        std::cout << "use_count: " << shared.use_count() << "\n";  // 1
        obs.observe(weak);  // 输出: Data value: 42
    }
    
    // shared 已销毁
    std::weak_ptr<int> dangling;
    obs.observe(dangling);  // 输出: Data no longer exists
}

int main() {
    weakPtrDemo();
    return 0;
}
```

### 3.5 智能指针选择指南

```
智能指针选择决策树:

                    需要共享所有权吗？
                          │
            ┌─────────────┴─────────────┐
            │                           │
           否                          是
            │                           │
     使用 unique_ptr              使用 shared_ptr
            │                           │
            │                    可能有循环引用？
            │                           │
            │                 ┌─────────┴─────────┐
            │                 │                   │
            │                否                  是
            │                 │                   │
            │                 │            用 weak_ptr 打破
            │                 │
            │                 │
            ▼                 ▼
    ┌─────────────┐    ┌─────────────┐
    │ unique_ptr  │    │ shared_ptr  │
    │ +           │    │ + weak_ptr  │
    │ 最低开销    │    │ (如需要)    │
    └─────────────┘    └─────────────┘
```

---

## 4. 右值引用与移动语义

### 4.1 理解左值与右值

```cpp
#include <iostream>
#include <string>
#include <utility>

void lvalueRvalueDemo() {
    int x = 10;        // x 是左值，10 是右值
    int y = x + 5;     // y 是左值，x+5 是右值
    
    // 左值：有名字、有地址、可以取地址
    int* px = &x;      // OK: 左值可以取地址
    
    // 右值：没有名字、临时存在、不能取地址
    // int* ptemp = &(x + 5);  // 错误：右值不能取地址
    
    // 左值引用
    int& ref1 = x;           // OK: 左值引用绑定左值
    // int& ref2 = 10;       // 错误：左值引用不能绑定右值
    const int& ref3 = 10;    // OK: const左值引用可以绑定右值
    
    // 右值引用（C++11）
    int&& rref1 = 10;        // OK: 右值引用绑定右值
    int&& rref2 = x + 5;     // OK: 右值引用绑定临时值
    // int&& rref3 = x;      // 错误：右值引用不能绑定左值
    int&& rref3 = std::move(x);  // OK: std::move将左值转为右值
    
    std::string s1 = "Hello";
    std::string s2 = std::move(s1);  // 移动而非复制
    std::cout << "s1: '" << s1 << "'\n";  // s1 变为空
    std::cout << "s2: '" << s2 << "'\n";
}
```

### 4.2 移动构造函数与移动赋值运算符

```cpp
#include <iostream>
#include <algorithm>

class MyString {
public:
    // 默认构造
    MyString() : data_(nullptr), size_(0) {}
    
    // 构造函数
    MyString(const char* str) {
        size_ = strlen(str);
        data_ = new char[size_ + 1];
        std::copy(str, str + size_ + 1, data_);
        std::cout << "Constructed: " << data_ << "\n";
    }
    
    // 析构函数
    ~MyString() {
        std::cout << "Destructed: " << (data_ ? data_ : "null") << "\n";
        delete[] data_;
    }
    
    // 拷贝构造（深拷贝）
    MyString(const MyString& other) : size_(other.size_) {
        data_ = new char[size_ + 1];
        std::copy(other.data_, other.data_ + size_ + 1, data_);
        std::cout << "Copy constructed: " << data_ << "\n";
    }
    
    // 移动构造（资源转移）
    MyString(MyString&& other) noexcept 
        : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;
        other.size_ = 0;
        std::cout << "Move constructed: " << data_ << "\n";
    }
    
    // 拷贝赋值
    MyString& operator=(const MyString& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = new char[size_ + 1];
            std::copy(other.data_, other.data_ + size_ + 1, data_);
            std::cout << "Copy assigned: " << data_ << "\n";
        }
        return *this;
    }
    
    // 移动赋值
    MyString& operator=(MyString&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            size_ = other.size_;
            other.data_ = nullptr;
            other.size_ = 0;
            std::cout << "Move assigned: " << data_ << "\n";
        }
        return *this;
    }
    
    const char* c_str() const { return data_ ? data_ : ""; }
    
private:
    char* data_;
    size_t size_;
};

void moveDemo() {
    MyString s1 = "Hello";
    MyString s2 = s1;              // 拷贝构造
    MyString s3 = std::move(s1);   // 移动构造
    MyString s4;
    s4 = std::move(s2);            // 移动赋值
}

int main() {
    moveDemo();
    return 0;
}
```

输出：
```
Constructed: Hello
Copy constructed: Hello
Move constructed: Hello
Destructed: null
Move assigned: Hello
Destructed: 
Destructed: Hello
Destructed: null
```

### 4.3 std::move 与 std::forward

```cpp
#include <iostream>
#include <utility>
#include <string>

// std::move: 将左值转换为右值引用
void moveDemo() {
    std::string str = "Hello";
    std::string moved = std::move(str);  // str 被移动
    
    std::cout << "Original: '" << str << "'\n";   // 可能是空
    std::cout << "Moved: '" << moved << "'\n";    // "Hello"
}

// std::forward: 完美转发
template<typename T>
void wrapper(T&& arg) {
    // 完美转发：保持参数的值类别
    process(std::forward<T>(arg));
}

void process(int& x) {
    std::cout << "Lvalue: " << x << "\n";
}

void process(int&& x) {
    std::cout << "Rvalue: " << x << "\n";
}

void forwardDemo() {
    int x = 42;
    wrapper(x);          // 转发为左值
    wrapper(100);        // 转发为右值
}

int main() {
    moveDemo();
    forwardDemo();
    return 0;
}
```

---

## 5. Lambda 表达式

### 5.1 Lambda 语法详解

```
Lambda 语法结构:

[capture](parameters) mutable -> return_type { body }
   │         │         │           │            │
   │         │         │           │            └─ 函数体
   │         │         │           └─ 尾置返回类型（可选）
   │         │         └─ 可选：允许修改捕获的变量
   │         └─ 参数列表
   └─ 捕获列表
```

### 5.2 捕获方式

```cpp
#include <iostream>
#include <functional>

void captureDemo() {
    int a = 10;
    int b = 20;
    
    // 1. 值捕获
    auto byValue = [a]() {
        // a 是副本
        return a * 2;
    };
    a = 100;  // 不影响 lambda 中的 a
    std::cout << "By value: " << byValue() << "\n";  // 20
    
    // 2. 引用捕获
    auto byRef = [&a]() {
        return a * 2;  // a 是引用
    };
    std::cout << "By ref: " << byRef() << "\n";  // 200
    
    // 3. 隐式捕获
    auto captureAll = [=]() {  // 所有变量值捕获
        return a + b;
    };
    
    auto captureAllRef = [&]() {  // 所有变量引用捕获
        a = 200;
        return a + b;
    };
    
    // 4. 混合捕获
    auto mixed = [=, &a]() {  // a 引用捕获，其他值捕获
        a = b * 2;  // 可以修改 a
        return a + b;
    };
    
    // 5. 初始化捕获（C++14）
    auto initCapture = [x = a + b]() {
        return x;
    };
    
    // 6. 可变 lambda
    int counter = 0;
    auto mutableLambda = [counter]() mutable {
        return ++counter;  // 可以修改值捕获的副本
    };
    std::cout << mutableLambda() << "\n";  // 1
    std::cout << mutableLambda() << "\n";  // 2
    std::cout << "counter: " << counter << "\n";  // 0（未改变）
}

int main() {
    captureDemo();
    return 0;
}
```

### 5.3 Lambda 实战应用

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

void lambdaPractical() {
    std::vector<int> numbers = {5, 2, 8, 1, 9, 3};
    
    // 1. 排序
    std::sort(numbers.begin(), numbers.end(), 
        [](int a, int b) { return a > b; });  // 降序
    
    // 2. 查找
    auto it = std::find_if(numbers.begin(), numbers.end(),
        [](int x) { return x > 5; });
    
    // 3. 变换
    std::transform(numbers.begin(), numbers.end(), numbers.begin(),
        [](int x) { return x * 2; });
    
    // 4. 统计
    int threshold = 10;
    int count = std::count_if(numbers.begin(), numbers.end(),
        [threshold](int x) { return x > threshold; });
    
    // 5. 递归 lambda（C++14）
    std::function<int(int)> factorial = [&factorial](int n) {
        return n <= 1 ? 1 : n * factorial(n - 1);
    };
    std::cout << "5! = " << factorial(5) << "\n";
    
    // 6. 泛型 lambda（C++14）
    auto print = [](const auto& value) {
        std::cout << value << " ";
    };
    print(42);
    print(3.14);
    print("hello");
    std::cout << "\n";
}

int main() {
    lambdaPractical();
    return 0;
}
```

---

## 6. 统一初始化与 initializer_list

### 6.1 统一初始化语法

```cpp
#include <iostream>
#include <vector>
#include <map>
#include <string>

class Point {
public:
    int x, y;
    
    // 聚合类，无需构造函数即可使用统一初始化
};

void uniformInitDemo() {
    // 基本类型
    int a{42};           // 初始化为 42
    int b = {42};        // 等价写法
    double d{3.14};
    
    // 数组
    int arr[]{1, 2, 3, 4, 5};
    
    // 容器
    std::vector<int> vec{1, 2, 3, 4, 5};
    std::map<std::string, int> scores{
        {"Alice", 95},
        {"Bob", 87},
        {"Carol", 92}
    };
    
    // 自定义类型
    Point p{10, 20};
    std::cout << "Point: (" << p.x << ", " << p.y << ")\n";
    
    // 动态数组
    auto ptr = new int[5]{1, 2, 3, 4, 5};
    delete[] ptr;
    
    // 防止窄化转换
    // int narrow = {3.14};  // 编译警告或错误
}

int main() {
    uniformInitDemo();
    return 0;
}
```

### 6.2 initializer_list

```cpp
#include <iostream>
#include <initializer_list>
#include <vector>

class MyVector {
public:
    MyVector(std::initializer_list<int> init) {
        data_.reserve(init.size());
        for (auto val : init) {
            data_.push_back(val);
        }
        std::cout << "Initialized with " << init.size() << " elements\n";
    }
    
    void print() const {
        for (auto val : data_) {
            std::cout << val << " ";
        }
        std::cout << "\n";
    }
    
private:
    std::vector<int> data_;
};

void initializerListDemo() {
    MyVector v1{1, 2, 3};
    MyVector v2 = {4, 5, 6, 7, 8};
    
    v1.print();
    v2.print();
    
    // 使用 initializer_list 的函数
    auto sum = [](std::initializer_list<int> nums) {
        int total = 0;
        for (int n : nums) total += n;
        return total;
    };
    
    std::cout << "Sum: " << sum({1, 2, 3, 4, 5}) << "\n";
}

int main() {
    initializerListDemo();
    return 0;
}
```

---

## 7. constexpr：编译期计算

### 7.1 constexpr 变量

```cpp
#include <iostream>

constexpr int MAX_SIZE = 100;           // 编译期常量
constexpr double PI = 3.14159265359;    // 编译期常量

// 编译期计算数组大小
constexpr int arrSize = MAX_SIZE * 2;
int globalArr[arrSize];  // 编译期确定大小

void constExprVarDemo() {
    // constexpr 变量必须是编译期可计算的
    constexpr int local = 42;
    
    // 用于模板参数
    std::array<int, local> arr;  // OK
    
    // 用于 switch case
    switch (local) {
        case MAX_SIZE: break;  // OK
        case local: break;      // OK
    }
}
```

### 7.2 constexpr 函数

```cpp
#include <iostream>

// constexpr 函数：可在编译期执行
constexpr int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

constexpr int fibonacci(int n) {
    if (n <= 1) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

// C++14 支持更复杂的 constexpr 函数
constexpr int sum(int n) {
    int result = 0;
    for (int i = 1; i <= n; ++i) {  // C++14 允许循环
        result += i;
    }
    return result;
}

void constExprFuncDemo() {
    // 编译期计算
    constexpr int fact5 = factorial(5);   // 编译期计算为 120
    constexpr int fib10 = fibonacci(10);  // 编译期计算为 55
    
    // 运行时计算
    int n;
    std::cin >> n;
    int factN = factorial(n);  // 运行时计算
    
    // 用于编译期需求
    int arr[factorial(5)];  // 大小为 120 的数组
    std::array<int, factorial(5)> stdArr;
    
    std::cout << "5! = " << fact5 << "\n";
    std::cout << "fib(10) = " << fib10 << "\n";
    std::cout << "sum(100) = " << sum(100) << "\n";
}
```

### 7.3 constexpr 类

```cpp
#include <iostream>

class Complex {
public:
    constexpr Complex(double r = 0, double i = 0) 
        : re_(r), im_(i) {}
    
    constexpr double real() const { return re_; }
    constexpr double imag() const { return im_; }
    
    constexpr Complex operator+(const Complex& other) const {
        return Complex(re_ + other.re_, im_ + other.im_);
    }
    
private:
    double re_, im_;
};

void constExprClassDemo() {
    constexpr Complex c1(3.0, 4.0);
    constexpr Complex c2(1.0, 2.0);
    constexpr Complex c3 = c1 + c2;  // 编译期计算
    
    static_assert(c3.real() == 4.0, "Real part mismatch");
    static_assert(c3.imag() == 6.0, "Imaginary part mismatch");
    
    std::cout << "c3 = " << c3.real() << " + " << c3.imag() << "i\n";
}

int main() {
    constExprClassDemo();
    return 0;
}
```

---

## 8. nullptr：空指针的新写法

### 8.1 nullptr vs NULL

```cpp
#include <iostream>

// 函数重载
void func(int value) {
    std::cout << "func(int): " << value << "\n";
}

void func(int* ptr) {
    std::cout << "func(int*): " << (ptr ? "not null" : "null") << "\n";
}

void nullptrDemo() {
    // NULL 的问题
    // func(NULL);  // 歧义！NULL 是整数 0
    // 在某些编译器中会调用 func(int)，而非 func(int*)
    
    // nullptr 解决歧义
    func(nullptr);  // 明确调用 func(int*)
    
    // nullptr 的类型
    auto p = nullptr;  // 类型是 std::nullptr_t
    std::cout << "nullptr type: std::nullptr_t\n";
    
    // 可以隐式转换为任意指针类型
    int* pi = nullptr;
    double* pd = nullptr;
    void (*pf)() = nullptr;
    
    // 但不能转换为整数
    // int n = nullptr;  // 错误
    
    // 安全的指针检查
    if (pi == nullptr) {
        std::cout << "pi is null\n";
    }
    
    // 与 bool 的关系
    bool b1 = nullptr;  // OK: 转换为 false
    std::cout << "nullptr as bool: " << b1 << "\n";
}

int main() {
    nullptrDemo();
    return 0;
}
```

---

## 9. 强类型枚举 enum class

### 9.1 enum class vs 传统 enum

```cpp
#include <iostream>

// 传统枚举的问题
enum OldColor { Red, Green, Blue };
enum OldSize { Small, Medium, Large };

// enum class: 强类型枚举
enum class Color { Red, Green, Blue };
enum class Size { Small, Medium, Large };

// 可以指定底层类型
enum class Permissions : unsigned char {
    Read = 1,
    Write = 2,
    Execute = 4
};

void enumClassDemo() {
    // 传统枚举的问题
    int oldColor = Red;  // 隐式转换为 int
    // if (Red == Small)  // 编译通过！不同枚举值可以比较
    
    // enum class 的优势
    Color c = Color::Red;  // 必须使用作用域
    // Color c2 = Red;     // 错误：需要 Color::Red
    // int value = c;      // 错误：不能隐式转换
    
    // 显式转换
    int value = static_cast<int>(c);  // OK
    std::cout << "Color value: " << value << "\n";
    
    // 强类型比较
    // if (Color::Red == Size::Small)  // 错误：类型不匹配
    
    // 使用枚举类
    Size s = Size::Medium;
    switch (s) {
        case Size::Small:
            std::cout << "Small\n";
            break;
        case Size::Medium:
            std::cout << "Medium\n";
            break;
        case Size::Large:
            std::cout << "Large\n";
            break;
    }
    
    // 位运算组合
    Permissions p = static_cast<Permissions>(
        static_cast<int>(Permissions::Read) | 
        static_cast<int>(Permissions::Write)
    );
    std::cout << "Permissions: " << static_cast<int>(p) << "\n";
}

int main() {
    enumClassDemo();
    return 0;
}
```

---

## 10. 范围 for 循环

### 10.1 基本用法

```cpp
#include <iostream>
#include <vector>
#include <map>
#include <array>

void rangeForDemo() {
    // 数组
    int arr[] = {1, 2, 3, 4, 5};
    for (int x : arr) {
        std::cout << x << " ";
    }
    std::cout << "\n";
    
    // vector
    std::vector<std::string> names = {"Alice", "Bob", "Carol"};
    for (const auto& name : names) {  // 使用引用避免复制
        std::cout << name << " ";
    }
    std::cout << "\n";
    
    // map
    std::map<std::string, int> scores = {{"Alice", 95}, {"Bob", 87}};
    for (const auto& [name, score] : scores) {  // C++17 结构化绑定
        std::cout << name << ": " << score << "\n";
    }
    
    // 修改元素
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    for (int& n : numbers) {
        n *= 2;  // 原地修改
    }
    
    // 初始化列表
    for (int x : {10, 20, 30}) {
        std::cout << x << " ";
    }
    std::cout << "\n";
}

int main() {
    rangeForDemo();
    return 0;
}
```

---

## 11. 标准容器增强

### 11.1 新容器

```cpp
#include <iostream>
#include <array>
#include <forward_list>
#include <unordered_map>
#include <unordered_set>

void newContainersDemo() {
    // std::array: 固定大小数组
    std::array<int, 5> arr = {1, 2, 3, 4, 5};
    std::cout << "Front: " << arr.front() << ", Back: " << arr.back() << "\n";
    std::cout << "Size: " << arr.size() << "\n";
    
    // std::forward_list: 单向链表
    std::forward_list<int> flist = {1, 2, 3, 4, 5};
    flist.push_front(0);
    flist.pop_front();
    std::cout << "Forward list: ";
    for (int x : flist) std::cout << x << " ";
    std::cout << "\n";
    
    // std::unordered_map: 哈希表
    std::unordered_map<std::string, int> hashMap = {
        {"Alice", 95},
        {"Bob", 87}
    };
    hashMap["Carol"] = 92;
    
    // 平均 O(1) 查找
    auto it = hashMap.find("Bob");
    if (it != hashMap.end()) {
        std::cout << "Found Bob: " << it->second << "\n";
    }
    
    // std::unordered_set: 哈希集合
    std::unordered_set<int> hashSet = {1, 2, 3, 4, 5};
    hashSet.insert(3);  // 重复值不会插入
    std::cout << "Set size: " << hashSet.size() << "\n";
}

int main() {
    newContainersDemo();
    return 0;
}
```

### 11.2 容器新方法

```cpp
#include <iostream>
#include <vector>
#include <map>

void containerMethodsDemo() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    
    // cbegin/cend: 返回 const 迭代器
    for (auto it = vec.cbegin(); it != vec.cend(); ++it) {
        std::cout << *it << " ";
        // *it = 10;  // 错误：不能修改
    }
    std::cout << "\n";
    
    std::map<int, std::string> m = {{1, "one"}, {2, "two"}};
    
    // emplace: 原地构造
    m.emplace(3, "three");
    
    // emplace_hint: 带提示的位置插入
    auto hint = m.find(2);
    m.emplace_hint(hint, 4, "four");
    
    for (const auto& [k, v] : m) {
        std::cout << k << ": " << v << "\n";
    }
    
    // shrink_to_fit: 减少容量以适应大小
    std::vector<int> v = {1, 2, 3};
    v.reserve(100);
    std::cout << "Before shrink: capacity = " << v.capacity() << "\n";
    v.shrink_to_fit();
    std::cout << "After shrink: capacity = " << v.capacity() << "\n";
}

int main() {
    containerMethodsDemo();
    return 0;
}
```

---

## 12. 实战项目：动态数组类

### 12.1 完整实现

```cpp
#include <iostream>
#include <stdexcept>
#include <initializer_list>
#include <algorithm>

template<typename T>
class DynamicArray {
public:
    // 默认构造
    DynamicArray() : data_(nullptr), size_(0), capacity_(0) {}
    
    // 带大小构造
    explicit DynamicArray(size_t size) 
        : data_(new T[size]()), size_(size), capacity_(size) {}
    
    // 初始化列表构造
    DynamicArray(std::initializer_list<T> init) 
        : data_(new T[init.size()]), 
          size_(init.size()), 
          capacity_(init.size()) {
        std::copy(init.begin(), init.end(), data_);
    }
    
    // 拷贝构造
    DynamicArray(const DynamicArray& other) 
        : data_(new T[other.capacity_]),
          size_(other.size_),
          capacity_(other.capacity_) {
        std::copy(other.data_, other.data_ + size_, data_);
        std::cout << "Copy constructor\n";
    }
    
    // 移动构造
    DynamicArray(DynamicArray&& other) noexcept
        : data_(other.data_), 
          size_(other.size_), 
          capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
        std::cout << "Move constructor\n";
    }
    
    // 析构
    ~DynamicArray() {
        delete[] data_;
    }
    
    // 拷贝赋值
    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            data_ = new T[capacity_];
            std::copy(other.data_, other.data_ + size_, data_);
        }
        std::cout << "Copy assignment\n";
        return *this;
    }
    
    // 移动赋值
    DynamicArray& operator=(DynamicArray&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        std::cout << "Move assignment\n";
        return *this;
    }
    
    // 元素访问
    T& operator[](size_t index) { return data_[index]; }
    const T& operator[](size_t index) const { return data_[index]; }
    
    T& at(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return data_[index];
    }
    
    T& front() { return data_[0]; }
    T& back() { return data_[size_ - 1]; }
    T* data() { return data_; }
    
    // 容量
    size_t size() const { return size_; }
    size_t capacity() const { return capacity_; }
    bool empty() const { return size_ == 0; }
    
    // 修改
    void push_back(const T& value) {
        if (size_ >= capacity_) {
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        data_[size_++] = value;
    }
    
    void push_back(T&& value) {
        if (size_ >= capacity_) {
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        data_[size_++] = std::move(value);
    }
    
    template<typename... Args>
    void emplace_back(Args&&... args) {
        if (size_ >= capacity_) {
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        new(&data_[size_++]) T(std::forward<Args>(args)...);
    }
    
    void pop_back() {
        if (size_ > 0) --size_;
    }
    
    void reserve(size_t newCapacity) {
        if (newCapacity > capacity_) {
            T* newData = new T[newCapacity];
            std::move(data_, data_ + size_, newData);
            delete[] data_;
            data_ = newData;
            capacity_ = newCapacity;
        }
    }
    
    void clear() { size_ = 0; }
    
    // 迭代器
    T* begin() { return data_; }
    T* end() { return data_ + size_; }
    const T* begin() const { return data_; }
    const T* end() const { return data_ + size_; }
    
private:
    T* data_;
    size_t size_;
    size_t capacity_;
};

int main() {
    // 初始化列表构造
    DynamicArray<int> arr = {1, 2, 3, 4, 5};
    
    // 范围 for 循环
    std::cout << "Initial: ";
    for (const auto& x : arr) {
        std::cout << x << " ";
    }
    std::cout << "\n";
    
    // 添加元素
    arr.push_back(6);
    arr.push_back(7);
    
    std::cout << "After push_back: ";
    for (const auto& x : arr) {
        std::cout << x << " ";
    }
    std::cout << "\n";
    
    // 拷贝构造
    DynamicArray<int> arr2 = arr;
    
    // 移动构造
    DynamicArray<int> arr3 = std::move(arr);
    
    std::cout << "arr3 size: " << arr3.size() << "\n";
    std::cout << "arr size after move: " << arr.size() << "\n";
    
    return 0;
}
```

---

## 附录：参考资料

1. **cppreference C++11 特性**: https://en.cppreference.com/w/cpp/11.html
2. **C 语言中文网**: https://nj.gitbooks.io/c/content/
3. **Effective Modern C++ 中文版**: https://cntransgroup.github.io/EffectiveModernCppChinese/
4. **C++ 标准库文档**: https://en.cppreference.com/w/cpp

---

## 学习建议

1. **循序渐进**：从 auto 和智能指针开始，逐步掌握移动语义和 Lambda
2. **实践为主**：每学一个特性，都要写代码验证
3. **深入理解**：不仅要会用，还要理解背后的原理
4. **阅读源码**：看标准库如何使用这些特性
5. **项目实战**：在实际项目中应用所学知识

---

*本教程基于 C++11 标准，适合初学者由浅入深学习现代 C++ 编程。*
