# Week 4 检查报告

## 检查时间
2026-07-26（Week 4 最小范围终审返工）

## 检查范围
第四周 Day 22-28 所有文档、代码、CMake target 与测试；本轮写入范围仅限 `week_04/**`

---

## 一、文件命名规范检查

### 规范要求
| 类型 | 命名规则 | 示例 |
|------|---------|------|
| 数据结构 | `{结构名}_demo.cpp` | `hash_table_demo.cpp` |
| C++11特性 | `{特性名}_demo.cpp` | `rvalue_reference_demo.cpp` |
| EMC++条款 | `item{条款号}_{主题}.cpp` | `item09_type_alias.cpp` |
| LeetCode | `{题号}_{题目名}/solution.cpp` | `0242_valid_anagram/solution.cpp` |

### 检查结果

| Day | 状态 | 问题 |
|-----|------|------|
| Day 22 | ✅ 通过 | 无 |
| Day 23 | ✅ 已修复 | 删除了多余的 `two_sum.cpp` 和 `four_sum_ii.cpp` |
| Day 24 | ✅ 已修复 | `item28_folding_rules.cpp` 的文件名与 Item 28 引用折叠语义一致 |
| Day 25 | ✅ 已修复 | Item 29/30 映射正确；`std_forward_demo.cpp` 的 `std::move` 用途、返回类型与 moved-from 契约已统一 |
| Day 26 | ✅ 已修复 | 对齐示例区分 `alignof`、`sizeof` 与缓存行边界，不再断言错误对象大小 |
| Day 27 | ✅ 已修复 | LC 76/567 可组合；主 README 的 LC567 任意字节完整函数与三组手工轨迹均进入文档门禁 |
| Day 28 | ✅ 已修复 | 泛型教学哈希表公开为头文件并补强异常保证；LRU/LFU 文档继续链接真实实现，不维护漂移副本 |

---

## 二、README.md 文档结构检查

### 必需章节（按模板）
- 📅 学习目标
- 📖 知识点（一/二/三）
- 🎯 LeetCode 刷题
- 🚀 运行代码
- 📚 相关术语
- 💡 学习提示
- 🔗 参考资料

### 检查结果

| Day | 学习目标 | 知识点 | LeetCode | 术语 | 提示 | 参考 |
|-----|---------|--------|----------|------|------|------|
| 22 | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
| 23 | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
| 24 | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
| 25 | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
| 26 | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
| 27 | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
| 28 | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |

---

## 三、代码文件完整性检查

### Day 22: 哈希表入门
- [x] `main.cpp`
- [x] `data_structure/hash_table_demo.cpp`
- [x] `cpp11_features/rvalue_reference_demo.cpp`
- [x] `emcpp/item09_type_alias.cpp`
- [x] `leetcode/0242_valid_anagram/solution.cpp`
- [x] `leetcode/0383_ransom_note/solution.cpp`

### Day 23: 移动语义
- [x] `main.cpp`
- [x] `cpp11_features/move_semantics_demo.cpp`
- [x] `cpp11_features/move_constructor_demo.cpp`
- [x] `emcpp/item23_std_move.cpp`
- [x] `emcpp/item24_universal_reference.cpp`
- [x] `emcpp/item25_std_forward.cpp`
- [x] `leetcode/0001_two_sum/solution.cpp`
- [x] `leetcode/0454_4sum_ii/solution.cpp`

### Day 24: 通用引用
- [x] `main.cpp`
- [x] `cpp11_features/universal_reference_demo.cpp`
- [x] `cpp11_features/reference_collapsing_demo.cpp`
- [x] `emcpp/item26_avoid_overloading.cpp`
- [x] `emcpp/item28_folding_rules.cpp`
- [x] `emcpp/item28_perfect_forward.cpp`
- [x] `leetcode/0049_group_anagrams/solution.cpp`
- [x] `leetcode/0128_longest_consecutive/solution.cpp`

