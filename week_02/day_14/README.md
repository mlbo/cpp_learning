# Day 14: 第二周复习与综合练习

## 📚 本周知识图谱

```mermaid
mindmap
  root((第二周<br/>Day 8-14))
    智能指针
      unique_ptr
        独占所有权
        移动语义
        自定义删除器
      shared_ptr
        共享所有权
        引用计数
        循环引用问题
      weak_ptr
        打破循环引用
        观察者模式
        lock()方法
    右值引用与移动语义
      左值vs右值
        类型特征
        绑定规则
      移动构造/赋值
        资源转移
        noexcept保证
      完美转发
        std::forward
        万能引用
    资源管理
      RAII原则
        资源获取即初始化
        作用域绑定生命周期
      EMC++条款17-22
        独占资源用unique_ptr
        共享资源用shared_ptr
        使用make函数
        用weak_ptr打破循环
        移动语义优化
    链表进阶
      经典操作
        反转链表
        快慢指针
        合并有序链表
      LeetCode实战
        234.回文链表
        138.随机链表复制
    并发编程基础
      线程安全
        mutex保护
        lock_guard
        死锁预防
```

## 📖 学习目标

1. **系统复习**第二周所有知识点
2. **深入理解**EMC++条款17-22的设计思想
3. **掌握**链表的高级操作技巧
4. **实践**线程安全编程模式

---

## 📝 本周知识总结

### Day 8-9: 智能指针

| 智能指针 | 所有权模式 | 引用计数 | 适用场景 |
|---------|-----------|---------|---------|
| `unique_ptr` | 独占 | 无 | 独占资源、工厂返回 |
| `shared_ptr` | 共享 | 有 | 共享资源、缓存 |
| `weak_ptr` | 无 | 不增加 | 打破循环、观察者 |

**核心要点：**
- 优先使用`unique_ptr`，只在需要共享时才用`shared_ptr`
- 使用`make_unique`和`make_shared`创建智能指针
- `weak_ptr::lock()`安全访问资源

### Day 10-11: 右值引用与移动语义

```cpp
// 左值 vs 右值
int x = 10;           // x是左值，10是右值
int& lr = x;          // 左值引用绑定左值
int&& rr = 10;        // 右值引用绑定右值
int&& rr2 = std::move(x);  // move将左值转为右值

// 移动构造函数
class Widget {
public:
    Widget(Widget&& other) noexcept
        : data_(std::move(other.data_)) {}
};
```

### Day 12-13: 链表与资源管理

**链表核心技巧：**
1. 快慢指针找中点
2. 反转链表（迭代/递归）
3. 哨兵节点简化边界

---

## 🎯 EMC++条款17-22要点

### 条款17: 理解特殊成员函数的生成

```cpp
class Widget {
public:
    Widget();                              // 默认构造
    ~Widget();                             // 析构
    Widget(const Widget&);                 // 拷贝构造
    Widget& operator=(const Widget&);      // 拷贝赋值
    Widget(Widget&&);                      // 移动构造
    Widget& operator=(Widget&&);           // 移动赋值
};

// C++11规则：
// - 声明析构函数：不阻止移动操作生成，但可能被弃用
// - 声明拷贝操作：阻止移动操作生成
// - 声明移动操作：阻止拷贝操作生成
```

### 条款18: 使用unique_ptr管理独占资源

```cpp
// 创建方式
auto p1 = std::make_unique<Widget>();
auto p2 = std::unique_ptr<Widget>(new Widget());

// 自定义删除器
auto deleter = [](FILE* f) { fclose(f); };
std::unique_ptr<FILE, decltype(deleter)> file(fopen("test.txt", "r"), deleter);

// 工厂模式返回
std::unique_ptr<Widget> createWidget() {
    return std::make_unique<Widget>();
}
```

### 条款19: 使用shared_ptr管理共享资源

```cpp
// 引用计数机制
auto sp1 = std::make_shared<Widget>();  // 控制块+对象一次分配
auto sp2 = sp1;  // 引用计数+1
// 当sp1和sp2都析构时，Widget才被销毁

// 注意：裸指针构造多个shared_ptr会导致重复释放
Widget* raw = new Widget();
std::shared_ptr<Widget> p1(raw);
std::shared_ptr<Widget> p2(raw);  // 危险！会导致double-free
```

### 条款20: 使用weak_ptr处理循环引用

