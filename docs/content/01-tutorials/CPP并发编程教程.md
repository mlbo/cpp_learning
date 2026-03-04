# C++ 并发编程教程

> 本教程系统讲解 C++11/14/17/20 并发编程，从基础概念到实战应用，帮助开发者掌握多线程编程技术。

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

### 1.2 为什么需要并发

**性能提升**：
- 利用多核处理器
- 提高资源利用率
- 处理阻塞操作（I/O、网络请求）

**架构优势**：
- 分离关注点
- 更好的响应性
- 简化异步处理

### 1.3 并发编程的挑战

```cpp
// 经典问题：数据竞争

// 错误示例：多线程修改共享变量
int counter = 0;

void increment() {
    for (int i = 0; i < 100000; ++i) {
        counter++;  // 非原子操作！
    }
}

int main() {
    std::thread t1(increment);
    std::thread t2(increment);
    
    t1.join();
    t2.join();
    
    // counter 可能小于 200000
    std::cout << counter << "\n";
    return 0;
}
```

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
#include <thread>
#include <string>

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
    // 方式1：函数指针
    std::thread t1(threadFunction, 1);
    
    // 方式2：Lambda
    std::thread t2([]() {
        std::cout << "Lambda thread\n";
    });
    
    // 方式3：仿函数
    std::thread t3(ThreadFunctor(), 3);
    // 注意：std::thread t3(ThreadFunctor()); 是函数声明！
    
    // 方式4：成员函数
    Worker worker;
    std::thread t4(&Worker::doWork, &worker, 5);
    
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    
    return 0;
}
```

### 2.2 线程的生命周期管理

```cpp
#include <iostream>
#include <thread>
#include <stdexcept>

class ThreadGuard {
public:
    explicit ThreadGuard(std::thread& t) : thread_(t) {}
    
    ~ThreadGuard() {
        if (thread_.joinable()) {
            thread_.join();  // 确保线程在析构前被 join
        }
    }
    
    // 禁止拷贝
    ThreadGuard(const ThreadGuard&) = delete;
    ThreadGuard& operator=(const ThreadGuard&) = delete;
    
private:
    std::thread& thread_;
};

void threadLifecycle() {
    std::thread t([]() {
        std::cout << "Worker thread\n";
    });
    
    ThreadGuard guard(t);
    
    // 即使这里抛出异常，ThreadGuard 也会确保 t.join() 被调用
    // throw std::runtime_error("Error!");
    
}  // ThreadGuard 析构，自动 join

int main() {
    threadLifecycle();
    return 0;
}
```

### 2.3 向线程传递参数

```cpp
#include <iostream>
#include <thread>
#include <string>
#include <memory>

void processString(std::string str) {
    std::cout << "Processing: " << str << "\n";
}

void processPointer(std::unique_ptr<int> ptr) {
    std::cout << "Value: " << *ptr << "\n";
}

void passArguments() {
    std::string s = "Hello";
    
    // 参数会被拷贝到线程的存储空间
    std::thread t1(processString, s);
    
    // 避免拷贝（需要确保 s 在线程执行期间有效）
    std::thread t2(processString, std::cref(s));
    
    // 移动语义
    std::thread t3(processString, std::move(s));
    // s 现在为空或未定义
    
    // 移动 unique_ptr
    auto ptr = std::make_unique<int>(42);
    std::thread t4(processPointer, std::move(ptr));
    
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}

int main() {
    passArguments();
    return 0;
}
```

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
    threads.emplace_back([]() { std::cout << "Thread 1\n"; });
    threads.emplace_back([]() { std::cout << "Thread 2\n"; });
    
    for (auto& t : threads) {
        t.join();
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
#include <hardware>

void hardwareInfo() {
    // 获取硬件并发能力
    unsigned int n = std::thread::hardware_concurrency();
    std::cout << "Hardware concurrency: " << n << " threads\n";
    
    // 用于确定线程池大小
    unsigned int threadCount = std::max(1u, n - 1);  // 保留一个核心给主线程
    
    // 注意：hardware_concurrency() 可能返回 0（无法确定）
}
```

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
    std::thread t2(printThreadId);
    
    std::cout << "Main thread ID: " << masterId << "\n";
    std::cout << "t1 ID: " << t1.get_id() << "\n";
    
    // 线程 ID 可以用于比较和作为容器键
    std::thread::id nullId;  // 默认构造，表示"无线程"
    
    t1.join();
    t2.join();
}
```

---

## 3. 线程间共享数据

### 3.1 使用互斥量保护共享数据

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

class ThreadSafeCounter {
public:
    void increment() {
        std::lock_guard<std::mutex> lock(mutex_);
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
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([&counter]() {
            for (int j = 0; j < 1000; ++j) {
                counter.increment();
            }
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    std::cout << "Final value: " << counter.get() << "\n";  // 10000
}
```

