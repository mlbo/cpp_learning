# 第五周：树、并发与综合复习（Day 29-35）

## 📅 本周概览

第五周是 35 天主线的阶段收官：算法线从二叉树递归定义推进到遍历、BST、DFS、回溯、LCA、重建、序列化和路径动态规划；工程线从 `std::thread` 生命周期推进到任务、锁、条件变量、future、atomic 和线程池停止协议。代码统一按 C++17 编译，C++11 并发组件是本周核心，但实现同时使用 `unique_ptr`、`optional`、`scoped_lock` 等更现代的表达方式。

并发不是独立语法点。开始前必须理解函数对象、RAII、智能指针、移动语义和对象生命周期；否则线程参数传递、锁管理和异步结果会变成死记 API。先修不稳时，回看 [第二周](../week_02/README.md)、[第三周](../week_03/README.md) 和 [第四周](../week_04/README.md)。

本周将完成以下目标：
- 掌握二叉树的基本概念和四种遍历方式
- 理解二叉搜索树（BST）的性质和操作
- 掌握DFS算法及其在路径问题中的应用
- 深入理解进程线程模型和线程安全
- 熟练运用C++11并发组件（thread/mutex/atomic/condition_variable）
- 串联本课程已讲的 EMC++ 条款，并补上 Item 41-42 的正确定位
- 学会定义接口契约、所有权、不变量、异常通道和停止协议
- 把“打印结果”升级为失败返回非零的自动化测试，并理解 Sanitizer 的能力边界

### 文档分工与阅读顺序

为避免同一个知识点在多份文档中反复出现，本周按下面顺序阅读：

1. [C++ 并发编程教程](../tutorials/CPP并发编程教程.md) 是线程、同步、内存模型和关闭协议的主讲文档；每日 README 只解释当天新增接口和工程差异。
2. [Effective Modern C++ 教程](../tutorials/Effective_Modern_CPP教程.md) 是 Item 11–16、35–40 的主讲文档；每日 README 负责把条款连接到当天代码，而不是重新复述整条内容。
3. [树与并发专题形象化题解指南](树与并发专题形象化题解指南.md) 只负责手算过程、递归栈、队列和状态迁移图；正式契约、复杂度和边界以对应 Day README 为准。
4. 每日 README 最后落到一个工程动作和恰好五句复盘；源码链接用于观察实现，CTest 结果用于验证协议。

### 本周统一证明框架

树题先回答“递归函数对一棵子树承诺什么”，再写空节点、叶子、递归前进方向和返回时不变量。若有 `n` 个可达节点、树高为 `h`、最大宽度为 `w`，常规遍历时间为 O(n)，递归或 DFS 显式栈最坏占 O(h)，层序队列最坏占 O(w)；输出所有路径时还必须加上输出总长度。课程用 `unique_ptr` 表达父节点独占孩子，算法参数使用指针或引用只作观察；退化树可能令 `h = n`，深度不受信任时应改用显式栈或设置输入上限。

并发题先列共享状态和所有者，再指出冲突访问由哪条同步边建立 happens-before，随后写等待谓词与关闭状态机。线程池和可关闭队列统一采用“锁内停止接受并确定提交归属 → 拒绝新工作 → 排空已接受工作 → 唤醒等待者 → worker 退出 → 外部 owner join”的协议；测试只检查 future、谓词、join 和最终不变量，不依赖 `sleep`、日志顺序或固定耗时。

---

## 📋 每日安排

| Day | 主题 | 数据结构 | C++11特性 | EMC++条款 | LeetCode |
|-----|------|---------|-----------|----------|----------|
| [29](day_29/README.md) | 二叉树入门 | 递归树与遍历语义 | thread、task、future | 35 | 144, 145 |
| [30](day_30/README.md) | 树遍历 | 递归/迭代/BFS | mutex、RAII、多锁 | 36-37 | 94, 102 |
| [31](day_31/README.md) | BST | 全局上下界与删除 | 条件变量、可关闭队列 | 38 | 98, 700 |
| [32](day_32/README.md) | DFS | 树/图 DFS 与 visited | atomic、内存序 | 39-40 | 104, 111 |
| [33](day_33/README.md) | 树路径 | 回溯不变量 | 线程池、future、停止协议 | 11-16 | 257, 113 |
| [34](day_34/README.md) | 并发综合 | LCA 与遍历重建 | 同步机制选择与关闭通道 | 35-40复习 | 236, 105 |
| [35](day_35/README.md) | 阶段总结 | 序列化与最大路径和 | 项目契约与验证证据 | 全部复习 | 297, 124 |

