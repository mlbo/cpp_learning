/**
 * @file solution.cpp
 * @brief LeetCode 704. 二分查找 - 实现
 */

#include "solution.h"

/**
 * @brief 迭代版本二分查找
 * 
 * 算法思路：
 * 1. 初始化左右边界
 * 2. 循环直到左边界超过右边界
 * 3. 每次取中间值进行比较
 * 4. 根据比较结果调整搜索范围
 * 
 * 时间复杂度：O(log n)
 * 空间复杂度：O(1)
 */
int Solution704::search(std::vector<int>& nums, int target) {
    // 边界检查
    if (nums.empty()) {
        return -1;
    }
    
    // 初始化搜索区间 [left, right]
    int left = 0;
    int right = static_cast<int>(nums.size()) - 1;
    
    // 循环搜索
    while (left <= right) {
        // 防溢出计算中间位置
        int mid = left + (right - left) / 2;
        
        if (nums[mid] == target) {
            // 找到目标值
            return mid;
        } else if (nums[mid] < target) {
            // 目标在右半部分
            left = mid + 1;
        } else {
            // 目标在左半部分
            right = mid - 1;
        }
    }
    
    // 未找到
    return -1;
}

/**
 * @brief 递归版本二分查找
 * 
 * 时间复杂度：O(log n)
 * 空间复杂度：O(log n) - 递归调用栈
 */
int Solution704::searchRecursive(std::vector<int>& nums, int target) {
    if (nums.empty()) {
        return -1;
    }
    return binarySearchHelper(nums, target, 0, static_cast<int>(nums.size()) - 1);
}

/**
 * @brief 递归辅助函数
 */
int Solution704::binarySearchHelper(std::vector<int>& nums, int target, int left, int right) {
    // 递归终止条件
    if (left > right) {
        return -1;
    }
    
    // 计算中间位置
    int mid = left + (right - left) / 2;
    
    if (nums[mid] == target) {
        return mid;
    } else if (nums[mid] < target) {
        // 搜索右半部分
        return binarySearchHelper(nums, target, mid + 1, right);
    } else {
        // 搜索左半部分
        return binarySearchHelper(nums, target, left, mid - 1);
    }
}
