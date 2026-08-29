# C++ 并发编程教程

> 本教程是并发专题的主要讲解位置。Day 29-35 使用其中的 C++17 主线；信号量、屏障、`jthread` 和协程等内容会明确标为 C++20 延伸，不是完成 35 天主线的前置条件。

建议先完成 [C++ 基础学习教程](CPP基础学习教程.md) 中的引用、RAII、智能指针、移动语义和 Lambda，再从线程生命周期开始学习。每日 README 负责把并发机制连接到当天真实目标；本教程负责完整说明共享状态、happens-before、等待谓词、异常传播和关闭协议。

| 路线 | 章节 | 编译基线 |
|---|---|---|
| 35 天主线 | 1-3、4.1、4.4-4.5、5-8 | C++17 |
| C++20 同步扩展 | 4.2-4.3、5.4 中标注的 wait/notify | C++20 |
| C++20 协程扩展 | 9 | C++20 |

并发正确性不能从一次输出顺序或一次“没有崩溃”推出。阅读每个组件时都要列出共享状态、互斥不变量、同步边、退出条件和所有线程的 join 责任。

---

## 目录

1. [并发编程基础概念](#1-并发编程基础概念)
2. [线程管理](#2-线程管理)
3. [线程间共享数据](#3-线程间共享数据)
4. [同步操作](#4-同步操作)
5. [原子操作与内存模型](#5-原子操作与内存模型)
6. [线程安全的数据结构](#6-线程安全的数据结构)
7. [并发设计模式](#7-并发设计模式)
8. [线程池实现](#8-线程池实现)
9. [协程（C++20）](#9-协程c20)

---

## 1. 并发编程基础概念

### 1.1 并发与并行的区别

```
并发 (Concurrency)：
┌──────────────────────────────────────────────────────┐
│ 单核处理器：通过时间片轮转实现"同时执行"             │
│                                                       │
│ Thread A: ████░░░░░░░░████░░░░░░░░░████░░░░░░░░      │
│ Thread B: ░░░░░░░░████░░░░░░░░████░░░░░░░░████       │
│           └─────────交替执行──────────┘              │
└──────────────────────────────────────────────────────┘

并行 (Parallelism)：
┌──────────────────────────────────────────────────────┐
│ 多核处理器：真正的同时执行                           │
│                                                       │
│ Core 1: ████████████████████████████████████         │
│ Core 2: ████████████████████████████████████         │
│         └──────────同时执行──────────┘               │
└──────────────────────────────────────────────────────┘
```

区分这两者是基础：**并发是结构、并行是执行**。并发指程序"被组织成多个可独立推进的任务"（多线程、协程、事件循环都是并发，哪怕跑在单核上）；并行指任务"在同一时刻真正同时执行"（需要多核或多机）。所以"单核多线程"是并发但不并行——它靠时间片轮转造成"同时"的错觉。反过来，单线程代码配 SIMD 指令也算并行（数据并行），却不是并发。一句话：**并发问"怎么组织任务"，并行问"怎么同时跑"**。C++ 的内存模型给出的是并发的可移植保证（多线程行为像某种交错），至于它是否真的并行，由硬件和运行时决定，程序不应依赖"是否真并行"来得到正确性。

### 1.2 为什么需要并发

**性能提升**：
- 利用多核处理器
- 提高资源利用率
- 处理阻塞操作（I/O、网络请求）

**架构优势**：
- 分离关注点
- 更好的响应性
- 简化异步处理

这些理由背后是一条因果：单核多线程在 CPU 密集任务上不会更快（甚至因切换开销更慢），它提升的是**资源利用率**——当一个线程阻塞在 I/O 时，另一个线程能用 CPU。所以并发的收益主要来自"任务里有等待"（I/O、网络、用户输入）或"有多个计算核心可同时用"（真正的并行）。只有计算、没有等待、又跑在单核上时，多线程往往比单线程更慢——这是"为什么要并发"不能一概而论的原因。

### 1.3 并发编程的挑战

并发带来收益，也带来一类单线程没有的问题：多个线程同时碰同一数据时，结果可能出错。先看一个最直白的例子——多个线程各自把同一个计数器加十万次：

```cpp
#include <iostream>
#include <thread>
#include <vector>

int counter = 0;                    // 普通int，非原子
void bump() { for (int i = 0; i < 100'000; ++i) ++counter; }

int main() {
    std::vector<std::thread> ts;
    for (int i = 0; i < 4; ++i) ts.emplace_back(bump);
    for (auto& t : ts) t.join();
    std::cout << "expect 400000, got " << counter << '\n';
}
```

期望 4×100000=400000，实际常得到十几万（如 165441）。为什么？因为 `++counter` 不是一步——它是"读当前值、加一、写回"三步。两个线程可能都读到 100、都加成 101、都写回 101，两次自增只长了 1。这种"两步之间被打断"的交错就是**数据竞争（data race）**的根源。

```
counter = 100
线程A 读 100 ──┐                线程B 读 100
              │ 算 101          │ 算 101
线程A 写 101 ◄┘                线程B 写 101 ◄── 覆盖A的结果
结果：两次自增只生效一次
```

C++ 标准明确规定：**数据竞争是未定义行为**。注意"未定义"不只是"结果可能错"——它意味着标准对这种情况不作任何保证：可能算少、可能算对、可能崩溃、可能在你换台机器/换优化级别后突然出错。上面这个例子在 `-O2` 下经常"算对"，在 `-O0` 下几乎总"算少"——这种**不可预测正是 UB 的危险**：不能用"测试跑过且结果对"来证明并发代码正确，因为换条件下它可能就不对。

解决数据竞争需要**同步**——让线程对共享数据的访问不再"撞在一起"。两种基本思路：①**互斥**（mutex）：一次只让一个线程进入临界区，串行化对共享数据的访问（§3）；②**原子操作**（atomic）：让"读-改-写"成为不可分割的一步，其他线程看不到中间态（§5）。本教程先讲互斥（它更通用、更易理解），再讲原子与内存模型。所以下面 §2 先学怎么建线程，§3 再用互斥解决刚才的数据竞争。

### 1.4 线程是什么

在深入之前，先给"线程"一句话定义：**线程是进程内的一条独立执行流**——它有自己的程序计数器、寄存器、栈，但与同进程的其他线程共享地址空间（代码、全局变量、堆）。操作系统调度的是线程（不是进程），多核上多个线程可真正同时跑。"共享地址空间"是线程间通信快的原因，也是数据竞争的根源——大家都能碰同一块内存，就得出"谁先谁后、会不会撞"的问题。

> 读者到这里只需记住：线程 = 一条执行流，共享内存 = 通信快但要同步。线程的创建、join、生命周期管理是 §2 的内容；同步原语（互斥、条件变量、原子）是 §3–§5 的内容。

### 1.5 旧的 1.3 内容说明

除非紧邻围栏明确标注“片段及省略项”，本教程的 `cpp` 围栏都可作为独立翻译单元用其章节标注的标准和严格告警编译；只定义类型或函数而没有 `main` 的围栏可使用 `-c` 编译。C++17 主线示例不得为了方便偷偷依赖 C++20 API。

下面这段是旧版的"原子计数器正确写法"，保留作为 §5 的引子——它用 `std::atomic` 解决了上面的数据竞争，机制（原子性、内存序）在 §5 详讲。现在读不懂 atomic 和 `memory_order_relaxed` 是正常的，到 §5 会展开。

```cpp
#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

// 错误写法不能作为运行示例：两个线程对普通 int 的并发读改写构成数据竞争。
// int counter = 0;
// void unsafeIncrement() { ++counter; }

std::atomic<int> counter{0};

void increment() {
    for (int i = 0; i < 100'000; ++i) {
        counter.fetch_add(1, std::memory_order_relaxed);
    }
}

int main() {
    std::vector<std::thread> threads;
    threads.reserve(2);
    try {
        threads.emplace_back(increment);
        threads.emplace_back(increment);
    } catch (...) {
        for (std::thread& thread : threads) {
            thread.join();
        }
        throw;
    }
    for (std::thread& thread : threads) {
        thread.join();
    }
    
    std::cout << counter.load(std::memory_order_relaxed) << '\n';
    return counter.load(std::memory_order_relaxed) == 200'000 ? 0 : 1;
}
```

原来对普通 `int` 执行 `counter++` 的版本不是“可能少算”这么简单：两个潜在并发的冲突访问中至少一个是非原子访问，且二者之间都没有 happens-before，就构成数据竞争；C++ 规定数据竞争是未定义行为。编译器不必保留下面的直觉交错，因此错误版本只适合作为推理图，不能放进测试等待某个数值。两个线程对同一原子对象的访问仍受内存序规则约束，但不会仅因“其中一个是写”就变成数据竞争。

**问题分析**：
```
counter++ 实际上是三步操作：
1. 读取 counter 值
2. 值加 1
3. 写回 counter

可能的交错执行：
t1: 读取 counter = 100
t2: 读取 counter = 100
t1: 计算 100 + 1 = 101
t2: 计算 100 + 1 = 101
t1: 写入 counter = 101
t2: 写入 counter = 101
结果：两次递增只增加了 1
```

---

## 2. 线程管理

### 2.1 创建线程

```cpp
#include <iostream>
#include <string>
#include <thread>
#include <vector>

// 方式1：函数指针
void threadFunction(int id) {
    std::cout << "Thread " << id << " running\n";
}

// 方式2：Lambda 表达式
void lambdaThread() {
    int value = 42;
    std::thread t([value]() {
        std::cout << "Lambda thread, value = " << value << "\n";
    });
    t.join();
}

// 方式3：函数对象（仿函数）
class ThreadFunctor {
public:
    void operator()(int x) const {
        std::cout << "Functor thread, x = " << x << "\n";
    }
};

// 方式4：成员函数
class Worker {
public:
    void doWork(int times) {
        for (int i = 0; i < times; ++i) {
            std::cout << "Working... " << i << "\n";
        }
    }
};

int main() {
    Worker worker;
    std::vector<std::thread> threads;
    threads.reserve(4);

    try {
        threads.emplace_back(threadFunction, 1);
        threads.emplace_back([] { std::cout << "Lambda thread\n"; });
        threads.emplace_back(ThreadFunctor{}, 3);
        threads.emplace_back(&Worker::doWork, &worker, 5);
    } catch (...) {
        // 后续 thread 构造失败时，先回收已经启动的线程再传播异常。
        for (std::thread& thread : threads) {
            thread.join();
        }
        throw;
    }

    for (std::thread& thread : threads) {
        thread.join();
    }
    
    return 0;
}
```

批量创建线程时，`reserve` 只能避免 vector 扩容，不能防止操作系统拒绝创建新线程；若第 N 个构造抛出而前 N−1 个线程仍 joinable，直接栈展开会触发 `std::terminate`，所以必须在异常路径 join 已启动线程，或使用拥有线程的 RAII 类型。

### 2.2 线程的生命周期管理

```cpp
#include <iostream>
#include <stdexcept>
#include <thread>
#include <utility>

class JoiningThread {
public:
    explicit JoiningThread(std::thread thread) : thread_(std::move(thread)) {}
    
    ~JoiningThread() {
        if (thread_.joinable()) {
            thread_.join();
        }
    }
    
    JoiningThread(const JoiningThread&) = delete;
    JoiningThread& operator=(const JoiningThread&) = delete;
    
private:
    std::thread thread_;
};

void threadLifecycle() {
    JoiningThread worker(std::thread([] {
        std::cout << "Worker thread\n";
    }));
    
    // 即使这里抛出异常，JoiningThread 也会在退栈时 join。
    // throw std::runtime_error("Error!");
    
}

int main() {
    threadLifecycle();
    return 0;
}
```

自动 join 只解决句柄回收，不会强制任务停止；析构可能无限等待，所以线程函数仍需有界工作或显式停止协议。拥有 `JoiningThread` 的对象不得在该 worker 自身销毁，否则会尝试自连接；C++20 可优先使用 `std::jthread` 配合 `stop_token`，但任务也必须主动响应停止请求。

### 2.3 向线程传递参数

```cpp
#include <iostream>
#include <functional>
#include <memory>
#include <string>
#include <thread>
#include <vector>

void processString(std::string str) {
    std::cout << "Processing: " << str << "\n";
}

void processPointer(std::unique_ptr<int> ptr) {
    std::cout << "Value: " << *ptr << "\n";
}

void passArguments() {
    const std::string snapshot = "copied value";
    const std::string borrowed = "borrowed value";
    std::string moved = "moved value";
    
    auto ptr = std::make_unique<int>(42);
    std::vector<std::thread> threads;
    threads.reserve(4);
    try {
        threads.emplace_back(processString, snapshot);
        threads.emplace_back(processString, std::cref(borrowed));
        threads.emplace_back(processString, std::move(moved));
        threads.emplace_back(processPointer, std::move(ptr));
    } catch (...) {
        for (std::thread& thread : threads) {
            thread.join();
        }
        throw;
    }

    for (std::thread& thread : threads) {
        thread.join();
    }
}

int main() {
    passArguments();
    return 0;
}
```

不能先把 `s` 通过 `std::cref(s)` 借给一个尚未完成的线程，又立刻在主线程 `std::move(s)`：线程入口把引用转换成按值参数时可能正在读取 `s`，主线程的移动同时写 `s`，两者会形成数据竞争。借用参数的契约是“对象活到 join，且所有并发访问都满足同步要求”；能复制快照时优先复制。

### 2.4 转移线程所有权

```cpp
#include <iostream>
#include <thread>
#include <vector>

std::thread createThread() {
    return std::thread([]() {
        std::cout << "Thread from function\n";
    });
}

void transferOwnership() {
    // 从函数返回线程
    std::thread t1 = createThread();
    
    // 转移所有权
    std::thread t2 = std::move(t1);
    // t1 现在不关联任何线程
    
    // 容器存储线程
    std::vector<std::thread> threads;
    try {
        threads.reserve(2);
        threads.emplace_back([] { std::cout << "Thread 1\n"; });
        threads.emplace_back([] { std::cout << "Thread 2\n"; });
    } catch (...) {
        for (std::thread& thread : threads) {
            thread.join();
        }
        t2.join();
        throw;
    }
    
    for (std::thread& thread : threads) {
        thread.join();
    }
    
    t2.join();
}

int main() {
    transferOwnership();
    return 0;
}
```

### 2.5 确定线程数量

```cpp
#include <iostream>
#include <thread>

void hardwareInfo() {
    const unsigned int hint = std::thread::hardware_concurrency();
    const unsigned int available = hint == 0U ? 1U : hint;
    
    // 只有大于 1 时才减一，避免 unsigned 的 0 - 1 下溢。
    const unsigned int threadCount = available > 1U ? available - 1U : 1U;
    
    std::cout << "Hardware hint: " << hint
              << ", chosen worker count: " << threadCount << '\n';
}
```

`hardware_concurrency()` 只是提示，允许返回 0，也不代表进程实际可使用的 CPU 配额；线程池还应结合任务性质、容器 CPU 限额和压测结果设置上限。

### 2.6 线程标识

```cpp
#include <iostream>
#include <thread>
#include <sstream>

void printThreadId() {
    std::thread::id id = std::this_thread::get_id();
    std::cout << "Thread ID: " << id << "\n";
}

void threadIdentifier() {
    std::thread::id masterId = std::this_thread::get_id();
    
    std::thread t1(printThreadId);
    try {
        std::thread t2(printThreadId);
    
        std::cout << "Main thread ID: " << masterId << "\n";
        std::cout << "t1 ID: " << t1.get_id() << "\n";
    
        const std::thread::id nullId;
        std::cout << "Null thread ID: " << nullId << '\n';
    
        t1.join();
        t2.join();
    } catch (...) {
        t1.join();
        throw;
    }
}
```

---

## 3. 线程间共享数据

### 3.1 使用互斥量保护共享数据

§1.3 的数据竞争根源是"多个线程同时读写同一变量、中间会交错"。**互斥量（mutex，mutual exclusion）**是最常用的解法：它是一把"一次只让一个线程通过"的锁——线程进入临界区前先"上锁"，离开时"解锁"，期间其他想进同一临界区的线程会等在外面，直到锁被释放。这样对共享数据的访问被串行化，不会再撞在一起。

`std::lock_guard` 是这把锁的 RAII 包装：构造时上锁、析构时（出作用域）自动解锁，即使中间抛异常也会解锁，不会留下"锁一直被占着"的死锁。

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

class ThreadSafeCounter {
public:
    void increment() {
        std::lock_guard<std::mutex> lock(mutex_);   // 进入临界区上锁，离开自动解锁
        ++value_;
    }
    
    int get() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return value_;
    }
    
private:
    mutable std::mutex mutex_;
    int value_ = 0;
};

void counterDemo() {
    ThreadSafeCounter counter;
    
    std::vector<std::thread> threads;
    threads.reserve(10);
    try {
        for (int i = 0; i < 10; ++i) {
            threads.emplace_back([&counter] {
                for (int j = 0; j < 1'000; ++j) {
                    counter.increment();
                }
            });
        }
    } catch (...) {
        for (std::thread& thread : threads) {
            thread.join();
        }
        throw;
    }
    
    for (std::thread& thread : threads) {
        thread.join();
    }
    
    std::cout << "Final value: " << counter.get() << "\n";  // 10000
}
```

### 3.2 死锁问题与解决

```cpp
#include <iostream>
#include <cassert>
#include <limits>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <vector>

class BankAccount {
public:
    explicit BankAccount(int balance) : balance_(balance) {
        if (balance < 0) {
            throw std::invalid_argument("account balance must be non-negative");
        }
    }
    
    bool transferTo(BankAccount& destination, int amount) {
        if (amount <= 0) {
            return false;
        }
        if (this == &destination) {
            return false;  // 拒绝别名输入，不能把同一 mutex 锁两次。
        }

        std::scoped_lock lock(mutex_, destination.mutex_);
        const int maximum = std::numeric_limits<int>::max();
        if (balance_ < amount || destination.balance_ > maximum - amount) {
            return false;
        }
        balance_ -= amount;
        destination.balance_ += amount;
        return true;
    }

    int balance() const {
        const std::lock_guard<std::mutex> lock(mutex_);
        return balance_;
    }
    
private:
    mutable std::mutex mutex_;
    int balance_;
};

void deadlockDemo() {
    BankAccount a(1000);
    BankAccount b(1000);
    
    std::vector<std::thread> threads;
    threads.reserve(2);
    try {
        threads.emplace_back([&] { static_cast<void>(a.transferTo(b, 100)); });
        threads.emplace_back([&] { static_cast<void>(b.transferTo(a, 100)); });
    } catch (...) {
        for (std::thread& thread : threads) {
            thread.join();
        }
        throw;
    }
    for (std::thread& thread : threads) {
        thread.join();
    }

    assert(!a.transferTo(a, 50));
    assert(!a.transferTo(b, 0));
    assert(a.balance() + b.balance() == 2'000);

    BankAccount overflowSource(1);
    BankAccount fullDestination(std::numeric_limits<int>::max());
    assert(!overflowSource.transferTo(fullDestination, 1));
    assert(overflowSource.balance() == 1);
    assert(fullDestination.balance() == std::numeric_limits<int>::max());

    BankAccount boundarySource(1);
    BankAccount boundaryDestination(std::numeric_limits<int>::max() - 1);
    assert(boundarySource.transferTo(boundaryDestination, 1));
    assert(boundarySource.balance() == 0);
    assert(boundaryDestination.balance() == std::numeric_limits<int>::max());
}
```

相反顺序分别手动锁两把 mutex 的版本可能循环等待，因此不应作为可执行演示。`std::scoped_lock` 使用避免死锁的算法获取不同 mutex，但不是“一条不可分割指令”；同一账户必须在加锁前拒绝，否则把同一非递归 mutex 传入两次仍然错误。账户构造保证余额非负，转账只接受正金额；取得两把锁后同时检查来源余额和目标容量，使用 `destination.balance_ > INT_MAX - amount` 在执行加法前拒绝溢出。失败路径不改任一余额，成功路径才一次提交减法和加法，因此“拒绝且零修改”与“总额守恒”都是接口不变量，而不是事后观察到的巧合。

### 3.3 避免死锁的准则

```cpp
#include <functional>
#include <limits>
#include <mutex>
#include <stdexcept>

// 准则1：避免嵌套锁
void noNestedLocks() {
    std::mutex m1, m2;
    
    // 坏做法
    // {
    //     std::lock_guard<std::mutex> lock1(m1);
    //     // ... 一些操作 ...
    //     std::lock_guard<std::mutex> lock2(m2);  // 危险！
    // }
    
    // 好做法：同时获取
    std::scoped_lock lock(m1, m2);  // C++17
}

// 准则2：调用用户代码时不要持有锁
class DataProcessor {
public:
    void process(const std::function<void(int)>& callback) {
        int value;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            value = data_;
        }  // 锁在这里释放
        
        callback(value);  // 调用用户代码时不持有锁
    }
    
private:
    std::mutex mutex_;
    int data_{0};
};

// 准则3：按固定顺序获取锁
class HierarchicalMutex {
public:
    explicit HierarchicalMutex(int hierarchyValue) 
        : hierarchyValue_(hierarchyValue) {}
    
    void lock() {
        // 检查层级约束
        if (currentHierarchy <= hierarchyValue_) {
            throw std::logic_error("Mutex hierarchy violated");
        }
        mutex_.lock();
        previousHierarchy_ = currentHierarchy;
        currentHierarchy = hierarchyValue_;
    }
    
    void unlock() {
        currentHierarchy = previousHierarchy_;
        mutex_.unlock();
    }
    
private:
    std::mutex mutex_;
    const int hierarchyValue_;
    int previousHierarchy_{0};
    inline static thread_local int currentHierarchy =
        std::numeric_limits<int>::max();
};
```

### 3.4 std::unique_lock 的灵活性

```cpp
#include <iostream>
#include <chrono>
#include <mutex>
#include <thread>
#include <utility>

void uniqueLockDemo() {
    std::mutex m;
    
    // 延迟锁定
    std::unique_lock<std::mutex> lock1(m, std::defer_lock);
    // ... 做一些不需要锁的操作 ...
    lock1.lock();  // 现在才锁定
    
    lock1.unlock();

    // 尝试锁定（非阻塞）
    std::unique_lock<std::mutex> lock2(m, std::try_to_lock);
    if (lock2.owns_lock()) {
        // 成功获取锁
    } else {
        // 获取失败
    }
    
    // try_lock_for 需要 timed_mutex；普通 mutex 只有 try_lock。
    std::timed_mutex timed;
    std::unique_lock<std::timed_mutex> lock3(timed, std::defer_lock);
    if (lock3.try_lock_for(std::chrono::milliseconds(100))) {
        // 在100ms内成功获取锁
    }
    
    if (lock2.owns_lock()) {
        lock2.unlock();
    }
    lock1.lock();
    
    // 转移所有权
    std::unique_lock<std::mutex> lock4 = std::move(lock1);
    std::cout << "moved lock owns mutex: " << std::boolalpha
              << lock4.owns_lock() << '\n';
}
```

---

## 4. 同步操作

互斥量解决"同时访问会出错"，但有一类问题它不够用：**一个线程要等某个条件成立才继续**（如消费者要等队列非空）。靠互斥量只能"反复加锁检查、不满足就解锁睡一下再查"（轮询），既浪费 CPU 又反应慢。**条件变量（condition variable）**就是为这类"等待-通知"设计的：线程可以"原子地释放锁并进入等待"，另一线程让条件成立后"通知"它醒来。它配合互斥量使用——检查条件时持锁、等待时临时释放锁、被唤醒时重新加锁再检查。

### 4.1 条件变量

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <optional>
#include <queue>

template<typename T>
class ThreadSafeQueue {
public:
    bool push(T value) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (closed_) {
                return false;
            }
            queue_.push(std::move(value));
        }
        cond_.notify_one();
        return true;
    }

    void close() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            closed_ = true;
        }
        cond_.notify_all();
    }

    std::optional<T> pop() {
        std::unique_lock<std::mutex> lock(mutex_);

        cond_.wait(lock, [this] { return closed_ || !queue_.empty(); });
        if (queue_.empty()) {
            return std::nullopt;  // 已 close 且旧元素已经排空。
        }
        T value = std::move(queue_.front());
        queue_.pop();
        return value;
    }

    std::optional<T> tryPopFor(std::chrono::milliseconds timeout) {
        std::unique_lock<std::mutex> lock(mutex_);

        if (!cond_.wait_for(lock, timeout,
                            [this] { return closed_ || !queue_.empty(); })) {
            return std::nullopt;
        }
        if (queue_.empty()) {
            return std::nullopt;
        }
        T value = std::move(queue_.front());
        queue_.pop();
        return value;
    }
    
private:
    std::mutex mutex_;
    std::condition_variable cond_;
    std::queue<T> queue_;
    bool closed_{false};
};

void producerConsumer() {
    ThreadSafeQueue<int> queue;
    
    // 生产者
    std::thread producer([&] {
        for (int i = 0; i < 10; ++i) {
            if (!queue.push(i)) {
                break;
            }
            std::cout << "Produced: " << i << "\n";
        }
        queue.close();
    });

    try {
        std::thread consumer([&] {
            while (std::optional<int> value = queue.pop()) {
                std::cout << "Consumed: " << *value << "\n";
            }
        });

        producer.join();
        consumer.join();
    } catch (...) {
        producer.join();
        throw;
    }
}
```

这里采用 close-reject-drain 契约：`close()` 后 `push` 拒绝新元素，消费者仍取完已经接受的元素，队列为空后 `pop` 返回 `nullopt`。析构队列不会自动安全地唤醒仍在使用它的线程；拥有者必须先 close，再 join 所有生产者和消费者，最后才能销毁队列。

#### 为什么 wait 要用谓词形式（循环而非 if）

上面 `pop` 用 `cond_.wait(lock, pred)`，`pred` 是个返回 `bool` 的谓词。它等价于手写的循环：

```
cond_.wait(lock, [&]{ return closed_ || !queue_.empty(); });
// 等价于：
while (!pred()) {                       // 条件不满足才等
    cond_.wait(lock);                    // 释放锁、阻塞、被唤醒后重新加锁
}                                       // 被唤醒后回到 while 重新检查条件
```

为什么必须是 `while` 循环、不能是 `if`？两个原因，都和"被唤醒"不等于"条件已满足"有关：

1. **虚假唤醒（spurious wakeup）**：标准允许 `wait` 在没有 `notify` 的情况下也返回。这是实现层面的让步（比如某些系统为避免丢失信号而保守唤醒），标准显式允许它。若用 `if`，被虚假唤醒后直接往下走，条件还可能不满足——读到空队列或已关闭但未排空的状态。`while` 则强制重新检查谓词，虚假唤醒只是白绕一圈、无害。
2. **唤醒后条件仍可能不满足**：即使是被真实 `notify` 唤醒，在你重新拿到锁之前，另一个消费者可能已经把元素取走了（多消费者场景）。`if` 会直接消费一个不存在的元素；`while` 重新检查发现队列又空了，重新等待。

所以条件变量的正确用法恒为"while 循环检查谓词"，而 `wait(lock, pred)` 的谓词重载就是把这条循环封装好了——**用谓词形式就不会写出 `if` 的 bug**。这也是 §6/§7 里消费者模式统一用谓词形式的原因。注意谓词里的条件要包含"关闭"分支（`closed_ || ...`），否则关闭时消费者会永远等不到满足"队列非空"而卡死。

### 4.2 信号量（C++20）

```cpp
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <semaphore>
#include <thread>
#include <vector>

std::counting_semaphore<5> accessSlots(3);

class SemaphorePermit {
public:
    explicit SemaphorePermit(std::counting_semaphore<5>& source)
        : semaphore_(source) {
        semaphore_.acquire();
    }

    ~SemaphorePermit() { semaphore_.release(); }

    SemaphorePermit(const SemaphorePermit&) = delete;
    SemaphorePermit& operator=(const SemaphorePermit&) = delete;

private:
    std::counting_semaphore<5>& semaphore_;
};

void limitedAccess(int id) {
    const SemaphorePermit permit(accessSlots);
    std::cout << "Thread " << id << " accessing resource\n";
}

void semaphoreDemo() {
    std::vector<std::thread> threads;
    threads.reserve(10);
    try {
        for (int i = 0; i < 10; ++i) {
            threads.emplace_back(limitedAccess, i);
        }
    } catch (...) {
        for (std::thread& thread : threads) {
            thread.join();
        }
        throw;
    }
    
    for (std::thread& thread : threads) {
        thread.join();
    }
}

// C++11 信号量实现
class Semaphore {
public:
    explicit Semaphore(int count = 0) : count_(count) {}
    
    void wait() {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [this]() { return count_ > 0; });
        --count_;
    }
    
    void signal() {
        std::lock_guard<std::mutex> lock(mutex_);
        ++count_;
        cond_.notify_one();
    }
    
private:
    std::mutex mutex_;
    std::condition_variable cond_;
    int count_;
};
```

### 4.3 屏障与栅栏（C++20）

```cpp
#include <iostream>
#include <barrier>
#include <cstddef>
#include <thread>
#include <vector>

void barrierDemo() {
    constexpr std::ptrdiff_t numThreads = 4;
    
    // C++20 barrier
    std::barrier syncPoint(numThreads, []() noexcept {
        std::cout << "--- All threads reached barrier ---\n";
    });
    
    auto worker = [&](std::ptrdiff_t id) {
        for (int phase = 0; phase < 3; ++phase) {
            std::cout << "Thread " << id << " phase " << phase << "\n";
            syncPoint.arrive_and_wait();  // 等待所有线程到达
        }
    };
    
    std::vector<std::thread> threads;
    threads.reserve(static_cast<std::size_t>(numThreads));
    std::ptrdiff_t started = 0;
    try {
        for (; started < numThreads; ++started) {
            threads.emplace_back(worker, started);
        }
    } catch (...) {
        // 未创建的参与者必须退出 barrier，否则已启动线程会永久等待。
        for (std::ptrdiff_t missing = started; missing < numThreads; ++missing) {
            syncPoint.arrive_and_drop();
        }
        for (std::thread& thread : threads) {
            thread.join();
        }
        throw;
    }
    
    for (std::thread& thread : threads) {
        thread.join();
    }
}
```

### 4.4 Future 与 Promise

```cpp
#include <iostream>
#include <chrono>
#include <exception>
#include <future>
#include <stdexcept>
#include <thread>
#include <vector>

void futurePromiseDemo() {
    // promise: 生产者端
    // future: 消费者端
    
    std::promise<int> promise;
    std::future<int> future = promise.get_future();
    
    // 在另一个线程中设置值
    std::thread producer([&promise]() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        promise.set_value(42);
    });
    
    producer.join();
    const int value = future.get();
    std::cout << "Got value: " << value << "\n";
}

void exceptionHandling() {
    std::promise<int> promise;
    std::future<int> future = promise.get_future();
    
    std::thread t([&promise]() {
        try {
            throw std::runtime_error("Error in thread");
        } catch (...) {
            promise.set_exception(std::current_exception());
        }
    });
    
    t.join();
    try {
        std::cout << "Value: " << future.get() << '\n';
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << "\n";
    }
}

void sharedFuture() {
    // shared_future 可以被多个线程等待
    std::promise<int> promise;
    std::shared_future<int> sf = promise.get_future().share();
    
    std::vector<std::thread> threads;
    threads.reserve(3);
    try {
        for (int i = 0; i < 3; ++i) {
            threads.emplace_back([sf, i] {
                const int value = sf.get();
                std::cout << "Thread " << i << ": " << value << "\n";
            });
        }
    } catch (...) {
        promise.set_value(100);  // 先解除已启动线程的等待。
        for (std::thread& thread : threads) {
            thread.join();
        }
        throw;
    }
    
    promise.set_value(100);
    
    for (std::thread& thread : threads) {
        thread.join();
    }
}
```

### 4.5 std::async

```cpp
#include <iostream>
#include <chrono>
#include <future>
#include <iterator>
#include <numeric>
#include <thread>
#include <vector>

void asyncDemo() {
    // 默认启动策略：由实现决定异步或延迟
    auto f1 = std::async([]() {
        return 42;
    });
    
    // 强制异步启动
    auto f2 = std::async(std::launch::async, []() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return 100;
    });
    
    // 延迟启动（在 get() 时才执行）
    auto f3 = std::async(std::launch::deferred, []() {
        return 200;
    });
    
    std::cout << "f1: " << f1.get() << "\n";
    std::cout << "f2: " << f2.get() << "\n";
    std::cout << "f3: " << f3.get() << "\n";
}

// 并行求和示例
template<typename Iterator, typename T>
T parallelSum(Iterator begin, Iterator end, T init) {
    auto length = std::distance(begin, end);
    if (length < 10000) {
        return std::accumulate(begin, end, init);
    }
    
    Iterator mid = begin;
    std::advance(mid, length / 2);
    
    auto future = std::async(std::launch::async,
        parallelSum<Iterator, T>, mid, end, T());
    
    T firstHalf = parallelSum(begin, mid, init);
    T secondHalf = future.get();
    
    return firstHalf + secondHalf;
}
```

默认 `std::async` 允许 deferred，只有显式 `launch::async` 才把异步执行写进契约；显式异步在资源不足时可抛 `std::system_error`。上面的递归求和可能随输入创建大量线程，只适合展示任务分解；生产代码应使用有界线程池或并行算法，并让调用方决定线程预算、停止和异常处理。

---

## 5. 原子操作与内存模型

### 5.0 为什么原子和内存模型是难点：单线程直觉在这里失效

单线程代码里，我们习惯了"代码按写的顺序执行、一个变量写完就能读到新值"。这两条直觉在多线程下会崩，这正是内存模型要解决的问题。崩在两处：

1. **交错执行**：多个线程的语句不是"一个线程跑完再跑另一个"，而是任意交错。§1.3 的 `++counter` 数据竞争就是交错导致——两步之间插进了别的线程。
2. **重排与可见性**：更反直觉的是，**即使你写 `data = 42; flag = true;`，在另一个线程里也可能先看到 `flag` 变 true、却还看到 `data` 是旧值**。原因有二：①编译器为了优化可能重排这两条语句的顺序；②CPU 有缓存，一个核的写不一定立刻对另一个核可见。

```
线程1（写数据置标志）        线程2（等标志读数据）
data = 42;                   while (flag != true) {}
flag = true;                 print(data);   // 期望 42，但可能读到旧值？
```

单线程里这不可能错——`data=42` 在 `flag=true` 之前，读 `flag` 为 true 时 `data` 一定已是 42。但多线程下，若没有同步约束，线程1 的 `data=42` 和 `flag=true` 可能被重排、且 `data` 的写可能还没对线程2 可见——于是线程2 看到 flag=true 却读到 data 的旧值。这就是为什么"光用普通变量当标志"在多线程是错的，需要原子操作和内存序来建立"重排约束"和"可见性保证"。

**内存模型就是 C++ 对"多线程下什么行为被保证"的契约**：它规定哪些重排允许、什么样的操作能建立 synchronizes-with 关系（从而让一个线程的写对另一个线程可见）。下面的原子操作和内存顺序，本质就是用各种约束来表达"我需要多强的可见性/顺序保证"。理解了这一点，再看 `acquire/release/seq_cst` 等就不是死记规则，而是"按需选保证强度"。

### 5.1 原子类型

```cpp
#include <iostream>
#include <thread>
#include <atomic>
#include <vector>

// 原子变量：提供原子语义；具体实现是否无锁需查询
std::atomic<int> atomicCounter(0);

void atomicIncrement() {
    for (int i = 0; i < 100000; ++i) {
        atomicCounter.fetch_add(1, std::memory_order_relaxed);
    }
}

void atomicDemo() {
    std::vector<std::thread> threads;
    threads.reserve(10);
    try {
        for (int i = 0; i < 10; ++i) {
            threads.emplace_back(atomicIncrement);
        }
    } catch (...) {
        for (std::thread& thread : threads) {
            thread.join();
        }
        throw;
    }
    
    for (std::thread& thread : threads) {
        thread.join();
    }
    
    std::cout << "Atomic counter: " << atomicCounter << "\n";  // 1000000
}
```

### 5.2 原子操作详解

```cpp
#include <iostream>
#include <atomic>

void atomicOperations() {
    std::atomic<int> a(10);
    
    // 基本操作
    a.store(20);           // 存储
    const int loaded = a.load();
    const int old = a.exchange(30);
    
    // 比较交换（CAS: Compare-And-Swap）
    int expected = 30;
    const bool success = a.compare_exchange_strong(expected, 40);
    // 如果 a == expected，则 a = 40，返回 true
    // 否则 expected = a，返回 false
    
    // 弱版本（可能虚假失败，但性能更好）
    expected = 40;
    while (!a.compare_exchange_weak(expected, 50)) {
        // expected 已被更新
    }
    
    // 整数特有操作
    a.fetch_add(10);    // 原子加
    a.fetch_sub(5);     // 原子减
    a.fetch_and(0xFF);  // 原子与
    a.fetch_or(0x01);   // 原子或
    a.fetch_xor(0xFF);  // 原子异或
    
    // 指针原子操作
    std::atomic<int*> ptr(nullptr);
    int x = 10;
    ptr.store(&x);
    int* const loadedPointer = ptr.load();

    std::cout << loaded << ' ' << old << ' ' << std::boolalpha << success
              << ' ' << *loadedPointer << ' ' << a.load() << '\n';
}
```

CAS 循环本身并不足以得到可安全复用的无锁栈：并发 `pop` 还会遇到 ABA、节点何时释放以及其他线程仍持有旧指针的问题，通常需要 hazard pointer、epoch 回收或成熟库。没有内存回收方案时，不要把“能原子更新 head”误当成完整容器。

### 5.3 内存顺序

原子操作除了"原子"，还要约束它和其他（包括非原子）内存访问的**可见顺序**。这就是内存顺序（memory order）。`std::memory_order` 有六个值，不是"强度刻度"，而是不同语义的同步原语：

| 值 | 语义 | 适用操作 | 何时用 |
|---|---|---|---|
| `memory_order_relaxed` | 只保证本操作原子、参与本对象的修改顺序；**不建立跨线程同步** | load/store/RMW | 计数器、统计：只关心总数对，不在乎与其他变量的先后 |
| `memory_order_release` | **store/RMW 做 release**：把本线程此前（program order 前）的内存写入"发布"出去 | store/RMW | 生产者写完数据后置标志 |
| `memory_order_acquire` | **load 做 acquire**：读到某个 release 发布的值后，禁止后续读被重排到这次 acquire 之前 | load | 消费者读标志后读数据 |
| `memory_order_acq_rel` | RMW 同时是 acquire 和 release | load+store（fetch_add 等） | 既要读到旧状态发布、又要发布本次写入的读改写 |
| `memory_order_seq_cst` | 在 acquire/release 之上，**所有 seq_cst 操作还参与一个单一总序**（所有线程看到同一顺序） | load/store/RMW | 默认；需要全局一致顺序时 |
| `memory_order_consume` | acquire 的弱化版：只对**有数据依赖**的访问建立同步 | load | **实践中几乎不用**，见下 |

#### happens-before 是怎么"连"起来的

并发正确性的本质是 `happens-before` 关系：若 A happens-before B，则 A 的内存写对 B 可见。原子操作的 acquire/release 就是用来**织出这条边**的。release/acquire 配对的机制：

```
线程1 (生产者)                       线程2 (消费者)
─────────────────────                ─────────────────────
data = 42;          // 普通写        while (!ready.load(acquire)) {}
ready.store(true,   // release       // ↑ 读到 true 时：与那次 release store
   release);                          //   建立 synchronizes-with 关系
                                      std::cout << data;  // 一定看到 42
```

连起来的逻辑（图中 `→hb` 表示 happens-before）：

```
data=42 ──seq─→ ready.store(rel) ──synchronizes-with─→ ready.load(acq) ──seq─→ 读 data
   └──────────────────────────── happens-before ───────────────────────────┘
        所以 producer 对 data 的写，对 consumer 读 data 可见（data 不是 atomic 也不构成数据竞争）
```

关键不是"CPU 恰好没重排"，而是 consumer 的 acquire load **读到了** producer 的 release store 写入的那个值（`true`），这一读才让两者建立 synchronizes-with，进而连起 happens-before。如果 acquire load 没读到对应值（还在轮询），两边写上 acquire/release 并不会自动连起两个线程。**没有"读到发布值"这一步，内存序不产生同步。**

> **代码性质：上下文片段。** 定义了 `producer/consumer/memoryOrderDemo`，需自配 `main()` 调用并加 `-pthread` 编译。

```cpp
#include <iostream>
#include <atomic>
#include <thread>

std::atomic<bool> ready(false);
int data = 0;

void producer() {
    data = 42;
    ready.store(true, std::memory_order_release);  // 发布此前对 data 的写入
}

void consumer() {
    while (!ready.load(std::memory_order_acquire)) {  // 读到 true 时与 release store 同步
        std::this_thread::yield();
    }
    std::cout << "Data: " << data << "\n";  // release/acquire 使普通 int 读取也是安全的
}

void memoryOrderDemo() {
    std::thread t1(producer);
    try {
        std::thread t2(consumer);
        t1.join();
        t2.join();
    } catch (...) {
        t1.join();
        throw;
    }
}
```

输出（实现无关）：
```
Data: 42
```

#### 为什么默认的 seq_cst 贵

`memory_order_seq_cst`（默认）比 acquire/release 多一条约束：**所有 seq_cst 操作参与一个单一总序**——所有线程看到的这些操作的先后完全一致。这条额外约束在硬件上不免费。以 x86 为例，seq_cst 的 store 要保证全序，编译器会把它编成带 LOCK 语义的指令（`XCHG`，隐含总线锁），而 release store 只是普通 `MOV`：

```
x.store(1, std::memory_order_seq_cst);   // -O2 汇编： movl $1,%eax ; xchgl x,%eax   ← XCHG 带 LOCK，贵
x.store(1, std::memory_order_release);   // -O2 汇编： movl $1,x                      ← 普通 MOV
```

（上面是 64 位 g++ `-O2` 的实测汇编片段。）x86 本身是强序架构，acquire/release 的语义大多由硬件天然保证，几乎零额外开销；但 seq_cst 要求全局单一总序，得插入更强的栅栏/原子指令。在 ARM、Power 等弱序架构上，seq_cst 与 acquire/release 的差距更大。**结论：默认 seq_cst 最安全但最贵；能用 acquire/release 表达的同步，就别让所有操作都背 seq_cst 的全序成本。**

#### memory_order_consume：几乎不用的第六个序

`memory_order_consume` 是 acquire 的弱化版：只对**依赖该 load 值**的后续访问建立同步（数据依赖），不对无关访问排序。设计意图是在弱序架构上比 acquire 省（少一道栅栏）。但标准 [atomics.order]/1.3 的官方注记直言："Prefer `memory_order_acquire`, which provides stronger guarantees... Implementations have found it infeasible to provide performance better than that of `memory_order_acquire`."——实现者发现 consume 的性能优势在主流硬件上无法兑现，**主流编译器都把 consume 提升为 acquire**。所以：知道有这个值，但实践中用 acquire，不要用 consume。这也解释了为什么很多教程只讲五个序——consume 在实际中等同 acquire。

#### false sharing：缓存行的隐性代价

原子操作保证了正确性，但若两个线程频繁写**位于同一缓存行（通常 64 字节）的不同变量**，每次写都会让对方 CPU 的该缓存行失效，导致两核反复互相刷新——这叫 false sharing（伪共享），性能可能差几倍到几十倍，即使逻辑上两个变量毫无关系。修复方法是用 `alignas(64)` 把各线程写的变量**隔到不同缓存行**：

> **代码性质：上下文片段。** 展示 false sharing 与 alignas 缓解，需放进可运行的计数循环上下文（真实差异需基准测试观察）。

```cpp
#include <atomic>
#include <cstddef>

// ❌ 两个计数器可能落在同一缓存行，两线程分别自增时互相失效
struct BadCounters {
    std::atomic<int> a{0};
    std::atomic<int> b{0};   // 与 a 大概率同处一个 64 字节缓存行
};

// ✅ 用 alignas 把每个计数器放到独立缓存行
struct alignas(64) PaddedCounter { std::atomic<int> v{0}; };
struct GoodCounters {
    PaddedCounter a;          // 各占一个缓存行，互不影响
    PaddedCounter b;
};
```

`alignas(64)` 让每个 `PaddedCounter` 起始地址按 64 字节对齐、且自身占满一个缓存行，于是两个计数器不会落在同一缓存行。这是无锁/高并发代码里常见且关键的优化，属于"内存序之外、但同样关乎正确并发性能"的底层知识。

#### 实践选型

- **计数器、统计**（只关心总数对、不与其他变量有先后要求）：`relaxed`。
- **发布/等待**（一个线程写数据置标志、另一个等标志读数据）：写用 `release`、等用 `acquire`。
- **读改写且要同时发布**（如原子队列的 push）：`acq_rel`。
- **需要全局一致顺序**（如自旋锁、初始化一次的全局标志、复杂的多变量不变量）：`seq_cst`（或默认）。
- **不要用 `consume`**（等同 acquire，无收益）。

从默认 `seq_cst` 起步保证正确，定位到性能瓶颈且能确认 acquire/release 足够后，再局部放宽。**先正确，再调优**——内存序调错的 bug 极难复现和调试。

双重检查锁定需要额外处理原子发布、对象销毁和测试隔离，不适合当作内存序入门模板。单例的选型与 Meyers Singleton 见 [设计模式教程](设计模式教程.md#21-单例模式面试必考)；实际项目通常更应先考虑由上层 owner 管理普通对象的生命周期。

### 5.4 自旋锁实现

```cpp
#include <iostream>
#include <thread>
#include <atomic>

class SpinLock {
public:
    void lock() {
        while (flag_.test_and_set(std::memory_order_acquire)) {
            // 自旋等待
            // 可以添加退避策略减少总线争用
            std::this_thread::yield();
        }
    }
    
    void unlock() {
        flag_.clear(std::memory_order_release);
    }
    
private:
    std::atomic_flag flag_ = ATOMIC_FLAG_INIT;
};

// C++20: 使用 std::atomic_flag 的 wait/notify
class SpinLockCpp20 {
public:
    void lock() {
        while (flag_.test_and_set(std::memory_order_acquire)) {
            flag_.wait(true, std::memory_order_relaxed);  // 等待而不是自旋
        }
    }
    
    void unlock() {
        flag_.clear(std::memory_order_release);
        flag_.notify_one();  // 唤醒一个等待线程
    }
    
private:
    std::atomic_flag flag_ = ATOMIC_FLAG_INIT;
};
```

---

## 6. 线程安全的数据结构

### 6.1 线程安全栈

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <stack>
#include <memory>
#include <exception>

struct EmptyStackException : std::exception {
    const char* what() const noexcept override {
        return "Stack is empty";
    }
};

template<typename T>
class ThreadSafeStack {
public:
    ThreadSafeStack() = default;
    
    ThreadSafeStack(const ThreadSafeStack& other) {
        std::lock_guard<std::mutex> lock(other.mutex_);
        stack_ = other.stack_;
    }
    
    void push(T value) {
        std::lock_guard<std::mutex> lock(mutex_);
        stack_.push(std::move(value));
    }
    
    std::shared_ptr<T> pop() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (stack_.empty()) {
            return nullptr;  // 或 throw EmptyStackException();
        }
        auto result = std::make_shared<T>(std::move(stack_.top()));
        stack_.pop();
        return result;
    }
    
    void pop(T& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (stack_.empty()) {
            throw EmptyStackException();
        }
        value = std::move(stack_.top());
        stack_.pop();
    }
    
    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return stack_.empty();
    }
    
private:
    mutable std::mutex mutex_;
    std::stack<T> stack_;
};
```

### 6.2 线程安全队列（带条件变量）

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <optional>
#include <queue>

template<typename T>
class ThreadSafeQueue {
public:
    ThreadSafeQueue() = default;

    ThreadSafeQueue(const ThreadSafeQueue&) = delete;
    ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;

    bool push(T value) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (closed_) {
                return false;
            }
            queue_.push(std::move(value));
        }
        cond_.notify_one();
        return true;
    }

    void close() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            closed_ = true;
        }
        cond_.notify_all();
    }

    std::optional<T> waitAndPop() {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [this] { return closed_ || !queue_.empty(); });
        if (queue_.empty()) {
            return std::nullopt;
        }
        T result = std::move(queue_.front());
        queue_.pop();
        return result;
    }

    std::optional<T> tryPop() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) {
            return std::nullopt;
        }
        T result = std::move(queue_.front());
        queue_.pop();
        return result;
    }
    
    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }
    
private:
    mutable std::mutex mutex_;
    std::queue<T> queue_;
    std::condition_variable cond_;
    bool closed_{false};
};
```

这版队列同样采用 close-reject-drain：关闭与队列为空是不同状态，等待谓词必须同时观察二者，才能让永久阻塞的消费者退出。`empty()` 只是一瞬间的快照，不能先 `if (!empty())` 再假设 `tryPop()` 一定成功；队列拥有者必须在销毁前 close 并 join 所有调用者。

### 6.3 线程安全查找表

```cpp
#include <iostream>
#include <algorithm>
#include <cstddef>
#include <functional>
#include <list>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <stdexcept>
#include <thread>
#include <vector>

template<typename Key, typename Value, typename Hash = std::hash<Key>>
class ThreadSafeLookupTable {
public:
    ThreadSafeLookupTable(
        std::size_t numBuckets = 19,
        const Hash& hasher = Hash()
    ) : buckets_(numBuckets), hasher_(hasher) {
        if (numBuckets == 0U) {
            throw std::invalid_argument("lookup table needs at least one bucket");
        }
        for (auto& bucket : buckets_) {
            bucket = std::make_unique<Bucket>();
        }
    }
    
    Value getValue(const Key& key, const Value& defaultValue = Value()) const {
        return getBucket(key).getValue(key, defaultValue);
    }
    
    void insertOrUpdate(const Key& key, const Value& value) {
        getBucket(key).insertOrUpdate(key, value);
    }
    
    void erase(const Key& key) {
        getBucket(key).erase(key);
    }
    
private:
    class Bucket {
    public:
        Value getValue(const Key& key, const Value& defaultValue) const {
            std::shared_lock<std::shared_mutex> lock(mutex_);
            auto it = std::find_if(data_.begin(), data_.end(),
                [&](const auto& pair) { return pair.first == key; });
            return it != data_.end() ? it->second : defaultValue;
        }
        
        void insertOrUpdate(const Key& key, const Value& value) {
            std::unique_lock<std::shared_mutex> lock(mutex_);
            auto it = std::find_if(data_.begin(), data_.end(),
                [&](const auto& pair) { return pair.first == key; });
            if (it != data_.end()) {
                it->second = value;
            } else {
                data_.emplace_back(key, value);
            }
        }
        
        void erase(const Key& key) {
            std::unique_lock<std::shared_mutex> lock(mutex_);
            data_.remove_if([&](const auto& pair) { return pair.first == key; });
        }
        
    private:
        mutable std::shared_mutex mutex_;
        std::list<std::pair<Key, Value>> data_;
    };
    
    Bucket& getBucket(const Key& key) const {
        const std::size_t index = hasher_(key) % buckets_.size();
        return *buckets_[index];
    }
    
    std::vector<std::unique_ptr<Bucket>> buckets_;
    Hash hasher_;
};
```

---

## 7. 并发设计模式

### 7.1 生产者-消费者模式

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <optional>
#include <queue>
#include <stdexcept>
#include <vector>

template<typename T>
class BoundedBuffer {
public:
    explicit BoundedBuffer(std::size_t capacity) : capacity_(capacity) {
        if (capacity_ == 0U) {
            throw std::invalid_argument("buffer capacity must be positive");
        }
    }
    
    bool put(T item) {
        std::unique_lock<std::mutex> lock(mutex_);
        notFull_.wait(lock, [this] {
            return closed_ || buffer_.size() < capacity_;
        });
        if (closed_) {
            return false;
        }
        
        buffer_.push(std::move(item));
        lock.unlock();
        notEmpty_.notify_one();
        return true;
    }
    
    std::optional<T> take() {
        std::unique_lock<std::mutex> lock(mutex_);
        notEmpty_.wait(lock, [this] { return closed_ || !buffer_.empty(); });
        if (buffer_.empty()) {
            return std::nullopt;
        }
        T item = std::move(buffer_.front());
        buffer_.pop();
        lock.unlock();
        notFull_.notify_one();
        return item;
    }

    void close() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            closed_ = true;
        }
        notEmpty_.notify_all();
        notFull_.notify_all();
    }
    
private:
    std::mutex mutex_;
    std::condition_variable notEmpty_;
    std::condition_variable notFull_;
    std::queue<T> buffer_;
    const std::size_t capacity_;
    bool closed_{false};
};

void producerConsumerDemo() {
    BoundedBuffer<int> buffer(10);
    
    // 先启动消费者；如果生产者创建失败，close 会解除消费者等待。
    std::thread consumer([&] {
        while (std::optional<int> item = buffer.take()) {
            std::cout << "Consumed: " << *item << "\n";
        }
    });

    try {
        std::thread producer([&] {
            for (int i = 0; i < 100; ++i) {
                if (!buffer.put(i)) {
                    break;
                }
                std::cout << "Produced: " << i << "\n";
            }
            buffer.close();
        });

        producer.join();
        consumer.join();
    } catch (...) {
        buffer.close();
        consumer.join();
        throw;
    }
}
```

有界缓冲区的停止协议必须同时唤醒两类等待者：close 后生产者从 `put` 得到拒绝，消费者继续 drain 已有元素，队列清空后 `take` 返回 `nullopt`。外部拥有者负责 close、join 所有调用者、最后销毁缓冲区；析构本身不能与正在访问成员的线程并发。

### 7.2 工作队列模式

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <vector>
#include <atomic>
#include <cassert>
#include <cstddef>
#include <exception>
#include <stdexcept>
#include <utility>

class ThreadPool {
public:
    explicit ThreadPool(std::size_t numThreads)
        : owner_(std::this_thread::get_id()) {
        if (numThreads == 0) {
            throw std::invalid_argument("thread pool needs at least one worker");
        }
        try {
            for (size_t i = 0; i < numThreads; ++i) {
                workers_.emplace_back([this]() {
                    while (true) {
                        std::function<void()> task;
                    
                        {
                            std::unique_lock<std::mutex> lock(mutex_);
                            condition_.wait(lock, [this]() {
                                return closed_ || !tasks_.empty();
                            });
                        
                            if (closed_ && tasks_.empty()) {
                                return;
                            }
                        
                            task = std::move(tasks_.front());
                            tasks_.pop();
                        }
                    
                        try {
                            task();
                        } catch (const std::exception& error) {
                            std::cerr << "fire-and-forget task failed: " << error.what() << "\n";
                        } catch (...) {
                            std::cerr << "fire-and-forget task failed with unknown exception\n";
                        }
                    }
                });
            }
        } catch (...) {
            {
                std::lock_guard<std::mutex> lock(mutex_);
                closed_ = true;
            }
            condition_.notify_all();
            for (auto& worker : workers_) {
                if (worker.joinable()) {
                    worker.join();
                }
            }
            throw;
        }
    }
    
    ~ThreadPool() {
        if (std::this_thread::get_id() != owner_) {
            std::terminate();
        }
        shutdown();
    }

    void close() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (closed_) {
                return;
            }
            closed_ = true;
        }
        condition_.notify_all();
    }

    void shutdown() {
        requireOwner();
        close();
        
        for (auto& worker : workers_) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }
    
    template<typename F>
    bool enqueue(F&& task) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (closed_) {
                return false;
            }
            tasks_.emplace(std::forward<F>(task));
        }
        condition_.notify_one();
        return true;
    }
    
private:
    void requireOwner() const {
        if (std::this_thread::get_id() != owner_) {
            throw std::logic_error("only the pool owner may join workers");
        }
    }

    const std::thread::id owner_;
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex mutex_;
    std::condition_variable condition_;
    bool closed_{false};
};

void threadPoolDemo() {
    ThreadPool pool(4);
    std::atomic<int> completed{0};
    
    for (int i = 0; i < 8; ++i) {
        const bool accepted = pool.enqueue([i, &completed]() {
            std::cout << "Task " << i << " executed by thread "
                      << std::this_thread::get_id() << "\n";
            completed.fetch_add(1, std::memory_order_relaxed);
        });
        assert(accepted);
    }
    
    pool.close();     // 原子切换为拒绝新任务，worker 继续排空队列。
    pool.shutdown();  // 只有创建池的 owner 可以 join worker。
    assert(completed.load(std::memory_order_relaxed) == 8);
    assert(!pool.enqueue([] {}));  // 停止后必须拒绝新任务
}
```

这个教学池选择 close-reject-drain 语义：`close()` 与任务入队在同一 mutex 下线性化，关闭后拒绝新任务，已经接受的任务必须执行完，随后 owner 用 `shutdown()` join worker。`shutdown()` 不能从 worker 内调用，否则会自连接，也不能由多个线程并发 join；示例用创建线程 ID 强制单 owner，若对象在 worker 或其他线程析构则终止程序以暴露所有权违约。worker 捕获 fire-and-forget 异常以避免异常逃逸导致 `std::terminate`，但调用者只能看到日志；需要返回值和异常传播时使用下一节的 `packaged_task`/`future` 版本。

### 7.3 Actor 模式

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <memory>
#include <future>
#include <cassert>
#include <exception>
#include <stdexcept>

class Actor {
public:
    Actor() : state_(std::make_shared<State>()) {
        thread_ = std::thread([state = state_] {
            while (true) {
                std::function<void()> message;
                
                {
                    std::unique_lock<std::mutex> lock(state->mutex);
                    state->condition.wait(lock, [&state] {
                        return state->closed || !state->mailbox.empty();
                    });
                    
                    if (state->closed && state->mailbox.empty()) {
                        return;
                    }
                    
                    message = std::move(state->mailbox.front());
                    state->mailbox.pop();
                }
                
                try {
                    message();
                } catch (const std::exception& error) {
                    std::cerr << "actor message failed: " << error.what() << "\n";
                } catch (...) {
                    std::cerr << "actor message failed with unknown exception\n";
                }
            }
        });
    }
    
    ~Actor() {
        close();
        if (!thread_.joinable()) {
            return;
        }
        if (std::this_thread::get_id() == thread_.get_id()) {
            // worker 只捕获共享 State，不访问已销毁的 Actor，因此这里可安全分离。
            thread_.detach();
        } else {
            thread_.join();
        }
    }

    Actor(const Actor&) = delete;
    Actor& operator=(const Actor&) = delete;

    bool send(std::function<void()> message) {
        {
            std::lock_guard<std::mutex> lock(state_->mutex);
            if (state_->closed) {
                return false;
            }
            state_->mailbox.push(std::move(message));
        }
        state_->condition.notify_one();
        return true;
    }

    void close() {
        {
            std::lock_guard<std::mutex> lock(state_->mutex);
            state_->closed = true;
        }
        state_->condition.notify_all();
    }

    void join() {
        if (std::this_thread::get_id() == thread_.get_id()) {
            throw std::logic_error("actor cannot join itself");
        }
        close();
        if (thread_.joinable()) {
            thread_.join();
        }
    }

private:
    struct State {
        std::mutex mutex;
        std::condition_variable condition;
        std::queue<std::function<void()>> mailbox;
        bool closed{false};
    };

    std::shared_ptr<State> state_;
    std::thread thread_;
};

void actorDemo() {
    auto actor = std::make_unique<Actor>();
    std::promise<void> all_prior_messages_done;
    auto completion = all_prior_messages_done.get_future();

    assert(actor->send([]() { std::cout << "Message 1\n"; }));
    assert(actor->send([]() { std::cout << "Message 2\n"; }));
    assert(actor->send([&all_prior_messages_done]() {
        std::cout << "Message 3\n";
        all_prior_messages_done.set_value();
    }));
    actor->close();
    assert(!actor->send([] {}));
    completion.get();  // close 后仍 drain 已接受的三条 FIFO 消息。
    actor->join();
}
```

Actor 的邮箱同样是 close-reject-drain：`send` 与 `close` 在同一 mutex 下决定先后，close 后拒绝新消息，worker 排空旧消息再退出。正常路径应由唯一外部 owner 执行 close/join，多个线程不能并发 join；为处理“最后一个 Actor 所有者恰好在消息中释放”的自销毁边界，worker 只持有独立共享状态，析构在 worker 线程发生时 detach 句柄，让状态活到 drain 完成，而不会再访问已经销毁的 Actor 对象。

---

## 8. 线程池实现

### 8.1 完整的线程池

```cpp
#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <functional>
#include <future>
#include <memory>
#include <cstddef>
#include <exception>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <utility>

class ThreadPool {
public:
    explicit ThreadPool(std::size_t numThreads =
                        std::thread::hardware_concurrency())
        : owner_(std::this_thread::get_id()) {
        if (numThreads == 0) {
            numThreads = 1;  // hardware_concurrency() 允许返回 0
        }
        try {
            for (std::size_t i = 0; i < numThreads; ++i) {
                workers_.emplace_back([this] {
                    workerThread();
                });
            }
        } catch (...) {
            {
                std::lock_guard<std::mutex> lock(mutex_);
                closed_ = true;
            }
            condition_.notify_all();
            for (auto& worker : workers_) {
                if (worker.joinable()) {
                    worker.join();
                }
            }
            throw;
        }
    }
    
    ~ThreadPool() {
        if (std::this_thread::get_id() != owner_) {
            std::terminate();
        }
        shutdown();
    }

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    
    // 提交任务，返回 future
    template<typename F, typename... Args>
    auto submit(F&& f, Args&&... args) 
        -> std::future<std::invoke_result_t<std::decay_t<F>,
                                            std::decay_t<Args>...>>
    {
        using StoredFunction = std::decay_t<F>;
        using StoredArguments = std::tuple<std::decay_t<Args>...>;
        using ReturnType =
            std::invoke_result_t<StoredFunction, std::decay_t<Args>...>;

        std::future<ReturnType> result;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (closed_) {
                throw std::runtime_error("ThreadPool has been stopped");
            }

            // 关闭检查与入队在同一临界区线性化。只有确认接受后才衰减并移动
            // 参数，因此被拒绝的 move-only 实参仍归调用者所有。
            auto invocation =
                [callable = StoredFunction(std::forward<F>(f)),
                 arguments = StoredArguments(std::forward<Args>(args)...)]() mutable
                -> ReturnType {
                return std::apply(
                    [&callable](auto&&... unpacked) mutable -> ReturnType {
                        return std::invoke(
                            std::move(callable),
                            std::forward<decltype(unpacked)>(unpacked)...);
                    },
                    std::move(arguments));
            };
            auto task = std::make_shared<std::packaged_task<ReturnType()>>(
                std::move(invocation));
            result = task->get_future();
            tasks_.emplace([task]() { (*task)(); });
        }
        
        condition_.notify_one();
        return result;
    }
    
    void close() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (closed_) {
                return;
            }
            closed_ = true;
        }
        condition_.notify_all();
    }

    void shutdown() {
        requireOwner();
        close();
        
        for (std::thread& worker : workers_) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }
    
    std::size_t size() const { return workers_.size(); }
    
private:
    void workerThread() {
        while (true) {
            std::function<void()> task;
            
            {
                std::unique_lock<std::mutex> lock(mutex_);
                condition_.wait(lock, [this]() {
                    return closed_ || !tasks_.empty();
                });
                
                if (closed_ && tasks_.empty()) {
                    return;
                }
                
                task = std::move(tasks_.front());
                tasks_.pop();
            }
            
            task();
        }
    }
    
    void requireOwner() const {
        if (std::this_thread::get_id() != owner_) {
            throw std::logic_error("only the pool owner may join workers");
        }
    }

    const std::thread::id owner_;
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex mutex_;
    std::condition_variable condition_;
    bool closed_{false};
};

void threadPoolExample() {
    ThreadPool pool(4);
    
    // 提交多个任务
    std::vector<std::future<int>> results;
    
    for (int i = 0; i < 10; ++i) {
        results.push_back(pool.submit([i]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            return i * i;
        }));
    }

    auto owned = std::make_unique<int>(21);
    auto movedResult = pool.submit(
        [](std::unique_ptr<int> value) { return *value * 2; },
        std::move(owned));
    assert(owned == nullptr);  // 接受任务后，所有权进入任务队列。

    pool.close();  // 后续 submit 必须拒绝，已接受任务继续 drain。
    
    // 获取结果
    for (auto& result : results) {
        std::cout << "Result: " << result.get() << "\n";
    }
    assert(movedResult.get() == 42);

    auto rejectedValue = std::make_unique<int>(7);
    bool rejected = false;
    try {
        static_cast<void>(pool.submit(
            [](std::unique_ptr<int> value) { return *value; },
            std::move(rejectedValue)));
    } catch (const std::runtime_error&) {
        rejected = true;
    }
    assert(rejected);
    assert(rejectedValue != nullptr && *rejectedValue == 7);
    pool.shutdown();
}
```

停止契约同样是 close-reject-drain：`submit` 与 `close` 在同一 mutex 下线性化，关闭后抛异常拒绝新任务，已接受任务由 `packaged_task` 保存结果或异常并排空。示例把关闭检查放在衰减复制/移动参数之前，所以“接受”才转移 move-only 所有权，“拒绝”不会暗中消费调用者对象；`std::ref` 则表示调用者主动借用，调用者必须保证被引用对象活到 future 就绪且没有无同步冲突访问。池的创建线程是唯一 join owner；worker 可以请求 close，但不得调用 shutdown、销毁池或与 owner 并发 join，否则会自连接或让 worker 访问已销毁的 `this`。生产系统还需要有界队列、背压、取消、可观测错误和明确的 owner 移交协议。

---

## 9. 协程（C++20）

### 9.1 协程基础概念

```cpp
#include <coroutine>
#include <exception>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <utility>

// C++20 协程需要三个核心组件：
// 1. promise_type：控制协程行为
// 2. coroutine_handle：操作协程
// 3. awaitable：控制等待行为

// 简单的生成器协程
template<typename T>
class Generator {
public:
    struct promise_type {
        std::optional<T> current;
        std::exception_ptr exception;
        
        Generator get_return_object() {
            return Generator{
                std::coroutine_handle<promise_type>::from_promise(*this)
            };
        }
        
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        
        void return_void() noexcept {}
        
        template<typename U>
        std::suspend_always yield_value(U&& value) {
            current.emplace(std::forward<U>(value));
            return {};
        }
        
        void unhandled_exception() noexcept {
            exception = std::current_exception();
        }
    };
    
    explicit Generator(std::coroutine_handle<promise_type> handle)
        : handle_(handle) {}

    Generator(const Generator&) = delete;
    Generator& operator=(const Generator&) = delete;

    Generator(Generator&& other) noexcept
        : handle_(std::exchange(other.handle_, {})) {}

    Generator& operator=(Generator&& other) noexcept {
        if (this != &other) {
            if (handle_) {
                handle_.destroy();
            }
            handle_ = std::exchange(other.handle_, {});
        }
        return *this;
    }
    
    ~Generator() {
        if (handle_) {
            handle_.destroy();
        }
    }
    
    // 迭代器接口
    bool next() {
        if (!handle_ || handle_.done()) {
            return false;
        }
        handle_.promise().current.reset();
        handle_.resume();
        if (handle_.promise().exception) {
            std::rethrow_exception(handle_.promise().exception);
        }
        return !handle_.done();
    }
    
    const T& value() const {
        if (!handle_ || !handle_.promise().current) {
            throw std::logic_error("Generator has no current value");
        }
        return *handle_.promise().current;
    }
    
private:
    std::coroutine_handle<promise_type> handle_;
};

// 使用协程生成斐波那契数列
Generator<int> fibonacci(int n) {
    int a = 0, b = 1;
    for (int i = 0; i < n; ++i) {
        co_yield a;
        const int temp = a;
        a = b;
        b = temp + b;
    }
}

void coroutineDemo() {
    auto gen = fibonacci(10);
    while (gen.next()) {
        std::cout << gen.value() << " ";
    }
    std::cout << "\n";
}
```

Generator 必须是 move-only，否则两个副本会对同一 coroutine handle 调用 `destroy()`；`optional` 避免要求 T 可默认构造，并明确“尚未产生值”。`next()` 在恢复前检查空句柄和 `done()`，在恢复后重新抛出协程异常；`value()` 只能在一次成功的 `next()` 之后、下一次 `next()` 之前调用。

### 9.2 异步任务协程

C++20 标准库的 `std::future` 没有 `await_ready/await_suspend/await_resume`，因此不能直接 `co_await future`。下面是一个完整可编译的局部模型：`Task<T>` 只支持可放入 `optional` 的非 `void` 对象结果，是惰性、move-only、单消费者任务，并且只组合其他 `Task<T>`；`Task<void>`、引用结果、取消、超时和通用执行器都需要额外设计，真正项目应使用明确提供 future 适配器与生命周期管理的协程库。示例在最外层用 `std::async` 把整条协程链调度到后台线程，而不是伪装成可直接等待 future。

```cpp
#include <coroutine>
#include <exception>
#include <future>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <utility>

template<typename T>
class Task {
public:
    struct promise_type {
        std::optional<T> value;
        std::exception_ptr exception;
        std::coroutine_handle<> continuation{std::noop_coroutine()};
        
        Task get_return_object() {
            return Task{
                std::coroutine_handle<promise_type>::from_promise(*this)
            };
        }
        
        std::suspend_always initial_suspend() noexcept { return {}; }
        
        struct FinalAwaiter {
            bool await_ready() const noexcept { return false; }

            std::coroutine_handle<> await_suspend(
                std::coroutine_handle<promise_type> handle) const noexcept {
                return handle.promise().continuation;
            }

            void await_resume() const noexcept {}
        };

        FinalAwaiter final_suspend() noexcept { return {}; }

        template<typename U>
        void return_value(U&& result) {
            value.emplace(std::forward<U>(result));
        }

        void unhandled_exception() noexcept {
            exception = std::current_exception();
        }

        T takeResult() {
            if (exception) {
                std::rethrow_exception(exception);
            }
            if (!value) {
                throw std::logic_error("Task has no result");
            }
            T result = std::move(*value);
            value.reset();
            return result;
        }
    };

    Task(const Task&) = delete;
    Task& operator=(const Task&) = delete;

    Task(Task&& other) noexcept
        : handle_(std::exchange(other.handle_, {})) {}

    Task& operator=(Task&& other) noexcept {
        if (this != &other) {
            if (handle_) {
                handle_.destroy();
            }
            handle_ = std::exchange(other.handle_, {});
        }
        return *this;
    }

    ~Task() {
        if (handle_) {
            handle_.destroy();
        }
    }

    class Awaiter {
    public:
        explicit Awaiter(std::coroutine_handle<promise_type> handle)
            : handle_(handle) {}

        Awaiter(const Awaiter&) = delete;
        Awaiter& operator=(const Awaiter&) = delete;

        Awaiter(Awaiter&& other) noexcept
            : handle_(std::exchange(other.handle_, {})) {}

        ~Awaiter() {
            if (handle_) {
                handle_.destroy();
            }
        }

        bool await_ready() const noexcept {
            return !handle_ || handle_.done();
        }

        std::coroutine_handle<> await_suspend(
            std::coroutine_handle<> continuation) noexcept {
            handle_.promise().continuation = continuation;
            return handle_;
        }

        T await_resume() {
            if (!handle_) {
                throw std::logic_error("awaiting an empty Task");
            }
            return handle_.promise().takeResult();
        }

    private:
        std::coroutine_handle<promise_type> handle_;
    };

    Awaiter operator co_await() && noexcept {
        return Awaiter(std::exchange(handle_, {}));
    }

    T get() {
        if (!handle_) {
            throw std::logic_error("getting an empty Task");
        }
        while (!handle_.done()) {
            handle_.resume();
        }
        return handle_.promise().takeResult();
    }

private:
    std::coroutine_handle<promise_type> handle_;

    explicit Task(std::coroutine_handle<promise_type> handle)
        : handle_(handle) {}
};

Task<int> computeTask(int value) {
    co_return value * 2;
}

Task<int> combinedTask() {
    const int a = co_await computeTask(10);
    const int b = co_await computeTask(20);
    co_return a + b;
}

void coroutineTaskDemo() {
    std::future<int> background = std::async(std::launch::async, [] {
        return combinedTask().get();
    });
    std::cout << "Combined result: " << background.get() << '\n';
}
```

---

## 参考资料

1. **Anthony Williams, _C++ Concurrency in Action_**：线程、同步、内存模型和并发组件设计。
2. **C++ working draft**：https://eel.is/c++draft/
3. **cppreference 并发支持**：https://en.cppreference.com/w/cpp/thread
4. **cppreference C++20 协程**：https://en.cppreference.com/w/cpp/language/coroutines
5. **ISO C++ Core Guidelines 并发规则**：https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-concurrency

---

## 学习建议

1. **先管生命周期**：任何线程句柄离开作用域前都要有明确的 join、转移或受控 detach 设计。
2. **再证明同步**：用互斥不变量、条件变量谓词和 happens-before 说明正确性。
3. **选择合适抽象**：`async`、线程池、队列或专用线程各有启动、异常、取消和关闭成本，不存在无条件的“最高层就最好”。
4. **确定性验证**：测试事件和最终不变量，不用 `sleep`、输出顺序或固定毫秒数证明协议。
5. **最后优化**：正确版本通过压力测试和工具检查后，再测量锁竞争、任务粒度、缓存和调度成本。

---

*本教程涵盖 C++11 到 C++20 的并发编程特性，适合有 C++ 基础的开发者学习多线程编程。*
