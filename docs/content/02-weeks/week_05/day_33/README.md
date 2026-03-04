# Day 33：树路径问题

## 📅 学习目标

今天我们将系统学习二叉树中的路径问题，这是一类经典的DFS应用问题。路径问题涉及在树中寻找满足特定条件的路径，如所有从根到叶子的路径、和为特定值的路径等。这些问题不仅考察对DFS的理解，还需要掌握回溯法和路径记录技巧。同时，我们将学习线程池的实现原理，它是并发编程中的重要组件，可以避免频繁创建销毁线程的开销。最后，我们还将学习Effective Modern C++中Item 11-16的重要条款，这些条款涵盖了函数删除、override关键字、const迭代器等实用技巧。

**本日学习重点：**
- 掌握二叉树路径问题的解题模板
- 理解回溯法在路径问题中的应用
- 学会实现简单的线程池
- 掌握EMC++ Item 11-16的核心要点
- 完成两道经典路径问题LeetCode题目

---

## 📖 知识点一：树路径问题

### 概念定义

在二叉树中，路径是指从一个节点出发，沿着父子连接到达另一个节点的节点序列。根据路径的定义不同，可以分为：
- **根到叶子路径**：从根节点到某个叶子节点的路径
- **任意路径**：可以从任意节点开始，到任意节点结束
- **向下路径**：必须从父节点向子节点方向

路径问题是DFS的经典应用，核心思路是在遍历过程中记录当前路径，到达目标节点时保存结果。

### 核心解题模板

**模板1：记录所有根到叶子路径**

```cpp
void dfs(TreeNode* root, vector<int>& path, vector<vector<int>>& result) {
    if (root == nullptr) return;
    
    path.push_back(root->val);  // 添加当前节点
    
    if (root->left == nullptr && root->right == nullptr) {
        result.push_back(path);  // 到达叶子，保存路径
    } else {
        dfs(root->left, path, result);
        dfs(root->right, path, result);
    }
    
    path.pop_back();  // 回溯：移除当前节点
}
```

**模板2：寻找和为target的路径**

```cpp
void dfs(TreeNode* root, int target, vector<int>& path, vector<vector<int>>& result) {
    if (root == nullptr) return;
    
    path.push_back(root->val);
    target -= root->val;
    
    if (root->left == nullptr && root->right == nullptr && target == 0) {
        result.push_back(path);
    } else {
        dfs(root->left, target, path, result);
        dfs(root->right, target, path, result);
    }
    
    path.pop_back();  // 回溯
}
```

### 回溯法的精髓

回溯法是一种通过探索所有可能情况来找到所有解的算法。在树路径问题中，回溯体现在：
1. **选择**：将当前节点加入路径
2. **探索**：递归处理子节点
3. **撤销选择**：将当前节点从路径中移除，以便探索其他分支

---

## 📖 知识点二：线程池

### 概念定义

线程池是一种并发编程设计模式，它预先创建一组工作线程，形成一个"池"，任务提交时从池中取出空闲线程执行，执行完毕后线程返回池中等待下一个任务。线程池避免了频繁创建和销毁线程的开销，同时可以控制并发线程数量，防止资源耗尽。

### 简单线程池实现

```cpp
class ThreadPool {
public:
    ThreadPool(size_t numThreads) : stop_(false) {
        for (size_t i = 0; i < numThreads; ++i) {
            workers_.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(mtx_);
                        cv_.wait(lock, [this] { return stop_ || !tasks_.empty(); });
                        if (stop_ && tasks_.empty()) return;
                        task = std::move(tasks_.front());
                        tasks_.pop();
                    }
                    task();  // 执行任务
                }
            });
        }
    }
    
    ~ThreadPool() {
        {
            std::lock_guard<std::mutex> lock(mtx_);
            stop_ = true;
        }
        cv_.notify_all();
        for (auto& worker : workers_) {
            worker.join();
        }
    }
    
    template<class F>
    void enqueue(F&& task) {
        {
            std::lock_guard<std::mutex> lock(mtx_);
            tasks_.emplace(std::forward<F>(task));
        }
        cv_.notify_one();
    }

private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex mtx_;
    std::condition_variable cv_;
    bool stop_;
};
```

---

## 📖 知识点三：EMC++ Item 11-16

### Item 11: 优先使用deleted函数而非private未定义函数

删除函数比声明为private且不定义更加清晰和安全。

```cpp
// 推荐：使用 = delete
class Widget {
public:
    Widget(const Widget&) = delete;
    Widget& operator=(const Widget&) = delete;
};

// 不推荐：private + 不定义
class OldWay {
private:
    OldWay(const OldWay&);  // 不定义
};
```

### Item 12: 将要覆盖的函数声明为override

