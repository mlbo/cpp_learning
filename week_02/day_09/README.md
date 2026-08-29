# Day 9: 链表操作技巧与智能指针进阶

> **学习定位**：在 Day 8 的独占所有权基础上引入共享所有权。先画控制块和节点关系，再学习 `shared_ptr`；务必区分“引用计数操作安全”与“所指对象线程安全”。

## 阅读导航

- `shared_ptr` 的控制块和线程安全边界是本日主讲；标准接口见 [cppreference shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr)。
- 合并两链表的完整不变量见 [LeetCode 21](code/leetcode/0021_merge_lists/README.md)，环检测的手算图见 [链表形象化指南](../链表专题形象化题解指南.md) 的 Day 9 部分。
- 前接 [Day 8 的独占所有权](../day_08/README.md)，后接 [Day 10 的弱观察与循环引用](../day_10/README.md)。

## 今日概览

| 主题 | 内容 |
|------|------|
| 数据结构 | 快慢指针、虚拟头节点 |
| C++11特性 | shared_ptr详解 |
| EMC++条款 | Item 19（shared_ptr 共享所有权）及其机制与成本 |
| LeetCode | 21题、141题 |

---

## 一、数据结构：链表操作技巧

### 1.1 快慢指针（Fast-Slow Pointer）

快慢指针是链表操作中最优雅的技巧之一，使用两个指针以不同速度遍历链表。

#### 基本原理

```cpp
ListNode* slow = head;
ListNode* fast = head;
while (fast && fast->next) {
    slow = slow->next;           // 慢指针走一步
    fast = fast->next->next;     // 快指针走两步
}
```

#### 应用场景

1. **找链表中点**
   - 当快指针到达末尾时，慢指针正好在中间
   - 常用于归并排序链表

2. **判断是否有环**
   - 如果链表有环，快指针最终会追上慢指针
   - 这是 Floyd 判圈算法的核心

3. **找环的入口**
   - 先用快慢指针判断有环
   - 再从头出发一个指针，与慢指针相遇即为入口

#### 为什么算法一定停下来？

- **无环**：`fast` 每轮向链尾前进两步。有限链表的最后一个 `next` 是空，因此 `fast == nullptr` 或 `fast->next == nullptr` 必然发生。
- **有环**：两指针进入环后，只看它们在长度为 `L` 的环上的相对距离。每轮 `fast` 比 `slow` 多走一步，相对距离按模 `L` 增加 1，所以最多 `L` 轮必然变为 0，也就是相遇。

找中点还要先约定偶数长度的策略。上述 `slow = head, fast = head` 写法在 `[1,2,3,4]` 中返回后中点 `3`；如果归并排序需要前中点，应调整初始位置或循环条件。算法代码相同不代表接口语义自动确定。

#### 图示理解

```
找中点：
[1] -> [2] -> [3] -> [4] -> [5] -> nullptr
  ^            ^            ^
 slow         slow         fast (停止时)

判断环：
[1] -> [2] -> [3] -> [4] -> [5] ---+
                      ^            |
                      |<-----------+
                   slow == fast (相遇)
```

### 1.2 虚拟头节点（Dummy Node）

虚拟头节点是一个在链表头部额外添加的哨兵节点，简化边界条件处理。

#### 为什么需要虚拟头节点？

```cpp
// 不使用虚拟头节点 - 需要特殊处理头节点
ListNode* deleteNode(ListNode* head, int val) {
    if (head->val == val) {
        ListNode* tmp = head;
        head = head->next;
        delete tmp;
        return head;
    }
    ListNode* cur = head;
    while (cur->next && cur->next->val != val) {
        cur = cur->next;
    }
    // ... 删除 cur->next
}

// 使用虚拟头节点 - 统一处理
ListNode* deleteNode(ListNode* head, int val) {
    if (head == nullptr) {
        return nullptr;
    }

    ListNode dummy(0, head);  // 栈对象，函数结束时自动销毁
    ListNode* cur = &dummy;
    while (cur->next && cur->next->val != val) {
        cur = cur->next;
    }
    if (cur->next) {
        ListNode* toDelete = cur->next;
        cur->next = toDelete->next;
        delete toDelete;
    }
    return dummy.next;
}
```

#### 典型应用

- 链表删除节点
- 链表合并
- 链表反转（部分反转）

---

## 二、C++11特性：shared_ptr详解

### 2.1 基本概念

`std::shared_ptr` 是 C++11 引入的智能指针，采用**引用计数**实现共享所有权。

#### 引用计数原理

