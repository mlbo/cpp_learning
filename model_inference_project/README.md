# C++ 模型推理综合项目

这是 35 天课程后的综合练习：用 C++17 实现一个小型 Tensor、常见推理层、Sequential 容器，以及带有界队列的多线程异步推理引擎。项目保留教学可读性，不以替代成熟深度学习框架为目标。

## 学习目标

- 用 shape 和所有权不变量管理动态内存。
- 在 Linear、激活与归一化层之间传递明确的输入输出契约。
- 用线程、条件变量、promise/future 表达任务的接收、完成、拒绝和停止。
- 用 CMake、CTest、严格警告和 Sanitizer 把边界条件变成可重复验证的工程约束。

## ⚠ 使用契约：析构前必须排空在途调用

`ParallelInferenceEngine` 析构时，**绝不能正处在该引擎的推理回调内部，也绝不能有任何 `inferAsync` / `inferBatchParallel` 调用仍在栈上（在途）**。违反会让进程直接 `std::terminate` 中止（与 `std::thread`/`boost::asio` 的 fail-fast 惯例一致，无法用异常恢复）。

合法的销毁顺序：`close()` 停止接收新任务 → `join()` 所有发起推理的线程 → 等异步 future 都完成 → 再析构引擎。析构会自动排空已接收但未跑完的异步任务；不能自动排空的是"回调内销毁"和"撞上栈上同步调用"这两种无法安全组合的情况。Debug 构建下会先 `assert` 打印定位信息，Release 构建直接 `terminate`。详见 `inference_engine.h` 中 `ParallelInferenceEngine` 的类注释与析构函数。

## 项目结构

```text
model_inference_project/
├── CMakeLists.txt
├── build.sh
├── README.md
├── include/
│   ├── tensor.h
│   ├── layers.h
│   └── inference_engine.h
├── src/
│   └── main.cpp
└── tests/
    ├── contract_tests.cpp
    ├── concurrency_tests.cpp
    ├── expect_exit_code.cmake
    ├── fault_injection_tests.cpp
    ├── header_tensor_self_contained.cpp
    ├── header_layers_self_contained.cpp
    └── header_inference_engine_self_contained.cpp
```

依赖只向下：`main/tests` 使用公开头，`inference_engine -> layers + tensor`，`layers -> tensor`，`tensor -> C++ 标准库`。因为推理引擎的公开签名直接使用 `Tensor`，它会直接包含 `tensor.h`，不依赖偶然的传递包含；下层模块也从不反向包含上层模块。

所有权按对象层次逐级明确：`Tensor` 用 `unique_ptr<float[]>` 独占数据，`Sequential` 用 `unique_ptr<Layer>` 独占各层，worker 任务用 `shared_ptr<Runtime>` 保证排空期间运行态存活。这里要特别区分三件事：`Runtime` 活着不等于引擎对象活着，引擎对象活着也不等于它拥有模型；推理引擎只保存 `const Sequential*`，所以调用者仍必须证明所有模型访问已经结束，才能释放模型。

## 环境基线

- C++17；推荐 GCC 11+ 或 Clang 14+
- CMake 3.16+
- 支持 pthread 的 Linux/macOS；Windows 可使用提供 C++17 与 Threads 支持的工具链

代码使用折叠表达式、`std::decay_t` 和数字分隔符等 C++14/C++17 能力，并统一按 C++17 构建，不宣称兼容 GCC 4.8 或仅 C++11 的工具链。

## 构建与测试

```bash
cd model_inference_project
./build.sh
./build/inference_demo
```

`build.sh` 会分别创建全新的 `build/`、`build-asan/`、`build-tsan/`，依次完成严格 C++17、压力与故障注入测试、ASan/UBSan、TSan。任一 configure、build 或 CTest 失败都会立即返回非零。普通构建的手动等价流程：

在 GCC/Clang 下，`MODEL_INFERENCE_STRICT=ON` 的项目入口固定包含 `-Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow -Werror`；不是“外部临时加严后才通过”。

```bash
cmake -S . -B build \
  -DCMAKE_BUILD_TYPE=Release \
  -DBUILD_TESTING=ON \
  -DMODEL_INFERENCE_STRICT=ON
cmake --build build --parallel
ctest --test-dir build --output-on-failure --timeout 15
```