### Day 25: 完美转发
- [x] `main.cpp`
- [x] `cpp11_features/perfect_forwarding_demo.cpp`
- [x] `cpp11_features/std_forward_demo.cpp`
- [x] `emcpp/item29_move_assumptions.cpp`
- [x] `emcpp/item30_perfect_forward_fail.cpp`
- [x] `leetcode/0003_longest_substring/solution.cpp`
- [x] `leetcode/0438_find_anagrams/solution.cpp`

### Day 26: CPU缓存
- [x] `main.cpp`
- [x] `data_structure/cpu_cache_demo.cpp`
- [x] `data_structure/memory_alignment_demo.cpp`
- [x] `data_structure/cache_friendly_demo.cpp`
- [x] `leetcode/0005_longest_palindrome/solution.cpp`
- [x] `leetcode/0647_palindromic_substrings/solution.cpp`

### Day 27: 字符串专题
- [x] `main.cpp`
- [x] `data_structure/string_operations_demo.cpp`
- [x] `data_structure/sliding_window_demo.cpp`
- [x] `data_structure/string_matching_demo.cpp`
- [x] `leetcode/0076_minimum_window/solution.cpp`
- [x] `leetcode/0567_permutation_in_string/solution.cpp`
- [x] `tests/day27_solution_composition.cpp`

### Day 28: 周复习
- [x] `main.cpp`
- [x] `data_structure/hash_table_review.cpp`
- [x] `data_structure/simple_hash_table.h`
- [x] `cpp11_features/move_semantics_review.cpp`
- [x] `emcpp/item09_23_30_review.cpp`
- [x] `leetcode/0146_lru_cache/solution.cpp`
- [x] `leetcode/0460_lfu_cache/solution.cpp`
- [x] `tests/day28_hash_table_exception_tests.cpp`

---

## 四、构建配置检查

| Day | CMakeLists.txt | build_and_run.sh | 编译测试 |
|-----|----------------|------------------|---------|
| 22 | ✅ | ✅ | ✅ 主程序只链接课程库，严格构建与真实 CTest 通过 |
| 23 | ✅ | ✅ | ✅ 严格构建、CLI 帮助契约及 README 完整程序通过 |
| 24 | ✅ | ✅ | ✅ 语言契约测试包含公开头并链接真实教学库 |
| 25 | ✅ | ✅ | ✅ 主程序链接真实库，测试调用真实转发与 LC438 模块，脚本执行 CTest 与演示 |
| 26 | ✅ | ✅ | ✅ 主程序链接真实库，脚本执行 Release 构建、CTest 与演示 |
| 27 | ✅ | ✅ | ✅ 严格构建及组合测试通过 |
| 28 | ✅ | ✅ | ✅ 严格构建、抛异常 Hash 强保证、真实转发、LFU 长序列及文档契约通过 |

---

## 五、内容规划符合性检查

### 规划对照表

| Day | 规划数据结构 | 实际数据结构 | 规划C++11 | 实际C++11 | 规划EMC++ | 实际EMC++ | 规划LC | 实际LC |
|-----|------------|-------------|----------|----------|----------|----------|--------|--------|
| 22 | 哈希表 | ✅ 哈希表 | 右值引用 | ✅ 右值引用 | Item 9 | ✅ Item 9 | 242,383 | ✅ 242,383 |
| 23 | - | - | 移动语义 | ✅ 移动语义 | 23-25 | ✅ 23-25 | 1,454 | ✅ 1,454 |
| 24 | - | - | 通用引用 | ✅ 通用引用 | 26-28 | ✅ 26-28 | 49,128 | ✅ 49,128 |
| 25 | - | - | 完美转发 | ✅ 完美转发 | 29-30 | ✅ 29-30 | 3,438 | ✅ 3,438 |
| 26 | CPU缓存 | ✅ CPU缓存 | - | - | - | - | 5,647 | ✅ 5,647 |
| 27 | 字符串 | ✅ 字符串 | - | - | - | - | 76,567 | ✅ 76,567 |
| 28 | 哈希表复习 | ✅ 复习 | 移动复习 | ✅ 复习 | 9,23-30 | ✅ 复习 | 146,460 | ✅ 146,460 |

---

## 六、修复操作记录

