/**
 * @file binary_search_right.cpp
 * @brief 查找右边界二分查找实现
 * 
 * 右边界查找模板：
 * - 搜索区间：左闭右开区间 [left, right)
 * - 循环条件：left < right
 * - 找到目标时：继续收缩左边界
 * 
 * 时间复杂度：O(log n)
 * 空间复杂度：O(1)
 */

#ifndef BINARY_SEARCH_RIGHT_HPP
#define BINARY_SEARCH_RIGHT_HPP

#include <vector>

/**
 * @brief 查找目标值的右边界（最后一个出现位置）
 * @param nums 已排序的数组（升序，可以有重复元素）
 * @param target 目标值
 * @return 目标值最后一次出现的索引，如果不存在返回-1
 * 
 * 使用场景：
 * - 查找目标值最后一次出现的位置
 * - 查找最后一个小于等于目标值的位置
 * 
 * 示例：
 * nums = [1, 2, 2, 2, 3], target = 2
 * 返回 3（最后一个2的位置）
 */
int binarySearchRight(const std::vector<int>& nums, int target) {
    // 边界检查：空数组
    if (nums.empty()) {
        return -1;
    }
    
    // 初始化搜索区间：左闭右开 [left, right)
    int left = 0;
    int right = static_cast<int>(nums.size());
    
    // 循环条件：left < right
    while (left < right) {
        // 计算中间位置（防溢出写法）
        int mid = left + (right - left) / 2;
        
        if (nums[mid] <= target) {
            // nums[mid] <= target，说明右边界在 (mid, right] 区间
            // 收缩左边界，不包含 mid
            left = mid + 1;
        } else {
            // nums[mid] > target，说明右边界在 [left, mid) 区间
            // 收缩右边界，不包含 mid
            right = mid;
        }
    }
    
    // 循环结束后，left == right
    // 此时 left 指向第一个大于 target 的位置
    // 所以右边界是 left - 1
    
    // 情况1：left == 0（target比所有元素都小）
    if (left == 0) {
        return -1;
    }
    
    // 情况2：nums[left - 1] 不等于 target（target不存在）
    if (nums[left - 1] != target) {
        return -1;
    }
    
    // 情况3：找到目标值的右边界
    return left - 1;
}

/**
 * @brief 查找第一个大于目标值的位置（upper_bound）
 * @param nums 已排序的数组（升序）
 * @param target 目标值
 * @return 第一个 > target 的索引
 * 
 * 与 binarySearchRight 的关系：
 * - upperBound - 1 就是右边界的位置（如果存在的话）
 * 
 * 示例：
 * nums = [1, 3, 5, 7, 9], target = 5
 * 返回 3（第一个 > 5 的元素是 7）
 */
int upperBound(const std::vector<int>& nums, int target) {
    if (nums.empty()) {
        return 0;
    }
    
    int left = 0;
    int right = static_cast<int>(nums.size());
    
    while (left < right) {
        int mid = left + (right - left) / 2;
        
        if (nums[mid] <= target) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    
    return left;
}

/**
 * @brief 查找右边界（简化版，使用闭区间）
 * @param nums 已排序的数组
 * @param target 目标值
 * @return 目标值最后一次出现的索引
 * 
 * 这是另一种实现方式，使用闭区间 [left, right]
 */
int binarySearchRightClosed(const std::vector<int>& nums, int target) {
    if (nums.empty()) {
        return -1;
    }
    
    int left = 0;
    int right = static_cast<int>(nums.size()) - 1;
    
    // 记录找到的右边界
    int result = -1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (nums[mid] == target) {
            // 找到目标，记录位置，继续向右找
            result = mid;
            left = mid + 1;
        } else if (nums[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    return result;
}

/**
 * @brief 查找最后一个小于等于目标值的位置
 * @param nums 已排序的数组（升序）
 * @param target 目标值
 * @return 最后一个 <= target 的索引
 * 
 * 示例：
 * nums = [1, 3, 5, 7, 9], target = 4
 * 返回 1（最后一个 <= 4 的元素是 3）
 */
int lastLessOrEqual(const std::vector<int>& nums, int target) {
    if (nums.empty()) {
        return -1;
    }
    
    int left = 0;
    int right = static_cast<int>(nums.size());
    
    while (left < right) {
        int mid = left + (right - left) / 2;
        
        if (nums[mid] <= target) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    
    // left 指向第一个 > target 的位置
    // left - 1 就是最后一个 <= target 的位置
    
    if (left == 0) {
        return -1;  // 所有元素都 > target
    }
    
    return left - 1;
}

#endif // BINARY_SEARCH_RIGHT_HPP
