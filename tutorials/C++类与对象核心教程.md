# C++ 类与对象核心教程

> 本教程补齐「经典 C++ 核心」这条线：类的对象模型、构造析构、拷贝语义、`static`/`const`/`friend`、继承、多态与虚函数、多态的代价与陷阱、强制类型转换、对象模型进阶。这些是大厂 C++ 面试出现频率最高、而现代 C++ 教程往往默认你已经懂的内容。
>
> 阅读前建议先看 [C++ 基础学习教程](CPP基础学习教程.md) 第 0 章和第 4 章；本教程在第 3 节直接复用那里的 `MyString` 和 `DynamicArray` 示例，把它们升华成「Rule of Three / Five」法则。设计模式教程里的 78 处 `override` 也以本教程第 6 节为前置依赖。

本篇统一使用 **C++17** 编译。每个代码块标明性质："完整可运行程序"可单独保存为 `.cpp` 并按 C++17 编译运行；"上下文片段"要放进已有上下文后验证；"反例片段"故意保留错误或危险写法，只用于阅读诊断或受控的 Sanitizer 观察，**不要照抄到生产代码**。

关于「实现相关」的标注：`sizeof`、内存对齐、虚表指针的大小、对象的内存布局都是**实现定义**的，本教程给出的数值默认在 **64 位 Linux、g++ 11、`-std=c++17`** 下成立，换编译器或平台可能不同。文中凡实现相关结论都会标注，并把它们与「标准保证的行为」区分开。

---

## 目录

