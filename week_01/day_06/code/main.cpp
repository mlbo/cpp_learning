/**
 * @file main.cpp
 * @brief Day 6: 二分查找详解 - 主程序
 * 
 * 本程序演示三种二分查找模板的使用：
 * 1. 标准二分查找
 * 2. 查找左边界
 * 3. 查找右边界
 */

#include <iostream>
#include <vector>
#include <iomanip>

#include "binary_search_basic.cpp"
#include "binary_search_left.cpp"
#include "binary_search_right.cpp"
#include "binary_search_template.cpp"
#include "leetcode/0704_binary_search/solution.h"
#include "leetcode/0034_find_first_and_last/solution.h"

// 打印数组
void printArray(const std::vector<int>& nums) {
    std::cout << "[";
    for (size_t i = 0; i < nums.size(); ++i) {
        std::cout << nums[i];
        if (i < nums.size() - 1) std::cout << ", ";
    }
    std::cout << "]";
}

// 打印分割线
void printSeparator() {
    std::cout << std::string(50, '-') << std::endl;
}

// 演示标准二分查找
void demoBasicBinarySearch() {
    std::cout << "\n📐 演示标准二分查找\n";
    printSeparator();
    
    std::vector<int> nums = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    std::cout << "数组: ";
    printArray(nums);
    std::cout << "\n\n";
    
    // 测试查找存在的元素
    std::vector<int> targets = {7, 1, 19, 6};
    
    for (int target : targets) {
        int result = binarySearch(nums, target);
        std::cout << "查找 " << std::setw(2) << target << ": ";
        if (result != -1) {
            std::cout << "✓ 找到，索引 = " << result;
        } else {
            std::cout << "✗ 未找到";
        }
        std::cout << std::endl;
    }
}

// 演示左边界查找
void demoLeftBinarySearch() {
    std::cout << "\n📐 演示左边界查找\n";
    printSeparator();
    
    std::vector<int> nums = {1, 2, 2, 2, 3, 4, 4, 4, 4, 5};
    std::cout << "数组（有重复元素）: ";
    printArray(nums);
    std::cout << "\n\n";
    
    std::vector<int> targets = {2, 4, 1, 5, 6};
    
    for (int target : targets) {
        int result = binarySearchLeft(nums, target);
        std::cout << "查找 " << std::setw(2) << target << " 的左边界: ";
        if (result != -1) {
            std::cout << "✓ 索引 = " << result;
        } else {
            std::cout << "✗ 未找到";
        }
        std::cout << std::endl;
    }
}

// 演示右边界查找
void demoRightBinarySearch() {
    std::cout << "\n📐 演示右边界查找\n";
    printSeparator();
    
    std::vector<int> nums = {1, 2, 2, 2, 3, 4, 4, 4, 4, 5};
    std::cout << "数组（有重复元素）: ";
    printArray(nums);
    std::cout << "\n\n";
    
    std::vector<int> targets = {2, 4, 1, 5, 6};
    
    for (int target : targets) {
        int result = binarySearchRight(nums, target);
        std::cout << "查找 " << std::setw(2) << target << " 的右边界: ";
        if (result != -1) {
            std::cout << "✓ 索引 = " << result;
        } else {
            std::cout << "✗ 未找到";
        }
        std::cout << std::endl;
    }
}

// 演示通用模板类
void demoBinarySearchTemplate() {
    std::cout << "\n📐 演示通用二分查找模板类\n";
    printSeparator();
    
    std::vector<int> nums = {5, 7, 7, 8, 8, 10};
    std::cout << "数组: ";
    printArray(nums);
    std::cout << "\n\n";
    
    BinarySearchTemplate<int> searcher(nums);
    
    // 查找目标值8的范围
    int target = 8;
    std::cout << "查找目标值 " << target << ":\n";
    std::cout << "  标准查找: 索引 = " << searcher.search(target) << "\n";
    std::cout << "  左边界: 索引 = " << searcher.searchLeft(target) << "\n";
    std::cout << "  右边界: 索引 = " << searcher.searchRight(target) << "\n";
    
    // 查找第一个大于等于目标值的位置
    target = 6;
    std::cout << "\n查找第一个 >= " << target << " 的位置: ";
    std::cout << "索引 = " << searcher.lowerBound(target) << "\n";
    
    // 查找第一个大于目标值的位置
    target = 8;
    std::cout << "查找第一个 > " << target << " 的位置: ";
    std::cout << "索引 = " << searcher.upperBound(target) << "\n";
}

// 演示LeetCode 704
void demoLeetCode704() {
    std::cout << "\n📐 LeetCode 704: 二分查找\n";
    printSeparator();
    
    std::vector<int> nums = {-1, 0, 3, 5, 9, 12};
    int target = 9;
    
    std::cout << "数组: ";
    printArray(nums);
    std::cout << "\n目标: " << target << "\n";
    
    Solution704 solution;
    int result = solution.search(nums, target);
    
    std::cout << "结果: 索引 = " << result << std::endl;
}

// 演示LeetCode 34
void demoLeetCode34() {
    std::cout << "\n📐 LeetCode 34: 查找元素的第一个和最后一个位置\n";
    printSeparator();
    
    std::vector<int> nums = {5, 7, 7, 8, 8, 10};
    int target = 8;
    
    std::cout << "数组: ";
    printArray(nums);
    std::cout << "\n目标: " << target << "\n";
    
    Solution34 solution;
    std::vector<int> result = solution.searchRange(nums, target);
    
    std::cout << "结果: [" << result[0] << ", " << result[1] << "]" << std::endl;
}

int main() {
    std::cout << "============================================\n";
    std::cout << "   Day 6: 二分查找详解\n";
    std::cout << "============================================\n";
    
    // 演示各种二分查找
    demoBasicBinarySearch();
    demoLeftBinarySearch();
    demoRightBinarySearch();
    demoBinarySearchTemplate();
    
    // 演示LeetCode题目
    demoLeetCode704();
    demoLeetCode34();
    
    std::cout << "\n============================================\n";
    std::cout << "   所有演示完成！\n";
    std::cout << "============================================\n";
    
    return 0;
}
