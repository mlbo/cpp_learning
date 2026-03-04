/**
 * @file solution.cpp
 * @brief LeetCode 88. 合并两个有序数组 - 解决方案实现
 */

#include "solution.h"
#include <algorithm>
#include <iostream>

namespace leetcode_0088 {

void Solution::merge(std::vector<int>& nums1, int m,
                     const std::vector<int>& nums2, int n) {
    /**
     * 逆向双指针法
     *
     * 关键洞察：
     * 1. nums1 有 m + n 的空间
     * 2. 从后往前填充，不会覆盖 nums1 中未处理的元素
     * 3. 最后只需要处理 nums2 剩余元素（nums1 剩余元素已在正确位置）
     *
     * 时间复杂度：O(m + n)
     * 空间复杂度：O(1)
     */

    int p1 = m - 1;          // nums1 有效元素的末尾索引
    int p2 = n - 1;          // nums2 的末尾索引
    int p = m + n - 1;       // 合并后数组的末尾索引

    // 从后往前比较并填充
    while (p1 >= 0 && p2 >= 0) {
        if (nums1[p1] > nums2[p2]) {
            nums1[p] = nums1[p1];
            --p1;
        } else {
            nums1[p] = nums2[p2];
            --p2;
        }
        --p;
    }

    // 处理 nums2 剩余元素
    // 注意：如果 p1 >= 0，nums1 的剩余元素已在正确位置，无需处理
    while (p2 >= 0) {
        nums1[p] = nums2[p2];
        --p;
        --p2;
    }
}

void Solution::merge_with_extra_space(std::vector<int>& nums1, int m,
                                       const std::vector<int>& nums2, int n) {
    /**
     * 正向双指针 + 额外空间法
     *
     * 思路：
     * 1. 复制 nums1 的前 m 个元素
     * 2. 使用双指针从前往后合并
     *
     * 时间复杂度：O(m + n)
     * 空间复杂度：O(m)
     */

    // 复制 nums1 的有效元素
    std::vector<int> nums1_copy(nums1.begin(), nums1.begin() + m);

    int p1 = 0;  // nums1_copy 的指针
    int p2 = 0;  // nums2 的指针
    int p = 0;   // nums1 的指针

    // 从前往后合并
    while (p1 < m && p2 < n) {
        if (nums1_copy[p1] <= nums2[p2]) {
            nums1[p] = nums1_copy[p1];
            ++p1;
        } else {
            nums1[p] = nums2[p2];
            ++p2;
        }
        ++p;
    }

    // 处理剩余元素
    while (p1 < m) {
        nums1[p] = nums1_copy[p1];
        ++p1;
        ++p;
    }

    while (p2 < n) {
        nums1[p] = nums2[p2];
        ++p2;
        ++p;
    }
}

} // namespace leetcode_0088