### 3.2 死锁问题与解决

```cpp
#include <iostream>
#include <thread>
#include <mutex>

class BankAccount {
public:
    BankAccount(int balance) : balance_(balance) {}
    
    void transfer(BankAccount& to, int amount) {
        // 危险：可能死锁
        std::lock_guard<std::mutex> lock1(mutex_);
        std::lock_guard<std::mutex> lock2(to.mutex_);
        
        if (balance_ >= amount) {
            balance_ -= amount;
            to.balance_ += amount;
        }
    }
    
    void transferSafe(BankAccount& to, int amount) {
        // 安全：使用 std::lock 同时锁定多个互斥量
        std::unique_lock<std::mutex> lock1(mutex_, std::defer_lock);
        std::unique_lock<std::mutex> lock2(to.mutex_, std::defer_lock);
        std::lock(lock1, lock2);  // 原子锁定，避免死锁
        
        if (balance_ >= amount) {
            balance_ -= amount;
            to.balance_ += amount;
        }
    }
    
    // C++17: std::scoped_lock
    void transferCpp17(BankAccount& to, int amount) {
        std::scoped_lock lock(mutex_, to.mutex_);  // 自动处理死锁
        
        if (balance_ >= amount) {
            balance_ -= amount;
            to.balance_ += amount;
        }
    }
    
private:
    mutable std::mutex mutex_;
    int balance_;
};

void deadlockDemo() {
    BankAccount a(1000);
    BankAccount b(1000);
    
    // 可能死锁的场景
    std::thread t1([&]() { a.transfer(b, 100); });
    std::thread t2([&]() { b.transfer(a, 100); });
    
    t1.join();
    t2.join();
}
```

### 3.3 避免死锁的准则

```cpp
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
    int data_;
};

// 准则3：按固定顺序获取锁
class HierarchicalMutex {
public:
    explicit HierarchicalMutex(int hierarchyValue) 
        : hierarchyValue_(hierarchyValue) {}
    
    void lock() {
        // 检查层级约束
        if (currentHierarchy >= hierarchyValue_) {
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
    int previousHierarchy_;
    static thread_local int currentHierarchy;
};
```

### 3.4 std::unique_lock 的灵活性

```cpp
#include <iostream>
#include <thread>
#include <mutex>

void uniqueLockDemo() {
    std::mutex m;
    
    // 延迟锁定
    std::unique_lock<std::mutex> lock1(m, std::defer_lock);
    // ... 做一些不需要锁的操作 ...
    lock1.lock();  // 现在才锁定
    
    // 尝试锁定（非阻塞）
    std::unique_lock<std::mutex> lock2(m, std::try_to_lock);
    if (lock2.owns_lock()) {
        // 成功获取锁
    } else {
        // 获取失败
    }
    
    // 定时尝试锁定
    std::unique_lock<std::mutex> lock3(m, std::defer_lock);
    if (lock3.try_lock_for(std::chrono::milliseconds(100))) {
        // 在100ms内成功获取锁
    }
    
    // 提前解锁
    lock1.unlock();
    // ... 做一些不需要锁的操作 ...
    lock1.lock();  // 再次锁定
    
    // 转移所有权
    std::unique_lock<std::mutex> lock4 = std::move(lock1);
}
```

---

## 4. 同步操作

### 4.1 条件变量

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

template<typename T>
class ThreadSafeQueue {
public:
    void push(T value) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(std::move(value));
        cond_.notify_one();  // 通知一个等待线程
    }
    
    T pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        
        // 等待队列非空（使用 lambda 避免虚假唤醒）
        cond_.wait(lock, [this]() { return !queue_.empty(); });
        
        T value = std::move(queue_.front());
        queue_.pop();
        return value;
    }
    
    bool tryPop(T& value, std::chrono::milliseconds timeout) {
        std::unique_lock<std::mutex> lock(mutex_);
        
        if (!cond_.wait_for(lock, timeout, [this]() { return !queue_.empty(); })) {
            return false;  // 超时
        }
        
        value = std::move(queue_.front());
        queue_.pop();
        return true;
    }
    
