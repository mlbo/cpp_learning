/**
 * @file solution.cpp
 * @brief LeetCode 283. 移动零 - 解决方案实现
 */

#include "solution.h"
#include <algorithm>
#include <iostream>

namespace leetcode_0283 {

void Solution::moveZeroes(std::vector<int>& nums) {
    /**
     * 双指针交换法
     *
     * 核心思想：
     * - slow 指针：下一个非零元素应该放置的位置
     * - fast 指针：遍历数组寻找非零元素
     *
     * 当 fast 找到非零元素时，交换到 slow 位置
     * 这样保证了非零元素的相对顺序不变
     *
     * 时间复杂度：O(n)
     * 空间复杂度：O(1)
     */

    int slow = 0;  // 下一个非零元素的位置

    for (int fast = 0; fast < static_cast<int>(nums.size()); ++fast) {
        if (nums[fast] != 0) {
            std::swap(nums[slow], nums[fast]);
            ++slow;
        }
    }
}

void Solution::moveZeroes_two_pass(std::vector<int>& nums) {
    /**
     * 两次遍历法
     *
     * 第一次遍历：将非零元素移动到前面
     * 第二次遍历：将剩余位置填充零
     *
     * 时间复杂度：O(n)
     * 空间复杂度：O(1)
     */

    int pos = 0;  // 下一个非零元素的位置

    // 第一次遍历：移动非零元素
    for (int num : nums) {
        if (num != 0) {
            nums[pos] = num;
            ++pos;
        }
    }

    // 第二次遍历：填充零
    while (pos < static_cast<int>(nums.size())) {
        nums[pos] = 0;
        ++pos;
    }
}

void Solution::moveZeroes_optimized(std::vector<int>& nums) {
    /**
     * 优化版：减少不必要的交换
     *
     * 当 slow == fast 时，不需要交换（元素已经在正确位置）
     * 只有当 slow < fast 时才需要交换
     *
     * 这种优化减少了自交换（交换同一个元素）的情况
     *
     * 时间复杂度：O(n)
     * 空间复杂度：O(1)
     */

    int slow = 0;

    for (int fast = 0; fast < static_cast<int>(nums.size()); ++fast) {
        if (nums[fast] != 0) {
            if (slow != fast) {
                std::swap(nums[slow], nums[fast]);
            }
            ++slow;
        }
    }
}

} // namespace leetcode_0283
