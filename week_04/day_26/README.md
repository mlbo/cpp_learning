# Day 26：CPU缓存与内存对齐

> **学习定位**：从语言语义暂时下沉到性能成本模型。本日建立局部性、缓存行、对齐和伪共享直觉；微基准只能说明特定机器和编译选项下的现象，不能把一次结果当成普遍定律。

## 📅 学习目标

- [ ] 理解CPU缓存的工作原理和层次结构
- [ ] 掌握缓存行的概念和缓存命中机制
- [ ] 了解伪共享问题及其解决方案
- [ ] 理解内存对齐的概念和对齐规则
- [ ] 掌握alignas和alignof关键字的使用
- [ ] 学习缓存友好的编程实践
- [ ] 完成LeetCode 5（最长回文子串）和647（回文子串）

---

## 📖 知识点一：CPU缓存

### 概念定义

CPU缓存（CPU Cache）是位于CPU和主内存之间的一种高速存储器，用于暂时存储CPU频繁访问的数据和指令。由于CPU的运算速度远快于主内存的访问速度，如果没有缓存，CPU将花费大量时间等待内存数据，导致性能严重下降。CPU缓存利用程序的局部性原理，通过预取和存储热点数据，显著减少CPU访问内存的次数，从而提升整体系统性能。缓存的设计是现代计算机体系结构中最重要的优化之一。

### 缓存层次结构

现代CPU通常采用多级缓存架构，但不能把“每核私有 L2、全芯片共享 L3”写成普遍拓扑。常见实现有私有 L1、私有或簇共享 L2，以及按芯粒、核心簇或插槽切分的最后级缓存；最后级缓存也未必名为 L3，更不保证所有核心等距共享。容量、包含/非包含策略、延迟和共享边界都应以目标处理器手册、操作系统拓扑信息或测量为准。下面的层级图只表达“逐级查询”的抽象关系，不断言具体机器的物理连接。

```mermaid
graph TB
    subgraph "CPU缓存层次结构"
    CPU[CPU核心] --> L1["L1 Cache<br/>容量与延迟依处理器而变"]
    L1 --> L2["L2 Cache<br/>可能私有也可能共享"]
    L2 --> L3["最后级缓存<br/>拓扑依处理器而变"]
    L3 --> RAM["主内存 DRAM<br/>延迟受平台和负载影响"]
    end
    
    style CPU fill:#ff6b6b
    style L1 fill:#feca57
    style L2 fill:#48dbfb
    style L3 fill:#1dd1a1
    style RAM fill:#5f27cd
```

### 缓存行（Cache Line）

缓存行是CPU缓存与更低层存储之间传输和一致性管理的重要单位，很多桌面与服务器处理器采用64字节，但这不是C++语言保证，也不是所有机器的固定值。当CPU访问某个地址时，硬件通常会把包含它的一整行带入缓存，从而利用空间局部性。还要注意：一个对象“大小为64字节”不代表它恰好占一个缓存行，因为起始地址可能不在缓存行边界；两个地址相差小于64字节也不代表它们属于同一行，因为它们可能跨过边界。

```mermaid
graph LR
    subgraph "假设缓存行为64字节时的示意映射"
    M1["内存块0<br/>64字节"] --> C1[缓存行0]
    M2["内存块1<br/>64字节"] --> C2[缓存行1]
    M3["内存块2<br/>64字节"] --> C3[缓存行2]
    M4["内存块3<br/>64字节"] --> C4[缓存行3]
    end
    
    style M1 fill:#74b9ff
    style M2 fill:#74b9ff
    style M3 fill:#74b9ff
    style M4 fill:#74b9ff
    style C1 fill:#55efc4
    style C2 fill:#55efc4
    style C3 fill:#55efc4
    style C4 fill:#55efc4
```

### 缓存命中与缓存缺失