private:
    std::mutex mutex_;
    std::condition_variable cond_;
    std::queue<T> queue_;
};

void producerConsumer() {
    ThreadSafeQueue<int> queue;
    
    // 生产者
    std::thread producer([&]() {
        for (int i = 0; i < 10; ++i) {
            queue.push(i);
            std::cout << "Produced: " << i << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });
    
    // 消费者
    std::thread consumer([&]() {
        for (int i = 0; i < 10; ++i) {
            int value = queue.pop();
            std::cout << "Consumed: " << value << "\n";
        }
    });
    
    producer.join();
    consumer.join();
}
```

### 4.2 信号量（C++20）

```cpp
#include <iostream>
#include <thread>
#include <semaphore>
#include <vector>

// C++20 信号量
std::counting_semaphore<5> semaphore(3);  // 最多 3 个线程同时访问

void limitedAccess(int id) {
    semaphore.acquire();  // 获取许可
    std::cout << "Thread " << id << " accessing resource\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    semaphore.release();  // 释放许可
}

void semaphoreDemo() {
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back(limitedAccess, i);
    }
    
    for (auto& t : threads) {
        t.join();
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
#include <thread>
#include <barrier>
#include <vector>

void barrierDemo() {
    const int numThreads = 4;
    
    // C++20 barrier
    std::barrier syncPoint(numThreads, []() noexcept {
        std::cout << "--- All threads reached barrier ---\n";
    });
    
    auto worker = [&](int id) {
        for (int phase = 0; phase < 3; ++phase) {
            std::cout << "Thread " << id << " phase " << phase << "\n";
            syncPoint.arrive_and_wait();  // 等待所有线程到达
        }
    };
    
    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(worker, i);
    }
    
    for (auto& t : threads) {
        t.join();
    }
}
```

### 4.4 Future 与 Promise

```cpp
#include <iostream>
#include <thread>
#include <future>
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
    
    // 主线程获取值（会阻塞直到值就绪）
    int value = future.get();  // 等待并获取值
    std::cout << "Got value: " << value << "\n";
    
    producer.join();
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
    
    try {
        int value = future.get();
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << "\n";
    }
    
    t.join();
}

void sharedFuture() {
    // shared_future 可以被多个线程等待
    std::promise<int> promise;
    std::shared_future<int> sf = promise.get_future().share();
    
    std::vector<std::thread> threads;
    for (int i = 0; i < 3; ++i) {
        threads.emplace_back([sf, i]() {
            int value = sf.get();  // 每个线程都可以获取值
            std::cout << "Thread " << i << ": " << value << "\n";
        });
    }
    
    promise.set_value(100);
    
    for (auto& t : threads) {
        t.join();
    }
}
```

### 4.5 std::async

```cpp
#include <iostream>
#include <future>
#include <vector>
#include <numeric>

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

---

## 5. 原子操作与内存模型

### 5.1 原子类型

```cpp
#include <iostream>
#include <thread>
#include <atomic>
#include <vector>

// 原子变量：无锁的线程安全操作
std::atomic<int> atomicCounter(0);

void atomicIncrement() {
    for (int i = 0; i < 100000; ++i) {
        atomicCounter.fetch_add(1, std::memory_order_relaxed);
    }
}

void atomicDemo() {
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back(atomicIncrement);
    }
    
    for (auto& t : threads) {
        t.join();
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
    int v = a.load();      // 加载
    int old = a.exchange(30);  // 交换并返回旧值
    
    // 比较交换（CAS: Compare-And-Swap）
    int expected = 30;
    bool success = a.compare_exchange_strong(expected, 40);
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
    int* p = ptr.load();
    
    // CAS 实现 无锁栈
    struct Node {
        int data;
        Node* next;
    };
    
    std::atomic<Node*> head(nullptr);
    
    auto push = [&head](int data) {
        Node* newNode = new Node{data, head.load()};
        while (!head.compare_exchange_weak(newNode->next, newNode)) {
            // 循环直到成功
        }
    };
}
```

### 5.3 内存顺序

