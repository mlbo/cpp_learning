/**
 * @file binary_search_basic.cpp
 * @brief 标准二分查找实现
 * 
 * 标准二分查找模板：
 * - 搜索区间：左闭右开区间 [left, right)
 * - 循环条件：left < right
 * - 找到目标时：直接返回索引
 * 
 * 时间复杂度：O(log n)
 * 空间复杂度：O(1)
 */

#ifndef BINARY_SEARCH_BASIC_HPP
#define BINARY_SEARCH_BASIC_HPP

#include <vector>

#include "../../../common/integer_contracts.h"

/**
 * @brief 标准二分查找
 * @param nums 已排序的数组（升序）
 * @param target 目标值
 * @return 目标值的索引，如果不存在返回-1
 * 
 * 使用场景：
 * - 查找数组中是否存在某个元素
 * - 数组无重复元素时，返回唯一的位置
 * 
 * 示例：
 * nums = [1, 3, 5, 7, 9], target = 5
 * 返回 2
 */
int binarySearch(const std::vector<int>& nums, int target) {
    // 边界检查：空数组
    if (nums.empty()) {
        return -1;
    }
    
    (void)week01::checked_index(nums.size());
    // 搜索区间采用左闭右开 [left, right)，空数组和末尾边界无需 -1 哨兵。
    std::size_t left = 0;
    std::size_t right = nums.size();
    
    // 循环条件：left < right
    // 当 left == right 时，半开搜索区间为空，循环结束
    while (left < right) {
        // 计算中间位置
        // 使用 left + (right - left) / 2 而不是 (left + right) / 2
        // 原因：防止 left + right 溢出
        const std::size_t mid = left + (right - left) / 2;
        
        if (nums[mid] == target) {
            // 找到目标值，直接返回
            return week01::checked_index(mid);
        } else if (nums[mid] < target) {
            // 目标值在右半部分
            // mid 已经检查过，所以从 mid + 1 开始
            left = mid + 1;
        } else {
            // 目标值在左半部分；mid 已经检查过，新区间为 [left, mid)
            right = mid;
        }
    }
    
    // 未找到目标值
    return -1;
}

/**
 * @brief 标准二分查找（迭代版，带详细输出）
 * @param nums 已排序的数组（升序）
 * @param target 目标值
 * @return 目标值的索引，如果不存在返回-1
 * 
 * 此版本用于演示二分查找的过程
 */
int binarySearchWithTrace(const std::vector<int>& nums, int target) {
    if (nums.empty()) {
        return -1;
    }
    
    (void)week01::checked_index(nums.size());
    std::size_t left = 0;
    std::size_t right = nums.size();
    
    while (left < right) {
        const std::size_t mid = left + (right - left) / 2;
        
        // 输出每一步的状态
        // std::cout << "Step " << ++step 
        //           << ": left=" << left 
        //           << ", right=" << right 
        //           << ", mid=" << mid 
        //           << ", nums[mid]=" << nums[mid] << "\n";
        
        if (nums[mid] == target) {
            return week01::checked_index(mid);
        } else if (nums[mid] < target) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    
    return -1;
}

#endif // BINARY_SEARCH_BASIC_HPP