缓存命中（Cache Hit）和缺失（Cache Miss）必须相对于**某一级缓存**来说。L1 缺失只表示数据不在 L1，接下来可能在 L2 或最后一级缓存命中；只有相关缓存层级都未命中时，这个简化模型才访问主内存 DRAM。强制性、容量和冲突缺失也是针对所分析的缓存层级分类；真实硬件还会受预取、包含/非包含策略和一致性流量影响。

```mermaid
flowchart TD
    A[CPU请求数据] --> B{L1 命中?}
    B -->|是| C["L1 Cache Hit<br/>返回数据"]
    B -->|否| D["L1 Cache Miss<br/>继续查询更低层"]
    D --> E{L2/最后级缓存命中?}
    E -->|是| F["从较低级缓存取回<br/>回填上层"]
    E -->|否| G["简化模型：访问主内存 DRAM<br/>取回缓存行"]
    F --> H[返回数据给CPU]
    G --> H
    
    style C fill:#00b894
    style D fill:#e17055
    style A fill:#74b9ff
```

### 伪共享（False Sharing）

伪共享是多线程编程中一个隐蔽的性能杀手。当多个线程分别修改位于同一缓存行中的不同变量时，由于缓存一致性协议的要求，会导致该缓存行在多个CPU核心之间频繁传递，造成大量缓存失效。虽然线程操作的是不同的变量，但因为它们位于同一缓存行，导致缓存行被反复"乒乓"传递，严重影响性能。解决伪共享的常用方法是使用缓存行填充或alignas对齐。

```mermaid
sequenceDiagram
    participant T1 as 线程1 (Core 0)
    participant C1 as Core 0 缓存
    participant C2 as Core 1 缓存
    participant T2 as 线程2 (Core 1)
    
    Note over C1,C2: 同一缓存行包含变量A和B
    T1->>C1: 修改变量A
    C1-->>C2: 缓存行失效通知
    C2->>T2: 需要重新加载
    T2->>C2: 修改变量B
    C2-->>C1: 缓存行失效通知
    C1->>T1: 需要重新加载
    Note over T1,T2: 乒乓效应导致性能下降
```

### 代码示例

下面这段是帮助看见伪共享现象的教学原型：它已用两个线程各自操作的 `std::atomic` 避免数据竞争，但仍只做单次计时，没有预热、重复统计和环境控制，因而输出只能是观察而不是普遍结论。`alignas(64)` 同样只是当前实验参数，不代表所有目标机的缓存行都是 64 字节；要作性能判断，应再加入预热、多次重复、中位数与异常值报告。

```cpp
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <atomic>
#include <cstdint>

// 仅用于本次布局实验，不代表运行机器一定是64字节缓存行
constexpr size_t CACHE_LINE_SIZE = 64;

// 演示伪共享问题
struct CounterBad {
    std::atomic<std::uint64_t> count1{0};
    std::atomic<std::uint64_t> count2{0};
    // 两个变量可能在同一缓存行，导致伪共享
};

// 使用缓存行填充避免伪共享
struct CounterGood {
    alignas(CACHE_LINE_SIZE) std::atomic<std::uint64_t> count1{0};
    alignas(CACHE_LINE_SIZE) std::atomic<std::uint64_t> count2{0};
};

// 测试伪共享影响
void testFalseSharing() {
    std::cout << "\n=== 伪共享测试 ===" << std::endl;
    
    const int ITERATIONS = 100'000'000;
    
    // 测试存在伪共享的情况
    CounterBad bad;
    bad.count1 = 0;
    bad.count2 = 0;
    
    auto start = std::chrono::high_resolution_clock::now();
    std::thread t1([&]() {
        for (int i = 0; i < ITERATIONS; ++i) {
            bad.count1++;
        }
    });
    std::thread t2([&]() {
        for (int i = 0; i < ITERATIONS; ++i) {
            bad.count2++;
        }
    });
    t1.join();
    t2.join();
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "存在伪共享耗时: " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() 
              << " ms" << std::endl;
    
    // 测试避免伪共享的情况
    CounterGood good;
    good.count1 = 0;
    good.count2 = 0;
    
    start = std::chrono::high_resolution_clock::now();
    std::thread t3([&]() {
        for (int i = 0; i < ITERATIONS; ++i) {
            good.count1++;
        }
    });
    std::thread t4([&]() {
        for (int i = 0; i < ITERATIONS; ++i) {
            good.count2++;
        }
    });
    t3.join();
    t4.join();
    end = std::chrono::high_resolution_clock::now();
    std::cout << "避免伪共享耗时: " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() 
              << " ms" << std::endl;
}
```

