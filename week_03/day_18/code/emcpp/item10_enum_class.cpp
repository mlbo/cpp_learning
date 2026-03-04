/**
 * EMC++ Item 10: 优先使用enum class而非enum
 * 
 * 要点：
 * 1. enum class避免命名污染
 * 2. enum class避免意外转换
 * 3. enum class支持前置声明
 * 4. enum class可以指定底层类型
 */

#include "item10_enum_class.h"
#include <iostream>
#include <type_traits>

// 传统enum的底层类型不明确
enum OldEnum { A, B, C };

// enum class可以明确指定
enum class NewEnum : unsigned char { X, Y, Z };

// 利用enum class实现类型安全的ID
class UserId {
private:
    enum class IdTag { VALID };
    int value_;
    
public:
    explicit UserId(int v) : value_(v) {}
    int value() const { return value_; }
};

class OrderId {
private:
    enum class IdTag { VALID };
    int value_;
    
public:
    explicit OrderId(int v) : value_(v) {}
    int value() const { return value_; }
};

// 使用enum class作为类型标签，防止混淆
void processUser(UserId id) {
    std::cout << "  处理用户ID: " << id.value() << std::endl;
}

void processOrder(OrderId id) {
    std::cout << "  处理订单ID: " << id.value() << std::endl;
}

void item10Demo() {
    std::cout << "╔══════════════════════════════════════╗" << std::endl;
    std::cout << "║  Item 10: 优先enum class而非enum     ║" << std::endl;
    std::cout << "╚══════════════════════════════════════╝" << std::endl;
    
    // ========== 1. 命名空间污染问题 ==========
    std::cout << "\n--- 1. 命名空间污染 ---" << std::endl;
    
    // 传统enum的枚举值泄漏到包含它的作用域
    std::cout << "  传统enum: A, B, C 在全局作用域" << std::endl;
    std::cout << "  A的值: " << A << std::endl;  // 直接使用
    
    // enum class不会污染
    std::cout << "  enum class: 必须用 NewEnum::X" << std::endl;
    std::cout << "  NewEnum::X的值: " << static_cast<int>(NewEnum::X) << std::endl;
    
    // ========== 2. 类型安全 ==========
    std::cout << "\n--- 2. 类型安全 ---" << std::endl;
    
    // 传统enum可以和整数比较（危险！）
    std::cout << "  传统enum可以隐式转int: A == 0 → " << (A == 0) << std::endl;
    
    // enum class不能隐式转换
    std::cout << "  enum class不能隐式转int（编译错误）" << std::endl;
    
    // ========== 3. 前置声明 ==========
    std::cout << "\n--- 3. 前置声明优势 ---" << std::endl;
    
    std::cout << "  enum class可以前置声明：" << std::endl;
    std::cout << "    enum class Status; // 前置声明" << std::endl;
    std::cout << "    void process(Status s); // 使用" << std::endl;
    std::cout << "    enum class Status { OK, ERROR }; // 定义" << std::endl;
    
    std::cout << "  好处：减少编译依赖，加快编译速度" << std::endl;
    
    // ========== 4. 指定底层类型 ==========
    std::cout << "\n--- 4. 指定底层类型 ---" << std::endl;
    
    std::cout << "  传统enum底层类型不确定" << std::endl;
    std::cout << "    sizeof(OldEnum) = " << sizeof(OldEnum) << std::endl;
    
    std::cout << "  enum class可以明确指定" << std::endl;
    std::cout << "    sizeof(NewEnum) = " << sizeof(NewEnum) << " (unsigned char)" << std::endl;
    
    // ========== 5. 类型安全ID示例 ==========
    std::cout << "\n--- 5. 类型安全ID应用 ---" << std::endl;
    
    UserId user(123);
    OrderId order(456);
    
    processUser(user);
    processOrder(order);
    
    // 编译错误！类型不匹配
    // processUser(order);  // OrderId不能传给UserId
    
    std::cout << "  使用enum class标签实现类型安全，防止ID混淆" << std::endl;
    
    // ========== 6. 何时可以用传统enum ==========
    std::cout << "\n--- 6. 传统enum的使用场景 ---" << std::endl;
    std::cout << "  传统enum可能在以下情况有用：" << std::endl;
    std::cout << "    1. 需要隐式转int时（如数组索引）" << std::endl;
    std::cout << "    2. 需要位运算组合标志时" << std::endl;
    std::cout << "  但即使这些情况，enum class + 显式转换通常更好" << std::endl;
}