ASan/UBSan 验证：

```bash
cmake -S . -B build-sanitize \
  -DCMAKE_BUILD_TYPE=Debug \
  -DBUILD_TESTING=ON \
  -DMODEL_INFERENCE_STRICT=ON \
  -DMODEL_INFERENCE_SANITIZERS=ON
cmake --build build-sanitize --parallel
ctest --test-dir build-sanitize --output-on-failure --timeout 15
```

TSan 验证必须使用独立构建目录，不能与 ASan/UBSan 混用：

```bash
cmake -S . -B build-tsan \
  -DCMAKE_BUILD_TYPE=Debug \
  -DBUILD_TESTING=ON \
  -DMODEL_INFERENCE_STRICT=ON \
  -DMODEL_INFERENCE_TSAN=ON
cmake --build build-tsan --parallel
TSAN_OPTIONS=halt_on_error=1 \
  ctest --test-dir build-tsan --output-on-failure --timeout 60
```

## Tensor 示例与契约

```cpp
#include "tensor.h"

using inference::Tensor;

auto left = Tensor::random({3, 4});
auto same_shape = Tensor::ones({3, 4});
auto right = Tensor::random({4, 2});

auto sum = left + same_shape;       // [3, 4] + [3, 4] -> [3, 4]
auto product = left.matmul(right);  // [3, 4] @ [4, 2] -> [3, 2]
auto activated = left.relu();
auto probabilities = left.softmax();  // 2D Tensor 按行计算
```

边界规则：

- `Tensor()` 是空哨兵，`size()==0` 且 `data()==nullptr`。
- 显式 shape 必须至少有一维且每一维大于 0；`Tensor({})`、`Tensor({2, 0})` 会抛出 `std::invalid_argument`。
- 空 Tensor 的 `sum()` 定义为 `0`；索引、均值、极值、argmax、激活、softmax、matmul 等需要元素的操作会抛出异常。
- `operator[]` 和 `at(i,j)` 都检查边界；`at(i,j)` 还要求 Tensor 为 2D。
- 逐元素运算要求 shape 完全相同；`matmul` 只接受内维匹配的两个 2D Tensor。
- `data()` 是供实现层使用的原始指针接口，不提供额外边界保护。
- 显式数据构造允许表示任意 `float`，可用 `allFinite()` 检查；需要概率语义的 `softmax` 只接受有限输入，遇到 NaN、`+inf`、`-inf`（包括全 `-inf`）会抛出 `std::invalid_argument`，不会静默生成 NaN。
- `random` 的上下界以及 `randn` 的均值/标准差必须有限，且标准差严格大于 0；生成器若仍产生不可表示值会抛出 `std::overflow_error`。

## 构建网络

```cpp
#include "layers.h"

using namespace inference;

Sequential model;
model.add(Linear(784, 256));
model.add(ReLU());
model.add(Linear(256, 128));
model.add(ReLU());
model.add(Linear(128, 10));
model.add(Softmax());

Tensor input = Tensor::random({784});
Tensor output = model.forward(input);  // shape: [10]
```

`Linear(in_features, out_features)` 只接受 `[in_features]` 或 `[batch, in_features]`；过短、过长或维度不匹配的输入都会在读取前抛出 `std::invalid_argument`。显式权重和偏置必须全部为有限值。

所有公开 `Layer::forward` 共享同一数值契约：输入必须非空且全部有限，成功返回的输出也必须有限。输入或参数不合法使用 `std::invalid_argument`，有限输入经计算超出 `float` 范围使用 `std::overflow_error`；因此单独调用某一层与放入 `Sequential` 时不会出现两套失败语义。

推理归一化层可以直接载入训练结果：

```cpp
Tensor gamma({2}, {2.0f, 3.0f});
Tensor beta({2}, {1.0f, -1.0f});
Tensor running_mean({2}, {10.0f, 20.0f});
Tensor running_variance({2}, {4.0f, 9.0f});

BatchNorm batch_norm(gamma, beta, running_mean, running_variance);
batch_norm.setInferenceParameters(gamma, beta, running_mean, running_variance);

LayerNorm layer_norm(gamma, beta);
layer_norm.setAffineParameters(gamma, beta);
```