---

## 📁 目录结构

```
week_05/
├── README.md                    # 本文件
├── day_29/                      # Day 29: 二叉树入门
│   ├── README.md
│   ├── CMakeLists.txt
│   ├── build_and_run.sh
│   └── code/
│       ├── main.cpp
│       ├── data_structure/
│       ├── cpp11_features/
│       ├── emcpp/
│       └── leetcode/
├── day_30/                      # Day 30: 树遍历
├── day_31/                      # Day 31: BST
├── day_32/                      # Day 32: DFS
├── day_33/                      # Day 33: 树路径
├── day_34/                      # Day 34: 并发综合
└── day_35/                      # Day 35: 总结
```

---

## 🎯 学习目标

### 数据结构
- 理解二叉树的定义、性质和存储方式
- 掌握四种遍历方式：前序、中序、后序、层序
- 理解二叉搜索树的查找、插入、删除操作
- 掌握DFS算法在树问题中的应用

### C++11特性
- 熟练使用std::thread创建和管理线程
- 掌握mutex的使用和死锁预防
- 理解condition_variable的等待/通知机制
- 掌握 atomic 的原子性与内存序基础，并知道原子类型不保证底层一定无锁
- 理解 data race 是未定义行为，通知不是状态，睡眠不是同步协议
- 能说明 thread、async、promise/future、线程池各自的适用边界
- 能精确定义 data race：两个可能并发的冲突访问作用于同一内存位置、至少一个访问不是原子操作，且两者之间没有 happens-before；其结果是未定义行为

### 工程能力

- 用 `unique_ptr` 表达树节点的独占所有权，用裸指针或引用表达非拥有观察
- 为 BST 重复值、LCA 缺失节点、反序列化非法输入等情况写出显式契约
- 为队列和线程池定义“拒绝新任务、排空已接收任务、唤醒等待者、join worker”的停止顺序
- 让每个 `.cpp` 都由 CMake 目标覆盖，让每个 CTest 失败时返回非零
- 只断言稳定语义，不断言线程调度顺序，也不用固定睡眠等待正确性

### EMC++条款
- Item 11-16: deleted函数、override、const_iterator等
- Item 35-40: 并发API的正确使用

### LeetCode刷题
- 完成二叉树遍历、深度、路径等经典题目
- 综合运用所学知识解决问题

---

## 🚀 快速开始

```bash
# 编译运行某一天的代码
cd week_05/day_29
./build_and_run.sh
```

七个脚本已随仓库保存为可执行文件，不需要先运行 `chmod`。每个脚本都会从本日全新 `build/` 目录按 C++17 Release 配置，启用 `-Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow -Werror`，构建全部目标并运行 CTest。一次验证七天可在仓库根目录执行：

```bash
for day in 29 30 31 32 33 34 35; do
  (cd "week_05/day_${day}" && ./build_and_run.sh)
done
```

学习代码时先读 [树与并发专题形象化题解指南](树与并发专题形象化题解指南.md) 建立直觉，再回到每日 README 写出递归/并发不变量，最后运行测试并故意制造一次失败，确认测试确实能阻止错误结果。

### 验收层次

1. **编译器**：严格告警发现窄化、符号转换、遮蔽和可疑接口。
2. **CTest**：覆盖正常值、空输入、极值、异常、重复关闭与停止后拒绝。
3. **ASan + UBSan**：用于树所有权、反序列化和递归代码，检查已执行路径上的越界、释放后使用、泄漏和部分未定义行为。
4. **TSan**：只用于真正并发且行为稳定的测试，检查本次平台和执行路径上的数据竞争；它不证明所有调度都正确，某些容器环境也可能因地址映射限制无法运行。

---

## 📚 参考资料

