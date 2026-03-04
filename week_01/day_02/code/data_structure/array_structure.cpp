/**
 * @file array_structure.cpp
 * @brief 数组数据结构详解
 * 
 * 内容包括：
 * 1. C风格数组的基本操作
 * 2. 数组的内存布局
 * 3. std::array的使用
 * 4. 数组作为函数参数
 * 5. 多维数组
 */

#include <iostream>
#include <array>
#include <algorithm>
#include <typeinfo>

namespace data_structure {

// ==================== 1. C风格数组基础 ====================

/**
 * @brief 演示C风格数组的基本操作
 */
void demonstrate_c_style_array() {
    // 声明和初始化
    int nums[5] = {1, 2, 3, 4, 5};
    
    // 遍历数组
    std::cout << "数组遍历: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << nums[i] << " ";
    }
    std::cout << std::endl;
    
    // 随机访问 - O(1)
    std::cout << "数组随机访问: nums[2] = " << nums[2] << std::endl;
    
    // 数组大小计算
    size_t size = sizeof(nums) / sizeof(nums[0]);
    std::cout << "数组大小: " << size << std::endl;
}

// ==================== 2. 数组内存布局 ====================

/**
 * @brief 演示数组的内存布局
 */
void demonstrate_memory_layout() {
    int arr[5] = {10, 20, 30, 40, 50};
    
    std::cout << "\n内存布局分析:" << std::endl;
    std::cout << "数组首地址: " << arr << std::endl;
    
    for (int i = 0; i < 5; ++i) {
        std::cout << "arr[" << i << "] = " << arr[i] 
                  << " 地址: " << &arr[i] 
                  << " 偏移: " << ((char*)&arr[i] - (char*)arr) << " 字节"
                  << std::endl;
    }
    
    std::cout << "\n结论: 相邻元素地址差 = sizeof(int) = " << sizeof(int) << " 字节" << std::endl;
}

// ==================== 3. std::array使用 ====================

/**
 * @brief 演示std::array的使用
 */
void demonstrate_std_array() {
    // 创建std::array
    std::array<int, 5> arr = {10, 20, 30, 40, 50};
    
    std::cout << "\nstd::array演示:" << std::endl;
    std::cout << "std::array大小: " << arr.size() << std::endl;
    
    // 边界检查访问
    try {
        arr.at(10);  // 会抛出异常
    } catch (const std::out_of_range& e) {
        // 预期会抛出异常，这里不打印错误
    }
    
    // 安全访问
    std::cout << "std::array元素: ";
    for (const auto& elem : arr) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    
    // 使用STL算法
    std::sort(arr.begin(), arr.end(), std::greater<int>());
    std::cout << "降序排序后: ";
    for (const auto& elem : arr) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

// ==================== 4. 多维数组 ====================

/**
 * @brief 演示多维数组
 */
void demonstrate_multidimensional_array() {
    // 2D数组
    int matrix[3][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };
    
    std::cout << "\n二维数组演示:" << std::endl;
    std::cout << "matrix[1][1] = " << matrix[1][1] << std::endl;
    
    // 内存布局：行优先
    std::cout << "行优先存储，地址连续" << std::endl;
    
    // 打印二维数组
    std::cout << "二维数组内容:" << std::endl;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << matrix[i][j] << "\t";
        }
        std::cout << std::endl;
    }
}

// ==================== 5. 数组操作时间复杂度 ====================

/**
 * @brief 演示数组操作的时间复杂度
 */
void demonstrate_time_complexity() {
    std::cout << "\n数组操作时间复杂度:" << std::endl;
    std::cout << "┌────────────┬─────────────┐" << std::endl;
    std::cout << "│    操作    │ 时间复杂度  │" << std::endl;
    std::cout << "├────────────┼─────────────┤" << std::endl;
    std::cout << "│  随机访问  │    O(1)     │" << std::endl;
    std::cout << "│  顺序搜索  │    O(n)     │" << std::endl;
    std::cout << "│  二分搜索  │   O(log n)  │" << std::endl;
    std::cout << "│  尾部插入  │    O(1)     │" << std::endl;
    std::cout << "│  中间插入  │    O(n)     │" << std::endl;
    std::cout << "│  尾部删除  │    O(1)     │" << std::endl;
    std::cout << "│  中间删除  │    O(n)     │" << std::endl;
    std::cout << "└────────────┴─────────────┘" << std::endl;
}

// ==================== 主演示函数 ====================

void demonstrate_array() {
    demonstrate_c_style_array();
    demonstrate_memory_layout();
    demonstrate_std_array();
    demonstrate_multidimensional_array();
    demonstrate_time_complexity();
}

} // namespace data_structure
