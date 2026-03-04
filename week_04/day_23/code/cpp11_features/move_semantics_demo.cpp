/**
 * @file move_semantics_demo.cpp
 * @brief 移动语义完整演示 - 展示移动语义的核心概念和工作原理
 * 
 * 本文件演示：
 * 1. 深拷贝与移动操作的性能对比
 * 2. std::move 的工作原理
 * 3. 移动语义在容器中的应用
 */

#include <iostream>
#include <utility>      // std::move, std::forward
#include <string>
#include <vector>
#include <chrono>
#include <cstring>

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
    MyString() : data_(nullptr), size_(0) {
        std::cout << "  [默认构造] 创建空字符串" << std::endl;
    }
    
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
        std::cout << "  [构造] 创建字符串: \"" << (data_ ? data_ : "") << "\"" << std::endl;
    }
    
    // 析构函数
    ~MyString() {
        if (data_) {
            std::cout << "  [析构] 释放字符串: \"" << data_ << "\"" << std::endl;
            delete[] data_;
        } else {
            std::cout << "  [析构] 空字符串对象" << std::endl;
        }
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
        std::cout << "  [拷贝构造] 深拷贝: \"" << (data_ ? data_ : "") 
                  << "\" (第 " << copyCount_ << " 次拷贝)" << std::endl;
    }
    
    // 移动构造函数 - 资源窃取
    MyString(MyString&& other) noexcept 
        : data_(other.data_), size_(other.size_) {
        // 源对象置空，防止重复释放
        other.data_ = nullptr;
        other.size_ = 0;
        ++moveCount_;
        std::cout << "  [移动构造] 资源转移 (第 " << moveCount_ << " 次移动)" << std::endl;
    }
    
    // 拷贝赋值运算符
    MyString& operator=(const MyString& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            if (other.data_) {
                data_ = new char[size_ + 1];
                std::strcpy(data_, other.data_);
            } else {
                data_ = nullptr;
            }
            ++copyCount_;
            std::cout << "  [拷贝赋值] 深拷贝: \"" << (data_ ? data_ : "") 
                      << "\" (第 " << copyCount_ << " 次拷贝)" << std::endl;
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
            // 源对象置空
            other.data_ = nullptr;
            other.size_ = 0;
            ++moveCount_;
            std::cout << "  [移动赋值] 资源转移 (第 " << moveCount_ << " 次移动)" << std::endl;
        }
        return *this;
    }
    
    // 获取字符串内容
    const char* c_str() const { return data_ ? data_ : ""; }
    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }
    
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
    // 注意：这里 s 本身是左值（因为它有名字）
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
    
    std::cout << "创建原始字符串 s1:" << std::endl;
    MyString s1("Hello World!");
    
    std::cout << "\n拷贝构造 s2 = s1:" << std::endl;
    MyString s2 = s1;  // 拷贝构造
    
    std::cout << "\n移动构造 s3 = std::move(s1):" << std::endl;
    MyString s3 = std::move(s1);  // 移动构造
    
    std::cout << "\n检查对象状态:" << std::endl;
    std::cout << "  s1 (移动后): \"" << s1.c_str() << "\" (长度: " << s1.size() << ")" << std::endl;
    std::cout << "  s2 (拷贝后): \"" << s2.c_str() << "\" (长度: " << s2.size() << ")" << std::endl;
    std::cout << "  s3 (移动后): \"" << s3.c_str() << "\" (长度: " << s3.size() << ")" << std::endl;
    
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
    processRValue(MyString("临时对象"));  // 临时对象是右值
    
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
    
    const int N = 10000;
    
    // 使用 std::string 进行性能测试
    std::cout << "\n创建包含 " << N << " 个元素的 vector:\n" << std::endl;
    
    // 测试拷贝方式
    auto start = std::chrono::high_resolution_clock::now();
    {
        std::vector<std::string> vec;
        vec.reserve(N);
        for (int i = 0; i < N; ++i) {
            std::string s = "String number " + std::to_string(i);
            vec.push_back(s);  // 拷贝
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto copyTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    
    // 测试移动方式
    start = std::chrono::high_resolution_clock::now();
    {
        std::vector<std::string> vec;
        vec.reserve(N);
        for (int i = 0; i < N; ++i) {
            std::string s = "String number " + std::to_string(i);
            vec.push_back(std::move(s));  // 移动
        }
    }
    end = std::chrono::high_resolution_clock::now();
    auto moveTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    
    std::cout << "拷贝方式耗时: " << copyTime << " 微秒" << std::endl;
    std::cout << "移动方式耗时: " << moveTime << " 微秒" << std::endl;
    std::cout << "性能提升: " << (double)copyTime / moveTime << " 倍" << std::endl;
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
