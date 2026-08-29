# Day 33：树路径、线程池与 EMC++ Item 11–16

> **学习定位**：树路径问题要求维护“选择—递归—撤销”的状态；线程池要求维护“提交—排队—执行—停止”的生命周期。本日 Item 11–16 是此前分散工程规则的补课，不建议只背条款标题。

> **共性入口**：路径 push/pop 的手算见 [形象化指南的 Day 33](../树与并发专题形象化题解指南.md#day33-visual)；线程池、条件变量和 future 的完整机制见 [C++ 并发编程教程](../../tutorials/CPP并发编程教程.md)；Item 11–16 主讲见 [Effective Modern C++ 教程](../../tutorials/Effective_Modern_CPP教程.md)。本日只展开“异常安全回溯”和“接受—拒绝—排空—停止—join”的工程协议。

> **前后关系**：上一日 [Day 32](../day_32/README.md) 分别学习 DFS 和单对象原子同步；下一日 [Day 34](../day_34/README.md) 用可关闭通道和选择矩阵把 thread、mutex、condition_variable、future 与 atomic 统一起来。

## 学习目标

完成今天的学习后，你应该能够：

- 区分“根到叶子”“任意节点之间”“只能向下”等不同路径定义；
- 写出 DFS 回溯的不变量，并说清借用节点、临时路径和结果副本各由谁拥有；
- 解决 LC 257 与 LC 113，处理空树、单节点、负数和“必须到叶子”等边界；
- 解释线程池的提交、排队、执行、停止协议，以及 drain 和 reject 的含义；
- 用 `std::future` 取得任务结果和异常，并安全提交 move-only 参数；
- 区分按值转移任务参数与通过 `std::ref` 显式借用对象；
- 理解 Effective Modern C++ Item 11–16 的机制、适用条件和误用后果；
- 把每个示例做成会报告失败的可执行测试，并让 CMake/CTest 覆盖全部源文件。

## 一、先把“路径”定义清楚

二叉树路径是由父子边连接的节点序列，但题目通常还会附加方向和端点约束。

| 类型 | 起点 | 终点 | 方向 | 本日例题 |
|---|---|---|---|---|
| 根到叶子路径 | 根 | 叶子 | 只能向下 | LC 257、LC 113 |
| 向下路径 | 任意节点 | 任意后代 | 只能向下 | 常见路径和变体 |
| 任意路径 | 任意节点 | 任意节点 | 可经过公共祖先 | 不可直接套本日模板 |

“叶子”是左右孩子都为空的节点，不是“当前累计值恰好满足条件”的节点。以 LC 113 为例，即使从根走到某个中间节点时和已经等于目标，也不能立即记录答案，因为题目要求终点必须是叶子。

### DFS 的接口契约与所有权

先给递归函数写契约，再写实现：

下块是局部接口声明，**不可单独编译**；省略 `<vector>` 和上文树节点 `TreeNode` 定义。`node` 是借用观察指针，`Path`/`Paths` 不保存节点地址。

```cpp
using Path = std::vector<int>;
using Paths = std::vector<Path>;

void dfs(const TreeNode* node, Path& current, Paths& result);
```

- `node` 是借用指针：DFS 可以读取节点，但不删除节点，也不把指针保存到结果中；调用者必须保证整次调用期间树仍然存在。
- `current` 是调用链共享的临时工作区：进入某层时，它表示“根到父节点”的路径。
- 当前层先把自己的值压入 `current`，递归孩子，返回前再弹出自己的值。
- `result.push_back(current)` 会复制一条完整路径，所以以后回溯修改 `current` 不会改坏已经保存的答案。
- 函数正常返回时，`current` 必须恢复到进入函数时的内容，这就是回溯不变量。

可以把一层递归看成一个小事务：

```text
进入：current == 根到父节点
选择：push_back(当前值)
探索：处理叶子或递归左右孩子
撤销：pop_back()
离开：current 再次 == 根到父节点
```

如果忘记 `pop_back()`，左子树留下的节点会污染右子树；如果在递归孩子之前就弹出，孩子会丢失祖先；如果把 `current` 的地址存入答案，则所有答案共享同一可变对象。这些失败都来自破坏不变量，而不是“DFS 语法没背熟”。

### 引用回溯、值语义与 RAII

引用回溯只维护一份 `vector`，通常分配和复制更少：

下块是局部回溯实现，**不可单独编译**；复用上块的 `Path`/`Paths` 和上文 `TreeNode { int value; ... }` 接口。工程版本用 RAII 守卫保证异常退栈时也执行撤销。

```cpp
void collect(const TreeNode* node, Path& current, Paths& result) {
    if (node == nullptr) {
        return;
    }

    current.push_back(node->value);
    if (node->left == nullptr && node->right == nullptr) {
        result.push_back(current);
    } else {
        collect(node->left, current, result);
        collect(node->right, current, result);
    }
    current.pop_back();
}
```

把 `current` 按值传递也正确，因为每个分支拥有自己的副本，因而不需要显式撤销；代价是沿途可能复制很多路径元素。按值版本适合先建立正确性直觉，引用版本适合练习显式不变量。

普通题解通常只讨论正常返回。若 `result.push_back` 因内存分配失败而抛异常，手写的 `pop_back` 不会执行；工程代码可以让一个局部 RAII 守卫在析构时弹出元素。本目录的 [树路径演示](code/data_structure/tree_path_demo.cpp) 展示了这种“获取即登记、离开作用域自动撤销”的写法。

### 边界与复杂度

- 空树没有根到叶子路径，应该返回空结果。
- 单节点既是根又是叶子，因此构成一条路径。
- 节点值可能为负数，不能用“当前和超过目标就剪枝”，除非题目明确保证剩余值非负。
- 累计和可用更宽的 `std::int64_t`，避免多个 `int` 相加时溢出。
- 递归额外空间是 `O(h)`，其中 `h` 是树高；极端链状树可能导致调用栈过深，此时可改用显式栈。
- 输出本身不能忽略：复制所有答案的成本至少与输出总长度成正比，不能笼统地只写“时间 `O(n)`”。

## 二、LC 257：二叉树的所有路径

[题目：LeetCode 257](https://leetcode.cn/problems/binary-tree-paths/)

题目要求把每条根到叶子路径格式化为 `1->2->5`。本质仍是保存路径，只是结果类型变成字符串。

### 三种策略

1. 递归时按值携带字符串，每个分支拥有自己的文本。
2. 用 `vector<int>` 回溯，到叶子时统一拼接字符串。
3. 用 BFS 队列保存“节点 + 到该节点的字符串”，避免递归调用栈。

回溯版本的关键仍然是对称的 push/pop：

下块是 LeetCode 257 的局部片段，**不可单独编译**；省略 `<vector>`、`<string>`、`TreeNode { int val; ... }` 和格式化函数 `buildPathString`。

```cpp
void dfs(const TreeNode* node,
         std::vector<int>& path,
         std::vector<std::string>& result) {
    if (node == nullptr) {
        return;
    }

    path.push_back(node->val);
    if (node->left == nullptr && node->right == nullptr) {
        result.push_back(buildPathString(path));
    } else {
        dfs(node->left, path, result);
        dfs(node->right, path, result);
    }
    path.pop_back();
}
```

不要把“必须到叶子”写成“遇到空指针就保存”，否则一个只有左孩子的节点可能被错误记录。完整的三种实现和空树、单节点负值测试见 [LC 257 实现](code/leetcode/0257_binary_tree_paths/solution.cpp)。

## 三、LC 113：路径总和 II

[题目：LeetCode 113](https://leetcode.cn/problems/path-sum-ii/)

与 LC 257 相比，路径所有权没有变化，只多了一个“剩余目标”状态。进入节点后令 `next_remaining = remaining - node->val`，只有叶子且 `next_remaining == 0` 才保存路径。

下块是 LeetCode 113 的局部片段，**不可单独编译**；省略 `<cstdint>`、`Path`/`Paths` 别名和 `TreeNode { int val; ... }`。用 `int64_t` 保存剩余值只是扩大范围，不允许超出该类型的数学和。

```cpp
void dfs(const TreeNode* node,
         std::int64_t remaining,
         Path& path,
         Paths& result) {
    if (node == nullptr) {
        return;
    }

    path.push_back(node->val);
    const auto next_remaining = remaining - node->val;
    if (node->left == nullptr && node->right == nullptr) {
        if (next_remaining == 0) {
            result.push_back(path);
        }
    } else {
        dfs(node->left, next_remaining, path, result);
        dfs(node->right, next_remaining, path, result);
    }
    path.pop_back();
}
```

失败方式包括：把中间节点当答案、只递归到一边、在一个分支命中后提前结束而漏掉其他答案、用全局路径却没有清理，以及用 `int` 累加超出范围。完整的回溯、按值和显式栈实现见 [LC 113 实现](code/leetcode/0113_path_sum_ii/solution.cpp)。

## 四、线程池：把生命周期写成协议

线程池不是“有一个线程数组和任务队列”就结束了。一个可用接口必须回答：谁能提交、提交何时算成功、停止后队列怎么办、任务异常到哪里去、析构何时返回。

### 状态与不变量

本日实现采用以下状态协议：

```text
Running --锁内关闭接受--> Draining --队列为空、worker 退出--> Joining --外部 join--> Stopped
   |                            |
   | submit 先锁内入队          | submit 拒绝
   v                            v
任务成为“已接受”             已接受任务继续执行
```

- `Running`：`accepting == true`，提交可在线程池互斥锁保护下进入队列。
- `Draining`：停止接收新任务，但此前成功入队的任务仍会全部执行。
- `Joining`：工作线程已经因“停止且队列为空”离开循环，外部 owner 正在回收线程句柄。
- `Stopped`：任务队列为空，所有工作线程都已 `join`，`shutdown()` 可以安全返回。
- 队列、`accepting` 和停止标志由同一把互斥锁保护；不能在锁外读写普通 `bool`。
- “提交成功”的线性化点是任务在锁内进入队列；若停止状态先取得同一把锁，则该提交抛异常并被拒绝。

`drain` 的意思是排空已接收任务再退出，不是立即丢弃队列。另一种合理策略是 `cancel pending`，但它必须定义被丢弃任务的 `future` 得到什么异常；本日实现没有偷偷混合两种策略。

### 工作线程为何使用条件变量

工作线程循环执行四步：

1. 用 `std::unique_lock` 锁住状态；
2. `condition_variable::wait(lock, predicate)` 等待“请求停止或队列非空”；
3. 在锁内取走一个任务，在队列为空且已请求停止时退出；
4. 解锁后执行任务。

谓词必须同时处理虚假唤醒和“通知发生在真正等待之前”的情况。任务必须在锁外执行，否则一个慢任务会长时间占住队列锁，其他工作线程无法取任务，提交者也无法入队，线程池实际上会被串行化。

### 提交、结果与异常

`submit` 返回 `std::future<R>`，而不是只接受一个无结果的 `std::function<void()>`：

下块是线程池局部用法，**不可单独编译**；省略 `<iostream>`、外围函数和已构造的 `ThreadPool pool`。`get()` 既取值又观察任务异常。

```cpp
auto answer = pool.submit([](int x) { return x * x; }, 6);
std::cout << answer.get(); // 36，同时也是完成同步点
```

实现先把 callable 和参数衰减为线程池拥有的值，再用 `tuple` 保存参数，并通过 `std::apply`/`std::invoke` 做一次性移动调用。这样 `unique_ptr` 等 move-only 参数可以把所有权转入任务；若任务要修改调用者仍拥有的对象，必须显式传 `std::ref(object)`，并保证该对象活到 future 就绪。包装后的调用放入 `std::packaged_task<R()>`，再用一个可复制的小包装器进入 `std::function<void()>` 队列。

任务正常返回时，值进入共享状态；任务抛异常时，`packaged_task` 把异常保存到共享状态，工作线程不会因该用户异常退出，随后 `future::get()` 在等待方重新抛出它。`submit` 与 `shutdown` 用同一把锁确定先后：提交先取得锁就完成参数转移并成为必须 drain 的已接收任务；停止先取得锁则提交抛异常，而且尚未衰减或移动调用者参数。

`future::get()` 只能调用一次；它既取得结果，也建立线程间同步。若调用者既不保存 future，也不检查异常，任务失败就可能被静默忽略。

### 停止、析构与失败边界

本日的 `shutdown()` 是幂等 drain，固定顺序如下：

1. **接受或拒绝**：`submit` 与 `shutdown` 争用同一把状态锁；先完成入队的任务被接受，关门之后到达的提交被拒绝。
2. **排空**：已接受任务仍保留在队列中，worker 逐个取走并把值或异常写入各自 future。
3. **停止并唤醒**：`shutdown` 在锁内设置停止状态，解锁后 `notify_all()`，让空闲 worker 重新检查“停止或队列非空”谓词。
4. **退出**：worker 只在“已停止接受且队列为空”时离开循环；任务必须在队列锁外执行。
5. **join**：只有线程池外部 owner 可以等待所有 worker，全部 join 后 `shutdown()` 才返回。

若池内任务调用公开 `shutdown()`，实现会在改变状态前抛出 `logic_error`，异常由该任务的 future 观察，避免 worker 对自己执行 `join`。析构仍要求外部所有者比全部任务活得更久；在池内任务中销毁池会让 worker 继续访问已经销毁的内部状态，不是可以靠 `detach` 修补的用法。

必须同时遵守这些契约：

- 工作线程数量不能为零，否则任务永远没有执行者；构造函数会拒绝零。
- 停止后的 `submit` 抛出异常，调用者不能假设任务“也许会执行”。
- 任务捕获的引用必须活到任务完成；优先按值捕获，或用有明确共享所有权的 `shared_ptr`。
- 不要从线程池自己的任务中销毁该线程池，否则对象生命周期和“线程 join 自己”都无法满足。
- 任务若等待同一线程池中尚未运行的另一个任务，小池可能发生饥饿死锁。
- 这里的队列没有容量上限，生产速度长期高于消费速度时会占用越来越多内存；生产系统通常还要背压、限流或有界队列。

完整实现及确定性测试见 [线程池示例](code/cpp11_features/thread_pool_demo.cpp)。测试没有用 `sleep_for`：`shutdown()` 返回保证 drain 与 join 完成，`future::get()` 保证单个结果就绪；需要把并发执行停在某个阶段时，用 promise/future 或受谓词保护的门闩等待明确事件。测试也不把任务执行顺序、线程 ID 或“某毫秒内完成”当断言，因为调度顺序不是接口契约。

## 五、Effective Modern C++ Item 11–16

下面每条都按“机制—为什么—边界”学习，配套可执行示例见 [Item 11–16 代码](code/emcpp/item11_16_misc.cpp)。

<a id="item-11"></a>
### Item 11：优先使用 deleted 函数

```cpp
class SocketOwner {
public:
    SocketOwner(const SocketOwner&) = delete;
    SocketOwner& operator=(const SocketOwner&) = delete;
};
```

旧式写法把拷贝函数声明为 `private` 却不定义：类外误用可能到编译或链接阶段才失败，成员和友元还可能通过访问检查。`= delete` 直接表达“这个函数存在于重载集合，但禁止调用”，诊断更早、更清楚，而且能删除普通函数或某个危险重载，不限于特殊成员函数。删除函数通常应放在 `public`，让编译器先报告“使用了 deleted 函数”，而不是只报告访问权限。

边界是：删除某个重载会影响重载决议，隐式转换可能恰好选中它；应通过编译测试确认接口接受和拒绝的类型。资源类型若仍需要转移所有权，应另外设计移动构造与移动赋值，而不是把“不可拷贝”误解成“不可移动”。

<a id="item-12"></a>
### Item 12：覆盖函数使用 `override`

```cpp
class Base {
public:
    virtual ~Base() = default;
    virtual int id() const noexcept = 0;
};

class Derived final : public Base {
public:
    int id() const noexcept override;
};
```

覆盖要求函数名、参数、`const`、引用限定符等满足规则；少写一个 `const` 可能不是覆盖，而是在派生类中新建同名函数。`override` 让编译器替你核对这一契约。若要经基类指针销毁派生对象，基类析构函数也必须是虚函数；`override` 不能弥补错误的析构设计。`final` 可用于明确禁止继续覆盖或继承，但不应只为“更现代”而滥加。

<a id="item-13"></a>
### Item 13：优先使用 `const_iterator`

下块是容器操作局部片段，**不可单独编译**；省略 `<algorithm>`、外围函数，以及已定义的 `values`/`target`/`new_value`。插入后旧迭代器是否失效由具体容器规则决定。

```cpp
const auto position = std::find(values.cbegin(), values.cend(), target);
values.insert(position, new_value);
```

只读遍历应使用 `cbegin/cend`，这样不能通过迭代器意外修改元素。C++11 起，许多插入和删除位置参数也接受 `const_iterator`，所以“需要修改容器结构”不等于“需要可写元素迭代器”。`const_iterator` 不会让容器本身不可变，也不会避免迭代器失效；例如 `vector` 扩容后，旧迭代器仍然不能再用。

<a id="item-14"></a>
### Item 14：能保证不抛出时再声明 `noexcept`

`noexcept` 是接口承诺，不只是优化提示。异常若逃出 `noexcept` 函数，程序调用 `std::terminate`，调用者没有正常捕获机会。标准容器在扩容时可能优先移动一个 `noexcept` 移动构造的类型，因为失败时更容易维持强异常保证；这也是正确声明移动操作的实际价值。

不要为了性能给尚未审计的函数盲目加 `noexcept`，也不要给会分配内存或调用未知回调的函数轻易承诺不抛。模板包装器常用条件式写法 `noexcept(noexcept(expression))`，让自己的保证跟底层操作一致。

<a id="item-15"></a>
### Item 15：尽可能使用 `constexpr`

下块是编译期/运行期对比片段，**不可单独编译**；省略运行期输入函数 `readValue()` 和外围使用上下文。

```cpp
constexpr int square(int value) noexcept {
    return value * value;
}

constexpr int compile_time = square(5);
int runtime = square(readValue());
```

`constexpr` 函数“可以”参与常量求值，不代表每次调用都发生在编译期；实参不是常量表达式时，它就是普通运行期调用。它能把不变量提前到编译期检查，并让同一个小函数同时服务编译期与运行期。边界包括当前语言版本对函数体的限制、运算是否允许出现在常量表达式中，以及有符号整数溢出仍然不合法；`constexpr` 不是绕过类型和数值范围规则的开关。

<a id="item-16"></a>
### Item 16：让 `const` 成员函数具备线程安全性

`const` 只表示不能通过该 `this` 修改普通数据成员，它不自动建立原子性或 happens-before 关系。逻辑只读函数可能更新缓存、统计或惰性初始化状态；若多个线程共享对象并调用这些函数，普通读写仍会数据竞争。

下块是类内局部片段，**不可单独编译**；省略 `<mutex>` 和外围类定义。`mutex_` 必须是 `mutable`，因为加锁改变同步状态而不改变业务值。

```cpp
int value() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return value_;
}

mutable std::mutex mutex_;
int value_{0};
```

`mutable` 允许 `const` 成员锁住互斥量，因为同步设施不属于对象对外可观察的业务值。单个独立计数器可考虑 `std::atomic`；多个字段必须一起满足不变量时，一把互斥锁往往更清楚。线程安全还应说明粒度：某个成员函数内部安全，不等于“先调用 A 再调用 B”这一组合操作是原子的。

## 六、工程动作：全新构建并运行全部测试

在仓库根目录执行：

```bash
cd week_05/day_33
./build_and_run.sh
```

脚本会删除并新建本日 `build` 目录，以 C++17 Release 配置工程，并启用 `-Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow -Werror`，最后运行全部 CTest。任一编译告警或测试失败都会让脚本返回非零。

| CTest | 验证重点 |
|---|---|
| `day33_main` | 根到叶子路径与临时状态恢复 |
| `day33_tree_path_demo` | 所有路径、路径和、空树、RAII 回溯 |
| `day33_thread_pool_demo` | drain、reject、future 值与异常 |
| `day33_thread_pool_regression` | 池内 shutdown 拒绝 self-join，外部仍可继续提交并最终关闭 |
| `day33_thread_pool_argument_regression` | move-only 参数、`std::ref` 借用与停止拒绝不消费参数 |
| `day33_item11_16` | Item 11–16 的编译期与运行期行为 |
| `day33_lc0257` | 三种 LC 257 实现的一致性和边界 |
| `day33_lc0113` | 三种 LC 113 实现、负数和叶子约束 |

脚本当前运行 8 个 CTest。建议完成后在同一工程动作中给 LC 113 增加“中间节点和等于目标但它不是叶子”的测试，先故意写错叶子判断观察 CTest 如何返回非零，再修复实现并重新运行脚本。这个动作把题意、接口契约和回归测试连接起来，而不是只比较屏幕输出。

## 参考资料

- [C++ working draft：并发支持库](https://eel.is/c++draft/thread)
- [cppreference：`std::condition_variable`](https://en.cppreference.com/w/cpp/thread/condition_variable.html)
- [cppreference：`std::future`](https://en.cppreference.com/w/cpp/thread/future.html)
- [cppreference：`std::packaged_task`](https://en.cppreference.com/w/cpp/thread/packaged_task.html)
- [C++ Core Guidelines CP.42、CP.43、CP.50](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rconc-wait)
- [LeetCode 257](https://leetcode.cn/problems/binary-tree-paths/)
- [LeetCode 113](https://leetcode.cn/problems/path-sum-ii/)
- Scott Meyers, *Effective Modern C++*, Item 11–16
- Anthony Williams, *C++ Concurrency in Action*（线程安全队列、任务与线程池）

## 每日复盘（恰好五句）

1. 我能用“进入与离开时临时路径相同”说明回溯不变量。
2. 我能区分树节点的借用生命周期、递归工作区和结果副本的所有权。
3. 我能解释线程池从提交到排队、执行、drain 停止和 reject 的完整协议。
4. 我能用 future 等待结果并观察任务异常，而不依赖睡眠和调度时序。
5. 我能说明 EMC++ Item 11–16 各自解决的问题及其适用边界。
