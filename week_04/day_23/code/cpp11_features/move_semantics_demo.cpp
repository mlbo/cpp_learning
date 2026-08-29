/**
 * @file move_semantics_demo.cpp
 * @brief 移动语义完整演示 - 展示移动语义的核心概念和工作原理
 * 
 * 本文件演示：
 * 1. 深拷贝与移动操作的性能对比
 * 2. std::move 的工作原理
 * 3. 移动语义在容器中的应用
 */

#include <algorithm>
#include <iostream>
#include <utility>      // std::move, std::forward
#include <string>
#include <vector>
#include <chrono>
#include <cstring>
#include <stdexcept>
#include <streambuf>

namespace {

using BenchmarkClock = std::chrono::steady_clock;
using BenchmarkDuration = std::chrono::nanoseconds;

volatile std::size_t moveBenchmarkSink = 0;

BenchmarkDuration median(std::vector<BenchmarkDuration> samples) {
    std::sort(samples.begin(), samples.end());
    return samples[samples.size() / 2];
}

BenchmarkDuration measureStringTransfer(
    const std::vector<std::string>& source, bool moveValues) {
    // 两条路径都在计时外准备同样的源数据和目标容量，只比较 push_back 阶段。
    std::vector<std::string> working = source;
    std::vector<std::string> destination;
    destination.reserve(working.size());

    const auto start = BenchmarkClock::now();
    if (moveValues) {
        for (std::string& value : working) {
            destination.push_back(std::move(value));
        }
    } else {
        for (const std::string& value : working) {
            destination.push_back(value);
        }
    }
    const auto elapsed = std::chrono::duration_cast<BenchmarkDuration>(
        BenchmarkClock::now() - start);

    const std::size_t observed = destination.size() +
        (destination.empty() ? 0U : destination.front().size());
    moveBenchmarkSink = observed;
    return elapsed;
}

class MoveSemanticsThrowingStreamBuffer : public std::streambuf {
protected:
    int_type overflow(int_type) override {
        throw std::runtime_error("injected output failure");
    }
};

} // namespace

// ============================================================
// 自定义字符串类 - 演示移动语义
// ============================================================

class MyString {
private:
    char* data_;        // 字符数据指针
    size_t size_;       // 字符串长度
    static int copyCount_;   // 拷贝计数
    static int moveCount_;   // 移动计数

public:
    // 默认构造函数
    MyString() : data_(nullptr), size_(0) {}
    
    // 带参数的构造函数
    explicit MyString(const char* str) {
        if (str) {
            size_ = std::strlen(str);
            data_ = new char[size_ + 1];
            std::strcpy(data_, str);
        } else {
            size_ = 0;
            data_ = nullptr;
        }
    }
    
    // 析构函数
    ~MyString() {
        delete[] data_;
    }
    
    // 拷贝构造函数 - 深拷贝
    MyString(const MyString& other) : size_(other.size_) {
        if (other.data_) {
            data_ = new char[size_ + 1];
            std::strcpy(data_, other.data_);
        } else {
            data_ = nullptr;
        }
        ++copyCount_;
    }
    
    // 移动构造函数 - 本类型转交独占指针
    MyString(MyString&& other) noexcept 
        : data_(other.data_), size_(other.size_) {
        // 本类型把源对象恢复为空状态，防止重复释放
        other.data_ = nullptr;
        other.size_ = 0;
        ++moveCount_;
    }
    
    // 拷贝赋值运算符
    MyString& operator=(const MyString& other) {
        if (this != &other) {
            // 先完成所有可能失败的分配，再提交新状态；分配失败时 *this 不变。
            char* replacement = nullptr;
            if (other.data_) {
                replacement = new char[other.size_ + 1];
                std::memcpy(replacement, other.data_, other.size_ + 1);
            }
            delete[] data_;
            data_ = replacement;
            size_ = other.size_;
            ++copyCount_;
        }
        return *this;
    }
    
    // 移动赋值运算符
    MyString& operator=(MyString&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            // 窃取资源
            data_ = other.data_;
            size_ = other.size_;
            // 本类型把源对象恢复为空状态
            other.data_ = nullptr;
            other.size_ = 0;
            ++moveCount_;
        }
        return *this;
    }
    
    // 获取字符串内容
    const char* c_str() const { return data_ ? data_ : ""; }
    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }
    static int copyCount() noexcept { return copyCount_; }
    static int moveCount() noexcept { return moveCount_; }
    
    // 静态方法：获取统计信息
    static void printStats() {
        std::cout << "\n  === 统计信息 ===" << std::endl;
        std::cout << "  总拷贝次数: " << copyCount_ << std::endl;
        std::cout << "  总移动次数: " << moveCount_ << std::endl;
    }
    
    static void resetStats() {
        copyCount_ = 0;
        moveCount_ = 0;
    }
};

// 静态成员初始化
int MyString::copyCount_ = 0;
int MyString::moveCount_ = 0;

