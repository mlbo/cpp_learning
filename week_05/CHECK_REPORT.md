# Week 5（Day 29–35）自主系统优化与最终验证报告

## 结论

检查日期：2026-07-26（Asia/Shanghai）。

本轮先检查并保留了 `week_05/**` 中已有的教程、源码、回归测试、CMake、脚本和报告修改，再从最终工作树独立复核；没有回退有价值的既有修复，没有创建提交，所有编辑严格限定在 `week_05/**`。最终树按 C++17 Release 与全告警构建通过，55/55 个实际 `.cpp` 均有 CMake 目标覆盖，51/51 个 CTest、51/51 个 ASan+UBSan 测试和 20/20 个 TSan 并发测试通过。

这里的“通过”只代表当前工具链、平台与实际执行路径没有发现失败；Sanitizer 和重复运行都不是形式化证明，接口契约、所有权、不变量与停止协议仍是正确性的第一层依据。

## 独立审计归纳的主要根因

1. **验证证据可能陈旧**：旧报告记录过完整通过，但课程仓库历史上出现过 CMake 未覆盖全部源码、脚本目标失配和 CTest 注册不足，因此不能把旧报告当作当前最终树证据。
2. **文档命令与文件状态不一致**：七个脚本已经是可执行文件，部分 README 仍要求先 `chmod +x`；Day 34 的手动构建还写到 `/tmp`，不符合本轮严格写入范围。
3. **并发直觉仍夹杂固定睡眠**：实际测试已经依赖 `join`、future、谓词和最终不变量，但 Day 29/30 的少数教程片段仍用 `sleep_for` 模拟耗时或交错，容易让初学者把时间当同步协议。
4. **学习索引与最终交付脱节**：专题指南末尾仍把已有源码和 CTest 的 LC 236、LC 124、可关闭队列等标为未完成，不能准确反映 Day 29–35 的学习顺序与工程证据。

## 本轮局部修复

- 保留现有 Day 29–35 主题、顺序、知识量、源码和契约测试，只做局部文档一致性修复。
- Week 总入口和 Day 30、31、35 改为直接执行已提交为 `755` 的 `./build_and_run.sh`，删除多余的 `chmod` 步骤。
- Day 31、32 增加明确的“今日工程动作”标题；机械检查确认七天各恰好一个工程动作标题、复盘各恰好五句。
- Day 29 的 thread/task 教程移除固定睡眠，明确只保证单条输出互斥和 `join_all()` 后全部完成，不保证线程间输出顺序。
- Day 30 用两个被保存并 `get()` 的 future 与结果不变量解释句柄生命周期，替代用五秒睡眠观察临时 future 析构。
- Day 34 的手动命令改用本日 `build-manual/`，避免超出 `week_05/**` 写入范围。
- 专题指南的完成清单与最终树对齐：树题、可关闭队列、生产者消费者和 reject-drain-join 线程池标为已覆盖，读写锁保留为进阶扩展。

## 最终树中的关键契约

- 树工程用 `unique_ptr` 表达父节点独占子树，算法参数只借用节点；LeetCode 原始指针接口由测试或 arena 负责外层所有权。
- BST 明确拒绝重复值，验证使用沿祖先传播的开区间；普通 BST 的操作复杂度写成 O(h)，有序插入退化到 O(n)。
- Day 30 转账在持有两把锁时检查正金额、来源余额和目标容量，拒绝 `destination > INT_MAX - amount`，失败时双方余额零修改。
- 条件变量等待 `closed || !queue.empty()`，关闭后拒绝新值、排空已接收值并唤醒全部等待者。
- `JoiningThreadGroup` 保证批量创建线程中途失败时仍拥有并回收已创建线程，并拒绝 worker 对自身执行公开 `join_all()`。
- 线程池把提交检查与停止检查放在同一把锁的线性化点上，采用外部 owner 主导的 reject-drain-join；任务结果和异常通过 future 传播，停止拒绝不消费调用者的 move-only 参数。
- Codec 拒绝空、截断、多余、部分整数、越界整数、过深和 token 超预算输入；最大路径和用受检 `int64_t` 加法报告溢出而不是执行有符号溢出 UB。

## 构建与测试环境

- 操作系统：Linux 5.14.0 x86_64
- 编译器：GCC 11.3.0
- CMake / CTest：3.27.7
- 语言标准：C++17，关闭编译器扩展
- 严格告警：`-Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow -Werror`

