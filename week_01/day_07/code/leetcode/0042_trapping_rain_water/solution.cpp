/**
 * @file solution.cpp
 * @brief LeetCode 42: 接雨水 - 解法实现
 */

#include "solution.h"
#include <algorithm>
#include <cstdint>
#include <limits>
#include <stack>
#include <stdexcept>

#include "../../../../common/integer_contracts.h"

namespace leetcode::p0042 {

namespace {

void validate_heights(const std::vector<int>& height) {
    (void)week01::checked_index(height.size());
    if (std::any_of(height.begin(), height.end(), [](int value) { return value < 0; })) {
        throw std::invalid_argument("rain-water heights must be nonnegative");
    }
}

void add_water(std::int64_t& water, std::int64_t amount) {
    const auto int_limit = static_cast<std::int64_t>(std::numeric_limits<int>::max());
    if (amount < 0 || amount > int_limit - water) {
        throw std::overflow_error("trapped water does not fit in int");
    }
    water += amount;
}

}  // namespace

// ============================================================================
// 解法1: 双指针法（线性时间、常数额外空间）
// ============================================================================

int Solution::trap_two_pointers(std::vector<int>& height) {
    validate_heights(height);
    if (height.empty()) return 0;
    
    std::size_t left = 0;
    std::size_t right = height.size() - 1;
    int left_max = 0;
    int right_max = 0;
    std::int64_t water = 0;
    
    while (left < right) {
        if (height[left] < height[right]) {
            // 左边较低，处理左边
            if (height[left] >= left_max) {
                left_max = height[left];  // 更新左边最大值
            } else {
                add_water(water, left_max - height[left]);  // 计算当前位置的储水量
            }
            ++left;
        } else {
            // 右边较低，处理右边
            if (height[right] >= right_max) {
                right_max = height[right];  // 更新右边最大值
            } else {
                add_water(water, right_max - height[right]);  // 计算当前位置的储水量
            }
            --right;
        }
    }
    
    return week01::checked_result(water);
}

// ============================================================================
// 解法2: 动态规划
// ============================================================================

int Solution::trap_dp(std::vector<int>& height) {
    validate_heights(height);
    const std::size_t n = height.size();
    if (n == 0) return 0;
    
    // left_max[i] 表示位置i左边（含i）的最大高度
    std::vector<int> left_max(n);
    // right_max[i] 表示位置i右边（含i）的最大高度
    std::vector<int> right_max(n);
    
    // 从左到右计算left_max
    left_max[0] = height[0];
    for (std::size_t i = 1; i < n; ++i) {
        left_max[i] = std::max(left_max[i - 1], height[i]);
    }
    
    // 从右到左计算right_max
    right_max[n - 1] = height[n - 1];
    for (std::size_t i = n - 1; i > 0; --i) {
        right_max[i - 1] = std::max(right_max[i], height[i - 1]);
    }
    
    // 计算每个位置的储水量
    std::int64_t water = 0;
    for (std::size_t i = 0; i < n; ++i) {
        // 当前位置的储水量 = min(左边最大高度, 右边最大高度) - 当前高度
        add_water(water, std::min(left_max[i], right_max[i]) - height[i]);
    }
    
    return week01::checked_result(water);
}

// ============================================================================
// 解法3: 单调栈
// ============================================================================

int Solution::trap_stack(std::vector<int>& height) {
    validate_heights(height);
    std::int64_t water = 0;
    std::stack<std::size_t> st;  // 存储下标，单调递减栈
    
    for (std::size_t i = 0; i < height.size(); ++i) {
        // 当前柱子高于栈顶，可能形成凹槽
        while (!st.empty() && height[i] > height[st.top()]) {
            const std::size_t bottom = st.top();  // 凹槽底部
            st.pop();
            
            if (st.empty()) break;  // 没有左边界，无法储水
            
            const std::size_t left = st.top();  // 左边界
            const auto width = static_cast<std::int64_t>(i - left - 1);
            const int bounded_height = std::min(height[i], height[left]) - height[bottom];
            add_water(water, width * bounded_height);
        }
        st.push(i);
    }
    
    return week01::checked_result(water);
}

// ============================================================================
// 解法4: 按行计算
// ============================================================================

int Solution::trap_by_row(std::vector<int>& height) {
    validate_heights(height);
    if (height.empty()) return 0;
    
    int max_height = *std::max_element(height.begin(), height.end());
    constexpr int max_demonstration_levels = 1'000'000;
    if (max_height > max_demonstration_levels) {
        throw std::length_error(
            "row-by-row demonstration is limited to 1,000,000 height levels; "
            "use the O(n) two-pointer method for taller inputs");
    }
    std::int64_t water = 0;
    
    // 逐层计算
    for (int h = 1; h <= max_height; ++h) {
        bool counting = false;  // 是否在计数
        std::int64_t temp = 0;
        
        for (int value : height) {
            if (value >= h) {
                // 遇到柱子，结束当前层级的计数
                add_water(water, temp);
                temp = 0;
                counting = true;
            } else if (counting) {
                ++temp;  // 空位，可能储水
            }
        }
    }
    
    return week01::checked_result(water);
}

} // namespace leetcode::p0042
