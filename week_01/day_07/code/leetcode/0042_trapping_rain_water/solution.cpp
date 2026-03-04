/**
 * @file solution.cpp
 * @brief LeetCode 42: 接雨水 - 解法实现
 */

#include "solution.h"
#include <algorithm>
#include <stack>

namespace leetcode {

// ============================================================================
// 解法1: 双指针法 (最优解)
// ============================================================================

int Solution::trap_two_pointers(std::vector<int>& height) {
    if (height.empty()) return 0;
    
    int left = 0;
    int right = static_cast<int>(height.size()) - 1;
    int left_max = 0;
    int right_max = 0;
    int water = 0;
    
    while (left < right) {
        if (height[left] < height[right]) {
            // 左边较低，处理左边
            if (height[left] >= left_max) {
                left_max = height[left];  // 更新左边最大值
            } else {
                water += left_max - height[left];  // 计算当前位置的储水量
            }
            ++left;
        } else {
            // 右边较低，处理右边
            if (height[right] >= right_max) {
                right_max = height[right];  // 更新右边最大值
            } else {
                water += right_max - height[right];  // 计算当前位置的储水量
            }
            --right;
        }
    }
    
    return water;
}

// ============================================================================
// 解法2: 动态规划
// ============================================================================

int Solution::trap_dp(std::vector<int>& height) {
    int n = static_cast<int>(height.size());
    if (n == 0) return 0;
    
    // left_max[i] 表示位置i左边（含i）的最大高度
    std::vector<int> left_max(n);
    // right_max[i] 表示位置i右边（含i）的最大高度
    std::vector<int> right_max(n);
    
    // 从左到右计算left_max
    left_max[0] = height[0];
    for (int i = 1; i < n; ++i) {
        left_max[i] = std::max(left_max[i - 1], height[i]);
    }
    
    // 从右到左计算right_max
    right_max[n - 1] = height[n - 1];
    for (int i = n - 2; i >= 0; --i) {
        right_max[i] = std::max(right_max[i + 1], height[i]);
    }
    
    // 计算每个位置的储水量
    int water = 0;
    for (int i = 0; i < n; ++i) {
        // 当前位置的储水量 = min(左边最大高度, 右边最大高度) - 当前高度
        water += std::min(left_max[i], right_max[i]) - height[i];
    }
    
    return water;
}

// ============================================================================
// 解法3: 单调栈
// ============================================================================

int Solution::trap_stack(std::vector<int>& height) {
    int water = 0;
    std::stack<int> st;  // 存储下标，单调递减栈
    
    for (int i = 0; i < static_cast<int>(height.size()); ++i) {
        // 当前柱子高于栈顶，可能形成凹槽
        while (!st.empty() && height[i] > height[st.top()]) {
            int bottom = st.top();  // 凹槽底部
            st.pop();
            
            if (st.empty()) break;  // 没有左边界，无法储水
            
            int left = st.top();  // 左边界
            int width = i - left - 1;  // 宽度
            int bounded_height = std::min(height[i], height[left]) - height[bottom];
            water += width * bounded_height;
        }
        st.push(i);
    }
    
    return water;
}

// ============================================================================
// 解法4: 按行计算
// ============================================================================

int Solution::trap_by_row(std::vector<int>& height) {
    if (height.empty()) return 0;
    
    int max_height = *std::max_element(height.begin(), height.end());
    int water = 0;
    
    // 逐层计算
    for (int h = 1; h <= max_height; ++h) {
        bool counting = false;  // 是否在计数
        int temp = 0;
        
        for (int i = 0; i < static_cast<int>(height.size()); ++i) {
            if (height[i] >= h) {
                // 遇到柱子，结束当前层级的计数
                water += temp;
                temp = 0;
                counting = true;
            } else if (counting) {
                ++temp;  // 空位，可能储水
            }
        }
    }
    
    return water;
}

} // namespace leetcode
