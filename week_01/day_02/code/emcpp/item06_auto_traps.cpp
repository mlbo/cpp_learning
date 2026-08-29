/**
 * @file item06_auto_traps.cpp
 * @brief EMC++ 条款6：auto推导陷阱
 * 
 * 主要陷阱：
 * 1. 列表初始化 - 带等号的 auto 花括号初始化推导为 std::initializer_list
 * 2. 代理类问题 - vector<bool>::operator[]返回代理对象
 * 3. 类型不匹配 - 隐式类型转换可能导致bug
 */

#include <iostream>
#include <vector>
#include <array>
#include <typeinfo>
#include <memory>
#include <type_traits>

namespace emcpp {

// ==================== 陷阱1：大括号初始化 ====================

/**
 * @brief 演示auto与大括号初始化的陷阱
 * 
 * auto x = {42}; 推导为 std::initializer_list<int>，
 * auto x{42}; 则推导为 int（单元素直接列表初始化）。
 */
void demonstrate_brace_init_trap() {
    std::cout << "陷阱1 - 大括号初始化:" << std::endl;
    
    // 使用等号初始化
    auto x1 = 42;           // int
    std::cout << "auto x1 = 42 -> " << typeid(x1).name() << std::endl;
    
    // 带等号的列表初始化会推导 std::initializer_list
    auto x2 = {42};         // std::initializer_list<int>
    std::cout << "auto x2 = {42} -> " << typeid(x2).name() << std::endl;
    
    // 多元素大括号初始化
    auto x3 = {1, 2, 3};    // std::initializer_list<int>
    std::cout << "auto x3 = {1,2,3} -> " << typeid(x3).name() << std::endl;
    
    // 推荐：直接初始化
    auto x4 = 42;           // 推荐
    int x5 = {42};          // 或者显式指定类型
    
    // 验证类型
    static_assert(std::is_same_v<decltype(x1), int>);
    static_assert(std::is_same_v<decltype(x2), std::initializer_list<int>>);
    (void)x4; (void)x5;
    
    std::cout << "\n最佳实践: 使用 auto x = 42; 而非 auto x = {42};" << std::endl;
}

// ==================== 陷阱2：代理类问题 ====================

/**
 * @brief 演示vector<bool>的代理类陷阱
 * 
 * vector<bool> 是标准库对 bool 的空间优化特化，operator[] 返回代理对象。
 * 代理不会在当前完整表达式结束时自动失效，但它依赖 vector 的底层存储。
 */
void demonstrate_proxy_class_trap() {
    std::cout << "\n陷阱2 - vector<bool>代理类问题:" << std::endl;
    
    std::vector<bool> vec = {true, false, true, false};
    
    // 问题代码：auto推导为代理类型
    // auto x = vec[0];  // x 是 std::vector<bool>::reference 代理对象
    // 容器发生重新分配、被清空或销毁后再使用它，才可能导致未定义行为
    
    std::cout << "  直接使用auto会保存依赖底层存储的代理对象" << std::endl;
    std::cout << "  正确做法: bool val = vec[0];" << std::endl;
    
    // 正确做法1：显式转换为bool
    bool val1 = vec[0];     // 显式类型
    
    // 正确做法2：使用static_cast
    auto val2 = static_cast<bool>(vec[1]);
    
    // 错误示例（仅演示，不要在生产代码中使用）：
    // auto bad = vec[0];
    // 这里 bad 是代理对象；如果 vec 的底层存储失效，bad 也可能失效
    
    std::cout << "  val1 = " << val1 << ", val2 = " << val2 << std::endl;
}

/**
 * @brief 演示其他代理类陷阱
 */
void demonstrate_other_proxy_traps() {
    std::cout << "\n其他代理类示例:" << std::endl;
    
    // 某些矩阵库的代理类
    // auto row = matrix[0];  // 可能是代理类
    
    // 某些字符串视图
    // auto sv = someStringOperation();  // 可能返回临时对象
    
    std::cout << "  规则: 了解你使用的库是否返回代理类" << std::endl;
}

// ==================== 陷阱3：类型不匹配 ====================

/**
 * @brief 演示类型不匹配的陷阱
 */
void demonstrate_type_mismatch_trap() {
    std::cout << "\n陷阱3 - 类型不匹配:" << std::endl;
    
    std::vector<int> vec = {1, 2, 3, 4, 5};
    
    // 问题代码：使用无符号数
    // auto size = vec.size();  // size_t (无符号)
    // int index = -1;
    // if (index < size) { ... }  // 结果可能违背直觉：index 会先转换为无符号数
    
    std::cout << "  vec.size() 返回 size_t (无符号)" << std::endl;
    std::cout << "  与有符号数比较可能导致意外结果" << std::endl;
    
    // 先保持接口的真实类型；若业务确实需要有符号索引，应先检查范围再转换
    auto size = vec.size();
    (void)size;
    std::cout << "  保持真实类型: auto size = vec.size();" << std::endl;
    std::cout << "  若允许负数索引，统一使用有符号类型并在转换前检查范围" << std::endl;
    
    // 另一个例子：数值范围
    // auto smallVal = someInt32Function();
    // auto largeVal = someInt64Function();
    // auto sum = smallVal + largeVal;  // 类型取决于平台
    
    std::cout << "  注意数值类型的隐式转换" << std::endl;
}

// ==================== 陷阱4：数组退化 ====================

/**
 * @brief 演示auto与数组的陷阱
 */
void demonstrate_array_decay_trap() {
    std::cout << "\n陷阱4 - 数组退化为指针:" << std::endl;
    
    int arr[5] = {1, 2, 3, 4, 5};
    
    // auto会使数组退化为指针
    auto ptr = arr;          // int*
    
    // decltype保留数组类型
    decltype(arr) arr2 = {6, 7, 8, 9, 10};  // int[5]
    
    std::cout << "auto数组推导: " << typeid(ptr).name() << " (指针)" << std::endl;
    std::cout << "decltype数组推导: " << typeid(arr2).name() << " (数组)" << std::endl;
    
    // 推荐：使用std::array
    std::array<int, 5> stdArr = {1, 2, 3, 4, 5};
    auto arrCopy = stdArr;   // std::array<int, 5> - 不会退化
    
    std::cout << "推荐使用 std::array 避免数组退化" << std::endl;
    (void)arrCopy;
}

// ==================== 陷阱5：顶层const忽略 ====================

/**
 * @brief 演示auto忽略顶层const的陷阱
 */
void demonstrate_const_trap() {
    std::cout << "\n陷阱5 - 顶层const忽略:" << std::endl;
    
    const int x = 10;
    
    // auto忽略顶层const
    auto a = x;              // int (const被忽略)
    
    // 需要显式添加const
    const auto b = x;        // const int
    
    // 但底层const会保留
    const int& ref = x;
    auto c = ref;            // int (顶层const和引用都忽略)
    const auto& d = ref;     // const int& (显式添加)
    
    std::cout << "auto a = const int -> " << typeid(a).name() << " (const被忽略)" << std::endl;
    std::cout << "const auto b = const int -> const int (显式保留)" << std::endl;
    (void)b; (void)c; (void)d;
    
    std::cout << "\n建议: 如果需要const，显式写出 const auto" << std::endl;
}

// ==================== 最佳实践总结 ====================

/**
 * @brief 总结auto使用的最佳实践
 */
void print_best_practices() {
    std::cout << "\n=== auto最佳实践 ===" << std::endl;
    std::cout << "┌─────────────────────────────────────────────────────┐" << std::endl;
    std::cout << "│ 1. 优先使用 auto 而非显式类型声明                   │" << std::endl;
    std::cout << "│ 2. 避免 auto x = { ... } 形式的初始化               │" << std::endl;
    std::cout << "│ 3. 警惕 vector<bool> 等代理类                       │" << std::endl;
    std::cout << "│ 4. 注意有符号/无符号数的比较                         │" << std::endl;
    std::cout << "│ 5. 需要const时，显式写出 const auto                 │" << std::endl;
    std::cout << "│ 6. 使用 std::array 替代 C 风格数组                  │" << std::endl;
    std::cout << "└─────────────────────────────────────────────────────┘" << std::endl;
}

// ==================== 主演示函数 ====================

void demonstrate_auto_traps() {
    demonstrate_brace_init_trap();
    demonstrate_proxy_class_trap();
    demonstrate_other_proxy_traps();
    demonstrate_type_mismatch_trap();
    demonstrate_array_decay_trap();
    demonstrate_const_trap();
    print_best_practices();
}

} // namespace emcpp
