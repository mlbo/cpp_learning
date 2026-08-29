# STL 容器底层原理

> 本教程补齐「STL 容器底层」这条线：`vector` 的扩容机制、`list`/`deque` 的底层结构、`map`/`set` 的红黑树、`unordered_map`/`unordered_set` 的哈希桶与 rehash，以及一张完整的迭代器失效规则表。这些是大厂 C++ 面试高频考点，而基础教程里只在"标准容器增强"一节顺带提过一句 rehash，没有集中讲解。
>
> 阅读前建议先看 [C++ 基础学习教程](CPP基础学习教程.md) 第 0.5 节（数组与 vector）和第 11 节（标准容器增强）。本教程聚焦"底层实现与代价"，不重复 API 用法。

本篇统一使用 **C++17** 编译。每个代码块标明性质："完整可运行程序"可单独保存为 `.cpp` 并按 C++17 编译运行；"上下文片段"放进已有上下文后验证。

关于「实现相关」的标注：扩容倍数、桶数量、负载因子默认值、`max_load_factor` 等具体数值是**实现定义**的——本教程给出 libstdc++（g++ 的标准库）在 **64 位 Linux、g++ 11** 下的实测值，MSVC 和 libc++ 可能不同。文中凡实现相关结论都会标注。

---

## 目录

