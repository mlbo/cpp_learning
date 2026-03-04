/**
 * @file solution.h
 * @brief LeetCode 283. 移动零 - 解决方案头文件
 *
 * 题目描述：
 * 给定一个数组 nums，编写一个函数将所有 0 移动到数组的末尾，
 * 同时保持非零元素的相对顺序。
 *
 * 要求：
 * 1. 必须在原数组上操作，不能拷贝额外的数组
 * 2. 尽量减少操作次数
 *
 * 示例：
 * 输入: [0,1,0,3,12]
 * 输出: [1,3,12,0,0]
 */

#ifndef SOLUTION_H
#define SOLUTION_H

#include <vector>

namespace leetcode_0283 {

class Solution {
public:
    /**
     * @brief 移动零（双指针 - 交换法）
     *
     * 使用快慢指针，一次遍历完成
     * 快指针遍历数组，慢指针记录下一个非零元素的位置
     *
     * @param nums 输入数组
     */
    void moveZeroes(std::vector<int>& nums);

    /**
     * @brief 移动零（两次遍历法）
     *
     * 第一次遍历移动非零元素，第二次遍历填充零
     *
     * @param nums 输入数组
     */
    void moveZeroes_two_pass(std::vector<int>& nums);

    /**
     * @brief 移动零（优化版 - 减少交换次数）
     *
     * 只在必要时才进行交换
     *
     * @param nums 输入数组
     */
    void moveZeroes_optimized(std::vector<int>& nums);
};

} // namespace leetcode_0283

#endif // SOLUTION_H
