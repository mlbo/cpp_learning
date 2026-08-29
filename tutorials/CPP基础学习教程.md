# C++ 基础学习教程

> 本教程面向 C++ 初学者。第 0 章先补齐编译、基础语法、函数、容器、引用、指针和类的最小知识闭环，后续章节再系统学习现代 C++ 特性。

本篇统一使用 **C++17** 编译，以便把 C++11、C++14 和 C++17 的常用写法放在同一套可运行环境中。每个特性会标注它最早来自哪个标准；“使用 C++17 编译”不表示所有内容都是 C++17 才引入。

推荐顺序：第 0 章基础准备 → 第 1、2、6、8、10、11 章 → 智能指针 → Lambda → 移动语义 → 动态数组项目。第 12 章先学习简化版 DynamicArray，再回到 Week 1 Day 7 对照进阶版，并在第二周 Day 12 学习 RAII 时再次回访。第 0 章不是完整语法百科，而是进入 35 天主线前必须掌握的阅读工具；移动语义依赖引用、类和对象生命周期，不建议提前死记。

本教程承担“第一次系统讲解”。需要查询特性的标准版本、选型和仓库落点时，使用 [现代 C++ 特性学习地图](../Modern_CPP_Features_Deep_Dive.md)；需要继续推导规则、错误模式和工程例外时，进入 [Effective Modern C++ 教程](Effective_Modern_CPP教程.md)。Day README 不会再复制这里的完整章节，而会告诉你当天带着什么问题阅读、在哪个真实目标中验证。

阅读时始终区分三类陈述：语言或标准库必须保证的行为、某个实现或平台的常见行为、为了可维护性提出的设计建议。后两类不能反过来当作 C++ 标准规则。

---

## 目录

0. [零基础准备：先读懂并运行第一周代码](#0-零基础准备先读懂并运行第一周代码)
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

## 0. 零基础准备：先读懂并运行第一周代码

这一章解决的不是“背完全部 C++ 语法”，而是三个更实际的问题：

1. 一段 C++ 代码怎样变成可以运行的程序？
2. 第一周代码里的变量、函数、数组、引用和指针分别在表达什么？
3. 出错时应该先看哪里，而不是盲目复制一条新命令？

建议把本章示例亲手输入并运行。只阅读代码很容易产生“好像懂了”的错觉；能修改输入、预测输出、解释报错，才算真正建立了基础。

本教程给每个 C++ 围栏标明性质：“完整可运行程序”可以单独保存和编译；“上下文片段”要放进已有函数、类或工程后验证；“反例片段”故意保留错误或危险写法，只用于阅读诊断、受控测试或 Sanitizer 观察。亲手运行不等于把所有片段原样塞进同一个文件，先按标签选择正确的验证方式。

### 0.1 从源代码到可执行程序

先看一个最小程序：

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <iostream>

int main() {
    int answer = 40 + 2;
    std::cout << "answer = " << answer << '\n';
    return 0;
}
```

逐行理解：

- `#include <iostream>` 让程序能够使用标准输入输出工具。
- `int main()` 是程序入口。操作系统启动程序后，会从 `main` 开始执行。
- `{}` 包围函数体，也形成一个作用域。
- `int answer = 40 + 2;` 创建一个 `int` 变量，并用计算结果初始化它。
- `std::cout` 把内容写到标准输出；`'\n'` 表示换行。
- `return 0;` 表示程序正常结束。

在 Linux 或 macOS 终端中，可以这样编译和运行：

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -g hello.cpp -o hello
./hello
```

这两条命令完成的是不同工作：第一条把 `hello.cpp` 翻译并链接为可执行文件 `hello`，第二条才真正运行它。不要把“编译成功”和“程序结果正确”混为一谈。

C++ 程序大致经历四步：

```text
源文件 .cpp
   ↓ 预处理：展开 #include、处理宏
翻译单元
   ↓ 编译：检查语法和类型，生成目标文件
目标文件 .o
   ↓ 链接：把多个目标文件和库连接起来
可执行程序
   ↓ 运行
输出或运行时错误
```

因此错误也要分层判断：

- **编译错误**：括号遗漏、类型不匹配、变量未声明等，通常带有文件名和行号。
- **链接错误**：函数声明存在但没有实现，或实现文件没有参与构建，常见提示含 `undefined reference`。
- **运行时错误**：程序已经启动，但发生越界、空指针访问等问题。
- **逻辑错误**：程序能运行，却算出了错误答案；算法题最常见。

处理报错时，先看第一条与自己代码有关的诊断，再看对应行附近。后续很多报错可能只是第一处错误引发的连锁反应。

### 0.2 变量、类型、表达式与初始化

变量可以理解为“有类型、有名字的一块存储”。类型决定它能保存什么值、支持什么操作，以及大致需要多少空间。

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <string>

int main() {
    int count = 3;                 // 整数
    double price = 19.5;           // 浮点数
    bool finished = false;         // true 或 false
    char grade = 'A';              // 单个字符使用单引号
    std::string name = "C++";      // 字符串使用双引号

    const int days_per_week = 7;   // 初始化后不能再修改
    count = count + 1;              // 赋值：修改已存在的变量
}
```

要区分三个容易混淆的动作：

> **代码性质：上下文片段。** 它用于观察局部规则，缺少周边声明或 `main`，不能直接当作独立程序运行。

```cpp
int x;       // 声明并定义 x，但内置局部变量此时未初始化
int y = 10;  // 定义并初始化 y
y = 20;      // 给已经存在的 y 赋新值
```

读取未初始化的局部变量会产生未定义行为。初学阶段最稳妥的习惯是：**定义变量时立即给出有意义的初值**。

表达式会根据操作数类型决定运算方式：

> **代码性质：上下文片段。** 它用于观察局部规则，缺少周边声明或 `main`，不能直接当作独立程序运行。

```cpp
int a = 7 / 2;             // 3：两个操作数都是整数，小数部分被舍去
double b = 7 / 2;          // 3.0：先做整数除法，再转换为 double
double c = 7.0 / 2;        // 3.5：至少一个操作数是浮点数
bool larger = (c > 3.0);   // true：比较表达式的结果是 bool
```

这也是 `auto` 必须依赖初始化器的原因：编译器要从右侧表达式推导类型。`auto value;` 没有足够信息，无法通过编译。

### 0.3 条件、循环、作用域与边界

算法本质上是在控制“什么情况下做什么”和“重复做多少次”。第一周最常见的是 `if`、`for` 和 `while`。

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> numbers{3, -1, 4, 0};
    int positive_count = 0;

    for (int value : numbers) {
        if (value > 0) {
            ++positive_count;
        }
    }

    std::cout << positive_count << '\n';  // 2
}
```

这里的执行逻辑是：依次取出容器中的每个元素，把它复制到 `value`，满足 `value > 0` 时计数。循环变量只在循环体内有效，这就是作用域。

需要下标时，可以使用传统 `for` 循环：

> **代码性质：上下文片段。** 它用于观察局部规则，缺少周边声明或 `main`，不能直接当作独立程序运行。

```cpp
for (std::size_t i = 0; i < numbers.size(); ++i) {
    std::cout << numbers[i] << '\n';
}
```

请把条件读成“只要 `i` 仍是合法下标就继续”。长度为 `n` 的序列，合法下标是 `0` 到 `n - 1`，所以条件是 `i < n`，不是 `i <= n`。这种差一错误会贯穿数组、双指针、滑动窗口和二分查找。

`while` 更适合“循环次数事先不明确，但继续条件明确”的场景：

> **代码性质：上下文片段。** 它用于观察局部规则，缺少周边声明或 `main`，不能直接当作独立程序运行。

```cpp
int left = 0;
int right = 5;

while (left < right) {
    ++left;
}
```

写循环前先回答三个问题：

1. 哪些变量描述当前状态？
2. 什么条件下循环继续？
3. 每轮操作是否一定让循环接近结束？

第三点尤其重要；若状态不变化，程序可能陷入死循环。

### 0.4 函数、参数传递、重载与函数模板

函数把一个可复用步骤命名。看到函数时，先读它的接口：需要什么参数、会不会修改参数、返回什么。

> **代码性质：上下文片段。** 它用于观察局部规则，缺少周边声明或 `main`，不能直接当作独立程序运行。

```cpp
#include <vector>

int square(int value) {               // 按值传递：函数得到一份副本
    return value * value;
}

void add_one(int& value) {            // 非 const 引用：会修改调用者的对象
    ++value;
}

int sum(const std::vector<int>& nums) { // const 引用：不复制，也不允许修改
    int result = 0;
    for (int value : nums) {
        result += value;
    }
    return result;
}
```

三种常见参数形式的意图不同：

| 写法 | 是否复制实参 | 能否修改调用者对象 | 常见用途 |
|---|---:|---:|---|
| `T value` | 是 | 否 | 小对象，或函数需要自己的副本 |
| `T& value` | 否 | 是 | 明确需要修改实参 |
| `const T& value` | 否 | 否 | 只读访问较大对象 |

“引用不复制”并不等于永远优于按值传递。`int`、`double` 等小类型按值传递通常更简单；容器等较大对象才常用 `const T&` 避免复制。

#### 函数重载

同一作用域中可以存在同名、参数列表不同的函数：

> **代码性质：上下文片段。** 它用于观察局部规则，缺少周边声明或 `main`，不能直接当作独立程序运行。

```cpp
void print(int value);
void print(const char* text);

