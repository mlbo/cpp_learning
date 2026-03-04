/**
 * @file solution.h
 * @brief LeetCode 88. 合并两个有序数组 - 解决方案头文件
 *
 * 题目描述：
 * 给你两个按非递减顺序排列的整数数组 nums1 和 nums2，
 * 另有两个整数 m 和 n ，分别表示 nums1 和 nums2 中的元素数目。
 * 请你合并 nums2 到 nums1 中，使 nums1 成为一个有序数组。
 *
 * 初始时，nums1 的长度为 m + n，其中前 m 个元素表示应合并的元素，
 * 后 n 个元素为 0 ，应忽略。nums2 的长度为 n 。
 *
 * 要求：时间复杂度 O(m+n)，空间复杂度 O(1)
 */

#ifndef SOLUTION_H
#define SOLUTION_H

#include <vector>

namespace leetcode_0088 {

class Solution {
public:
    /**
     * @brief 合并两个有序数组（逆向双指针）
     *
     * 核心思想：利用 nums1 后面的空位，从后往前填充
     *
     * @param nums1 第一个数组（包含足够的缓冲空间）
     * @param m nums1 的有效元素个数
     * @param nums2 第二个数组
     * @param n nums2 的元素个数
     */
    void merge(std::vector<int>& nums1, int m,
               const std::vector<int>& nums2, int n);

    /**
     * @brief 合并两个有序数组（正向双指针 + 额外空间）
     *
     * 使用额外的 O(m) 空间存储 nums1 的原始数据
     */
    void merge_with_extra_space(std::vector<int>& nums1, int m,
                                 const std::vector<int>& nums2, int n);
};

} // namespace leetcode_0088

#endif // SOLUTION_H
