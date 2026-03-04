/**
 * @file solution.h
 * @brief LeetCode 42: 接雨水 - 解法声明
 * 
 * 给定 n 个非负整数表示每个宽度为 1 的柱子的高度图，
 * 计算按此排列的柱子，下雨之后能接多少雨水。
 */

#ifndef TRAPPING_RAIN_WATER_H
#define TRAPPING_RAIN_WATER_H

#include <vector>

namespace leetcode {

class Solution {
public:
    /**
     * @brief 解法1: 双指针法
     * @param height 高度数组
     * @return 可接雨水量
     * 
     * 时间复杂度: O(n)
     * 空间复杂度: O(1)
     */
    static int trap_two_pointers(std::vector<int>& height);

    /**
     * @brief 解法2: 动态规划
     * @param height 高度数组
     * @return 可接雨水量
     * 
     * 时间复杂度: O(n)
     * 空间复杂度: O(n)
     */
    static int trap_dp(std::vector<int>& height);

    /**
     * @brief 解法3: 单调栈
     * @param height 高度数组
     * @return 可接雨水量
     * 
     * 时间复杂度: O(n)
     * 空间复杂度: O(n)
     */
    static int trap_stack(std::vector<int>& height);

    /**
     * @brief 解法4: 按行计算
     * @param height 高度数组
     * @return 可接雨水量
     * 
     * 时间复杂度: O(n * max_height)
     * 空间复杂度: O(1)
     */
    static int trap_by_row(std::vector<int>& height);

    // 默认使用最优解法
    int trap(std::vector<int>& height) {
        return trap_two_pointers(height);
    }
};

} // namespace leetcode

#endif // TRAPPING_RAIN_WATER_H