1. [C++ working draft：并发支持库](https://eel.is/c++draft/thread)
2. [cppreference：Thread support library](https://en.cppreference.com/w/cpp/thread.html)
3. [ISO C++ Core Guidelines：Concurrency](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-concurrency)
4. Anthony Williams, *C++ Concurrency in Action*（线程生命周期、条件变量、future、内存模型与线程池）
5. Scott Meyers, *Effective Modern C++*（Item 11–16、35–40）
6. Stanley B. Lippman 等，*C++ Primer*（容器、RAII、对象生命周期与标准库基础）
7. [Hello-Algo：二叉树](https://www.hello-algo.com/chapter_tree/)
8. [仓库 Effective Modern C++ 教程](../tutorials/Effective_Modern_CPP教程.md)
9. [仓库 C++ 并发编程教程](../tutorials/CPP并发编程教程.md)

---

## ✅ 课程结束后的正确定位

完成 Day 35 代表你建立了继续学习现代 C++ 的骨架，而不是已经“精通 C++”。接下来应通过项目、测试、调试和代码评审巩固知识，并补学异常安全、模板泛型、C++20、工程构建与性能分析。

回到 [课程总入口](../README.md) 查看最终验收标准和后续专题。

## 💼 本周面试高频问题汇总

把本周的并发、原子操作、同步机制与线程池串成问答。

### 线程与同步原语

```
Q1: std::thread 为什么要 join 或 detach？不处理会怎样？
A: 线程对象析构时若仍 joinable（未 join/detach），程序调用 std::terminate 直接崩溃。
   join 等待线程结束；detach 分离让线程后台运行。新代码优先 join；
   detach 后线程对象失效，难管理生命周期，慎用。RAII 包装线程句柄更安全。

Q2: mutex 和 atomic 怎么选？
A: 保护多步复合操作用 mutex（如多字段一起改）；保护单个标量的读改写用 atomic。
   atomic 保证操作原子性，但**不保证无锁**——某些类型/平台用内部锁实现，需用
   is_lock_free() / is_always_lock_free 查询是否真正无锁。
   atomic 适合计数器、标志位；mutex 适合临界区、数据结构。

Q3: 条件变量为什么要在循环里 wait？什么是虚假唤醒？
A: 标准允许 wait 在没有 notify 时也返回（虚假唤醒），所以必须用 while 循环
   重新检查条件，而不是 if 一次性判断。C++11 起可用 wait 的谓词版本
   （cv.wait(lock, [&]{ return cond; }））自动循环，但底层仍是循环。

Q4: 怎么预防死锁？
A: 常见四条件：互斥、占有等待、不可剥夺、循环等待。破坏任一即可：
   一次申请所有锁、用 std::lock/lock_guard 配 std::adopt_lock、固定加锁顺序、
   或用 std::scoped_lock（C++17）一次性原子地锁多把。避免持锁时再回调未知代码。
```

### 内存序与数据竞争

```
Q5: 什么是 data race？为什么是未定义行为？
A: 至少一个线程写、多个线程同时访问同一内存、且无同步关系，就是 data race。
   标准规定 data race 是未定义行为——编译器可假设不会发生并据此优化，
   结果不可预测。修复：用 mutex 或 atomic + 合适内存序建立 happens-before 关系。

Q6: happens-before 是什么意思？
A: happens-before 是标准定义的偏序关系：若 A happens-before B，
   则 A 的内存写对 B 可见。同步原语（释放锁、join、原子操作）建立 happens-before 边。
   多线程正确性的本质就是用这些原语"在并发访问间织出 happens-before 关系"，
   而非靠"直觉时间先后"。

Q7: memory_order 怎么选？seq_cst 为什么贵？
A: 默认 memory_order_seq_cst 是全局顺序一致，最直观但最贵（要全局栅栏）。
   需要高性能且能推理内存序时用 acquire-release（acquire 读、release 写），
   计数器可用 relaxed（无同步语义，仅原子）。从 seq_cst 起步，
   确认瓶颈后局部放宽到 acquire-release，再按需降到 relaxed。
```

### 线程池与停止协议

```
Q8: 线程池的停止协议怎么设计？
A: 典型五步：锁内停止标志置位 → 拒绝新任务 → 唤醒所有 worker →
   worker 醒来发现停止则排空队列后退出 → 主线程 join 所有 worker。
   关键是用 condition_variable::notify_all 唤醒等待中的 worker，
   且停止检查要在锁内与任务取出同一段临界区，避免漏唤醒。

Q9: future/promise 解决了什么？
A: 让线程把结果异步传回调用者：promise 写、future 读，二者配对。
   避免手动管理共享结果变量和同步原语；future::get() 阻塞等结果，
   异常也会通过 future 传播。线程池返回 std::future 让任务结果可被调用方获取。
```

---

