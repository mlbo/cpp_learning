/**
 * @file binary_search_left.cpp
 * @brief 查找左边界二分查找实现
 * 
 * 左边界查找模板：
 * - 搜索区间：左闭右开区间 [left, right)
 * - 循环条件：left < right
 * - 找到目标时：继续收缩右边界
 * 
 * 时间复杂度：O(log n)
 * 空间复杂度：O(1)
 */

#ifndef BINARY_SEARCH_LEFT_HPP
#define BINARY_SEARCH_LEFT_HPP

#include <vector>

#include "../../../common/integer_contracts.h"

/**
 * @brief 查找目标值的左边界（第一个出现位置）
 * @param nums 已排序的数组（升序，可以有重复元素）
 * @param target 目标值
 * @return 目标值第一次出现的索引，如果不存在返回-1
 * 
 * 使用场景：
 * - 查找目标值第一次出现的位置
 * - 查找第一个大于等于目标值的位置（lower_bound）
 * 
 * 示例：
 * nums = [1, 2, 2, 2, 3], target = 2
 * 返回 1（第一个2的位置）
 */
int binarySearchLeft(const std::vector<int>& nums, int target) {
    // 边界检查：空数组
    if (nums.empty()) {
        return -1;
    }
    
    // 初始化搜索区间：左闭右开 [left, right)
    (void)week01::checked_index(nums.size());
    std::size_t left = 0;
    std::size_t right = nums.size();
    
    // 循环条件：left < right
    // 当 left == right 时，搜索区间为空，循环结束
    while (left < right) {
        // 计算中间位置（防溢出写法）
        const std::size_t mid = left + (right - left) / 2;
        
        if (nums[mid] >= target) {
            // nums[mid] >= target，把 mid 纳入已确认后缀；待判定元素变为 [left, mid)
            right = mid;
        } else {
            // nums[mid] < target，把 [left, mid] 排除；待判定元素变为 [mid + 1, right)
            left = mid + 1;
        }
    }
    
    // 循环结束后，left == right
    // 需要检查是否真的找到了目标
    
    // 情况1：left 超出数组范围（target比所有元素都大）
    if (left == nums.size()) {
        return -1;
    }
    
    // 情况2：nums[left] 不等于 target（target不存在）
    if (nums[left] != target) {
        return -1;
    }
    
    // 情况3：找到目标值的左边界
    return week01::checked_index(left);
}

/**
 * @brief 查找第一个大于等于目标值的位置（lower_bound）
 * @param nums 已排序的数组（升序）
 * @param target 目标值
 * @return 第一个 >= target 的索引
 * 
 * 与 binarySearchLeft 的区别：
 * - 此函数不检查 nums[left] == target
 * - 如果 target 大于所有元素，返回 nums.size()
 * 
 * 示例：
 * nums = [1, 3, 5, 7, 9], target = 4
 * 返回 2（第一个 >= 4 的元素是 5）
 */
int lowerBound(const std::vector<int>& nums, int target) {
    (void)week01::checked_index(nums.size());
    std::size_t left = 0;
    std::size_t right = nums.size();
    
    while (left < right) {
        const std::size_t mid = left + (right - left) / 2;
        
        if (nums[mid] >= target) {
            right = mid;
        } else {
            left = mid + 1;
        }
    }
    
    return week01::checked_index(left);
}

/**
 * @brief 查找左边界（简化版，使用闭区间）
 * @param nums 已排序的数组
 * @param target 目标值
 * @return 目标值第一次出现的索引
 * 
 * 这是另一种实现方式，使用闭区间 [left, right]
 */
int binarySearchLeftClosed(const std::vector<int>& nums, int target) {
    if (nums.empty()) {
        return -1;
    }
    
    int left = 0;
    int right = week01::checked_index(nums.size()) - 1;
    
    // 记录找到的左边界
    int result = -1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        const auto index = static_cast<std::size_t>(mid);
        if (nums[index] == target) {
            // 找到目标，记录位置，继续向左找
            result = mid;
            right = mid - 1;
        } else if (nums[index] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    return result;
}

#endif // BINARY_SEARCH_LEFT_HPP
