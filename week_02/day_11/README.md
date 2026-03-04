# Day 11: Pimpl模式与链表专题

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
#include <memory>  // 只需要<memory>

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

### 1.3 Pimpl模式的优势

| 优势 | 说明 |
|------|------|
| **编译防火墙** | 修改实现类无需重新编译使用者代码 |
| **减少头文件依赖** | 实现依赖不会传播给使用者 |
| **ABI兼容** | 二进制接口更稳定，便于库升级 |
| **更快的编译速度** | 头文件更简洁，包含更少 |

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

```cpp
// widget.h
~Widget();  // 声明

// widget.cpp
Widget::~Widget() = default;  // 或手动实现
```

**为什么？**
- `unique_ptr`的析构函数会调用`static_assert`检查是否为完整类型
- 在头文件中`Impl`是不完整类型，必须延迟到.cpp中

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

### 2.3 shared_ptr vs unique_ptr

| 特性 | unique_ptr | shared_ptr |
|------|------------|------------|
| 析构函数 | 必须在.cpp中定义 | 不需要在.cpp中定义 |
| 类型检查 | 析构时检查完整类型 | 运行时删除器 |
| 性能 | 更高效（无控制块） | 稍慢（有控制块） |
| 推荐度 | **推荐** | 可用但非最优 |

```cpp
// shared_ptr版本 - 不需要定义析构函数
class Widget {
public:
    Widget() : pImpl_(std::make_shared<Impl>()) {}
    // 析构函数可以不定义！
private:
    class Impl;
    std::shared_ptr<Impl> pImpl_;
};
```

---

## 三、LeetCode题目精讲

### 3.1 LeetCode 23: 合并K个升序链表

#### 题目描述
给你一个链表数组，每个链表都已经按升序排列。请将所有链表合并到一个升序链表中，返回合并后的链表。

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
cd /home/z/my-project/download/week_02/day_11

# 编译并运行所有程序
./build_and_run.sh all

# 只运行主程序
./build_and_run.sh main

# 运行LeetCode题目测试
./build_and_run.sh leetcode23
./build_and_run.sh leetcode61
```

---

**Day 11 完成！** 🎉