1. [类的定义与对象模型](#1-类的定义与对象模型)
2. [构造与析构](#2-构造与析构)
3. [拷贝语义：Rule of Three 与 Rule of Five](#3-拷贝语义rule-of-three-与-rule-of-five)
4. [static、const 与 friend](#4-staticconst-与-friend)
5. [继承](#5-继承)
6. [多态与虚函数](#6-多态与虚函数)
7. [多态的代价与陷阱](#7-多态的代价与陷阱)
8. [强制类型转换](#8-强制类型转换)
9. [对象模型进阶](#9-对象模型进阶)

---

## 1. 类的定义与对象模型

类把数据和维护数据的操作组织在一起，并用访问控制对外隐藏实现细节。先看一个最小的类：

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <iostream>

class Point {
public:
    Point(int x, int y) : x_(x), y_(y) {}

    int x() const { return x_; }
    int y() const { return y_; }

    void offset(int dx, int dy) {
        x_ += dx;
        y_ += dy;
    }

private:
    int x_;
    int y_;
};

int main() {
    Point p(1, 2);
    p.offset(3, 4);
    std::cout << "x=" << p.x() << " y=" << p.y() << '\n';
}
```

输出（实现无关）：
```
x=4 y=6
```

### 1.1 访问控制

- `public`：使用者可以访问的接口。
- `private`：只有类自己（和友元）能访问的实现细节。`class` 默认是 `private`，`struct` 默认是 `public`——这是两者唯一的语法区别。
- `protected`：类自己和派生类能访问，外部不能。第 5 节讲继承时再展开。

访问控制是**编译期**约束，不带来运行期开销，也不影响对象布局（同一个访问段内的成员按声明顺序排列）。

### 1.2 this 指针

每个非静态成员函数都隐含一个指向当前对象的 `this` 指针。`x_` 在成员函数里等价于 `this->x_`。返回 `*this` 可以实现链式调用：

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <iostream>

class Box {
public:
    explicit Box(int v) : v_(v) {}

    Box& add(int x) { v_ += x; return *this; }
    void print() const { std::cout << "v=" << v_ << '\n'; }

private:
    int v_;
};

int main() {
    Box b(1);
    b.add(2).add(3).print();   // 链式调用
}
```

输出（实现无关）：
```
v=6
```

`this` 在 const 成员函数里类型是 `const T*`，所以 const 成员函数不能通过 `this` 修改成员（除非该成员被 `mutable` 标记，见第 4 节）。

### 1.3 对象大小：sizeof、空类与内存对齐

对象的 `sizeof` 由成员大小和**内存对齐**共同决定，这是面试高频题。三条规则：

1. **空类 `sizeof` 为 1**（不是 0）。标准要求不同对象的地址必须不同，所以编译器给空类塞一个字节占位。空基类优化（EBO）下，作为基类的空类可以不占空间。
2. **每个成员有自己的对齐要求**；编译器在成员之间插入填充字节，使每个成员落在其对齐倍数的地址上。
3. **整个对象的大小是其最大成员对齐的整数倍**，保证数组里相邻对象也都对齐。

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <iostream>

struct Empty {};
struct A { char c; };                 // 1 字节，对齐 1
struct B { char c; int i; };          // char + 3 填充 + int
struct C { int i; char c; };          // int + char + 3 填充
struct D { char a; char b; char c; }; // 3 字节，对齐 1

int main() {
    std::cout << "Empty: " << sizeof(Empty) << '\n';  // 1（实现相关）
    std::cout << "A:     " << sizeof(A) << '\n';      // 1
    std::cout << "B:     " << sizeof(B) << '\n';      // 8（char+3填充+int）
    std::cout << "C:     " << sizeof(C) << '\n';      // 8（int+char+3填充）
    std::cout << "D:     " << sizeof(D) << '\n';      // 3
    std::cout << "alignof(B): " << alignof(B) << '\n';// 4（最大成员 int 的对齐）
}
```

输出（64 位 g++，实现相关）：
```
Empty: 1
A:     1
B:     8
C:     8
D:     3
alignof(B): 4
```

`B` 和 `C` 成员一样、顺序不同，大小却相同（都 8），但**填充位置不同**。把大对齐的成员放前面能减少填充，这是一个常见的优化技巧。含 `virtual` 函数的类会额外多一个虚表指针（64 位下 +8 字节，见第 6、9 节）。

### 1.4 位域

当需要紧凑存储多个小整数（如协议头、硬件寄存器）时可用位域。位域的内存布局是**实现定义**的，跨平台代码不要依赖它的具体排布。

> **代码性质：上下文片段。** 展示位域语法，需放进可编译上下文。

```cpp
struct Flags {
    unsigned int a : 1;   // 1 bit
    unsigned int b : 3;   // 3 bits
    unsigned int c : 4;   // 4 bits
};
// sizeof(Flags) 通常为 4（实现相关），8 个 bit 装进一个 unsigned int
```

位域成员不能取地址（没有 `&flags.a`），也不能用 `sizeof`，且标准库容器要求元素可寻址，所以位域成员不能直接放进 `vector`。

---

## 2. 构造与析构

构造函数建立对象的有效初始状态，析构函数在对象生命周期结束时释放资源。这一节讲清几个最容易踩坑的点：成员初始化列表、初始化顺序、构造析构顺序、`explicit`。

### 2.1 成员初始化列表

成员可以在构造函数体里赋值，但**初始化列表才是真正的"初始化"**：它直接构造成员，而不是"先默认构造再赋值"。对 `const` 成员、引用成员、没有默认构造的成员类型，**必须**用初始化列表——它们不能被"赋值"。

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <iostream>
#include <string>

class Widget {
public:
    Widget(const std::string& tag, int id)
        : tag_(tag),     // const 成员：必须初始化列表
          id_(id),       // 引用成员绑定的对象：必须初始化列表
          ref_(id_) {}

    void print() const { std::cout << tag_ << " id=" << id_ << " ref=" << ref_ << '\n'; }

private:
    const std::string tag_;   // const 成员
    int id_;
    int& ref_;                // 引用成员
};

int main() {
    Widget w("sensor", 7);
    w.print();
}
```

输出（实现无关）：
```
sensor id=7 ref=7
```

含有 const 成员或引用成员的类，编译器**不会**再生成拷贝赋值运算符（给 const/引用赋值无意义），所以这类对象默认不能拷贝赋值。

### 2.2 初始化顺序陷阱（面试常考）

初始化列表里成员的**构造顺序由成员在类中的声明顺序决定，而不是列表里的书写顺序**。把列表写成与声明不同的顺序，g++ 会用 `-Wreorder` 警告，且实际行为以声明顺序为准，容易产生"依赖未初始化成员"的 bug。

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <iostream>

class Bad {
public:
    // 列表写成 sum_(n_), n_(10)，但声明顺序是 n_ 在前、sum_ 在后。
    // 实际先构造 n_（值为 10），再用 n_ 构造 sum_，得到 10。
    Bad() : sum_(n_), n_(10) {}

    void print() const { std::cout << "n=" << n_ << " sum=" << sum_ << '\n'; }

private:
    int n_;     // 声明在前：先构造
    int sum_;   // 声明在后：后构造
};

int main() {
    Bad b;
    b.print();
}
```

输出（实现无关；编译时会出现 `-Wreorder` 警告）：
```
n=10 sum=10
```

如果把声明顺序反过来（`sum_` 在前、`n_` 在后），`sum_(n_)` 会在 `n_` 尚未初始化时读取它，结果是未定义行为。**规则：始终让初始化列表的书写顺序与成员声明顺序一致**，这样 `-Wreorder` 就不会响，也不会有隐患。

### 2.3 构造与析构的顺序

对于**成员**：成员按声明顺序构造，按声明逆序析构。对于**继承**（第 5 节）：先构造基类再构造派生类成员，析构相反——先析构派生再析构基类。把构造析构想象成"穿脱衣服"：构造从里（基类）穿到外（派生），析构从外脱到里。

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <iostream>

struct Member {
    explicit Member(const char* n) : name(n) { std::cout << "ctor " << name << '\n'; }
    ~Member() { std::cout << "dtor " << name << '\n'; }
    const char* name;
};

struct Base { Base() { std::cout << "Base ctor\n"; } virtual ~Base() { std::cout << "Base dtor\n"; } };
struct Derived : Base {
    Member m1{"m1"};
    Member m2{"m2"};
    Derived() { std::cout << "Derived ctor\n"; }
    ~Derived() override { std::cout << "Derived dtor\n"; }
};

int main() {
    std::cout << "--- enter block ---\n";
    Derived d;
    std::cout << "--- leave block ---\n";
}
```

输出（实现无关）：
```
--- enter block ---
Base ctor
ctor m1
ctor m2
Derived ctor
--- leave block ---
Derived dtor
dtor m2
dtor m1
Base dtor
```

顺序：`Base` → 成员 `m1` → 成员 `m2` → `Derived` 构造体；析构完全相反。注意 `~Base` 是 `virtual`，原因见第 6.7 节。

### 2.4 explicit 与隐式转换

单参数构造函数（或除第一个外都有默认值的构造函数）能被用作**隐式转换**：把一个参数类型的值悄悄变成你的类对象。这通常不是你想要的，所以单参数构造函数应尽量标 `explicit`，禁止隐式转换，强制调用者显式写 `Widget("x")`。

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <iostream>

class Distance {
public:
    explicit Distance(double meters) : m_(meters) {}
    double meters() const { return m_; }
private:
    double m_;
};

void travel(Distance d) { std::cout << "travel " << d.meters() << "m\n"; }

int main() {
    Distance d(100.0);          // OK：显式构造
    travel(d);                  // OK：本就是 Distance
    // travel(100.0);           // 若构造函数非 explicit，此行会编译通过：100.0 隐式转成 Distance
    travel(Distance(100.0));    // explicit 下必须显式
}
```

输出（实现无关）：
```
travel 100m
travel 100m
```

`explicit` 同样适用于 C++11 起的转换运算符（`explicit operator bool()`），避免对象被偷偷转成 bool 参与算术。

---

## 3. 拷贝语义：Rule of Three 与 Rule of Five

拷贝语义是 C++ 面试必考的核心。一个管理资源的类（持有指针、文件句柄、锁等），如果允许默认拷贝，就会掉进"两个对象指向同一资源、析构时重复释放"的坑。本节先看浅拷贝的反例，再用 `MyString` 讲深拷贝，用 `DynamicArray` 讲 copy-and-swap 与强异常保证，最后总结 Rule of Three / Five。

### 3.1 浅拷贝的反例：double free

编译器为类自动生成的拷贝构造和拷贝赋值是**逐成员拷贝**。对指针成员，这只是把地址复制一份——两个对象的指针指向同一块内存，析构时各自 `delete` 一次，于是 double free（未定义行为）。

> **代码性质：反例片段。** 故意演示浅拷贝导致的 double free。**不要照抄**。要用带 Sanitizer 的方式观察：编译时加 `-fsanitize=address` 运行，会报告 heap-use-after-free / double-free。

```cpp
#include <cstring>

class BadString {
public:
    BadString(const char* s) {
        data_ = new char[std::strlen(s) + 1];
        std::strcpy(data_, s);
    }
    ~BadString() { delete[] data_; }          // 有析构，却没有自定义拷贝
private:
    char* data_;                              // 编译器生成的拷贝只复制这个指针
};

// BadString a("hi");
// BadString b = a;   // 浅拷贝：b.data_ == a.data_
// 离开作用域：先析构 b（delete[] 那块内存），再析构 a（再 delete[] 同一块）→ double free
```

这个类只写了析构函数，没有写拷贝操作，于是用了编译器生成的浅拷贝。规则见 3.4：**只要你写了析构、拷贝构造、拷贝赋值三者之一，通常就需要写齐另外两个**。

浅拷贝 vs 深拷贝的区别用图最直观。`BadString a("hi"); BadString b = a;` 之后：

```
浅拷贝（BadString 的默认拷贝）——两个对象共享一块内存，double free:

  对象 a                  对象 b              堆
  +-----------+           +-----------+      +--------+
  | data_ ----+----+      | data_ ----+--+--> | "hi\0" |  ← 只有这一块
  +-----------+    |      +-----------+  |    +--------+
                   +----------------------+
  析构 a: delete[] 这块
  析构 b: delete[] 同一块 → double free（UB）

深拷贝（MyString 的自定义拷贝）——各持一份，互不影响:

  对象 a                  对象 b
  +-----------+           +-----------+
  | data_ ----+--> +----+ | data_ ----+--> +----+
  +-----------+    |hi\0 | +-----------+    |hi\0 |  ← 两块独立内存
                   +-----+                  +-----+
  析构 a: delete[] a 的块      析构 b: delete[] b 的块  ← 各删各的，安全
```

浅拷贝只复制了指针的值（地址），两份指针指向同一块堆内存；深拷贝则 `new` 一块新内存、把内容复制过去。这正是 3.2 `MyString` 拷贝构造做的事。

### 3.2 深拷贝：MyString

深拷贝的做法是：拷贝时**重新分配**一块同样大小的新内存，把内容复制过去，让两个对象各持其份、互不影响。下面这个 `MyString` 复用自 [基础学习教程第 4.2 节](CPP基础学习教程.md)，它实现了完整的五件套（析构、拷贝构造、拷贝赋值、移动构造、移动赋值）：

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <iostream>
#include <algorithm>
#include <cstring>

class MyString {
public:
    MyString() : data_(nullptr), size_(0) {}

    MyString(const char* str) {
        size_ = std::strlen(str);
        data_ = new char[size_ + 1];
        std::copy(str, str + size_ + 1, data_);
        std::cout << "Constructed: " << data_ << "\n";
    }

    ~MyString() {
        std::cout << "Destructed: " << (data_ ? data_ : "null") << "\n";
        delete[] data_;
    }

    // 拷贝构造（深拷贝）：分配新内存再复制内容
    MyString(const MyString& other) : size_(other.size_) {
        data_ = new char[size_ + 1];
        if (other.data_ != nullptr) {              // moved-from 源可能 data_ 为 null，判空避免解引用 null
            std::copy(other.data_, other.data_ + size_ + 1, data_);
        } else {
            data_[0] = '\0';                        // 空源对象当作空串
        }
        std::cout << "Copy constructed: " << data_ << "\n";
    }

    // 移动构造（资源转移）：偷走指针，把源对象置为空但有效
    MyString(MyString&& other) noexcept
        : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;
        other.size_ = 0;
        std::cout << "Move constructed: " << data_ << "\n";
    }

    // 拷贝赋值：先释放旧资源，再深拷贝（非异常安全版本，见 3.3 的 copy-and-swap）
    MyString& operator=(const MyString& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = new char[size_ + 1];
            if (other.data_ != nullptr) {           // 同样判空，处理 moved-from 源
                std::copy(other.data_, other.data_ + size_ + 1, data_);
            } else {
                data_[0] = '\0';
            }
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

输出（实现无关；析构顺序与对象离开作用域的逆序一致）：
```
Constructed: Hello
Copy constructed: Hello
Move constructed: Hello
Move assigned: Hello
Destructed: Hello
Destructed: Hello
Destructed: null
Destructed: null
```

注意上面 `operator=` 的拷贝赋值版本有一个隐患：它**先 `delete[] data_`，再 `new`**。如果 `new` 抛异常（内存不足），当前对象已经丢了原来的数据，处于半破坏状态——这违反强异常保证。3.3 的 copy-and-swap 解决了它。

### 3.3 copy-and-swap 惯用法与强异常保证

copy-and-swap 的思路是：先用拷贝构造造一个**临时副本**（这一步可能抛异常，但此时原对象还没动），再与当前对象 `swap`（`noexcept`）。这样无论复制多复杂，原对象要么不变（复制失败），要么成功替换——这就是**强异常保证**。

`a = b;`（拷贝赋值）执行 `DynamicArray temporary(b); swap(*this, temporary);` 的三步：

```
        拷贝赋值前                ① 先造临时副本 temporary(b)         ② 再 swap(*this, temporary)

  a (旧)      b            a (旧)      b            temporary    a (新=b的副本)  temporary (旧 a)
 +------+   +------+      +------+   +------+    +---------+    +------------+  +------------+
 |旧数组|   |b数组 |      |旧数组|   |b数组 |    |b数组副本|     |b数组副本  |   |旧数组      |
 +------+   +------+      +------+   +------+    +---------+    +------------+   +-----------+
                                  ↑ 可能抛异常：此时 a 还没动
                                  ↓ 临时副本构造成功后，swap 三把指针交换
                          ③ 临时副本出作用域，析构掉"旧 a 的数组" → 安全释放，无泄漏
```

关键：可能失败的那一步（拷贝构造 `temporary`）在**原对象没被触碰**时发生——失败了 a 原样不变；成功了才用 `noexcept` 的 `swap` 整体替换。临时代替 a 扛走旧资源、析构时释放。对比 3.2 末尾"先 `delete[]` 再 `new`"的写法：那种写法 `new` 失败时旧数据已删，无法回滚。

下面这个 `DynamicArray` 复用自 [基础学习教程第 12 节](CPP基础学习教程.md)，它的拷贝赋值正是 copy-and-swap，移动赋值用 `std::exchange` 偷资源：

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <initializer_list>
#include <limits>
#include <memory>
#include <stdexcept>
#include <utility>

template<typename T>
class DynamicArray {
public:
    DynamicArray() = default;

    explicit DynamicArray(std::size_t size)
        : data_(allocate(size)), size_(size), capacity_(size) {}

    DynamicArray(std::initializer_list<T> init)
        : data_(copy_from(init)), size_(init.size()), capacity_(init.size()) {}

    // 拷贝构造：深拷贝
    DynamicArray(const DynamicArray& other)
        : data_(clone(other)), size_(other.size_), capacity_(other.capacity_) {
        std::cout << "Copy constructor\n";
    }

    // 移动构造：偷资源，noexcept
    DynamicArray(DynamicArray&& other) noexcept
        : data_(std::exchange(other.data_, nullptr)),
          size_(std::exchange(other.size_, 0)),
          capacity_(std::exchange(other.capacity_, 0)) {
        std::cout << "Move constructor\n";
    }

    ~DynamicArray() { delete[] data_; }

    // 拷贝赋值：copy-and-swap，强异常保证
    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) {
            DynamicArray temporary(other);   // 复制失败时，当前对象保持原状
            swap(temporary);
        }
        std::cout << "Copy assignment\n";
        return *this;
    }

    // 移动赋值
    DynamicArray& operator=(DynamicArray&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = std::exchange(other.data_, nullptr);
            size_ = std::exchange(other.size_, 0);
            capacity_ = std::exchange(other.capacity_, 0);
        }
        std::cout << "Move assignment\n";
        return *this;
    }

    std::size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }
    T& operator[](std::size_t i) { return data_[i]; }
    const T& operator[](std::size_t i) const { return data_[i]; }

    void swap(DynamicArray& other) noexcept {
        using std::swap;
        swap(data_, other.data_);
        swap(size_, other.size_);
        swap(capacity_, other.capacity_);
    }

private:
    static T* allocate(std::size_t capacity) {
        return capacity == 0 ? nullptr : new T[capacity]{};
    }
    static T* copy_from(std::initializer_list<T> init) {
        std::unique_ptr<T[]> result(allocate(init.size()));
        if (init.size() > 0) std::copy(init.begin(), init.end(), result.get());
        return result.release();
    }
    static T* clone(const DynamicArray& other) {
        std::unique_ptr<T[]> result(allocate(other.capacity_));
        if (other.size_ > 0) std::copy_n(other.data_, other.size_, result.get());
        return result.release();
    }

    T* data_ = nullptr;
    std::size_t size_ = 0;
    std::size_t capacity_ = 0;
};

int main() {
    DynamicArray<int> arr = {1, 2, 3, 4, 5};
    DynamicArray<int> arr2 = arr;                 // 拷贝构造
    DynamicArray<int> arr3 = std::move(arr);      // 移动构造
    arr2 = arr3;                                  // 拷贝赋值（copy-and-swap）
    std::cout << "arr3 size=" << arr3.size() << " arr size=" << arr.size() << '\n';
}
```

输出（实现无关）：
```
Copy constructor          // arr2 = arr（拷贝构造）
Move constructor          // arr3 = std::move(arr)
Copy constructor          // arr2 = arr3：copy-and-swap 先构造临时副本（这一行常被漏算）
Copy assignment           // 再 swap，打印 Copy assignment
arr3 size=5 arr size=0
```

为什么移动操作要标 `noexcept`？因为 `std::vector` 在扩容时，会检查元素的移动构造是否 `noexcept`：是就用移动，否则退化为拷贝以保证扩容中途抛异常时的强异常保证。你的移动构造若不是 `noexcept`，性能优势可能在 `vector` 里完全体现不出来。这是面试常被追问的点。

#### 实证：vector 扩容到底走移动还是拷贝

口说无凭，下面两个类成员相同、唯一区别是移动构造有没有标 `noexcept`，观察 `vector` 扩容时搬运旧元素用哪个构造函数：

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <iostream>
#include <vector>

struct NoNoexceptMove {
    int id;
    explicit NoNoexceptMove(int i) : id(i) {}
    NoNoexceptMove(NoNoexceptMove&& o) : id(o.id) {              // 未标 noexcept
        std::cout << "  move(id=" << id << ")\n";
    }
    NoNoexceptMove(const NoNoexceptMove& o) : id(o.id) {
        std::cout << "  copy(id=" << id << ")\n";
    }
};

struct NoexceptMove {
    int id;
    explicit NoexceptMove(int i) : id(i) {}
    NoexceptMove(NoexceptMove&& o) noexcept : id(o.id) {          // 标了 noexcept
        std::cout << "  move(id=" << id << ")\n";
    }
    NoexceptMove(const NoexceptMove& o) : id(o.id) {
        std::cout << "  copy(id=" << id << ")\n";
    }
};

int main() {
    std::cout << "== NoNoexceptMove：扩容应走拷贝 ==\n";
    std::vector<NoNoexceptMove> v; v.reserve(2);
    v.emplace_back(1); v.emplace_back(2);
    std::cout << "触发扩容:\n"; v.emplace_back(3);

    std::cout << "== NoexceptMove：扩容应走移动 ==\n";
    std::vector<NoexceptMove> w; w.reserve(2);
    w.emplace_back(1); w.emplace_back(2);
    std::cout << "触发扩容:\n"; w.emplace_back(3);
}
```

输出（实现无关——标准要求的行为）：
```
== NoNoexceptMove：扩容应走拷贝 ==
触发扩容:
  copy(id=1)
  copy(id=2)
== NoexceptMove：扩容应走移动 ==
触发扩容:
  move(id=1)
  move(id=2)
```

仅差一个 `noexcept`，扩容搬运旧元素就从拷贝变成移动。对持有堆内存的对象（如 `std::string`、含 `buffer` 的类），拷贝要深复制整块内存、移动只换指针，两者可能差上百倍。

#### 背后机制：std::move_if_noexcept

标准库不是直接用 `std::move`，而是 `std::move_if_noexcept`——它按编译期类型特征决定移动还是拷贝：移动构造是 `noexcept`（或类型不可拷贝）就转成右值走移动，否则保持左值走拷贝。libstdc++ 的 `vector` 扩容正是用 `_GLIBCXX_MAKE_MOVE_IF_NOEXCEPT_ITERATOR` 包裹迭代器，搬运时按这条规则选择。其本质是一段 `conditional`：若 `is_nothrow_move_constructible` 不成立且类型可拷贝，就用普通迭代器（拷贝）。

设计动机是**强异常安全**：扩容要保证"中途抛异常时原容器数据完好"。移动若抛异常，旧元素已被移走、回滚不回去；拷贝失败时旧元素完整保留、释放新内存即可回滚。所以宁可慢也要安全。**推论：资源类的移动构造/赋值必须标 `noexcept`，否则在容器里等于没写。**

#### 析构函数是唯一"隐式 noexcept"的特殊成员

一个常被忽略的细节：从 C++11 起，析构函数是特殊成员里**唯一**用户手写也默认带隐式 `noexcept` 的——编译器会按"所有成员与基类的析构是否 noexcept"推导。而移动/拷贝构造、移动/拷贝赋值，**只有编译器生成或 `= default` 时才隐式推导 noexcept；你手写函数体，默认就是可能抛出（`noexcept(false)`）**。所以上面 `MyString`、`DynamicArray` 的移动操作必须显式标 `noexcept`，否则 `vector` 不会用它们。这也提醒：**手写移动操作，永远记得标 `noexcept`。**

### 3.4 Rule of Three / Rule of Five

经验法则：

- **Rule of Three**（C++98）：需要自定义**析构函数、拷贝构造、拷贝赋值**三者之一时，通常需要写齐三者。因为析构需要释放资源，意味着默认的浅拷贝会重复释放，所以拷贝操作也得自定义。
- **Rule of Five**（C++11）：在 Rule of Three 基础上再加**移动构造、移动赋值**。资源类写齐五件套才能既正确又高效。

但现代 C++ 更推崇 **Rule of Zero**：如果你的成员都用 RAII 类型（`std::vector`、`std::string`、智能指针），编译器生成的默认五件套就是对的，你**什么都不用写**。上面的 `DynamicArray` 是为了教学而手写；实际代码里直接 `std::vector<int>` 即可，无需操心拷贝移动。**优先 Rule of Zero，只有当类直接管理原始资源时才落到 Rule of Five。**

#### 多态类的一个例外：虚析构会阻止隐式移动

一个容易踩的坑（cppreference 三五零原则、Core Guidelines C.21/C.67 指出）：**用户声明了析构函数（哪怕是 `= default`），就会阻止编译器隐式生成移动构造和移动赋值**。而多态基类为了通过基类指针正确析构必须把析构声明为 `virtual`——一旦你写了 `virtual ~Base() = default;`，编译器就不再给 `Base` 生成移动操作，复制操作也会被弃用（deprecated）。

正确写法是多态基类把五件套都显式 `= default`，并按需禁用拷贝：

> **代码性质：上下文片段。** 展示多态基类的正确特殊成员声明。

```cpp
class PolymorphicBase {
public:
    PolymorphicBase() = default;
    PolymorphicBase(const PolymorphicBase&) = default;            // 显式声明，避免被虚析构抑制
    PolymorphicBase(PolymorphicBase&&) = default;
    PolymorphicBase& operator=(const PolymorphicBase&) = default;
    PolymorphicBase& operator=(PolymorphicBase&&) = default;
    virtual ~PolymorphicBase() = default;                         // virtual 析构：多态删除必须
};
```

更进一步（Core Guidelines C.67）：**多态类通常应禁用拷贝**，因为按值拷贝一个派生类对象到基类会发生对象切片（见第 7.1 节），语义错误。所以多态基类常把拷贝 `= delete`，只留移动或全禁。这是"基类析构必须 virtual"之外的另一条多态类设计准则。

### 3.5 禁止拷贝

有时一个类语义上不该被拷贝（如单例、`std::mutex`、`std::unique_ptr`）。两种做法：

- C++11 起：`T(const T&) = delete;` 和 `T& operator=(const T&) = delete;`（推荐，错误信息清晰）。
- C++11 前：把拷贝构造和拷贝赋值声明为 `private` 且不实现（调用即编译期或链接期错误）。

> **代码性质：上下文片段。** 展示 `=delete` 禁拷贝，需放进可编译上下文。

```cpp
class NonCopyable {
public:
    NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;            // 禁止拷贝构造
    NonCopyable& operator=(const NonCopyable&) = delete; // 禁止拷贝赋值
    // 注意：用户声明（含 =delete）的拷贝操作会抑制编译器隐式生成移动操作。
    // 若想仍可移动，需显式 =default 移动构造和移动赋值；否则该类不可拷贝也不可移动。
    NonCopyable(NonCopyable&&) = default;
    NonCopyable& operator=(NonCopyable&&) = default;
};
```

> 这条"声明拷贝就抑制移动"的规则（Rule of Five 的推论）详见 [Effective Modern C++ Item 17](Effective_Modern_CPP教程.md#item-17-理解特殊成员函数的生成规则)。

`std::mutex` 则不同：它不可拷贝也不可移动（移动也被删），因为锁住的资源不能随便转移。

---

## 4. static、const 与 friend

### 4.1 static 成员

`static` 成员变量属于**类本身**，而不是某个对象——所有对象共享同一份，生命周期贯穿整个程序。它必须在类外定义一次（C++17 起若声明为 `inline` 可在类内定义）。

`static` 成员函数没有 `this` 指针，因此只能访问 static 成员，不能访问普通成员。它常用于工厂方法、计数器、单例获取。

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <iostream>

class Counter {
public:
    Counter() { ++count_; }
    ~Counter() { --count_; }

    static int alive() { return count_; }   // static 成员函数，无 this

private:
    static int count_;                      // 声明
};

int Counter::count_ = 0;                    // 类外定义（必须，且只一次）

int main() {
    std::cout << "alive=" << Counter::alive() << '\n';   // 0，可不创建对象就调用
    {
        Counter a, b, c;
        std::cout << "alive=" << Counter::alive() << '\n'; // 3：所有对象共享一份 count_
    }
    std::cout << "alive=" << Counter::alive() << '\n';   // 0
}
```

输出（实现无关）：
```
alive=0
alive=3
alive=0
```

### 4.2 const 成员函数与 mutable

在成员函数参数列表后加 `const`，表示"这个函数不会修改对象的可观察状态"。const 成员函数内不能修改成员，也不能调用非 const 成员函数。const 对象只能调用 const 成员函数。

有时一个成员在逻辑上不算"不变"，但物理上需要修改（如缓存、互斥锁）。这时用 `mutable` 让该成员即使在 const 成员函数里也能改。

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <iostream>
#include <mutex>

class Cache {
public:
    int get(int key) const {
        std::lock_guard<std::mutex> lk(mtx_);   // const 函数里也要能加锁
        ++access_;                               // mutable：逻辑常量，物理可变（计数）
        return key * 2;                          // 简化：省略真实缓存查表
    }
    int accesses() const {
        std::lock_guard<std::mutex> lk(mtx_);   // 读计数也要加锁，否则与 get() 的写构成数据竞争
        return access_;
    }
private:
    mutable std::mutex mtx_;    // mutable：const 成员函数里也能锁
    mutable int access_ = 0;    // mutable：访问计数，逻辑上不改变对象状态
};

int main() {
    const Cache c;              // const 对象：只能调 const 成员函数
    c.get(3);
    c.get(5);
    std::cout << "accesses=" << c.accesses() << '\n';
}
```

输出（实现无关）：
```
accesses=2
```

### 4.3 friend

`friend` 让一个外部函数或类获得访问私有成员的权限。它破坏封装，应谨慎使用，主要用于运算符重载（如 `operator<<` 需要 `ostream` 而成员函数做不到）或两个紧密协作的类。

> **代码性质：上下文片段。** 展示 friend 用法，需放进可编译上下文。

```cpp
#include <iostream>

class Vector {
public:
    Vector(int x, int y) : x_(x), y_(y) {}
    friend std::ostream& operator<<(std::ostream& os, const Vector& v);  // 声明友元
private:
    int x_, y_;
};

// 友元函数不是成员，但能访问 Vector 的 private 成员
std::ostream& operator<<(std::ostream& os, const Vector& v) {
    return os << "(" << v.x_ << "," << v.y_ << ")";
}
```

`friend` 是单向且不可继承的：A 声明 B 是友元，B 能访问 A 的私有，但 A 不能访问 B 的，且 B 的派生类也不是 A 的友元。

---

## 5. 继承

继承表达 "is-a" 关系：派生类是一个基类。C++ 支持三种继承方式（public/protected/private），它们决定基类的成员在派生类里的最高访问级别。

### 5.1 三种继承方式

| 基类成员 | public 继承 | protected 继承 | private 继承 |
|---|---|---|---|
| `public` 成员 | 派生里仍是 `public` | 变 `protected` | 变 `private` |
| `protected` 成员 | 派生里仍是 `protected` | 仍是 `protected` | 变 `private` |
| `private` 成员 | 派生类**不可访问** | 不可访问 | 不可访问 |

绝大多数情况用 **public 继承**（真正的 is-a）。protected/private 继承表达"用实现"而非"是一个"，用得少；表达"有/包含"关系更应该用**成员**（has-a，组合）而不是继承。

### 5.2 构造析构顺序

继承链上：**构造先基后派，析构先派后基**（第 2.3 节已演示）。派生类构造函数用初始化列表把参数传给基类构造函数；如果基类没有默认构造函数，派生类必须显式调用。

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <iostream>
#include <string>

class Animal {
public:
    explicit Animal(const std::string& name) : name_(name) {
        std::cout << "Animal ctor: " << name_ << '\n';
    }
    virtual ~Animal() { std::cout << "Animal dtor: " << name_ << '\n'; }
    const std::string& name() const { return name_; }
private:
    std::string name_;
};

class Dog : public Animal {
public:
    Dog(const std::string& name, const std::string& breed)
        : Animal(name), breed_(breed) {            // 必须在列表里调用基类构造
        std::cout << "Dog ctor: " << breed_ << '\n';
    }
    ~Dog() override { std::cout << "Dog dtor: " << breed_ << '\n'; }
private:
    std::string breed_;
};

int main() {
    Dog d("Rex", "Labrador");
}
```

输出（实现无关）：
```
Animal ctor: Rex
Dog ctor: Labrador
Dog dtor: Labrador
Animal dtor: Rex
```

### 5.3 名字隐藏 vs 重写（易混点）

- **名字隐藏（hiding）**：派生类声明了与基类**同名**的成员（无论签名是否相同），基类的同名成员在派生类里就被隐藏。要调用被隐藏的基类成员需用 `Base::f()` 或 `using Base::f;`。普通（非虚）函数重定义都是隐藏，不是多态。
- **重写（overriding）**：派生类重新定义基类的**虚函数**，签名必须一致，通过基类指针/引用调用时走动态绑定（第 6 节）。

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <iostream>

struct Base {
    void f(int)        { std::cout << "Base::f(int)\n"; }
    void f(int, int)   { std::cout << "Base::f(int,int)\n"; }
};

struct Derived : Base {
    void f(int) { std::cout << "Derived::f(int)\n"; }  // 隐藏了 Base 的两个 f，不是重载
    // 想让 Base::f(int,int) 仍可用： using Base::f;
};

int main() {
    Derived d;
    d.f(1);          // Derived::f(int)
    // d.f(1, 2);    // 编译错误：Derived::f 隐藏了 Base::f(int,int)
    d.Base::f(1, 2); // 显式调用被隐藏的基类版本
}
```

输出（实现无关）：
```
Derived::f(int)
Base::f(int,int)
```

注意：派生类的 `f(int)` **隐藏**了基类 `f(int)` 和 `f(int,int)` 两个重载版本，而不是只覆盖其中一个。这是 C++ 新手最常踩的坑——基类重载在派生类里"莫名其妙"消失了，解决方法是 `using Base::f;` 把基类重载引入派生类作用域。

### 5.4 is-a vs has-a

不要滥用继承。判断标准：

- **is-a（是一个）**：派生类是基类的一种，能用基类的地方都能用派生类 → public 继承。如 `Dog` is-a `Animal`。
- **has-a（有一个）/ 用实现**：派生类内部用了基类的功能，但不暴露"是基类"的接口 → 用成员组合，或 private 继承。如 `Car` has-a `Engine`（组合，不是继承）。

**组合优先于继承**是面向对象设计的基本原则：组合在运行期可换实现、不破坏封装，比继承灵活得多。

---

## 6. 多态与虚函数

多态是 C++ 面试最高频的考点：虚函数、虚函数表（vtable）、`override`、纯虚函数、虚析构。本节把这些一次讲透。

### 6.1 静态绑定 vs 动态绑定

- **静态绑定（早绑定）**：编译期就确定调用哪个函数。普通函数、非虚函数、重载都是静态绑定——根据指针/引用的**静态类型**决定。
- **动态绑定（晚绑定）**：运行期根据对象的**实际类型**决定调用哪个虚函数。通过基类指针/引用调用 `virtual` 函数时发生。

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <iostream>

struct Base {
    void who()              { std::cout << "Base\n"; }   // 非虚：静态绑定
    virtual void whoVirt()  { std::cout << "Base\n"; }   // 虚：动态绑定
    virtual ~Base() = default;
};

struct Derived : Base {
    void who()              { std::cout << "Derived\n"; }     // 隐藏，不是重写
    void whoVirt() override { std::cout << "Derived\n"; }     // 重写虚函数
};

int main() {
    Derived d;
    Base& rb = d;
    rb.who();      // 静态绑定：静态类型 Base& → "Base"
    rb.whoVirt();  // 动态绑定：实际对象 Derived → "Derived"
}
```

输出（实现无关）：
```
Base
Derived
```

关键区别：`who()` 没声明 `virtual`，所以 `rb.who()` 看 `rb` 的静态类型 `Base&`，调用 `Base::who`；`whoVirt()` 是 `virtual`，`rb.whoVirt()` 在运行期查实际对象的类型，调用 `Derived::whoVirt`。这就是多态。

同一个 `Base& rb = d;`，两种调用走不同路径：

```
静态绑定（非虚 who）：编译期就定了                动态绑定（虚 whoVirt）：运行期查表

  rb.who()                                          rb.whoVirt()
    │ 静态类型 Base& → 编译期直接绑到 Base::who        │ 看 rb 实际指向的对象 d
    ▼                                                ▼
  Base::who()  → "Base"                             对象 d: [vptr] ──┐
                                                      +----+         │ 查 vptr
                                                      |... |         ▼
                                                   Derived 的 vtable: [whoVirt → &Derived::whoVirt]
                                                                        │
                                                                        ▼
                                                                   Derived::whoVirt() → "Derived"
```

左边：`who` 非 virtual，调用地址编译期就嵌进指令，不管 `rb` 实际指向谁都调 `Base::who`。右边：`whoVirt` 是 virtual，编译期只生成"读 vptr→查表→跳转"的代码，运行期才决定跳到哪个 `whoVirt`——这就是"动态绑定"。

### 6.2 virtual 的语义与 vtable 原理

只要类里有 `virtual` 函数，编译器就为这个类生成一张**虚函数表（vtable）**：一张存着该类各虚函数实际地址的函数指针数组。每个对象内部多一个隐藏的**虚表指针（vptr）**，指向所属类的 vtable。调用虚函数时，程序通过 `vptr` 找到 vtable，再按偏移取出正确函数地址调用——这就是动态绑定的代价：一次额外间接跳转。

`vptr` 的大小依赖平台：**64 位平台通常是 8 字节**。下面验证 `virtual` 让对象变大：

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <iostream>
#include <typeinfo>

struct NoVirtual  { int a; };                  // 无虚函数
struct HasVirtual { int a; virtual void f() {} };  // 有虚函数：多一个 vptr

int main() {
    std::cout << "sizeof(NoVirtual)=" << sizeof(NoVirtual) << '\n';
    std::cout << "sizeof(HasVirtual)=" << sizeof(HasVirtual) << '\n';
    std::cout << "alignof(HasVirtual)=" << alignof(HasVirtual) << '\n';
}
```

输出（64 位 g++，实现相关）：
```
sizeof(NoVirtual)=4
sizeof(HasVirtual)=16
alignof(HasVirtual)=8
```

`NoVirtual` 只有一个 `int`，4 字节；`HasVirtual` 多了 8 字节 vptr，加上 `int a` (4) 和对齐填充 (4)，按 8 对齐得到 16 字节。这就是虚函数的空间代价。对象内存布局大致是：

```
HasVirtual 对象（64 位）:
+--------+----------------+
| vptr   | 8 字节，指向 HasVirtual 的 vtable
+--------+----------------+
| int a  | 4 字节
+--------+----------------+
| 填充   | 4 字节（凑成 8 的倍数）
+--------+----------------+
```

### 6.3 vtable 的真实内存布局（进阶）

上面把 vtable 说成"函数指针数组"是简化。按主流的 Itanium C++ ABI（g++/clang 采用），vtable 的真实结构在函数指针数组**之前还有两个前缀字段**：

```
vtable for Derived（Itanium ABI，64 位）:
+---------------------+
| offset_to_top       | 指针：把"指向本子对象的指针"调整到完整对象起点的偏移
+---------------------+
| typeinfo_ptr        | 指针：本类的 std::type_info，RTTI 之源（typeid/dynamic_cast 用）
+---------------------+
| &Derived::f0        | ← vptr 实际指向这里（虚函数地址数组起点）
| &Derived::f1        |
| ...                 |
+---------------------+
```

要点（Itanium ABI，实现相关）：

- 对象里的 vptr **不是指向 vtable 的开头**，而是指向"虚函数地址数组"的起点——即跳过 `offset_to_top` 和 `typeinfo` 两个字段。这两个字段位于 vptr 所指位置**之前**。
- `offset_to_top`：多继承下，一个对象可能含多个基类子对象，每个子对象一个 vptr。通过某个基类子对象的指针往回转成"完整对象指针"时，需要加这个偏移。单继承下它通常是 0。
- `typeinfo_ptr`：指向该类的 `std::type_info`，这是 `typeid(*p).name()` 和 `dynamic_cast` 运行期类型判断的依据。**RTTI 信息挂在 vtable 上，所以只有含虚函数的类才能用 `dynamic_cast`**——这正是第 8.2 节"dynamic_cast 依赖虚函数"的底层原因。
- vtable 存放在程序的**只读数据段（`.rodata`）**，每个类一份、全局共享；vptr 存在每个对象内部，每个对象一份。所以多创建对象不会复制 vtable，只多一个 8 字节指针。

**vptr 何时初始化**：构造函数执行的最开始，编译器插入代码把 vptr 指向当前正在构造的类的 vtable；析构时则在进入各层析构函数时把 vptr 重新指回当前类。这解释了第 7.2 节"构造/析构中调 virtual 不会多态"——构造基类子对象时 vptr 还指向基类的 vtable。

**多继承下的多个 vptr**：一个类若继承多个含虚函数的基类，对象里就有**多个 vptr，每个基类一个**。第 9.3 节会展示菱形继承的 sizeof。

### 6.4 绕开 vtable：CRTP 静态多态

虚函数的代价（vptr 8 字节 + 一次间接跳转 + 无法内联）在"热路径上频繁调用小虚函数"时会显现。如果多态的派生类型在**编译期已知**，可用 **CRTP（Curiously Recurring Template Pattern）** 实现静态多态——编译期就绑定，无 vtable、可内联：

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <iostream>

template<typename Derived>
struct ShapeBase {
    // 编译期把 this 转成派生类，直接调派生类实现——无虚函数、无 vtable
    double area() const { return static_cast<const Derived*>(this)->areaImpl(); }
};

struct Circle : ShapeBase<Circle> {
    explicit Circle(double r) : r_(r) {}
    double areaImpl() const { return 3.14159 * r_ * r_; }
private:
    double r_;
};

int main() {
    Circle c(2);
    std::cout << "area=" << c.area() << " (static dispatch, no vtable)\n";
    std::cout << "sizeof(Circle)=" << sizeof(Circle) << " (no vptr)\n";
}
```

输出（64 位 g++，实现无关）：
```
area=12.5664 (static dispatch, no vtable)
sizeof(Circle)=8 (no vptr)
```

`Circle` 同样能通过基类接口 `area()` 调到派生类的 `areaImpl()`，但走的是编译期绑定：没有 vptr（sizeof 仍是 8，对比 §6.2 含虚函数的 `HasVirtual` 是 16），调用可被内联。代价是派生类型必须在编译期确定，不能像 `Shape*` 那样在运行期装进同一个容器混合不同派生类。**虚函数用于运行期多态（容器装混合派生类），CRTP 用于编译期多态（类型已知、要内联、要省 vptr）。**

### 6.5 override 与 final（C++11）

`override` 显式告诉编译器"这是在重写基类虚函数"。如果签名对不上（比如基类没有这个虚函数，或参数/const 不一致），编译器报错。**所有重写都应写 `override`**——它是编译期检查，零运行开销，却能抓住"以为重写了其实只是隐藏"这类 bug。

> **代码性质：上下文片段。** 展示 override 的编译期检查。

```cpp
struct Base {
    virtual void f(int) {}
    virtual void g() const {}
};
struct Derived : Base {
    void f(int) override {}        // OK：签名一致
    // void f(double) override;    // 编译错误：基类没有 f(double)，override 抓住"没真正重写"
    // void g() override {}         // 编译错误：基类 g() 是 const，这里漏了 const → 不是重写
};
```

仓库的 [设计模式教程](设计模式教程.md) 里有 78 处 `override`，正是依赖这个检查来保证多态正确。不写 `override` 时，基类签名一改，派生类就会悄悄变成"隐藏"而非"重写"，运行期行为错误且无任何编译警告——这是多态最隐蔽的 bug 来源。

`final` 禁止进一步重写或进一步继承：`void f() override final;`（此虚函数不能再被重写），或 `class Derived final : Base`（此类不能再被继承）。

### 6.6 纯虚函数与抽象基类

纯虚函数是 `= 0` 的虚函数，没有（默认）实现。含纯虚函数的类是**抽象类**，不能直接实例化，只能作为接口被派生类实现。派生类必须重写所有纯虚函数才能成为具体类。

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <iostream>
#include <string>

struct Shape {                       // 抽象基类（接口）
    virtual double area() const = 0; // 纯虚函数
    virtual ~Shape() = default;      // 抽象类也要有虚析构
};

struct Circle : Shape {
    explicit Circle(double r) : r_(r) {}
    double area() const override { return 3.14159 * r_ * r_; }
private:
    double r_;
};

struct Square : Shape {
    explicit Square(double s) : s_(s) {}
    double area() const override { return s_ * s_; }
private:
    double s_;
};

int main() {
    // Shape s;            // 编译错误：抽象类不能实例化
    Circle c(2);
    Square q(3);
    Shape* shapes[] = {&c, &q};       // 基类指针多态容器
    for (Shape* s : shapes) {
        std::cout << "area=" << s->area() << '\n';   // 动态绑定
    }
}
```

输出（实现无关）：
```
area=12.5664
area=9
```

这正是模型推理项目里 `unique_ptr<Layer>` 多态容器的原理：`Layer` 是抽象基类，`Conv2D`/`ReLU` 等是派生类，通过基类指针调用 `forward()` 走动态绑定。

```
"一个基类指针数组、各装不同派生类"是多态的典型用法：

  shapes[] (Shape* 数组)         实际对象（各自有自己的 vptr 和 vtable）
 +----------+                   Circle c: [vptr → Circle表]  [r_=2]
 | &c ------+---------------->                 表里 area → &Circle::area
 +----------+
 | &q ------+----------------> Square q: [vptr → Square表]  [s_=3]
 +----------+                              表里 area → &Square::area

 循环 for (Shape* s : shapes) s->area():
   s->area() → 读 s 的 vptr → 查各自 vtable 的 area 槽 → 跳到对应实现
   → 12.5664（圆）, 9（方）            同一句调用，因 vptr 不同而分发到不同函数 = 多态
```

抽象基类 `Shape`（有纯虚 `area()`）不能实例化，只起"接口"作用；`shapes[]` 里只能放具体派生类的指针。这就是"接口 + 多态分发"的标准范式。

### 6.7 虚析构：为什么基类析构必须是 virtual

这是经典面试题。当你用 `Base*` 指针 `delete` 一个实际是派生类的对象时：如果 `~Base()` 不是虚的，只调用 `~Base()`，**派生类的析构函数不会被调用**，派生类持有的资源泄漏（未定义行为）。把基类析构声明为 `virtual`，析构链才完整。

> **代码性质：反例片段。** 第一段 `delete` 通过非虚析构基类指针是**未定义行为**——标准不保证只跳过 `~BadDerived`，实际可能跳过、可能崩溃、可能"看似正常"。下面给出的"~BadBase"是 g++ 常见行为，但**不是实现无关的保证**。观察它须用 ASan 或受控方式，不要当确定行为来记。第二段（GoodBase 虚析构）是合法程序。**不要把第一段当可复制的正确代码。**

```cpp
#include <iostream>

struct BadBase  { virtual void f() {} ~BadBase()  { std::cout << "~BadBase\n"; } };   // 非虚析构
struct GoodBase { virtual void f() {} virtual ~GoodBase() { std::cout << "~GoodBase\n"; } };

struct BadDerived  : BadBase  { ~BadDerived()  { std::cout << "~BadDerived\n"; } };
struct GoodDerived : GoodBase { ~GoodDerived() override { std::cout << "~GoodDerived\n"; } };

int main() {
    {
        BadBase* p = new BadDerived;
        delete p;          // UB：基类析构非虚，~BadDerived 不保证被调用 → 典型实现只打 ~BadBase、资源泄漏
    }
    {
        GoodBase* p = new GoodDerived;
        delete p;          // 合法且完整：~GoodDerived → ~GoodBase
    }
}
```

输出（第一段为 g++ 常见行为、**非标准保证**；第二段为实现无关）：
```
~BadBase
~GoodDerived
~GoodBase
```

第一条只打印 `~BadBase`——`~BadDerived` 没跑。第二条正确调用了 `~GoodDerived` 再 `~GoodBase`。**规则：只要一个类打算被多态使用（有其他虚函数、或会被 `delete` 通过基类指针），它的析构函数就应该是 `virtual`。** 这是"基类析构必须 virtual"的真正含义。

抽象基类即使没有其他资源，也常用 `virtual ~Base() = default;` 或纯虚析构来保证通过基类指针删除时正确链到派生类析构。

---

## 7. 多态的代价与陷阱

### 7.1 对象切片（slicing）

按值传递或按值返回一个派生类对象给基类时，派生类独有的部分被"切掉"，只剩下基类部分。后续通过它调用虚函数也只能调到基类版本——多态被破坏。多态必须用**指针或引用**，不能按值传对象。

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <iostream>

struct Base {
    int b = 1;
    virtual ~Base() = default;
    virtual void who() { std::cout << "B\n"; }
};

struct Derived : Base {
    int d = 2;                         // 派生类独有数据
    void who() override { std::cout << "D\n"; }
};

void show(Base byval) {                // 按值传：触发对象切片
    std::cout << "sizeof=" << sizeof(byval) << ' ';
    byval.who();                       // 只剩基类部分，多态丢失 → "B"
}

int main() {
    Derived der;
    std::cout << "before slice sizeof(der)=" << sizeof(der) << '\n';
    show(der);                         // 切片
}
```

输出（64 位 g++，实现相关）：
```
before slice sizeof(der)=16
sizeof=16 B
```

切片到底切掉了什么？看内存布局：

```
Derived der（完整对象，16 字节）              show(der) 按值传，拷贝构造 Base byval
+----------------+                              +----------------+
| vptr → Derived |  指向 Derived 的 vtable       | vptr → Base    | ← 切片后 vptr 被改成指向 Base 的 vtable
+----------------+   who() 在 Derived 表里 → "D" +----------------+   who() 现在查 Base 表 → "B"
| int b = 1      |  基类成员                     | int b = 1      |   只拷基类可见部分
+----------------+                              +----------------+
| int d = 2      |  派生类独有                   (基类部分到此为止)
+----------------+                              ↑ d 被丢弃（"切掉"）
```

派生类按值传给基类形参时，调用的是**基类的拷贝构造**，它只认识基类子对象：把 vptr 改成指向 `Base` 的 vtable、拷贝 `b`，派生类独有的 `d` 没人拷、被丢掉。结果 `byval` 是个"长得像 Base"的对象，`who()` 自然查到 `Base` 的版本——多态被破坏。`sizeof=16` 是因为这个 `Base` 子对象本身含 vptr(8)+b(4)+填充(4)，恰好 16（不是 `der` 的 16 被原样保留，而是 `Base` 子对象刚好也占 16）。

修复：把形参改成引用或指针，不触发拷贝构造——`void show(Base& byref)` 或 `Base* byptr`，此时 vptr 不被改写，多态保留。**多态必须用指针或引用，不能按值传对象。**

### 7.2 构造/析构中调 virtual 不会多态

构造函数执行时，对象当前类型是正在构造的类（vptr 指向当前类的 vtable），所以调用虚函数调到**当前类的版本**，不是派生类的。析构同理。这是为了安全：构造派生类部分时，基类部分先就绪，此时派生类的成员还没初始化，调用派生类虚函数会访问未初始化的数据。

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <iostream>

struct Base {
    Base() { std::cout << "[Base ctor] "; who(); }   // 构造中调 virtual
    virtual void who() { std::cout << "Base\n"; }
    virtual ~Base() { std::cout << "[Base dtor] "; who(); }  // 析构中调 virtual
};

struct Derived : Base {
    Derived() { std::cout << "[Derived ctor] "; who(); }
    void who() override { std::cout << "Derived\n"; }
    ~Derived() override { std::cout << "[Derived dtor] "; who(); }
};

int main() {
    std::cout << "--- create ---\n";
    Derived d;
    std::cout << "--- destroy ---\n";
}
```

输出（实现无关）：
```
--- create ---
[Base ctor] Base
[Derived ctor] Derived
--- destroy ---
[Derived dtor] Derived
[Base dtor] Base
```

`Base` 构造函数里调 `who()` 打印的是 `Base`，不是 `Derived`——此时 `Derived` 还没构造，多态未生效。析构时同理：销毁 `Base` 部分时 `Derived` 已经没了，`who()` 调到 `Base`。**结论：不要在构造/析构里调虚函数期望多态。**

### 7.3 多继承与 vtable

C++ 允许多继承。每个有虚函数的基类各自带一个 vptr，多继承对象里就有多个 vptr，按基类声明顺序排列。多继承会带来名字歧义（两个基类都有同名成员），需要用 `Base::member` 消歧。

### 7.4 菱形继承与虚继承简介

`D` 同时继承 `B1` 和 `B2`，而 `B1`、`B2` 都继承 `A`，构成菱形。普通继承下 `D` 会有**两份** `A` 的子对象，访问 `A` 成员时歧义，`A` 的数据被复制两份。

**虚继承**（`class B1 : virtual public A`）解决此问题：让 `A` 成为共享子对象，`D` 里只有一份 `A`。虚继承的实现较复杂（引入 vbase 偏移表），且访问虚基类成员稍慢。**菱形继承和虚继承了解原理即可，现代 C++ 设计应优先用组合而非复杂继承层级来避免菱形。**

---

## 8. 强制类型转换

C++ 提供四种命名的显式转换，各自用途不同。仓库里 `dynamic_cast` 和 `const_cast` 此前完全没有，`reinterpret_cast` 用得很少，这一节补齐。**命名转换比 C 风格的 `(T)x` 好**：转换意图明确、作用域受限、危险的转换会被编译器检查出来。

### 8.1 static_cast

`static_cast` 用于编译期已知的、合法的类型转换：

- 数值类型之间（`double`→`int` 截断、`int`→`double` 提升等）。
- 指针的上行转换（`Derived*`→`Base*`，安全）和下行转换（`Base*`→`Derived*`，**不检查**对象是否真的是派生类，错了是未定义行为）。
- `void*` 与具体类型指针互转。
- 调用显式构造/转换运算符。

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <iostream>

struct Base { virtual ~Base() = default; };
struct Derived : Base { int value = 42; };

int main() {
    double pi = 3.14;
    int n = static_cast<int>(pi);          // 截断
    std::cout << "n=" << n << '\n';        // 3

    Derived d;
    Base* bp = &d;                          // 上行：隐式即可，static_cast 也可
    Derived* dp = static_cast<Derived*>(bp);// 下行：不检查，这里碰巧安全
    std::cout << "value=" << dp->value << '\n';
}
```

输出（实现无关）：
```
n=3
value=42
```

下行转换若对象实际不是派生类，`static_cast` 不报错但行为未定义。需要安全下行转换用 `dynamic_cast`。

### 8.2 dynamic_cast 与 RTTI

`dynamic_cast` 专门用于**多态类型**（基类至少有一个虚函数）的下行或交叉转换。它在**运行期**做 RTTI 检查：能转就返回指针，不能转指针返回 `nullptr`、引用抛 `std::bad_cast`。它依赖虚函数表里的类型信息，所以要求基类有虚函数。

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <iostream>
#include <typeinfo>
#include <stdexcept>

struct Base { virtual ~Base() = default; virtual void who() { std::cout << "B\n"; } };
struct Derived : Base { void who() override { std::cout << "D\n"; } };

int main() {
    Derived d;
    Base* p = &d;

    // 指针形式：失败返回 nullptr
    Derived* ok = dynamic_cast<Derived*>(p);
    std::cout << "cast real Derived: " << (ok ? "ok" : "null") << '\n';   // ok

    Base b;                       // 一个真正的 Base（不是 Derived）
    Base* p2 = &b;
    Derived* fail = dynamic_cast<Derived*>(p2);
    std::cout << "cast real Base: " << (fail ? "ok" : "null") << '\n';    // null

    // 引用形式：失败抛 bad_cast
    try {
        Derived& ref = dynamic_cast<Derived&>(*p2);   // *p2 实际是 Base
        (void)ref;
    } catch (const std::bad_cast& e) {
        std::cout << "bad_cast caught: " << e.what() << '\n';
    }

    // typeid：RTTI 的另一面，用于查实际类型
    std::cout << "typeid(*p).name()=" << typeid(*p).name() << '\n';   // 派生类
    std::cout << "typeid(*p2).name()=" << typeid(*p2).name() << '\n'; // 基类
}
```

输出（64 位 g++，实现相关；name() 的格式是实现定义的，g++ 给 `7Derived`/`4Base`）：
```
cast real Derived: ok
cast real Base: null
bad_cast caught: std::bad_cast
typeid(*p).name()=7Derived
typeid(*p2).name()=4Base
```

注意 `typeid().name()` 返回的字符串格式是**实现定义**的：g++ 用 `7Derived`（前缀数字是名字长度），MSVC 给 `class Derived`。跨平台代码不能依赖它的具体内容，只能用 `typeid(a) == typeid(b)` 比较。`dynamic_cast` 频繁使用通常是设计信号——往往说明你的多态设计该改用虚函数，而不是上层用类型判断分支。

#### dynamic_cast 在运行期做了什么（进阶）

`dynamic_cast` 不是简单的类型比较，而是一次**遍历继承图的运行期搜索**。按主流的 Itanium C++ ABI（g++/clang）实现：

1. 从对象的 vptr 找到 vtable，vtable 前缀里有 `typeinfo` 指针（见第 6.3 节），据此拿到对象的**动态类型** `type_info`。
2. 同时用 vtable 前缀的 `offset_to_top` 把"指向当前子对象的指针"调整回**完整对象起点**。
3. 在动态类型的 `type_info` 里，沿基类链向上搜索目标类型：单继承下是线性向上找，多继承下是遍历一张"基类表"。`type_info` 按类的继承形态分为几类（无基类、单继承、多继承），各自的查找路径不同。
4. 找到则算出从完整对象到目标子对象的偏移、调整指针返回；找不到（目标类型不在该对象的基类链上）指针返回 `nullptr`、引用抛 `std::bad_cast`。

推论：

- `dynamic_cast` 只能用在有虚函数的类上——它依赖 vtable 里的 `typeinfo`，没有虚函数就没有 vtable 也就没有 RTTI。
- 复杂继承层级下 `dynamic_cast` 不便宜（是图搜索，最坏 O(继承链深度)），热路径上频繁 `dynamic_cast` 是性能信号，更说明该用虚函数而非运行期类型判断。
- `static_cast` 下行转换不做这些检查，所以更快但危险（错了是 UB）；需要安全才用 `dynamic_cast`。

理解了这一层，就能回答"为什么 `dynamic_cast` 要求基类有虚函数""它和 `static_cast` 下行转换的代价差在哪""为什么频繁 `dynamic_cast` 是设计气味"——这些都是面试追问点。

### 8.3 const_cast

`const_cast` 唯一的作用是**添加或移除 `const`/`volatile`**（不能做其他类型转换）。移除 const 本身编译期合法，但**如果原对象原本真是 const，修改它是未定义行为**。

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。注意：合法用例（原对象非真 const）安全；反例见注释。

```cpp
#include <iostream>

int main() {
    // 合法：原对象可变，只是通过 const 引用看到
    int x = 10;
    const int& cr = x;
    const_cast<int&>(cr) = 20;
    std::cout << "x=" << x << '\n';        // 20

    // 反例（UB，不要运行）：原对象是真 const
    // const int y = 5;
    // const_cast<int&>(y) = 99;   // 编译通过，但写只读内存是 UB：可能段错误，也可能"看似成功"
}
```

输出（实现无关）：
```
x=20
```

`const_cast` 真正合理的场景极少，主要是和 C 接口互操作（C 接口常把 `const char*` 写成 `char*`）。如果发现自己频繁用 `const_cast` 修改数据，通常说明 const 设计错了。

### 8.4 reinterpret_cast

`reinterpret_cast` 做**位模式重解释**：把一段内存按另一种类型看待，不实际转换值。它是最危险的转换，几乎总是平台相关、可移植性差。常见用途是 `void*` 和具体指针互转、整数和指针互转、不同指针类型重解释。

> **代码性质：反例片段。** 故意演示会触发严格别名违规的写法。`reinterpret_cast<float&>(bits)` 通过不相关类型的引用访问 `int`，是未定义行为——在某些优化级别下结果可能"看起来对"，换个编译选项就错。**不要照抄**。

```cpp
#include <iostream>

int main() {
    int bits = 0x40490FDB;                // 3.14159... 的 IEEE 754 位模式
    float f = reinterpret_cast<float&>(bits);  // UB：违反严格别名，把 int 当 float 读
    std::cout << "bits=" << bits << " as_float=" << f << '\n';  // 可能打印 3.14159，但这是 UB
}
```

输出（64 位 g++，-O0 下"看似正确"；**实现相关、优化级别相关**）：
```
bits=1078530011 as_float=3.14159
```

#### 严格别名规则（strict aliasing）

这是 `reinterpret_cast` 最隐蔽的坑。标准规定：**通过类型不兼容的指针/引用访问一个对象是未定义行为**。编译器被允许假设"不同类型的指针不会指向同一块内存"，并据此优化——一旦你真的让 `int*` 和 `float*` 指向同一对象并都解引用，优化后的结果不可预测。

关键例外——**字符类型别名例外**：通过 `char`、`unsigned char`、`std::byte` 的指针/引用访问**任意**对象的字节表示是合法的。这正是序列化、看对象内存布局时的正确做法。

另一常见误解——**用 `union` 做类型双关（type punning）在 C++ 中也是 UB**：先写 union 的一个成员、再读另一个成员，在 C99 起允许，但 C++ 不允许（读"非活跃成员"是未定义行为）。很多人从 C 带来的这个习惯在 C++ 里不成立。

#### 正确的类型重解释方式

要做安全的类型双关，有两种标准保证正确的方式：

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <iostream>
#include <cstring>

int main() {
    int bits = 0x40490FDB;

    // 方式一（C++11 起可用）：std::memcpy 拷贝对象表示
    float f;
    std::memcpy(&f, &bits, sizeof(f));   // 合法：拷贝字节，不违反别名
    std::cout << "memcpy: bits=" << bits << " as_float=" << f << '\n';  // 3.14159

    // 字符类型别名例外：合法地查看任意对象的字节
    int x = 0x41424344;
    unsigned char* p = reinterpret_cast<unsigned char*>(&x);
    std::cout << "bytes: " << std::hex
              << static_cast<int>(p[0]) << ' ' << static_cast<int>(p[1]) << ' '
              << static_cast<int>(p[2]) << ' ' << static_cast<int>(p[3])
              << std::dec << '\n';
}
```

输出（64 位 g++，小端；实现相关）：
```
memcpy: bits=1078530011 as_float=3.14159
bytes: 44 43 42 41
```

- **`std::memcpy`**：C++11 起就可用，是 type punning 的可移植做法。注意它不真的调用 C 库函数——编译器认识这个用法，会优化成寄存器间的位移动，没有函数调用开销。
- **`std::bit_cast`（C++20，`<bit>`）**：`constexpr`、类型安全，要求两个类型大小相等且都是 trivially copyable。是现代 C++ 的首选：`float f = std::bit_cast<float>(bits);`。编译期也能用。

两种都要求源、目标类型大小相同。`reinterpret_cast` 看似一行更短，但代价是 UB 风险——**新代码做类型双关一律用 `memcpy` 或 `std::bit_cast`，不用 `reinterpret_cast` 也不 union。**

### 8.5 C 风格转换 (T)x 的问题

C 风格 `(T)x` 或函数风格 `T(x)` 会依次尝试 `const_cast`、`static_cast`、`reinterpret_cast` 的组合，挑第一个能编译通过的。这看起来方便，实则危险：它可能悄悄做 `const_cast` + `reinterpret_cast` 的组合，把本该被编译器拒绝的危险转换蒙混过去。**C++ 代码应避免 C 风格转换**，改用命名转换，让转换意图和危险性都可见。

---

## 9. 对象模型进阶

把前几节的对象大小、对齐、虚表指针、继承布局综合起来，给出一个整体图景。

### 9.1 sizeof 的计算要点

计算一个类的 `sizeof` 时考虑：

1. 成员按声明顺序排列，各自占自己大小，并满足自身对齐（前面补填充）。
2. 含 `virtual` 函数的类加一个 vptr（64 位 +8）。
3. 单继承下，派生类把基类子对象放在前面，再加自己的成员。
4. 多继承下，每个有虚函数的基类各贡献一个 vptr，按基类声明顺序排列。
5. 菱形（非虚）继承下，被重复继承的基类会有多份子对象。
6. 对象总大小是**最大对齐要求**（通常是最大成员的对齐或 vptr 的对齐）的整数倍。

### 9.2 对齐与 #pragma pack

`#pragma pack(n)` 改变结构体的最大对齐为 `n`，用于与协议/硬件布局严格对齐。代价是访问未对齐成员可能变慢、某些平台直接报错。`alignas` 是 C++11 的标准化对齐控制，比 `#pragma pack` 更可移植。

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <iostream>

struct Default { char c; int i; double d; };

#pragma pack(push, 1)
struct Packed1 { char c; int i; double d; };
#pragma pack(pop)

int main() {
    std::cout << "Default  size=" << sizeof(Default)  << " align=" << alignof(Default)  << '\n';
    std::cout << "Packed1  size=" << sizeof(Packed1)  << " align=" << alignof(Packed1)  << '\n';
}
```

输出（64 位 g++，实现相关）：
```
Default  size=16 align=8
Packed1  size=13 align=1
```

`Default` 默认对齐 8：`char`(1) + 填充(3) + `int`(4) + `double`(8) = 16。`Packed1` 在 `pack(1)` 下无任何填充，按 1 字节对齐，`char`(1)+`int`(4)+`double`(8)=13。紧凑但访问 `i`、`d` 时可能不对齐。

### 9.3 菱形继承的内存布局

非虚菱形继承下，派生类持有共享基类的**多份**子对象：

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <iostream>

struct A { int a; };          // 4 字节
struct B1 : A { int b1; };   // A 子对象 + b1
struct B2 : A { int b2; };   // 另一份 A 子对象 + b2
struct D : B1, B2 { int d; }; // 两份 A

int main() {
    std::cout << "sizeof(D)=" << sizeof(D) << '\n';   // 两份 A 各 4 + b1,b2,d 各 4 = 20
    D obj;
    // obj.a = 1;          // 歧义：来自 B1 还是 B2？
    obj.B1::a = 1;          // 必须消歧
    obj.B2::a = 2;
    std::cout << "B1::a=" << obj.B1::a << " B2::a=" << obj.B2::a << '\n';
}
```

输出（64 位 g++，实现相关）：
```
sizeof(D)=20
B1::a=1 B2::a=2
```

`D` 里有**两份** `A`（一份来自 `B1`，一份来自 `B2`），所以 `sizeof` 是 20 而非 16。访问 `obj.a` 歧义，必须用 `obj.B1::a` / `obj.B2::a` 消歧。要让 `D` 只含一份 `A`，需要虚继承（`struct B1 : virtual A`），代价是引入虚基类偏移表、访问虚基类成员变慢。

### 9.4 内存分区与对象生命周期

面试常问"一个对象 `new` 出来放在哪、`static` 局部变量放在哪、字符串字面量放在哪"——这是 C++ 程序视角的内存分区。一个运行中的 C++ 程序，其内存大致分四区（具体布局由 OS 与链接器决定，**实现相关**，但概念上稳定）：

| 分区 | 存什么 | 生命周期 |
|---|---|---|
| **栈区** | 函数内局部变量、函数参数、返回地址 | 函数返回自动销毁，LIFO |
| **堆区** | `new`/`malloc` 分配的对象 | 手动管理，`delete`/`free` 才销毁，否则泄漏 |
| **静态/全局区** | 全局变量、`static` 局部变量、`static` 成员 | 程序启动到结束 |
| **常量/只读区** | 字符串字面量、`const` 全局常量（部分） | 程序生命周期，常不可写 |

> **代码区**（存放编译后的机器指令）有时算第五区，但对 C++ 程序员来说重点是上面四区。各区边界与具体地址由平台决定，**不要靠打印地址大小来判断"在哪个区"**——只看"变量怎么声明、何时销毁"。

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <iostream>

int g_global = 1;            // 静态/全局区（全局变量）
const char* g_str = "hello";// g_str 在静态区；"hello" 字面量在常量/只读区

void foo() {
    static int s = 2;        // 静态区：静态局部变量，首次调用初始化，程序结束才销毁
    int local = 3;          // 栈区：局部变量，foo 返回即销毁
    int* p = new int(4);    // new 出来的对象在堆区；p 这个指针本身在栈
    std::cout << g_global << ' ' << g_str << ' ' << s << ' ' << local << ' ' << *p << '\n';
    delete p;               // 堆对象需手动 delete，否则泄漏
}

int main() { foo(); }
```

输出（实现无关）：
```
1 hello 2 3 4
```

要点：`static` 局部变量虽在函数内声明，却在**静态区**，首次执行到时初始化、之后不再重新初始化、程序结束才销毁——这是单例懒初始化的底层依据。`new` 返回的对象在堆，其指针在栈；忘了 `delete` 就泄漏，这是 RAII（§3）要解决的。

#### new/delete vs malloc/free

`new`/`delete` 是 C++ 的，`malloc`/`free` 是 C 的，**不要混用**：

- `new` 会调用构造函数、`delete` 调用析构函数；`malloc`/`free` 只分配/释放原始字节，不构造/析构对象。对非平凡类型，`malloc` 一块内存当对象用是 UB（没构造就访问）。
- `new` 失败抛 `std::bad_alloc`（默认）、`malloc` 失败返回 `NULL`。
- `new[]`/`delete[]` 必须配对（数组形式），`new`/`delete` 配对（单对象）；混用是 UB。

```cpp
struct Widget { Widget(){ std::cout<<"ctor\n"; } ~Widget(){ std::cout<<"dtor\n"; } int x=0; };

Widget* w1 = new Widget;     // 调构造，堆上构造一个 Widget
delete w1;                   // 调析构，再释放内存

Widget* w2 = (Widget*)std::malloc(sizeof(Widget));  // 只分配字节，没调构造！直接用是 UB
std::free(w2);                                       // 只释放，没调析构
```

实际代码应优先用 RAII（`std::vector`、智能指针）而非裸 `new`/`malloc`，从源头避免配对错误和泄漏。

#### 野指针、悬空指针、double free、内存泄漏

这几个术语面试必问，区分清楚：

- **野指针（wild）**：指针未初始化就使用，指向未知内存。读它 UB、可能崩溃。
- **悬空指针（dangling）**：指针曾指向有效对象，但对象已销毁（如 `delete p` 后还用 `p`、或指向的局部变量已出作用域）。访问悬空指针是 UB——可能"看似正常"也可能崩，不可预测。
- **double free**：对同一块内存 `delete` 两次（常见于浅拷贝两个对象指向同一块）。UB，分配器可能崩溃。
- **内存泄漏**：`new` 了却永远不 `delete`（如丢失了唯一指向它的指针）。内存持续增长，长期运行程序（服务端）会被拖垮。

```cpp
int* p; *p = 1;                 // 野指针：p 未初始化，UB

int* q = new int(1);
delete q;
*q = 2;                         // 悬空：q 指向已释放内存，UB

int* r = new int(1);
int* r2 = r;
delete r; delete r2;            // double free：同一块释放两次，UB

void leak() { new int(1); }     // 泄漏：分配了却丢了指针，永远无法释放
```

RAII（智能指针/容器）让"获取即对象、析构即释放"，能同时杜绝悬空、double free、泄漏——这是现代 C++ 不手写 `new`/`delete` 的根本理由。检测工具：AddressSanitizer（`-fsanitize=address`）能抓 use-after-free/double-free/leak；Valgrind 也行。

#### 指针 vs 引用（面试必问）

| 维度 | 指针 `T*` | 引用 `T&` |
|---|---|---|
| 是否必须初始化 | 否（可为野指针） | **是**（定义时必须绑定对象） |
| 能否重绑定 | 能（指向别处） | 否（绑定后终身不变） |
| 可能为空 | 是（`nullptr`） | 否（引用须绑有效对象） |
| 是否有多级 | `T**`、`T***` 合法 | 引用的引用不存在（折叠见 §4.3） |
| 使用语法 | 解引用 `*p`、`p->` | 直接用 `r`（像对象本身） |

引用更安全（不可空、不可重绑、不可悬空到"野"），但引用也能悬空（绑的对象销毁后用引用是 UB）。函数参数传"不拥有的只读视图"用 `const T&`、传"可能为空或可重指"用 `T*`，是常见约定。

### 9.5 小结：对象模型的成本清单

- 普通类：成员大小 + 对齐填充，无额外开销。
- 有虚函数：每个对象 +1 个 vptr（64 位 +8），每次虚调用多一次间接跳转。
- 单继承：vptr 一份，基类子对象在前。
- 多继承：每个有虚函数的基类一份 vptr。
- 虚继承：额外的虚基偏移，访问虚基类成员略慢。
- 对齐填充：成员顺序影响填充量，紧凑排列可省空间。

记住这张成本表，面试问"含虚函数的类 `sizeof` 多少""多继承对象几个 vptr""菱形继承 sizeof 为什么偏大"都能答上来。

---

## 附录：参考资料

1. **C++ working draft**：https://eel.is/c++draft/ —— 语言规则原文。
2. **cppreference**：https://en.cppreference.com/w/ —— 虚函数、`dynamic_cast`、`sizeof`、对齐、[三五零原则](https://en.cppreference.com/cpp/language/rule_of_three)、[`std::bit_cast`](https://en.cppreference.com/cpp/numeric/bit_cast) 各词条。
3. **Itanium C++ ABI**：https://itanium-cxx-abi.github.io/cxx-abi/abi.html —— vtable 真实内存布局（`offset_to_top`、`typeinfo` 前缀）、RTTI 与 `dynamic_cast` 的实现约定（§6.3、§8.2 所据）。
4. **Arthur O'Dwyer，dynamic_cast from scratch**：https://itp.uni-frankfurt.de/~sciarra/C++_User_Group/Talks/2018_11.pdf —— 继承图、vtable、`dynamic_cast` 实现的可视化讲解（§8.2 进阶所据）。
5. **libcxxabi / libcxxrt `dynamic_cast.cc`** —— 主流运行库 `dynamic_cast` 的真实实现源码（§8.2 所据）。
6. **ISO C++ Core Guidelines**：https://isocpp.github.io/CppCoreGuidelines/ —— C.6 起：何时写 `virtual`、何时禁拷贝、何时用 `override`；C.21/C.67 多态类抑制拷贝（§3.4 所据）。
7. **Practical Type Punning in C++（Timur Doumler, CppCon 2019）** / cppreference `reinterpret_cast` —— 严格别名、`union` 类型双关的 UB、`std::bit_cast`/`memcpy` 正确做法（§8.4 所据）。
8. **Stanley B. Lippman 等，*C++ Primer*** —— 第 13/15 章拷贝控制与面向对象。
9. **Scott Meyers，*Effective C++*（第三版）** —— 条款 7 多态基类声明虚析构、条款 32-33 继承与名字隐藏。

使用参考资料时区分**标准保证的行为**（如空类 `sizeof` 为 1、`override` 是编译期检查、严格别名违规是 UB）、**实现行为**（如 vptr 大小、`typeid().name()` 字符串、`#pragma pack` 后的大小、vtable 放在 `.rodata`）和**设计建议**（如优先组合、Rule of Zero、多态类禁拷贝）。