print(0);         // 选择 print(int)
print("hello");   // 选择 print(const char*)
```

编译器会根据实参类型选择最匹配的重载。Day 4 学习 `nullptr` 时会看到：旧式空指针常量 `0` 可能被当作整数，导致选择错误的函数；理解重载是理解这个问题的前提。

#### 函数模板

模板是在告诉编译器：“这套函数逻辑可以用于一类类型，请在实际调用时生成合适版本。”

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
template<typename T>
T maximum(T a, T b) {
    return a < b ? b : a;
}

int main() {
    int x = maximum(3, 5);          // T 推导为 int
    double y = maximum(2.5, 1.0);   // T 推导为 double
}
```

调用 `maximum(3, 5)` 时，编译器从两个实参推导出 `T` 是 `int`，然后实例化一个可用的 `maximum<int>`。这就是 Day 1 EMC++ Item 1 所讨论的“模板类型推导”的最小背景。现阶段先掌握“模板参数由实参推导”，随后再学习引用和 `const` 如何影响结果。

### 0.5 C 风格数组、`std::array` 与 `std::vector`

第一周的算法围绕连续序列展开。下面三种类型都能保存一组同类型元素，但它们表达的所有权和长度不同：

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <array>
#include <iostream>
#include <vector>

int main() {
    int raw[3]{10, 20, 30};                 // C 风格数组，长度是类型的一部分
    std::array<int, 3> fixed{10, 20, 30};   // 固定长度的标准库容器
    std::vector<int> dynamic{10, 20, 30};   // 可在运行时增长或缩小

    dynamic.push_back(40);                   // vector 现在有 4 个元素
    std::cout << raw[0] << ' ' << fixed[0] << ' ' << dynamic.size() << '\n';
}
```

| 特性 | C 风格数组 | `std::array` | `std::vector` |
|---|---|---|---|
| 长度 | 编译期固定 | 编译期固定 | 运行期可变 |
| 连续存储 | 是 | 是 | 是 |
| 可直接获得长度 | 容易在传参时丢失 | `size()` | `size()` |
| 支持赋值和常用容器接口 | 很有限 | 是 | 是 |
| 第一周用途 | 理解底层数组 | 固定长度数据 | 算法题的主要输入类型 |

`vector` 会管理自己的动态内存，但访问规则与数组类似：

> **代码性质：上下文片段。** 它用于观察局部规则，缺少周边声明或 `main`，不能直接当作独立程序运行。

```cpp
std::vector<int> values{10, 20, 30};
int first = values[0];       // 快，但不检查越界
int second = values.at(1);   // 检查越界，失败时抛出异常
```

`values[3]` 不会自动得到“空元素”；它越过了合法范围。`operator[]` 越界属于未定义行为，程序可能崩溃，也可能暂时看似正常。算法代码必须由循环条件保证下标合法。

连续存储意味着相邻元素通常位于相邻内存位置，因此可以 O(1) 随机访问，并具有较好的缓存局部性。Day 2 会进一步比较这些容器，Day 6 的二分查找也依赖这种高效随机访问。

### 0.6 引用、指针、`nullptr` 与对象生命周期

引用和指针都能间接访问另一个对象，但语义不同：

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <iostream>

int main() {
    int value = 42;

    int& reference = value;   // reference 是 value 的别名，必须绑定到有效对象
    int* pointer = &value;     // pointer 保存 value 的地址

    reference = 50;           // 修改 value
    *pointer = 60;             // 解引用 pointer，再修改 value
    pointer = nullptr;         // 指针可以明确表示“当前不指向对象”

    std::cout << value << '\n'; // 60
}
```

同一个符号会因上下文表达不同含义：

- 声明中的 `int& ref` 表示引用；表达式中的 `&value` 表示取地址。
- 声明中的 `int* ptr` 表示指针；表达式中的 `*ptr` 表示解引用。

解引用前必须保证指针不为空，并且仍指向一个活着的对象：

> **代码性质：上下文片段。** 它用于观察局部规则，缺少周边声明或 `main`，不能直接当作独立程序运行。

```cpp
if (pointer != nullptr) {
    std::cout << *pointer << '\n';
}
```

下面的函数返回了悬空指针，不能使用：

> **代码性质：反例片段。** 它故意展示编译错误或生命周期风险，只用于观察与解释，不要直接运行。

```cpp
int* wrong() {
    int local = 42;
    return &local;  // 错误：函数结束后 local 的生命周期结束
}
```

核心不是“地址看起来还在”，而是地址所代表的对象是否仍处于生命周期内。Day 4 会深入学习为什么用 `nullptr` 表示空指针；Day 7 的动态数组会第一次接触堆内存和资源生命周期。

### 0.7 类、构造析构与 RAII 预览

类把数据和维护数据的操作组织在一起。对象创建时调用构造函数，生命周期结束时调用析构函数。

> **代码性质：上下文片段。** 它用于观察局部规则，缺少周边声明或 `main`，不能直接当作独立程序运行。

```cpp
#include <cstddef>
#include <vector>

class NumberList {
public:
    explicit NumberList(std::size_t count)
        : values_(count, 0) {}

    void set(std::size_t index, int value) {
        values_.at(index) = value;
    }

    std::size_t size() const {
        return values_.size();
    }

private:
    std::vector<int> values_;
};
```

读这个类时可以按以下顺序：

1. `public` 是使用者可以调用的接口；`private` 是类自己维护的实现细节。
2. 构造函数与类同名，没有返回类型，用来建立有效初始状态。
3. `: values_(count, 0)` 是成员初始化列表，创建含 `count` 个零的 `vector`。
4. `size() const` 末尾的 `const` 表示该成员函数不修改对象的可观察状态。
5. `NumberList` 不需要手写析构函数，因为成员 `vector` 会自动释放自己的资源。

“让负责资源的对象在析构时自动释放资源”就是 RAII 的核心思想。资源不只指内存，也可以是文件、锁或网络连接。

Day 7 的 `DynamicArray` 会为了教学直接管理动态数组，因此会看到下面的配对：

> **代码性质：上下文片段。** 它用于观察局部规则，缺少周边声明或 `main`，不能直接当作独立程序运行。

```cpp
#include <cstddef>

void raw_array_example(std::size_t capacity) {
    int* data = new int[capacity];
    delete[] data;
}
```

如果拥有资源的类允许默认复制，两个对象可能错误地持有同一地址，最终重复释放。完整解决方案涉及析构、复制、移动和异常安全。第一周项目的目标是观察这些问题，不是鼓励业务代码到处手写 `new`；实际开发中应优先使用 `std::vector`、智能指针等 RAII 类型。

### 0.8 一套可执行的学习与排错方法

对每个示例使用“预测—运行—修改—解释”四步法：

1. **预测**：运行前写下输出，或写下你认为会出现的编译错误。
2. **运行**：使用课程提供的 CMake 或脚本验证，不只看答案。
3. **修改**：改变一个变量、边界或参数类型，再次运行。
4. **解释**：不用“编译器就是这样”搪塞，说明是哪条类型、作用域或生命周期规则导致结果。

遇到算法错误时，选择一个很小的输入，手工列出每轮状态。例如双指针可以记录：

| 轮次 | `left` | `right` | 当前值 | 为什么移动 |
|---:|---:|---:|---|---|
| 1 | 0 | 4 | … | … |

这种跟踪表比盯着完整代码更容易发现越界、漏解和错误的移动条件。

#### 第一周开始前的自测

如果还不能立刻回答，不必推迟课程；回到对应小节运行一次示例即可。

1. `int x = 3;` 与 `x = 3;` 分别在做什么？
2. 为什么长度为 `n` 的序列通常使用 `i < n`？
3. `T`、`T&`、`const T&` 作为函数参数时，调用者能观察到什么差别？
4. `vector` 的 `size()` 为 3 时，合法下标有哪些？
5. 空指针和悬空指针有什么区别？
6. 函数模板的类型参数在什么时候被推导？
7. 为什么 `vector` 成员通常不需要类自己手写 `delete`？

完成这些准备后，再进入 C++11 概览。后续章节会把这里建立的直觉升级成更精确的现代 C++ 规则。

---

## 1. C++11 新特性概述

### 1.1 为什么学习 C++11

C++11 是 C++ 语言的一次重大更新，于 2011 年正式发布，通常被视为现代 C++ 的起点。它引入的类型推导、统一初始化、智能指针、移动语义、Lambda 和标准并发库，不是彼此孤立的语法糖，而是在共同解决几类长期问题：减少重复类型、明确资源所有权、避免不安全的特殊值、降低不必要复制，并让常见并发操作进入标准库。

学习时不要按“新关键字列表”死记。每个特性都按同一组问题理解：旧写法有什么风险、编译器据什么规则处理、新写法付出什么代价、什么时候不该使用。后续章节会沿着这条线展开。

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

### 1.3 本课程环境

