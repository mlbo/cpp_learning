# Day 32：DFS深度优先搜索

> **学习定位**：DFS 把递归模板推广到路径与状态搜索；原子类型把同步缩小到单个操作。EMC++ Item 39 是一次性事件通信，Item 40 才是 `atomic` 与 `volatile` 的边界，本日按这个正确顺序学习。

> **共性入口**：DFS 的递归栈和深度手算见 [形象化指南的 Day 32](../树与并发专题形象化题解指南.md#day32-visual)；atomic、内存模型和 future/promise 的完整机制见 [C++ 并发编程教程](../../tutorials/CPP并发编程教程.md)；Item 39–40 主讲见 [Effective Modern C++ 教程](../../tutorials/Effective_Modern_CPP教程.md)。本日只新增“图 visited 不变量”“单对象原子边界”和“一次性事件”。

> **前后关系**：上一日 [Day 31](../day_31/README.md) 用共享谓词协调重复事件；下一日 [Day 33](../day_33/README.md) 把 DFS 状态变成可回溯路径，并把 future、队列和 worker 组合成线程池。

## 📅 学习目标

今天我们将深入学习深度优先搜索（DFS）算法，这是解决树和图问题的核心算法之一。DFS沿着一条路径尽可能深入，直到无法继续再回溯探索其他路径，这种策略使得它非常适合处理路径搜索、连通性判断、组合枚举等问题。同时，我们将学习C++11的原子操作（std::atomic）：它为单个对象上的操作提供原子语义和内存序控制，是构建部分无锁算法的基础，但具体实现不保证一定无锁。最后，我们还将学习Effective Modern C++中一次性事件通信以及volatile和atomic的使用边界。

**本日学习重点：**
- 掌握DFS算法的核心思想和实现模板
- 理解递归DFS和迭代DFS的区别与联系
- 学会使用 `std::atomic` 完成简单原子操作，并判断某个原子类型在当前平台是否 lock-free
- 理解volatile和atomic的本质区别
- 完成两道经典的DFS LeetCode题目

---

## 📖 知识点一：DFS深度优先搜索

### 概念定义

深度优先搜索（Depth-First Search, DFS）是一种用于遍历或搜索树、图等数据结构的算法。DFS的核心思想是：从起始节点出发，沿着一条路径尽可能深入，直到到达叶子节点或无法继续前进，然后回溯到上一个节点，继续探索其他未访问的路径。这种"深入到底再回溯"的策略使得DFS天然适合用递归实现。

DFS与BFS（广度优先搜索）形成鲜明对比：BFS像水波一样逐层向外扩散，而DFS像探险家一样沿着一条路走到底，再换另一条路。BFS 能直接保证的是无权图或每条边等权时的最少边数；一般非负带权图要用 Dijkstra，含负权时还需其他算法。DFS 常用于枚举路径和判断连通性，但是否找“所有路径”取决于状态恢复、去重和输出规模，不能只凭遍历名字保证。

### DFS的核心模板

**递归实现（最常用）：**

下块是树 DFS 的局部骨架，**不可单独编译**；省略 `TreeNode` 定义和业务处理函数 `process(TreeNode*)`。若树由 `unique_ptr` 拥有，算法应只借用 `const TreeNode*` 并通过 `.get()` 进入子树。

```cpp
void dfs(TreeNode* node) {
    if (node == nullptr) return;  // 1. 基线条件
    
    // 前序位置：处理当前节点
    process(node);
    
    dfs(node->left);   // 递归左子树
    dfs(node->right);  // 递归右子树
    
    // 后序位置：处理完子树后的操作
}
```

每次递归调用都有自己的栈帧，保存参数、局部变量和返回位置；“回溯”就是子调用返回后，父调用从下一条语句继续。写递归 DFS 时应先说清函数契约，例如“`dfs(node)` 把以 `node` 为根的前序结果追加到 `result`”，再检查三件事：空节点能终止、每次调用都朝更小子问题前进、返回时不变量仍成立。树高为 `h` 时额外栈空间是 O(h)；退化树可能让 `h = n` 并耗尽调用栈，此时应改用显式栈或限制输入深度。

**迭代实现（使用栈）：**

下块是迭代 DFS 的局部骨架，**不可单独编译**；省略 `<stack>`、`std::` 限定、`TreeNode` 和 `process` 定义。栈里的指针是观察者，不延长节点生命。

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

显式栈不是“自动等价于递归”的魔法：前序遍历要先压右孩子再压左孩子，因为栈后进先出；若调换顺序，访问集合不变但顺序改变。需要模拟后序位置时，还要保存“节点是否已经展开”等额外状态，而不能只保存节点指针。

### 从树推广到图：必须有 visited

树在父子方向上没有环，所以从根向下通常不需要访问标记；一般图可能出现 `0 -> 1 -> 2 -> 0`，若直接套树模板会无限递归。正确图 DFS 在进入顶点时立刻标记 `visited[v] = true`，再递归尚未访问的邻居；如果等到返回时才标记，环上的顶点会在标记前再次进入。时间复杂度是 O(V + E)，额外空间包括 O(V) 的 visited、遍历结果和最坏 O(V) 的递归栈。

下块是图 DFS 的局部骨架，**不可单独编译**；省略 `<cstddef>`、`<vector>`，并假定 `using Graph = std::vector<std::vector<std::size_t>>`。入口还必须先验证 `v < graph.size()` 且 `visited.size() == graph.size()`。

```cpp
void dfsGraph(const Graph& graph, std::size_t v, std::vector<bool>& visited) {
    if (visited[v]) return;
    visited[v] = true;                 // 必须先标记
    for (std::size_t next : graph[v]) {
        dfsGraph(graph, next, visited);
    }
}
```

路径搜索和组合枚举还会维护“当前路径”这类可变状态，递归前 `push_back` 做选择，递归后 `pop_back` 撤销选择；忘记撤销会让兄弟分支互相污染。若只问“是否存在”，找到答案后可短路返回；若要“所有方案”，则不能在第一个答案处停止，这也是算法接口契约的一部分。

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

原子操作按标准规定作为一个不可分割的修改或读取参与该原子对象的修改顺序，不会暴露“写了一半”的中间状态。C++11 的 `std::atomic<T>` 允许多个线程通过原子操作访问同一个原子对象，但它只直接保护该对象上的原子操作；“先检查 A 再修改 B”这类跨对象不变量不会因为 A、B 各自是 atomic 就自动成立。C++17 中主模板要求 `T` 是可平凡复制的合格类型，并不是任意用户类型包一层 `atomic` 都合法。

许多常见原子类型可由 CPU 指令实现，但标准不保证所有 `atomic<T>` 都 lock-free，较大类型可能调用内部锁；可用 `is_lock_free()` 查询当前对象在当前实现上的结果。即使无锁也不等于更快、更容易写对或 wait-free：竞争下的 CAS 重试可能持续失败。需要共同维护队列、余额和状态机等多个字段时，清晰的 `mutex` 临界区通常是更合适的替代方案。

普通对象被两个线程并发访问且至少一个写、又没有 happens-before 关系时会产生**数据竞争**，其结果是未定义行为，不只是“偶尔读到旧值”。把计数器改为 atomic 可以修复该对象的数据竞争，但不能把邻近普通数据自动发布给其他线程；发布数据需要正确的同步关系。

### 基本使用

```cpp
#include <atomic>

int main() {
    std::atomic<int> counter(0);
    std::atomic<bool> flag(false);
    std::atomic<int*> ptr(nullptr);

    counter.store(10);
    const int value = counter.load();
    const int old = counter.exchange(20);
    static_cast<void>(counter.fetch_add(1));
    static_cast<void>(counter.fetch_sub(1));
    ++counter;

    int expected = 10;
    const bool success = counter.compare_exchange_strong(expected, 20);
    // 失败时 expected 被改写为原子对象的实际值 21。
    return value == 10 && old == 10 && !success && expected == 21 &&
                   counter.load() == 21 && !flag.load() && ptr.load() == nullptr
               ? 0
               : 1;
}
```

CAS 的失败路径会把原子对象的实际值写回 `expected`，所以循环重试时不能假设 `expected` 保持原值。`compare_exchange_weak` 还允许匹配时虚假失败，适合本来就要循环的代码；`strong` 适合一次尝试。复合更新应尽量使用 `fetch_add` 等直接的读改写操作，避免手写“load 后 store”造成丢失更新。

### 内存序（Memory Order）

C++11提供了多种内存序选项，用于控制原子操作的可见性约束：

| 内存序 | 说明 | 使用场景 |
|-------|------|---------|
| `memory_order_relaxed` | 只保证该原子操作和该对象的修改顺序，不发布其他数据 | 只关心最终数值的统计计数 |
| `memory_order_acquire` | 读到匹配 release 写入的值后，可见 release 之前的写入 | 消费已发布的数据 |
| `memory_order_release` | 把此前写入发布给读到它的 acquire 操作 | 发布数据后设置就绪标志 |
| `memory_order_acq_rel` | 在同一个读改写操作上同时 acquire 和 release | 状态机或引用计数的特定步骤 |
| `memory_order_seq_cst` | 默认；除 acquire/release 关系外，所有 seq_cst 操作还有单一全序 | 首先保证正确性、尚无证据需要放宽时 |

典型发布模式是：写线程先写普通 `payload`，再对 `ready.store(true, release)`；读线程以 acquire 读到 `true` 后再读 payload。只有读操作真的读到该 release 写入或其 release sequence 中的值，才建立 synchronizes-with 与 happens-before。若两边都用 relaxed，ready 本身仍无数据竞争，但 payload 的跨线程访问仍可能是未定义行为；因此内存序不是“性能等级”，而是算法证明的一部分。

表中没有把 `memory_order_consume` 作为入门方案：C++17 标准仍有该枚举值，但依赖序关系难以正确使用，主流实现长期倾向按 acquire 处理，C++26 又将其弃用。课程先用默认 `seq_cst` 建立正确性，再只在能写出 release/acquire 证明并有测量依据时放宽；不能把弱内存序当成“免费加速开关”。

---

## 📖 知识点三：EMC++ Item 39-40

<a id="item-39"></a>

### Item 39: 使用 `void future` 进行一次性事件通信

对于只需要发生一次的通知，可以使用 `std::promise<void>` 和 `std::future<void>`。它不传递业务值，只通过共享状态表达“事件已发生”，并且不会像条件变量那样发生虚假唤醒。

```cpp
#include <future>
#include <thread>
#include <utility>

int main() {
    std::promise<void> ready_promise;
    std::future<void> ready_future = ready_promise.get_future();
    bool event_observed = false;

    std::thread waiter([future = std::move(ready_future), &event_observed]() mutable {
        future.wait();
        event_observed = true;
    });
    ready_promise.set_value();
    waiter.join();
    return event_observed ? 0 : 1;
}
```

与条件变量的裸通知不同，共享状态会记住“已就绪”：即使 `set_value()` 先发生，后来的 `wait()` 也会立即返回，因此不需要用 sleep 确认等待线程已经启动。`set_value()` 与成功等待建立同步，通知前写入的普通数据可在等待后安全读取；异常也可以用 `set_exception()` 传递。

边界要点：`get_future()` 对一个 promise 只能成功调用一次，普通 `future` 的 `get()` 也只能消费一次；需要多个等待者时，在启动线程前转换成可复制的 `shared_future`。promise 只能满足一次，重复 `set_value()` 会抛出 `std::future_error`；若 promise 未满足就销毁，等待者会收到 `broken_promise`。void future 适合一次性事件，不适合可重复通知、队列条件或取消协议，这些场景仍需要条件变量、队列状态或更明确的停止对象。

<a id="item-40"></a>

### Item 40: 对并发使用 `std::atomic`，对特殊内存使用 `volatile`

`std::atomic`用于并发编程，`volatile`用于特殊内存（如内存映射I/O）。两者用途完全不同，不能混用。

**volatile 不能提供的能力：**
- 不保证原子性
- 不保证可见性（内存序）
- 不能用于线程同步

**正确的使用方式：**

```cpp
#include <atomic>

int main() {
    std::atomic<int> counter(0);
    ++counter;  // 多线程共享时，这个读改写是原子的

    volatile int device_like_value = 0;
    ++device_like_value;  // volatile 访问仍不提供线程同步
    return counter.load() == 1 && device_like_value == 1 ? 0 : 1;
}
```

`volatile` 不能提供原子性、线程间 happens-before 关系或互斥；它让某些访问保留为抽象机可观察的副作用，但具体硬件语义仍由实现和平台接口决定。它可能出现在内存映射设备寄存器等“非 C++ 内存”接口中，真实代码还必须遵守平台提供的地址映射、访问宽度、屏障和设备内存模型，不能凭空把整数地址强转后解引用。本日可执行代码只演示 `atomic` 的并发用途，不伪造不可移植的硬件寄存器。

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

在线评测通常用裸 `TreeNode*` 表示平台拥有的节点；本地测试则让节点以 `std::unique_ptr` 独占孩子，深度函数接收 `const TreeNode*` 作为只读观察指针。这样算法契约仍与题意一致，同时测试树在作用域结束时自动释放，不需要容易遗漏的手写 `deleteTree`。

下块是 LeetCode 的局部递归片段，**不可单独编译**；省略 `<algorithm>`、`TreeNode` 定义和 `std::max` 限定。本地工程对应接口为 `const TreeNode*`，不修改树。

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
- 空间复杂度：O(h)，递归栈深度；退化树中 h 可达到 n

---

### 实战题：LC 111 二叉树的最小深度

#### 题目链接
[LeetCode 111](https://leetcode.cn/problems/minimum-depth-of-binary-tree/)

#### 题目描述
给定一个二叉树，找出其最小深度。最小深度是从根节点到最近叶子节点的最短路径上的节点数量。

#### 解题思路

注意：最小深度的定义是到**叶子节点**的最短路径，如果左子树为空，不能直接返回0。

下块是 LeetCode 的局部递归片段，**不可单独编译**；省略 `<algorithm>`、`TreeNode` 定义和 `std::min` 限定。单侧为空时必须走另一侧，不能把空孩子当叶子。

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

### 今日工程动作：把边界变成自动测试

```bash
cd week_05/day_32
./build_and_run.sh
```

脚本每次从空 `build/` 开始，以 C++17 Release 配置和 `-Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow -Werror` 编译，再运行 8 个 CTest；任一检查失败都由程序返回非零。执行后应确认递归/迭代/有环图、深度边界、一次性事件和原子同步都由自动测试判断，而不是依赖输出长得像预期或依赖线程时序碰巧发生。

---

## 📚 相关术语

| 术语 | 英文 | 定义 |
|------|------|------|
| 深度优先搜索 | Depth-First Search | 沿着一条路径深入再回溯的搜索策略 |
| 原子操作 | Atomic Operation | 不可分割的操作 |
| 内存序 | Memory Order | 原子操作的可见性约束 |
| CAS | Compare-And-Swap | 比较并交换，原子操作的基础 |
| volatile | 特殊访问限定 | 用于平台定义的特殊内存访问，不提供线程同步 |
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

1. [C++ working draft：atomics](https://eel.is/c++draft/atomics)
2. [cppreference：`std::atomic`](https://en.cppreference.com/w/cpp/atomic/atomic.html)
3. [cppreference：`std::memory_order`](https://en.cppreference.com/w/cpp/atomic/memory_order.html)
4. [C++ Core Guidelines CP.8 / CP.200：`volatile` 不是线程同步](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rconc-volatile)
5. [LeetCode 104 - Maximum Depth](https://leetcode.com/problems/maximum-depth-of-binary-tree/)
6. [LeetCode 111 - Minimum Depth](https://leetcode.com/problems/minimum-depth-of-binary-tree/)
7. Scott Meyers, *Effective Modern C++*, Item 39–40
8. Anthony Williams, *C++ Concurrency in Action*（原子操作与 C++ 内存模型）

## 每日复盘（恰好五句）

1. 我能写出带明确基线条件和函数契约的递归 DFS，并在深树上选择显式栈替代递归。
2. 我能解释迭代 DFS 的压栈顺序，并在有环图中于递归前标记 visited。
3. 我能区分单个原子操作、跨字段不变量和 lock-free，实现复杂状态时会考虑 mutex。
4. 我能用 release/acquire 的 synchronizes-with 关系证明普通 payload 的发布，而不会把 relaxed 当作通用同步。
5. 我能准确说明 Item 39 的一次性 void future 和 Item 40 的 atomic/volatile 边界及各自失败方式。