1. **删除多余文件**
   - `day_23/code/leetcode/0001_two_sum/two_sum.cpp` - 已删除
   - `day_23/code/leetcode/0454_4sum_ii/four_sum_ii.cpp` - 已删除

2. **条款文件语义对齐**
   - Day 24：`item28_folding_rules.cpp` 对应 Item 28“理解引用折叠”
   - Day 25：`item29_move_assumptions.cpp` 对应 Item 29“假定移动不存在、不便宜、未被使用”

3. **Day 27 公开接口组合边界**
   - LC 76 保留 `class Solution`，置于 `day27::lc0076`；LC 567 保留 `class Solution`，置于 `day27::lc0567`
   - 两个库 target 通过 `target_include_directories(... PUBLIC ...)` 传播公开包含根，测试只链接各自实现库
   - 删除目录级 `include_directories`，新增 `day27_solution_composition` 同时包含两个公开头并链接两个实现库

4. **Day 28 LRU/LFU 文档单一真源**
   - 删除与真实头/实现不一致的伪完整实现片段，两份文档都改为直接包含 `solution.h` 并链接 `solution.cpp` 的契约程序
   - 文档明确公开接口、所有权与淘汰不变量；LFU 另说明空桶删除、非正容量和 `std::uint64_t` 频率饱和语义
   - 契约程序覆盖 LRU 更新/淘汰，以及 LFU 测试构造重载、同频 LRU 淘汰和 `frequencyBucketCount() <= size()` 的空间边界

5. **Day 23/28 教学契约一致性补充**
   - Day 23 环境要求与真实 CMake、`std::string_view` 用法统一为 C++17，CLI 帮助从实际 `argv[0]` 输出程序名
   - 新增 `Day23Help` CTest，并用输出正则锁定 `day_23_main [选项]`，防止 target 与帮助文本再次漂移
   - Day 28 的值类别图和术语表统一说明“值类别是表达式属性”，右值包含纯右值与将亡值
   - Day 23/28 的建议统一为“只有确实不抛异常时才标记 `noexcept`”，避免把优化条件误写成无条件要求

6. **Week 4 教程内部一致性终审**
   - Day 23 的 `move_constructor_demo` 改为条件化解释 `noexcept`：移动可能抛且拷贝可用时容器可能复制，拷贝不可用时仍可能使用可抛移动
   - Day 28 源码输出统一为“值类别是表达式属性”，`std::move` 产生 xvalue 但不执行移动，moved-from 对象只承诺满足类型契约
   - Day 28 区分固定 `int&&` 与转发引用，并让 generic lambda 真实调用 `int&`/`int&&` 目标重载；`day28_main_contract` 以退出码验证两条路线
   - Day 25 Item 29 文件头重新反映“不存在、不便宜、未被使用”的主体，保留的 Item 25 前置回顾也改为返回真实目标重载结果
   - Day 22-28 中 `std::move` 的同源旧措辞统一为“无条件产生 xvalue，本身不搬运资源”，并把“有名字所以是左值”“右值引用只接临时量”等速记改为表达式规则
   - Day 27 的字符串说明不再把移动写成必然只转移所有权，明确复制消除、表示形式、可用重载与异常保证都会影响实际路径；未改变课程主题与讲解量
   - Day 23 Item 25 的错误示例明确区分“左值被无条件交给 xvalue/右值路径”与“目标是否真的移动资源”；Day 28 主总结和术语表同步改为按类型契约复用或转移资源

7. **真实模块边界与脚本闭环**
   - Day 22/25/26 的主程序不再把已经组成静态库的实现源重复加入 executable，而是只编译 `main.cpp` 并链接真实模块库
   - Day 25 新增最小公开转发契约头，测试直接调用模块的 `forwardingRoute` 与真实目标重载，不再在测试中复制 `relay/target` 替身
   - Day 24 以 `universal_reference_demo.h` 导出重载与转发路由，教材演示和契约测试共用同一实现，测试不再重新定义关键逻辑
   - Day 23-28 的脚本统一配置 Release；Day 25/26 补齐 `配置 -> 构建 -> CTest -> 演示`，Day 23 非交互执行 `--all`，Day 25-27 共用 `ENABLE_SANITIZERS=ON` 入口

