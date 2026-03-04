/**
 * @file item09_23_30_review.cpp
 * @brief EMC++ Item 9, 23-30 条款复习示例
 * 
 * 涵盖内容：
 * - Item 9: 优先使用别名声明而非typedef
 * - Item 23: 理解std::move和std::forward
 * - Item 24: 区分通用引用和右值引用
 * - Item 25: 对右值引用使用std::move，对通用引用使用std::forward
 * - Item 26: 避免在通用引用上重载
 * - Item 27: 熟悉通用引用重载的替代方案
 * - Item 28: 理解引用折叠
 * - Item 29: 认识移动操作的代价
 * - Item 30: 熟悉完美转发失败的情况
 */

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <functional>
#include <type_traits>

namespace emcpp_review {

// ========================================
// Item 9: 优先使用别名声明而非typedef
// ========================================

// 模板别名定义（必须在namespace级别）
template <typename T>
using MyVector = std::vector<T>;

void demonstrateItem09() {
    std::cout << "=== Item 9: 别名声明 vs typedef ===\n\n";
    
    // 传统typedef
    typedef std::unique_ptr<std::vector<int>> IntVectorPtr_typedef;
    
    // C++11别名声明（推荐）
    using IntVectorPtr = std::unique_ptr<std::vector<int>>;
    
    // 使用上效果相同
    IntVectorPtr ptr1 = std::make_unique<std::vector<int>>();
    ptr1->push_back(1);
    std::cout << "别名声明创建的vector大小: " << ptr1->size() << "\n";
    
    // 别名声明的优势：支持模板化
    // typedef无法做到这一点
    MyVector<int> vec = {1, 2, 3};
    std::cout << "模板别名创建的vector: ";
    for (int x : vec) std::cout << x << " ";
    std::cout << "\n";
    
    // 函数指针别名
    using Callback = void(*)(int);
    // typedef void(*Callback_typedef)(int);  // 等价但更难读
    
    std::cout << "\n别名声明的优势:\n";
    std::cout << "  1. 语法更清晰直观\n";
    std::cout << "  2. 支持模板别名\n";
    std::cout << "  3. 可以直接在类中声明\n";
}

// ========================================
// Item 23: 理解std::move和std::forward
// ========================================

void demonstrateItem23() {
    std::cout << "\n=== Item 23: std::move和std::forward ===\n\n";
    
    std::cout << "std::move的本质:\n";
    std::cout << "  - 只是一个类型转换（static_cast<T&&>）\n";
    std::cout << "  - 不生成任何代码\n";
    std::cout << "  - 告诉编译器该对象可以被移动\n\n";
    
    std::string s1 = "Hello";
    std::string s2 = std::move(s1);
    
    std::cout << "std::move后:\n";
    std::cout << "  s1: \"" << s1 << "\" (可能为空)\n";
    std::cout << "  s2: \"" << s2 << "\"\n\n";
    
    std::cout << "std::forward的本质:\n";
    std::cout << "  - 条件转换\n";
    std::cout << "  - 保持参数的原始值类别\n";
    std::cout << "  - 主要用于完美转发\n";
    
    // 完美转发示例
    auto process = [](auto&& x) {
        using T = decltype(x);
        if constexpr (std::is_lvalue_reference_v<T>) {
            std::cout << "  处理左值: " << x << "\n";
        } else {
            std::cout << "  处理右值: " << x << "\n";
        }
    };
    
    std::string str = "World";
    process(str);           // 左值
    process(std::string("Temp"));  // 右值
}

// ========================================
// Item 24: 区分通用引用和右值引用
// ========================================

void demonstrateItem24() {
    std::cout << "\n=== Item 24: 通用引用 vs 右值引用 ===\n\n";
    
    // 右值引用示例
    auto rvalueRef = [](std::string&& s) {
        std::cout << "  右值引用: " << s << "\n";
    };
    
    // 通用引用示例
    auto universalRef = [](auto&& s) {
        std::cout << "  通用引用: " << s << "\n";
    };
    
    std::string str = "Hello";
    
    std::cout << "右值引用只能绑定右值:\n";
    rvalueRef(std::string("Temp"));
    // rvalueRef(str);  // 编译错误！
    
    std::cout << "\n通用引用可以绑定左值和右值:\n";
    universalRef(str);                      // 绑定左值
    universalRef(std::string("Temp"));      // 绑定右值
    
    std::cout << "\n识别方法:\n";
    std::cout << "  1. T&& 在模板参数推导中 → 通用引用\n";
    std::cout << "  2. auto&& → 通用引用\n";
    std::cout << "  3. 类型已知的 X&& → 右值引用\n";
}

// ========================================
// Item 25: std::move vs std::forward的使用
// ========================================

class Widget {
private:
    std::string name_;
    
public:
    // 对右值引用参数使用std::move
    void setName(std::string&& name) {
        name_ = std::move(name);
        std::cout << "  [右值引用] setName: " << name_ << "\n";
    }
    
