/**
 * @file enum_vs_enum_class.cpp
 * @brief 传统 enum 与 enum class 对比演示
 * 
 * 本文件通过对比展示：
 * 1. 命名污染问题
 * 2. 类型安全问题
 * 3. 实际使用差异
 * 
 * 注释中的编译错误仅供说明，实际代码已处理避免编译失败
 */

#include <iostream>
#include <iomanip>
#include <string>

// ============================================================================
// 问题1：命名污染演示
// ============================================================================

/**
 * 传统 enum 的命名污染问题
 * 枚举值会"泄露"到外部作用域
 */

// 使用命名空间模拟传统 enum 的"隔离"尝试
namespace LegacyEnum {
    // 传统 enum：枚举值在 enclosing 作用域可见
    enum Color {
        Red,    // ::LegacyEnum::Red 或者 using 后直接 Red
        Green,
        Blue
    };
    
    // 问题：如果定义另一个枚举，可能会冲突
    // enum TrafficLight {
    //     Red,     // 编译错误！Red 已经定义
    //     Yellow,
    //     Green    // 编译错误！Green 已经定义
    // };
    
    // 不得不加前缀来避免冲突
    enum TrafficLight {
        TrafficLight_Red,
        TrafficLight_Yellow,
        TrafficLight_Green
    };
}

/**
 * enum class 解决命名污染
 * 每个枚举值都在自己的作用域内
 */
namespace ModernEnum {
    // 两个 enum class 可以有相同的枚举值名称
    enum class Color {
        Red, Green, Blue  // Color::Red, Color::Green, Color::Blue
    };
    
    enum class TrafficLight {
        Red, Yellow, Green  // TrafficLight::Red, TrafficLight::Green
        // 不会冲突！因为各自在独立的作用域内
    };
}

// ============================================================================
// 问题2：类型安全演示
// ============================================================================

/**
 * 传统 enum 的类型安全问题
 */

namespace LegacyEnum {
    // 两个不同的枚举
    enum Status { OK, Error, Pending };
    // enum Color { Red, Green, Blue };  // 已在上面定义
    
    // 危险的函数
    void processStatus(Status s) {
        switch (s) {
            case OK: std::cout << "状态正常\n"; break;
            case Error: std::cout << "状态错误\n"; break;
            case Pending: std::cout << "等待中\n"; break;
        }
    }
}

namespace ModernEnum {
    enum class Status { OK, Error, Pending };
    enum class Color { Red, Green, Blue };
    
    // 类型安全的函数
    void processStatus(Status s) {
        switch (s) {
            case Status::OK: std::cout << "状态正常\n"; break;
            case Status::Error: std::cout << "状态错误\n"; break;
            case Status::Pending: std::cout << "等待中\n"; break;
        }
    }
}

// ============================================================================
// 问题3：隐式转换演示
// ============================================================================

void demonstrateImplicitConversion() {
    std::cout << "\n  【隐式转换问题】\n";
    std::cout << "  ─────────────────────────────────────────────\n\n";
    
    // 传统 enum 可以隐式转换为整数
    LegacyEnum::Color legacyColor = LegacyEnum::Red;
    
    std::cout << "  传统 enum:\n";
    int value = legacyColor;  // 隐式转换，OK
    std::cout << "    int value = legacyColor;  // 隐式转换\n";
    std::cout << "    value = " << value << "\n";
    
    // 可以参与算术运算
    std::cout << "    legacyColor + 10 = " << (legacyColor + 10) << "\n";
    
    // 可以和整数比较
    std::cout << "    legacyColor > 0 ? " << (legacyColor > 0 ? "true" : "false") << "\n";
    
    // enum class 不允许隐式转换
    std::cout << "\n  enum class:\n";
    ModernEnum::Color modernColor = ModernEnum::Color::Red;
    
    std::cout << "    // int value = modernColor;  // 编译错误！\n";
    std::cout << "    // 必须显式转换:\n";
    int modernValue = static_cast<int>(modernColor);
    std::cout << "    int value = static_cast<int>(modernColor);\n";
    std::cout << "    value = " << modernValue << "\n";
}

// ============================================================================
// 问题4：不同枚举类型混用演示
// ============================================================================

void demonstrateTypeMixing() {
    std::cout << "\n  【不同枚举类型混用问题】\n";
    std::cout << "  ─────────────────────────────────────────────\n\n";
    
    using namespace LegacyEnum;
    
    // 传统 enum：不同类型可以混用（危险！）
    Color c = Red;
    Status s = OK;
    
    std::cout << "  传统 enum 的危险行为:\n";
    
    // 可以比较不同类型的枚举
    std::cout << "    Color::Red == Status::OK ? " << (c == s ? "true" : "false") << "\n";
    std::cout << "    (两者底层值都是0，所以相等)\n";
    
    // 可以把一种枚举赋给另一种
    Color c2 = static_cast<Color>(Error);
    std::cout << "    Color c = Status::Error; // 可以赋值！\n";
    std::cout << "    c 的值: " << c2 << "\n";
    
    // 错误的函数调用
    std::cout << "\n    // 危险：传入错误的枚举类型\n";
    std::cout << "    processStatus(static_cast<Status>(Green));  // 逻辑错误！\n";
    
    // enum class：类型安全
    std::cout << "\n  enum class 的类型安全:\n";
    ModernEnum::Color mc = ModernEnum::Color::Red;
    ModernEnum::Status ms = ModernEnum::Status::OK;
    
    std::cout << "    // if (mc == ms)  // 编译错误！类型不匹配\n";
    std::cout << "    // ModernEnum::processStatus(mc);  // 编译错误！\n";
    std::cout << "    类型不匹配时编译器会报错，防止逻辑错误\n";
}