8. **文档代码与单一真源**
   - Day 22 补清局部直接绑定临时量的生命周期延长范围及不会跨返回/继续传递传播的边界
   - Day 25 Item 30 的重载函数与函数模板成功路径都先固定为 `int(*)(int)`，再进入同一个真实目标接口并分别得到 42 与 22
   - Day 27 明确 SSO 是字符内联于对象表示而非等同栈存储，返回字符串只说明复制消除/移动/复制的可能路径
   - Day 28 以 Rule of Zero 为默认选择；LRU/LFU 文档都只保留链接真实 `solution.cpp` 的公开接口契约程序
   - Day 28 对齐契约不再把 16 字节类型对齐、对象大小和 64 字节硬件缓存行混为一谈

9. **完整教材程序可验证性**
   - Day 23 Rule of Zero/Five 完整程序补齐它自身使用的标准库头，并真实实例化 Rule of Zero 类，不再依赖偶然的间接包含
   - 新增 `verify_readme_programs.sh`，自动提取 README 中含 `int main` 的 C++ 代码块，锁定每日数量并用严格 C++17 编译运行
   - LRU、LFU 与 LC438 契约程序链接真实 `solution.cpp`；当前 Day 22/23/24/25/28 分别覆盖 4/1/2/3/3 个，合计 13 个

10. **LC438 公开契约闭环**
   - README、`solution.h`、`solution.cpp` 与真实模块测试统一规定 `s`/`p` 只接受 `'a'`-`'z'`，非法字节抛 `std::invalid_argument`，空模式返回空结果
   - 输入域校验先于长度早退，不会因 `s.size() < p.size()` 隐藏非法输入；超出 `int` 下标表示范围时抛 `std::overflow_error`
   - 文档说明若需支持任意字节或 Unicode，应改变计数结构和接口契约，不能继续直接以字节减 `'a'` 索引

11. **窄范围教学与验证边界收口**
   - Day 25 的失败对比改为真实机制：`T&` 包装器只接左值；被推导的精确 `T&&` 是转发引用，能接左、右值，但不用 `std::forward` 时两次均到左值重载；测试直接调用课程模块并断言目标重载返回值
   - Day 23/24/28 的正文、Mermaid、术语表和演示统一使用“函数模板调用正在推导、未加 cv 的模板参数 `T` 之精确 `T&&`”，并显式列出 `auto&&` 直接列表初始化例外
   - Day 22 区分扩容重建 `rehash` 与开放寻址的 `double hashing`，Day 22/23 主讲契约及 Day 24/28、周内哈希专题的相关说明均写明平均 O(1)/最坏可退化；Day 26 的 miss 流程先查询 L2/最后级缓存，不再从 L1 miss 直接跳到 DRAM
   - Day 28 区分移动操作的隐式声明与实际可用性：基类或成员需要的操作不可用时，默认化移动操作可被定义为删除
   - Day 25/26 的默认 CMake 现在直接启用 `-Wall -Wextra -Wpedantic`，不再依赖外部验收命令才获得基础告警门禁
   - 可搬迁边界明确为整棵 `week_04`：Day 22-28 的不完整单日复制都在 CMake 配置阶段给出明确失败，复制整周后七个脚本均可运行
   - README 程序验证器为每个程序增加默认 10 秒内部超时，CTest 再提供 120 秒外层超时；无可用 `timeout` 工具时门禁拒绝降级运行

