/**
 * @file solution.cpp
 * @brief LeetCode 34. 在排序数组中查找元素的第一个和最后一个位置 - 实现
 */

#include "solution.h"

/**
 * @brief 查找目标值的起始和结束位置
 * 
 * 算法思路：
 * 1. 使用二分查找找到左边界（第一个等于target的位置）
 * 2. 如果左边界不存在，返回 {-1, -1}
 * 3. 使用二分查找找到右边界（最后一个等于target的位置）
 * 4. 返回 {左边界, 右边界}
 * 
 * 时间复杂度：O(log n)
 * 空间复杂度：O(1)
 */
std::vector<int> Solution34::searchRange(std::vector<int>& nums, int target) {
    // 查找左边界
    int leftBound = binarySearchLeft(nums, target);
    
    // 如果左边界不存在，目标值不在数组中
    if (leftBound == -1) {
        return {-1, -1};
    }
    
    // 查找右边界
    int rightBound = binarySearchRight(nums, target);
    
    return {leftBound, rightBound};
}

/**
 * @brief 查找左边界（公开接口）
 */
int Solution34::findLeftBound(std::vector<int>& nums, int target) {
    return binarySearchLeft(nums, target);
}

/**
 * @brief 查找右边界（公开接口）
 */
int Solution34::findRightBound(std::vector<int>& nums, int target) {
    return binarySearchRight(nums, target);
}

/**
 * @brief 二分查找左边界
 * 
 * 使用左闭右开区间 [left, right)
 * 找到第一个 >= target 的位置，然后检查是否等于 target
 */
int Solution34::binarySearchLeft(std::vector<int>& nums, int target) {
    if (nums.empty()) {
        return -1;
    }
    
    int left = 0;
    int right = static_cast<int>(nums.size());
    
    // 查找第一个 >= target 的位置
    while (left < right) {
        int mid = left + (right - left) / 2;
        
        if (nums[mid] >= target) {
            right = mid;  // 收缩右边界
        } else {
            left = mid + 1;  // 收缩左边界
        }
    }
    
    // 检查是否找到
    // left == right，需要检查是否在数组范围内且等于target
    if (left == static_cast<int>(nums.size()) || nums[left] != target) {
        return -1;
    }
    
    return left;
}

/**
 * @brief 二分查找右边界
 * 
 * 使用左闭右开区间 [left, right)
 * 找到第一个 > target 的位置，然后返回前一个位置
 */
int Solution34::binarySearchRight(std::vector<int>& nums, int target) {
    if (nums.empty()) {
        return -1;
    }
    
    int left = 0;
    int right = static_cast<int>(nums.size());
    
    // 查找第一个 > target 的位置
    while (left < right) {
        int mid = left + (right - left) / 2;
        
        if (nums[mid] <= target) {
            left = mid + 1;  // 收缩左边界
        } else {
            right = mid;  // 收缩右边界
        }
    }
    
    // left 指向第一个 > target 的位置
    // left - 1 就是最后一个 <= target 的位置
    // 需要检查是否等于 target
    
    if (left == 0 || nums[left - 1] != target) {
        return -1;
    }
    
    return left - 1;
}

/**
 * 备选实现方式：使用闭区间的二分查找
 * 
 * int findLeftBoundClosed(vector<int>& nums, int target) {
 *     if (nums.empty()) return -1;
 *     
 *     int left = 0, right = nums.size() - 1;
 *     int result = -1;
 *     
 *     while (left <= right) {
 *         int mid = left + (right - left) / 2;
 *         
 *         if (nums[mid] == target) {
 *             result = mid;
 *             right = mid - 1;  // 继续向左找
 *         } else if (nums[mid] < target) {
 *             left = mid + 1;
 *         } else {
 *             right = mid - 1;
 *         }
 *     }
 *     
 *     return result;
 * }
 * 
 * int findRightBoundClosed(vector<int>& nums, int target) {
 *     if (nums.empty()) return -1;
 *     
 *     int left = 0, right = nums.size() - 1;
 *     int result = -1;
 *     
 *     while (left <= right) {
 *         int mid = left + (right - left) / 2;
 *         
 *         if (nums[mid] == target) {
 *             result = mid;
 *             left = mid + 1;  // 继续向右找
 *         } else if (nums[mid] < target) {
 *             left = mid + 1;
 *         } else {
 *             right = mid - 1;
 *         }
 *     }
 *     
 *     return result;
 * }
 */
