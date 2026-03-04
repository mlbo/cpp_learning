/**
 * Day 22: EMC++ Item 09 - 类型别名
 * 实现文件
 * 
 * 核心要点：优先使用using而非typedef
 * 1. using语法更直观
 * 2. using支持模板别名
 * 3. using可读性更好
 */

#include "item09_type_alias.h"
#include <vector>
#include <map>
#include <memory>
#include <string>

// ==================== 传统typedef定义 ====================

// 基本类型别名
typedef int TInt;
typedef unsigned long TULong;
typedef double TDouble;

// 指针类型别名
typedef int* TIntPtr;
typedef const char* TCString;

// 函数指针类型别名（typedef语法较难理解）
typedef int (*TOldFuncPtr)(int, int);
typedef void (*TOldCallback)(int);

// ==================== 现代using定义 ====================

// 基本类型别名（using语法更清晰）
using ULong = unsigned long;
using Double = double;

// 指针类型别名
using IntPtr = int*;
using CString = const char*;

// 函数指针类型别名（using语法更直观）
using NewFuncPtr = int (*)(int, int);
using Callback = void (*)(int);

// ==================== 模板别名（using独有优势）====================

// 容器别名模板
template<typename T>
using Vec = std::vector<T>;

template<typename K, typename V>
using Map = std::map<K, V>;

// 智能指针别名模板
template<typename T>
using Ptr = std::shared_ptr<T>;

template<typename T>
using UPtr = std::unique_ptr<T>;

// 固定大小数组别名
template<typename T, size_t N>
using Arr = T[N];

// ==================== 演示函数 ====================

void typedefVsUsingDemo() {
    std::cout << "\n--- typedef vs using 语法对比 ---" << std::endl;
    
    // 使用typedef定义的类型
    TInt ti = 10;
    TULong tul = 20UL;
    std::cout << "typedef定义的类型：" << std::endl;
    std::cout << "  TInt ti = " << ti << std::endl;
    std::cout << "  TULong tul = " << tul << std::endl;
    
    // 使用using定义的类型
    ULong ul = 30UL;
    Double d = 3.14;
    std::cout << "\nusing定义的类型：" << std::endl;
    std::cout << "  ULong ul = " << ul << std::endl;
    std::cout << "  Double d = " << d << std::endl;
    
    // 语法对比
    std::cout << "\n语法对比：" << std::endl;
    std::cout << "  typedef: typedef int INT;     // 名字在后面" << std::endl;
    std::cout << "  using:   using INT = int;     // 名字在前面（更自然）" << std::endl;
    std::cout << "\n优势：using采用\"别名 = 类型\"的形式，更符合直觉" << std::endl;
}

void templateAliasDemo() {
    std::cout << "\n--- 模板别名（using独有优势）---" << std::endl;
    
    // typedef 无法实现模板别名
    // template<typename T>
    // typedef std::vector<T> Vec;  // 编译错误！
    
    // using 可以轻松实现
    Vec<int> numbers = {1, 2, 3, 4, 5};
    Vec<std::string> names = {"Alice", "Bob", "Charlie"};
    
    std::cout << "Vec<int> numbers: ";
    for (int n : numbers) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Vec<string> names: ";
    for (const auto& name : names) {
        std::cout << name << " ";
    }
    std::cout << std::endl;
    
    // Map别名
    Map<std::string, int> scores = {
        {"Alice", 95},
        {"Bob", 87},
        {"Charlie", 92}
    };
    
    std::cout << "\nMap<string, int> scores:" << std::endl;
    for (const auto& [name, score] : scores) {
        std::cout << "  " << name << ": " << score << std::endl;
    }
    
    // 智能指针别名
    Ptr<int> sharedPtr = std::make_shared<int>(42);
    std::cout << "\nPtr<int> sharedPtr: " << *sharedPtr << std::endl;
    
    std::cout << "\n结论：模板别名是using最大的优势，typedef无法实现！" << std::endl;
}

void functionPointerAliasDemo() {
    std::cout << "\n--- 函数指针别名 ---" << std::endl;
    
    // 定义简单的加法和乘法函数
    auto add = [](int a, int b) -> int { return a + b; };
    auto multiply = [](int a, int b) -> int { return a * b; };
    
    // 使用typedef定义的类型
    std::cout << "使用typedef定义的函数指针：" << std::endl;
    TOldFuncPtr fp1 = add;
    std::cout << "  add(3, 4) = " << fp1(3, 4) << std::endl;
    
    // 使用using定义的类型（更直观）
    std::cout << "\n使用using定义的函数指针：" << std::endl;
    NewFuncPtr fp2 = multiply;
    std::cout << "  multiply(3, 4) = " << fp2(3, 4) << std::endl;
    
    // 语法对比
    std::cout << "\n语法对比：" << std::endl;
    std::cout << "  typedef: typedef int (*FuncPtr)(int, int);" << std::endl;
    std::cout << "  using:   using FuncPtr = int (*)(int, int);" << std::endl;
    std::cout << "\n结论：using语法中，FuncPtr这个名字更加突出，易于理解" << std::endl;
}

void standardLibraryAliasDemo() {
    std::cout << "\n--- 标准库中的类型别名 ---" << std::endl;
    
    // C++11后标准库大量使用using
    std::cout << "标准库中常见的类型别名：" << std::endl;
    
    // size_t类型
    std::vector<int>::size_type sz = 100;
    std::cout << "  vector<int>::size_type = " << sz << std::endl;
    
    // 使用using简化
    using SizeType = std::vector<int>::size_type;
    SizeType sz2 = 200;
    std::cout << "  SizeType(using) = " << sz2 << std::endl;
    
    // 字符串相关
    using String = std::string;
    using StringVec = std::vector<std::string>;
    
    StringVec words = {"hello", "world"};
    std::cout << "\n  StringVec words: ";
    for (const String& word : words) {
        std::cout << word << " ";
    }
    std::cout << std::endl;
}

void typeAliasDemo() {
    std::cout << "========== EMC++ Item 09: 类型别名 ==========" << std::endl;
    
    // 1. typedef vs using语法对比
    typedefVsUsingDemo();
    
    // 2. 模板别名
    templateAliasDemo();
    
    // 3. 函数指针别名
    functionPointerAliasDemo();
    
    // 4. 标准库中的应用
    standardLibraryAliasDemo();
    
    // 总结
    std::cout << "\n========== 总结 ==========" << std::endl;
    std::cout << "优先使用using的原因：" << std::endl;
    std::cout << "  1. 语法更直观：别名在左，类型在右" << std::endl;
    std::cout << "  2. 支持模板别名：typedef无法实现" << std::endl;
    std::cout << "  3. 函数指针更清晰：名字更突出" << std::endl;
    std::cout << "  4. 可读性更好：统一使用\"=\"形式" << std::endl;
}
