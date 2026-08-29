# Day 35：阶段总结——建立可继续进阶的 C++ 知识体系

> **学习定位**：本日用于验收、查漏和规划项目实践，不再把“看完章节”当成掌握。课程内容覆盖 EMC++ 42 个条款的索引与关键主题，但真正内化仍需要在后续代码评审和项目中反复使用。

> **共性入口**：35 天课表以 [科学学习规划](../../CPP_35天科学学习规划_最终版.md) 为单一来源，语言机制与 EMC++ 条款分别回到对应专题教程；本日只保留阶段检索框架、LC 297/124 的完整契约、核心/进阶自测分层和项目验收动作。树序列化与最大路径和的手算图见 [形象化指南的 Day 35](../树与并发专题形象化题解指南.md#day35-visual)。

> **前后关系**：先确认 [Day 34](../day_34/README.md) 的综合同步与树重建契约，再完成本日验收；之后回到 [课程总入口](../../README.md) 选择项目、调试、构建或 C++20 等下一阶段路线。

> 恭喜你完成了35天的C++学习之旅！今天我们将系统性地回顾整个学习内容，巩固知识点，并为未来的进阶学习指明方向。

---

## 📅 学习目标

- **系统回顾**：全面梳理35天学习的核心知识点，形成完整的知识体系框架
- **融会贯通**：理解数据结构、C++特性、设计原则与算法之间的内在联系
- **查漏补缺**：通过综合检验题发现知识盲区，针对性强化薄弱环节
- **能力跃升**：完成两道经典LeetCode题目，检验综合应用能力
- **展望未来**：明确进阶学习路径，为持续成长奠定基础

---

## 📖 35 天知识体系总览：从重复阅读改成检索与证明

Day 1–35 的唯一课表、先修关系和阶段出口统一查看 [35 天科学学习规划](../../CPP_35天科学学习规划_最终版.md)。本日不再逐段复制前四周教程，也不重新摘要 42 条 EMC++；阶段总结的任务是面对一个问题时，能定位主讲文档、写出契约并给出验证证据。

| 主线 | 已建立的能力 | Day 35 的检索问题 | 主入口 |
|---|---|---|---|
| Week 1：数组与类型 | 连续存储、复杂度、类型推导 | 变量是值、引用还是观察者，边界由谁检查？ | [Week 1](../../week_01/README.md) |
| Week 2：链表与所有权 | 节点重连、RAII、智能指针 | 谁拥有节点，删除或异常后哪些指针仍有效？ | [Week 2](../../week_02/README.md) |
| Week 3：栈队列与可调用对象 | 受限容器、BFS、Lambda | 状态放在栈、队列还是闭包中，生命周期够长吗？ | [Week 3](../../week_03/README.md) |
| Week 4：哈希与移动 | 平均复杂度、值类别、转移 | 哈希前提是什么，移动是否真的发生，移动后允许做什么？ | [Week 4](../../week_04/README.md) |
| Week 5：树与并发 | 递归不变量、同步与停止 | 递归深度多大，哪条同步边建立 happens-before，怎样关闭？ | [Week 5](../README.md) |

复习时不要问“我是否见过这个 API”，而要独立回答四句话：输入与失败是什么、状态与所有权由谁维护、正确性不变量是什么、测试怎样让错误稳定失败。若回答不出，再按上表回到主讲处；这比再次浏览一份压缩摘要更能暴露知识缺口。

---

## 📖 知识点回顾一：数据结构选择必须带前提

### 选择矩阵

| 需求 | 候选结构 | 复杂度与成立前提 | 主要失效点 |
|---|---|---|---|
| 连续随机访问 | 数组或 <code>vector</code> | 下标访问 O(1)；中间插删通常 O(n) | 越界、扩容导致引用/迭代器失效 |
| 已知连接位置的重连 | 链表 | 已知节点和所需前驱时修改链接 O(1)；定位仍 O(n) | 所有权断链、悬空观察者 |
| 最近进入者优先 | 栈 | 受限端 push/pop 通常 O(1) | 把容器适配器误当随机访问容器 |
| 先进入者优先或逐层展开 | 队列 | 受限端入队/出队通常 O(1) | BFS 标记过晚导致重复入队 |
| 平均常数时间按键查找 | 哈希表 | 平均 O(1)，依赖哈希分布和负载；最坏可 O(n) | 等价关系与哈希不一致、rehash 失效 |
| 保持有序并做范围查询 | 平衡关联结构 | 搜索/插删通常 O(log n) | 把普通 BST 退化情况忽略 |
| 层次关系与递归分解 | 树 | 一次遍历 O(n)，额外空间取决于 h 或 w | 深树调用栈、所有权和输入结构不合法 |

复杂度必须说清输入规模和空间口径。树 DFS 的额外空间通常是 O(h)，层序 BFS 队列是 O(w)；哈希 O(1) 是平均界；链表 O(1) 插删不包含定位；输出所有路径还要把输出总长度计入时间和空间。复杂度不是标签，而是由“不变量维持了什么、每个元素被处理几次”推导出的结果。

### 所有权检查

- 容器或根对象拥有元素与节点，算法中的指针/迭代器通常只观察。
- 返回观察指针时，要说明其有效期不能超过原容器或树所有者。
- 修改容器前先查迭代器、引用和指针失效规则。
- 递归构造树时优先让返回值转移 <code>unique_ptr</code>，异常发生后已建子树自动释放。
- 共享所有权只在确有多个独立 owner 时使用；控制块线程安全不等于被管理对象线程安全。

---

## 📖 知识点回顾二：现代 C++ 是一条语义链

完整语言机制统一回到 [C++ 基础学习教程](../../tutorials/CPP基础学习教程.md)，42 条工程建议统一回到 [Effective Modern C++ 教程](../../tutorials/Effective_Modern_CPP教程.md)。Day 35 只保留把这些机制串起来的检查顺序。

1. **类型**：<code>auto</code>、模板推导和 <code>decltype</code> 得到的类型是什么，顶层 const、引用和数组是否被保留？
2. **值类别**：表达式是左值、将亡值还是纯右值；<code>std::move</code> 只是转换，后续是否移动由重载和类型能力决定。
3. **所有权**：资源由值对象、<code>unique_ptr</code>、<code>shared_ptr</code> 还是外部系统拥有；裸指针和引用是否仅观察。
4. **生命周期**：闭包、异步任务、迭代器和观察指针会不会活过它们引用的对象。
5. **失败语义**：错误是编译失败、异常、未定义行为、有效但未指定状态，还是只有性能不佳。
6. **接口承诺**：<code>const</code>、<code>noexcept</code>、<code>constexpr</code>、<code>override</code> 和 deleted 函数各自承诺什么，又不承诺什么。

### EMC++ 检索地图

| Item | 主题 | 阶段复习时应能解释 |
|---|---|---|
| 1–6 | 推导与 auto | 推导结果、代理类型和花括号初始化差异 |
| 7–16 | 现代接口规则 | 初始化、nullptr、别名、枚举、deleted、override、const_iterator、noexcept、constexpr、const 线程安全 |
| 17–22 | 特殊成员与智能指针 | Rule of Zero、独占/共享/弱观察、Pimpl 完整类型边界 |
| 23–30 | 移动与转发 | move/forward、转发引用、引用折叠和完美转发失败 |
| 31–34 | Lambda | 捕获时机、悬空引用、move-only 捕获和泛型转发 |
| 35–40 | 并发 API | task/thread、启动策略、句柄析构、一次性事件、atomic/volatile |
| 41–42 | 参数与 emplacement | 按值接收的复制移动成本，emplace 的适用条件和例外 |

条款是带适用条件的设计建议，不是语言定律。阶段验收至少要能给每组条款举出一个反例、一个适用边界和一个仓库中的可执行落点；只背标题不算掌握。

---

## 📖 知识点回顾三：算法先声明状态，再写循环或递归

### 通用证明模板

1. **契约**：输入是否合法，空输入怎样处理，结果是否唯一。
2. **状态含义**：指针、窗口、栈、队列或递归返回值在任一时刻代表什么。
3. **不变量**：一次循环或一次递归前后，哪些条件保持成立。
4. **前进与终止**：哪一个区间缩小、哪个节点被标记、为何不会无限重复。
5. **正确性**：终止时不变量为何推出目标结果。
6. **复杂度**：每个元素进出几次，辅助结构最大多大，是否还要计入输出。

| 模式 | 核心状态 | 关键不变量 | 常见错误 |
|---|---|---|---|
| 双指针 | 左右边界或快慢位置 | 被排除区间已不可能包含答案 | 没有单调前进，整数求和溢出 |
| 滑动窗口 | 半开或闭区间及频次 | 窗口状态与边界完全对应 | 收缩条件错误、混淆“至多”和“恰好” |
| DFS | 当前节点、visited、路径 | 调用处理一棵子树或一个搜索状态 | 环图不标记、回溯不撤销 |
| BFS | 队列前沿和距离 | 出队状态按最少边数层次展开 | 入队后才标记，带权图误用普通 BFS |
| 树后序 DP | 子树返回值和全局候选 | 返回值只能表达可连接父节点的状态 | 把当前完整答案直接返回给父节点 |

---

## 📖 知识点回顾四：Week 5 的综合契约

### 树算法检查表

- 根为空时是否有明确定义，叶子是否严格等于“左右孩子都为空”。
- 递归函数是一条返回值契约还是向结果追加的副作用契约。
- 父节点是否独占孩子，算法是否只借用节点而不保存悬空地址。
- 时间是否真为 O(n)，额外空间究竟是 O(h)、O(w) 还是输出规模。
- 退化树令 h 等于 n 时，是否需要显式栈或输入深度上限。
- BST、LCA、重建和序列化的在线评测前提，是否已经转成通用接口可检查的失败条件。

### 并发算法检查表

- 共享状态、mutex、atomic、任务、future 和线程句柄分别由谁拥有。
- 每对冲突访问由 mutex、future、join 或 release/acquire 中哪条边建立 happens-before。
- 条件变量谓词由哪把 mutex 保护，通知之前改变了什么状态。
- close 与 submit/send 如何在线程安全的线性化点决定接受或拒绝。
- 已接受工作是 drain 还是 cancel，停止时谁唤醒，worker 何时退出，谁最终 join。
- 任务异常由谁 get，析构是否可能等待，引用捕获能否活到任务完成。
- 测试是否使用同步门、future、join 和最终不变量，而不是 sleep、日志顺序或固定耗时。

这两张检查表是 Day 35 后续项目的设计评审入口；具体 API 语义继续查阅 [C++ 并发编程教程](../../tutorials/CPP并发编程教程.md) 和本周各日 README。

## 🎯 LeetCode 刷题

### 讲解题：LC 297 二叉树的序列化与反序列化

#### 📝 题目描述

序列化是将一个数据结构或者对象转换为连续的比特位的操作，进而可以将转换后的数据存储在一个文件或者内存中，同时也可以通过网络传输到另一个计算机环境，采取相反方式重构得到原数据。

请设计一个算法来实现二叉树的序列化与反序列化。这里不限定你的序列化/反序列化算法执行逻辑，你只需要保证一个二叉树可以被序列化为一个字符串并且将这个字符串反序列化为原始的树结构。

#### 💡 形象化提示

想象你要把一棵树"打包"成一个字符串，然后能在另一台电脑上"拆包"恢复成完全一样的树。就像把乐高积木拆散后用说明书记录每个积木的位置，收到的人可以照着说明书重新组装。

**关键洞察**：
- 前序或后序配合空节点标记可以唯一重建；层序必须为每个父节点保留左右孩子槽位
- **普通中序序列不能单独唯一表示任意二叉树**：`#,1,#,2,#` 既可解释为 1 的右孩子是 2，也可解释为 2 的左孩子是 1；除非额外加入括号结构或另一种遍历
- 需要用特殊标记（如 `#`）表示空节点，并规定是否允许尾随分隔符、空 token 和数值空白
- 前序遍历便于反序列化：先处理根节点，再递归处理左右子树
- 层序遍历更直观：按层输出，用队列辅助重建

工程版本还必须定义格式和所有权契约：仓库代码用 `unique_ptr` 表达反序列化结果的独占所有权，规范格式不产生尾随逗号，并拒绝开头/结尾/连续逗号形成的空 token、非法或越界整数、提前结束和多余 token。数值解析必须确认整个 token 都被消费；只写 `stoi(token)` 会错误接受 `12abc`。为避免恶意输入耗尽内存或调用栈，教学实现还限制最多 100000 个 token 且树高不超过 1024，超限抛 `length_error`。下面两种算法都假设节点字段名为 `value`，`left`/`right` 是 `unique_ptr<TreeNode>`；序列化参数是只读观察者，调用者仍拥有原树。

#### 🔍 解题思路

**方法一：前序遍历DFS**

序列化：按照"根-左-右"的顺序遍历树，空节点用 `#` 表示，节点之间用逗号连接。

反序列化：按逗号拆分字符串，按照前序遍历的顺序重建树。当前 token 为 `#` 则返回空子树，否则严格解析整数并递归构建左右子树。

下面是与实际 [`solution.h`](code/leetcode/0297_serialize_deserialize/solution.h) 一致的核心递归片段。**代码块性质：局部实现片段，不可单独编译**；省略了头文件、严格 `tokenize`、无尾随分隔符的拼接循环、`TreeNode` 定义以及 token/递归深度限制参数，完整版本由 CMake 编译并由契约测试覆盖。

```cpp
class Codec final {
public:
    std::unique_ptr<TreeNode> deserialize(const std::string& data) const {
        const std::vector<std::string> tokens = tokenize(data);
        std::size_t index = 0U;
        auto root = deserialize_node(tokens, index);
        if (index != tokens.size()) {
            throw std::invalid_argument("serialized tree has trailing tokens");
        }
        return root;
    }

private:
    static void serialize_node(const TreeNode* node,
                               std::vector<std::string>& tokens) {
        if (node == nullptr) {
            tokens.emplace_back("#");
            return;
        }
        tokens.push_back(std::to_string(node->value));
        serialize_node(node->left.get(), tokens);
        serialize_node(node->right.get(), tokens);
    }

    static std::unique_ptr<TreeNode> deserialize_node(
        const std::vector<std::string>& tokens, std::size_t& index) {
        if (index >= tokens.size()) {
            throw std::invalid_argument("serialized tree ended early");
        }
        const std::string& token = tokens[index++];
        if (token == "#") {
            return nullptr;
        }
        const int value = parse_integer_exactly(token);  // from_chars 且 position == end
        auto node = std::make_unique<TreeNode>(value);
        node->left = deserialize_node(tokens, index);
        node->right = deserialize_node(tokens, index);
        return node;
    }
};
```

**方法二：层序遍历BFS**

层序版本复用同一套严格 tokenizer 与整数解析器。**代码块性质：局部替代算法片段，不可单独编译**；省略的 `tokenize`/`parse_integer_exactly` 契约与前序版本相同。

```cpp
class LevelOrderCodec final {
public:
    std::vector<std::string> serialize_tokens(const TreeNode* root) const {
        std::vector<std::string> tokens;
        std::queue<const TreeNode*> pending;
        pending.push(root);
        while (!pending.empty()) {
            const TreeNode* node = pending.front();
            pending.pop();
            if (node != nullptr) {
                tokens.push_back(std::to_string(node->value));
                pending.push(node->left.get());
                pending.push(node->right.get());
            } else {
                tokens.emplace_back("#");
            }
        }
        return tokens;
    }

    std::unique_ptr<TreeNode> deserialize(const std::string& data) const {
        std::queue<std::string> tokens = tokenize_to_queue(data);
        if (tokens.front() == "#") {
            tokens.pop();
            if (!tokens.empty()) {
                throw std::invalid_argument("trailing tree token");
            }
            return nullptr;
        }
        auto root = std::make_unique<TreeNode>(parse_integer_exactly(tokens.front()));
        tokens.pop();
        std::queue<TreeNode*> parents;
        parents.push(root.get());
        // 每个 parent 必须精确消费 left/right 两个 token；缺失或多余都抛异常。
        attach_children_strictly(parents, tokens);
        return root;
    }
};
```

**复杂度分析**：
- 时间复杂度：O(n)，序列化与反序列化都只处理每个节点/空标记常数次。
- 输出空间：序列化文本需要 Θ(n) 个 token；反序列化得到的结果树本身需要 Θ(n) 节点。
- 辅助空间：本实现先保存全部 token，因此为 O(n)，其中父节点 BFS 队列峰值为 O(w)，`w` 是最大层宽；若改用流式 token 输入可去掉完整 token 队列。递归 DFS 版本的辅助调用栈则是 O(h)，`h` 是树高。

---

### 实战题：LC 124 二叉树最大路径和

#### 📝 题目描述

二叉树中的路径被定义为一条节点序列，序列中每对相邻节点之间都存在一条边。同一个节点在一条路径序列中至多出现一次。该路径至少包含一个节点，且不一定经过根节点。

路径和是路径中各节点值的总和。给你一个二叉树的根节点root，返回其最大路径和。

#### 💡 形象化提示

想象树是一张连接的城市地图，每个城市有一个收益值（可正可负）。你要找到一条路线，使得总收益最大。这条路线可以从任意城市开始，到任意城市结束，但不能走回头路（一个城市只能经过一次）。

**关键洞察**：
- 路径可以是一条"直线"，也可以是一条"人字形"（在某个节点转弯）
- 对于每个节点，计算以其为转折点的最大路径和：左子树贡献 + 节点值 + 右子树贡献
- 递归返回的是以该节点为一端的最大路径和，只能选择一边（左边或右边或都不选）
- 用“本次调用共享的 `best` 状态”维护最大路径和，通过引用传入递归，避免全局变量破坏可重入性

#### 🔍 解题思路

**核心思想**：后序遍历 + 动态规划

以下代码沿用本日 `unique_ptr` 树节点：父节点独占子树，算法只接收 `const TreeNode*` 观察者，因此递归不转移所有权。

1. 对于每个节点，计算其能贡献给父节点的最大路径和
2. 当前节点作为转折点的候选答案 = 当前值 + max(0, 左贡献) + max(0, 右贡献)
3. 返回给父节点的单臂贡献只能选择一边：当前值 + max(左贡献, 右贡献)
4. 每次加法都必须符合数值接口；本工程结果超出 `std::int64_t` 时抛出 `overflow_error`，而不是执行有符号溢出 UB

下面是与实际 [`solution.h`](code/leetcode/0124_max_path_sum/solution.h) 相同的核心算法。**代码块性质：局部实现片段，不可单独编译**；省略了 `TreeNode` 与标准头，但没有省略空树和溢出契约。

```cpp
class MaxPathSum final {
private:
    static std::int64_t checked_add(std::int64_t left, std::int64_t right) {
        const auto maximum = std::numeric_limits<std::int64_t>::max();
        const auto minimum = std::numeric_limits<std::int64_t>::lowest();
        if ((right > 0 && left > maximum - right) ||
            (right < 0 && left < minimum - right)) {
            throw std::overflow_error("maximum path sum is outside int64_t");
        }
        return left + right;
    }

    static std::int64_t gain(const TreeNode* node, std::int64_t& best) {
        if (node == nullptr) {
            return 0;
        }
        const auto left = std::max<std::int64_t>(0, gain(node->left.get(), best));
        const auto right = std::max<std::int64_t>(0, gain(node->right.get(), best));
        best = std::max(best, checked_add(checked_add(node->value, left), right));
        return checked_add(node->value, std::max(left, right));
    }

public:
    std::int64_t operator()(const TreeNode* root) const {
        if (root == nullptr) {
            throw std::invalid_argument("maximum path requires a non-empty tree");
        }
        std::int64_t best = std::numeric_limits<std::int64_t>::lowest();
        static_cast<void>(gain(root, best));
        return best;
    }
};
```

**详细解释**：

1. **后序遍历**：先处理子树，再处理当前节点，这样我们才能知道子树的最大贡献

2. **贡献值计算**：
   - 左子树贡献 = max(左子树返回值, 0)，如果是负数就不选
   - 右子树贡献同理

3. **路径和计算**：
   - 以当前节点为"最高点"的路径：左贡献 + 节点值 + 右贡献
   - 这条路径无法延伸到父节点（因为已经用尽了左右两边）

4. **返回值**：
   - 只能选择一边延伸到父节点：节点值 + max(左贡献, 右贡献)

**复杂度分析**：
- 时间复杂度：O(n)，每个节点访问一次
- 空间复杂度：O(h)，递归栈深度为树高

关键不变量是：递归返回值只能是一条能继续接到父节点的“单臂路径”，而全局答案可以在当前节点把左右两臂合并。全负树不能把答案初始化为 0，否则会错误地返回空路径；题目要求路径至少含一个节点。仓库实现把状态放在单次调用的局部变量中，空树是契约错误，并用受检 `std::int64_t` 加法明确报告越界；把节点类型简单扩大为 `long long` 只能扩大范围，不能让 `LLONG_MAX + 1` 变得合法。

---

## 📊 学习成果自测

自测分成两层，不能把分数混在一起。核心阶段验收只覆盖本课程已经系统讲解或实际练过的内容；进阶挑战保留用于规划下一阶段，未完成表示“尚未学习对应先修”，不等于 Week 1–5 基础未掌握。

| 分区 | 题号 | 如何解释结果 |
|---|---|---|
| 核心阶段验收 | 1–3、5–15 | 应能独立说明不变量、所有权、复杂度或失败方式；不会时回到对应周复习 |
| 进阶挑战 | 4、16–20 | 不计入 Day 35 结业判断；先完成标注的先修，再把已学模板迁移到新模型 |

### 核心阶段验收：课程已教内容

#### 数据结构篇

- **题 1（数组）**：给定一个有序数组，原地删除重复元素，使得每个元素最多出现两次，返回新长度。要求O(1)空间复杂度。

- **题 2（链表）**：如何判断链表是否有环？如果有环，如何找到环的入口节点？

- **题 3（栈）**：设计一个支持O(1)时间获取最小元素的栈。

- **题 5（树）**：给定二叉树的前序和中序遍历序列，重建二叉树。

#### C++特性篇

- **题 6（auto）**：以下代码中，a、b、c的类型分别是什么？
```cpp
int x = 10;
auto a = x;
auto& b = x;
auto&& c = 10;
```

- **题 7（智能指针）**：shared_ptr 的控制块并发保证到哪里为止，为什么它不等于所管理对象线程安全？

- **题 8（Lambda）**：以下Lambda的捕获列表有什么问题？
```cpp
#include <functional>

std::function<int()> createMultiplier(int factor) {
    return [&]() { return factor * 2; };
}
```

- **题 9（移动语义）**：解释std::move和std::forward的区别。

- **题 10（并发）**：如何避免死锁？RAII锁管理器是如何帮助的？

#### EMC++条款篇

- **题 11**：为什么优先使用nullptr而不是NULL？

- **题 12**：unique_ptr为什么比auto_ptr更安全？

- **题 13**：什么情况下编译器不会生成移动构造函数？

- **题 14**：为什么说“转发引用重载”是危险的？

- **题 15**：volatile和atomic的区别是什么？

### 进阶挑战：保留内容，但不计入核心结业

下面六题都能复用课程中的局部工具，但还需要新模型。建议顺序是题 18（网格建图）→ 题 16（一维动态规划）→ 题 4（组合数据结构与随机性）→ 题 20（树形动态规划）→ 题 19（图染色）→ 题 17（二维动态规划）；每完成一题，先写状态定义和失败边界，再写代码。

- **题 4（组合数据结构挑战）**：设计一个数据结构，支持O(1)时间的插入、删除和获取随机元素。先修是动态数组尾部交换删除、哈希索引同步、均匀随机数与接口不变量；课程讲过数组和哈希，但没有系统完成这一组合设计。

- **题 16（动态规划入门）**：给定一个包含正负数的数组，找出和最大的连续子数组。先修是“以当前位置结尾”的状态定义、状态转移和空输入契约；Kadane 是该递推的常用名称，不属于本阶段必会术语。

- **题 17（二维动态规划）**：给定一个字符串s和模式串p，实现支持'.'和'*'的正则表达式匹配。先修是二维 DP、模式合法性和 `*` 对前一元素的零次/多次解释；课程只讲过普通字符串处理，没有系统教授正则匹配。

- **题 18（DFS/BFS 迁移）**：给定一个二维网格地图，计算岛屿的数量。先修是把网格单元建模为图顶点、定义四邻接并管理 visited；课程已教 DFS/BFS，但网格建模仍是一次新迁移。

- **题 19（图论挑战）**：给定一个无向图，判断是否为二分图。先修是邻接表、连通分量和二染色不变量；“会 BFS”本身还不足以推出二分图判定。

- **题 20（树形动态规划）**：给定一棵二叉树，找到最深的叶子节点的最近公共祖先。先修是让后序递归同时返回子树深度与候选祖先；课程讲过普通 LCA 和树高，但没有系统推导这个组合状态。

### 参考答案提示

<details>
<summary>点击查看答案提示</summary>

#### 核心阶段答案提示

- **题 1**：双指针：快慢指针，快指针遍历，慢指针记录有效位置。

- **题 2**：快慢指针相遇则环存在；相遇后一指针从头开始，两指针同步移动，再次相遇即为入口。

- **题 3**：辅助栈记录最小值，或在栈节点中额外存储当前最小值。

- **题 5**：前序第一个元素为根，在中序中找到根位置，左边为左子树，右边为右子树，递归处理。

- **题 6**：a是int，b是int&，c是int&&（`auto&&` 在这里是转发引用，并被右值初始化）。

- **题 7**：不同 shared_ptr 实例可以并发复制和销毁，因为控制块计数按标准要求安全更新；但多个线程通过它们访问同一个可变对象仍需同步，同一个 shared_ptr 对象本身被一边读一边写也不能当然安全。

- **题 8**：引用捕获局部变量factor，函数返回后factor被销毁，产生悬垂引用。

- **题 9**：`std::move` 无条件把表达式转换成 xvalue，`std::forward<T>` 根据推导得到的 `T` 条件恢复左值或右值类别；是否真的移动资源仍由后续重载决议和目标类型能力决定。

- **题 10**：按固定顺序获取锁；lock_guard/unique_lock自动释放锁，即使异常也会析构。

- **题 11**：nullptr是std::nullptr_t类型，不会与整型重载混淆。

- **题 12**：unique_ptr独占所有权，禁止拷贝，只能移动。

- **题 12 补充**：`auto_ptr` 的“复制”会从源对象偷走所有权，使按值传参和容器操作出现意外失效；`unique_ptr` 直接删除复制操作，只允许显式移动，所有权转移在类型和调用点上都可见。

- **题 13**：当类用户声明了拷贝构造、拷贝赋值、移动构造、移动赋值或析构函数时，编译器不会再同时为它隐式声明默认移动操作；成员自身不可移动时，即使写 `= default`，对应操作也可能被定义为 deleted。工程上优先 Rule of Zero，需要直接管理资源时再成套设计 Rule of Five。

- **题 14**：转发引用匹配范围极广，会导致重载决议意外匹配。

- **题 15**：volatile不保证原子性和内存序，atomic提供线程安全的原子操作。

#### 进阶挑战路线提示

- **题 4**：哈希表+数组：数组存储元素支持随机访问，哈希表存储值到索引的映射。删除时把末尾元素移到洞位，并同步更新索引；随机接口还要说明均匀性和空容器行为。

- **题 16**：动态规划或Kadane算法，维护当前最大和全局最大。

- **题 17**：动态规划，dp[i][j]表示s[0:i]与p[0:j]是否匹配。

- **题 18**：DFS/BFS遍历，访问过的标记为'0'，计数连通分量。

- **题 19**：BFS染色，相邻节点颜色不同，冲突则非二分图。

- **题 20**：BFS找最深层节点，递归找最近公共祖先；也可一次后序返回“最大深度+该深度所有叶子的公共祖先”，避免先收集最深叶再做额外搜索。

</details>

---

## 🚀 运行代码

```bash
# 进入Day 35目录
cd week_05/day_35

# 脚本已随仓库保存为可执行文件，直接编译并运行
./build_and_run.sh
```

### 预期输出

```
100% tests passed, 0 tests failed out of 8
```

其中 `day35_emcpp_review` 不只检查条目数量，还核对并发条款与 Item 41–42 的具体位置；`day35_lc0297_contract` 覆盖尾随/连续分隔符、部分整数、越界整数、精确边界、token 总量和树高限制；`day35_lc0124_contract` 验证 `INT64_MAX + 1` 被报告为溢出而不是执行 UB。输出文字可随教学说明调整，测试只断言稳定语义，不把展示格式当作接口。

### 今日工程动作：写一页项目验收契约

本日不要求立刻写一个几千行项目，而是先把“大项目”拆成可独立构建和测试的模块。仓库中的 `code/main.cpp` 给出一个树任务服务清单：`tree_model` 位于第 0 层，只定义所有权模型；`codec`/`algorithms`/`executor` 位于第 1 层，分别负责文本边界、纯计算和任务停止；`tree_service` 位于第 2 层，只编排接口。依赖必须严格从高层指向低层，因而自依赖、底层反向包含应用层和任意环都会让契约测试失败。

建议落地后的目录形状如下，公开头、实现、应用和测试不混放：

```text
tree_service/
├── CMakeLists.txt
├── include/tree_service/      # Tree、Codec、Executor 的稳定公开接口
├── src/                       # 私有实现与不导出的辅助类型
├── app/main.cpp               # 组合模块，不承载算法细节
└── tests/                     # 单元、接口契约、停止与 sanitizer 回归
```

可执行动作只有一个：打开 `code/main.cpp`，为你准备继续实现的项目改写 `modules` 和 `ProjectContract`，逐模块写清公开接口、所有者、不变量、层级和允许依赖，再运行 `ctest -R day35_project_contract --test-dir build --output-on-failure`。先故意把 `tree_service` 的依赖改成不存在的模块、让模块依赖自己，再让 `codec` 反向依赖 `tree_service`，逐次确认清单测试以非零退出；恢复后运行完整 `./build_and_run.sh`。随后把这张清单作为真正拆分 library target、app target 和 test target 的依据，而不是按“一个功能一个巨大 cpp”继续堆代码。

---

## 📚 相关术语汇总

| 术语 | 英文 | 定义 |
|------|------|------|
| 时间复杂度 | Time Complexity | 算法执行时间与输入规模的增长关系 |
| 空间复杂度 | Space Complexity | 算法占用空间与输入规模的增长关系 |
| RAII | Resource Acquisition Is Initialization | 资源获取即初始化，利用对象生命周期管理资源 |
| 右值引用 | Rvalue Reference | 绑定到右值的引用类型 T&& |
| 移动语义 | Move Semantics | 允许类型通过右值重载复用或转移资源；实际行为由类型契约决定 |
| 完美转发 | Perfect Forwarding | 保持参数原有值类别的转发 |
| 转发引用 | Forwarding Reference | 特定推导语境下可保留左值或右值类别的 `T&&`/`auto&&` |
| 引用折叠 | Reference Collapsing | 多层引用的简化规则 |
| 智能指针 | Smart Pointer | 自动管理内存的指针封装类 |
| 控制块 | Control Block | shared_ptr管理的引用计数等元数据 |
| Lambda表达式 | Lambda Expression | 匿名函数的语法糖 |
| 闭包 | Closure | Lambda捕获变量后生成的函数对象 |
| 深拷贝 | Deep Copy | 复制对象及其所有资源 |
| 浅拷贝 | Shallow Copy | 仅复制指针，共享资源 |
| DFS | Depth-First Search | 深度优先搜索 |
| BFS | Breadth-First Search | 广度优先搜索 |
| 前序遍历 | Preorder Traversal | 根-左-右 |
| 中序遍历 | Inorder Traversal | 左-根-右 |
| 后序遍历 | Postorder Traversal | 左-右-根 |
| 层序遍历 | Level Order Traversal | 按层从上到下、从左到右 |

---

## 💡 进阶学习建议

恭喜你完成了35天的C++学习之旅！这只是编程世界的入门，以下是进阶学习建议：

### 知识深化

1. **数据结构进阶**：学习红黑树、B+树、跳表、布隆过滤器等高级数据结构，理解它们在数据库和分布式系统中的应用。

2. **算法进阶**：深入研究动态规划、贪心算法、图论算法、字符串匹配算法，参加Codeforces/AtCoder等算法竞赛提升实战能力。

3. **C++深入**：学习模板元编程、C++17/20新特性（结构化绑定、协程、概念）、编译器优化技术。

### 实践项目

1. **系统编程**：实现一个简易数据库、HTTP服务器、内存池、线程池。

2. **开源贡献**：参与LLVM、Boost、folly等知名开源项目，学习工业级代码风格。

3. **性能优化**：学习性能分析工具（perf、VTune）、内存分析工具（Valgrind、ASan）、缓存优化技术。

### 推荐书籍

1. 《Effective C++》- Scott Meyers
2. 《More Effective C++》- Scott Meyers
3. 《Effective Modern C++》- Scott Meyers
4. 《C++ Concurrency in Action》- Anthony Williams
5. 《算法导论》- CLRS
6. 《编程珠玑》- Jon Bentley

### 在线资源

1. [cppreference.com](https://en.cppreference.com/) - C++标准库参考
2. [Compiler Explorer](https://godbolt.org/) - 在线查看编译结果
3. [LeetCode](https://leetcode.com/) - 算法练习平台
4. [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/) - C++编码规范

---

## 🔗 参考资料

1. [C++ working draft](https://eel.is/c++draft/)
2. [cppreference](https://en.cppreference.com/w/)
3. [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)
4. Stanley B. Lippman 等，*C++ Primer*（语言、标准库与对象模型基础）
5. Scott Meyers, *Effective Modern C++*（42 条工程建议及适用边界）
6. Anthony Williams, *C++ Concurrency in Action*（并发语义与工程协议）
7. Robert Sedgewick / Kevin Wayne, *Algorithms*（不变量与复杂度组织）
8. [LeetCode 题库](https://leetcode.com/problemset/)
9. [仓库 Effective Modern C++ 教程](../../tutorials/Effective_Modern_CPP教程.md)
10. [仓库并发编程教程](../../tutorials/CPP并发编程教程.md)

---

> "学习的目的不是为了记住所有知识，而是建立一套能够快速定位和解决问题的知识体系。35天只是一个开始，保持学习，持续进步！"

**祝你编程之路越走越远！** 🚀

---

## 五句复盘（恰好五句）

1. 数据结构复杂度必须连同平均最坏情况、内存布局和输入前提一起说明。
2. 现代 C++ 的核心不是堆叠语法，而是用类型和 RAII 表达所有权、寿命与失败处理。
3. EMC++ 条款是带适用条件的设计建议，必须理解反例与代价后再使用。
4. 并发模块只有在不变量、异常传播和停止协议都可测试时才算完成。
5. 阶段学习的交付物应是可构建代码、失败会报错的测试和能够解释设计取舍的项目记录。