12. **教学语义与生命周期最终窄修复**
   - Day 23 Item 24 的 generic lambda 对命名形参使用 `std::forward<decltype(param)>` 后再分类，测试共享同一 `ForwardedCategory` 结果，右值调用不再被二次推导成左值
   - Day 23 Item 25 将右值限定成员提取与转发参数拥有结果都改为按值返回；原 `T&&` 借用模式保留为明确失败示例，两个独立 ASan 探针都以非零退出；文档命令本轮报告 `stack-use-after-scope`，并说明不同优化/字符串表示也可能显示为 `heap-use-after-free`
   - Day 23 Item 23 真实执行字符串移动，只报告目标内容和源对象“有效但状态未指定”的观察值，不再把实现偶然为空写成保证
   - Day 28 的名称模板只在参数可构造 `std::string` 且不是类自身时参与；真实契约证明 `int` 与 `short` 都走索引重载，字符串类型才走名称路线
   - Day 24 LC128 README 与实现统一在 `INT_MIN` 前停止 `num - 1`、在 `INT_MAX` 前停止 `current + 1`；Day 23 LC1/LC454 也以宽整数计算补数与二元和，并由真实库契约覆盖极值
   - Day 26 删除“每核私有 L2、全芯片共享 L3”的绝对拓扑；Day 27 通用窗口块明确标为结构伪代码；Day 25/26 算法片段补齐整数、字节与返回类型前提
   - Item 9 的真实优势固定为 alias template 与 `_t` 风格减少类模板嵌套 `type`；Item 30 完整覆盖大括号、0/NULL、静态成员 ODR-use、重载函数名/函数模板名和位域
   - 七天统一最低 CMake 3.14，并通过共享 helper 分拆 ASan/UBSan；Day 28 复习图和表明确 LRU/LFU 的哈希平均复杂度前提，以及 Item 29“移动成本假设”与 Item 30“转发失败边界”的映射

13. **最终自主诊断与边界收口**
   - Day 22 LC242/383 与 Day 24 LC49 的固定 26 项数组此前把输入域当作隐含前提，非小写字节可形成越界下标；现在三层实现、公开头、测试与 README 统一限定 `'a'`-`'z'`，先校验再做长度早退，非法输入抛 `std::invalid_argument`
   - Day 22/23/28 的资源特殊成员不再执行观察流输出：避免构造完成前因日志异常泄漏裸资源，也避免复制赋值已提交却向调用方抛出；契约测试在可控抛异常输出缓冲下真实执行构造、复制、赋值与移动
   - Day 25 的教学移动不再虚假标记 `noexcept`；`std_forward_demo` 明确 `std::move` 只产生 xvalue，不承诺所有权转移，并把返回类型写成 `remove_reference_t<T>&&`、立即说明标准字符串 moved-from 状态未指定
   - Day 25 `makeDeferredTask` 以 `std::decay_t<Func>` 拥有左值 callable，测试链接真实教学库并证明延迟任务不保存悬空引用；`abcabcbb`、`babad` 与 LC76 三组轨迹均按真实算法逐步重算
   - Day 26 将对象布局数字明确为常见 ABI 下的示例，并修正 `AlignDemo::c` 后填充说明，不再把演示结果误写成语言保证
   - Day 27 的 LC76 空目标演示先按公开契约返回；LC567 主 README 改为有完整返回路径的 256 项无符号字节函数，并作为关键非 main 片段严格独立编译；LC209 明确并验证正数前置条件
   - Day 28 LFU 在淘汰旧项前先取得所有可能失败的资源，异常时清理临时节点与空桶；已有键先完成可能失败的升频再提交新值，使失败路径保持容量、所有权和桶不变量
   - 共享 CMake helper 现在由七天全部调用，Day 26/27 也注册 README 数量门禁；真正单独复制任一 Day 时均在配置阶段说明必须复制完整周树

14. **Day 28 泛型哈希表异常保证闭环**
   - 审计探针在旧实现上复现 `threw=1 key0=-1 key4=400`：rehash 每算出一个新桶位置就立即改写旧节点 `next`，第二次 Hash 抛出后旧桶已无法到达 key 0
   - `SimpleHashTable<K,V,Hash>` 从 `.cpp` 移入自足公开头，调用方现在可以实例化状态化、自定义且会抛异常的 Hash；Day 28 的公开头独立/组合门禁同步覆盖该模板
   - rehash 先分配新桶与 O(n) 迁移计划，完成全部 Hash 调用后才进入只写指针的 commit；`insert` 还先构造新节点，已有键更新使用完整替代节点，避免可能部分改写的 `V::operator=` 破坏旧值
   - 契约限定为表内键值、`size`、桶数和链拓扑的强保证；用户 Hash/相等比较自身的外部副作用不回滚，Hash 仍须稳定且对相等键给出相同结果，相等比较不得改写已存键
   - 新增 `day28_hash_table_exception_contracts` CTest，真实覆盖 rehash 中途 Hash 抛出、值复制抛出、失败后继续扩容和成功更新；修复后同一探针输出 `threw=1 key0=0 key4=400`