```cpp
std::shared_ptr<int> sp1 = std::make_shared<int>(42);
// 引用计数 = 1

{
    std::shared_ptr<int> sp2 = sp1;  // 拷贝构造
    // 引用计数 = 2
    
    std::shared_ptr<int> sp3 = sp1;  // 再次拷贝
    // 引用计数 = 3
}  // sp2, sp3 离开作用域
// 引用计数 = 1

// 只有 sp1 持有资源，引用计数 = 1
```

### 2.2 控制块（Control Block）

`shared_ptr` 的核心是控制块，它存储：

```
shared_ptr 副本 A ─┬─ 对象指针 ───────────────> Widget 对象
                  └─ 控制块指针 ──┐
shared_ptr 副本 B ─┬─ 对象指针 ───┼──────────> 同一个 Widget 对象
                  └─ 控制块指针 ──┘
                                   │
                                   v
                         ┌───────────────────┐
                         │ 强计数 / 弱计数   │
                         │ 删除器 / 分配器   │
                         └───────────────────┘
```

`shared_ptr` 中“指向对象的指针”和“指向控制块的指针”是两个概念，别把它简化成对象内部自带一个计数器。别名构造甚至允许对象指针指向子对象，而控制块仍拥有外层对象。强计数归零时调用删除器销毁对象；只要还有 `weak_ptr`，控制块通常仍需保留，直到弱计数也归零。

#### 创建控制块的时机

1. **`std::make_shared`**（推荐）
   ```cpp
   auto sp = std::make_shared<int>(42);
   // 典型实现用一次分配同时容纳对象和控制块
   ```

2. **从裸指针构造**
   ```cpp
   int* p = new int(42);
   std::shared_ptr<int> sp(p);
   // 创建控制块，但需要注意：
   // 不要用同一个裸指针创建多个 shared_ptr！
   ```

若再执行 `std::shared_ptr<int> sp2(p)`，不是“计数变成 2”，而是产生两个都认为自己是唯一所有者的控制块，最终 double delete。正确共享方式是复制已有的 `sp`。

### 2.3 shared_ptr vs unique_ptr

| 特性 | shared_ptr | unique_ptr |
|------|-----------|------------|
| 所有权 | 共享 | 独占 |
| 大小 | 典型实现为2个指针 | 默认删除器的 unique_ptr 典型为1个指针 |
| 性能 | 控制块和计数更新有额外成本 | 默认删除器时通常接近裸指针访问成本 |
| 使用场景 | 多处需要管理同一资源 | 明确单一所有权 |

### 2.4 常用操作

```cpp
// 创建
auto sp1 = std::make_shared<int>(42);
auto sp2 = std::make_shared<std::vector<int>>(100, 0);

// 拷贝与移动
auto sp3 = sp1;           // 拷贝：引用计数+1
auto sp4 = std::move(sp1);// 移动：引用计数不变，sp1变空

// 检查与访问
if (sp3) {                // 检查是否为空
    std::cout << *sp3;    // 解引用
}

// 获取信息
std::cout << sp3.use_count();  // 引用计数
std::cout << sp3.unique();     // 是否唯一所有者(C++17已弃用)

// 重置
sp3.reset();              // 释放所有权
sp3.reset(new int(100));  // 释放旧资源，管理新资源
```

`use_count()` 适合调试和观察，不适合做并发正确性判断：读取计数后，其他线程可能立刻增加或减少所有者。业务逻辑若依赖“现在只有我一个所有者”，通常说明同步或所有权模型设计有问题。

### 2.5 成本与线程安全边界

复制/销毁 `shared_ptr` 通常要对控制块计数执行原子增减。原子操作避免控制块被并发破坏，但也会带来同步和缓存行竞争成本。这不等于“`shared_ptr` 里的对象自动线程安全”。

| 场景 | 是否自动安全 |
|------|--------------|
| 不同线程操作各自的 `shared_ptr` 副本 | 控制块计数安全 |
| 多线程同时读写同一个 `shared_ptr` 变量 | 普通 `shared_ptr` 不保证；需同步或使用相应原子接口 |
| 多线程通过不同副本修改同一个 `T` 对象 | `T` 自己决定，通常仍需锁 |

接口也要表达是否保留所有权：

```cpp
void inspect(const Widget&);                 // 只访问对象，不延长生命期
void schedule(std::shared_ptr<Widget>);       // 可能保存一份共享所有权
void replace(std::shared_ptr<Widget>& slot);  // 明确要改调用者的指针变量，较少见
```

