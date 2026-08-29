/**
 * @file item26_avoid_overloading.cpp
 * @brief EMC++ Item 26: 避免在通用引用上重载
 * 
 * 通用引用是"贪婪"的，它会匹配几乎任何参数类型，
 * 这可能导致重载决议产生意想不到的结果。
 * 
 * 本文件演示：
 * 1. 通用引用重载陷阱
 * 2. 解决方案：标签分发、SFINAE、放弃重载
 */

#include <iostream>
#include <string>
#include <vector>
#include <type_traits>
#include <chrono>
#include <set>

namespace day24::item26_and_27 {

// ==================== 问题演示 ====================

/**
 * @brief 一个简单的 Person 类，用于演示问题
 */
class Person {
public:
    std::string name;
    int age;
    
    Person() : name("Unknown"), age(0) {}
    
    Person(const std::string& n, int a) : name(n), age(a) {
        std::cout << "  Person 构造: " << name << ", " << age << "\n";
    }
    
    void print() const {
        std::cout << "  Person[" << name << ", " << age << "]\n";
    }
};

// ==================== 陷阱示例 ====================

namespace trap {

/**
 * @brief 问题代码：在通用引用上重载
 * 
 * 这会导致以下问题：
 * 1. 重载 2 会"劫持"本应调用重载 1 的调用
 * 2. 拷贝构造时可能触发错误的重载
 */
std::multiset<std::string> names;  // 全局名字集合

// 重载 1：接受整数索引
void logData(int idx) {
    std::cout << "  [重载1] 整数索引: " << idx << "\n";
}

// 重载 2：通用引用版本（贪婪！）
template<typename T>
void logData(T&& name) {
    std::cout << "  [重载2] 通用引用版本\n";
    if constexpr (std::is_constructible_v<std::string, T&&>) {
        names.emplace(std::forward<T>(name));
    } else {
        std::cout << "  该类型不能构造string；真实业务模板若直接emplace会在此编译失败\n";
    }
}

/**
 * @brief 演示陷阱的函数
 */
void demonstrateTrap() {
    std::cout << "\n=== 陷阱演示：通用引用重载 ===\n";
    
    std::string name = "Alice";
    
    std::cout << "\n调用 logData(42):\n";
    logData(42);  // int 精确匹配；同等级时优先非模板重载

    short index = 7;
    std::cout << "\n调用 short 类型的 logData(index):\n";
    logData(index);  // 通用引用精确匹配 short&，胜过 short 到 int 的提升
    
    std::cout << "\n调用 logData(name):\n";
    logData(name);  // 调用重载2，OK
    
    std::cout << "\n调用 logData(std::move(name)):\n";
    logData(std::move(name));  // 调用重载2，OK
    
    std::cout << "\n问题总结:\n";
    std::cout << "  - 通用引用匹配 'too well'\n";
    std::cout << "  - 会劫持其他重载版本的调用\n";
    std::cout << "  - int 实参仍会选非模板 int 重载；问题出在 short 等需要转换的实参\n";
}

} // namespace trap

// ==================== 解决方案 1：放弃重载 ====================

namespace solution1 {

// 使用不同的函数名，完全避免重载问题
void logById(int idx) {
    std::cout << "  按索引记录: " << idx << "\n";
}

template<typename T>
void logByName(T&& name) {
    std::cout << "  按名称记录: " << name << "\n";
}

void demonstrate() {
    std::cout << "\n=== 解决方案 1：放弃重载 ===\n";
    
    logById(42);      // 明确的函数名
    logByName("Bob"); // 明确的函数名
    
    std::cout << "优点：简单直观，避免歧义\n";
    std::cout << "缺点：需要记忆更多函数名\n";
}

} // namespace solution1

// ==================== 解决方案 2：使用具体类型的 const 引用 ====================

namespace solution2 {

void logData(int idx) {
    std::cout << "  [const T&] 整数: " << idx << "\n";
}

void logData(const std::string& name) {
    std::cout << "  [const string&] 名称: " << name << "\n";
}

void demonstrate() {
    std::cout << "\n=== 解决方案 2：传递 const std::string& ===\n";
    
    logData(42);         // 调用 int 版本
    short index = 7;
    logData(index);       // string重载不可行，short提升到int版本
    logData(std::string("Charlie"));
    
    std::cout << "优点：不会劫持重载\n";
    std::cout << "缺点：调用方可能先构造临时string，且接口只接受这一具体语义类型\n";
}

} // namespace solution2

// ==================== 解决方案 3：按值接收 ====================

namespace solution_by_value {

void logName(std::string name) {
    std::cout << "  [按值] 名称: " << name << "\n";
}

void demonstrate() {
    std::cout << "\n=== 解决方案 3：按值接收名称 ===\n";
    std::string name = "Dora";
    logName(name);                 // 左值复制到形参
    logName(std::string("Evan")); // 右值可移动或直接构造形参
    std::cout << "适用：函数本来就要保存一份值，且一次复制/移动成本可接受\n";
}

} // namespace solution_by_value

// ==================== 解决方案 4：标签分发 ====================

namespace solution3 {

// 标签类型
struct TrueType {};
struct FalseType {};

// 根据类型特征分发
template<typename T>
void logDataImpl(T&& val, TrueType) {
    std::cout << "  [标签分发] 整数类型处理: " << val << "\n";
}

template<typename T>
void logDataImpl(T&& val, FalseType) {
    std::cout << "  [标签分发] 其他类型处理: " << val << "\n";
}

// 入口函数：使用标签分发
template<typename T>
void logData(T&& val) {
    // 使用 is_integral 判断是否为整数
    logDataImpl(std::forward<T>(val),
                std::conditional_t<std::is_integral_v<std::decay_t<T>>,
                                   TrueType, FalseType>{});
}

void demonstrate() {
    std::cout << "\n=== 解决方案 4：标签分发 ===\n";
    
    logData(42);           // 整数 → TrueType 分支
    logData("David");      // 非整数 → FalseType 分支
    logData(3.14);         // 非整数 → FalseType 分支
    
    std::cout << "优点：保持通用引用，正确分发\n";
    std::cout << "缺点：需要额外的标签类型和分发逻辑\n";
}

} // namespace solution3

// ==================== 解决方案 5：SFINAE 约束 ====================

namespace solution4 {

// SFINAE 约束：只在非整数时启用通用引用版本
template<typename T,
         typename = std::enable_if_t<!std::is_integral_v<std::decay_t<T>>>>
void logData(T&& name) {
    std::cout << "  [SFINAE] 非整数版本: " << name << "\n";
}

// 整数版本
void logData(int idx) {
    std::cout << "  [SFINAE] 整数版本: " << idx << "\n";
}

void demonstrate() {
    std::cout << "\n=== 解决方案 5：SFINAE 约束 ===\n";
    
    logData(42);         // 整数版本
    logData("Eve");      // SFINAE 约束后的通用引用版本
    
    std::cout << "优点：编译期约束，不产生歧义\n";
    std::cout << "缺点：模板语法复杂，可读性差\n";
}

} // namespace solution4

// ==================== 解决方案 6：C++20 Concepts ====================

#if __cplusplus >= 202002L

namespace solution5 {

// C++20 Concepts 约束
template<typename T>
concept NonIntegral = !std::is_integral_v<std::decay_t<T>>;

void logData(int idx) {
    std::cout << "  [Concepts] 整数版本: " << idx << "\n";
}

template<NonIntegral T>
void logData(T&& name) {
    std::cout << "  [Concepts] 非整数版本\n";
}

void demonstrate() {
    std::cout << "\n=== 解决方案 6：C++20 Concepts ===\n";
    
    logData(42);         // 整数版本
    logData("Frank");    // Concepts 约束版本
    
    std::cout << "优点：语法清晰，约束直观\n";
    std::cout << "缺点：需要 C++20 支持\n";
}

} // namespace solution5

#endif

// ==================== 主演示函数 ====================

void run() {
    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "   EMC++ Item 26-27: 重载风险与替代方案\n";
    std::cout << "========================================\n";
    
    // 演示陷阱
    trap::demonstrateTrap();
    
    // 演示解决方案
    solution1::demonstrate();
    solution2::demonstrate();
    solution_by_value::demonstrate();
    solution3::demonstrate();
    solution4::demonstrate();
    
#if __cplusplus >= 202002L
    solution5::demonstrate();
#else
    std::cout << "\n=== 解决方案 6：C++20 Concepts ===\n";
    std::cout << "  (需要 C++20 支持，当前编译器不支持)\n";
#endif
    
    std::cout << "\n========================================\n";
    std::cout << "   Item 26-27 演示完成\n";
    std::cout << "========================================\n";
    
    std::cout << "\n关键要点:\n";
    std::cout << "  1. 通用引用匹配范围极广，容易「劫持」其他重载\n";
    std::cout << "  2. 避免在通用引用上重载，除非必要\n";
    std::cout << "  3. Item 27 的替代方案包括不同函数名、传 const 引用、传值和标签分发\n";
    std::cout << "  4. 必须保留重载时，再用 SFINAE/Concepts 约束模板参与条件\n";
}

} // namespace day24::item26_and_27

void runItem26And27Demo() {
    day24::item26_and_27::run();
}
