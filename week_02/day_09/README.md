# Day 9: 链表操作技巧与智能指针进阶

## 今日概览

| 主题 | 内容 |
|------|------|
| 数据结构 | 快慢指针、虚拟头节点 |
| C++11特性 | shared_ptr详解 |
| EMC++条款 | 19-20（shared_ptr资源管理） |
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
    ListNode* dummy = new ListNode(0, head);
    ListNode* cur = dummy;
    while (cur->next && cur->next->val != val) {
        cur = cur->next;
    }
    // ... 统一删除逻辑
    return dummy->next;
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
┌─────────────────────────────────────┐
│           Control Block             │
├─────────────────────────────────────┤
│  • 强引用计数 (use_count)            │
│  • 弱引用计数 (weak_count)           │
│  • 删除器 (deleter)                  │
│  • 分配器 (allocator)                │
│  • 管理的对象指针                     │
└─────────────────────────────────────┘
```

#### 创建控制块的时机

1. **`std::make_shared`**（推荐）
   ```cpp
   auto sp = std::make_shared<int>(42);
   // 一次内存分配，同时创建对象和控制块
   ```

2. **从裸指针构造**
   ```cpp
   int* p = new int(42);
   std::shared_ptr<int> sp(p);
   // 创建控制块，但需要注意：
   // 不要用同一个裸指针创建多个 shared_ptr！
   ```

### 2.3 shared_ptr vs unique_ptr

| 特性 | shared_ptr | unique_ptr |
|------|-----------|------------|
| 所有权 | 共享 | 独占 |
| 大小 | 2个指针 | 1个指针 |
| 性能 | 有开销 | 零开销 |
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

### 2.5 循环引用问题（预览）

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

## 三、EMC++条款19-20详解

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

### 条款20：shared_ptr 的性能考虑

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
   // 方法1：两次内存分配
   std::shared_ptr<Widget> sp1(new Widget());
   
   // 方法2：一次内存分配（推荐）
   auto sp2 = std::make_shared<Widget>();
   ```

#### 线程安全性

```cpp
// 引用计数的原子操作是线程安全的
std::shared_ptr<int> global_sp = std::make_shared<int>(42);

// 线程1
auto local1 = global_sp;  // 安全：原子递增

// 线程2
auto local2 = global_sp;  // 安全：原子递增

// 但访问对象本身需要同步！
// *global_sp = 100;  // 不安全：需要额外同步
```

#### 性能优化建议

1. 优先使用 `unique_ptr`，只在需要共享所有权时用 `shared_ptr`
2. 使用 `make_shared` 减少内存分配
3. 函数参数传递 `shared_ptr` 时：
   - 不涉及所有权转移：传 `const shared_ptr<T>&`
   - 涉及所有权转移：传值或移动语义

```cpp
// 不涉及所有权：传引用
void observe(const std::shared_ptr<Widget>& sp);

// 涉及所有权：传值
void takeOwnership(std::shared_ptr<Widget> sp);

// 调用时
observe(widget);           // 引用传递，无开销
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
- 必然会在有限步内追上慢指针

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

---

## 六、知识点总结

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
│   └── 循环引用问题预览
│
├── EMC++ 条款
│   ├── 条款19：共享所有权管理
│   └── 条款20：性能考虑
│
└── 算法思想
    ├── 快慢指针的数学原理
    └── 递归 vs 迭代的权衡
```

---

## 七、明日预告

Day 10 将学习：
- weak_ptr 与循环引用解决
- EMC++ 条款21-22
- LeetCode 142（环入口）、234（回文链表）