// ============================================================================
// 底层类型对比
// ============================================================================

void demonstrateUnderlyingType() {
    std::cout << "\n  【底层类型对比】\n";
    std::cout << "  ─────────────────────────────────────────────\n\n";
    
    std::cout << "  传统 enum:\n";
    std::cout << "    - 底层类型由编译器决定\n";
    std::cout << "    - 可能是 int, unsigned int, 或其他\n";
    std::cout << "    - 不同平台可能不同\n";
    std::cout << "    - sizeof 可能不同\n\n";
    
    std::cout << "  enum class:\n";
    std::cout << "    - 可以明确指定底层类型\n";
    std::cout << "    - 保证跨平台一致性\n";
    std::cout << "    - 示例:\n";
    std::cout << "      enum class SmallEnum : uint8_t { A, B };\n";
    std::cout << "      enum class BigEnum : uint64_t { X = 0xFFFFFFFFFFFFFFFF };\n";
}

// ============================================================================
// 实际代码对比
// ============================================================================

void demonstrateCodeComparison() {
    std::cout << "\n  【代码风格对比】\n";
    std::cout << "  ─────────────────────────────────────────────\n\n";
    
    std::cout << "  传统 enum 代码风格:\n";
    std::cout << "  ┌─────────────────────────────────────────────┐\n";
    std::cout << "  │ enum Color { Color_Red, Color_Green,       │\n";
    std::cout << "  │              Color_Blue };  // 需要前缀      │\n";
    std::cout << "  │                                            │\n";
    std::cout << "  │ Color c = Color_Red;  // 或者 using 后 Red  │\n";
    std::cout << "  │ int x = c;  // 隐式转换                    │\n";
    std::cout << "  └─────────────────────────────────────────────┘\n\n";
    
    std::cout << "  enum class 代码风格:\n";
    std::cout << "  ┌─────────────────────────────────────────────┐\n";
    std::cout << "  │ enum class Color { Red, Green, Blue };      │\n";
    std::cout << "  │ // 不需要前缀，作用域自动隔离              │\n";
    std::cout << "  │                                            │\n";
    std::cout << "  │ Color c = Color::Red;  // 清晰的作用域      │\n";
    std::cout << "  │ int x = static_cast<int>(c);  // 显式转换  │\n";
    std::cout << "  └─────────────────────────────────────────────┘\n";
}

// ============================================================================
// 演示函数
// ============================================================================

void run_enum_vs_enum_class_demo() {
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                                                                      ║\n";
    std::cout << "║          传统 enum vs enum class 对比演示                          ║\n";
    std::cout << "║                                                                      ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════════════╝\n";
    
    std::cout << "\n  ══════════════════════════════════════════════════════════════\n";
    std::cout << "  问题 1：命名污染\n";
    std::cout << "  ══════════════════════════════════════════════════════════════\n";
    
    std::cout << "\n  传统 enum:\n";
    std::cout << "    enum Color { Red, Green, Blue };\n";
    std::cout << "    // Red, Green, Blue 直接可见于外部作用域\n";
    std::cout << "    // 如果定义另一个枚举有 Red，会冲突！\n\n";
    
    std::cout << "  enum class:\n";
    std::cout << "    enum class Color { Red, Green, Blue };\n";
    std::cout << "    // 使用 Color::Red, Color::Green, Color::Blue\n";
    std::cout << "    // 不同枚举可以重名，不会冲突\n";
    
    demonstrateImplicitConversion();
    demonstrateTypeMixing();
    demonstrateUnderlyingType();
    demonstrateCodeComparison();
    
    // 总结表格
    std::cout << "\n\n";
    std::cout << "══════════════════════════════════════════════════════════════\n";
    std::cout << "  📊 对比总结表\n";
    std::cout << "══════════════════════════════════════════════════════════════\n\n";
    
    std::cout << "  ┌─────────────────────┬────────────────┬────────────────┐\n";
    std::cout << "  │       特性          │  传统 enum     │  enum class    │\n";
    std::cout << "  ├─────────────────────┼────────────────┼────────────────┤\n";
    std::cout << "  │ 作用域封装          │      否        │      是        │\n";
    std::cout << "  │ 命名污染            │      会        │      不会      │\n";
    std::cout << "  │ 隐式转整数          │      允许      │      禁止      │\n";
    std::cout << "  │ 类型安全            │      否        │      是        │\n";
    std::cout << "  │ 指定底层类型        │      不支持    │      支持      │\n";
    std::cout << "  │ 前向声明            │      受限      │      支持      │\n";
    std::cout << "  │ 不同类型可比较      │      可以      │      禁止      │\n";
    std::cout << "  └─────────────────────┴────────────────┴────────────────┘\n";
    
    std::cout << "\n\n";
    std::cout << "══════════════════════════════════════════════════════════════\n";
    std::cout << "  💡 结论：在现代 C++ 中，优先使用 enum class\n";
    std::cout << "══════════════════════════════════════════════════════════════\n";
    std::cout << "  1. 更好的封装性：避免命名冲突\n";
    std::cout << "  2. 更强的类型安全：防止意外错误\n";
    std::cout << "  3. 更清晰的代码：必须使用作用域限定\n";
    std::cout << "  4. 更灵活的控制：可指定底层类型\n";
    std::cout << "══════════════════════════════════════════════════════════════\n";
}
