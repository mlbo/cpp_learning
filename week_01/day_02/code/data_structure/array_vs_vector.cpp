/**
 * @file array_vs_vector.cpp
 * @brief array 与 vector 对比分析
 * 
 * 本文件详细讲解：
 * 1. 原生数组、std::array、std::vector 的区别
 * 2. 内存布局对比
 * 3. 性能特点对比
 * 4. 使用场景选择
 */

#include <iostream>
#include <array>
#include <vector>
#include <chrono>
#include <iomanip>
#include <memory>

// 辅助宏
#define PRINT_SEPARATOR() std::cout << "\n" << std::string(60, '=') << "\n"
#define PRINT_TITLE(title) std::cout << "\n【" << title << "】\n"

/**
 * @brief 对比三种数组类型的大小
 * 
 * 原生数组和 std::array 的大小就是元素大小之和
 * std::vector 管理对象有实现相关的固定开销，元素存储不计入 sizeof(vec)
 */
void demonstrateSizeDifference() {
    PRINT_TITLE("1. 内存大小对比");
    
    const int N = 100;
    
    // 原生数组
    int nativeArr[N];
    
    // std::array
    std::array<int, N> stdArr;
    
    // std::vector
    std::vector<int> vec(N);
    
    std::cout << "数组大小: " << N << " 个 int 元素\n\n";
    
    std::cout << "┌───────────────────┬────────────────┬─────────────────────────┐\n";
    std::cout << "│       类型        │    内存大小     │          说明           │\n";
    std::cout << "├───────────────────┼────────────────┼─────────────────────────┤\n";
    std::cout << "│ 原生数组 int[N]   │" << std::setw(11) << sizeof(nativeArr) 
              << " 字节 │ 无额外开销              │\n";
    std::cout << "│ std::array<int,N> │" << std::setw(11) << sizeof(stdArr) 
              << " 字节 │ 无额外开销              │\n";
    std::cout << "│ std::vector<int>  │" << std::setw(11) << sizeof(vec) 
              << " 字节 │ 管理对象开销（实现相关，不含元素区） │\n";
    std::cout << "└───────────────────┴────────────────┴─────────────────────────┘\n";
    
    // 计算元素数据大小
    std::cout << "\n元素数据大小: " << N << " * " << sizeof(int) 
              << " = " << N * sizeof(int) << " 字节\n";
    
    // vector 内部结构（只描述常见实现思路，不要求固定为三个指针）
    PRINT_SEPARATOR();
    std::cout << "\nstd::vector 常见实现会记录：\n";
    std::cout << "  - 连续元素区的位置\n";
    std::cout << "  - 已构造元素数量（size）\n";
    std::cout << "  - 当前可容纳元素数量（capacity）\n";
    std::cout << "标准只规定可观察行为，不规定必须恰好用三个指针实现。\n\n";
    std::cout << "  容量 (capacity) = " << vec.capacity() << "\n";
    std::cout << "  大小 (size) = " << vec.size() << "\n";
}

/**
 * @brief 演示内存位置的区别
 * 
 * 本函数中的原生数组和 std::array 具有自动存储期，常见实现放在线程栈中；
 * vector 的元素来自动态分配存储。标准不要求具体的“栈/堆”布局。
 */
void demonstrateMemoryLocation() {
    PRINT_TITLE("2. 内存位置对比");
    
    int stackVar = 42;  // 栈变量
    int nativeArr[100];  // 自动存储期
    std::array<int, 100> stdArr;  // 自动存储期
    std::vector<int> vec(100);  // 元素使用动态分配存储
    
    std::cout << "变量地址分析:\n";
    std::cout << "  自动存储期变量地址: " << &stackVar << "\n";
    std::cout << "  原生数组元素地址: " << nativeArr << "\n";
    std::cout << "  std::array元素地址: " << stdArr.data() << "\n";
    std::cout << "  vector动态元素区地址: " << vec.data() << "\n";
    
    std::cout << "\n【结论】\n";
    std::cout << "  - 地址只展示本次运行的常见布局，不能据此推出标准保证\n";
    std::cout << "  - 存储位置本身不能直接决定遍历速度；三者的元素都可连续存放\n";
    std::cout << "  - vector 需要动态分配，但能在运行期调整容量并处理较大数据\n";
}