```cpp
#include <iostream>
#include <thread>
#include <atomic>

/*
内存顺序（Memory Order）：
- memory_order_relaxed: 只保证原子性，无顺序约束
- memory_order_acquire: 读操作，之后的读写不能重排到此操作之前
- memory_order_release: 写操作，之前的读写不能重排到此操作之后
- memory_order_acq_rel: 同时具有 acquire 和 release 语义
- memory_order_seq_cst: 顺序一致性（默认）
*/

std::atomic<bool> ready(false);
std::atomic<int> data(0);

void producer() {
    data.store(42, std::memory_order_relaxed);
    ready.store(true, std::memory_order_release);  // 释放：确保上面的写可见
}

void consumer() {
    while (!ready.load(std::memory_order_acquire)) {  // 获取：确保看到上面的写
        std::this_thread::yield();
    }
    std::cout << "Data: " << data.load(std::memory_order_relaxed) << "\n";  // 保证是 42
}

void memoryOrderDemo() {
    std::thread t1(producer);
    std::thread t2(consumer);
    
    t1.join();
    t2.join();
}

// 双重检查锁定
class Singleton {
public:
    static Singleton* getInstance() {
        Singleton* instance = instance_.load(std::memory_order_acquire);
        if (!instance) {
            std::lock_guard<std::mutex> lock(mutex_);
            instance = instance_.load(std::memory_order_relaxed);
            if (!instance) {
                instance = new Singleton();
                instance_.store(instance, std::memory_order_release);
            }
        }
        return instance;
    }
    
private:
    Singleton() = default;
    static std::atomic<Singleton*> instance_;
    static std::mutex mutex_;
};
```

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
#include <queue>
#include <memory>

template<typename T>
class ThreadSafeQueue {
public:
    ThreadSafeQueue() = default;
    
    ThreadSafeQueue(const ThreadSafeQueue& other) {
        std::lock_guard<std::mutex> lock(other.mutex_);
        queue_ = other.queue_;
    }
    
    void push(T value) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            queue_.push(std::move(value));
        }
        cond_.notify_one();
    }
    
    void waitAndPop(T& value) {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [this]() { return !queue_.empty(); });
        value = std::move(queue_.front());
        queue_.pop();
    }
    
    std::shared_ptr<T> waitAndPop() {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [this]() { return !queue_.empty(); });
        auto result = std::make_shared<T>(std::move(queue_.front()));
        queue_.pop();
        return result;
    }
    
    bool tryPop(T& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) {
            return false;
        }
        value = std::move(queue_.front());
        queue_.pop();
        return true;
    }
    
    std::shared_ptr<T> tryPop() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) {
            return nullptr;
        }
        auto result = std::make_shared<T>(std::move(queue_.front()));
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
};
```

### 6.3 线程安全查找表

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <list>
#include <vector>
#include <algorithm>
#include <memory>

template<typename Key, typename Value, typename Hash = std::hash<Key>>
class ThreadSafeLookupTable {
public:
    ThreadSafeLookupTable(
        size_t numBuckets = 19, 
        const Hash& hasher = Hash()
    ) : buckets_(numBuckets), hasher_(hasher) {
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
        size_t index = hasher_(key) % buckets_.size();
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
#include <queue>
#include <vector>

template<typename T>
class BoundedBuffer {
public:
    explicit BoundedBuffer(size_t capacity) : capacity_(capacity) {}
    
    void put(T item) {
        std::unique_lock<std::mutex> lock(mutex_);
        notFull_.wait(lock, [this]() { return buffer_.size() < capacity_; });
        
        buffer_.push(std::move(item));
        notEmpty_.notify_one();
    }
    
    T take() {
        std::unique_lock<std::mutex> lock(mutex_);
        notEmpty_.wait(lock, [this]() { return !buffer_.empty(); });
        
        T item = std::move(buffer_.front());
        buffer_.pop();
        notFull_.notify_one();
        return item;
    }
    
private:
    std::mutex mutex_;
    std::condition_variable notEmpty_;
    std::condition_variable notFull_;
    std::queue<T> buffer_;
    size_t capacity_;
};

void producerConsumerDemo() {
    BoundedBuffer<int> buffer(10);
    
    // 生产者
    std::thread producer([&]() {
        for (int i = 0; i < 100; ++i) {
            buffer.put(i);
            std::cout << "Produced: " << i << "\n";
        }
    });
    
    // 消费者
    std::thread consumer([&]() {
        for (int i = 0; i < 100; ++i) {
            int item = buffer.take();
            std::cout << "Consumed: " << item << "\n";
        }
    });
    
    producer.join();
    consumer.join();
}
```

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

