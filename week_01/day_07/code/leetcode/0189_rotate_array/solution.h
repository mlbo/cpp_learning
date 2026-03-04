/**
 * @file solution.h
 * @brief LeetCode 189: 轮转数组 - 解法声明
 * 
 * 给定一个整数数组 nums，将数组中的元素向右轮转 k 个位置。
 */

#ifndef ROTATE_ARRAY_H
#define ROTATE_ARRAY_H

#include <vector>

namespace leetcode {

class Solution {
public:
    /**
     * @brief 解法1: 数组翻转法
     * @param nums 待轮转的数组
     * @param k 轮转步数
     * 
     * 时间复杂度: O(n)
     * 空间复杂度: O(1)
     */
    static void rotate_reverse(std::vector<int>& nums, int k);

    /**
     * @brief 解法2: 额外数组法
     * @param nums 待轮转的数组
     * @param k 轮转步数
     * 
     * 时间复杂度: O(n)
     * 空间复杂度: O(n)
     */
    static void rotate_extra_array(std::vector<int>& nums, int k);

    /**
     * @brief 解法3: 环形替换法
     * @param nums 待轮转的数组
     * @param k 轮转步数
     * 
     * 时间复杂度: O(n)
     * 空间复杂度: O(1)
     */
    static void rotate_cycle(std::vector<int>& nums, int k);

    /**
     * @brief 解法4: std::rotate 使用
     * @param nums 待轮转的数组
     * @param k 轮转步数
     * 
     * 时间复杂度: O(n)
     * 空间复杂度: O(1)
     */
    static void rotate_std(std::vector<int>& nums, int k);

    // 默认使用翻转法
    void rotate(std::vector<int>& nums, int k) {
        rotate_reverse(nums, k);
    }
};

} // namespace leetcode

#endif // ROTATE_ARRAY_H