// ============================================================
// 辅助函数
// ============================================================

// 返回临时对象的工厂函数
MyString createString(const char* content) {
    std::cout << "\n  --- createString() 函数内部 ---" << std::endl;
    MyString temp(content);
    std::cout << "  准备返回临时对象..." << std::endl;
    return temp;  // 可能触发 RVO 或移动语义
}

// 接受左值引用的函数
void processLValue(const MyString& s) {
    std::cout << "  [processLValue] 处理左值: \"" << s.c_str() << "\"" << std::endl;
}

// 接受右值引用的函数
void processRValue(MyString&& s) {
    std::cout << "  [processRValue] 处理右值: \"" << s.c_str() << "\"" << std::endl;
    // 注意：命名参数变量的表达式 s 是左值；这是表达式规则，不是对象的永久标签。
    // 如果需要继续移动，需要再次 std::move
}

// ============================================================
// 演示函数
// ============================================================

void demonstrateMoveSemantics() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "移动语义演示" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    MyString::resetStats();
    
    // -------------------------------------------------------
    // 演示 1：拷贝 vs 移动
    // -------------------------------------------------------
    std::cout << "\n【演示 1】拷贝语义 vs 移动语义\n" << std::endl;
    std::cout << "特殊成员不承担日志输出；调用点结合值与计数观察真实操作，"
                 "避免流异常削弱资源操作契约。\n" << std::endl;
    
    std::cout << "创建原始字符串 s1:" << std::endl;
    MyString s1("Hello World!");
    std::cout << "  构造完成: \"" << s1.c_str() << "\"" << std::endl;
    
    std::cout << "\n拷贝构造 s2 = s1:" << std::endl;
    MyString s2 = s1;  // 拷贝构造
    std::cout << "  拷贝完成，累计拷贝次数: " << MyString::copyCount() << std::endl;
    
    std::cout << "\n移动构造 s3 = std::move(s1):" << std::endl;
    MyString s3 = std::move(s1);  // 移动构造
    
    std::cout << "\n检查对象状态:" << std::endl;
    std::cout << "  s1 (移动后): \"" << s1.c_str() << "\" (长度: " << s1.size() << ")" << std::endl;
    std::cout << "  s2 (拷贝后): \"" << s2.c_str() << "\" (长度: " << s2.size() << ")" << std::endl;
    std::cout << "  s3 (移动后): \"" << s3.c_str() << "\" (长度: " << s3.size() << ")" << std::endl;
    std::cout << "  本教学类型把 moved-from 状态定义为空串；标准只要求对象保持有效，"
                 "不同类型不必采用空状态。" << std::endl;
    
    // -------------------------------------------------------
    // 演示 2：移动赋值
    // -------------------------------------------------------
    std::cout << "\n【演示 2】移动赋值运算符\n" << std::endl;
    
    MyString s4("Initial");
    std::cout << "\n移动赋值 s4 = std::move(s2):" << std::endl;
    s4 = std::move(s2);  // 移动赋值
    
    std::cout << "\n检查对象状态:" << std::endl;
    std::cout << "  s2 (移动后): \"" << s2.c_str() << "\"" << std::endl;
    std::cout << "  s4 (移动后): \"" << s4.c_str() << "\"" << std::endl;
    
    // -------------------------------------------------------
    // 演示 3：函数返回值
    // -------------------------------------------------------
    std::cout << "\n【演示 3】函数返回值与移动语义\n" << std::endl;
    
    std::cout << "调用 createString() 获取返回值:" << std::endl;
    MyString s5 = createString("From Function");
    std::cout << "  s5: \"" << s5.c_str() << "\"" << std::endl;
    
    // -------------------------------------------------------
    // 演示 4：右值引用参数
    // -------------------------------------------------------
    std::cout << "\n【演示 4】左值引用与右值引用参数\n" << std::endl;
    
    MyString s6("Left Value");
    
    std::cout << "调用 processLValue(s6):" << std::endl;
    processLValue(s6);
    
    std::cout << "\n调用 processRValue(std::move(s6)):" << std::endl;
    processRValue(std::move(s6));
    
    std::cout << "\n调用 processRValue(MyString(\"临时对象\")):" << std::endl;
    processRValue(MyString("临时对象"));  // 构造临时对象的表达式是纯右值
    
    // 打印统计信息
    MyString::printStats();
    
    std::cout << "\n退出作用域，开始析构..." << std::endl;
}

// ============================================================
// 容器中的移动语义演示
// ============================================================

