# 第三周：栈队列、Lambda 与 BFS（Day 15-21）

本阶段建立在函数、标准容器和 RAII 之上：先用栈和队列表达“访问顺序”，再把 Lambda 当作可传递的行为，最后进入堆、单调栈和 BFS。完成本阶段后，才进入值类别、移动语义和完美转发。

**先修要求**：能使用 `vector`、`string`、引用和智能指针；理解对象生命周期；能独立写多函数程序。如果所有权概念仍不清楚，先回到 [第二周](../week_02/README.md)。

## 从 Week 2 到 Week 3，再到 Week 4

第二周主要回答“谁拥有对象、对象何时销毁”；第三周把问题推进为“**谁保存一段行为、这段行为稍后执行时依赖的对象是否还活着**”。Lambda 不是特殊语法糖而已，每个 Lambda 表达式都会产生一个闭包对象，捕获值就是这个对象携带的状态。因此，第二周的所有权图在本周仍然有效，只是图中多了“回调保存谁、借用谁”的边。

| 阶段 | 核心对象 | 本阶段要回答的问题 |
|---|---|---|
| Week 2 | 资源对象与智能指针 | 谁拥有资源，观察者能活多久？ |
| Week 3 | 容器适配器、闭包与可调用对象 | 谁保存回调，捕获在调用时仍有效吗？ |
| Week 4 | 右值、转发引用与完美转发 | 回调和参数怎样移动、怎样保持原值类别？ |

Day 16 会先学会初始化捕获和 `std::forward<decltype(x)>(x)` 的正确形状，但不要求此时完整推导移动语义和引用折叠。完成 Week 4 的 [移动语义](../week_04/day_23/README.md)、[通用引用](../week_04/day_24/README.md) 与 [完美转发](../week_04/day_25/README.md) 后，应回看本周 Item 32–33，解释代码为什么成立，而不只是记住写法。

## 📋 本周概览