历史编译器对 C++11 的支持并不整齐，新手没有必要使用十年前的最低版本验证特性。推荐 GCC 11+、Clang 14+ 或 Visual Studio 2022，并统一启用 C++17：

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -g your_code.cpp -o app
./app
```

遇到示例无法编译时，先检查注释中的版本标签。例如 `std::make_unique` 和初始化捕获来自 C++14，结构化绑定和 `std::is_same_v` 来自 C++17。

---

## 2. 类型推导：auto 与 decltype

### 2.1 auto 关键字详解

`auto` 让编译器根据初始化表达式推导变量类型。它的价值不只是少打几个字，更重要的是让声明与表达式的真实类型保持一致。例如容器迭代器、Lambda 和泛型代码中的类型可能很长，甚至无法直接写出。

`auto` 不是动态类型。类型仍然在编译期确定，变量之后也不能随意改成另一种类型：

> **代码性质：上下文片段。** 它用于观察局部规则，缺少周边声明或 `main`，不能直接当作独立程序运行。

```cpp
auto value = 42;  // value 的类型在这里确定为 int
value = 100;      // 正确：仍然赋 int
// value = "text"; // 错误：int 变量不能改成 const char*
```

#### 基本用法

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <iostream>
#include <map>
#include <string>
#include <vector>

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

注意 `auto s = "hello"` 推导出的是 `const char*`，不是 `std::string`。如果想得到拥有字符串内容的对象，应写 `std::string s = "hello";`，或在启用相应字面量命名空间后使用字符串字面量后缀。

> 本示例用 `std::map`、迭代器（`::iterator`）只为演示"类型很长时 auto 的价值"，它们要等到 §11 才正式讲。若还没学到，只需记住：`data.begin()` 返回一个名字很长的迭代器类型，手写它很啰嗦，`auto` 让编译器替你写。后续"auto 使用建议"里出现的 Lambda（§5）和 `std::make_unique`（§3）同理——这些会在对应章节展开。

#### auto 的推导规则

> **代码性质：上下文片段。** 它用于观察局部规则，缺少周边声明或 `main`，不能直接当作独立程序运行。

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

这里“忽略引用”和“忽略顶层 `const`”只适用于按值形式的 `auto`。先分清两层 `const`：

> **代码性质：上下文片段。** 它用于观察局部规则，缺少周边声明或 `main`，不能直接当作独立程序运行。

```cpp
const int value = 42;
const int* pointer = &value;

auto a = value;    // int：value 自身的顶层 const 被去掉
auto b = pointer;  // const int*：指向对象的底层 const 被保留
auto& c = value;   // const int&：引用形式保留 const
```

花括号还要单独判断：

> **代码性质：上下文片段。** 它用于观察局部规则，缺少周边声明或 `main`，不能直接当作独立程序运行。

```cpp
auto x = {1, 2, 3};  // std::initializer_list<int>
auto y{1};           // C++17 中是 int
// auto z = {1, 2.0}; // 错误：无法为列表推导出统一元素类型
```

因此 `auto` 不是“把左边类型删掉”这么简单。先看声明形式是 `auto`、`auto&` 还是 `const auto&`，再看初始化表达式，最后检查花括号的特殊规则。

#### auto 使用建议

> **代码性质：上下文片段。** 它用于观察局部规则，缺少周边声明或 `main`，不能直接当作独立程序运行。

```cpp
// ✅ 推荐使用的场景
auto iter = container.begin();           // 迭代器类型复杂
auto lambda = [](int x) { return x*2; }; // lambda类型难写
auto ptr = std::make_unique<int>(42);    // 智能指针（make_unique 来自 C++14）

// ⚠️ 需要结合上下文判断的场景
auto x = 0;       // 类型清楚，但显式 int 也可以强调业务含义
auto y = func();  // 函数名若不能表达返回语义，应优先改进命名或补充类型信息

// ✅ 显式写法更好的情况
int count = 0;           // 清晰表明意图
double ratio = 0.5;      // 数值类型最好显式声明
```

### 2.2 decltype 关键字详解

`decltype` 用于取得实体或表达式的类型，通常不会求值。它在泛型返回类型、类型验证和需要精确保留引用时很有用。

#### 基本用法

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

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

`decltype` 最容易出错的地方是它有两套观察方式：

1. 若操作数是**没有额外括号的变量名或成员访问**，直接得到该实体声明时的类型。
2. 其他表达式根据值类别得到类型：左值表达式得到 `T&`，将亡值表达式得到 `T&&`，纯右值表达式得到 `T`。

所以 `decltype(x)` 是 `int`，而 `decltype((x))` 是 `int&`。额外括号没有改变运行结果，却让 `decltype` 从“查看变量声明”切换成“查看表达式类别”。

为什么 `(x)` 会变成左值？规则根因：**带括号的 `(x)` 是一个表达式，而表达式 `x`（一个具名变量）本身是左值**；只有"不加括号的名字"才走"查声明类型"的特殊路径。一旦你加了括号，它就成了普通表达式，`decltype` 就按值类别推导——左值表达式得 `T&`。记住这层区别：

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <iostream>
#include <type_traits>

using namespace std;
int g = 42;
int& f() { return g; }   // 返回 int&

int main() {
    int x = 10;
    static_assert(is_same_v<decltype(x),    int>,  "x：未加括号的名字 → 声明类型 int");
    static_assert(is_same_v<decltype((x)),  int&>, "(x)：左值表达式 → int&");
    static_assert(is_same_v<decltype(f()),  int&>, "f()：函数返回类型 int&");
    static_assert(is_same_v<decltype(42),   int>,  "42：纯右值 → int（非引用）");
    static_assert(is_same_v<decltype(x + 1),int>,  "x+1：算术结果是纯右值 → int");
    cout << "all static_asserts passed\n";
}
```

输出（实现无关）：
```
all static_asserts passed
```

三条速记：**① 未加括号的名字 → 查声明类型；② 加括号或任何表达式 → 按值类别（左值→`T&`，将亡值→`T&&`，纯右值→`T`）；③ 纯右值（字面量、算术结果、按值返回的函数调用）不产生引用。** 最常见的坑就是"想返回引用却用了普通 `auto`"——`auto` 会丢引用，要保留就得用 `decltype(auto)`（见下）。

#### decltype 与 auto 配合使用

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

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

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

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

`decltype(auto)` 会忠实保留引用，也因此更容易返回悬空引用。绝不能返回局部变量的引用：

> **代码性质：反例片段。** 它故意展示编译错误或生命周期风险，只用于观察与解释，不要直接运行。

```cpp
decltype(auto) dangerous() {
    int local = 42;
    return (local);  // 错误：推导为 int&，函数结束后引用悬空
}
```

使用它之前要先回答：“返回值引用的对象，在调用者使用时还活着吗？”如果没有保留引用的明确需求，普通 `auto` 返回通常更安全、更容易理解。

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

#### RAII 到底靠什么机制：析构与作用域绑定

`RAII`（Resource Acquisition Is Initialization）是个出了名的糟糕名字——它真正管的不是"获取"而是"释放"。更准确的名字是 **Scope-Bound Resource Management（作用域绑定资源管理）**：把资源的生命周期绑到一个对象上，对象构造时获取资源、析构时释放资源，而对象的析构由作用域结束（或异常展开）自动触发。你只要把资源装进 RAII 对象，就不再需要在每条路径上手写释放。

对比看为什么它安全——同样的"用一块内存"，裸资源要靠你记得 `delete`，异常一来就泄漏；RAII 把释放绑到析构，自动走：

```cpp
// 裸资源：必须记得在每条路径上释放，异常会让 delete 执行不到 → 泄漏
void riskyRaw() {
    int* p = new int(42);
    // 若这里抛异常，下面的 delete 永远执行不到
    delete p;
}

// RAII：资源生命周期绑定到对象 p，作用域结束(含异常)自动析构释放
void safeRAII() {
    auto p = std::make_unique<int>(42);
}   // ← p 离开作用域，析构自动 delete，即使上面抛异常也照走
```

关键机制有三：①**构造获取、析构释放**成对出现在一个类里（`fopen`/`fclose`、`lock`/`unlock`、`new`/`delete` 都可这样封装）；②**析构与作用域绑定**——局部对象出作用域自动析构，无需手写释放；③**异常安全**——栈展开时也会析构已构造的对象，所以即使中途抛异常，已获取的资源也不会泄漏。资源不限于内存：文件句柄、互斥锁、网络套接字、数据库连接都能用 RAII 管。智能指针只是 RAII 思想在内存上的应用；`std::lock_guard`（管锁）、`std::fstream`（管文件）也是 RAII。

### 3.2 unique_ptr：独占所有权

`unique_ptr` 拥有它所指向对象的独占所有权，不可复制，只能移动。

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

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

线程安全边界要说准确：不同 `shared_ptr` 对象共享同一控制块时，引用计数增减可安全并发；但并发修改同一个 `shared_ptr` 变量仍需同步，所指对象本身也不会因此自动线程安全。

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

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

#### 控制块：shared_ptr 真正在管什么

`shared_ptr` 不只持有一个对象指针，还持有一个**控制块（control block）**指针。控制块概念上保存：

```
Control Block
├── strong count   还有多少 shared_ptr 持有对象（归零 → 析构对象）
├── weak count     还有多少 weak_ptr 依赖这个控制块（归零 → 释放控制块本身）
├── deleter        类型擦除的删除器（默认是 delete，可自定义）
└── allocator      内存分配器等实现信息
```

强计数归零时调用删除器析构对象；**强、弱计数都归零时才释放控制块**。这条"弱计数也要归零"是理解 `weak_ptr` 和 `make_shared` 行为的关键。

#### make_shared 一次分配 vs `shared_ptr(new T)` 两次分配

创建 `shared_ptr` 有两种写法，内存布局不同：