void demonstrateVectorMove() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "容器中的移动语义演示" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    MyString::resetStats();
    
    std::cout << "\n创建 vector 并添加元素:\n" << std::endl;
    std::vector<MyString> vec;
    
    // 预留空间避免频繁扩容
    vec.reserve(5);
    
    std::cout << "\npush_back(左值):" << std::endl;
    MyString s1("Push LValue");
    vec.push_back(s1);  // 拷贝
    
    std::cout << "\npush_back(右值):" << std::endl;
    vec.push_back(MyString("Push RValue"));  // 移动
    
    std::cout << "\npush_back(std::move(左值)):" << std::endl;
    MyString s2("Move Into Vector");
    vec.push_back(std::move(s2));  // 移动
    
    std::cout << "\nemplace_back(原地构造):" << std::endl;
    vec.emplace_back("Emplace");  // 原地构造，无拷贝无移动
    
    MyString::printStats();
    
    std::cout << "\n退出作用域，开始析构..." << std::endl;
}

// ============================================================
// 性能对比演示
// ============================================================

void demonstratePerformance() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "性能对比演示" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    constexpr std::size_t itemCount = 2'000;
    constexpr std::size_t payloadBytes = 4'096;
    constexpr std::size_t repeats = 7;
    const std::vector<std::string> source(
        itemCount, std::string(payloadBytes, 'x'));

    // 预热分配器和代码路径；正式样本交替先测拷贝/先测移动，减小固定顺序偏差。
    static_cast<void>(measureStringTransfer(source, false));
    static_cast<void>(measureStringTransfer(source, true));
    std::vector<BenchmarkDuration> copySamples;
    std::vector<BenchmarkDuration> moveSamples;
    copySamples.reserve(repeats);
    moveSamples.reserve(repeats);
    for (std::size_t repeat = 0; repeat < repeats; ++repeat) {
        if (repeat % 2U == 0U) {
            copySamples.push_back(measureStringTransfer(source, false));
            moveSamples.push_back(measureStringTransfer(source, true));
        } else {
            moveSamples.push_back(measureStringTransfer(source, true));
            copySamples.push_back(measureStringTransfer(source, false));
        }
    }

    const BenchmarkDuration copyTime = median(std::move(copySamples));
    const BenchmarkDuration moveTime = median(std::move(moveSamples));
    const auto toMilliseconds = [](BenchmarkDuration duration) {
        return std::chrono::duration<double, std::milli>(duration).count();
    };

    std::cout << "每条路径都处理 " << itemCount << " 个、每个 " << payloadBytes
              << " 字节的字符串；源数据构造和目标 reserve 不计时。" << std::endl;
    std::cout << "预热后交替测量 " << repeats << " 次并取中位数。" << std::endl;
    std::cout << "拷贝 push_back 中位数: " << toMilliseconds(copyTime) << " ms" << std::endl;
    std::cout << "移动 push_back 中位数: " << toMilliseconds(moveTime) << " ms" << std::endl;
    if (moveTime.count() > 0) {
        const double ratio = static_cast<double>(copyTime.count()) /
                             static_cast<double>(moveTime.count());
        std::cout << "本次测量的耗时比(拷贝/移动): " << ratio << std::endl;
    } else {
        std::cout << "本次移动测量低于计时器分辨率，无法计算耗时比" << std::endl;
    }
    std::cout << "benchmarkSink=" << moveBenchmarkSink
              << " 防止整个结果被删除；数字仍只适用于当前实现、编译选项和机器。"
              << std::endl;
    std::cout << "提示：长负载避免让实验主要测到常见SSO短字符串路径，"
                 "但移动是否存在、成本多高仍必须按具体类型判断。" << std::endl;
}

// ============================================================
// 入口函数
// ============================================================

/**
 * @brief 移动语义演示入口函数
 */
void run_move_semantics_demo() {
    demonstrateMoveSemantics();
    demonstrateVectorMove();
    demonstratePerformance();
}

bool verify_move_semantics_contract() {
    MyString::resetStats();
    MoveSemanticsThrowingStreamBuffer throwingBuffer;
    std::streambuf* const originalBuffer = std::cout.rdbuf(&throwingBuffer);
    const std::ios::iostate originalExceptions = std::cout.exceptions();
    std::cout.exceptions(std::ios::badbit | std::ios::failbit);

    bool passed = false;
    try {
        MyString original("contract");
        MyString copied(original);
        MyString assigned("old");
        assigned = original;
        MyString moved(std::move(original));

        const bool valuesPreserved = std::string(copied.c_str()) == "contract" &&
                                     std::string(assigned.c_str()) == "contract" &&
                                     std::string(moved.c_str()) == "contract";
        const bool countsObserved = MyString::copyCount() == 2 &&
                                    MyString::moveCount() == 1;
        const bool movedFromContract = original.empty() &&
                                       std::string(original.c_str()).empty();

        original = MyString("reused");
        const bool reusable = std::string(original.c_str()) == "reused";
        passed = valuesPreserved && countsObserved && movedFromContract && reusable;
    } catch (...) {
        passed = false;
    }

    std::cout.exceptions(std::ios::goodbit);
    std::cout.clear();
    std::cout.rdbuf(originalBuffer);
    std::cout.exceptions(originalExceptions);
    return passed;
}
