# Day 32：DFS深度优先搜索

## 📅 学习目标

今天我们将深入学习深度优先搜索（DFS）算法，这是解决树和图问题的核心算法之一。DFS沿着一条路径尽可能深入，直到无法继续再回溯探索其他路径，这种策略使得它非常适合处理路径搜索、连通性判断、组合枚举等问题。同时，我们将学习C++11的原子操作（std::atomic），它是无锁编程的基础，提供了比互斥锁更轻量的线程安全保证。最后，我们还将学习Effective Modern C++中关于volatile和atomic使用的重要条款，避免常见的并发编程陷阱。

**本日学习重点：**
- 掌握DFS算法的核心思想和实现模板
- 理解递归DFS和迭代DFS的区别与联系
- 学会使用std::atomic进行无锁编程
- 理解volatile和atomic的本质区别
- 完成两道经典的DFS LeetCode题目

---

## 📖 知识点一：DFS深度优先搜索

### 概念定义

深度优先搜索（Depth-First Search, DFS）是一种用于遍历或搜索树、图等数据结构的算法。DFS的核心思想是：从起始节点出发，沿着一条路径尽可能深入，直到到达叶子节点或无法继续前进，然后回溯到上一个节点，继续探索其他未访问的路径。这种"深入到底再回溯"的策略使得DFS天然适合用递归实现。

DFS与BFS（广度优先搜索）形成鲜明对比：BFS像水波一样逐层向外扩散，而DFS像探险家一样沿着一条路走到黑，再换另一条路。这两种策略各有适用场景：BFS适合寻找最短路径，DFS适合寻找所有路径或判断连通性。

### DFS的核心模板

**递归实现（最常用）：**

```cpp
void dfs(TreeNode* node) {
    if (node == nullptr) return;  // 终止条件
    
    // 前序位置：处理当前节点
    process(node);
    
    dfs(node->left);   // 递归左子树
    dfs(node->right);  // 递归右子树
    
    // 后序位置：处理完子树后的操作
}
```

**迭代实现（使用栈）：**

```cpp
void dfsIterative(TreeNode* root) {
    if (root == nullptr) return;
    
    stack<TreeNode*> stk;
    stk.push(root);
    
    while (!stk.empty()) {
        TreeNode* node = stk.top();
        stk.pop();
        
        process(node);  // 处理当前节点
        
        // 注意入栈顺序：右先左后，保证左先被处理
        if (node->right) stk.push(node->right);
        if (node->left) stk.push(node->left);
    }
}
```

### DFS的典型应用

| 应用场景 | 说明 | 示例题目 |
|---------|------|---------|
| 路径搜索 | 找到从起点到终点的路径 | 二叉树的所有路径 |
| 连通性判断 | 判断两点是否连通 | 岛屿数量 |
| 组合枚举 | 枚举所有可能的组合 | 全排列、子集 |
| 拓扑排序 | 确定任务的执行顺序 | 课程表 |
| 回溯问题 | 在解空间中搜索可行解 | N皇后、解数独 |

---

## 📖 知识点二：std::atomic 原子操作

### 概念定义

原子操作是不可分割的操作，在执行过程中不会被中断，也不会被其他线程观察到中间状态。C++11引入的`std::atomic`模板类提供了对基本类型的原子操作支持，使得多线程环境下无需使用互斥锁就能安全地访问共享变量。

原子操作比互斥锁更轻量，因为它们通常使用CPU的特殊指令（如CAS，Compare-And-Swap）实现，避免了锁的获取和释放开销。但是，原子操作只能保证单个操作的原子性，对于复合操作仍需使用锁。

### 基本使用

```cpp
#include <atomic>

std::atomic<int> counter(0);
std::atomic<bool> flag(false);
std::atomic<int*> ptr(nullptr);

// 基本操作
counter.store(10);           // 原子存储
int val = counter.load();     // 原子读取
int old = counter.exchange(20);  // 原子交换

// 原子算术操作（仅适用于整数类型）
counter.fetch_add(1);   // 原子加，返回旧值
counter.fetch_sub(1);   // 原子减
counter++;              // 等价于 fetch_add(1)

// 比较并交换（CAS）
int expected = 10;
bool success = counter.compare_exchange_strong(expected, 20);
// 如果 counter == expected，则 counter = 20，返回 true
// 否则 expected = counter，返回 false
```

### 内存序（Memory Order）

C++11提供了多种内存序选项，用于控制原子操作的可见性约束：

| 内存序 | 说明 | 使用场景 |
|-------|------|---------|
| memory_order_relaxed | 只保证原子性，不保证顺序 | 简单计数器 |
| memory_order_acquire | 获取语义，防止后续操作重排到此之前 | 读取共享数据 |
| memory_order_release | 释放语义，防止之前操作重排到此之后 | 写入共享数据 |
| memory_order_seq_cst | 默认，最强约束，全局顺序一致 | 通用场景 |