---

## 📖 知识点二：内存对齐

### 概念定义

内存对齐（Memory Alignment）是指对象地址满足该类型的 `alignof(T)` 边界要求，而不是笼统地要求地址能被 `sizeof(T)` 整除。对齐既受硬件影响，也是C++对象模型的一部分：把任意 `char` 缓冲区转成 `int*` 并解引用，不仅可能未对齐，还可能因为那里根本没有开始 `int` 对象生命周期而产生未定义行为。观察对象字节可以通过 `unsigned char` 视图完成，复制表示应优先使用 `std::memcpy` 到一个已经存在的目标对象。

### 对齐规则

不同数据类型有不同的对齐要求，必须用 `alignof` 查询当前目标。标准布局结构体通常至少要满足各成员的对齐要求，编译器会插入填充，结构体大小也会满足数组连续排列所需的边界；具体偏移和字节数属于当前ABI结果，不应照抄成跨平台常量。下面 12 字节图只是假设 `alignof(int)==4` 的常见 ABI 示例，真实程序应以 `sizeof`、`alignof` 和 `offsetof` 的输出为准。

```mermaid
graph TB
    subgraph "结构体内存布局示例"
    S1["常见ABI示例：<br/>struct Example {<br/>char a;   // 1字节<br/>// 填充到int边界（此处3字节）<br/>int b;    // 4字节<br/>char c;   // 1字节<br/>// 尾部填充（此处3字节）<br/>}<br/>本示例总大小: 12字节"]
    end
    
    subgraph "内存布局图"
    A["a (1B)"] --> P1["填充 (3B)"]
    P1 --> B["b (4B)"]
    B --> C["c (1B)"]
    C --> P2["填充 (3B)"]
    end
    
    style P1 fill:#fab1a0
    style P2 fill:#fab1a0
    style A fill:#55efc4
    style B fill:#55efc4
    style C fill:#55efc4
```

### 对齐规则详解

```cpp
#include <iostream>

// 演示内存对齐规则
struct AlignDemo {
    char a;     // 后续填充到满足 int 的对齐要求
    int b;      // 4字节
    char c;     // 后续填充到满足 double 的对齐要求；常见8字节对齐ABI下为7字节
    double d;   // 8字节
};

void printAlignment() {
    std::cout << "\n=== 内存对齐详解 ===" << std::endl;
    
    std::cout << "char 对齐要求: " << alignof(char) << " 字节" << std::endl;
    std::cout << "short 对齐要求: " << alignof(short) << " 字节" << std::endl;
    std::cout << "int 对齐要求: " << alignof(int) << " 字节" << std::endl;
    std::cout << "long 对齐要求: " << alignof(long) << " 字节" << std::endl;
    std::cout << "double 对齐要求: " << alignof(double) << " 字节" << std::endl;
    std::cout << "指针 对齐要求: " << alignof(void*) << " 字节" << std::endl;
    
    std::cout << "\nAlignDemo 结构体分析:" << std::endl;
    std::cout << "  sizeof(AlignDemo) = " << sizeof(AlignDemo) << " 字节" << std::endl;
    std::cout << "  alignof(AlignDemo) = " << alignof(AlignDemo) << " 字节" << std::endl;
    
    AlignDemo demo;
    std::cout << "\n各成员偏移量:" << std::endl;
    std::cout << "  a 偏移: " << offsetof(AlignDemo, a) << std::endl;
    std::cout << "  b 偏移: " << offsetof(AlignDemo, b) << std::endl;
    std::cout << "  c 偏移: " << offsetof(AlignDemo, c) << std::endl;
    std::cout << "  d 偏移: " << offsetof(AlignDemo, d) << std::endl;
}
```

