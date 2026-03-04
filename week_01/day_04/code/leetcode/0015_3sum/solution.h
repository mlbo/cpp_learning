/**
 * @file solution.h
 * @brief LeetCode 15: 三数之和 (3Sum)
 * 
 * 给定一个整数数组 nums，判断是否存在三元组 [nums[i], nums[j], nums[k]]
 * 满足 i != j != k 且 nums[i] + nums[j] + nums[k] == 0。
 * 
 * 返回所有不重复的三元组。
 * 
 * 示例:
 * 输入: nums = [-1,0,1,2,-1,-4]
 * 输出: [[-1,-1,2],[-1,0,1]]
 * 解释:
 * - nums[0] + nums[1] + nums[2] = (-1) + 0 + 1 = 0
 * - nums[1] + nums[2] + nums[4] = 0 + 1 + (-1) = 0
 * - 注意 [-1, 0, 1] 和 [0, 1, -1] 是重复的，只保留一个
 */

#ifndef SOLUTION_H
#define SOLUTION_H

#include <vector>
#include <algorithm>

class Solution {
public:
    /**
     * @brief 找出所有和为0的三元组
     * 
     * 方法：排序 + 双指针 + 去重
     * 
     * 核心思想:
     * 1. 首先排序，使去重和双指针成为可能
     * 2. 固定第一个数，用双指针找另外两个数
     * 3. 在三个层面进行去重
     * 
     * @param nums 输入数组
     * @return 所有唯一的三元组，每个三元组内元素和为0
     * 
     * 时间复杂度: O(n²)
     * 空间复杂度: O(1)（不考虑输出空间）
     */
    std::vector<std::vector<int>> threeSum(std::vector<int>& nums);
};

#endif // SOLUTION_H