使用override关键字可以避免因签名不匹配导致的意外隐藏。

```cpp
class Derived : public Base {
public:
    void foo() override;  // 明确表示覆盖基类函数
};
```

### Item 13: 优先使用const_iterator而非iterator

const_iterator提供更好的安全性，C++11简化了获取方式。

```cpp
// C++11
auto it = std::find(v.cbegin(), v.cend(), target);
```

### Item 14: 如果函数不会抛出异常，声明为noexcept

noexcept可以向编译器提供优化机会，同时明确表达意图。

```cpp
void swap(Widget& other) noexcept {
    // 交换操作
}
```

### Item 15: 尽可能使用constexpr

constexpr允许在编译期计算，提高运行时性能。

```cpp
constexpr int square(int x) { return x * x; }
constexpr int result = square(5);  // 编译期计算
```

### Item 16: 确保const成员函数线程安全

const成员函数在多线程环境下可能被并发调用，需要保证线程安全。

```cpp
class Widget {
public:
    int getValue() const {
        std::lock_guard<std::mutex> lock(mtx_);
        return value_;
    }
private:
    mutable std::mutex mtx_;
    int value_;
};
```

---

## 🎯 LeetCode 刷题

### 讲解题：LC 257 二叉树的所有路径

#### 题目链接
[LeetCode 257](https://leetcode.cn/problems/binary-tree-paths/)

#### 题目描述
给定一个二叉树，返回所有从根节点到叶子节点的路径。

#### 解题思路

使用DFS+回溯，遍历过程中记录路径。

```cpp
vector<string> binaryTreePaths(TreeNode* root) {
    vector<string> result;
    vector<int> path;
    dfs(root, path, result);
    return result;
}

void dfs(TreeNode* root, vector<int>& path, vector<string>& result) {
    if (root == nullptr) return;
    
    path.push_back(root->val);
    
    if (root->left == nullptr && root->right == nullptr) {
        // 构建路径字符串
        string s;
        for (int i = 0; i < path.size(); ++i) {
            s += to_string(path[i]);
            if (i < path.size() - 1) s += "->";
        }
        result.push_back(s);
    } else {
        dfs(root->left, path, result);
        dfs(root->right, path, result);
    }
    
    path.pop_back();
}
```

---

### 实战题：LC 113 路径总和 II

#### 题目链接
[LeetCode 113](https://leetcode.cn/problems/path-sum-ii/)

#### 题目描述
给定一个二叉树和一个目标和，找到所有从根节点到叶子节点路径总和等于给定目标和的路径。

#### 解题思路

在DFS过程中记录路径和，到达叶子节点时判断是否满足条件。

```cpp
vector<vector<int>> pathSum(TreeNode* root, int targetSum) {
    vector<vector<int>> result;
    vector<int> path;
    dfs(root, targetSum, path, result);
    return result;
}

void dfs(TreeNode* root, int target, vector<int>& path, vector<vector<int>>& result) {
    if (root == nullptr) return;
    
    path.push_back(root->val);
    
    if (root->left == nullptr && root->right == nullptr && target == root->val) {
        result.push_back(path);
    } else {
        dfs(root->left, target - root->val, path, result);
        dfs(root->right, target - root->val, path, result);
    }
    
    path.pop_back();
}
```

---

## 🚀 运行代码

```bash
cd /home/z/my-project/download/week_05/day_33
./build_and_run.sh
```

---

## 📚 相关术语

| 术语 | 英文 | 定义 |
|------|------|------|
| 路径 | Path | 从一个节点到另一个节点的节点序列 |
| 回溯 | Backtracking | 通过撤销选择来探索所有可能解的算法 |
| 线程池 | Thread Pool | 预创建线程组，避免频繁创建销毁 |
| deleted函数 | Deleted Function | 使用=delete禁止的函数 |
| override | 覆盖 | 明确表示覆盖基类虚函数 |
| constexpr | 常量表达式 | 编译期可计算的值或函数 |

---

## 💡 学习提示

1. **路径问题核心**：记住"添加-递归-移除"的回溯模板，适用于大多数路径问题。

2. **线程池使用场景**：当需要处理大量短任务时，使用线程池比每次创建新线程效率高得多。

3. **EMC++实践**：在日常编码中养成好习惯：覆盖函数加override，不抛异常加noexcept。

---

## 🔗 参考资料

1. [cppreference - std::thread](https://en.cppreference.com/w/cpp/thread/thread)
2. [LeetCode 257 - Binary Tree Paths](https://leetcode.com/problems/binary-tree-paths/)
3. [LeetCode 113 - Path Sum II](https://leetcode.com/problems/path-sum-ii/)
4. *Effective Modern C++* - Item 11-16