### 性能影响

未对齐访问的代价依指令、访问是否跨边界、处理器和编译器而变：可能没有可见差异，也可能变慢，某些目标上还可能不被允许，因此不能写成“总要两次访问”或“某架构一定明显变慢”。更重要的是，C++源代码必须先满足对齐和对象生命周期规则，只有语言行为合法后才有资格讨论性能；本日源码不执行任何未对齐 `T*` 解引用，而用合法字节观察和 `memcpy` 解释边界。

### alignas 和 alignof

C++11引入了alignas和alignof关键字，使程序员能够更精确地控制数据对齐。alignof用于查询类型的对齐要求，返回一个size_t类型的值。alignas用于指定自定义的对齐要求，可以用于变量声明、类定义等场景。这两个关键字为编写跨平台、高性能的代码提供了更好的支持，特别是在SIMD编程、内存映射IO等需要特定对齐的场景中。

```cpp
#include <iostream>
#include <cstddef>

// 使用alignas指定对齐
struct alignas(16) AlignedStruct {
    int x;
    int y;
    // 整个结构体按16字节对齐
};

// 缓存行对齐
struct alignas(64) CacheLineAligned {
    int data[14];  // 64只是实验对齐参数，sizeof仍由编译器决定
};

void demoAlignasAlignof() {
    std::cout << "\n=== alignas/alignof 演示 ===" << std::endl;
    
    // alignof 示例
    std::cout << "alignof(int) = " << alignof(int) << std::endl;
    std::cout << "alignof(double) = " << alignof(double) << std::endl;
    std::cout << "alignof(AlignedStruct) = " << alignof(AlignedStruct) << std::endl;
    std::cout << "alignof(CacheLineAligned) = " << alignof(CacheLineAligned) << std::endl;
    
    // alignas 变量示例
    alignas(16) int alignedInt = 42;
    std::cout << "\nalignas(16) int 的地址: " << &alignedInt << std::endl;
    std::cout << "地址是否能被16整除: " << ((reinterpret_cast<std::uintptr_t>(&alignedInt) % 16) == 0) << std::endl;
    
    // 大小比较
    std::cout << "\n结构体大小比较:" << std::endl;
    std::cout << "sizeof(AlignedStruct) = " << sizeof(AlignedStruct) << std::endl;
    std::cout << "sizeof(CacheLineAligned) = " << sizeof(CacheLineAligned) << std::endl;
}
```

---

## 📖 知识点三：缓存友好编程实践

### 数据局部性原则

编写缓存友好的代码，核心是充分利用数据的局部性。时间局部性指最近访问的数据很可能再次被访问，空间局部性指访问某个地址后很可能访问其附近地址。在代码中，应该尽量让数据访问连续、有规律，避免随机访问模式。例如，遍历二维数组时应该按行遍历（C/C++中），而不是按列遍历，这样能充分利用缓存行。

```mermaid
graph LR
    subgraph "缓存友好 vs 不友好"
    subgraph "缓存友好（按行遍历）"
    A1["a[0][0]"] --> A2["a[0][1]"]
    A2 --> A3["a[0][2]"]
    A3 --> A4["a[0][3]"]
    end
    
    subgraph "缓存不友好（按列遍历）"
    B1["a[0][0]"] --> B2["a[1][0]"]
    B2 --> B3["a[2][0]"]
    B3 --> B4["a[3][0]"]
    end
    end
    
    style A1 fill:#00b894
    style A2 fill:#00b894
    style A3 fill:#00b894
    style A4 fill:#00b894
    style B1 fill:#e17055
    style B2 fill:#e17055
    style B3 fill:#e17055
    style B4 fill:#e17055
```

### 数组 vs 链表

