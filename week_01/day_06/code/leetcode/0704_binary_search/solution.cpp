/**
 * @file solution.cpp
 * @brief LeetCode 704. 二分查找 - 实现
 */

#include "solution.h"

#include "../../../../common/integer_contracts.h"

/**
 * @brief 迭代版本二分查找
 * 
 * 算法思路：
 * 1. 初始化左右边界
 * 2. 在半开区间 [left, right) 非空时继续循环
 * 3. 每次取中间值进行比较
 * 4. 根据比较结果调整搜索范围
 * 
 * 时间复杂度：O(log n)
 * 空间复杂度：O(1)
 */
int Solution704::search(const std::vector<int>& nums, int target) {
    (void)week01::checked_index(nums.size());
    std::size_t left = 0;
    std::size_t right = nums.size();
    
    // 循环搜索
    while (left < right) {
        // 防溢出计算中间位置
        const std::size_t mid = left + (right - left) / 2;
        
        if (nums[mid] == target) {
            // 找到目标值
            return week01::checked_index(mid);
        } else if (nums[mid] < target) {
            // 目标在右半部分
            left = mid + 1;
        } else {
            // 目标在左半部分
            right = mid;
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
int Solution704::searchRecursive(const std::vector<int>& nums, int target) {
    (void)week01::checked_index(nums.size());
    return binarySearchHelper(nums, target, 0, nums.size());
}

/**
 * @brief 递归辅助函数
 */
int Solution704::binarySearchHelper(const std::vector<int>& nums, int target,
                                    std::size_t left, std::size_t right) {
    // 递归终止条件
    if (left == right) {
        return -1;
    }
    
    // 计算中间位置
    const std::size_t mid = left + (right - left) / 2;
    
    if (nums[mid] == target) {
        return week01::checked_index(mid);
    } else if (nums[mid] < target) {
        // 搜索右半部分
        return binarySearchHelper(nums, target, mid + 1, right);
    } else {
        // 搜索左半部分
        return binarySearchHelper(nums, target, left, mid);
    }
}
