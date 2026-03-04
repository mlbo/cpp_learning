# 第三周：栈队列 + Lambda表达式

## 📋 本周概览

| Day | 主题 | 数据结构 | C++11特性 | EMC++条款 | LeetCode |
|-----|------|---------|-----------|----------|----------|
| 15 | 栈入门 | 栈数据结构 | Lambda入门 | 31 | 20, 1047 |
| 16 | 队列入门 | 队列数据结构 | Lambda进阶 | 32-33 | 232, 225 |
| 17 | 单调栈 | 单调栈算法 | function/bind | 34 | 739, 496 |
| 18 | 函数调用栈 | 函数调用栈 | enum class | 10 | 84, 42 |
| 19 | 优先队列 | 堆/优先队列 | - | - | 215, 347 |
| 20 | BFS基础 | BFS算法 | - | - | 102, 107 |
| 21 | 周复习 | 栈队列综合 | Lambda综合 | 31-34复习 | 155, 150 |

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
# 编译运行某天的代码
cd day_15
./build_and_run.sh

# 或手动编译
mkdir build && cd build
cmake ..
make
./day15_main
```

## 📚 核心知识点

### 栈 Stack
- **特点**：LIFO（后进先出）
- **操作**：push, pop, top, empty, size
- **应用**：括号匹配、表达式求值、函数调用、单调栈

### 队列 Queue
- **特点**：FIFO（先进先出）
- **操作**：push, pop, front, back, empty, size
- **应用**：BFS、任务调度、消息队列

### 单调栈
- **单调递增栈**：找下一个更大元素
- **单调递减栈**：找下一个更小元素
- **时间复杂度**：O(n)

### Lambda表达式
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

### BFS广度优先搜索
- **数据结构**：队列
- **应用**：最短路径、层级遍历
- **模板**：queue + visited

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

## 🔗 参考资料

- [Hello-Algo - 栈和队列](https://www.hello-algo.com/chapter_stack_and_queue/)
- [cppreference - stack](https://en.cppreference.com/w/cpp/container/stack)
- [cppreference - queue](https://en.cppreference.com/w/cpp/container/queue)
- [Effective Modern C++ - Item 31-34](https://www.aristeia.com/EMC++.html)