从缓存角度看，数组（连续内存）比链表（离散内存）有天然优势。数组的连续存储特性使得遍历时缓存行能被高效利用，预取机制也能有效工作。而链表节点分散在内存各处，每次访问都可能导致缓存缺失。在性能敏感场景，优先选择数组或vector，或考虑使用"扁平化"的数据结构（如将链表节点预分配在连续内存池中）。

下面的单次计时代码只用来观察两种遍历形状，不能据此宣布性能结论：它没有预热、没有重复统计，而且 `volatile` 会改变生成代码。仓库中的可运行版本让两者处理相同元素并执行相同求和，Release 下预热后重复五次取中位数，同时明确差异还包含指针追踪和编译器优化。

```cpp
#include <iostream>
#include <vector>
#include <list>
#include <chrono>
#include <random>

void compareCachePerformance() {
    std::cout << "\n=== 缓存性能对比：数组 vs 链表 ===" << std::endl;
    
    const int N = 1000000;
    
    // 数组（连续内存）
    std::vector<int> arr(N);
    for (int i = 0; i < N; ++i) arr[i] = i;
    
    // 链表（离散内存）
    std::list<int> lst(arr.begin(), arr.end());
    
    volatile long long sum = 0;
    
    // 数组遍历
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        sum += arr[i];
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "数组遍历耗时: " 
              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() 
              << " μs" << std::endl;
    
    // 链表遍历
    start = std::chrono::high_resolution_clock::now();
    for (int val : lst) {
        sum += val;
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "链表遍历耗时: " 
              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() 
              << " μs" << std::endl;
    
    std::cout << "提示: 数组因连续内存布局，缓存命中率更高" << std::endl;
}
```

### 结构体成员排序优化

合理安排结构体成员的顺序可以减少内存填充，提高缓存利用率。一般原则是：将相同或相似对齐要求的成员放在一起，将大对齐要求的成员放在前面。这样既能减少内存浪费，又能提高缓存行的利用效率。使用`#pragma pack`可以强制改变对齐方式，但通常不建议这样做，因为可能影响性能甚至导致程序崩溃。

```cpp
#include <iostream>

// 优化前：内存布局不紧凑
struct BeforeOpt {
    char a;     // 1字节 + 3字节填充
    int b;      // 4字节
    char c;     // 1字节 + 7字节填充
    double d;   // 8字节
};  // 总共24字节

// 优化后：按对齐要求排序
struct AfterOpt {
    double d;   // 8字节
    int b;      // 4字节
    char a;     // 1字节
    char c;     // 1字节 + 2字节填充
};  // 总共16字节

void structOptimization() {
    std::cout << "\n=== 结构体成员排序优化 ===" << std::endl;
    
    std::cout << "优化前 sizeof(BeforeOpt) = " << sizeof(BeforeOpt) << " 字节" << std::endl;
    std::cout << "优化后 sizeof(AfterOpt) = " << sizeof(AfterOpt) << " 字节" << std::endl;
    std::cout << "节省内存: " << sizeof(BeforeOpt) - sizeof(AfterOpt) << " 字节" << std::endl;
    
    std::cout << "\n成员排序建议:" << std::endl;
    std::cout << "  1. 将大对齐要求的成员（如double）放前面" << std::endl;
    std::cout << "  2. 将相同类型的成员放在一起" << std::endl;
    std::cout << "  3. 考虑使用位域压缩bool/enum成员" << std::endl;
}
```

### 缓存友好的循环

循环是程序中最常见的结构之一，优化循环访问模式对缓存性能至关重要。应该尽量避免在循环内部进行随机内存访问，优先使用步长为1的顺序访问。对于嵌套循环，应该将最内层循环设为访问最连续维度的循环。此外，循环展开和分块技术也能显著改善缓存利用。

同样，下面的一次性毫秒计时是待改进基线；毫秒粒度可能把快速操作显示为0，`vector<vector<int>>` 也不是单块二维数组。实际源码改用扁平 `vector`、相同的元素访问次数、预热、重复测量、中位数与防优化汇总值。