15. **WEEK4_FINAL_REPAIR_V2 独立复核**
   - 修复前重新运行 Day 25 演示，真实看到“移动对象的所有权”“总是 Type&&”以及标准字符串移动后直接打印偶然空值；修复后输出同时给出 xvalue 机制、精确返回类型、状态未指定边界与重新赋值复用
   - Day 27 主 README 旧 LC567 片段在严格编译下触发 `size_t` 到 `int`、有符号下标等错误，函数体也没有成功/失败的完整算法；最终片段与公开实现同用 256 项无符号字节计数，并由现有 README CTest 编译为独立对象
   - 真实算法重算得到 `abcabcbb` 最后窗口 `[7,7]`、最后位置表 `{a:3,b:7,c:5}`；`babad` 索引2中心得到 `aba` 后在 `b != d` 停止；LC76 三次有效收缩最终由 `ADOBEC` 更新到 `EBANC`、`BANC`
   - LC454 在 `n=100` 时两个分组阶段分别枚举 10,000 个数对，合计约 20,000；与 100,000,000 个暴力四元组的枚举数量比约 5,000，而不是遗漏第二阶段后声称 10,000 倍
   - `verify_readme_programs.sh` 除锁定 13 个完整程序外，还锁定并严格编译 1 个关键非 main 片段，同时拒绝上述陈旧语义、数量级和三组轨迹文本回归
   - 最终复跑发现完整程序路径此前少于关键片段的 `-Wconversion -Wsign-conversion -Wshadow`；统一全套告警后真实捕获 Day 22 类型别名示例的局部变量遮蔽，改用语义化名称后 13 个完整程序与 1 个关键片段全部通过

---

## 七、终审后一致性审计验证证据