```
方式一：std::shared_ptr<T> p(new T(args));   两次堆分配
  堆分配1: [   T 对象   ]      ← 你自己 new 出来的
  堆分配2: [ 控制块 ]            ← shared_ptr 构造时另开一块
  强计数归零：析构 T、释放分配1；弱计数再归零：释放分配2

方式二：std::make_shared<T>(args);           一次堆分配（控制块与对象合体）
  堆分配: [ 控制块 |  T 对象  ]   ← 一块内存里前段是控制块、后段是对象
  强计数归零：析构 T（但内存不还，因为和对象同块）；弱计数再归零：整块释放
```

`make_shared` 只分配一次、控制块与对象相邻（缓存局部性更好），但代价是：**只要还有一个 `weak_ptr` 指向它，整块内存（含已析构对象的存储空间）都不能释放**——因为控制块与对象同块，控制块要活到弱计数归零。而两次分配的写法里，对象内存在强计数归零时就还掉了，只剩小控制块等弱计数归零。

> **代码性质：上下文片段。** 展示两种创建方式的取舍，需放进可编译上下文。

```cpp
// 一次分配：更高效，但 weak_ptr 会拖住整块（含对象内存）到 weak 也归零
auto p1 = std::make_shared<BigObject>(args);

// 两次分配：弱引用先失效时，对象内存能更早还掉
std::shared_ptr<BigObject> p2(new BigObject(args));   // 对象与控制块分开
```

选择：默认用 `make_shared`（多数场景一次分配更优）；只有当对象很大、且会有长期存活的 `weak_ptr`（如缓存）时，才考虑两次分配以让对象内存早还。`make_shared` 还有异常安全优势：`f(std::shared_ptr<T>(new T), g())` 在 C++17 前可能因求值顺序泄漏（`new` 后、构造 `shared_ptr` 前 `g()` 抛异常则泄漏），`make_shared` 不存在这个窗口。

#### enable_shared_from_this：在对象内安全拿自己的 shared_ptr

若一个对象已被 `shared_ptr` 管理，它在成员函数里直接 `std::shared_ptr<T>(this)` 会**新建一个控制块**，导致两个控制块各自以为自己是最后所有者，最终 double delete。正确做法是继承 `std::enable_shared_from_this<T>` 并调 `shared_from_this()`，它会**加入已有控制块**（强计数 +1）而不是新建：

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <iostream>
#include <memory>

class Widget : public std::enable_shared_from_this<Widget> {
public:
    std::shared_ptr<Widget> getShared() {
        return shared_from_this();   // 安全：加入已有控制块
        // return std::shared_ptr<Widget>(this);  // 危险：新建控制块 → double delete
    }
};

int main() {
    auto w = std::make_shared<Widget>();
    auto w2 = w->getShared();          // w2 与 w 共享同一控制块
    std::cout << "use_count=" << w.use_count() << '\n';   // 2
}
```

输出（实现无关）：
```
use_count=2
```

前提是对象一开始就由 `shared_ptr` 管理（否则 `shared_from_this()` 会抛 `std::bad_weak_ptr` 异常，因为没有控制块可提升）。`weak_ptr` 打破循环引用、`enable_shared_from_this` 安全取自身句柄，背后都是同一个机制：**控制块只应创建一次**。

#### 自定义删除器与 unique_ptr 的对比

`shared_ptr` 的删除器被**类型擦除**存进控制块，所以**不影响 `shared_ptr<T>` 的静态类型**——同一类型可装不同删除器。而 `unique_ptr<T, Deleter>` 的删除器是**类型的一部分**，不同删除器就是不同类型：

> **代码性质：上下文片段。** 展示两种智能指针删除器的类型差异，需放进可编译上下文。

```cpp
// 删除器带 null 检查：fopen 失败返回 NULL，析构时对 NULL 调 fclose 是 UB，故须判空
auto file_closer = [](FILE* f){ if (f) std::fclose(f); };

std::shared_ptr<FILE> fp1(std::fopen("a", "r"), file_closer);          // 类型 shared_ptr<FILE>
std::shared_ptr<FILE> fp2(std::fopen("b", "r"), [](FILE* f){ if (f) std::fclose(f); });  // 仍 shared_ptr<FILE>，删除器被擦除
// fp1、fp2 同类型，可放进同一个 vector<std::shared_ptr<FILE>>

std::unique_ptr<FILE, decltype(&std::fclose)> up1(std::fopen("a", "r"), std::fclose);  // 类型 unique_ptr<FILE, int(*)(FILE*)>
// C++17 不允许 lambda 直接出现在 decltype 里（C++20 才允许），需先绑定到变量再用 decltype：
std::unique_ptr<FILE, decltype(file_closer)> up2(std::fopen("a", "r"), file_closer);   // 另一种类型，与 up1 不同
```

这是 `shared_ptr` 比 `unique_ptr` 在"异质资源管理"上更灵活的原因——代价是控制块的一次类型擦除开销。

> 更深入的控制块机制、双控制块 bug、`make_shared` 与线程安全边界，见 [Effective Modern C++ 教程 Item 19](Effective_Modern_CPP教程.md#item-19-使用-stdshared_ptr-管理共享所有权)。

### 3.4 weak_ptr：打破循环引用

`weak_ptr` 是一种不控制对象生命周期的智能指针，它指向一个由 `shared_ptr` 管理的对象。

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

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

### 4.1 理解值类别：先掌握可操作的判断方法

“左值就是有名字，右值就是没名字”只能当第一眼直觉，不能作为完整规则。例如，**有名字的右值引用变量在表达式中仍是左值**。初学阶段先记住：左值通常表示有稳定身份的对象；右值通常表示临时值或可被安全接管资源的值。到 EMC++ Item 23-25 再系统学习 lvalue、xvalue 和 prvalue。

> **代码性质：上下文片段。** 它用于观察局部规则，缺少周边声明或 `main`，不能直接当作独立程序运行。

```cpp
#include <iostream>
#include <string>
#include <utility>

void lvalueRvalueDemo() {
    int x = 10;        // x 是左值，10 是右值
    int y = x + 5;     // y 是左值，x+5 是右值
    
    // x 是左值表达式，可以取得它所代表对象的地址
    int* px = &x;      // OK: 左值可以取地址
    
    // x + 5 是临时计算结果，不能直接用内置取地址运算符取地址
    // int* ptemp = &(x + 5);  // 错误：右值不能取地址
    
    // 左值引用
    int& ref1 = x;           // OK: 左值引用绑定左值
    // int& ref2 = 10;       // 错误：左值引用不能绑定右值
    const int& ref3 = 10;    // OK: const左值引用可以绑定右值
    
    // 右值引用（C++11）
    int&& rref1 = 10;        // OK: 右值引用绑定右值
    int&& rref2 = x + 5;     // OK: 右值引用绑定临时值
    // int&& rref3 = x;      // 错误：右值引用不能绑定左值
    int&& rref3 = std::move(x);  // OK: std::move 将左值表达式转换为 xvalue
    
    std::string s1 = "Hello";
    std::string s2 = std::move(s1);  // 移动而非复制
    std::cout << "s1: '" << s1 << "'\n";  // s1 有效但状态未指定，不能假定为空
    std::cout << "s2: '" << s2 << "'\n";
}
```

### 4.2 移动构造函数与移动赋值运算符

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <iostream>
#include <algorithm>
#include <cstring>

class MyString {
public:
    // 默认构造
    MyString() : data_(nullptr), size_(0) {}
    
    // 构造函数
    MyString(const char* str) {
        size_ = std::strlen(str);
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

#### 为什么移动操作要标 noexcept：与 vector 扩容的联动

注意上面移动构造和移动赋值都标了 `noexcept`。这不是装饰，而是有实际后果的承诺。`std::vector` 在扩容（容量不够、要换大块搬元素）时，会检查元素类型的移动构造是否 `noexcept`：

- 移动构造**是 `noexcept`** → 用移动搬元素，O(n) 且廉价（只换指针）。
- 移动构造**不是 `noexcept`** 且类型可拷贝 → **退化为拷贝**搬元素，深复制每个对象。

原因是**强异常安全**：vector 要保证"扩容中途抛异常时原数据完好"。移动若抛异常，旧元素已被移走、回滚不回去；拷贝失败时旧元素完整保留、释放新内存即可回滚。标准库用一个 `std::move_if_noexcept` 按这条规则选移动还是拷贝：

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
    std::cout << "== NoNoexceptMove：扩容走拷贝 ==\n";
    std::vector<NoNoexceptMove> v; v.reserve(2);
    v.emplace_back(1); v.emplace_back(2);
    std::cout << "触发扩容:\n"; v.emplace_back(3);

    std::cout << "== NoexceptMove：扩容走移动 ==\n";
    std::vector<NoexceptMove> w; w.reserve(2);
    w.emplace_back(1); w.emplace_back(2);
    std::cout << "触发扩容:\n"; w.emplace_back(3);
}
```

输出（实现无关——标准要求的行为）：
```
== NoNoexceptMove：扩容走拷贝 ==
触发扩容:
  copy(id=1)
  copy(id=2)
== NoexceptMove：扩容走移动 ==
触发扩容:
  move(id=1)
  move(id=2)
```

仅差一个 `noexcept`，扩容搬运旧元素就从深拷贝变成移动。对持有堆内存的对象（如上面的 `MyString`），拷贝要深复制整块、移动只换指针，两者可能差上百倍。**所以资源类的移动构造/赋值必须标 `noexcept`，否则在容器里等于没写。** 一个常被忽略的细节：析构函数是特殊成员里**唯一**用户手写也默认隐式 `noexcept` 的；移动/拷贝构造赋值只有编译器生成或 `=default` 时才隐式推导 `noexcept`，你手写函数体默认就是可能抛出——所以上面 `MyString` 的移动操作要显式标。