class ThreadPool {
public:
    explicit ThreadPool(size_t numThreads) : stop_(false) {
        for (size_t i = 0; i < numThreads; ++i) {
            workers_.emplace_back([this]() {
                while (true) {
                    std::function<void()> task;
                    
                    {
                        std::unique_lock<std::mutex> lock(mutex_);
                        condition_.wait(lock, [this]() {
                            return stop_ || !tasks_.empty();
                        });
                        
                        if (stop_ && tasks_.empty()) {
                            return;
                        }
                        
                        task = std::move(tasks_.front());
                        tasks_.pop();
                    }
                    
                    task();
                }
            });
        }
    }
    
    ~ThreadPool() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            stop_ = true;
        }
        condition_.notify_all();
        
        for (auto& worker : workers_) {
            worker.join();
        }
    }
    
    template<typename F>
    void enqueue(F&& task) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            tasks_.emplace(std::forward<F>(task));
        }
        condition_.notify_one();
    }
    
private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex mutex_;
    std::condition_variable condition_;
    bool stop_;
};

void threadPoolDemo() {
    ThreadPool pool(4);
    
    for (int i = 0; i < 8; ++i) {
        pool.enqueue([i]() {
            std::cout << "Task " << i << " executed by thread "
                      << std::this_thread::get_id() << "\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
        });
    }
    
    // 等待所有任务完成
    std::this_thread::sleep_for(std::chrono::seconds(3));
}
```

### 7.3 Actor 模式

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <memory>

class Actor {
public:
    Actor() : running_(true) {
        thread_ = std::thread([this]() {
            while (running_) {
                std::function<void()> message;
                
                {
                    std::unique_lock<std::mutex> lock(mutex_);
                    condition_.wait(lock, [this]() {
                        return !running_ || !mailbox_.empty();
                    });
                    
                    if (!running_ && mailbox_.empty()) {
                        return;
                    }
                    
                    message = std::move(mailbox_.front());
                    mailbox_.pop();
                }
                
                message();
            }
        });
    }
    
    virtual ~Actor() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            running_ = false;
        }
        condition_.notify_all();
        thread_.join();
    }
    
    void send(std::function<void()> message) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            mailbox_.push(std::move(message));
        }
        condition_.notify_one();
    }
    
private:
    std::thread thread_;
    std::mutex mutex_;
    std::condition_variable condition_;
    std::queue<std::function<void()>> mailbox_;
    bool running_;
};

void actorDemo() {
    auto actor = std::make_unique<Actor>();
    
    actor->send([]() { std::cout << "Message 1\n"; });
    actor->send([]() { std::cout << "Message 2\n"; });
    actor->send([]() { std::cout << "Message 3\n"; });
    
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
```

---

## 8. 线程池实现

### 8.1 完整的线程池

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <functional>
#include <future>
#include <memory>

class ThreadPool {
public:
    explicit ThreadPool(size_t numThreads = 
                        std::thread::hardware_concurrency()) 
        : stop_(false) {
        for (size_t i = 0; i < numThreads; ++i) {
            workers_.emplace_back([this]() {
                workerThread();
            });
        }
    }
    
    ~ThreadPool() {
        shutdown();
    }
    
    // 提交任务，返回 future
    template<typename F, typename... Args>
    auto submit(F&& f, Args&&... args) 
        -> std::future<std::invoke_result_t<F, Args...>> 
    {
        using ReturnType = std::invoke_result_t<F, Args...>;
        
        auto task = std::make_shared<std::packaged_task<ReturnType()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        
        std::future<ReturnType> result = task->get_future();
        
        {
            std::unique_lock<std::mutex> lock(mutex_);
            if (stop_) {
                throw std::runtime_error("ThreadPool has been stopped");
            }
            tasks_.emplace([task]() { (*task)(); });
        }
        
        condition_.notify_one();
        return result;
    }
    
    void shutdown() {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            stop_ = true;
        }
        condition_.notify_all();
        
        for (auto& worker : workers_) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }
    
    size_t size() const { return workers_.size(); }
    
private:
    void workerThread() {
        while (true) {
            std::function<void()> task;
            
            {
                std::unique_lock<std::mutex> lock(mutex_);
                condition_.wait(lock, [this]() {
                    return stop_ || !tasks_.empty();
                });
                
                if (stop_ && tasks_.empty()) {
                    return;
                }
                
                task = std::move(tasks_.front());
                tasks_.pop();
            }
            
            task();
        }
    }
    
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex mutex_;
    std::condition_variable condition_;
    bool stop_;
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
    
    // 获取结果
    for (auto& result : results) {
        std::cout << "Result: " << result.get() << "\n";
    }
}
```

---

## 9. 协程（C++20）

### 9.1 协程基础概念

```cpp
#include <iostream>
#include <coroutine>
#include <exception>