仅仅为了调用 `Widget` 成员函数，不要传 `const shared_ptr<Widget>&`；那会把无关的所有权类型泄漏进接口。按值传 `shared_ptr` 的一次计数增加，则是“被调用者可能保存所有权”的有意成本。

### 2.6 循环引用问题（预览）

```cpp
class Node {
public:
    std::shared_ptr<Node> next;
    std::shared_ptr<Node> prev;  // 危险！
};

// 循环引用：
// A.next = B; B.prev = A;
// A 和 B 的引用计数都不会归零 → 内存泄漏！
```

解决方案：使用 `std::weak_ptr` 打破循环（Day 10详解）

---

## 三、EMC++ Item 19：shared_ptr 共享所有权详解

> 编号纠正：Effective Modern C++ 的 Item 19 是“使用 `shared_ptr` 管理共享所有权”；Item 20 是“当 `shared_ptr` 可能悬空时使用 `weak_ptr`”，留到 Day 10。下面的大小、控制块和性能内容是 Item 19 的机制补充，不是另一个名为“shared_ptr 性能”的 Item 20。

### 条款19：使用shared_ptr管理共享所有权的资源

#### 核心要点

1. **shared_ptr 提供共享所有权语义**
   ```cpp
   auto widget = std::make_shared<Widget>();
   // 多个 shared_ptr 可以安全地共享所有权
   processWidget(widget);  // 函数可能存储 widget
   // 即使 processWidget 存储了它，widget 仍然有效
   ```

2. **引用计数的开销**
   - 控制块需要动态分配
   - 引用计数增减需要原子操作（线程安全）
   - 这也是 shared_ptr 比 unique_ptr 慢的原因

3. **避免从同一个裸指针创建多个 shared_ptr**
   ```cpp
   int* p = new int(42);
   std::shared_ptr<int> sp1(p);
   std::shared_ptr<int> sp2(p);  // 灾难！两个控制块
   // 当 sp1 和 sp2 都销毁时，p 被删除两次！
   ```

#### 最佳实践

```cpp
// 推荐：使用 make_shared
auto sp = std::make_shared<Widget>();

// 如果必须使用自定义删除器
std::shared_ptr<FILE> file(
    fopen("test.txt", "r"),
    [](FILE* f) { if(f) fclose(f); }
);
```

### Item 19 的机制与性能补充

#### 性能分析

1. **内存开销**
   ```cpp
   sizeof(std::shared_ptr<T>)  // 通常是 2 * sizeof(void*)
   // 一个指针指向对象，一个指针指向控制块
   ```

2. **控制块开销**
   - 单独的内存分配
   - 原子操作的引用计数

3. **`make_shared` 的优势**
   ```cpp
   // 方法1：通常分别为对象和控制块分配
   std::shared_ptr<Widget> sp1(new Widget());
   
   // 方法2：通常合并为一次分配（推荐）
   auto sp2 = std::make_shared<Widget>();
   ```

#### 线程安全性

```cpp
// 引用计数的原子操作是线程安全的
std::shared_ptr<int> global_sp = std::make_shared<int>(42);

// 安全示意：两个线程各自拿到一个副本后，再操作各自的副本。
// 若它们同时从同一个 global_sp 变量读写，仍需同步。

// 但访问对象本身需要同步！
// *global_sp = 100;  // 不安全：需要额外同步
```

#### 性能优化建议

1. 优先使用 `unique_ptr`，只在需要共享所有权时用 `shared_ptr`
2. 使用 `make_shared` 减少内存分配
3. 函数参数按所有权语义选择：
   - 只访问对象：传 `T&` 或 `const T&`
   - 函数可能保存共享所有权：按值传 `shared_ptr<T>`
   - 函数要改调用者的智能指针变量：才考虑传引用

```cpp
// 不涉及所有权：直接借用对象
void observe(const Widget& widget);

// 涉及所有权：传值
void takeOwnership(std::shared_ptr<Widget> sp);

// 调用时
observe(*widget);          // 不复制 shared_ptr，不更新强引用计数
takeOwnership(std::move(widget));  // 移动语义
```

---

## 四、LeetCode题解

### 21. 合并两个有序链表

#### 题目描述
将两个升序链表合并为一个新的升序链表。

#### 解法一：迭代（使用虚拟头节点）

```cpp
ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
    ListNode dummy(0);       // 虚拟头节点
    ListNode* tail = &dummy;
    
    while (list1 && list2) {
        if (list1->val < list2->val) {
            tail->next = list1;
            list1 = list1->next;
        } else {
            tail->next = list2;
            list2 = list2->next;
        }
        tail = tail->next;
    }
    
    tail->next = list1 ? list1 : list2;
    return dummy.next;
}
```