---

## 📖 知识点三：EMC++ Item 39-40

### Item 39: 对并发使用std::atomic，对特殊内存使用volatile

`std::atomic`用于并发编程，`volatile`用于特殊内存（如内存映射I/O）。两者用途完全不同，不能混用。

**volatile的问题：**
- 不保证原子性
- 不保证可见性（内存序）
- 不能用于线程同步

**正确的使用方式：**

```cpp
// 正确：使用atomic进行并发访问
std::atomic<int> counter(0);
counter++;  // 原子递增，线程安全

// 正确：使用volatile访问特殊内存
volatile int* hardwareReg = reinterpret_cast<volatile int*>(0xFFFF0000);
*hardwareReg = 0x01;  // 每次都要写入，不能被优化掉

// 错误：volatile不保证线程安全
volatile int x = 0;
x++;  // 非原子操作，不线程安全！
```

### Item 40: 使用void future进行一次性事件通信

对于一次性事件通信，使用`std::promise<void>`和`std::future<void>`比条件变量更简洁且不易出错。

```cpp
// 使用promise/future进行一次性通知
std::promise<void> readyPromise;
std::future<void> readyFuture = readyPromise.get_future();

// 等待线程
readyFuture.wait();  // 阻塞直到被通知

// 通知线程
readyPromise.set_value();  // 发送通知
```

---

## 🎯 LeetCode 刷题

### 讲解题：LC 104 二叉树的最大深度

#### 题目链接
[LeetCode 104](https://leetcode.cn/problems/maximum-depth-of-binary-tree/)

#### 题目描述
给定一个二叉树，找出其最大深度。二叉树的深度为根节点到最远叶子节点的最长路径上的节点数。

#### 解题思路

**方法一：递归DFS**
- 树的深度 = max(左子树深度, 右子树深度) + 1
- 递归终止条件：空节点深度为0

```cpp
int maxDepth(TreeNode* root) {
    if (root == nullptr) return 0;
    return max(maxDepth(root->left), maxDepth(root->right)) + 1;
}
```

**方法二：迭代DFS/BFS**
- 使用栈（DFS）或队列（BFS）记录节点和深度

**复杂度分析：**
- 时间复杂度：O(n)，每个节点访问一次
- 空间复杂度：O(h)，递归栈深度

---

### 实战题：LC 111 二叉树的最小深度

#### 题目链接
[LeetCode 111](https://leetcode.cn/problems/minimum-depth-of-binary-tree/)

#### 题目描述
给定一个二叉树，找出其最小深度。最小深度是从根节点到最近叶子节点的最短路径上的节点数量。

#### 解题思路

注意：最小深度的定义是到**叶子节点**的最短路径，如果左子树为空，不能直接返回0。

```cpp
int minDepth(TreeNode* root) {
    if (root == nullptr) return 0;
    if (root->left == nullptr) return minDepth(root->right) + 1;
    if (root->right == nullptr) return minDepth(root->left) + 1;
    return min(minDepth(root->left), minDepth(root->right)) + 1;
}
```

---

## 🚀 运行代码

```bash
cd /home/z/my-project/download/week_05/day_32
./build_and_run.sh
```

---

## 📚 相关术语

| 术语 | 英文 | 定义 |
|------|------|------|
| 深度优先搜索 | Depth-First Search | 沿着一条路径深入再回溯的搜索策略 |
| 原子操作 | Atomic Operation | 不可分割的操作 |
| 内存序 | Memory Order | 原子操作的可见性约束 |
| CAS | Compare-And-Swap | 比较并交换，原子操作的基础 |
| volatile | 易变关键字 | 告诉编译器不要优化对变量的访问 |
| 回溯 | Backtracking | 搜索过程中撤销选择的算法技术 |

---

## 💡 学习提示

1. **DFS理解要点**：DFS的核心是"一条路走到黑"，用递归思维理解：假设子问题已解决，当前层如何处理。

2. **递归vs迭代**：递归代码简洁但可能栈溢出；迭代需要显式管理栈，更复杂但可控。

3. **atomic使用建议**：
   - 简单计数器/标志位使用atomic
   - 复合操作仍需使用mutex
   - 注意内存序的选择

4. **volatile陷阱**：volatile不提供任何线程安全保证，只用于特殊内存访问。

---

## 🔗 参考资料

1. [cppreference - std::atomic](https://en.cppreference.com/w/cpp/atomic/atomic)
2. [LeetCode 104 - Maximum Depth](https://leetcode.com/problems/maximum-depth-of-binary-tree/)
3. [LeetCode 111 - Minimum Depth](https://leetcode.com/problems/minimum-depth-of-binary-tree/)
4. *Effective Modern C++* - Item 39 & 40