/**
 * @brief 性能对比测试
 */
void demonstratePerformance() {
    PRINT_TITLE("3. 性能对比测试");
    
    const int ITERATIONS = 10000000;  // 1000万次迭代
    const int SIZE = 1000;
    
    // 原生数组
    auto nativeArr = std::make_unique<int[]>(SIZE);
    
    // std::array
    auto stdArr = std::make_unique<std::array<int, SIZE>>();
    stdArr->fill(0);
    
    // std::vector
    std::vector<int> vec(SIZE, 0);
    
    // 测试1：顺序访问 - 原生数组
    auto start = std::chrono::high_resolution_clock::now();
    for (int iter = 0; iter < ITERATIONS / SIZE; ++iter) {
        for (std::size_t i = 0; i < static_cast<std::size_t>(SIZE); ++i) {
            nativeArr[i] += 1;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto nativeTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    
    // 测试2：顺序访问 - std::array
    start = std::chrono::high_resolution_clock::now();
    for (int iter = 0; iter < ITERATIONS / SIZE; ++iter) {
        for (std::size_t i = 0; i < static_cast<std::size_t>(SIZE); ++i) {
            (*stdArr)[i] += 1;
        }
    }
    end = std::chrono::high_resolution_clock::now();
    auto arrayTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    
    // 测试3：顺序访问 - std::vector
    start = std::chrono::high_resolution_clock::now();
    for (int iter = 0; iter < ITERATIONS / SIZE; ++iter) {
        for (std::size_t i = 0; i < static_cast<std::size_t>(SIZE); ++i) {
            vec[i] += 1;
        }
    }
    end = std::chrono::high_resolution_clock::now();
    auto vectorTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    
    std::cout << "测试：顺序访问 " << ITERATIONS << " 次\n\n";
    std::cout << "┌───────────────────┬────────────────┐\n";
    std::cout << "│       类型        │    耗时(微秒)   │\n";
    std::cout << "├───────────────────┼────────────────┤\n";
    std::cout << "│ 原生数组          │" << std::setw(11) << nativeTime << " │\n";
    std::cout << "│ std::array        │" << std::setw(11) << arrayTime << " │\n";
    std::cout << "│ std::vector       │" << std::setw(11) << vectorTime << " │\n";
    std::cout << "└───────────────────┴────────────────┘\n";
    
    std::cout << "\n【结论】\n";
    std::cout << "  顺序访问性能接近（都是连续内存，缓存友好）\n";
    std::cout << "  单次微基准受优化、缓存和机器影响，不能证明所有场景性能相同\n";
    
    std::cout << "  两块动态存储都由 unique_ptr 独占，异常或正常返回都会自动释放\n";
}

/**
 * @brief 演示动态扩展能力
 */
void demonstrateDynamicResize() {
    PRINT_TITLE("4. 动态扩展能力对比");
    
    // 原生数组和 std::array：大小固定
    std::cout << "原生数组和 std::array:\n";
    std::cout << "  大小在编译时确定，运行时无法改变\n";
    std::cout << "  如果需要更多空间，必须重新分配并复制\n\n";
    
    // std::vector：动态扩展
    std::cout << "std::vector 动态扩展示例:\n";
    std::vector<int> vec;
    
    std::cout << "初始状态: size=" << vec.size() << ", capacity=" << vec.capacity() << "\n";
    
    for (int i = 0; i < 20; ++i) {
        vec.push_back(i);
        std::cout << "push_back(" << i << "): size=" << vec.size() 
                  << ", capacity=" << vec.capacity() << "\n";
    }
    
    std::cout << "\n【扩容策略】\n";
    std::cout << "  - 当 size == capacity 时需要扩容\n";
    std::cout << "  - 容量按某个增长策略扩大，具体倍率由实现决定\n";
    std::cout << "  - 扩容涉及：分配新内存、移动或复制元素、销毁旧元素并释放旧内存\n";
    std::cout << "  - 均摊时间复杂度：O(1)\n";
    
    // 预分配优化
    PRINT_SEPARATOR();
    std::cout << "\n优化技巧：使用 reserve() 预分配\n";
    
    std::vector<int> optimized;
    optimized.reserve(20);  // 预分配空间
    
    std::cout << "reserve(20)后: size=" << optimized.size() 
              << ", capacity=" << optimized.capacity() << "\n";
    
    for (int i = 0; i < 20; ++i) {
        optimized.push_back(i);
    }
    std::cout << "插入20个元素后: size=" << optimized.size() 
              << ", capacity=" << optimized.capacity() << " (无扩容)\n";
}

/**
 * @brief 演示迭代器支持
 */
void demonstrateIteratorSupport() {
    PRINT_TITLE("5. 迭代器支持对比");
    
    // 原生数组：有限支持
    int nativeArr[] = {1, 2, 3, 4, 5};
    const std::size_t size = sizeof(nativeArr) / sizeof(nativeArr[0]);
    
    std::cout << "原生数组遍历:\n";
    std::cout << "  方式1: 下标循环\n    ";
    for (std::size_t i = 0; i < size; ++i) {
        std::cout << nativeArr[i] << " ";
    }
    
    std::cout << "\n  方式2: 指针迭代\n    ";
    for (int* p = nativeArr; p != nativeArr + size; ++p) {
        std::cout << *p << " ";
    }
    
    // std::array 和 std::vector：完整迭代器支持
    PRINT_SEPARATOR();
    std::array<int, 5> stdArr = {1, 2, 3, 4, 5};
    
    std::cout << "\nstd::array/std::vector 遍历:\n";
    
    std::cout << "  方式1: 下标循环\n    ";
    for (size_t i = 0; i < stdArr.size(); ++i) {
        std::cout << stdArr[i] << " ";
    }
    
    std::cout << "\n  方式2: 迭代器\n    ";
    for (auto it = stdArr.begin(); it != stdArr.end(); ++it) {
        std::cout << *it << " ";
    }
    
    std::cout << "\n  方式3: range-based for (C++11)\n    ";
    for (int x : stdArr) {
        std::cout << x << " ";
    }
    
    std::cout << "\n\n迭代器优势:\n";
    std::cout << "  - 与 STL 算法无缝配合\n";
    std::cout << "  - 统一的遍历接口\n";
    std::cout << "  - 支持反向迭代\n";
}

/**
 * @brief 使用场景选择指南
 */
void printSelectionGuide() {
    PRINT_TITLE("6. 使用场景选择指南");
    
    std::cout << R"(
┌─────────────────┬────────────────────────────────────────────────────┐
│      类型       │                    适用场景                         │
├─────────────────┼────────────────────────────────────────────────────┤
│ 原生数组        │ - C 风格 API 或特定底层布局交互                      │
│                 │ - 维护既有 C/C++ 接口                                │
├─────────────────┼────────────────────────────────────────────────────┤
│ std::array      │ - 固定大小容器，需要 STL 接口                       │
│                 │ - 编译时已知大小                                    │
│                 │ - 不需要动态扩容                                    │
│                 │ - 希望大小成为类型一部分                            │
├─────────────────┼────────────────────────────────────────────────────┤
│ std::vector     │ - 运行时大小可变                                    │
│                 │ - 需要在尾部动态增删元素                            │
│                 │ - 一般用途的动态数组                                │
│                 │ - 作为默认容器首选                                  │
└─────────────────┴────────────────────────────────────────────────────┘

【最佳实践建议】
1. 默认使用 std::vector，除非有明确理由选择其他
2. 固定大小且已知时，优先 std::array
3. 需要与 C 代码交互时，使用原生数组或 std::array::data()
4. 大数组（超过栈大小限制）使用 std::vector
5. 下标范围尚未证明时使用 at()；证明不变量后再按需要使用 []
)";
}

/**
 * @brief main 函数
 */
int main() {
    PRINT_SEPARATOR();
    std::cout << "      array 与 vector 对比分析 - Day 2";
    PRINT_SEPARATOR();
    
    demonstrateSizeDifference();
    demonstrateMemoryLocation();
    demonstratePerformance();
    demonstrateDynamicResize();
    demonstrateIteratorSupport();
    printSelectionGuide();
    
    PRINT_SEPARATOR();
    std::cout << "对比分析结束！\n";
    PRINT_SEPARATOR();
    
    return 0;
}
