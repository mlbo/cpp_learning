# Week 3 检查报告

> **历史记录边界**：本文只记录 2024 年 3 月进行的规划、命名和目录结构核对，不是当前源码的严格构建、CTest、Sanitizer、公开头自包含或外部消费者验证结论。当前交付状态必须以针对现有工作树重新执行的全新验证为准。

## 检查时间
2024年3月

## 规划要求核对

### 第三周内容安排

| Day | 主题 | 数据结构 | C++11特性 | EMC++条款 | LeetCode | 状态 |
|-----|------|---------|-----------|----------|----------|------|
| 15 | 栈入门 | 栈数据结构 | Lambda入门 | 31 | 20, 1047 | ✅ |
| 16 | 队列入门 | 队列数据结构 | Lambda进阶 | 32-33 | 232, 225 | ✅ |
| 17 | 单调栈 | 单调栈算法 | function/bind | 34 | 739, 496 | ✅ |
| 18 | 函数调用栈 | 函数调用栈 | enum class | 10 | 84, 42 | ✅ |
| 19 | 优先队列 | 堆/优先队列 | - | - | 215, 347 | ✅ |
| 20 | BFS基础 | BFS算法 | - | - | 102, 107 | ✅ |
| 21 | 周复习 | 栈队列综合 | Lambda综合 | 31-34复习 | 155, 150 | ✅ |

### 文件结构检查

每天目录结构要求：
- [x] README.md - 学习文档
- [x] CMakeLists.txt - CMake配置
- [x] build_and_run.sh - 编译运行脚本
- [x] code/ - 源代码目录
  - [x] main.cpp - 主程序入口
  - [x] data_structure/ - 数据结构代码
  - [x] cpp11_features/ - C++11特性代码
  - [x] emcpp/ - EMC++条款代码
  - [x] leetcode/ - LeetCode题目代码

### 文件命名规范检查

规划要求的命名规范：
- 数据结构：`{结构名}_demo.cpp` ✓
- C++11特性：`{特性名}_demo.cpp` ✓
- EMC++条款：`item{条款号}_{主题}.cpp` ✓
- LeetCode：`{题号}_{题目名}/solution.cpp` ✓

### 已修正的问题

1. **文件命名修正**：
   - `lambda_basics.cpp` → `lambda_demo.cpp`
   - `lambda_capture.cpp` → `lambda_capture_demo.cpp`
   - `lambda_generic.cpp` → `lambda_generic_demo.cpp`
   - `lambda_init_capture.cpp` → `lambda_init_capture_demo.cpp`
   - `monotonic_stack.cpp` → `monotonic_stack_demo.cpp`
   - `call_stack.cpp` → `call_stack_demo.cpp`

2. **头文件保护符更新**：已更新所有对应的头文件保护符

3. **include路径更新**：已更新main.cpp中的所有include语句

## 总结

截至 2024 年 3 月，本报告所列的规划、文件命名和目录结构核对已完成；它不对之后演进的 CMake 依赖图、源码覆盖、测试注册或运行结果作持续有效的保证。
