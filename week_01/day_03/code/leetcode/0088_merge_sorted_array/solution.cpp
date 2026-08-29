/**
 * @file solution.cpp
 * @brief LeetCode 88. 合并两个有序数组 - 解决方案实现
 */

#include "solution.h"
#include <algorithm>
#include <iostream>
#include <stdexcept>

namespace {

std::pair<std::size_t, std::size_t> validate_lengths(
        const std::vector<int>& nums1, int m,
        const std::vector<int>& nums2, int n) {
    if (m < 0 || n < 0) {
        throw std::invalid_argument("merge lengths must be nonnegative");
    }
    const auto first_size = static_cast<std::size_t>(m);
    const auto second_size = static_cast<std::size_t>(n);
    if (first_size > nums1.size() || second_size > nums2.size() ||
        second_size > nums1.size() - first_size) {
        throw std::length_error("merge lengths exceed input buffers");
    }
    return {first_size, second_size};
}

}  // namespace

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

    const auto [first_size, second_size] = validate_lengths(nums1, m, nums2, n);
    std::size_t p1 = first_size;                   // nums1 未处理区间 [0, p1)
    std::size_t p2 = second_size;                  // nums2 未处理区间 [0, p2)
    std::size_t output = first_size + second_size; // 待填充区间 [0, output)

    // 从后往前比较并填充
    while (p1 > 0 && p2 > 0) {
        if (nums1[p1 - 1] > nums2[p2 - 1]) {
            --p1;
            --output;
            nums1[output] = nums1[p1];
        } else {
            --p2;
            --output;
            nums1[output] = nums2[p2];
        }
    }

    // 处理 nums2 剩余元素
    // nums1 的剩余元素已在正确位置，无需处理。
    while (p2 > 0) {
        --p2;
        --output;
        nums1[output] = nums2[p2];
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

    const auto [first_size, second_size] = validate_lengths(nums1, m, nums2, n);

    // 复制 nums1 的有效元素；显式循环不依赖 size_t 到 difference_type 的窄化。
    std::vector<int> nums1_copy;
    nums1_copy.reserve(first_size);
    for (std::size_t i = 0; i < first_size; ++i) {
        nums1_copy.push_back(nums1[i]);
    }

    std::size_t p1 = 0;      // nums1_copy 的指针
    std::size_t p2 = 0;      // nums2 的指针
    std::size_t output = 0;  // nums1 的指针

    // 从前往后合并
    while (p1 < first_size && p2 < second_size) {
        if (nums1_copy[p1] <= nums2[p2]) {
            nums1[output] = nums1_copy[p1];
            ++p1;
        } else {
            nums1[output] = nums2[p2];
            ++p2;
        }
        ++output;
    }

    // 处理剩余元素
    while (p1 < first_size) {
        nums1[output] = nums1_copy[p1];
        ++p1;
        ++output;
    }

    while (p2 < second_size) {
        nums1[output] = nums2[p2];
        ++p2;
        ++output;
    }
}

} // namespace leetcode_0088
