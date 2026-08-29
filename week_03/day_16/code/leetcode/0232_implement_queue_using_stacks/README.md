# LeetCode 232. 用栈实现队列

> **文档职责**：[Day 16 主文](../../../README.md#day16-queue-adaptor) 讲 `std::queue` 的底层容器契约；本文只负责 LC 232 的状态不变量、均摊证明与空队列边界。形象化手算可配合 [栈队列专题指南](../../../../栈队列专题形象化题解指南.md) 阅读。

## 题目描述

请你仅使用两个栈实现一个先入先出（FIFO）的队列，并支持队列的四种操作：
- `push(x)`：将元素 x 推到队列的末尾
- `pop()`：从队列的开头移除并返回元素
- `peek()`：返回队列开头的元素
- `empty()`：如果队列为空，返回 true，否则返回 false

## 示例

### 示例 1
```
输入：
["MyQueue", "push", "push", "peek", "pop", "empty"]
[[], [1], [2], [], [], []]
输出：
[null, null, null, 1, 1, false]

解释：
MyQueue myQueue = new MyQueue();
myQueue.push(1); // queue is: [1]
myQueue.push(2); // queue is: [1, 2] (leftmost is front of the queue)
myQueue.peek(); // return 1
myQueue.pop(); // return 1, queue is [2]
myQueue.empty(); // return false
```

## 提示

- `1 <= x <= 9`
- 最多调用 `100` 次 `push`、`pop`、`peek` 和 `empty`
- 假设所有操作都是有效的（例如，一个空的队列不会调用 `pop` 或者 `peek` 操作）

## 进阶

你能否实现每个操作均摊时间复杂度为 O(1) 的队列？换句话说，执行 n 个操作的总时间复杂度为 O(n) ，即使其中一个操作可能花费较长时间。

---

## 形象化理解：两个碗的智慧

### 日常比喻

想象你在吃饭，有两个碗：
- **碗A（入队栈）**：用来放新盛的食物
- **碗B（出队栈）**：用来吃食物

**问题**：你想吃最先盛的那口饭（队头），但它在碗A的最下面！

**解决方案**：
1. 把碗A里的饭全部倒到碗B
2. 原来在最下面的饭现在变成了最上面的！
3. 直接从碗B顶部吃就行！

### 图解

```
入队顺序：1, 2, 3

步骤1：入队
碗A (入队栈)        碗B (出队栈)
┌─────────┐        ┌─────────┐
│    3    │ ← 顶   │         │
│    2    │        │         │
│    1    │ ← 底   │         │
└─────────┘        └─────────┘

步骤2：需要出队时，将碗A倒入碗B
碗A (入队栈)        碗B (出队栈)
┌─────────┐        ┌─────────┐
│         │        │    1    │ ← 顶（队头！）
│         │        │    2    │
│         │        │    3    │ ← 底
└─────────┘        └─────────┘

步骤3：从碗B弹出
碗A (入队栈)        碗B (出队栈)
┌─────────┐        ┌─────────┐
│         │        │    2    │ ← 新的顶
│         │        │    3    │
│         │        │         │
└─────────┘        └─────────┘
```

---

## 解题思路

### 核心思想

**栈是 LIFO（后进先出），队列是 FIFO（先进先出）**

关键洞察：**两次反转可以恢复顺序，但转移只在出队栈为空时发生**

- 第一次入栈：顺序变反
- 第二次入栈：顺序再变反 = 恢复原顺序

这里不是说“每次队列操作固定对应两次栈操作”。一次 `pop()` 可能触发很多元素的转移，单次最坏是 O(n)；只是同一个元素不会被反复转移，所以一串操作的总成本仍是线性的。

### 算法步骤

1. **push(x)**：直接压入入队栈
2. **pop()/peek()**：
   - 如果出队栈为空，将入队栈所有元素转移到出队栈
   - 操作出队栈栈顶元素

---

## 代码实现

```cpp
class MyQueue {
private:
    std::stack<int> inStack_;   // 入队栈
    std::stack<int> outStack_;  // 出队栈
    
    // 将入队栈的元素转移到出队栈
    void transfer() {
        while (!inStack_.empty()) {
            outStack_.push(inStack_.top());
            inStack_.pop();
        }
    }
    
public:
    MyQueue() {}
    
    void push(int x) {
        inStack_.push(x);
    }
    
    int pop() {
        if (outStack_.empty()) {
            transfer();
        }
        if (outStack_.empty()) {
            throw std::underflow_error("不能从空队列弹出元素");
        }
        int result = outStack_.top();
        outStack_.pop();
        return result;
    }
    
    int peek() {
        if (outStack_.empty()) {
            transfer();
        }
        if (outStack_.empty()) {
            throw std::underflow_error("空队列没有队首元素");
        }
        return outStack_.top();
    }
    
    bool empty() const noexcept {
        return inStack_.empty() && outStack_.empty();
    }
};
```

---

## 复杂度分析

| 操作 | 时间复杂度 | 说明 |
|------|-----------|------|
| push | O(1) | 直接压栈 |
| pop | 均摊O(1) | 每个元素最多转移一次 |
| peek | 均摊O(1) | 同上 |
| empty | O(1) | 直接判断 |

### 均摊分析

为什么是均摊 O(1)？

- 每个元素最多被 push 进 inStack 一次
- 每个元素最多被转移（从 inStack pop + push 进 outStack）一次
- 每个元素最多被 pop 出 outStack 一次

所以 n 个元素的总操作次数 = O(n)，均摊每次操作 = O(1)

更精确地说，`pop/peek` 的**单次最坏复杂度**是 O(n)，**均摊复杂度**才是 O(1)。状态不变量是：`outStack` 非空时，其栈顶就是当前队首；只有它为空时，才把 `inStack` 整体转移。否则新入队元素会越过还没出队的旧元素。

---

## 关键点

1. **延迟转移策略**：只在需要时才转移元素，提高效率
2. **两个栈的分工**：入队栈负责接收，出队栈负责弹出
3. **均摊分析**：理解为什么均摊复杂度是 O(1)
4. **边界情况**：出队栈为空时需要转移

LeetCode 题面保证 `pop/peek` 不会作用于空队列，但仓库公开类把这一前置条件提升成可测试契约：非法调用抛 `std::underflow_error`。不要先读 `top()` 再判空；访问空栈顶属于不满足标准库前置条件，不是“返回一个未知值”的普通失败。

---

## 相关题目

- [225. 用队列实现栈](https://leetcode.cn/problems/implement-stack-using-queues/)
- [面试题 03.04. 化栈为队](https://leetcode.cn/problems/implement-queue-using-stacks-lcci/)

---

## 运行测试

```bash
# 在 week_03/day_16 目录执行
cmake -S . -B /tmp/day16-build -DCMAKE_BUILD_TYPE=Release
cmake --build /tmp/day16-build --target day16_lc232_tests
ctest --test-dir /tmp/day16-build -R day16_lc232_detailed --output-on-failure
```
