# Day 10: weak_ptr详解与链表双指针技巧

## 学习目标

1. **深入理解 weak_ptr 的设计目的和用法**
2. **掌握打破循环引用的方法**
3. **学会使用 lock() 安全获取 shared_ptr**
4. **理解快慢指针找环入口的数学推导**
5. **掌握删除倒数第N个节点的双指针技巧**

---

## 一、weak_ptr 详解

### 1.1 weak_ptr 的设计目的

`weak_ptr` 是一种不控制对象生命周期的智能指针，它指向一个由 `shared_ptr` 管理的对象。

**核心特点：**
- 不增加引用计数
- 不影响对象的生命周期
- 需要通过 `lock()` 转换为 `shared_ptr` 才能访问对象

### 1.2 为什么需要 weak_ptr？

```cpp
// 场景1：循环引用问题
struct Node {
    std::shared_ptr<Node> next;
    std::shared_ptr<Node> prev;  // 导致循环引用
};

// 场景2：观察者模式
class Subject {
    std::vector<std::weak_ptr<Observer>> observers;  // 不控制观察者生命周期
};
```

### 1.3 weak_ptr 核心操作

| 方法 | 说明 |
|------|------|
| `lock()` | 返回 shared_ptr，若对象已销毁则返回空 |
| `expired()` | 检查对象是否已销毁 |
| `use_count()` | 返回共享对象的 shared_ptr 数量 |
| `reset()` | 释放引用 |

### 1.4 lock() 方法详解

```cpp
std::weak_ptr<int> wp = sp;
if (auto sp2 = wp.lock()) {  // 安全获取
    // sp2 有效，可以使用
    *sp2 = 42;
} else {
    // 对象已被销毁
}
```

---

## 二、循环引用与解决方案

### 2.1 循环引用问题

```
┌─────────┐         ┌─────────┐
│ Node A  │────────>│ Node B  │
│ ref:1   │         │ ref:1   │
│<────────│         │<────────│
└─────────┘         └─────────┘
    ↑                    │
    │    循环引用         │
    └────────────────────┘
```

### 2.2 解决方案：使用 weak_ptr

```cpp
struct Node {
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> prev;  // 改为 weak_ptr
};
```

---

## 三、EMC++ 条款21：shared_ptr 与 weak_ptr 配合

> **条款要点：** 当需要访问但不拥有资源时，使用 `weak_ptr`。

**典型应用场景：**
1. 缓存系统 - 不阻止缓存对象被释放
2. 观察者模式 - Subject 不拥有 Observer
3. 打破循环引用 - 解决 shared_ptr 循环依赖
4. 对象池 - 允许对象被正常回收

**使用模式：**
```cpp
if (auto sp = wp.lock()) {
    // 安全使用 sp
} else {
    // 对象已销毁
}
```

---

## 四、LeetCode 题目精讲

### 4.1 LeetCode 142: 环形链表 II

**题目：** 给定链表头节点，返回链表开始入环的第一个节点。

**快慢指针数学推导：**

```
设：
- a: 头到入环点的距离
- b: 入环点到相遇点的距离
- c: 相遇点到入环点的距离

快指针走的距离：a + n(b+c) + b = a + (n+1)b + nc
慢指针走的距离：a + b

快指针是慢指针的两倍：
2(a + b) = a + (n+1)b + nc
=> a = (n-1)(b+c) + c

结论：从相遇点和头同时出发，会在入环点相遇！
```

**算法步骤：**
1. 快指针走两步，慢指针走一步
2. 相遇后，一个指针回起点
3. 两指针各走一步，相遇点即为入口

**时间复杂度：** O(n)  
**空间复杂度：** O(1)

### 4.2 LeetCode 19: 删除链表的倒数第N个节点

**双指针技巧：**
1. 快指针先走 n 步
2. 快慢指针同时移动，直到快指针到达末尾
3. 慢指针指向要删除节点的前一个节点

**可视化演示：**
```
链表: 1->2->3->4->5, n=2

初始:     dummy->1->2->3->4->5
          slow  fast

快指针走2步: dummy->1->2->3->4->5
                    slow    fast

同时移动:   dummy->1->2->3->4->5
                          slow  fast

slow在倒数第3个节点，删除slow->next
```

**时间复杂度：** O(n) 一次遍历  
**空间复杂度：** O(1)

---

## 五、代码结构

```
code/
├── main.cpp                      # 主入口
├── cpp11_features/
│   └── weak_ptr_demo.cpp         # weak_ptr基本用法
├── emcpp/
│   └── item21.cpp                # EMC++条款21实践
└── leetcode/
    ├── 0142_cycle_entrance/      # 环形链表II
    │   └── solution.cpp
    └── 0019_remove_nth/          # 删除倒数第N个节点
        └── solution.cpp
```

---

## 六、编译运行

```bash
cd /home/z/my-project/download/week_02/day_10
./build_and_run.sh
```

---

## 七、学习要点总结

| 知识点 | 关键内容 |
|--------|----------|
| weak_ptr 目的 | 不增加引用计数，不控制生命周期 |
| lock() | 安全获取 shared_ptr，返回空表示已销毁 |
| 循环引用 | 用 weak_ptr 打破 shared_ptr 循环 |
| 快慢指针 | 找环入口，数学推导证明 |
| 双指针删除 | 快指针先行n步，一次遍历完成 |

---

## 八、延伸阅读

1. 《Effective Modern C++》条款21
2. 《C++ Primer》第12章 智能指针
3. C++ Reference: std::weak_ptr

---

**Day 10 完成！**
