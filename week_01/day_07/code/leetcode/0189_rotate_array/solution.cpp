/**
 * @file solution.cpp
 * @brief LeetCode 189: 轮转数组 - 解法实现
 */

#include "solution.h"
#include <algorithm>
#include <cstdint>
#include <numeric>
#include <utility>

#include "../../../../common/integer_contracts.h"

namespace leetcode::p0189 {

namespace {

std::size_t normalized_shift(const std::vector<int>& nums, int k) {
    if (nums.empty()) {
        return 0;
    }
    const int size = week01::checked_index(nums.size());
    auto shift = static_cast<std::int64_t>(k) % static_cast<std::int64_t>(size);
    if (shift < 0) {
        shift += size;  // 负 k 表示向左轮转，统一换算为向右轮转。
    }
    return static_cast<std::size_t>(shift);
}

}  // namespace

// ============================================================================
// 解法1: 数组翻转法 (推荐)
// ============================================================================

void Solution::rotate_reverse(std::vector<int>& nums, int k) {
    const std::size_t shift = normalized_shift(nums, k);
    if (shift == 0) return;
    
    // 步骤1: 翻转整个数组
    // [1,2,3,4,5,6,7] -> [7,6,5,4,3,2,1]
    std::reverse(nums.begin(), nums.end());
    
    // 步骤2: 翻转前 k 个元素
    // [7,6,5,4,3,2,1] -> [5,6,7,4,3,2,1]
    const auto middle = nums.begin() +
                        static_cast<std::vector<int>::difference_type>(shift);
    std::reverse(nums.begin(), middle);
    
    // 步骤3: 翻转剩余元素
    // [5,6,7,4,3,2,1] -> [5,6,7,1,2,3,4]
    std::reverse(middle, nums.end());
}

// ============================================================================
// 解法2: 额外数组法
// ============================================================================

void Solution::rotate_extra_array(std::vector<int>& nums, int k) {
    const std::size_t shift = normalized_shift(nums, k);
    if (shift == 0) return;
    
    // 创建临时数组
    std::vector<int> temp(nums.size());
    
    // 计算新位置: 原位置 i -> 新位置 (i + k) % n
    for (std::size_t i = 0; i < nums.size(); ++i) {
        temp[(i + shift) % nums.size()] = nums[i];
    }
    
    // 移动回去
    nums = std::move(temp);
}

// ============================================================================
// 解法3: 环形替换法
// ============================================================================

void Solution::rotate_cycle(std::vector<int>& nums, int k) {
    const std::size_t shift = normalized_shift(nums, k);
    if (shift == 0) return;
    
    std::size_t count = 0;  // 已处理的元素数量
    
    for (std::size_t start = 0; count < nums.size(); ++start) {
        std::size_t current = start;
        int previous = nums[start];
        
        do {
            const std::size_t next = (current + shift) % nums.size();
            const int displaced = nums[next];
            nums[next] = previous;
            previous = displaced;
            current = next;
            ++count;
        } while (current != start);
    }
}

// ============================================================================
// 解法4: 使用 std::rotate
// ============================================================================

void Solution::rotate_std(std::vector<int>& nums, int k) {
    const std::size_t shift = normalized_shift(nums, k);
    if (shift == 0) return;
    
    // std::rotate 将 [first, middle) 和 [middle, last) 交换
    // 向右轮转 k 位，等价于从 n-k 位置开始
    const auto offset = static_cast<std::vector<int>::difference_type>(shift);
    std::rotate(nums.begin(), nums.end() - offset, nums.end());
}

} // namespace leetcode::p0189
