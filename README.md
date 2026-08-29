# C++ 35 天系统学习教程

这套教程面向 C++ 初学者，保留五条相互配合的学习内容：基础语法、现代 C++、数据结构与算法、Effective Modern C++、并发编程。

课程仍按 Day 1-35 学习。完全零基础的读者先阅读 [C++ 基础学习教程](tutorials/CPP基础学习教程.md) 中的入门准备部分，确认会编译运行、变量、控制流、函数、`string`、`vector`、引用和类，再进入 Day 1。

## 学习顺序

| 周次 | 核心内容 | 进入前建议掌握 |
|---|---|---|
| [Week 1](week_01/README.md) | 复杂度、数组、类型推导、双指针、窗口、二分 | 基础语法、函数、`vector`、引用 |
| [Week 2](week_02/README.md) | 链表、RAII、智能指针、Pimpl | 指针、类、构造与析构 |
| [Week 3](week_03/README.md) | 栈、队列、堆、Lambda、BFS | 标准容器、函数、对象生命周期 |
| [Week 4](week_04/README.md) | 哈希、移动语义、完美转发、字符串 | 引用、模板基础、所有权 |
| [Week 5](week_05/README.md) | 树、DFS、并发组件与综合复习 | RAII、Lambda、移动语义 |

## 每天怎么学

1. 先看当天开头的前置知识和学习目标。
2. 运行最小示例，再阅读完整机制说明。
3. 算法题先画过程、写边界和循环不变量，再看代码。
4. EMC++ 条款按“问题—错误原因—正确方案—例外”理解，不只背结论。
5. 运行当天代码和测试，完成文末自测后再进入下一天。
6. 完成当天的工程动作和五句复盘；项目设计使用固定模板，不要求一开始凭空总结。

“一天”只是课次编号。如果某天同时包含较难的算法和 C++ 机制，可以拆成两次学习，不必追赶进度。

## 编译环境

仓库统一以 C++17 作为构建基线；文档会注明特性最早来自 C++11、C++14、C++17 或 C++20。

推荐：

- GCC 11+ 或 Clang 14+
- CMake 3.16+
- 根验证需要 GNU `timeout`；macOS 可通过 coreutils 使用 `gtimeout`

检查环境：

```bash
g++ --version
cmake --version
```

运行某一天：

```bash
cd week_01/day_01
./build_and_run.sh
```

根级全量验证（精确枚举 Day 01-35 和模型综合项目，分别执行 configure、build、CTest）：

```bash
./build_all.sh
```

只验证综合项目；`--clean` 是兼容选项，因为每次验证本来就会创建全新的临时构建树：

```bash
./build_all.sh --model-only --clean
./build_all.sh --clean
```

根目录没有统一 `CMakeLists.txt`；`build_all.sh` 会先审计项目集合必须精确等于 Day 01-35，再逐一在 `mktemp` 创建的全新树中构建。它统一传入 `BUILD_TESTING=ON`，并对使用独立开关的 Day 03、24、25、26 显式传入 `BUILD_TESTS=ON`。CTest 的“发现数”必须等于实际显示 `Passed` 的数量，`DISABLED`、`SKIP_RETURN_CODE`、没有测试、日志写入失败、超时或未运行都会使脚本可靠返回非零。阶段命令运行在独立进程组中，超时后会强制清理忽略 TERM 的父子进程。临时验证树和每项目日志路径会在汇总中输出；可用 `CPP_LEARNING_VERIFY_PARENT` 指定其父目录。

`./build_all.sh --pack-only` 在发布目录所在文件系统创建唯一隐藏暂存目录，并在所有必需路径复制、压缩成功后用同文件系统 `mv` 原子替换发布包；不会删除仓库中同名的 `CPP_Learning_35Days_temp`。归档包含根 README、35 天规划、现代 C++ 深入手册、`build_all.sh`、专题教程、五周项目和模型综合项目；任一缺失或复制失败都返回非零且保留旧发布包。

## 构建教程网站

文档站使用 Sphinx、MyST Parser 和 Sphinx Book Theme，从现有 Markdown 生成静态 HTML：

```bash
python3 -m pip install -r docs/requirements.txt
./docs/build.sh
```

构建结果位于 `docs/_build/html/index.html`。详细的本地预览和 GitHub Pages 配置见 [文档站说明](docs/README.md)。

## 教程导航

| 入口 | 主要用途 |
|---|---|
| [35 天学习规划](CPP_35天科学学习规划_最终版.md) | 唯一课表、先修关系、每日流程与阶段验收 |
| [C++ 基础学习教程](tutorials/CPP基础学习教程.md) | 零基础准备和现代 C++ 核心机制的第一次系统学习 |
| [现代 C++ 特性学习地图](Modern_CPP_Features_Deep_Dive.md) | 查询标准版本、选型流程、迁移方法和仓库学习路径 |
| [Effective Modern C++ 教程](tutorials/Effective_Modern_CPP教程.md) | 42 个条款背后的推导规则、错误模式、例外与工程取舍 |
| [C++ 并发编程教程](tutorials/CPP并发编程教程.md) | 线程、同步、内存模型、线程安全组件和关闭协议 |
| [C++ 项目组织、设计与思维训练教程](tutorials/C++项目组织与设计教程.md) | 从问题卡、不变量和接口逐步进入多文件、模块和系统设计 |
| [设计模式教程](tutorials/设计模式教程.md) | 在掌握所有权和多态后学习模式的动机、代价与现代 C++ 实现 |
| [操作系统面试常考知识](tutorials/OS面试常考知识.md) | 补充进程、调度、虚拟内存和 I/O 背景，不替代 C++ 并发语义 |
| [模型推理综合项目](model_inference_project/README.md) | 把类型、所有权、并发、测试和项目组织用于完整工程 |

同一主题在多份文档出现时，不需要全部重读：专题教程负责完整机制，每日 README 负责当天差异、真实代码路径和练习，题目 README 负责单题完整证明与复杂度。文档的去重与补全规则见 [文档重构计划](35天文档重构与逐日改写方案.md)。

## 学完后的基本验收

- 能独立编译、调试一个多文件 C++ 程序。
- 能从需求写出问题卡、接口卡、模块依赖和测试边界，而不是直接堆代码。
- 能说明常见容器和算法的复杂度及边界。
- 能使用 RAII 和智能指针表达资源所有权。
- 能解释 `auto`、引用、移动和转发的基本规则。
- 能完成数组、链表、栈队列、哈希表和树的典型题。
- 能说明数据竞争、死锁、条件变量和原子操作的适用范围。

源文档位于根目录和 `week_xx/day_xx/`；`docs/content/` 是网站同步副本，不直接手工修改。
