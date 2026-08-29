/**
 * @file solution.cpp
 * @brief LeetCode 209: 长度最小的子数组 - 实现文件
 */

#include "solution.h"
#include <algorithm>
#include <cstdint>
#include <limits>
#include <stdexcept>

#include "../../../../common/integer_contracts.h"

namespace leetcode_0209 {

namespace {

void validate_input(int target, const std::vector<int>& nums) {
    if (target <= 0) {
        throw std::invalid_argument("target must be positive");
    }
    if (std::any_of(nums.begin(), nums.end(), [](int value) { return value <= 0; })) {
        throw std::invalid_argument("sliding-window input values must be positive");
    }
    (void)week01::checked_index(nums.size());
}

}  // namespace

/**
 * 方法一：滑动窗口
 * 
 * 核心思想：
 * 1. 使用双指针left和right维护一个窗口
 * 2. right指针向右扩展窗口，增加和
 * 3. 当sum >= target时，left指针向右收缩窗口，尝试找到更小的满足条件的子数组
 * 4. 窗口大小即为子数组长度
 * 
 * 滑动窗口正确性保证：
 * - 数组元素均为正数，所以sum随right增大而增大，随left增大而减小
 * - 这保证了单调性，使得滑动窗口算法正确
 */
int Solution::minSubArrayLen(int target, const std::vector<int>& nums) {
    validate_input(target, nums);
    std::size_t left = 0;
    std::int64_t sum = 0;
    std::size_t min_len = nums.size() + 1;
    
    // 遍历右边界
    for (std::size_t right = 0; right < nums.size(); ++right) {
        sum += nums[right];  // 扩展窗口
        
        // 当窗口满足条件时，尝试收缩
        while (sum >= static_cast<std::int64_t>(target)) {
            min_len = std::min(min_len, right - left + 1);  // 更新最小长度
            sum -= nums[left];  // 收缩左边界
            ++left;
        }
    }
    
    return min_len > nums.size() ? 0 : week01::checked_index(min_len);
}

/**
 * 方法二：前缀和 + 二分查找
 * 
 * 核心思想：
 * 1. 计算前缀和数组prefix，其中prefix[i]表示nums[0..i-1]的和
 * 2. 对于每个起始位置i，二分查找最小的结束位置j，使得prefix[j] - prefix[i] >= target
 * 3. prefix数组是单调递增的（因为元素都是正数），所以可以二分查找
 * 
 * 适用场景：
 * - 当需要多次查询不同target时效率更高
 * - 当前二分实现仍依赖正数使前缀和严格递增
 * - 如果允许负数，需要改用单调队列等不同机制，不能直接复用本函数
 */
int Solution::minSubArrayLenBinarySearch(int target, const std::vector<int>& nums) {
    validate_input(target, nums);
    std::size_t min_len = nums.size() + 1;
    
    // 计算前缀和
    std::vector<std::int64_t> prefix(nums.size() + 1, 0);
    for (std::size_t i = 0; i < nums.size(); ++i) {
        prefix[i + 1] = prefix[i] + nums[i];
    }
    
    // 对每个起始位置，二分查找结束位置
    for (std::size_t i = 0; i < nums.size(); ++i) {
        const auto need = prefix[i] + target;
        const auto first = prefix.begin() +
                           static_cast<std::vector<std::int64_t>::difference_type>(i + 1);
        const auto found = std::lower_bound(first, prefix.end(), need);
        if (found != prefix.end()) {
            const auto end_index = static_cast<std::size_t>(found - prefix.begin());
            min_len = std::min(min_len, end_index - i);
        }
    }
    
    return min_len > nums.size() ? 0 : week01::checked_index(min_len);
}

/**
 * 方法三：暴力法
 * 
 * 枚举所有可能的子数组，计算其和
 * 用于验证滑动窗口算法的正确性
 */
int Solution::minSubArrayLenBruteForce(int target, const std::vector<int>& nums) {
    validate_input(target, nums);
    std::size_t min_len = nums.size() + 1;
    
    for (std::size_t i = 0; i < nums.size(); ++i) {
        std::int64_t sum = 0;
        for (std::size_t j = i; j < nums.size(); ++j) {
            sum += nums[j];
            if (sum >= static_cast<std::int64_t>(target)) {
                min_len = std::min(min_len, j - i + 1);
                break;  // 找到最短的以i开头的子数组
            }
        }
    }
    
    return min_len > nums.size() ? 0 : week01::checked_index(min_len);
}

} // namespace leetcode_0209