> 这条机制的更完整推导（copy-and-swap 强异常保证、moved-from 契约）见 [类与对象核心教程 §3.3](C++类与对象核心教程.md#33-copy-and-swap-惯用法与强异常保证)。

### 4.3 std::move 与 std::forward

#### 先补一块：引用折叠与"转发引用"

下面 `wrapper(T&& arg)` 能同时接左值 `x` 和右值 `100`，但 `T&&` 长得就像右值引用——为什么能接左值？这要靠**引用折叠**。它是理解完美转发的关键，先在这里讲清。

C++ 禁止直接写"引用的引用"（`int& &` 不合法）。但模板推导时会出现：`template<typename T> void f(T&& arg)`，若实参是左值 `int x`，编译器会把 `T` 推成 `int&`，于是 `T&&` 变成 `int& &&`——这就是"引用的引用"。规则规定这种情况要**折叠**成单一引用：

| 推导出的组合 | 折叠结果 |
|---|---|
| `& &` | `&` |
| `&& &` | `&` |
| `& &&` | `&` |
| `&& &&` | `&&` |

口诀：**只要有左值引用 `&` 参与，就折成 `&`；只有全是右值引用 `&&` 才保留 `&&`。**

> **代码性质：上下文片段。** 用 static_assert 验证四条折叠规则，需放进可编译上下文。

```cpp
#include <type_traits>
using namespace std;

template<typename T> void f(T&& arg) {}

int main() {
    int x = 1;
    f(x);    // 传左值：T 推成 int&，T&& = int& && → 折叠成 int&  （所以能接左值！）
    f(100);  // 传右值：T 推成 int， T&& = int&&                （仍是右值引用）

    // 通过别名触发折叠（直接写 int& && 不合法，须先用 using 起别名）
    using A = int&;   static_assert(is_same_v<A&&,  int&>,   "& && → &");
    using B = int&&;  static_assert(is_same_v<B&&,  int&&>,  "&& && → &&");
    using C = int&;   static_assert(is_same_v<C&,   int&>,   "& & → &");
    using D = int&&;  static_assert(is_same_v<D&,   int&>,   "&& & → &");
}
```

所以 `T&&` 在模板推导语境下不是"右值引用"，而是能同时绑左值和右值的**转发引用**（forwarding reference，旧称 universal reference）——判定条件：`T` 是被推导的模板参数。形如 `int&&`（类型已定、无推导）才是普通右值引用。`auto&&` 也走同样规则（`auto` 被推导）。

折叠解决了"`T&&` 为何能接左值"，`std::forward` 则用它**把值类别原样传下去**：`std::forward<T>(arg)` 在 `T=int&` 时转成左值、`T=int` 时转成右值，从而让 `wrapper` 把调用者传入的左/右值性保留着传给 `process`。这就是完美转发。

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <iostream>
#include <utility>
#include <string>

// std::move: 无条件把表达式转换为 xvalue；它本身不执行移动
void moveDemo() {
    std::string str = "Hello";
    std::string moved = std::move(str);  // 允许移动构造接管 str 的资源
    
    std::cout << "Original: '" << str << "'\n";   // 有效但状态未指定；不要依赖具体内容
    std::cout << "Moved: '" << moved << "'\n";    // "Hello"
}

void process(int& x) {
    std::cout << "Lvalue: " << x << "\n";
}

void process(int&& x) {
    std::cout << "Rvalue: " << x << "\n";
}

// std::forward: 完美转发。被调用函数要在模板定义点之前可见。
template<typename T>
void wrapper(T&& arg) {
    process(std::forward<T>(arg));
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

#### 闭包本质：编译器生成的类（心智模型）

Lambda 看着像函数，本质是一个**编译器生成的、唯一的、未命名的类**（标准 [expr.prim.lambda.closure] 称为闭包类型）。捕获列表 = 该类的成员变量，函数体 = 该类的 `operator()`。所以一个 lambda 表达式产生一个闭包**对象**，调用它就是调它的 `operator()`。

以 `[counter]() mutable { return ++counter; }` 为例，它等价于一个这样的类：

```
auto c = [counter]() mutable { return ++counter; };
// 概念上等价于编译器生成的类（实际类型未命名、唯一）：
class __closure_1 {                  // 名字仅示意，每个 lambda 类型唯一
    int counter;                     // 值捕获 → 成员变量（副本）
public:
    __closure_1(int c) : counter(c) {}
    int operator()() /* 非 const，因为 mutable */ { return ++counter; }
};
// auto c = __closure_1{counter};   // 在 lambda 构造那一刻拷贝 counter 进闭包
```

理解了"闭包是对象、捕获是成员"，几个要点就自然了：

- **捕获发生在 lambda 构造的那一刻**，不是调用时。值捕获拷贝当时的值，之后外部变量改变不影响闭包里的副本；引用捕获只存引用，外部改变闭包也看得到。
- **`mutable` 的作用**：默认 `operator()` 是 `const` 的，不能修改值捕获的成员；加 `mutable` 后 `operator()` 变非 const，才能修改闭包内副本。注意它改的是**闭包里的副本**，不是外部原变量：

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <iostream>

int main() {
    int counter = 0;
    auto c = [counter]() mutable { return ++counter; };  // 值捕获副本
    std::cout << c() << ' ' << c() << ' ' << c() << '\n'; // 1 2 3：闭包内副本累加
    std::cout << "outer counter=" << counter << '\n';    // 0：外部原变量没动
}
```

输出（实现无关）：
```
1 2 3
outer counter=0
```

- **每个 lambda 是不同类型**：两个 lambda 即使长得一样，也是不同的闭包类型，不能互相赋值。所以多个 lambda 要统一存储时只能用 `std::function`（有类型擦除开销）或函数指针（无捕获时才能转）。
- **无捕获的 lambda 可转为函数指针**：因为没有成员变量，`operator()` 不依赖实例状态，能退化成普通函数指针。

### 5.2 捕获方式

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

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

#### 捕获的两大风险：引用悬空与 this 悬空

因为捕获发生在**构造时刻**而调用可能在很久之后，引用捕获和 `this` 捕获都可能让闭包在调用时访问已失效的对象——这是 Lambda 最隐蔽的 bug 来源。

- **引用捕获悬空**：`[&]` 或 `[&x]` 只存引用。若闭包比被引用的变量活得久（如存进成员、异步回调、`std::function` 容器），变量已销毁时再调用闭包，就是访问已失效内存（未定义行为）。
- **`this` 悬空**：在成员函数里写 `[=]`（或显式 `[this]`）并用到成员时，会**隐式捕获 `this`**（不是拷贝成员的值，而是存指向对象的指针）。若对象在闭包调用前销毁，`this` 悬空。**这是 `[=]` 在成员函数里的常见误解**——你以为值捕获了成员，其实捕获的是指针。（注意：空捕获 `[]` 不捕获 this，在成员函数里直接用成员会**编译错**"not captured"，而非悬空——区分清楚。）

> **代码性质：反例片段。** 故意展示 `this` 悬空。**不要照抄**。要观察未定义行为，可用 ASan。

```cpp
#include <functional>

struct Engine {
    int power = 100;
    std::function<int()> makeGetter() {
        // [=] 隐式捕获 this，而非拷贝 power 的值
        return [=]() { return power; };   // 存的是 this 指针
    }
};

// Engine e;
// auto f = e.makeGetter();
// { Engine e2; f = e2.makeGetter(); }   // e2 析构后 f 内部的 this 悬空
// f();  // UB：访问已销毁对象的成员
```

修复：要么保证对象生命周期覆盖闭包调用；要么**把需要的值显式拷进闭包**，避免捕获 `this`。C++14 起可用初始化捕获 `[power = power]`（拷一份当前值，不依赖对象存活）或 `[p = power]`；C++11 里可先拷到局部变量再值捕获。这也解释了为什么 Core Guidelines F.53 建议"不要在会被非局部使用的 lambda 里用引用捕获"。

### 5.3 Lambda 实战应用

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

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
    
    // 5. 递归 lambda：借助 std::function（C++11 即可）
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

花括号提供了一套覆盖内置类型、数组、聚合类型、类对象和容器的初始化语法，同时禁止许多可能丢失信息的窄化转换。但“统一语法”不表示背后永远调用同一种机制：

- 聚合类型按成员顺序初始化。
- 类类型会参与构造函数重载决议。
- 若类存在可匹配的 `std::initializer_list` 构造函数，花括号通常会优先考虑它。
- 内置数值类型会检查窄化转换。

因此选择 `{}` 前既要看到它的安全性，也要检查目标类型有哪些构造函数。

#### most vexing parse：`()` 被解析成函数声明

`{}` 能规避 C++ 一个老坑：`Widget w();` 在能被解析为函数声明时，**会被解析成函数声明**，而不是"默认构造对象"。这条规则叫 most vexing parse。`{}` 不受其害：

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行（g++ 会给 `-Wvexing-parse` 警告提示 `w1` 被当成函数）。

```cpp
#include <iostream>

struct Widget {
    Widget() { std::cout << "default ctor\n"; }
};

int main() {
    Widget w1();      // ⚠ 不是对象：被解析成"返回 Widget、无参的函数 w1"的声明
    Widget w2{};     // ✅ 对象：调用默认构造
    Widget w3;       // ✅ 对象：默认构造
    std::cout << "w2、w3 是对象；w1 是函数声明（most vexing parse）\n";
}
```

输出（实现无关；编译时 `w1` 行触发 `-Wvexing-parse` 警告）：
```
default ctor
default ctor
w2、w3 是对象；w1 是函数声明（most vexing parse）
```

`Widget w1();` 本意是"用 `()` 默认构造"，但语法上 `w1` 完全像一个函数声明（函数名 `w1`、无参、返回 `Widget`），C++ 规定"能解析为声明就解析为声明"，于是它成了函数。规避：用 `{}`（`Widget w2{};`）或直接 `Widget w3;`。带临时实参时同理，`std::vector<int> v(std::istream_iterator<int>(is), std::istream_iterator<int>());` 也会被误解析成一个函数声明，改用 `{}` 即可。**结论：想默认构造或对临时实参初始化时，优先 `{}`，可天然避开 most vexing parse。**

#### `{}` vs `=` vs `()` 的系统对照

| 写法 | 默认构造 | 单值构造 | 多值/聚合 | initializer_list 优先？ | most-vexing-parse 风险 |
|---|---|---|---|---|---|
| `T x{...}` | `T x{}` ✅ | `T x{5}` ✅ | `T x{1,2,3}` ✅ | 是（若类型有 init_list 构造） | 无 ✅ |
| `T x(...)` | `T x()` ⚠ 被解析成函数声明 | `T x(5)` ✅ | `T x(1,2,3)` ✅ | 否 | 有 ⚠ |
| `T x = ...` | `T x = T()` ✅ | `T x = 5` ✅（若 `5` 可隐式转） | 不支持聚合多值 | 否（显式构造） | 无 ✅ |

记忆要点：`{}` 最安全（无 vexing parse、禁窄化），但当类型有 `initializer_list` 构造时要小心它会"抢"走看似该匹配非 init_list 构造的调用（如 `std::vector<int> v{5, 3}` 得到的是含 `5,3` 两元素而非容量 5、初值 3）；`()` 最灵活但有 vexing parse 风险；`=` 只支持单值且要求隐式转换可及。先看类型有哪些构造函数，再选写法。

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

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
    // int narrow{3.14};  // 标准要求编译失败，不只是普通警告
}

int main() {
    uniformInitDemo();
    return 0;
}
```

### 6.2 initializer_list

`std::initializer_list<T>` 是一个轻量只读视图，指向编译器为花括号元素准备的一段临时数组。复制 `initializer_list` 本身通常只复制指针和长度，不会复制全部元素；通过它访问到的元素是 `const T`，不能原地修改。

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

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

它最需要警惕的是构造函数选择。下面两个 `vector` 看起来相似，含义完全不同：

> **代码性质：上下文片段。** 它用于观察局部规则，缺少周边声明或 `main`，不能直接当作独立程序运行。

```cpp
#include <vector>

std::vector<int> a(10, 20); // 10 个元素，每个都是 20
std::vector<int> b{10, 20}; // 2 个元素：10 和 20
```

原因是 `{10, 20}` 会优先匹配 `initializer_list<int>` 构造函数。若某个类同时提供普通构造函数和 `initializer_list` 构造函数，不要只凭括号外观猜结果，要查看候选构造函数。

另一个边界是生命周期：可以在接收 `initializer_list` 的函数调用期间遍历它，但不要把 `begin()` 得到的裸指针保存起来供以后使用；底层临时数组结束生命周期后，该指针会悬空。

---

## 7. constexpr：编译期计算

`constexpr` 表示“这个实体可以参与常量表达式”，但不能简单理解为“函数一定在编译期运行”。同一个 `constexpr` 函数，用常量实参并放在必须常量求值的位置时会在编译期求值；用运行时输入调用时，仍可以在运行期执行。

### 7.1 constexpr 变量

> **代码性质：上下文片段。** 它用于观察局部规则，缺少周边声明或 `main`，不能直接当作独立程序运行。

```cpp
#include <array>
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

`const` 和 `constexpr` 解决的问题不同：

> **代码性质：上下文片段。** 它用于观察局部规则，缺少周边声明或 `main`，不能直接当作独立程序运行。

```cpp
int runtime_value();

const int a = runtime_value(); // a 初始化后只读，但值可能到运行时才知道
constexpr int b = 42;          // b 必须由常量表达式初始化
```

所有 `constexpr` 对象都是 `const` 的，但并非所有 `const` 对象都能用于数组长度、模板实参或 `static_assert`。

### 7.2 constexpr 函数

`constexpr` 函数可在编译期被求值（实参都是常量时），也可在运行期被普通调用。但"能在编译期求值"对函数体有严格限制，且这些限制随标准演进在放宽——这是初学最容易踩的坑。

#### C++11 对 constexpr 函数体的限制

C++11 的 `constexpr` 函数体只能包含：null 语句、`typedef`/`using` 声明，以及**恰好一个 `return` 语句**。不能有 `if`/`for`/`while`，不能声明局部变量。原因不是"语法偏好"，而是标准要保证 `constexpr` 函数是**无副作用、可证明终止**的纯函数：允许可变局部变量加循环加分支会引入停机问题，编译器无法判断能否终止；要求单表达式则天然无副作用（cppreference、ISO C++11 [dcl.constexpr]）。

> **代码性质：反例片段。** 故意展示在 C++11 下不合法的写法。**不要照抄**。

```cpp
// ❌ C++11 不合法：两个 return 语句 + if 分支
constexpr int fibonacci_bad(int n) {
    if (n <= 1) return n;
    return fibonacci_bad(n - 1) + fibonacci_bad(n - 2);
}
// 编译报错：body of constexpr function 'fibonacci_bad(int)' not a return-statement
```

同一逻辑要写成 C++11 合法，必须用单个三元 `return` + 递归（用条件表达式表达分支，用递归表达循环）：

> **代码性质：上下文片段。** 它用于观察局部规则，缺少周边声明或 `main`，不能直接当作独立程序运行。

```cpp
#include <array>
#include <iostream>
#include <stdexcept>

// ✅ C++11 合法：恰好一个 return 语句，分支用三元、循环用递归
constexpr int factorial(int n) {
    return (0 <= n && n <= 12)
        ? (n <= 1 ? 1 : n * factorial(n - 1))
        : throw std::out_of_range("factorial(int) requires 0 <= n <= 12");
}

constexpr int fibonacci(int n) {
    return n <= 1 ? n : fibonacci(n - 1) + fibonacci(n - 2);
}

// ❌ C++11 不合法、C++14 起才合法：局部变量 + for 循环
constexpr int sum(int n) {
    int result = 0;                       // 局部变量：C++11 禁止，C++14 起允许
    for (int i = 1; i <= n; ++i) {        // 循环：C++11 禁止，C++14 起允许
        result += i;
    }
    return result;
}
```

`factorial` 把契约写进表达式：负数无意义、`13!` 超过 `int` 上限，所以越界时 `throw`（`throw` 在常量表达式里使该次编译期求值失败，但运行时调用照常抛异常）。`sum` 注释里标记了它依赖 C++14 放宽的限制；本教程用 C++17 编译能通过，但若改用 `-std=c++11` 编译，`sum` 会和 `fibonacci_bad` 一样报 "not a return-statement" 错误。**判断一条 `constexpr` 写法在哪个标准合法，看它是否违反 C++11 的"单 return、无局部变量、无循环分支"约束。**

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <array>
#include <iostream>

constexpr int factorial(int n) {
    return (0 <= n && n <= 12)
        ? (n <= 1 ? 1 : n * factorial(n - 1))
        : throw std::out_of_range("factorial(int) requires 0 <= n <= 12");
}
constexpr int fibonacci(int n) {
    return n <= 1 ? n : fibonacci(n - 1) + fibonacci(n - 2);
}

void constExprFuncDemo() {
    // 编译期计算
    constexpr int fact5 = factorial(5);   // 编译期计算为 120
    constexpr int fib10 = fibonacci(10);  // 编译期计算为 55

    // 用于编译期需求
    int arr[factorial(5)];              // 大小为 120 的数组
    std::array<int, factorial(5)> stdArr;

    std::cout << "5! = " << fact5 << "\n";
    std::cout << "fib(10) = " << fib10 << "\n";
}

int main() {
    constExprFuncDemo();
    return 0;
}
```

输出（实现无关）：
```
5! = 120
fib(10) = 55
```

`constexpr int fact5 = factorial(5);` 要求常量表达式，因此必须在编译期得到结果。注意 `constexpr` 给函数增加"可在编译期使用"的能力，并不禁止普通运行时调用：`factorial(n)` 若 `n` 来自运行时输入，就退化为普通运行时调用。递归版 Fibonacci 只用于教学，它会重复计算大量子问题，不适合较大的运行时输入。

#### C++14 起放宽了什么

C++14 放宽了 `constexpr` 函数体：允许局部变量、循环（`for`/`while`）、`if`/`switch` 分支、多 `return`。所以 `sum`（循环+局部变量）在 C++14+ 合法。代价是 `constexpr` 函数仍须能在常量表达式语境终止，且不能有副作用（局部变量不能在运行期逃逸）。C++17 又加了 `if constexpr`（编译期选择代码分支，见 7.4）。记忆要点：**C++11 的 constexpr 函数本质是"一个表达式"，C++14 起才接近"普通函数但满足编译期约束"。**

### 7.3 constexpr 类

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

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

### 7.4 `if constexpr`：编译期选择代码分支（C++17）

普通 `if` 会在运行时选择分支，两个分支都必须是可编译的。`if constexpr` 的条件在编译期确定，未选中的分支不会对当前模板实例产生通常的编译要求。

> **代码性质：上下文片段。** 它用于观察局部规则，缺少周边声明或 `main`，不能直接当作独立程序运行。

```cpp
#include <iostream>
#include <type_traits>

template<typename T>
void describe(const T& value) {
    if constexpr (std::is_integral_v<T>) {
        std::cout << value << " is an integer\n";
    } else {
        std::cout << value << " is not an integer\n";
    }
}
```

它主要服务于泛型代码，不是“更快的普通 `if`”。条件若依赖用户输入，就不能使用 `if constexpr`。

---

## 8. nullptr：空指针的新写法

### 8.1 nullptr vs NULL

空指针表示“当前不指向任何对象或函数”。旧代码常用 `0` 或 `NULL`；问题在于 `0` 本来就是整数，而 `NULL` 的具体宏定义由实现决定，可能仍表现为整型空指针常量。遇到函数重载或模板推导时，这种双重身份会丢失“我想传指针”的意图。

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

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
    // func(NULL);  // 结果依实现而异：可能歧义，也可能选择整数重载
    func(0);        // 明确选择 func(int)，不是指针重载
    
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
    
    // 指针可在布尔上下文中检查；直接表达判断意图更清楚
    bool is_empty = (pi == nullptr);
    if (!pi) {
        std::cout << "pi is empty: " << is_empty << "\n";
    }
}

int main() {
    nullptrDemo();
    return 0;
}
```

`nullptr` 的类型是 `std::nullptr_t`，它可以转换为任意对象指针或函数指针类型，却不能像整数一样参与算术。正因为类型信息被保留，重载决议能选择 `func(int*)`。

模板中差别更明显：

> **代码性质：上下文片段。** 它用于观察局部规则，缺少周边声明或 `main`，不能直接当作独立程序运行。

```cpp
template<typename T>
void inspect(T value);

inspect(0);        // T 是 int
inspect(nullptr);  // T 是 std::nullptr_t
```

即使某个被调用函数最终需要指针，模板看到 `0` 时也只会推导出整数。现代 C++ 新代码应直接使用 `nullptr`；只有与要求整数空指针常量的旧式接口交互时，才需要单独处理兼容问题。

---

## 9. 强类型枚举 enum class

### 9.1 enum class vs 传统 enum

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

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

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <iostream>
#include <vector>
#include <map>
#include <array>
#include <string>

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
    // ↑ [name, score] 是 C++17 结构化绑定：把 map 的每个元素(键值对)拆成 name 和 score 两个变量，
    //   省去写 it->first / it->second。这里只需知道它"拆键值对"，语法细节可暂不深究。
    
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

#### 范围 for 的脱糖：begin/end 与 ADL 钩子

`for (auto x : range) body` 看着像新语法，实际是编译期"糖"，脱糖成等价的迭代器循环（C++17 精确形式）：

```
for (auto x : range) body
        ↓ 脱糖
{
    auto&& __range = range;                    // 转发引用持有范围，延长临时对象寿命
    auto __begin = begin(__range);             // 非成员 begin()，经 ADL 查找
    auto __end   = end(__range);               // 非成员 end()
    for (; __begin != __end; ++__begin) {
        auto x = *__begin;                     // 或 auto& x / const auto& x，按声明形式
        body;
    }
}
```

两个要点：

- **用 `auto&& __range` 持有范围**：转发引用既能绑左值也能绑右值，从而延长临时容器（如函数返回的 `vector`）的寿命到整个循环结束。所以 `for (auto x : makeVector())` 是安全的，不会悬空。
- **用非成员 `begin(range)`/`end(range)`**：这给了**自定义类型参与的钩子**。你的类只要在自身命名空间定义自由函数 `begin(c)`/`end(c)`（或成员 `begin()`/`end()`），就能用范围 for；数组类型则由标准库的 `std::begin(arr)`/`std::end(arr)` 处理。这就是范围 for 能用于自定义容器的机制——它不是只认 `std::vector` 等标准容器。

#### 陷阱：vector<bool> 的代理迭代器

`std::vector<bool>` 是 `vector` 的特化，它为了省空间把 `bool` 紧凑成位。后果：`*it` 返回的不是 `bool&`，而是一个**代理对象** `std::vector<bool>::reference`（临时表达右值）。于是 `for (auto& x : vec_bool)` 会编译失败——代理是右值，不能绑到非 const 左值引用 `bool&`：

> **代码性质：反例片段。** 展示 `vector<bool>` 代理迭代器陷阱。**不要照抄**。

```cpp
#include <vector>
std::vector<bool> flags = {true, false, true};

// ❌ 编译错误：*it 是 vector<bool>::reference（右值），不能绑到 bool&
// for (bool& x : flags) { x = false; }

// ⚠ 注意：auto 在 vector<bool> 上推成的是 vector<bool>::reference（代理），不是 bool！
// for (auto x : flags)  // x 是 reference，对 x 赋值会写回 vector（与直觉相反）
// 要拿到真正的 bool 值副本，必须显式写类型：
for (bool x : flags) { (void)x; }      // x 是真正的 bool 副本，赋值不影响 vector

// ✅ 要原地改用代理的赋值（flags[i] = ... 通过代理写回）
flags[0] = false;       // operator[] 返回 reference，赋值经代理写到位
```

这是 `vector<bool>` 唯独不满足"普通容器迭代器解引用得到 `T&`"的著名反例，也是范围 for 的典型意外。教训：**遍历 `vector<bool>` 要取真值时用 `bool x`（不要用 `auto`——它会推成代理 reference）**；若需要可写访问，用下标 `flags[i]`。更稳的做法是改用 `std::vector<char>` 或 `std::deque<bool>`，它们返回真正的引用。

---

## 11. 标准容器增强

### 11.1 新容器

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <iostream>
#include <array>
#include <forward_list>
#include <string>
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

“平均 O(1)”不是无条件保证。哈希函数分布较好、负载因子受控时，查找通常接近常数时间；大量键落入同一桶时，最坏情况可退化到 O(n)。插入还可能触发 `rehash`，使已有迭代器失效。另一个常见陷阱是 `hashMap[key]`：键不存在时它会插入一个默认值；只想查询时应使用 `find` 或 C++20 的 `contains`。

### 11.2 容器新方法

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <iostream>
#include <map>
#include <string>
#include <vector>

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

`shrink_to_fit()` 只是一个非强制请求，标准库实现可以不缩减容量。因此教学代码只能观察调用前后的容量，不能断言调用后一定满足 `capacity() == size()`。另外，`vector` 扩容会让指向原元素的指针、引用和迭代器失效；保存 `begin()` 或 `&vec[0]` 后再 `push_back`，必须重新取得位置。

#### emplace vs push_back：原地构造省一次临时对象

`push_back(x)` 需要一个已存在的 `x`，再把它拷贝/移动进容器——所以路径上至少有一个临时对象（你构造的或编译器构造的），加一次移动/拷贝。`emplace_back(args...)` 把 `args` **直接转发给容器槽位上的构造函数**，原地构造，省掉这个临时对象和那次移动：

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <iostream>
#include <vector>
#include <string>

struct Widget {
    Widget(int a, const std::string& b) : a_(a), b_(b) { std::cout << "ctor\n"; }
    Widget(const Widget& o) : a_(o.a_), b_(o.b_) { std::cout << "copy\n"; }
    Widget(Widget&& o) noexcept : a_(o.a_), b_(std::move(o.b_)) { std::cout << "move\n"; }
    int a_; std::string b_;
};

int main() {
    std::cout << "== push_back：先构造临时，再移动进容器 ==\n";
    std::vector<Widget> v; v.reserve(4);
    v.push_back(Widget(1, "x"));      // ctor（临时）+ move（进容器）

    std::cout << "== emplace_back：直接在容器槽位构造，无临时对象 ==\n";
    std::vector<Widget> w; w.reserve(4);
    w.emplace_back(1, "x");           // 仅 ctor（在槽位）
}
```

输出（实现无关）：
```
== push_back：先构造临时，再移动进容器 ==
ctor
move
== emplace_back：直接在容器槽位构造，无临时对象 ==
ctor
```

`push_back` 路径是 ctor + move 两次，`emplace_back` 只有一次 ctor。对持资源的类型（如含 `std::string`），省下的那次 move 虽然通常廉价（指针交换），但对不可移动或构造昂贵的类型收益明显。注意 `emplace` 也可能退化为转换：若传入的类型不能直接构造成元素、需要一次显式转换，`emplace` 仍会构造一个临时——此时和 `push_back` 差别不大。**一般原则：能 `emplace` 就 `emplace`，但可读性差时 `push_back` 也无妨。**

#### 迭代器失效规则一览

容器操作是否让"已持有的迭代器/指针/引用"失效，是面试必背、且实战最易踩的规则。下表按"插入"和"删除"分列（更全的底层原理见 [STL 容器底层原理](STL容器底层原理.md#5-迭代器失效规则一览表)）：

| 容器 | 插入失效 | 删除失效 |
|---|---|---|
| `vector` | 扩容时全部失效；不扩容时插入点之后失效 | 删除点之后失效 |
| `deque` | 任何插入全部失效 | 中间删除全部失效；端部删除只失效端 |
| `list` / `forward_list` | 不失效 | 只失效被删元素 |
| `map` / `set` / `multimap` / `multiset` | 不失效 | 只失效被删元素 |
| `unordered_map` / `unordered_set` | rehash 时全部失效；否则不失效 | 只失效被删元素 |

记忆：**节点式容器**（`list`/`map`/`set`）增删不影响其他节点，最安全；**连续存储**（`vector`）要搬元素，插入点/删除点之后都失效，扩容更全军覆没；`deque` 最脆弱，任何插入全失效；`unordered_*` 平时像节点式，但 rehash 时全失效。

边遍历边删除时，必须用 `it = c.erase(it);` 接住返回的下一个迭代器，不能 `c.erase(it); ++it;`——后者对已失效迭代器自增是未定义行为。这一条对所有容器都适用。

---

<a id="12-实战项目动态数组类"></a>

## 12. 实战项目：动态数组类

这里保留的是**第一阶段简化版**，目的是先看清所有权、五类特殊成员函数和“成功后再替换旧资源”的扩容思路。完成本章后，再学习 [Week 1 Day 7 进阶版](../week_01/day_07/README.md#综合项目dynamicarray类)：进阶版使用原始存储和 placement new，只让 `[0, size_)` 中的元素处于生命周期内。

两版最容易混淆的是 `pop_back()` 与 `clear()`。本章的 `new T[capacity]` 已构造容量内所有 `T`，所以这两个函数只改变逻辑长度，对象要等整块数组释放时才统一析构；Day 7 版会立即析构移除的元素。先运行本章完整程序理解简化模型，再运行 Day 7 的 `TestDynamicArray`（其中 `LifetimeProbe` 会验证立即析构），最后在第二周 Day 12 用 RAII 视角解释这种差异。

### 12.1 教学版完整实现

这个项目用来串起资源所有权、拷贝、移动、扩容和异常安全。它不是 `std::vector` 的替代品：为了让第一版代码仍然可读，下面的 `T` 需要能够默认构造、复制构造、复制赋值和移动赋值，而且 `pop_back()`、`clear()` 只改变逻辑长度，元素会在整块数组释放时统一析构。真正的 `vector` 会使用分配器分别管理“原始存储”和“已构造对象”，实现复杂得多。

先明确类始终要维护的三个不变量：

1. `0 <= size_ <= capacity_`。
2. `capacity_ == 0` 时 `data_ == nullptr`；容量大于零时 `data_` 指向 `new[]` 得到的数组。
3. 只有当前对象拥有 `data_`，析构时恰好执行一次 `delete[]`。

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
    // 默认构造
    DynamicArray() = default;
    
    // 带大小构造
    explicit DynamicArray(std::size_t size)
        : data_(allocate(size)), size_(size), capacity_(size) {}
    
    // 初始化列表构造
    DynamicArray(std::initializer_list<T> init)
        : data_(copy_from(init)),
          size_(init.size()),
          capacity_(init.size()) {}
    
    // 拷贝构造
    DynamicArray(const DynamicArray& other)
        : data_(clone(other)),
          size_(other.size_),
          capacity_(other.capacity_) {
        std::cout << "Copy constructor\n";
    }
    
    // 移动构造
    DynamicArray(DynamicArray&& other) noexcept
        : data_(std::exchange(other.data_, nullptr)),
          size_(std::exchange(other.size_, 0)),
          capacity_(std::exchange(other.capacity_, 0)) {
        std::cout << "Move constructor\n";
    }
    
    // 析构
    ~DynamicArray() {
        delete[] data_;
    }
    
    // 拷贝赋值
    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) {
            DynamicArray temporary(other); // 复制失败时，当前对象保持原状
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
    
    // 元素访问
    T& operator[](std::size_t index) { return data_[index]; }
    const T& operator[](std::size_t index) const { return data_[index]; }
    
    T& at(std::size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return data_[index];
    }

    const T& at(std::size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return data_[index];
    }
    
    T& front() { return at(0); }
    const T& front() const { return at(0); }

    T& back() {
        if (empty()) throw std::out_of_range("back() on empty array");
        return data_[size_ - 1];
    }

    const T& back() const {
        if (empty()) throw std::out_of_range("back() on empty array");
        return data_[size_ - 1];
    }

    T* data() { return data_; }
    const T* data() const { return data_; }
    
    // 容量
    std::size_t size() const { return size_; }
    std::size_t capacity() const { return capacity_; }
    bool empty() const { return size_ == 0; }
    
    // 修改
    void push_back(const T& value) {
        T copy = value;        // 即使 value 来自本数组，扩容后也不会悬空
        ensure_capacity();
        data_[size_] = std::move(copy);
        ++size_;
    }
    
    void push_back(T&& value) {
        T temporary = std::move(value);
        ensure_capacity();
        data_[size_] = std::move(temporary);
        ++size_;
    }
    
    template<typename... Args>
    void emplace_back(Args&&... args) {
        // 教学简化版：先构造临时对象，再移动赋值到已构造的数组槽位。
        T temporary(std::forward<Args>(args)...);
        push_back(std::move(temporary));
    }
    
    void pop_back() {
        if (size_ > 0) --size_;
    }
    
    void reserve(std::size_t new_capacity) {
        if (new_capacity <= capacity_) return;

        // 临时智能指针保证复制过程中抛异常时不会泄漏新数组。
        std::unique_ptr<T[]> new_data(allocate(new_capacity));
        if (size_ > 0) {
            std::copy_n(data_, size_, new_data.get());
        }

        delete[] data_;
        data_ = new_data.release();
        capacity_ = new_capacity;
    }
    
    void clear() { size_ = 0; }

    void swap(DynamicArray& other) noexcept {
        using std::swap;
        swap(data_, other.data_);
        swap(size_, other.size_);
        swap(capacity_, other.capacity_);
    }
    
    // 迭代器
    T* begin() { return data_; }
    T* end() { return data_ == nullptr ? nullptr : data_ + size_; }
    const T* begin() const { return data_; }
    const T* end() const {
        return data_ == nullptr ? nullptr : data_ + size_;
    }
    
private:
    static T* allocate(std::size_t capacity) {
        return capacity == 0 ? nullptr : new T[capacity]{};
    }

    static T* copy_from(std::initializer_list<T> init) {
        std::unique_ptr<T[]> result(allocate(init.size()));
        if (init.size() > 0) {
            std::copy(init.begin(), init.end(), result.get());
        }
        return result.release();
    }

    static T* clone(const DynamicArray& other) {
        std::unique_ptr<T[]> result(allocate(other.capacity_));
        if (other.size_ > 0) {
            std::copy_n(other.data_, other.size_, result.get());
        }
        return result.release();
    }

    void ensure_capacity() {
        if (size_ < capacity_) return;

        if (capacity_ > std::numeric_limits<std::size_t>::max() / 2) {
            throw std::length_error("DynamicArray capacity overflow");
        }
        reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }

    T* data_ = nullptr;
    std::size_t size_ = 0;
    std::size_t capacity_ = 0;
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

### 12.2 为什么这样实现

- **析构函数**只释放当前对象拥有的数组，落实 RAII。
- **拷贝构造**分配新数组，因此副本和原对象互不影响。
- **拷贝赋值**先构造临时副本，再交换资源。若分配或复制抛异常，原对象没有被提前销毁，这叫强异常保证。
- **移动操作**只转移指针、长度和容量，随后把源对象恢复为空但有效的状态。
- **扩容**先在临时资源中完成复制，成功后才替换旧数组；不能先 `delete[]` 再尝试分配。
- **容量翻倍**让连续多次 `push_back` 的平均成本保持较低，但单次触发扩容仍是 O(n)。

不要把 `emplace_back` 中的写法误认为标准容器的实现。由于 `new T[capacity]` 已经构造了每个槽位，再对同一地址直接使用 placement new 会覆盖一个仍活着的对象并导致未定义行为。教学版选择“构造临时对象再赋值”；真正要支持不可默认构造类型并精确控制析构时机，需要学习分配器、原始存储和对象生命周期，这属于后续进阶主题。

建议用下面几组情况验证类不变量：空数组第一次插入、连续插入触发多次扩容、自赋值、复制后分别修改、移动后继续给源对象插入，以及 `at(size())` 必须抛出异常。

---

## 附录：参考资料

1. **C++ working draft**：https://eel.is/c++draft/
2. **cppreference 语言与标准库**：https://en.cppreference.com/w/cpp
3. **ISO C++ Core Guidelines**：https://isocpp.github.io/CppCoreGuidelines/
4. **《C++ Primer》**：用于语言规则、类、模板和标准库基础。
5. **《A Tour of C++》**：用于建立现代 C++ 的整体地图。
6. **《Effective Modern C++》**：用于继续学习类型推导、所有权、移动和 Lambda 的工程边界。

working draft/cppreference 用于核对标准语义和版本，教材用于建立直觉和组织练习；实现行为或经验建议会在正文中明确标注，不把它们写成语言保证。

---

## 学习建议

1. **先补基础**：完成第 0 章自测，再按 35 天主线逐日推进，不用一次背完全部规则。
2. **实践为主**：每学一个特性，都要先预测结果，再编译运行并修改输入验证。
3. **理解边界**：不仅记住推荐写法，还要说明旧写法错在哪里、推荐写法有哪些前提。
4. **先用标准库**：动态数组等项目用于理解机制，实际代码优先选 `vector`、智能指针等成熟组件。
5. **形成闭环**：保留编译错误、错题和修正原因，每周复习时重新独立完成一次。

---

*本教程以 C++17 为统一编译环境，覆盖进入 35 天主线所需的基础语法，以及 C++11/14/17 常用现代特性。*