```cpp
#include <iostream>
#include <vector>
#include <chrono>

constexpr int SIZE = 1024;

void loopOptimization() {
    std::cout << "\n=== 循环优化演示 ===" << std::endl;
    
    std::vector<std::vector<int>> matrix(SIZE, std::vector<int>(SIZE, 1));
    volatile long long sum = 0;
    
    // 按行遍历（缓存友好）
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            sum += matrix[i][j];
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "按行遍历耗时: " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() 
              << " ms" << std::endl;
    
    // 按列遍历（缓存不友好）
    start = std::chrono::high_resolution_clock::now();
    for (int j = 0; j < SIZE; ++j) {
        for (int i = 0; i < SIZE; ++i) {
            sum += matrix[i][j];
        }
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "按列遍历耗时: " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() 
              << " ms" << std::endl;
    
    std::cout << "提示: C++中二维数组按行存储，按行遍历缓存命中率更高" << std::endl;
}
```

---

<a id="day26-microbenchmark-checklist"></a>

### 微基准验收清单

1. 使用 `Release` 或明确记录优化选项，Debug 数字不能代表发布性能。
2. 被比较方案必须完成等量工作；顺序与跨步版本都读取每个元素恰好一次，矩阵版本执行相同数量的乘加。
3. 计时前先预热，避开首次页映射、首次代码执行等一次性成本。
4. 重复多次并报告中位数等统计量，不把单次抖动当结论。
5. 使用可观察汇总值防止计算被优化删除，同时把数据准备和随机索引生成放在计时区间外。
6. 报告机器、编译器与选项，并把结论限定为当前环境；若要解释缓存缺失，应进一步使用 `perf` 等硬件计数器。

---

## 🎯 LeetCode 刷题

### 讲解题：LC 5. 最长回文子串

#### 题目链接