```cpp
class Node {
public:
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> prev;  // 使用weak_ptr打破循环
};

// 观察者模式
class Observer {
public:
    virtual void notify() = 0;
};

class Subject {
    std::vector<std::weak_ptr<Observer>> observers_;
public:
    void notifyAll() {
        for (auto& weakObs : observers_) {
            if (auto obs = weakObs.lock()) {  // 安全访问
                obs->notify();
            }
        }
    }
};
```

### 条款21: 优先使用make函数

```cpp
// 推荐方式
auto up = std::make_unique<Widget>(arg1, arg2);
auto sp = std::make_shared<Widget>(arg1, arg2);

// make的优势：
// 1. 异常安全：避免函数调用之间的内存泄漏
// 2. 性能优化：shared_ptr一次分配完成对象和控制块
// 3. 代码简洁：不重复写类型名

// 不适用make的场景：
// 1. 需要自定义删除器
// 2. 对象内存对齐有特殊要求
```

### 条款22: 使用Pimpl模式减少编译依赖

```cpp
// Widget.h
class Widget {
public:
    Widget();
    ~Widget();
private:
    class Impl;  // 前向声明
    std::unique_ptr<Impl> pImpl_;
};

// Widget.cpp
class Widget::Impl {
    // 实现细节放在这里
    std::string name_;
    std::vector<int> data_;
};

Widget::Widget() : pImpl_(std::make_unique<Impl>()) {}
Widget::~Widget() = default;  // 必须在Impl定义之后
```

---

## 🧩 LeetCode题目解析

### 234. 回文链表

**难度：简单** | **标签：链表、双指针**

#### 题目描述
给你一个单链表的头节点 `head`，请你判断该链表是否为回文链表。

#### 解题思路
1. **快慢指针**找到链表中点
2. **反转**后半部分链表
3. **比较**前后两半
4. **恢复**链表（可选）

#### 复杂度分析
- 时间复杂度：O(n)
- 空间复杂度：O(1)

```cpp
bool isPalindrome(ListNode* head) {
    if (!head || !head->next) return true;
    
    // 1. 快慢指针找中点
    ListNode* slow = head;
    ListNode* fast = head;
    while (fast->next && fast->next->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    
    // 2. 反转后半部分
    ListNode* second = reverseList(slow->next);
    
    // 3. 比较两半
    ListNode* first = head;
    ListNode* temp = second;
    bool result = true;
    while (temp) {
        if (first->val != temp->val) {
            result = false;
            break;
        }
        first = first->next;
        temp = temp->next;
    }
    
    // 4. 恢复链表（可选）
    slow->next = reverseList(second);
    
    return result;
}
```

---

### 138. 随机链表的复制

**难度：中等** | **标签：链表、哈希表**

#### 题目描述
给你一个长度为 `n` 的链表，每个节点包含一个额外增加的随机指针 `random`，该指针可以指向链表中的任何节点或空节点。请构造这个链表的深拷贝。

#### 解法一：哈希表法

**复杂度：** O(n) 时间，O(n) 空间

```cpp
Node* copyRandomList(Node* head) {
    if (!head) return nullptr;
    
    // 第一遍：创建节点并建立映射
    std::unordered_map<Node*, Node*> mapping;
    Node* curr = head;
    while (curr) {
        mapping[curr] = new Node(curr->val);
        curr = curr->next;
    }
    
    // 第二遍：设置next和random指针
    curr = head;
    while (curr) {
        mapping[curr]->next = mapping[curr->next];
        mapping[curr]->random = mapping[curr->random];
        curr = curr->next;
    }
    
    return mapping[head];
}
```

#### 解法二：节点拆分法

**复杂度：** O(n) 时间，O(1) 空间

```cpp
Node* copyRandomList(Node* head) {
    if (!head) return nullptr;
    
    // 第一步：在原节点后插入复制节点
    // A -> B -> C  变成  A -> A' -> B -> B' -> C -> C'
    Node* curr = head;
    while (curr) {
        Node* copy = new Node(curr->val);
        copy->next = curr->next;
        curr->next = copy;
        curr = copy->next;
    }
    
    // 第二步：设置random指针
    curr = head;
    while (curr) {
        if (curr->random) {
            curr->next->random = curr->random->next;
        }
        curr = curr->next->next;
    }
    
    // 第三步：拆分链表
    Node* dummy = new Node(0);
    Node* copyCurr = dummy;
    curr = head;
    while (curr) {
        copyCurr->next = curr->next;
        copyCurr = copyCurr->next;
        curr->next = curr->next->next;
        curr = curr->next;
    }
    
    Node* result = dummy->next;
    delete dummy;
    return result;
}
```