    // 对通用引用参数使用std::forward
    template<typename T>
    void setNameUniversal(T&& name) {
        name_ = std::forward<T>(name);
        std::cout << "  [通用引用] setName: " << name_ << "\n";
    }
    
    const std::string& name() const { return name_; }
};

void demonstrateItem25() {
    std::cout << "\n=== Item 25: 正确使用std::move和std::forward ===\n\n";
    
    Widget w;
    
    std::cout << "右值引用参数使用std::move:\n";
    w.setName(std::string("RightValue"));
    
    std::cout << "\n通用引用参数使用std::forward:\n";
    std::string name = "LeftValue";
    w.setNameUniversal(name);
    w.setNameUniversal(std::string("RightValue2"));
    
    std::cout << "\n规则总结:\n";
    std::cout << "  - 右值引用参数 → std::move\n";
    std::cout << "  - 通用引用参数 → std::forward\n";
    std::cout << "  - 返回值优化场景 → 都不用\n";
}

// ========================================
// Item 26-27: 避免在通用引用上重载
// ========================================

// 错误示例：通用引用与重载的组合问题
class BadStringSetter {
private:
    std::string name_;
    
public:
    // 重载1：普通参数
    void setName(const std::string& name) {
        name_ = name;
        std::cout << "  [const&] setName: " << name_ << "\n";
    }
    
    // 重载2：通用引用（会匹配几乎所有类型！）
    template<typename T>
    void setName(T&& name) {
        name_ = std::forward<T>(name);
        std::cout << "  [通用引用] setName: " << name_ << "\n";
    }
};

// 正确方案：使用标签分发或限制模板
class GoodStringSetter {
private:
    std::string name_;
    
public:
    // 使用SFINAE限制通用引用
    template<typename T,
             typename = std::enable_if_t<!std::is_same_v<std::decay_t<T>, GoodStringSetter>>>
    void setName(T&& name) {
        name_ = std::forward<T>(name);
        std::cout << "  [受限通用引用] setName: " << name_ << "\n";
    }
    
