/**
 * @file complexity.cpp
 * @brief 复杂度分析示例代码
 * 
 * 演示不同时间复杂度的算法：
 * - O(1) 常数时间
 * - O(log n) 对数时间
 * - O(n) 线性时间
 * - O(n²) 平方时间
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <iomanip>

// 计时辅助类
class Timer {
private:
    std::chrono::high_resolution_clock::time_point start_;
    std::string name_;
public:
    Timer(const std::string& name) : name_(name) {
        start_ = std::chrono::high_resolution_clock::now();
    }
    ~Timer() {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start_);
        std::cout << "  [" << name_ << "] 耗时: " << duration.count() << " 微秒\n";
    }
};

// ============================================
// O(1) 常数时间复杂度示例
// ============================================

// 访问数组第一个元素
int get_first(const std::vector<int>& arr) {
    return arr[0];  // 无论数组多大，只需一次操作
}

// 计算两数之和
int add(int a, int b) {
    return a + b;  // 固定的计算步骤
}

// ============================================
// O(log n) 对数时间复杂度示例
// ============================================

// 二分查找
int binary_search(const std::vector<int>& arr, int target) {
    int left = 0;
    int right = static_cast<int>(arr.size()) - 1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;  // 防止溢出
        
        if (arr[mid] == target) {
            return mid;
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    return -1;  // 未找到
}

// ============================================
// O(n) 线性时间复杂度示例
// ============================================

// 线性查找
int linear_search(const std::vector<int>& arr, int target) {
    for (size_t i = 0; i < arr.size(); ++i) {
        if (arr[i] == target) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

// 计算数组元素和
long long sum(const std::vector<int>& arr) {
    long long total = 0;
    for (int num : arr) {
        total += num;
    }
    return total;
}

// ============================================
// O(n²) 平方时间复杂度示例
// ============================================

// 冒泡排序
void bubble_sort(std::vector<int>& arr) {
    size_t n = arr.size();
    for (size_t i = 0; i < n - 1; ++i) {
        for (size_t j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// 两数之和（暴力解法）
std::pair<int, int> two_sum_brute_force(const std::vector<int>& arr, int target) {
    for (size_t i = 0; i < arr.size(); ++i) {
        for (size_t j = i + 1; j < arr.size(); ++j) {
            if (arr[i] + arr[j] == target) {
                return {static_cast<int>(i), static_cast<int>(j)};
            }
        }
    }
    return {-1, -1};
}

// ============================================
// 复杂度对比演示
// ============================================

void demonstrate_complexity() {
    // 创建不同大小的测试数据
    std::vector<int> small_data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> large_data;
    for (int i = 0; i < 10000; ++i) {
        large_data.push_back(i);
    }
    
    std::cout << "\n--- O(1) vs O(n) vs O(n²) 对比 ---\n";
    
    // O(1) 测试
    {
        Timer t("O(1) - 获取首元素");
        auto result = get_first(large_data);
        (void)result;  // 避免未使用警告
    }
    
    // O(n) 测试
    {
        Timer t("O(n) - 线性查找");
        auto result = linear_search(large_data, 9999);
        (void)result;
    }
    
    // O(log n) 测试
    {
        Timer t("O(log n) - 二分查找");
        auto result = binary_search(large_data, 9999);
        (void)result;
    }
    
    // O(n²) 测试（使用小数据）
    {
        std::vector<int> test_arr = {5, 3, 8, 1, 9, 2, 7, 4, 6, 0};
        Timer t("O(n²) - 冒泡排序(10个元素)");
        bubble_sort(test_arr);
    }
    
    std::cout << "\n--- 复杂度增长率对比 ---\n";
    std::cout << std::setw(10) << "n" 
              << std::setw(15) << "O(log n)" 
              << std::setw(15) << "O(n)" 
              << std::setw(15) << "O(n log n)" 
              << std::setw(15) << "O(n²)" << "\n";
    std::cout << std::string(70, '-') << "\n";
    
    for (int n : {10, 100, 1000, 10000}) {
        std::cout << std::setw(10) << n 
                  << std::setw(15) << std::log2(n)
                  << std::setw(15) << n 
                  << std::setw(15) << n * std::log2(n) 
                  << std::setw(15) << static_cast<long long>(n) * n << "\n";
    }
}

// ============================================
// 空间复杂度示例
// ============================================

// O(1) 空间 - 原地操作
void reverse_inplace(std::vector<int>& arr) {
    int left = 0;
    int right = static_cast<int>(arr.size()) - 1;
    while (left < right) {
        std::swap(arr[left], arr[right]);
        ++left;
        --right;
    }
}

// O(n) 空间 - 需要额外数组
std::vector<int> create_copy(const std::vector<int>& arr) {
    std::vector<int> copy = arr;  // 复制整个数组
    return copy;
}

// O(n²) 空间 - 二维矩阵
std::vector<std::vector<int>> create_matrix(int n) {
    return std::vector<std::vector<int>>(n, std::vector<int>(n, 0));
}