## 逐日验证结果

| Day | 主题 | `.cpp` / CMake | Release CTest | ASan+UBSan | TSan 并发子集 |
|---:|---|---:|---:|---:|---:|
| 29 | 二叉树、thread、task/future、Item 35 | 9/9 | 7/7 | 7/7 | 4/4 |
| 30 | 遍历、mutex、async、Item 36–37 | 8/8 | 6/6 | 6/6 | 3/3 |
| 31 | BST、条件变量、Item 38 | 6/6 | 6/6 | 6/6 | 2/2 |
| 32 | DFS、atomic、Item 39–40 | 8/8 | 8/8 | 8/8 | 2/2 |
| 33 | 路径回溯、线程池、Item 11–16 | 8/8 | 8/8 | 8/8 | 4/4 |
| 34 | LCA/重建、并发机制综合 | 8/8 | 8/8 | 8/8 | 4/4 |
| 35 | 序列化、最大路径和、阶段总结 | 8/8 | 8/8 | 8/8 | 1/1 |
| **合计** |  | **55/55** | **51/51** | **51/51** | **20/20** |

Release 验证通过 Week README 中的七天循环命令原样执行，每个脚本都会删除本日旧 `build/`、重新配置、构建全部目标并运行 CTest。Day 29、Day 30 与 Day 34 README 中的手动 CMake 命令也分别原样得到 7/7、6/6 与 8/8。

## Sanitizer 与稳定性

ASan+UBSan 使用 `-O1 -g -fno-omit-frame-pointer -fsanitize=address,undefined`，并启用 leak detection、错误立即停止和 UBSan 栈信息；七天 51 项 CTest 全部通过。

TSan 使用 `-O1 -g -fno-omit-frame-pointer -fsanitize=thread`，只运行实际创建线程或建立跨线程同步的 20 个稳定目标：

- Day 29：thread demo、thread group regression、Item 35、integration main
- Day 30：mutex transfer、Item 36–37、integration main
- Day 31：condition variable、Item 38
- Day 32：atomic、Item 39–40
- Day 33：thread pool demo、两个线程池回归、Item 11–16
- Day 34：project action、process/thread、concurrency、EMC++ review
- Day 35：C++17 summary

上述 20/20 均通过。另将 thread group、转账、可关闭队列、atomic 发布、线程池三个关键目标、线程 ID 门闩和 Day 35 并发总结分别连续运行 100 次，九组均为 100/100；这些程序只断言 future、谓词、join 和最终状态，不断言固定耗时或线程输出顺序。

## 教学材料、脚本与覆盖检查

- `week_05` 共 10 份 Markdown、87 个 C++ 围栏：42 个完整块按 C++17 与严格告警通过语法检查，45 个局部块均在邻近文字明确标注省略前提、上下文或不可单独编译。
- Markdown 围栏全部成对闭合，本地相对链接全部解析成功；扫描时排除了 fenced code，避免把代码表达式误判为链接。
- 12 个头文件均作为翻译单元首个包含项独立通过严格语法检查，不依赖偶然的传递包含。
- 55 个实际 `.cpp` 全部出现在对应日 CMake 中；CTest 注册数逐日为 7、6、6、8、8、8、8。
- 七个 `build_and_run.sh` 均通过 `bash -n`，文件模式均为 `755`，Week README 的七天循环和三组手动命令原样通过。
- 七天每日 README 均恰好一个工程动作标题，复盘均严格为编号 1–5 的五句。

## 明确限制

- TSan 通过只代表当前 GCC 11.3/Linux x86_64 与已采样交错，不证明所有平台和所有调度都无竞态。
- ASan、UBSan 与重复运行只能检查已执行路径；递归资源上限、生产级背压、取消、优先级和可观测性仍需在真实项目中继续设计。
- 教学线程池刻意保持小型，已覆盖结果、异常、参数所有权和 reject-drain-join，但不宣称具备生产级线程池的全部能力。

## 范围与最终检查

- 本轮所有编辑均位于 `week_05/**`；仓库其他目录的既有脏改动被保留，未提交、未清理、未回退。
- 所有本轮构建目录都位于 `week_05/day_*/`，没有用 README 命令向范围外写入。
- `git diff --check -- week_05` 与最终全仓 `git diff --check` 均已通过。
