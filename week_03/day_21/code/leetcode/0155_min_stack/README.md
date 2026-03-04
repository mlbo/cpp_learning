# LC 155. 最小栈

## 题目描述

设计一个支持 `push`、`pop`、`top` 操作，并能在**常数时间**内检索到最小元素的栈。

### 要求

- `MinStack()` 初始化堆栈对象
- `void push(int val)` 将元素val推入堆栈
- `void pop()` 删除堆栈顶部的元素
- `int top()` 获取堆栈顶部的元素
- `int getMin()` 获取堆栈中的最小元素（**O(1)** 时间复杂度）

## 形象化理解

### 比喻：带"身高记录"的电梯

```
想象一个电梯（栈），每次有人进入：

普通电梯：只知道谁在电梯里
特殊电梯：随时知道电梯里最矮的人

解决方案：
每个人进电梯时，同时记录：
┌─────────────────────────────────────┐
│  1. 这个人是谁（值）                 │
│  2. 当前电梯里最矮的人是谁（最小值） │
└─────────────────────────────────────┘

这样当一个人离开时：
- 我们知道谁走了
- "最矮记录"自动恢复到上一个状态
```

### 图示演示

```
操作演示：push(5) → push(3) → push(7) → push(1)

步骤1: push(5)
┌───────────────────────────────┐
│  主栈       │   最小栈        │
│  ┌───┐      │   ┌───┐        │
│  │ 5 │      │   │ 5 │ min=5  │
│  └───┘      │   └───┘        │
└───────────────────────────────┘

步骤2: push(3)
┌───────────────────────────────┐
│  主栈       │   最小栈        │
│  ┌───┐      │   ┌───┐        │
│  │ 3 │      │   │ 3 │ min=3  │
│  ├───┤      │   ├───┤        │
│  │ 5 │      │   │ 5 │        │
│  └───┘      │   └───┘        │
└───────────────────────────────┘
   3 < 5，更新最小值

步骤3: push(7)
┌───────────────────────────────┐
│  主栈       │   最小栈        │
│  ┌───┐      │   ┌───┐        │
│  │ 7 │      │   │ 3 │ min=3  │
│  ├───┤      │   ├───┤        │
│  │ 3 │      │   │ 3 │        │
│  ├───┤      │   ├───┤        │
│  │ 5 │      │   │ 5 │        │
│  └───┘      │   └───┘        │
└───────────────────────────────┘
   7 > 3，最小值不变，仍压入3

步骤4: push(1)
┌───────────────────────────────┐
│  主栈       │   最小栈        │
│  ┌───┐      │   ┌───┐        │
│  │ 1 │      │   │ 1 │ min=1  │
│  ├───┤      │   ├───┤        │
│  │ 7 │      │   │ 3 │        │
│  ├───┤      │   ├───┤        │
│  │ 3 │      │   │ 3 │        │
│  ├───┤      │   ├───┤        │
│  │ 5 │      │   │ 5 │        │
│  └───┘      │   └───┘        │
└───────────────────────────────┘
   1 < 3，更新最小值为1
```

## 解法对比

### 方法一：辅助栈法（推荐）

```cpp
class MinStack {
private:
    stack<int> mainStack;  // 主栈
    stack<int> minStack;   // 辅助栈（存最小值）

public:
    void push(int val) {
        mainStack.push(val);
        // 关键：每次都压入当前最小值
        if (minStack.empty()) {
            minStack.push(val);
        } else {
            minStack.push(min(val, minStack.top()));
        }
    }
    
    void pop() {
        mainStack.pop();
        minStack.pop();  // 同步弹出
    }
    
    int getMin() {
        return minStack.top();  // O(1)
    }
};
```

**优点**：
- 思路清晰，易于理解
- 空间换时间，getMin 为 O(1)

**缺点**：
- 需要额外 O(n) 空间

### 方法二：单栈存储 pair

```cpp
class MinStack {
private:
    stack<pair<int, int>> stk;  // (值, 当前最小值)

public:
    void push(int val) {
        if (stk.empty()) {
            stk.push({val, val});
        } else {
            stk.push({val, min(val, stk.top().second)});
        }
    }
    
    int getMin() {
        return stk.top().second;
    }
};
```

**优点**：
- 只需要一个栈
- 代码简洁

**缺点**：
- pair 有一定的内存开销

### 方法三：差值法

```cpp
class MinStack {
private:
    stack<long long> diffStack;  // 存储差值
    long long minVal;            // 当前最小值

public:
    void push(int val) {
        if (diffStack.empty()) {
            diffStack.push(0);
            minVal = val;
        } else {
            long long diff = val - minVal;
            diffStack.push(diff);
            if (diff < 0) minVal = val;
        }
    }
    
    int getMin() { return minVal; }
};
```

**优点**：
- 只需要一个变量存最小值
- 空间效率最高

**缺点**：
- 可能溢出（差值可能很大）
- 逻辑较复杂

## 复杂度分析

| 方法 | 时间复杂度 | 空间复杂度 |
|------|-----------|-----------|
| 辅助栈 | O(1) per op | O(n) |
| pair法 | O(1) per op | O(n) |
| 差值法 | O(1) per op | O(n) |

## 关键洞察

1. **每个元素"记住"它成为栈顶时的最小值**
2. 出栈时，最小值自动恢复到上一个状态
3. 空间换时间：用 O(n) 额外空间换取 O(1) 的 getMin

## 类似题目

- LC 716. 最大栈
- LC 232. 用栈实现队列
- LC 225. 用队列实现栈