[LeetCode 5](https://leetcode.cn/problems/longest-palindromic-substring/)

#### 题目描述

给你一个字符串 `s`，找到 `s` 中最长的回文子串。

#### 形象化理解

想象你正在检查一串珠子，想找到最长的一段对称的珠子：

```
字符串: b a b a d
下标:   0 1 2 3 4

回文子串检测：
- "bab" (下标0-2): b→a→b ← 对称！长度3
- "aba" (下标1-3): a→b→a ← 对称！长度3
- "babad" 整体: 不对称

最长回文子串: "bab" 或 "aba"
```

回文就像镜子：从中心向两边看，左边和右边是一样的。

```mermaid
graph LR
    subgraph "中心扩展法示意图"
    C1["中心点"] --> L["向左扩展"]
    C1 --> R["向右扩展"]
    L --> C2["比较左右字符"]
    R --> C2
    C2 --> D{"相等?"}
    D -->|是| E["继续扩展"]
    D -->|否| F["停止，记录结果"]
    E --> L
    end
```

#### 理论介绍

**回文串**是指正读和反读都相同的字符串，如"aba"、"abba"。最长回文子串问题的核心是找到字符串中所有的回文子串，并返回最长的一个。

解决这个问题有多种方法：
1. **暴力法**：枚举所有子串，检查是否回文，时间复杂度O(n³)
2. **动态规划**：利用子问题的解，时间复杂度O(n²)，空间复杂度O(n²)
3. **中心扩展法**：以每个字符为中心向两边扩展，时间复杂度O(n²)，空间复杂度O(1)
4. **Manacher算法**：线性时间复杂度O(n)，但较复杂

对于初学者，推荐掌握**中心扩展法**，它直观易懂，空间效率高。

#### 解题思路

使用**中心扩展法**：
1. 遍历字符串的每个字符作为中心
2. 注意：回文中心可以是单个字符（奇数长度）或两个字符之间（偶数长度）
3. 从中心向两边扩展，直到左右字符不相等
4. 记录过程中发现的最长回文子串

#### 代码实现

下面是中心扩展的核心教学片段，不是独立可执行程序；为保持推导简洁，它沿用题目字符串长度可由 `int` 表示的前提。仓库真实实现使用 `std::size_t` 和半开区间向两侧扩展，避免负下标与无符号下溢。

```cpp
#include <string>
#include <algorithm>

class Solution {
public:
    std::string longestPalindrome(std::string s) {
        int n = s.length();
        if (n < 2) return s;
        
        int start = 0, maxLen = 1;
        
        // 中心扩展函数
        auto expandAroundCenter = [&](int left, int right) {
            while (left >= 0 && right < n && s[left] == s[right]) {
                left--;
                right++;
            }
            // 循环结束时，left和right是多扩展了一位的
            // 实际回文长度 = right - left - 1
            int len = right - left - 1;
            if (len > maxLen) {
                maxLen = len;
                start = left + 1;
            }
        };
        
        for (int i = 0; i < n; ++i) {
            // 以s[i]为中心的奇数长度回文
            expandAroundCenter(i, i);
            // 以s[i]和s[i+1]之间为中心的偶数长度回文
            expandAroundCenter(i, i + 1);
        }
        
        return s.substr(start, maxLen);
    }
};
```

---

### 实战题：LC 647. 回文子串

#### 题目链接

[LeetCode 647](https://leetcode.cn/problems/palindromic-substrings/)

#### 题目描述

给你一个字符串 `s`，请你统计并返回这个字符串中回文子串的数目。

#### 形象化理解

这次不是找最长的，而是数有多少个回文子串：

```
字符串: "abc"
回文子串: "a", "b", "c" 共3个

字符串: "aaa"
回文子串: "a", "a", "a", "aa", "aa", "aaa" 共6个

字符串: "aba"
回文子串: "a", "b", "a", "aba" 共4个
```

每一个单独的字符都是回文，连续相同的字符能组成更多回文。

#### 解题思路

同样使用**中心扩展法**，但这次要统计所有回文子串的数量：
1. 遍历每个可能的中心点
2. 对于每个中心，向两边扩展
3. 每次成功扩展（左右字符相等），就找到了一个回文子串，计数加一

#### 代码实现

下面同样是核心教学片段，并沿用字符串长度与回文总数都可由 `int` 表示的题目前提。仓库真实实现使用 `std::size_t` 累计，并在转换为公开 `int` 返回类型前检查范围。

```cpp
#include <string>

class Solution {
public:
    int countSubstrings(std::string s) {
        int n = s.length();
        int count = 0;
        
        // 中心扩展函数，返回以[left, right]为中心的回文子串数量
        auto extendPalindrome = [&](int left, int right) {
            while (left >= 0 && right < n && s[left] == s[right]) {
                count++;    // 找到一个回文子串
                left--;
                right++;
            }
        };
        
        for (int i = 0; i < n; ++i) {
            // 奇数长度回文：以s[i]为中心
            extendPalindrome(i, i);
            // 偶数长度回文：以s[i]和s[i+1]之间为中心
            extendPalindrome(i, i + 1);
        }
        
        return count;
    }
};
```

#### 两题对比

| 方面 | LC 5 最长回文子串 | LC 647 回文子串数 |
|------|------------------|------------------|
| 目标 | 找最长的 | 统计总数 |
| 返回值 | 字符串 | 整数 |
| 核心方法 | 中心扩展，记录最长 | 中心扩展，累加计数 |
| 时间复杂度 | O(n²) | O(n²) |
| 空间复杂度 | O(1) | O(1) |

---

## 🛠️ 今日工程动作：为缓存项目拆出性能 target

在缓存项目卡的“文件和 target”一栏中新增独立的 `cache_benchmark`，把正确性测试与性能实验分开：普通测试可以在多种配置下运行，性能 target 固定使用 Release 并打印编译器、数据规模、重复次数和中位数。为顺序、跨步、命中、淘汰四类场景写清等量工作不变量，并把缓存填充字节数作为可配置实验参数，而不是藏在实现中的硬编码常量。

## 🧭 恰好五句复盘

1. 缓存容量、延迟和缓存行大小属于具体硬件事实，课程里的64字节只能作为实验参数。
2. 对象大小等于64字节不保证恰占一个缓存行，地址差小于64字节也不保证两个地址在同一行。
3. 对齐访问首先要满足 `alignof` 与对象生命周期，不能把字节缓冲区随意转成 `int*` 解引用。
4. 公平微基准需要 Release、等量工作、预热、重复统计和防优化，单次耗时只能算观察值。
5. 缓存项目应把正确性 target 与性能 target 分开，让每个数字都能追溯到机器、编译选项和输入规模。

## 🚀 运行代码

```bash
./build_and_run.sh

# 同一入口启用 ASan/UBSan
ENABLE_SANITIZERS=ON ./build_and_run.sh
```

默认 CMake 已为 GCC/Clang 系列编译器启用 `-Wall -Wextra -Wpedantic`，因此一键脚本不依赖外部 `CMAKE_CXX_FLAGS` 才看到基础警告；严格终审仍额外使用 `-Werror -pedantic-errors`。

---

## 📚 相关术语

| 术语 | 英文 | 定义 |
|------|------|------|
| CPU缓存 | CPU Cache | CPU和主内存之间的高速存储器 |
| 缓存行 | Cache Line | 缓存传输与一致性的重要单位，大小依具体处理器而变 |
| 缓存命中 | Cache Hit | 请求数据在所考察的某级缓存中找到 |
| 缓存缺失 | Cache Miss | 请求数据不在所考察的某级缓存中；仍可能在更低级缓存命中 |
| 伪共享 | False Sharing | 多线程修改同一缓存行导致的性能问题 |
| 内存对齐 | Memory Alignment | 数据地址满足特定边界要求 |
| 对齐边界 | Alignment Boundary | 数据地址必须满足的整数倍要求 |
| 填充字节 | Padding | 编译器插入的用于满足对齐的字节 |
| 局部性原理 | Locality Principle | 程序访问数据的时空集中性 |
| SIMD | Single Instruction Multiple Data | 单指令多数据流并行计算 |

---

## 💡 学习提示

### 要点记忆

1. **缓存层次**：某级 miss 会继续查询更低层；容量、共享方式和延迟应以具体处理器为准
2. **缓存行大小**：64字节是常见实验值，不是C++或所有硬件的保证
3. **伪共享**：同一缓存行的不同变量被多线程修改导致性能下降
4. **内存对齐**：对象地址满足 `alignof(T)`，且对象生命周期必须已经开始
5. **alignas/alignof**：C++11关键字，用于控制和查询对齐要求

### 实践建议

1. **性能测试**：Release下预热、等量工作、重复测量并报告统计量
2. **结构体设计**：按对齐要求排序成员，减少填充
3. **数据结构选择**：性能敏感场景优先使用连续内存结构
4. **循环优化**：优先顺序访问，避免随机访问模式
5. **工具使用**：使用性能分析工具（如perf、VTune）定位缓存问题

### 常见误区

1. ❌ 认为"内存对齐只是编译器的事" → 理解对齐才能写出高性能代码
2. ❌ 认为链表比数组"更灵活所以更好" → 忽视了缓存效率差异
3. ❌ 认为"现代CPU很快，不需要考虑缓存" → 先用剖析确认内存访问是否为当前热点
4. ❌ 过度使用`#pragma pack(1)` → 可能导致严重的性能问题或崩溃

---

## 🔗 参考资料

1. [What Every Programmer Should Know About Memory](https://people.freebsd.org/~lstewart/articles/cpumemory.pdf) - Ulrich Drepper的经典文章
2. [CPU Caches and Why You Care](https://www.aristeia.com/TalkNotes/codedive-CPUCachesHandouts.pdf) - Scott Meyers
3. [cppreference - alignas specifier](https://en.cppreference.com/w/cpp/language/alignas)
4. [cppreference - alignof operator](https://en.cppreference.com/w/cpp/language/alignof)
5. [False Sharing - Intel](https://software.intel.com/content/www/us/en/develop/articles/avoiding-and-identifying-false-sharing-among-threads.html)