---

## 🚀 综合项目：线程安全链表

### 设计要点

1. **细粒度锁**：每个节点一个mutex，提高并发度
2. **RAII锁管理**：使用`std::lock_guard`和`std::unique_lock`
3. **异常安全**：确保操作失败不破坏数据结构
4. **接口设计**：提供安全的遍历和修改接口

### 核心实现

```cpp
template<typename T>
class ThreadSafeList {
private:
    struct Node {
        std::mutex m;
        std::shared_ptr<T> data;
        std::unique_ptr<Node> next;
        
        Node() : data(nullptr) {}
        Node(const T& val) : data(std::make_shared<T>(val)) {}
    };
    
    Node head;  // 哨兵节点

public:
    void push_front(const T& value) {
        std::unique_ptr<Node> new_node = std::make_unique<Node>(value);
        std::lock_guard<std::mutex> lk(head.m);
        new_node->next = std::move(head.next);
        head.next = std::move(new_node);
    }
    
    template<typename Func>
    void for_each(Func f) {
        Node* current = &head;
        std::unique_lock<std::mutex> lk(head.m);
        while (Node* next = current->next.get()) {
            std::unique_lock<std::mutex> next_lk(next->m);
            lk.unlock();  // 释放前一个锁
            f(*next->data);
            current = next;
            lk = std::move(next_lk);  // 转移锁所有权
        }
    }
};
```

---

## 📁 代码结构

```
day_14/
├── README.md                    # 本教程文档
├── CMakeLists.txt               # CMake构建配置
├── build_and_run.sh             # 一键构建运行脚本
└── code/
    ├── main.cpp                 # 主程序入口
    ├── review/
    │   ├── week2_summary.cpp    # 本周知识点总结代码
    │   ├── emcpp_review.cpp     # EMC++条款复习
    │   └── exercises.cpp        # 综合练习题
    ├── project/
    │   ├── thread_safe_list.h   # 线程安全链表头文件
    │   ├── thread_safe_list.cpp # 实现
    │   └── test_thread_safe.cpp # 测试用例
    └── leetcode/
        ├── 0234_palindrome/     # 回文链表
        │   ├── solution.h
        │   ├── solution.cpp
        │   ├── test.cpp
        │   └── README.md
        └── 0138_copy_random_list/ # 随机链表复制
            ├── solution.h
            ├── solution.cpp
            ├── test.cpp
            └── README.md
```

---

## 🔧 构建与运行

```bash
# 方式一：使用脚本
cd day_14
chmod +x build_and_run.sh
./build_and_run.sh

# 方式二：手动构建
mkdir build && cd build
cmake ..
make -j4
./day14_main
```

---

## 📋 练习任务

### 基础任务
- [ ] 阅读并理解所有复习代码
- [ ] 完成LeetCode 234题的提交
- [ ] 完成LeetCode 138题的提交

### 进阶任务
- [ ] 实现线程安全链表的删除操作
- [ ] 添加线程安全链表的迭代器支持
- [ ] 使用`std::shared_mutex`实现读写锁优化

### 挑战任务
- [ ] 实现无锁链表（使用`std::atomic`）
- [ ] 对比细粒度锁与单一锁的性能差异
- [ ] 实现线程安全的跳表

---

## 📚 参考资料

1. **Effective Modern C++** - Scott Meyers
   - Item 17-22: 智能指针与资源管理

2. **C++ Concurrency in Action** - Anthony Williams
   - Chapter 6: Designing lock-based concurrent data structures

3. **LeetCode官方题解**
   - [234. 回文链表](https://leetcode.cn/problems/palindrome-linked-list/)
   - [138. 随机链表的复制](https://leetcode.cn/problems/copy-list-with-random-pointer/)

---

## 🎉 第二周总结

恭喜你完成了第二周的学习！本周我们深入探索了：

- **智能指针**：现代C++资源管理的基石
- **移动语义**：理解右值引用和完美转发
- **链表算法**：快慢指针、反转链表等核心技巧
- **线程安全**：并发编程的基础实践

下周我们将进入**STL容器**的深入学习，包括序列容器、关联容器和无序容器。

**Keep coding, keep learning!** 🚀