// C++20 协程需要三个核心组件：
// 1. promise_type：控制协程行为
// 2. coroutine_handle：操作协程
// 3. awaitable：控制等待行为

// 简单的生成器协程
template<typename T>
class Generator {
public:
    struct promise_type {
        T value;
        std::exception_ptr exception;
        
        Generator get_return_object() {
            return Generator{
                std::coroutine_handle<promise_type>::from_promise(*this)
            };
        }
        
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        
        void return_void() {}
        
        template<typename U>
        std::suspend_always yield_value(U&& val) {
            value = std::forward<U>(val);
            return {};
        }
        
        void unhandled_exception() {
            exception = std::current_exception();
        }
    };
    
    Generator(std::coroutine_handle<promise_type> h) : handle_(h) {}
    
    ~Generator() {
        if (handle_) {
            handle_.destroy();
        }
    }
    
    // 迭代器接口
    bool next() {
        handle_.resume();
        return !handle_.done();
    }
    
    T value() const {
        return handle_.promise().value;
    }
    
private:
    std::coroutine_handle<promise_type> handle_;
};

// 使用协程生成斐波那契数列
Generator<int> fibonacci(int n) {
    int a = 0, b = 1;
    for (int i = 0; i < n; ++i) {
        co_yield a;
        auto temp = a;
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

### 9.2 异步任务协程

```cpp
#include <iostream>
#include <coroutine>
#include <future>
#include <thread>

// 异步任务
template<typename T>
class Task {
public:
    struct promise_type {
        T value;
        std::exception_ptr exception;
        std::coroutine_handle<> continuation;
        
        Task get_return_object() {
            return Task{
                std::coroutine_handle<promise_type>::from_promise(*this)
            };
        }
        
        std::suspend_always initial_suspend() noexcept { return {}; }
        
        auto final_suspend() noexcept {
            struct Awaiter {
                bool await_ready() noexcept { return false; }
                
                std::coroutine_handle<> await_suspend(
                    std::coroutine_handle<promise_type> h) noexcept 
                {
                    return h.promise().continuation;
                }
                
                void await_resume() noexcept {}
            };
            return Awaiter{};
        }
        
        void return_value(T val) { value = std::move(val); }
        void unhandled_exception() { exception = std::current_exception(); }
    };
    
    ~Task() {
        if (handle_) {
            handle_.destroy();
        }
    }
    
    bool await_ready() { return handle_.done(); }
    
    std::coroutine_handle<> await_suspend(std::coroutine_handle<> h) {
        handle_.promise().continuation = h;
        return handle_;
    }
    
    T await_resume() {
        if (handle_.promise().exception) {
            std::rethrow_exception(handle_.promise().exception);
        }
        return std::move(handle_.promise().value);
    }
    
private:
    std::coroutine_handle<promise_type> handle_;
    
    Task(std::coroutine_handle<promise_type> h) : handle_(h) {}
};

// 异步睡眠
Task<void> asyncSleep(int ms) {
    co_await std::async(std::launch::async, [ms]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    });
}

// 异步计算
Task<int> asyncCompute(int value) {
    co_await asyncSleep(100);  // 模拟异步操作
    co_return value * 2;
}

// 组合多个异步任务
Task<int> combinedTask() {
    int a = co_await asyncCompute(10);
    int b = co_await asyncCompute(20);
    co_return a + b;
}
```

---

## 参考资料

1. **C++ Concurrency in Action** - Anthony Williams
2. **cppreference 并发支持**: https://en.cppreference.com/w/cpp/thread
3. **C++20 协程**: https://en.cppreference.com/w/cpp/language/coroutines

---

## 学习建议

1. **理解基础**：先掌握线程、互斥量、条件变量
2. **避免陷阱**：重点学习死锁、数据竞争的预防
3. **使用高层抽象**：优先使用 std::async、线程池
4. **性能测试**：多线程程序需要充分测试和性能分析
5. **逐步进阶**：从简单示例开始，逐步实现复杂系统

---

*本教程涵盖 C++11 到 C++20 的并发编程特性，适合有 C++ 基础的开发者学习多线程编程。*