    void setName(int index) {
        name_ = "Index_" + std::to_string(index);
        std::cout << "  [int重载] setName: " << name_ << "\n";
    }
};

void demonstrateItem26_27() {
    std::cout << "\n=== Item 26-27: 通用引用与重载 ===\n\n";
    
    std::cout << "问题：通用引用是贪婪的匹配\n";
    std::cout << "  它会匹配几乎任何类型，包括重载候选\n\n";
    
    BadStringSetter bad;
    std::cout << "BadStringSetter:\n";
    bad.setName(std::string("Hello"));  // 调用哪个？
    bad.setName("World");               // 调用哪个？
    
    std::cout << "\n解决方案:\n";
    std::cout << "  1. 放弃重载（使用不同函数名）\n";
    std::cout << "  2. 使用const T&（放弃移动语义）\n";
    std::cout << "  3. 标签分发（Tag Dispatch）\n";
    std::cout << "  4. 限制模板参数（SFINAE）\n";
    
    std::cout << "\nGoodStringSetter示例:\n";
    GoodStringSetter good;
    std::string s = "Test";
    good.setName(s);
    good.setName(std::string("Move"));
    good.setName(42);
}

// ========================================
// Item 28: 引用折叠
// ========================================

// 辅助模板函数：类型推导演示
// 必须在namespace级别定义
template <typename T>
const char* deduceType(T&& param) {
    if constexpr (std::is_lvalue_reference_v<T>) {
        return "左值引用";
    } else {
        return "右值引用";
    }
}

void demonstrateItem28() {
    std::cout << "\n=== Item 28: 引用折叠规则 ===\n\n";
    
    std::cout << "引用折叠规则:\n";
    std::cout << "  T& &   → T&\n";
    std::cout << "  T& &&  → T&\n";
    std::cout << "  T&& &  → T&\n";
    std::cout << "  T&& && → T&&\n\n";
    
    std::cout << "简化记忆：只有 && && 才会折叠为 &&\n\n";
    
    int x = 10;
    int& lr = x;
    
    std::cout << "类型推导结果:\n";
    std::cout << "  int x = 10; deduceType(x) → " << deduceType(x) << "\n";
    std::cout << "  int& lr = x; deduceType(lr) → " << deduceType(lr) << "\n";
    std::cout << "  deduceType(10) → " << deduceType(10) << "\n";
}

// ========================================
// Item 29: 移动操作的代价
// ========================================

void demonstrateItem29() {
    std::cout << "\n=== Item 29: 移动操作的代价 ===\n\n";
    
    std::cout << "移动不是免费的:\n";
    std::cout << "  1. 需要更新源对象和目标对象的指针\n";
    std::cout << "  2. 对于小型对象，移动可能不比拷贝快\n";
    std::cout << "  3. 标准库容器的移动需要考虑元素数量\n\n";
    
    // 小型数组：移动不比拷贝快
    std::array<int, 5> arr1 = {1, 2, 3, 4, 5};
    auto arr2 = std::move(arr1);  // 实际上是元素级拷贝
    
    std::cout << "std::array的移动:\n";
    std::cout << "  小型array的移动 = 元素级拷贝\n";
    std::cout << "  arr1仍包含: ";
    for (int x : arr1) std::cout << x << " ";
    std::cout << "\n\n";
    
    std::cout << "最佳实践:\n";
    std::cout << "  1. 大型对象优先移动\n";
    std::cout << "  2. 不要过度优化\n";
    std::cout << "  3. 让编译器处理返回值优化\n";
}

// ========================================
// Item 30: 完美转发失败的情况
// ========================================

void demonstrateItem30() {
    std::cout << "\n=== Item 30: 完美转发失败的情况 ===\n\n";
    
    // 完美转发失败的情况示例
    auto fwd = [](auto&&... args) {
        // 模拟完美转发
    };
    
    std::cout << "完美转发失败的情况:\n\n";
    
    // 1. 花括号初始化列表
    std::cout << "1. 花括号初始化列表:\n";
    // fwd({1, 2, 3});  // 编译错误！无法推导
    std::vector<int> v{1, 2, 3};  // 需要先创建对象
    std::cout << "   解决：先创建对象再转发\n\n";
    
    // 2. 0和NULL作为空指针
    std::cout << "2. 0/NULL作为空指针:\n";
    void (*func)(int*) = nullptr;
    // fwd(func, 0);    // 可能导致int而非int*
    // fwd(func, NULL); // 同样的问题
    fwd(func, nullptr); // 正确
    std::cout << "   解决：使用nullptr\n\n";
    
    // 3. 仅声明的静态常量整型成员
    struct Widget2 {
        static const int value = 42;  // 仅声明
    };
    // const int Widget2::value;  // 需要定义
    std::cout << "3. 静态常量成员:\n";
    std::cout << "   仅声明的静态常量成员无法取地址\n";
    std::cout << "   解决：提供定义或使用constexpr\n\n";
    
    // 4. 重载函数和模板函数
    std::cout << "4. 重载函数名:\n";
    auto f1 = [](int) {};
    auto f2 = [](double) {};
    // fwd(重载函数名);  // 无法确定调用哪个
    std::cout << "   解决：显式指定类型或使用函数指针\n";
}

// ========================================
// 主演示函数
// ========================================

void demonstrate() {
    std::cout << "╔════════════════════════════════════════╗\n";
    std::cout << "║     EMC++ Item 9, 23-30 复习           ║\n";
    std::cout << "╚════════════════════════════════════════╝\n\n";
    
    demonstrateItem09();
    demonstrateItem23();
    demonstrateItem24();
    demonstrateItem25();
    demonstrateItem26_27();
    demonstrateItem28();
    demonstrateItem29();
    demonstrateItem30();
    
    std::cout << "\nEMC++条款复习完成！\n";
}

} // namespace emcpp_review