理解迭代法不要只记模板。循环不变量是：`dummy.next ... tail` 始终是已经合并好的有序前缀，`tail` 始终指向该前缀最后一个节点；`list1` 和 `list2` 则分别指向两个尚未合并的有序后缀。每轮取两个后缀头部较小者，四个条件继续成立，因此循环结束后可以直接接上唯一剩余的有序后缀。

**复杂度**：时间 O(n+m)，空间 O(1)

#### 解法二：递归

```cpp
ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
    if (!list1) return list2;
    if (!list2) return list1;
    
    if (list1->val < list2->val) {
        list1->next = mergeTwoLists(list1->next, list2);
        return list1;
    } else {
        list2->next = mergeTwoLists(list1, list2->next);
        return list2;
    }
}
```

**复杂度**：时间 O(n+m)，空间 O(n+m)（递归栈）

### 141. 环形链表

#### 题目描述
判断链表是否有环。

#### 解法：快慢指针

```cpp
bool hasCycle(ListNode* head) {
    if (!head || !head->next) return false;
    
    ListNode* slow = head;
    ListNode* fast = head;
    
    while (fast && fast->next) {
        slow = slow->next;       // 慢指针走1步
        fast = fast->next->next; // 快指针走2步
        
        if (slow == fast) {
            return true;  // 相遇，有环
        }
    }
    
    return false;  // 快指针到达末尾，无环
}
```

**原理证明**：
- 如果有环，快指针最终会进入环
- 在环中，快指针相对于慢指针每步多走1步
- 进入环后相对距离每轮按模环长增加 1，必然会在有限步内变为 0

**复杂度**：时间 O(n)，空间 O(1)

---

## 五、今日练习

### 必做题
- [x] LeetCode 21：合并两个有序链表
- [x] LeetCode 141：环形链表

### 选做题
- [ ] LeetCode 876：链表的中间节点
- [ ] LeetCode 142：环形链表 II（找环入口）
- [ ] LeetCode 23：合并K个有序链表

### 代码实践
1. 使用 `shared_ptr` 实现链表节点
2. 实现带虚拟头节点的链表操作
3. 比较迭代和递归解法的性能

### 今日工程动作：按所有权语义检查接口

把你写过的接口分别标成“只访问对象 / 临时共享并可能复制 / 保存共享所有权”。只访问时传 `T&`/`const T&`，只有确实需要共享或保存所有权时才让 `shared_ptr` 出现在接口中。

### 五句复盘

1. **核心问题**：什么时候多个参与者确实需要共同延长同一对象的生命期？
2. **旧误解**：`shared_ptr` 不是普通裸指针加一个对象内计数器，引用计数安全也不代表所指对象线程安全。
3. **规则前提**：共享所有权依赖同一个控制块；从同一裸指针创建两个控制块会导致重复释放。
4. **测试/反例证据**：21 的合并不变量、141 的有环/无环用例，以及双控制块反例共同验证算法和所有权边界。
5. **与前后课连接**：本日从 Day 8 的独占所有权扩展到共享所有权，Day 10 将用 `weak_ptr` 表达不延长生命期的观察关系。

---

## 六、编译运行

```bash
cd week_02/day_09
./build_and_run.sh
```

脚本会构建主示例、虚拟头节点独立示例、EMC++ 扩展示例以及两道算法测试，并运行 CTest。虚拟头节点与快慢指针示例使用了两个不同的教学节点类型，因此分别编译，避免同名 `ListNode` 混在一个程序中违反单一定义规则。

---

## 七、知识点总结

```
Day 9 核心知识点
│
├── 链表技巧
│   ├── 快慢指针：找中点、判环、找环入口
│   └── 虚拟头节点：统一边界处理
│
├── shared_ptr
│   ├── 引用计数原理
│   ├── 控制块结构
│   ├── make_shared 的优势
│   ├── 线程安全边界与原子计数成本
│   └── 循环引用问题预览
│
├── EMC++ 条款
│   └── Item 19：共享所有权管理（含控制块与成本机制）
│
└── 算法思想
    ├── 快慢指针的数学原理
    └── 递归 vs 迭代的权衡
```

---

## 八、明日预告

Day 10 将学习：
- weak_ptr 与循环引用解决
- EMC++ Item 20（weak_ptr）与 Item 21（make 函数）
- LeetCode 142（环入口）、19（删除倒数第 N 个节点）