1. [vector：动态数组与扩容](#1-vector动态数组与扩容)
2. [list 与 deque](#2-list-与-deque)
3. [map 与 set：红黑树](#3-map-与-set红黑树)
4. [unordered_map 与 unordered_set：哈希桶](#4-unordered_map-与-unordered_set哈希桶)
5. [迭代器失效规则一览表](#5-迭代器失效规则一览表)

---

## 1. vector：动态数组与扩容

`std::vector` 是动态数组：元素在内存中**连续存储**，支持下标随机访问 O(1)，尾部增删均摊 O(1)。它维护两个计数：`size()`（已存元素数）和 `capacity()`（已分配但可用的槽位数，`size <= capacity`）。

### 1.1 capacity vs size

`size` 是逻辑长度，`capacity` 是物理容量。`push_back` 在 `size < capacity` 时只往已有空间写，O(1)；当 `size == capacity` 时需要重新分配更大空间、搬移旧元素、释放旧空间，这一次是 O(n)。把单次 O(n) 摊到多次 `push_back` 上，平均仍是 O(1)——这就是"均摊常数时间"。

```
vector 内部：一块连续内存，前 size 个槽已放元素，后面 [size, capacity) 是预留空槽

下标:  0   1   2   3   4   5   6   7
     +---+---+---+---+---+---+---+---+
     | 1 | 2 | 3 | . | . | . | . | . |   size=3, capacity=8
     +---+---+---+---+---+---+---+---+
     <---已存---> <--- 预留(未初始化) --->
     push_back 时若 size<capacity：写入 [size]，size++，O(1)
     push_back 时若 size==capacity：换大块、搬过去、释放旧块，O(n)
```

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v;
    std::cout << "init size=" << v.size() << " cap=" << v.capacity() << '\n';
    for (int i = 0; i < 10; ++i) {
        v.push_back(i);
        std::cout << "after push " << i << ": size=" << v.size() << " cap=" << v.capacity() << '\n';
    }
}
```

输出（libstdc++，64 位 g++，实现相关）：
```
init size=0 cap=0
after push 0: size=1 cap=1
after push 1: size=2 cap=2
after push 2: size=3 cap=4
after push 3: size=4 cap=4
after push 4: size=5 cap=8
after push 5: size=6 cap=8
after push 6: size=7 cap=8
after push 7: size=8 cap=8
after push 8: size=9 cap=16
after push 9: size=10 cap=16
```

容量序列 1→2→4→8→16，每次不够时**翻倍**。注意这是 libstdc++ 的策略；MSVC 的 `std::vector` 增长因子约 **1.5 倍**（1→2→3→4→6→9…）。倍数是实现定义的，标准只要求均摊 O(1)。

#### 为什么是 2 或 1.5？——增长因子的内存复用权衡

"任选大于 1 的因子都能保证均摊 O(1)"，但选哪个因子关系到**已释放的旧内存能否被复用**。考虑扩容序列 `C, C·f, C·f², C·f³, …`：新请求的块若小于"此前所有已释放块之和"，分配器才可能复用旧块。

- **因子 2 是理论上最差**：`C + 2C + 4C + … + 2ⁿC = 2ⁿ⁺¹C − 1 > 2ⁿC`，即每次请求的新块都严格大于此前所有已释放块之和——`vector` 只能在内存里"一路向前爬"，永远回不到旧块。Facebook folly 的 FBVector 文档明确指出 2 是"可被证明的最差选择"。
- **理论极限是黄金比例 φ ≈ 1.618**（`(1+√5)/2`）：由 Howard Hinnant 推导的 `f^i − 1 ≥ f^(i+1)(f−1)` 给出"恰好能复用"的临界值。任何 < φ 的因子都允许在某次扩容后复用此前释放的块。
- **1.5 留出复用空间**：用 1.5 时约 4 次扩容后就能复用此前释放的块；1.45 约 3 次、1.3 约 2 次。MSVC/Dinkumware 选 1.5 是"内存利用率与扩容效率的折中"；libstdc++ 仍坚持 2，对缓存和分配器都不友好（这也是 folly 用 1.5 的动机）。

所以"为什么 libstdc++ 是 2"其实是个**待改进的历史遗留**，不是最优选择。理解了这段，面试问"vector 扩容倍数为什么是 2 或 1.5""能不能自己定"都能答上来：倍数是实现定义、标准只要求均摊 O(1)、1.5 在内存复用上更优、φ 是理论分界。

### 1.2 扩容致迭代器/指针/引用失效

扩容意味着"换一块新内存、把元素搬过去、释放旧内存"。所以扩容后，**所有**指向旧元素的迭代器、指针、引用全部失效（指向已释放的旧地址）。这是 vector 最常见的坑。

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v = {1, 2, 3};
    int* p = &v[0];
    v.push_back(4);   // 可能扩容：旧地址失效
    // 此时 p 可能指向已释放内存；解引用 *p 是未定义行为
    std::cout << "p still points to old buffer? "
              << (p == &v[0] ? "yes(same)" : "no(reallocated, p is invalid)") << '\n';
}
```

输出（libstdc++，64 位 g++，实现相关）：
```
p still points to old buffer? no(reallocated, p is invalid)
```

规则：**保存了 `begin()`、`&v[0]`、`&v[i]` 或某元素的指针/引用后，只要后续可能扩容（`push_back`、`emplace_back`、`insert`），就必须重新获取它们。** 用 `range-for` 的 `&v[0]` 配合 `push_back` 是经典 bug。

```
扩容前后，旧缓冲区整块被释放、换了新地址：

扩容前（size==capacity==3，p=&v[0]）：        push_back(4) 触发扩容（换 8 槽新块）后：
  v 的缓冲                                  v 的新缓冲（新地址）
 +---+---+---+                              +---+---+---+---+---+---+---+---+
 | 1 | 2 | 3 |   cap=3                      | 1 | 2 | 3 | 4 | . | . | . | . | cap=8
 +---+---+---+                              +---+---+---+---+---+---+---+---+
   ^                                          ^
   p                                          &v[0]（已变到新地址）
   旧块已释放 → p 悬空，*p 是 UB              p 仍指向旧（已释放）地址 → 失效
```

`p` 保存的是扩容前的地址，扩容后那块内存已还给分配器。继续用 `*p` 是 use-after-free（未定义行为）。

### 1.3 reserve vs resize

两者都和 capacity/size 有关，但用途不同：

- `reserve(n)`：**只改 capacity**，不创建元素，`size` 不变。预分配避免后续多次扩容，是性能优化的关键手段。
- `resize(n)`：**改 size**。变大时新位置元素被值初始化（`int` 为 0），变小时多余元素被析构。capacity 可能随之增长。

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v;
    v.reserve(100);                       // 预分配 100 容量，size 仍为 0
    std::cout << "after reserve(100): size=" << v.size() << " cap=" << v.capacity() << '\n';
    // 注意：reserve 只改 capacity、size 仍为 0，此时 v[0] 越界（UB）；要安全取首元素地址须先有元素，如用 v.data()

    v.resize(5);                          // size 变 5，5 个元素值初始化为 0
    std::cout << "after resize(5): size=" << v.size() << " cap=" << v.capacity() << '\n';
    for (int x : v) std::cout << x << ' ';
    std::cout << '\n';

    v.reserve(10);                        // 请求 capacity≥10，但因已为 100 不会缩小
    std::cout << "after reserve(10): size=" << v.size() << " cap=" << v.capacity() << '\n';
}
```

输出（libstdc++，实现相关）：
```
after reserve(100): size=0 cap=100
after resize(5): size=5 cap=100
0 0 0 0 0
after reserve(10): size=5 cap=100
```

注意 `reserve(100)` 已经把 capacity 提到 100，所以后续 `resize(5)` 只把 size 调到 5（5 个元素值初始化为 0），capacity 仍是 100；`reserve(10)` 请求的容量比当前 100 小，不会缩小，capacity 保持 100。`resize` 改的是逻辑长度，`reserve` 改的是物理容量——这张对照清楚地显示了二者的区别。

实践：已知元素数量时，先 `reserve` 再循环 `push_back`，能避免多次扩容的搬移开销。

### 1.4 emplace_back vs push_back

- `push_back(x)`：要求一个已存在的元素对象，再把它拷贝/移动进容器。
- `emplace_back(args...)`：直接在容器的内存位置**原地构造**元素，参数转发给构造函数，省一次临时对象的构造和移动。

> **代码性质：上下文片段。** 展示 emplace 原地构造的优势，需放进含相关类型定义的上下文。

```cpp
struct Widget {
    Widget(int a, const std::string& b);
};
std::vector<Widget> v;
v.push_back(Widget(1, "x"));      // 构造临时 Widget，再移动进容器
v.emplace_back(1, "x");           // 直接在容器槽位构造 Widget，无临时对象
```

对不可移动或构造昂贵的类型，`emplace_back` 能省一次构造+移动。但对已存在对象或基本类型，两者没差别，甚至 `push_back` 可读性更好。

---

## 2. list 与 deque

### 2.1 list：双向链表

`std::list` 是双向链表：每个节点存元素和前后指针。特点：

- 插入/删除任意位置 O(1)（已知迭代器位置时），不需要搬移其他元素。
- 随机访问 O(n)，`list[i]` 不存在。
- 内存不连续，缓存不友好。
- 插入和删除**不失效**其他元素的迭代器（只失效被删元素本身）。

`std::forward_list` 是单向链表，更省内存（每节点一个指针），但只能向前遍历。

### 2.2 deque：分段连续的双端队列

`std::deque`（double-ended queue）不是单纯链表，而是**分段连续**结构：内部维护一个"指向多个固定大小缓冲块的指针数组"，每个缓冲块连续存元素。特点：

```
deque 的分段连续结构（示意）：

  map 数组（指向各缓冲块的指针）
 +-------+-------+-------+-------+
 |  *----+--*----+--*----+  NULL |  ← 头部索引在中间，前后可扩
 +-------+-|-----+-|-----+-|-----+
           |       |       |
           v       v       v
 块0: [ . . . . ][ 块1: a b c d ][ 块2: e f g . ]   ← 每块定长、连续
        预留         ^back               ^front
   push_front 写块0、push_back 写块2：O(1)，且元素地址不变（只在块满换块时才动）
```

- 随机访问 O(1)：先算元素在第几块（`index / 块大小`）、块内偏移（`index % 块大小`），两步寻址。
- 头尾插入/删除 O(1)，且元素指针/引用不失效（中间插入会失效，且常数较大）。
- 比纯链表缓存友好，但又不像 `vector` 那样整体连续。

`std::stack`、`std::queue` 默认用 `deque` 作底层容器（容器适配器）。`std::stack` 只暴露 `push`/`pop`/`top`，隐藏了 deque 的其他接口。

---

## 3. map 与 set：红黑树

`std::map` 和 `std::set` 底层是**红黑树**——一种自平衡二叉搜索树。红黑树通过"每个节点带颜色（红/黑）+ 五条性质约束"保证任意路径长度不超过最短路径的两倍，从而保证树高 O(log n)，使增删查都为 O(log n)。

`map<int,string> m = {{3,"c"},{1,"a"},{2,"b"},{5,"e"},{4,"d"}}` 的红黑树大致长这样（颜色为示意，满足红黑树性质）：

```
            [3,c] 黑
           /       \
       [1,a] 黑    [5,e] 黑
          \         /
        [2,b] 红 [4,d] 红

中序遍历（左→根→右）: 1 → 2 → 3 → 4 → 5   ← 这就是 map "按键有序"的来源
增删查：每次从根 3 出发比较，沿树高走，O(log n)
```

红黑树的五条性质保证：根黑、叶(nil)黑、红节点的孩子必黑、任一节点到其叶的**所有**路径上黑节点数相同（黑高相等）、由此推出最长路径 ≤ 2×最短路径 → 树高 O(log n)。颜色在插入/删除后用旋转和重新染色来恢复，这就是"自平衡"的代价所在。

### 3.1 关键特性

- **有序**：中序遍历得到按键升序的序列。下面验证 map 遍历有序。
- **复杂度**：插入、删除、查找都是 O(log n)。
- **迭代器稳定性**：插入不失效已有迭代器；删除只失效被删节点的迭代器。
- `map` 存键值对（`pair<const Key, T>`），`set` 只存键。`multimap`/`multiset` 允许重复键。

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <iostream>
#include <map>
#include <string>

int main() {
    std::map<int, std::string> m = {{3, "c"}, {1, "a"}, {2, "b"}};
    std::cout << "inorder: ";
    for (const auto& [k, v] : m) std::cout << k << ' ';
    std::cout << '\n';            // 1 2 3：按键有序
    std::cout << "find(2)=" << m.find(2)->second << '\n';
}
```

输出（实现无关）：
```
inorder: 1 2 3
find(2)=b
```

插入顺序是 3,1,2，但中序遍历得到 1,2,3——这是红黑树有序性的直接体现，也是 `map` 与 `unordered_map` 的核心区别。

### 3.2 为什么是红黑树而不是 AVL / B+ 树

面试常问"为什么标准库 map 用红黑树"。简要回答：

- **vs AVL 树**：两者增删查都是 O(log n)，差别在**重新平衡的代价**。AVL 平衡更严格（任意节点左右子树高度差 ≤ 1），插入重平衡只需 1 次（单/双旋转），但**删除**可能从被删处向上传播、需要 O(log n) 次旋转。红黑树放松了平衡条件（只保证最长路径 ≤ 2×最短路径），插入重平衡最多 2 次旋转、删除最多 3 次——增删时的旋转次数更少且更稳定，对"增删查并重"的通用关联容器更优。AVL 查找略快（更平衡→树更矮），适合"查多改少"的场景。一个更现代的观察：不少实测里 AVL 在查找密集场景比红黑树快 20%–40%，但标准库要兼顾增删，选了红黑树。
- **vs B+ 树**：B+ 树面向磁盘/数据库场景（块读、高扇出、缓存行友好），内存里用红黑树节点更小、指针操作更直接。B+ 树适合外存索引，不适合作为通用内存容器。
- **类型要求最小化**（Stepanov 的设计动机）：`std::map` 只要求键有 `operator<`（严格弱序），不要求哈希函数。这让任何可比的类型都能当键，泛用性最广；哈希表则要求每个键类型都提供哈希和相等比较——这正是 `unordered_map` 直到 C++11 才进入标准、且需要额外 `Hash`/`Pred` 模板参数的原因。
- **现代视角的注脚**：STL 之父 Stepanov 后来表示，若重写 `std::map`，会选对缓存更友好的 **B* 树**而非红黑树——节点式结构（链表、红黑树）局部性差，现代 CPU cache miss 代价高。这印证了红黑树是历史选择，不是最优。

这些只是工程权衡，不是绝对——选择取决于场景。

---

## 4. unordered_map 与 unordered_set：哈希桶

`std::unordered_map`/`unordered_set` 底层是**哈希表**。每个元素经哈希函数映射到一个桶（bucket），桶内用链表（libstdc++ 单链表）串起冲突元素。注意 C++ 标准的 `unordered_map` **不会**像 Java 的 HashMap 那样把过长桶链表自动转成树——桶内始终是链表，所以最坏情况（大量键冲突落入同一桶）查找仍是 O(n)。平均 O(1) 依赖哈希分布好、负载因子受控。

```
unordered_map 的桶 + 链表结构（示意，桶数=8）：

bucket_count()=8       哈希：h(key) % 8 决定落哪个桶
 +----+
 | 0 | -> [k=h2..]                                                元素落在第 0 桶
 +----+
 | 1 | -> NULL
 +----+
 | 2 | -> [k=h6..] -> [k=h14..] -> [k=h30..]  → 一个桶里多个 = 冲突，挂链表
 +----+
 | 3 | -> [k=h3..]
 +----+
 | 4 | -> NULL
 +----+
 |.. |
 +----+
 | 7 | -> [k=h15..]

 查找 k：先算 h(k)%8 定到桶，再沿桶内链表逐个比 == 。分布好→链表短→O(1)；
        全挤进一个桶→链表长→退化 O(n)（C++ 标准下桶内始终是链表，不转树，没有 O(log n) 上限）。
 load_factor = size/bucket_count；超过 max_load_factor(默认1.0) → rehash：换更多桶、重排所有元素
```

### 4.1 关键概念

- **平均 O(1)，最坏 O(n)**：哈希分布好时查找接近常数；大量键冲突落入同一桶时退化为线性。
- **负载因子** = `size() / bucket_count()`。`max_load_factor()` 默认为 **1.0**（libstdc++）。
- **rehash 触发**：当 `load_factor > max_load_factor` 时，容器重新分配更多桶、把所有元素重新哈希到新桶。rehash 平均 O(n)、最坏 O(n²)，并让**所有迭代器失效**。注意一个关键差别（cppreference）：指向键/值的**指针和引用**在 rehash 后仍有效（元素节点没被销毁，只是换桶），失效的只是迭代器。
- **无序**：遍历顺序与插入或键的大小无关，只取决于哈希值和桶。

> **代码性质：完整可运行程序。** 可单独保存为 `.cpp` 文件，并按 C++17 编译运行。

```cpp
#include <iostream>
#include <unordered_map>

int main() {
    std::unordered_map<int, int> h;
    std::cout << "init buckets=" << h.bucket_count()
              << " max_load_factor=" << h.max_load_factor() << '\n';
    size_t prev_b = h.bucket_count();
    int rehashes = 0;
    for (int i = 0; i < 1000; ++i) {
        h[i] = i;
        if (h.bucket_count() != prev_b) { ++rehashes; prev_b = h.bucket_count(); }
    }
    std::cout << "after 1000 inserts: buckets=" << h.bucket_count()
              << " size=" << h.size()
              << " load=" << h.load_factor()
              << " rehashes=" << rehashes << '\n';
}
```

输出（libstdc++，64 位 g++，实现相关）：
```
init buckets=1 max_load_factor=1
after 1000 inserts: buckets=1109 size=1000 load=0.901713 rehashes=7
```

`max_load_factor` 默认 1.0。插入 1000 个元素过程中桶数从 1 涨到 1109（7 次 rehash），最终负载因子 0.90 < 1.0。`load` 始终被压在 `max_load_factor` 以下。

### 4.2 哈希冲突：开放寻址 vs 链表法

两种主流冲突处理：

- **链表法（separate chaining）**：每个桶是一个链表，冲突元素挂链表上。libstdc++ 的 `unordered_*` 用此法。优点是不易聚集、负载因子可超 1；缺点是指针跳转破坏缓存。
- **开放寻址（open addressing）**：冲突时按探测策略（线性探测、二次探测、双哈希）找下一个空槽。缓存友好，但删除复杂（需惰性删除）、负载因子不能太高。标准库 `unordered_*` 不用开放寻址，但很多高性能哈希表（如 `absl::flat_hash_map`）用。

`operator[]` 的陷阱：`unordered_map[key]` 在键不存在时会**插入一个默认值**，这既改变了容器（rehash 可能触发），也不是纯查询。只想查询时应使用 `find` 或 C++20 的 `contains`。

### 4.3 自定义类型作键

要用自定义类型作 `unordered_map` 的键，需要提供该类型的哈希函数（一个返回 `size_t` 的可调用对象）和相等比较。标准库只对内置类型和部分标准类型提供了默认 `std::hash`。

> **代码性质：上下文片段。** 展示自定义哈希与相等比较，需放进可编译上下文。

```cpp
#include <unordered_map>
#include <string>

struct Pair {
    int a, b;
    bool operator==(const Pair& o) const { return a == o.a && b == o.b; }
};

struct PairHash {
    size_t operator()(const Pair& p) const noexcept {
        return std::hash<int>()(p.a) ^ (std::hash<int>()(p.b) << 1);  // 简化哈希
    }
};

std::unordered_map<Pair, int, PairHash> m;   // 提供哈希与相等比较
```

`std::map` 则不需要哈希，只需要键有 `operator<`（严格弱序），因为它走红黑树。这是选择 `map` 还是 `unordered_map` 的一个考量点。

### 4.4 map vs unordered_map 选型

| 维度 | `map` | `unordered_map` |
|---|---|---|
| 底层 | 红黑树 | 哈希桶 |
| 查找 | O(log n) | 平均 O(1)，最坏 O(n) |
| 有序性 | 有序遍历 | 无序 |
| 迭代器稳定性 | 插入不失效；删除只失效被删 | rehash 全失效 |
| 内存 | 节点分散，开销较大 | 桶+链表，常更省 |
| 键要求 | `operator<` | `==` + `hash` |

需要有序遍历、找前驱/后继、范围查询选 `map`；只要快速查找、不关心顺序选 `unordered_map`。

---

## 5. 迭代器失效规则一览表

这是面试必背的表。规则按"插入"和"删除"分别列：

| 容器 | 插入失效 | 删除失效 |
|---|---|---|
| `vector` | 全部失效（扩容时）；不扩容时插入点之后失效 | 删除点之后失效 |
| `deque` | 全部失效 | 中间删除全部失效；端部删除只失效端 |
| `list` / `forward_list` | 不失效 | 只失效被删元素 |
| `map` / `set` / `multimap` / `multiset` | 不失效 | 只失效被删元素 |
| `unordered_map` / `unordered_set` | rehash 时全部失效；否则不失效 | 只失效被删元素 |

记忆要点：

- **节点式容器**（`list`、`map`、`set`）插入不影响其他节点，删除只动被删节点——最"安全"。
- **连续存储**（`vector`）插入/删除要搬移元素，插入点/删除点之后的迭代器都失效；扩容更是全军覆没。
- **`deque`** 最"脆弱"：任何插入都让全部迭代器失效。
- **`unordered_*`** 的特殊点是 rehash：平时和节点式容器一样安全，但 rehash 时全失效。删除 `unordered_*` 元素时，注意 `erase` 返回的是下一个有效迭代器（C++14 起），用它来继续遍历，避免用已删迭代器。
- **迭代器失效 ≠ 指针/引用失效**（cppreference 的关键区分）：对 `unordered_*`，插入或 rehash 可能失效迭代器，但指向键/值的**指针和引用只在 erase 被删元素时才失效**——rehash 不搬动元素节点，只换桶，所以现存元素的地址不变。这条对需要稳定持有元素地址的代码很重要。

实践模式：在循环里边遍历边删除时，用 `it = c.erase(it);` 接住返回的下一个迭代器，而不是 `c.erase(it); ++it;`（后者对失效迭代器 `++it` 是未定义行为）。这一条对 `vector`、`list`、`map`、`unordered_map` 都适用。

---

## 附录：参考资料

1. **cppreference**：https://en.cppreference.com/w/ —— `vector`、`map`、`unordered_map` 各词条，含复杂度与迭代器失效的标准化表格；`unordered_map` 的迭代器失效规则见 [unordered_map] 成员函数表。
2. **C++ working draft**：https://eel.is/c++draft/ —— [vector.capacity]、[unord.req] 节。
3. **facebook/folly FBVector.md**：https://github.com/facebook/folly/blob/main/folly/docs/FBVector.md —— 增长因子 2 是"理论上最差"、1.5 在内存复用上更优的论证。
4. **Howard Hinnant，Growth Factor derivation for reallocating a buffer** —— 增长因子理论极限 φ≈1.618 的推导（Boost 邮件列表存档）。
5. **Arthur O'Dwyer，dynamic_cast from scratch** / Itanium C++ ABI —— `dynamic_cast` 与 RTTI 的实现机制。
6. **Matt Austern，*Generic Programming and the STL*** —— 容器复杂度与失效规则的设计动机。
7. **侯捷，*STL 源码剖析*** —— 红黑树、hashtable 的实现细节（基于 SGI STL，与 libstdc++ 接近但非完全一致）。

使用参考资料时区分**标准保证的行为**（如 `vector` 均摊 O(1)、`map` 有序）、**实现行为**（如扩容倍数、桶数、`max_load_factor` 默认值）和**设计建议**（如 reserve 预分配、选型取舍）。
