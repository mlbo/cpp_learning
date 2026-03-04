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
    names.emplace(std::forward<T>(name));
}

/**
 * @brief 演示陷阱的函数
 */
void demonstrateTrap() {
    std::cout << "\n=== 陷阱演示：通用引用重载 ===\n";
    
    std::string name = "Alice";
    
    std::cout << "\n调用 logData(42):\n";
    logData(42);  // 期望调用重载1，实际调用重载2！
    // 原因：T&& = int&& 是精确匹配，比 int 转换更优
    
    std::cout << "\n调用 logData(name):\n";
    logData(name);  // 调用重载2，OK
    
    std::cout << "\n调用 logData(std::move(name)):\n";
    logData(std::move(name));  // 调用重载2，OK
    
    std::cout << "\n问题总结:\n";
    std::cout << "  - 通用引用匹配 'too well'\n";
    std::cout << "  - 会劫持其他重载版本的调用\n";
    std::cout << "  - 特别是精确匹配优于类型转换的情况\n";
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
    std::cout << "  按名称记录\n";
}

void demonstrate() {
    std::cout << "\n=== 解决方案 1：放弃重载 ===\n";
    
    logById(42);      // 明确的函数名
    logByName("Bob"); // 明确的函数名
    
    std::cout << "优点：简单直观，避免歧义\n";
    std::cout << "缺点：需要记忆更多函数名\n";
}

} // namespace solution1

// ==================== 解决方案 2：传递 const T& ====================

namespace solution2 {

void logData(int idx) {
    std::cout << "  [const T&] 整数: " << idx << "\n";
}

template<typename T>
void logData(const T& name) {
    std::cout << "  [const T&] 名称版本\n";
}

void demonstrate() {
    std::cout << "\n=== 解决方案 2：传递 const T& ===\n";
    
    logData(42);         // 调用 int 版本
    logData("Charlie");  // 调用 const T& 版本
    
    std::cout << "优点：不会劫持重载\n";
    std::cout << "缺点：无法移动语义，性能损失\n";
}

} // namespace solution2

// ==================== 解决方案 3：标签分发 ====================

namespace solution3 {

// 标签类型
struct TrueType {};
struct FalseType {};

// 根据类型特征分发
template<typename T>
void logDataImpl(T&& val, TrueType) {
    std::cout << "  [标签分发] 整数类型处理\n";
}

template<typename T>
void logDataImpl(T&& val, FalseType) {
    std::cout << "  [标签分发] 其他类型处理\n";
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
    std::cout << "\n=== 解决方案 3：标签分发 ===\n";
    
    logData(42);           // 整数 → TrueType 分支
    logData("David");      // 非整数 → FalseType 分支
    logData(3.14);         // 非整数 → FalseType 分支
    
    std::cout << "优点：保持通用引用，正确分发\n";
    std::cout << "缺点：需要额外的标签类型和分发逻辑\n";
}

} // namespace solution3

// ==================== 解决方案 4：SFINAE 约束 ====================

namespace solution4 {

// SFINAE 约束：只在非整数时启用通用引用版本
template<typename T,
         typename = std::enable_if_t<!std::is_integral_v<std::decay_t<T>>>>
void logData(T&& name) {
    std::cout << "  [SFINAE] 非整数版本\n";
}

// 整数版本
void logData(int idx) {
    std::cout << "  [SFINAE] 整数版本: " << idx << "\n";
}

void demonstrate() {
    std::cout << "\n=== 解决方案 4：SFINAE 约束 ===\n";
    
    logData(42);         // 整数版本
    logData("Eve");      // SFINAE 约束后的通用引用版本
    
    std::cout << "优点：编译期约束，不产生歧义\n";
    std::cout << "缺点：模板语法复杂，可读性差\n";
}

} // namespace solution4

// ==================== 解决方案 5：C++20 Concepts ====================

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
    std::cout << "\n=== 解决方案 5：C++20 Concepts ===\n";
    
    logData(42);         // 整数版本
    logData("Frank");    // Concepts 约束版本
    
    std::cout << "优点：语法清晰，约束直观\n";
    std::cout << "缺点：需要 C++20 支持\n";
}

} // namespace solution5

#endif

// ==================== 主演示函数 ====================

void runItem26Demo() {
    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "   EMC++ Item 26: 避免在通用引用上重载\n";
    std::cout << "========================================\n";
    
    // 演示陷阱
    trap::demonstrateTrap();
    
    // 演示解决方案
    solution1::demonstrate();
    solution2::demonstrate();
    solution3::demonstrate();
    solution4::demonstrate();
    
#if __cplusplus >= 202002L
    solution5::demonstrate();
#else
    std::cout << "\n=== 解决方案 5：C++20 Concepts ===\n";
    std::cout << "  (需要 C++20 支持，当前编译器不支持)\n";
#endif
    
    std::cout << "\n========================================\n";
    std::cout << "   Item 26 演示完成\n";
    std::cout << "========================================\n";
    
    std::cout << "\n关键要点:\n";
    std::cout << "  1. 通用引用匹配范围极广，容易「劫持」其他重载\n";
    std::cout << "  2. 避免在通用引用上重载，除非必要\n";
    std::cout << "  3. 优先使用标签分发或 SFINAE/Concepts 约束\n";
    std::cout << "  4. 最简单的方案是使用不同的函数名\n";
}