- `BatchNorm(num_features, eps)` 仍提供单位 gamma、零 beta、零均值和单位方差的教学默认值；训练后部署应使用四 Tensor 构造函数或 `setInferenceParameters`。运行方差必须非负，四组数据必须是一维、同 shape 且全部有限。
- 项目没有训练路径，因此 BatchNorm 不再提供不会被使用的 `momentum`。`running_mean`、`running_variance` 是推理状态，`numParameters()` 只统计可训练的 gamma 和 beta。
- LayerNorm 可通过构造函数或 `setAffineParameters` 载入 gamma/beta。均值和方差使用 double Welford 中间量计算，因此 `{FLT_MAX, FLT_MAX}` 得到有限零输出，`{1e20, -1e20}` 得到约 `{1, -1}`，而不是 NaN 或错误的 `{0, 0}`。
- BatchNorm 与 LayerNorm 都要求输入有限；参数、shape 或输入不合法抛出 `std::invalid_argument`，最终结果超出 `float` 可表示范围则抛出 `std::overflow_error`。参数 setter 会先完整校验和复制，再一次性替换，不留下部分更新。

本项目只实现推理路径：`Dropout::forward` 是恒等映射，`training()` 恒为 `false`，尝试 `setTraining(true)` 会抛出 `std::logic_error`，不再暴露“显示训练态但仍执行恒等推理”的矛盾接口。

## 多线程与异步契约

```cpp
#include "inference_engine.h"

using namespace inference;

Sequential model;
model.add(Linear(4, 2));
model.add(Softmax());

EngineConfig config;
config.num_threads = 4;
config.batch_size = 16;
config.max_queue_size = 64;
config.enable_profiling = false;

ParallelInferenceEngine engine(&model, config);
Tensor synchronous = engine.infer(Tensor::random({4}));
auto future = engine.inferAsync(Tensor::random({4}));
InferenceResult result = future.get();

std::vector<Tensor> inputs(100, Tensor::ones({4}));
auto results = engine.inferBatchParallel(inputs);
engine.stop();  // 非阻塞：关闭所有新入口，不丢弃已接收任务
engine.join();  // 阻塞：排空工作、回收 worker，进入 Joined
```

- `num_threads`、`batch_size`、`max_queue_size` 必须大于 0；worker 数在构造时固定。
- `InferenceEngine::infer`、`inferBatch` 和 `setConfig` 是公开的非虚模板方法，真正的差异点放在受保护虚实现中。因此通过 `InferenceEngine&`、指针，甚至显式写 `engine.InferenceEngine::infer(...)`，都不能绕过并行引擎的关闭与回调检查。
- `Sequential` 必须从引擎构造开始保持有效且结构、参数只读，直到外部 `join()` 正常返回，或在满足析构前提时由外部析构完成排空。`Runtime` 的 `shared_ptr` 只保护队列状态，不拥有 `Sequential`，单个任务 future 也不会延长模型寿命。
- 引擎对象本身由创建它的外部作用域拥有。销毁前必须先阻止新调用，并用调用方的 `future::get()` 或线程 `join()` 证明没有公开成员函数仍留在其他线程栈上；引擎的 `join()` 证明内部工作和模型访问归零，但不替调用者回收自己创建的线程。
- `config()` 返回加锁后的值快照；`setConfig()`、配置读取和提交可并发执行，不返回可形成数据竞态的内部引用。
- `batch_size` 是批量 API 每个提交/收集窗口的上限；并行窗口同时受 `max_queue_size` 限制。
- 批量 API 在入口取得窗口配置快照，但每个异步提交仍服从当时最新的队列上限和接收状态；若其他线程同时缩小队列或调用 `close()`，返回向量保持与输入等长，受影响项以 `success == false` 明确报告，而不是丢项或悬挂。
- `max_queue_size` 限制等待队列。直接异步提交遇到满队列时不会永久悬挂，而是返回一个已经就绪、`success == false` 的 future 结果。某个 `inferAsync` future 就绪只证明这一项已经产出结果；其他已接收任务仍可能排队或正在模型中运行。
- `close()` 是幂等的“关闭接收”步骤；它同时关闭同步与异步入口，只获取短期状态锁，不等待排空。之后的同步 `infer`/`inferBatch` 和阻塞式 `inferBatchParallel` 抛出 `std::logic_error`，`inferAsync` 返回已就绪失败结果，已接收工作继续完成。
- `stop()` 是 `close()` 的非阻塞停止请求别名，在任何线程、任何可见执行上下文中都不等待 worker。这个语义不依赖 `thread_local`，因此即使回调错误地把 `stop()` 委托给一个未传播上下文的新线程，同引擎自停和 A/B 交叉停止也不会在 `stop()` 内形成 join 环。
- `join()` 是唯一显式阻塞完成边界；它会先关闭入口，再等待 worker、队列、模型中的同步/异步工作以及已接收的同步/阻塞批量调用离开实现。外部多个 `join()` 调用者可安全并发且结果幂等；返回时 lifecycle 为 `Joined`，三个状态计数都为 0，模型不再被引擎访问。
- `closed()` 表示入口已经关闭，`stopped()` 只在 `Joined` 时为真。因此 `stop()` 返回后通常是 `closed()==true`、`stopped()==false`；不要把一个听起来像“停止”的函数名误当成完成证据。
- `Sequential` 要求输入有限，并在每层后检查输出；因此异步结果的 `success == true` 同时表示模型输出有限。非法 Softmax/归一化输入、数值溢出或其他模型异常都会转换成 `InferenceResult{success=false}`；调用者必须检查 `success` 与 `error_message`。
- worker 启动支持显式 `WorkerLauncher` 故障注入；任一线程创建失败时先关闭接收、唤醒并连接已创建线程，再重抛原异常，不留下 joinable thread。
- 空 batch 合法并立即返回空结果；统计在零成功或零耗时时保持有限的 `0`，不会除零。
- `status()` 在同一把运行态锁下返回 `lifecycle`、`accepting_requests`、`queued_requests`、`active_requests`、`in_flight_calls`。active 统计正在模型中的同步调用和已出队任务；in-flight 统计已经接收但尚未离开实现的同步与阻塞批量调用。状态只沿 `Accepting -> Draining -> Joined` 单调前进。

