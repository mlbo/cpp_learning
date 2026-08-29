# Day 11: Pimpl模式与链表专题

> **学习定位**：前三日主要关注运行期生命周期，本日把所有权推进到编译依赖与接口稳定性。先理解为什么要隐藏实现，再看 `unique_ptr` 管理不完整类型时析构函数应放在哪里定义。

## 阅读导航

- Pimpl 与不完整类型是本日语言主线；可对照 [cppreference Pimpl](https://en.cppreference.com/w/cpp/language/pimpl) 与 `unique_ptr` 的完整类型要求。
- K 路合并和旋转链表的完整题解分别见 [LeetCode 23](code/leetcode/0023_merge_k_lists/README.md) 与 [LeetCode 61](code/leetcode/0061_rotate_list/README.md)。
- 前接 [Day 10 的观察关系](../day_10/README.md)，后接 [Day 12 的 RAII 与资源选择](../day_12/README.md)。

## 学习目标
- 掌握Pimpl模式（编译防火墙）的原理与实现
- 理解unique_ptr在Pimpl模式中的应用
- 学习EMC++条款22：使用Pimpl模式
- 解决LeetCode链表经典问题：合并K个升序链表、旋转链表

---

## 一、Pimpl模式（编译防火墙）

### 1.1 什么是Pimpl模式？

**Pimpl**（Pointer to Implementation，指向实现的指针）是一种C++编程技巧，也称为"编译防火墙"（Compilation Firewall）或"Cheshire Cat"模式。

**核心思想**：将类的实现细节从类定义中分离出来，通过指针访问实现类。

### 1.2 为什么需要Pimpl？

#### 问题：头文件依赖导致的编译时间问题

```cpp
// 传统写法 - widget.h
#include <string>
#include <vector>
#include <map>
#include "heavy_lib.h"  // 大型库头文件

class Widget {
public:
    Widget();
    ~Widget();
private:
    std::string name_;
    std::vector<int> data_;
    std::map<int, std::string> lookup_;
    HeavyObject heavy_;  // 依赖HeavyLib
};
```

**问题**：
1. 任何私有成员的改变都需要重新编译所有包含该头文件的代码
2. 头文件包含的依赖会传播给所有使用者
3. 编译时间大大增加

#### 解决方案：Pimpl模式

```cpp
// widget.h - 对外公开的头文件
#include <memory>  // 本例公开接口没有出现其他库类型，所以这里只需<memory>

class Widget {
public:
    Widget();
    ~Widget();  // 必须在.cpp中定义
private:
    class Impl;  // 前向声明
    std::unique_ptr<Impl> pImpl_;  // 指向实现的指针
};

// widget.cpp - 实现文件
#include "widget.h"
#include <string>
#include <vector>
#include <map>
#include "heavy_lib.h"

class Widget::Impl {
public:
    std::string name_;
    std::vector<int> data_;
    std::map<int, std::string> lookup_;
    HeavyObject heavy_;
};

Widget::Widget() : pImpl_(std::make_unique<Impl>()) {}
Widget::~Widget() = default;  // 必须定义，因为unique_ptr需要完整类型
```

这里的“只需要 `<memory>`”有前提：公开接口没有使用其他类型。如果接口是 `std::string name() const` 或 `const std::vector<int>& data() const`，公开头文件仍必须正确包含 `<string>`、`<vector>`。Pimpl 能隐藏 `map`、大型第三方库等**实现专属依赖**，不能隐藏已经成为接口契约的类型。

### 1.3 Pimpl模式的优势

| 优势 | 说明 |
|------|------|
| **编译防火墙** | 只修改实现细节时，通常无需重新编译使用者代码 |
| **减少头文件依赖** | 实现依赖不会传播给使用者 |
| **ABI稳定辅助** | 对象布局更稳定，但不自动保证虚函数、异常、编译器选项等ABI兼容 |
| **更快的编译速度** | 头文件更简洁，包含更少 |

Pimpl 不是免费午餐：通常需要一次动态分配，访问成员多一次指针间接寻址；`unique_ptr` 默认只提供独占语义，如果外层类还要像值一样可拷贝，就要自己实现深拷贝。

### 1.4 C++11中的Pimpl实现要点

#### 关键点1：使用unique_ptr

```cpp
class Widget {
private:
    class Impl;
    std::unique_ptr<Impl> pImpl_;  // 推荐使用unique_ptr
};
```

#### 关键点2：析构函数必须定义

先分清“不完整类型”能做什么：前向声明之后，编译器知道 `Impl` 是一个类型，所以可以声明 `Impl*` 或 `unique_ptr<Impl>`；但它还不知道对象大小和成员布局，因此不能创建 `Impl` 对象、访问成员，也不能在看不到完整定义的地方执行 `delete Impl`。

```cpp
// widget.h
~Widget();  // 声明

// widget.cpp
Widget::~Widget() = default;  // 或手动实现
```

**为什么？** `unique_ptr<Impl>` 最终会删除 `Impl`。标准库实现通常会在删除路径检查类型是否完整，因此外层 `Widget` 的析构函数要延迟到 `.cpp`，并放在 `Impl` 完整定义可见之后。出于同样的实例化边界，默认移动构造和移动赋值也放到 `.cpp` 定义最稳妥。

#### 关键点3：需要处理的特殊成员函数

```cpp
class Widget {
public:
    Widget();
    ~Widget();

    // 需要实现的特殊成员函数
    Widget(Widget&& rhs) noexcept;           // 移动构造
    Widget& operator=(Widget&& rhs) noexcept; // 移动赋值

    // 拷贝操作需要自定义（unique_ptr不可拷贝）
    Widget(const Widget& rhs);
    Widget& operator=(const Widget& rhs);
};
```

还要明确“有效但未指定状态”在这个具体教学类里的可用边界：这里的移动操作直接移动 `unique_ptr`，所以源对象的 `pImpl_` 为空。被移动的 `Widget` 只保证可以析构、作为拷贝/移动赋值的目标或参与 `swap`；在重新获得有效实现对象前，不得调用 `getName`、`setId`、`addData` 等普通查询和修改接口，因为这些接口会解引用 `pImpl_`。这不是所有类都必须采用的设计，而是本示例选择的移动后契约。

### 1.5 完整示例

详见 `code/cpp11_features/` 目录。

---

## 二、EMC++条款22：使用Pimpl模式

### 条款要点

> **条款22**：当使用Pimpl模式时，将特殊成员函数定义在实现文件中。

### 2.1 常见错误

```cpp
// 错误示例 - widget.h
class Widget {
public:
    Widget();
    // 编译器生成的析构函数会出错！
    // 因为在.h中Impl是不完整类型
private:
    class Impl;
    std::unique_ptr<Impl> pImpl_;
};
```

**错误信息**：
```
error: invalid application of 'sizeof' to incomplete type 'Widget::Impl'
```

### 2.2 正确做法

```cpp
// widget.h
class Widget {
public:
    Widget();
    ~Widget();  // 声明
    Widget(Widget&& rhs) noexcept;
    Widget& operator=(Widget&& rhs) noexcept;
private:
    class Impl;
    std::unique_ptr<Impl> pImpl_;
};

// widget.cpp
#include "widget.h"

class Widget::Impl { /*...*/ };

Widget::~Widget() = default;
Widget::Widget(Widget&& rhs) noexcept = default;
Widget& Widget::operator=(Widget&& rhs) noexcept = default;
```

这里的 `noexcept` 成立，是因为默认移动只转移 `unique_ptr`，实现体不再夹带日志、重新分配或给源对象补写字符串。若把 `std::cout` 写进移动函数，流被设置为遇错抛异常时就会在 `noexcept` 边界触发 `std::terminate`；本日的 `PimplContract` 测试会注入输出失败，确认移动仍不抛。拷贝赋值采用“先完整构造临时副本，再 `swap` 提交”：分配、复制乃至临时对象构造日志失败都发生在提交前，目标保持原值；提交后不要再做可能抛出的输出，否则就不能宣称强保证。`Impl` 由 `string`、`vector`、`map` 等成员组成，因此内部遵循 Rule of Zero；外层 `Widget` 只因 `unique_ptr<Impl>` 的不完整类型边界和所选值语义而在 `.cpp` 定义特殊成员函数。

### 2.3 shared_ptr vs unique_ptr

| 特性 | unique_ptr | shared_ptr |
|------|------------|------------|
| 表达的所有权 | 独占实现对象 | 共享实现对象 |
| 外层析构处的完整类型 | 通常必须可见 | 析构当前 `shared_ptr` 时通常不要求当前点看到完整类型 |
| 构造实现对象 | 必须看到完整类型 | 同样必须看到完整类型 |
| 额外状态 | 无控制块；删除器是类型的一部分 | 有控制块和引用计数 |
| Pimpl默认选择 | **通常推荐** | 只有确实需要共享实现对象时才选 |

```cpp
// shared_ptr版本：构造仍应放在Impl完整定义可见的.cpp中
class Widget {
public:
    Widget();
private:
    class Impl;
    std::shared_ptr<Impl> pImpl_;
};

// widget.cpp
class Widget::Impl { /*...*/ };
Widget::Widget() : pImpl_(std::make_shared<Impl>()) {}
```

原来把 `make_shared<Impl>()` 直接写在头文件类定义中的做法是错误的：构造 `Impl` 必须知道它的完整大小。`shared_ptr` 放宽的是“稍后析构这个句柄时”的要求，不是“可以在不完整类型处创建对象”。

---

## 三、LeetCode题目精讲

### 3.1 LeetCode 23: 合并K个升序链表

#### 题目描述
给你一个链表数组，每个链表都已经按升序排列。请将所有链表合并到一个升序链表中，返回合并后的链表。

先固定符号：`K` 是链表数量，`N` 是所有链表的节点总数。复杂度里的 `N` 不是“每条链表长度”；各链表长度可以完全不同。

#### 解法一：分治合并

```cpp
ListNode* mergeKLists(vector<ListNode*>& lists) {
    if (lists.empty()) return nullptr;
    return merge(lists, 0, lists.size() - 1);
}

ListNode* merge(vector<ListNode*>& lists, int left, int right) {
    if (left == right) return lists[left];
    int mid = left + (right - left) / 2;
    ListNode* l1 = merge(lists, left, mid);
    ListNode* l2 = merge(lists, mid + 1, right);
    return mergeTwoLists(l1, l2);
}
```

**时间复杂度**：O(N × log K)，其中N是所有节点总数，K是链表数量
**空间复杂度**：O(log K)，递归栈深度

为什么是 `log K` 层？每层把链表组数减半；每一层合并时，全部 `N` 个节点总共被处理一次，所以总时间是 `N × log K`。

#### 解法二：优先队列（最小堆）

```cpp
ListNode* mergeKLists(vector<ListNode*>& lists) {
    auto cmp = [](ListNode* a, ListNode* b) { return a->val > b->val; };
    priority_queue<ListNode*, vector<ListNode*>, decltype(cmp)> pq(cmp);

    for (auto list : lists) {
        if (list) pq.push(list);
    }

    ListNode dummy;
    ListNode* tail = &dummy;

    while (!pq.empty()) {
        ListNode* node = pq.top(); pq.pop();
        tail->next = node;
        tail = tail->next;
        if (node->next) pq.push(node->next);
    }

    return dummy.next;
}
```

**时间复杂度**：O(N × log K)
**空间复杂度**：O(K)，优先队列大小

堆里只保留“每条尚未耗尽链表的当前头节点”，最多 `K` 个。每取出一个节点，最多再压入它的后继，因此 `N` 个节点各经历一次 `pop` 和至多一次 `push`。

#### 解法三：顺序合并

把结果先与第 1 条链表合并，再与第 2 条合并，代码最直接，但已经合并好的长前缀会被反复扫描。若链表规模接近，最坏时间复杂度是 `O(NK)`，额外空间 `O(1)`。它适合 `K` 很小的简单场景，不应误写成 `O(K²N)`。

---

### 3.2 LeetCode 61: 旋转链表

#### 题目描述
给定链表的头节点 `head`，旋转链表，将链表每个节点向右移动 `k` 个位置。

#### 解题思路
1. 计算链表长度
2. 将链表首尾相连形成环
3. 找到新的断开点
4. 断开并返回新头节点

```cpp
ListNode* rotateRight(ListNode* head, int k) {
    if (!head || !head->next || k == 0) return head;

    // 1. 计算长度并找到尾节点
    int len = 1;
    ListNode* tail = head;
    while (tail->next) {
        tail = tail->next;
        len++;
    }

    // 2. 计算有效旋转次数
    k = k % len;
    if (k == 0) return head;

    // 3. 成环
    tail->next = head;

    // 4. 找到新的断开点（倒数第k+1个节点）
    int steps = len - k;
    ListNode* newTail = head;
    for (int i = 1; i < steps; i++) {
        newTail = newTail->next;
    }

    // 5. 断开并返回
    ListNode* newHead = newTail->next;
    newTail->next = nullptr;

    return newHead;
}
```

**时间复杂度**：O(N)，遍历链表
**空间复杂度**：O(1)，常数空间

---

## 四、今日练习

### 基础练习
1. 实现一个完整的Pimpl模式类`Person`，包含姓名、年龄、地址等私有信息
2. 比较Pimpl和传统实现在编译时间上的差异

### 进阶练习
1. 实现Pimpl模式的移动语义和拷贝语义
2. 使用Pimpl模式重构一个已有的类

### LeetCode练习
1. 完成23题的两种解法实现
2. 完成61题的成环解法
3. 扩展练习：LeetCode 19（删除链表倒数第N个节点）

### 今日工程动作：亲眼观察“编译防火墙”

先构建一次，再分别触碰实现专属头文件和公开头文件，观察详细构建日志里哪些 `.cpp` 被重新编译：

```bash
cd week_02/day_11
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --target day_11_main

touch code/cpp11_features/pimpl_widget_impl.h
cmake --build build --target day_11_main --verbose

touch code/cpp11_features/pimpl_widget.h
cmake --build build --target day_11_main --verbose
```

按当前工程依赖，修改 `pimpl_widget_impl.h` 主要重编实现文件；修改公开的 `pimpl_widget.h` 会影响所有包含它的翻译单元。真实收益取决于项目规模和构建系统，但这个实验能把“减少依赖传播”从口号变成可观察事实。

### 五句复盘

1. 前向声明足以声明指针成员，但创建、访问或删除 `Impl` 对象时必须看到完整类型。
2. `Widget` 的析构和可能销毁旧实现的移动赋值必须在完整类型处实例化，把移动操作统一放在 `.cpp` 也能保持不完整类型边界一致。
3. Pimpl 只能隐藏实现专属依赖，已经出现在公开函数签名中的类型仍属于头文件契约。
4. 分治与最小堆都让每个节点经历 `O(log K)` 层或堆操作，因此时间为 `O(N log K)`，额外空间分别为 `O(log K)` 与 `O(K)`。
5. 旋转链表把有效位移归一化为 `k % N`，成环后从头走 `N - k % N - 1` 条边找到新尾再断开。

---

## 五、代码结构

```
day_11/
├── README.md               # 本教程文档
├── CMakeLists.txt          # CMake构建配置
├── build_and_run.sh        # 构建脚本
└── code/
    ├── main.cpp            # 主程序入口
    ├── cpp11_features/     # Pimpl模式示例
    │   ├── pimpl_widget.h
    │   ├── pimpl_widget.cpp
    │   ├── pimpl_widget_impl.h
    │   ├── pimpl_contract_test.cpp
    │   └── pimpl_demo.cpp
    ├── emcpp/              # EMC++条款22示例
    │   └── item22_pimpl.cpp
    └── leetcode/           # LeetCode题目
        ├── 0023_merge_k_lists/
        │   ├── solution.h
        │   ├── solution.cpp
        │   ├── test.cpp
        │   └── README.md
        └── 0061_rotate_list/
            ├── solution.h
            ├── solution.cpp
            ├── test.cpp
            └── README.md
```

---

## 六、参考资料

1. 《Effective Modern C++》条款22
2. 《Effective C++》条款31
3. [Pimp idiom - CppReference](https://en.cppreference.com/w/cpp/language/pimpl)
4. Herb Sutter: "GotW #7: Compilation Firewalls"

---

## 七、编译运行

```bash
# 进入day_11目录
cd week_02/day_11

# 非交互运行Pimpl演示和知识总结，并自动执行全部CTest
./build_and_run.sh all

# 只运行主程序（交互菜单；标准输入结束时会友好退出）
./build_and_run.sh main

# 条款22的独立详细/性能演示按需运行，避免默认流程产生大量教学日志
./build_and_run.sh item22

# 运行单个LeetCode题目，或统一运行5项CTest
./build_and_run.sh leetcode23
./build_and_run.sh leetcode61
./build_and_run.sh test

# 主程序也可直接以非交互模式运行，适合CI和脚本
./build/day_11_main --all

# 可选：使用独立的 Release 构建目录
DAY11_BUILD_DIR="$PWD/build-release" BUILD_TYPE=Release ./build_and_run.sh all
```

---

**Day 11 完成！** 🎉
