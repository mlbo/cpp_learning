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

// 辅助宏
#define PRINT_SEPARATOR() std::cout << "\n" << std::string(60, '=') << "\n"
#define PRINT_TITLE(title) std::cout << "\n【" << title << "】\n"

/**
 * @brief 对比三种数组类型的大小
 * 
 * 原生数组和 std::array 的大小就是元素大小之和
 * std::vector 有额外开销（三个指针）
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
              << " 字节 │ 3个指针开销 (start/finish/end_of_storage) │\n";
    std::cout << "└───────────────────┴────────────────┴─────────────────────────┘\n";
    
    // 计算元素数据大小
    std::cout << "\n元素数据大小: " << N << " * " << sizeof(int) 
              << " = " << N * sizeof(int) << " 字节\n";
    
    // vector 内部结构
    PRINT_SEPARATOR();
    std::cout << "\nstd::vector 内部结构（3个指针）:\n";
    std::cout << "  - start: 指向数组起始位置\n";
    std::cout << "  - finish: 指向最后一个元素之后的位置\n";
    std::cout << "  - end_of_storage: 指向分配内存的末尾\n\n";
    std::cout << "  容量 (capacity) = end_of_storage - start = " << vec.capacity() << "\n";
    std::cout << "  大小 (size) = finish - start = " << vec.size() << "\n";
}

/**
 * @brief 演示内存位置的区别
 * 
 * 原生数组和 std::array 通常在栈上（自动存储期）
 * std::vector 的元素在堆上
 */
void demonstrateMemoryLocation() {
    PRINT_TITLE("2. 内存位置对比");
    
    int stackVar = 42;  // 栈变量
    int nativeArr[100];  // 栈上分配
    std::array<int, 100> stdArr;  // 栈上分配
    std::vector<int> vec(100);  // 元素在堆上
    
    std::cout << "变量地址分析:\n";
    std::cout << "  栈变量地址: " << &stackVar << "\n";
    std::cout << "  原生数组地址: " << nativeArr << " (栈上)\n";
    std::cout << "  std::array地址: " << stdArr.data() << " (栈上)\n";
    std::cout << "  vector数据地址: " << vec.data() << " (堆上)\n";
    
    std::cout << "\n【结论】\n";
    std::cout << "  - 原生数组和 std::array 的地址接近栈变量（在栈上）\n";
    std::cout << "  - vector 的数据地址远离栈变量（在堆上）\n";
    std::cout << "  - 栈上分配更快，但大小受限\n";
    std::cout << "  - 堆上分配可以处理大数据\n";
}

/**
 * @brief 性能对比测试
 */
void demonstratePerformance() {
    PRINT_TITLE("3. 性能对比测试");
    
    const int ITERATIONS = 10000000;  // 1000万次迭代
    const int SIZE = 1000;
    
    // 原生数组
    int* nativeArr = new int[SIZE]();
    
    // std::array
    auto* stdArr = new std::array<int, SIZE>();
    stdArr->fill(0);
    
    // std::vector
    std::vector<int> vec(SIZE, 0);
    
    // 测试1：顺序访问 - 原生数组
    auto start = std::chrono::high_resolution_clock::now();
    for (int iter = 0; iter < ITERATIONS / SIZE; ++iter) {
        for (int i = 0; i < SIZE; ++i) {
            nativeArr[i] += 1;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto nativeTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    
    // 测试2：顺序访问 - std::array
    start = std::chrono::high_resolution_clock::now();
    for (int iter = 0; iter < ITERATIONS / SIZE; ++iter) {
        for (int i = 0; i < SIZE; ++i) {
            (*stdArr)[i] += 1;
        }
    }
    end = std::chrono::high_resolution_clock::now();
    auto arrayTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    
    // 测试3：顺序访问 - std::vector
    start = std::chrono::high_resolution_clock::now();
    for (int iter = 0; iter < ITERATIONS / SIZE; ++iter) {
        for (int i = 0; i < SIZE; ++i) {
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
    std::cout << "  编译器优化后差异几乎可以忽略\n";
    
    delete[] nativeArr;
    delete stdArr;
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
    std::cout << "  - 通常按 2 倍扩容（实现可能不同）\n";
    std::cout << "  - 扩容涉及：分配新内存、复制元素、释放旧内存\n";
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
    int size = sizeof(nativeArr) / sizeof(nativeArr[0]);
    
    std::cout << "原生数组遍历:\n";
    std::cout << "  方式1: 下标循环\n    ";
    for (int i = 0; i < size; ++i) {
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
│ 原生数组        │ - C 风格 API 交互                                  │
│                 │ - 极致性能要求的底层代码                            │
│                 │ - 已知大小的常量数组                                │
├─────────────────┼────────────────────────────────────────────────────┤
│ std::array      │ - 固定大小容器，需要 STL 接口                       │
│                 │ - 编译时已知大小                                    │
│                 │ - 不需要动态扩容                                    │
│                 │ - 性能敏感场景（栈分配）                            │
├─────────────────┼────────────────────────────────────────────────────┤
│ std::vector     │ - 运行时大小可变                                    │
│                 │ - 需要频繁增删元素                                  │
│                 │ - 一般用途的动态数组                                │
│                 │ - 作为默认容器首选                                  │
└─────────────────┴────────────────────────────────────────────────────┘

【最佳实践建议】
1. 默认使用 std::vector，除非有明确理由选择其他
2. 固定大小且已知时，优先 std::array
3. 需要与 C 代码交互时，使用原生数组或 std::array::data()
4. 大数组（超过栈大小限制）使用 std::vector
5. 使用 at() 进行边界检查，[] 用于性能关键代码
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