| Day | 主题 | 数据结构 | C++11特性 | EMC++条款 | LeetCode |
|-----|------|---------|-----------|----------|----------|
| [15](day_15/README.md) | 栈入门 | 栈数据结构 | Lambda入门 | [Item 31](../tutorials/Effective_Modern_CPP教程.md#emcpp-item-31) | 20, 1047 |
| [16](day_16/README.md) | 队列入门 | 队列数据结构 | Lambda进阶初识 | [Item 32](../tutorials/Effective_Modern_CPP教程.md#emcpp-item-32)–[33](../tutorials/Effective_Modern_CPP教程.md#emcpp-item-33) | 232, 225 |
| [17](day_17/README.md) | 单调栈 | 单调栈算法 | function/bind | [Item 34](../tutorials/Effective_Modern_CPP教程.md#emcpp-item-34) | 739, 496 |
| [18](day_18/README.md) | 函数调用栈 | 函数调用栈 | enum class | [Item 10](../tutorials/Effective_Modern_CPP教程.md#emcpp-item-10) | 84, 42 |
| [19](day_19/README.md) | 优先队列 | 堆/优先队列 | 比较器 Lambda | - | 215, 347 |
| [20](day_20/README.md) | BFS基础 | BFS算法 | 回调式遍历边界 | - | 102, 107 |
| [21](day_21/README.md) | 周复习 | 栈队列综合 | Lambda综合 | [Item 31-34 复习](../tutorials/Effective_Modern_CPP教程.md#emcpp-item-31) | 155, 150 |

条款编号以上表和 [Effective Modern C++ 教程](../tutorials/Effective_Modern_CPP教程.md) 为准。Item 31 讨论默认捕获，Item 32 讨论初始化捕获，Item 33 讨论泛型 Lambda 中的转发，Item 34 比较 Lambda 与 `std::bind`；Item 10 则是限定作用域枚举，与 Lambda 章节不是连续编号。

### 本周单一信息源

本周各文档不再重复承担同一件事：日 README 负责当天新增的语义和完成闭环，专题指南负责手算轨迹，单题 README 负责输入契约、不变量、正确性与复杂度。遇到重复内容时，以下表中的主讲位置为准：

| 主题 | 主讲文档 | 其他文档只保留什么 |
|------|----------|------------------------|
| 容器适配器与底层容器约束 | [Day 15](day_15/README.md#day15-stack-adaptor)、[Day 16](day_16/README.md#day16-queue-adaptor)、[Day 19](day_19/README.md#day19-priority-queue-contract) | 具体算法为什么选这种访问顺序 |
| Lambda 捕获、`this` 与生命周期 | [Day 15](day_15/README.md#day15-lambda-lifetime) | Day 16 只讲初始化捕获和泛型 Lambda，Day 17 只比较可调用对象边界 |
| 单调栈通用不变量 | [Day 17](day_17/README.md#day17-monotonic-stack-template) | 专题指南只展示具体输入的入栈、弹栈轨迹 |
| BFS 的 `visited` 时机与层边界 | [Day 20](day_20/README.md#day20-bfs-enqueue-mark) | LC 102/107 单题文档只说本题输出差异 |

这种分工保留每天的学习量，但避免同一段定义在周总览、每日文档、形象化指南和题解中各自演化。

## 📁 目录结构

```
week_03/
├── day_15/           # 栈入门 + Lambda入门
│   ├── README.md
│   ├── CMakeLists.txt
│   ├── build_and_run.sh
│   └── code/
│       ├── main.cpp
│       ├── data_structure/
│       ├── cpp11_features/
│       ├── emcpp/
│       └── leetcode/
│
├── day_16/           # 队列入门 + Lambda进阶
├── day_17/           # 单调栈 + function/bind
├── day_18/           # 函数调用栈 + enum class
├── day_19/           # 优先队列/堆
├── day_20/           # BFS基础
└── day_21/           # 周复习
```

## 🚀 快速开始

```bash
# 从仓库根目录进入某天
cd week_03/day_15
./build_and_run.sh

# 或手动编译
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

脚本的第一个可选参数是构建目录，例如 `./build_and_run.sh /tmp/week3-day15`；后续参数会原样传给 CMake，例如 `./build_and_run.sh /tmp/week3-day18-asan -DENABLE_SANITIZERS=ON`（支持该选项的 Day 18–21）。

## 🔗 构建图中的模块边界

本周开始把“代码分文件”推进为“消费者通过公开接口使用模块”。每个可复用实现源只属于一个静态库 target；总入口和算法测试只编译自己的入口源，再用 `target_link_libraries` 消费库，避免同一实现被主程序或测试重复编译。库通过 `PUBLIC` usage requirement 公开消费者所需的头文件搜索路径，而不是用目录级全局 include 路径替所有 target 隐藏依赖。

每一天的 `PublicApiConsumer` 都模拟一个只知道公开头和链接库的外部消费者：若头文件不能自包含、include 路径没有随 target 传播、声明与实现不一致，或库没有提供所声明的符号，它就会在编译或链接阶段失败。完成某天构建后可单独运行这层检查：

```bash
ctest --test-dir build --output-on-failure -R PublicApiConsumer
```

阅读 CMake 时要沿依赖方向提问：入口源直接包含哪些公开头、链接哪个库、库向消费者传播哪些使用要求，以及测试是在调用公开接口还是把实现源重新编译了一遍。

## 📚 核心知识点

### 栈 Stack
- **特点**：LIFO（后进先出）
- **操作**：push, pop, top, empty, size
- **应用**：括号匹配、表达式求值、函数调用、单调栈

### 队列 Queue
- **特点**：FIFO（先进先出）
- **操作**：push, pop, front, back, empty, size
- **应用**：BFS、任务调度、消息队列

`std::stack`、`std::queue` 和 `std::priority_queue` 都是**容器适配器**：它们不自己定义一套存储结构，而是把一个底层容器限制成更小的接口。“可以指定底层容器”不等于“任意 STL 容器都能替换”：栈需要 `back/push_back/pop_back`，队列需要 `front/back/push_back/pop_front`，优先队列还需要随机访问迭代器以运行堆算法。具体要求和复杂度分别放在 Day 15、16 和 19 主讲。

### 单调栈
- **栈内从底到顶递减**：常用于在弹栈时确定“下一个更大元素”
- **栈内从底到顶递增**：常用于在弹栈时确定“下一个更小元素”
- **时间复杂度**：O(n)

不同资料可能按“栈底到栈顶的顺序”或“弹栈条件”命名单调栈。不要只背名称，要写清栈内存的是值还是下标，以及什么条件触发弹栈。

### Lambda表达式

Lambda 表达式产生的是一个匿名闭包类型；保存 Lambda，就是保存一个带状态的对象。分析捕获时要同时写出“捕获的是副本、引用还是 `this` 指针”以及“闭包会被调用到什么时候”。

```cpp
// 基本语法
[capture](params) -> return_type { body }

// 捕获方式
[x]      // 值捕获
[&x]     // 引用捕获
[=, &x]  // 混合捕获

// C++14泛型Lambda
auto add = [](auto a, auto b) { return a + b; };
```

### 可调用对象的四种常见边界

| 边界 | 是否能携带状态 | 主要优点 | 主要代价/限制 | 本周使用建议 |
|---|---|---|---|---|
| 函数指针 | 否 | 简单、ABI 清楚 | 不能保存捕获状态 | 无状态策略或 C 接口 |
| 具体 Lambda/函数对象模板参数 | 是 | 保留具体类型，通常易内联 | 模板实现通常放头文件，接口类型不固定 | 算法内部默认选择 |
| `std::function<R(Args...)>` | 是 | 类型擦除，便于稳定存储和替换 | C++17 要求目标可复制，可能分配并有间接调用 | 明确需要运行期替换时使用 |
| `std::bind` 返回对象 | 是 | 可适配旧式 C++11 接口 | 参数映射、求值时机和引用语义不直观 | 优先改写为 Lambda |

不要把 `std::function`、Lambda 和 `std::bind` 当成同一层概念：Lambda/`bind` 产生具体可调用对象，`std::function` 是可选择用来存储这些对象的类型擦除容器。

### BFS广度优先搜索
- **数据结构**：队列
- **应用**：最短路径、层级遍历
- **模板**：queue + visited

## 本周工程能力线

### 1. 容器接口与业务流程分离

栈和队列只负责保存顺序，不应同时承担题目规则、打印菜单和测试数据。以括号匹配为例：

- 容器层只提供 `push`、`pop`、`top/front`、`empty` 等状态操作。
- 业务层决定遇到左括号为何入栈、右括号如何匹配、失败时返回什么。
- 展示层决定输出中文提示还是机器可检查的结果。
- 测试层直接调用业务函数，不通过交互菜单间接验证。

如果更换 `std::stack` 的底层容器就必须修改题目判断逻辑，或测试只能解析控制台文本，说明边界仍然混在一起。

### 2. 回调捕获与生命周期卡

每当回调可能被保存到当前语句之外，都填写一张小卡：

| 字段 | 必须回答的问题 |
|---|---|
| 保存者 | 谁保存闭包，保存到什么时候？ |
| 捕获清单 | 每个名字是值捕获、引用捕获、`this` 还是初始化捕获？ |
| 所有权 | 闭包拥有副本、共享对象，还是只借用外部对象？ |
| 失效条件 | 哪个作用域结束、对象析构或容器操作会让捕获失效？ |
| 可复制性 | 闭包能否复制，是否要进入 `std::function`？ |
| 调用约束 | 可调用一次还是多次，调用时允许修改捕获状态吗？ |

“现在立刻调用”与“注册后稍后调用”是两种完全不同的生命周期。前者的引用捕获通常容易证明安全，后者必须让有效期成为接口契约。

### 3. 用测试替代内部依赖

不要让 BFS、任务调度或过滤器直接读取全局图、系统时钟和固定输出流。把变化点作为回调传入：

```cpp
using Neighbors = std::function<std::vector<int>(int)>;
using Goal = std::function<bool(int)>;
using Visit = std::function<void(int)>;

bool bfsSearch(int start,
               const Neighbors& neighbors,
               const Goal& is_goal,
               const Visit& on_visit);
```

生产代码传入真实邻接表和日志动作；测试传入很小的固定图、可控目标谓词和记录访问顺序的 Lambda。这样测试验证的是 BFS 流程，而不是依赖真实文件、随机数或终端输出。这里选择 `std::function` 是为了展示稳定的运行期边界；Week 4 回看时，再比较模板可调用参数如何避免类型擦除并接受移动型闭包。

### 4. Week 3 项目卡：可测试的搜索/任务处理器

本周项目不要求另造大型框架，而是把队列、Lambda、BFS 和接口分层组合成一个小型“搜索/任务处理器”。

| 项目项 | 最低要求 |
|---|---|
| 核心流程 | 使用队列维护待处理状态，入队时标记 visited，避免重复处理 |
| 业务接口 | 邻居生成、目标判断、访问通知由回调注入，不写死在容器类里 |
| 状态类型 | 用 Day 18 的 `enum class` 表达明确状态/结果，避免裸整数混用 |
| 捕获契约 | 为三个回调分别写生命周期卡；禁止默认捕获后直接长期保存 |
| 可调用边界 | 说明为何当前选择模板、函数指针或 `std::function`，记录其复制与分配限制 |
| 测试替身 | 用 Lambda 构造固定图，记录访问顺序；覆盖起点即目标、不可达、存在环和重复边 |
| 错误路径 | 空回调、非法起点或回调抛异常时，写清返回值和已产生副作用 |
| 非目标 | 本周不要求并发队列、无锁结构，也不要求完整实现完美转发 |

验收时至少交付：一张容器/业务/展示分层图、三张回调生命周期卡、四类边界测试，以及一次“把真实依赖替换为测试 Lambda”的演示。

## 📝 LeetCode题目总结

| 题号 | 题目 | 难度 | 核心算法 |
|------|------|------|---------|
| 20 | 有效的括号 | 简单 | 栈 |
| 1047 | 删除相邻重复项 | 简单 | 栈 |
| 232 | 用栈实现队列 | 简单 | 双栈 |
| 225 | 用队列实现栈 | 简单 | 单队列 |
| 739 | 每日温度 | 中等 | 单调栈 |
| 496 | 下一个更大元素 | 简单 | 单调栈 |
| 84 | 柱状图最大矩形 | 困难 | 单调栈 |
| 42 | 接雨水 | 困难 | 单调栈/双指针 |
| 215 | 第K个最大元素 | 中等 | 堆/快速选择 |
| 347 | 前K个高频元素 | 中等 | 堆 |
| 102 | 层序遍历 | 中等 | BFS |
| 107 | 自底向上层序遍历 | 中等 | BFS |
| 155 | 最小栈 | 中等 | 辅助栈 |
| 150 | 逆波兰表达式 | 中等 | 栈 |

## 💡 学习建议

1. **先理解数据结构**：栈和队列的特点和应用场景
2. **掌握单调栈模板**：找下一个更大/更小元素的标准解法
3. **Lambda最佳实践**：显式捕获，避免默认捕获
4. **BFS是重点**：队列实现，层级遍历模板
5. **Day 16 先会用、Week 4 再推导**：初始化捕获和转发写法先建立正确习惯，学完移动语义与引用折叠后必须回看原因

## ✅ 本周验收与下一步

进入第四周前，应能：

- 从空文件写出栈、队列、优先队列和 BFS 的基本模板。
- 解释闭包对象、值捕获、引用捕获、`this` 捕获和初始化捕获的生命周期差异。
- 区分具体 Lambda、函数指针、`std::function` 和 `std::bind` 所处的接口层次。
- 把容器操作、业务流程、展示和测试拆开，并用 Lambda 替代一个内部依赖。
- 完成 Week 3 项目卡，覆盖环、重复状态、不可达目标和回调异常等边界。
- 说明 Item 33 的正确写法是 `std::forward<decltype(x)>(x)`，同时诚实标注其机制要等 Week 4 引用折叠后完整解释。
- 说明队列模拟、单调栈和堆题的识别依据。

下一步：[第四周：哈希表、移动语义与字符串](../week_04/README.md)。学完 Day 23–25 后，返回本周 [Item 32–33](../tutorials/Effective_Modern_CPP教程.md#emcpp-item-32)，重新解释移动捕获为何让闭包可能不可复制、`decltype(x)` 如何携带引用类别，以及何时模板可调用边界优于 `std::function`。

## 🔗 参考资料

- [Hello-Algo - 栈和队列](https://www.hello-algo.com/chapter_stack_and_queue/)
- [cppreference - stack](https://en.cppreference.com/w/cpp/container/stack)
- [cppreference - queue](https://en.cppreference.com/w/cpp/container/queue)
- [cppreference - priority_queue](https://en.cppreference.com/w/cpp/container/priority_queue)
- [cppreference - Lambda expressions](https://en.cppreference.com/w/cpp/language/lambda)
- [C++ Core Guidelines F.53/F.54 - Lambda 捕获与生命周期](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#f53-avoid-capturing-by-reference-in-lambdas-that-will-be-used-non-locally-including-returned-stored-on-the-heap-or-passed-to-another-thread)
- [Effective Modern C++ - Item 31-34](https://www.aristeia.com/EMC++.html)
- 《C++ Primer》第 5 版：泛型算法、Lambda 表达式与标准库容器
- 《A Tour of C++》第 2 版：容器、算法与函数对象概览

## 💼 本周面试高频问题汇总

把本周的 Lambda、可调用对象、枚举与容器适配器串成问答。

### Lambda 与闭包

```
Q1: Lambda 的本质是什么？
A: Lambda 是编译器生成的闭包类的匿名对象。捕获列表对应类的成员，
   调用 operator() 对应函数体。每个 Lambda 类型唯一，不同 Lambda 互不相同类型。
   可用 auto 存 Lambda；要统一类型用 std::function（有类型擦除开销）。

Q2: 值捕获和引用捕获的区别？各自有什么风险？
A: 值捕获在 Lambda 创建时拷贝一份，之后外部变量改变不影响闭包；
   引用捕获只存引用，外部变量改变后闭包看到的也变。
   引用捕获风险：闭包比外部变量活得久时悬空（尤其异步、返回 Lambda 时）。
   值捕获安全但有拷贝开销；引用捕获轻量但需保证生命周期。

Q3: 捕获 this 有什么悬空风险？
A: 捕获 this（显式或隐式 [=] 在成员函数里）只存指针，不延长对象生命周期。
   若对象已销毁，闭包再访问成员就是未定义行为。
   C++17 的 [*this] 会拷贝整个对象，更安全但要求对象可拷贝。
   异步回调里捕获 this 是经典 bug 来源，应优先用安全引用或值捕获关键状态。
```

### 可调用对象与枚举

```
Q4: function、函数指针、Lambda、bind 怎么选？
A: 需要统一存储任意可调用对象、且类型擦除可接受用 std::function；
   只调用一个固定 C 函数用函数指针（零开销）；
   需要就地捕获/定制逻辑用 Lambda（最推荐）；
   bind 已基本被 Lambda 取代，新代码不用 bind。

Q5: enum class 相比传统 enum 解决了什么？
A: 传统 enum 名字泄漏到外层作用域，且隐式转 int，易和整数混淆。
   enum class 作用域受限（要用 Color::Red），不隐式转 int，
   也不能和别的枚举混用，类型更安全。
   代价：要显式 static_cast<int> 才能拿到底层值。
```

### 容器适配器

```
Q6: stack/queue/priority_queue 底层是什么？
A: 它们是容器适配器：stack、queue 默认用 std::deque 作底层；priority_queue 默认用 std::vector。
   都可换其它顺序容器作底层。适配器只暴露 push/pop/top 或 push/pop/front 的子集，隐藏底层容器其他接口。
   priority_queue 底层是堆（在 vector 上用 make_heap 逻辑维护）。

Q7: priority_queue 为什么默认用 vector 而不是 set？
A: 堆只需连续存储 + 父子下标运算（i 的父是 (i-1)/2），vector 完美匹配且缓存友好。
   只需要取最大/最小 + 插入，不要求有序遍历，set（红黑树）开销更大且无必要。
   priority_queue 用堆维护，取顶 O(1)，插入删除 O(log n)。
```

---