- **严格 C++17 Release 构建**：Day 22-28 在全新临时目录使用 `CMAKE_CXX_EXTENSIONS=OFF`、`-Werror` 和 `-pedantic-errors`，7/7 全部成功
- **CTest**：七天合计 28/28 通过；各日依次为 3/4/3/3/3/5/7 项，算法契约、README、公开头、综合演示、哈希表异常路径与 LRU/LFU 失败路径全部由真实 target 覆盖
- **ASan 覆盖**：独立 ASan 构建执行 `ctest -LE strict-no-sanitizer`，21/21 个真正链接仪器化 target 的测试通过
- **UBSan 覆盖**：独立 UBSan 构建执行同一筛选，21/21 通过；Day 23 的 LC1/LC454 极值整数路径与 Day 28 哈希表异常路径都由真实课程库契约覆盖
- **生命周期负例**：按 README 的 `RelWithDebInfo` 命令运行 `day23_lifetime_boundary_asan_demo member` 与 `forward`，均以非零退出且本轮两次报告 `stack-use-after-scope`；Debug 下两条路径也都被捕获并显示 `heap-use-after-free`，证明诊断类别会受对象表示与优化影响，负例 2/2 不进入默认 CTest
- **Sanitizer 统计边界**：7 个带 `strict-no-sanitizer` 标签的 README CTest 另起严格编译器，不计入上述 21 个仪器化测试；13 个完整程序与 1 个非 main 片段单独报告为严格非 Sanitizer 文档门禁
- **专用边界探针**：Day 22/24 非法输入在 ASan 下由 `std::invalid_argument` 拒绝而非越界；Day 27 LC567 13/13 用例通过；Day 25 左值 callable 与 moved-from 复用契约通过；Day 22/23/28 在抛异常流缓冲下完成资源操作
- **LFU 可复现证据边界**：注册的 `day28_lfu_cache` 覆盖容量、更新、同频 LRU、长升频和饱和边界；报告不再把未提交的临时全局分配替换探针计作永久门禁证据
- **真实转发证据**：Day 23 Item 24、Item 25，Day 24 重载路由，Day 25 Item 29/30 和 Day 28 复习契约都返回可断言事件，并由测试链接真实课程库；不再用自报文本证明值类别或失败边界
- **负向文本搜索**：教材正文与源码（不含本报告对旧反例的引用）未再发现把 `std::move` 写成所有权转移、把其返回类型写成“总是 Type&&”、把标准库 moved-from 状态保证为空、把固定右值引用泛称为转发引用或把 LC567 不完整 26 项片段冒充真实实现的表述
- **LFU 长序列**：Day 28 CTest 覆盖容量 1 下连续 10000 次升频，最终只保留一个非空频率桶
- **公开头边界**：21 个公开头分别在独立翻译单元严格编译；全部公开头在同一翻译单元组合编译并由 CTest 运行通过；Day 27 仍保留两算法真实链接组合测试
- **文档代码验证**：`verify_readme_programs.sh` 对 13 个含 `int main` 的完整程序和 Day 27 LC567 关键非 main 函数统一使用全套转换、符号转换与遮蔽告警；完整程序均编译运行，关键函数独立编译为对象，LRU、LFU、LC438 直接链接真实实现
- **算法与轨迹契约**：LC242/383/49 覆盖非法字节；LC128/1/454 覆盖极值与宽整数；LC454 数量级、`abcabcbb` 最后窗口、`babad` 中心扩展和 LC76 收缩序列均有文本门禁；LC438、LC76、LC567 覆盖输入域、空模式、组合与差分路径
- **一键脚本**：当前整棵 `week_04` 复制到临时目录后，Day 22-28 的 `build_and_run.sh` 全部完成清理、Release 配置、构建、CTest 与演示，7/7 退出码为 0
- **可搬迁失败方式**：真正只复制 Day 22-28 中任一天时，七个项目都以非零配置结果明确说明需要整棵 `week_04`，7/7 通过失败方式门禁
- **结构不变量**：七天均恰好一个“今日工程动作”和恰好五句复盘；主知识点与两道算法题结构保留；公开头清单恰好 21，README 完整程序恰好 13
- **门禁超时**：13 个 README 程序每个默认限时 10 秒；`README_PROGRAM_TIMEOUT_SECONDS=0` 以退出码 2 被拒绝，防止无效配置关闭保护
- **无效参数**：README 验证器不支持的 Day 以退出码 2 拒绝，Day 23 未知 CLI 选项以退出码 1 拒绝并提示 `--help`
- **范围与格式**：`git diff --check` 通过；本轮未修改其他 Week、综合项目或 `docs/content`，Day 22-28 的原主题、工程动作和恰好五句复盘保持不变

---

## 八、总结

### 检查结果：✅ 全部通过

- **文件命名**: 符合规范
- **文档结构**: 完整
- **代码文件**: 齐全
- **构建配置**: 完整
- **内容规划**: 符合要求
- **编译测试**: 严格 C++17 Release `-Werror -pedantic-errors` 构建与 28/28 CTest 通过
- **接口组合**: Week 4 的 21 个公开头均可独立编译且全组合无冲突
- **文档一致性**: 13 个 README 完整程序和 1 个关键非 main 片段自动验证；三组手工轨迹、Day 25 移动语义与 LC454 数量级另有漂移拒绝门禁

### 统计数据

- 严格构建: 7/7 个 Day
- CTest: 28/28
- ASan 仪器化 target 测试: 21/21
- UBSan 仪器化 target 测试: 21/21
- ASan 生命周期负例: 2/2 捕获
- README 严格非 Sanitizer CTest: 7/7，共 13/13 个完整程序
- 关键非 main 教材片段: 1/1 严格独立编译
- 公开头: 21 个独立编译 + 1 个全组合运行契约
- LeetCode题解: 14 道

### 备注

Week 4 终审发现的模块重复编译、测试替身、脚本断链、文档代码漂移、对齐概念混淆及移动/转发教学契约冲突均已修复，当前通过上述可重复验证。
工作树在本轮开始时已有范围外未提交改动；本轮未操作或清理这些既有改动，也未创建提交。