### `thread_local`、执行上下文和前置条件

引擎在调用 `Sequential::forward` 前建立内部 `thread_local` 回调深度。它能可靠拦截同一线程上的直接回调误用，但新建 OS 线程有自己独立的 `thread_local`，不会自动继承这个深度。为此公开 API 提供可复制的 `InferenceExecutionContext`：回调委托者在父线程 `capture()`，再在 `std::thread` 或强制异步的 `std::async(std::launch::async, ...)` 中用 `run()` 恢复标记。

```cpp
const InferenceExecutionContext context =
    InferenceExecutionContext::capture();  // 在 Layer::forward 内捕获

std::thread child([context, &target]() {
    context.run([&target]() {
        target.stop();  // 合法且始终非阻塞
        // target.infer(...) 会按回调叶子规则拒绝
        // target.join() 也会在改变目标状态前拒绝
    });
});
child.join();

auto delegated = std::async(std::launch::async, [context, &target]() {
    return context.run([&target]() { return target.inferAsync(Tensor::ones({1})); });
});
InferenceResult rejected = delegated.get().get();
```

由任一推理引擎驱动的 `Layer::forward` 对推理 API 都是叶子节点：在同线程或已传播上下文中调用同一/不同引擎的 `infer`、`inferBatch`、`inferAsync`、`inferBatchParallel` 都会在排队前拒绝；同步入口抛出 `std::logic_error`，异步入口返回已就绪失败结果。跨模型流水线应在回调外部按普通返回值或 future 组合。

这几层保证必须分开理解：

| 机制 | 库能强制保证 | 调用者仍须保证 |
|---|---|---|
| 内部 `thread_local` 回调深度 | 同线程直接推理与 `join()` 误用会被拒绝 | 不把它误认为会自动跨线程传播 |
| `InferenceExecutionContext::capture/run` | 显式传播后，`std::thread`/`std::async` 与父回调服从相同限制 | 回调委托的工作只要可能访问引擎，就必须捕获并传播 |
| 非阻塞 `stop()` | 无论标记是否传播，都只关闭入口，不会 join | 返回后仍要由合法外部拥有者调用 `join()` |
| 阻塞 `join()` | 可见回调上下文中先拒绝且不偷偷 close；正常外部并发可安全 join | 未传播的回调子线程对库看起来与外部线程相同，因此绝不能在其中调用 `join()` |
| 析构 | 可见回调或仍登记的阻塞成员调用会 fail-fast；合法外部析构会排空已接收异步任务 | 析构前阻止新调用，并证明其他线程栈上没有公开成员调用；忘记传播的回调子线程也不能销毁引擎 |

