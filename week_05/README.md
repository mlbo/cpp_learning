# 第五周：树 + 并发编程

## 📅 本周概览

第五周是C++学习之旅的收官之周，我们将系统学习树形数据结构和并发编程。树是最重要的非线性数据结构，二叉树及其遍历算法是解决众多算法问题的基础。并发编程则是现代多核处理器时代的必备技能，C++11提供的线程库为并发编程提供了强有力的支持。

本周将完成以下目标：
- 掌握二叉树的基本概念和四种遍历方式
- 理解二叉搜索树（BST）的性质和操作
- 掌握DFS算法及其在路径问题中的应用
- 深入理解进程线程模型和线程安全
- 熟练运用C++11并发组件（thread/mutex/atomic/condition_variable）
- 系统复习EMC++全部42条款

---

## 📋 每日安排

| Day | 主题 | 数据结构 | C++11特性 | EMC++条款 | LeetCode |
|-----|------|---------|-----------|----------|----------|
| 29 | 二叉树入门 | 二叉树数据结构 | std::thread | 35 | 144, 145 |
| 30 | 树遍历 | 二叉树遍历 | mutex | 36-37 | 94, 102 |
| 31 | BST | 二叉搜索树 | 条件变量 | 38 | 98, 700 |
| 32 | DFS | DFS算法 | atomic | 39-40 | 104, 111 |
| 33 | 树路径 | 路径问题 | 线程池 | 11-16 | 257, 113 |
| 34 | 并发综合 | 进程线程模型 | 并发综合 | 35-40复习 | 236, 105 |
| 35 | 总结 | 知识体系 | 综合项目 | 全部复习 | 297, 124 |

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
- 掌握atomic的无锁编程

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
cd /home/z/my-project/download/week_05/day_29
chmod +x build_and_run.sh
./build_and_run.sh
```

---

## 📚 参考资料

1. [Hello-Algo - 二叉树](https://www.hello-algo.com/chapter_tree/)
2. [cppreference - Thread Support](https://en.cppreference.com/w/cpp/thread)
3. [Effective Modern C++ - Scott Meyers](https://www.aristeia.com/EMC++.html)
4. [C++ Concurrency in Action - Anthony Williams](https://www.manning.com/books/c-plus-plus-concurrency-in-action)

---

> 恭喜你即将完成35天的C++学习之旅！坚持就是胜利！
