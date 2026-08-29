/**
 * enum class演示
 * C++11强类型枚举
 */

#include "enum_class_demo.h"
#include <iostream>
#include <string>

// 传统enum（不推荐）
enum OldColor { RED, GREEN, BLUE };
enum OldSize { SMALL, MEDIUM, LARGE };

// C++11 enum class（推荐）
enum class Color { RED, GREEN, BLUE };
enum class Size { SMALL, MEDIUM, LARGE };

// 指定底层类型
enum class Priority : char { LOW = 'L', MEDIUM = 'M', HIGH = 'H' };

// 前置声明
enum class Status;  // 可以前置声明

void processStatus(Status s);

enum class Status { OK, ERROR, PENDING };

void processStatus(Status s) {
    switch (s) {
        case Status::OK: std::cout << "OK"; break;
        case Status::ERROR: std::cout << "ERROR"; break;
        case Status::PENDING: std::cout << "PENDING"; break;
    }
}

void enumClassDemo() {
    std::cout << "╔══════════════════════════════════════╗" << std::endl;
    std::cout << "║        enum class 强类型枚举         ║" << std::endl;
    std::cout << "╚══════════════════════════════════════╝" << std::endl;
    
    // ========== 1. 传统enum的问题 ==========
    std::cout << "\n--- 1. 传统enum的问题 ---" << std::endl;
    
    // 问题1：命名污染
    std::cout << "  问题1：命名污染" << std::endl;
    int x = RED;  // RED在全局作用域
    std::cout << "    RED = " << x << std::endl;
    
    // 问题2：隐式转换
    std::cout << "  问题2：隐式转换（可以和int比较）" << std::endl;
    if (RED == 0) {
        std::cout << "    RED == 0 （意外匹配！）" << std::endl;
    }
    
    // 问题3：类型不安全
    std::cout << "  问题3：类型不安全" << std::endl;
    OldColor c = RED;
    OldSize s = SMALL;
    // 传统枚举都能隐式转成整数；直接跨枚举比较会得到编译器诊断，
    // 但转成整数后仍可能把两个无关领域的相同编码误当成相等。
    if (static_cast<int>(c) == static_cast<int>(s)) {
        std::cout << "    OldColor::RED == OldSize::SMALL（语义错误！）" << std::endl;
    }
    
    // ========== 2. enum class的优势 ==========
    std::cout << "\n--- 2. enum class的优势 ---" << std::endl;
    
    // 优势1：作用域限定
    std::cout << "  优势1：作用域限定" << std::endl;
    Color c2 = Color::RED;  // 必须使用 Color::
    // int y = RED;  // 错误！RED不在全局作用域
    std::cout << "    Color::RED - 必须用类名限定，编码为 "
              << static_cast<int>(c2) << std::endl;
    
    // 优势2：不隐式转换
    std::cout << "  优势2：不隐式转换" << std::endl;
    // if (Color::RED == 0) { }  // 错误！
    std::cout << "    Color::RED == 0 编译错误！" << std::endl;
    
    // 显式转换
    int value = static_cast<int>(Color::RED);
    std::cout << "    static_cast<int>(Color::RED) = " << value << std::endl;
    std::cout << "    显式转换是有意的摩擦：只在序列化、索引或旧接口边界转换" << std::endl;
    
    // 优势3：类型安全
    std::cout << "  优势3：类型安全" << std::endl;
    Color c3 = Color::GREEN;
    Size s3 = Size::SMALL;
    // if (c3 == s3) { }  // 错误！不同类型不能比较
    std::cout << "    Color::GREEN(" << static_cast<int>(c3) << ") 与 Size::SMALL("
              << static_cast<int>(s3) << ") 即使编码可见，也不能直接比较" << std::endl;
    
    // ========== 3. switch用法 ==========
    std::cout << "\n--- 3. switch用法 ---" << std::endl;
    
    Color myColor = Color::BLUE;
    std::cout << "  myColor = Color::BLUE" << std::endl;
    std::cout << "  switch结果: ";
    
    switch (myColor) {
        case Color::RED:
            std::cout << "红色" << std::endl;
            break;
        case Color::GREEN:
            std::cout << "绿色" << std::endl;
            break;
        case Color::BLUE:
            std::cout << "蓝色" << std::endl;
            break;
    }
    
    // ========== 4. 指定底层类型 ==========
    std::cout << "\n--- 4. 指定底层类型 ---" << std::endl;
    
    Priority p = Priority::HIGH;
    std::cout << "  Priority使用char作为底层类型" << std::endl;
    std::cout << "  Priority::HIGH = '" << static_cast<char>(p) << "'" << std::endl;
    
    std::cout << "  sizeof(Color) = " << sizeof(Color) << " (默认int)" << std::endl;
    std::cout << "  sizeof(Priority) = " << sizeof(Priority) << " (指定char)" << std::endl;
    
    // ========== 5. 前置声明 ==========
    std::cout << "\n--- 5. 前置声明 ---" << std::endl;
    
    Status status = Status::OK;
    std::cout << "  Status: ";
    processStatus(status);
    std::cout << std::endl;
    std::cout << "  enum class可以前置声明，减少编译依赖" << std::endl;
    
    // ========== 6. 总结 ==========
    std::cout << "\n--- 6. 总结 ---" << std::endl;
    std::cout << "  enum class优势：" << std::endl;
    std::cout << "    1. 避免命名污染（作用域限定）" << std::endl;
    std::cout << "    2. 避免意外转换（类型安全）" << std::endl;
    std::cout << "    3. 支持前置声明" << std::endl;
    std::cout << "    4. 可以指定底层类型" << std::endl;
}