库不能仅凭一个普通新线程反推出“它由哪个回调创建”。因此未传播的回调子线程推理会像正常外部调用一样被接受；测试保留这个反例，是为了证明传播属于调用者前置条件，而不是宣称这种写法安全。反过来，真正的正常外部线程 `capture()` 得到不携带回调标记的上下文，仍可并发调用推理、`stop()` 和 `join()`；实现没有为了拦截回调误用而全局禁止外部并发。

回调仍可读取 `status()`/`config()`，可调用 `close()` 或 `stop()`；`setConfig()` 不等待 worker，但关闭后会按状态机拒绝。回调中销毁任何推理引擎没有可恢复的安全语义：析构函数不能抛异常或“取消析构”，所以可见违例使用 `std::terminate`，而不是分离当前 worker 后留下仍在栈上的成员调用和排队任务。

把关闭证据分开看，组合关系会更清楚：

| 观察到的证据 | 能证明 | 不能单独证明 |
|---|---|---|
| 某个 `inferAsync` future 完成 | 该任务已有成功或失败结果 | 队列已空、其他任务结束、模型可释放 |
| `close()` 或 `stop()` 返回 | 新同步/异步入口已关闭；调用本身没有等待图 | 旧任务排空、worker 已 join、模型可释放 |
| 外部 `join()` 返回 | queue/active/in-flight 为 0，worker 已回收，模型不再被引擎访问 | 调用者自己启动的线程已 join，其他成员调用栈已由调用者回收 |
| 包装成员调用的 future `get()` 或线程 `join()` | 那次调用已经从调用者线程返回 | 其他异步任务和其他调用者已结束 |
| 合法外部析构返回 | 在析构前提成立时，引擎对象和 worker 已回收，模型不再被访问 | 不满足前提的并发销毁会被自动“修好” |

推荐的拥有者关闭顺序是：

```cpp
engine.stop();          // 非阻塞关闭入口
batch_future.get();     // 证明调用方启动的批处理成员调用已经返回
caller_thread.join();   // 若使用了显式线程，同样由拥有者回收
engine.join();          // 证明内部工作与模型访问归零
// 现在才销毁 engine，随后可销毁非拥有的 model
```

如果调用者已经先建立了“没有其他公开成员调用仍在外部线程栈上”的屏障，也可以直接调用 `engine.join()`；它内部会先 close。测试还验证了 `join()` 返回后，即使引擎对象暂时仍存活，也可以先释放非拥有的模型，再销毁已 Joined 的引擎。

停止协议可以按状态理解：

```text
Accepting --close/stop(非阻塞)--> Draining --外部 join(阻塞)--> Joined
 同步/异步入口开放                 新入口拒绝，旧工作继续          三计数为 0，stopped()==true
```

## 自动测试覆盖

运行时测试分别覆盖：`inference_contracts` 验证 Tensor、各层、配置、同步/异步统一关闭、非阻塞 stop、显式 join、阻塞同步推理与并行批处理的计数快照，以及正常外部 `std::thread`/`std::async`；`inference_concurrency` 覆盖同线程直接回调、同引擎与跨引擎组合、传播和故意未传播的 `std::thread`/`std::async` 推理、传播与未传播的子线程 stop、传播后的 join 拒绝、双外部 join、submit/stop 竞争、外部析构排空、join/析构后的模型释放和并发配置。两个精确退出测试分别验证“回调直接销毁且队列仍有任务”和“外部销毁撞上活动批处理且队列仍有任务”都进入 terminate 处理器；`inference_fault_injection` 验证 worker 启动回滚。另有三个独立翻译单元持续验证公开头自足与 include guard。

并发测试用条件变量谓词、显式起跑门、promise/future、`join()` 和最终计数不变量建立 happens-before；`wait_for` 只作为死锁看门狗，不参与正确性排序。测试中没有 `sleep`、固定耗时判断或输出顺序断言。CTest 还为每个进程设置有限总超时，错误会可靠返回非零。
