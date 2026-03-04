/**
 * @file solution.h
 * @brief LC 128: 最长连续序列
 * 
 * 题目描述：
 * 给定一个未排序的整数数组 nums，找出数字连续的最长序列的长度。
 * 要求算法的时间复杂度为 O(n)。
 * 
 * 示例：
 * 输入: nums = [100, 4, 200, 1, 3, 2]
 * 输出: 4
 * 解释: 最长连续序列是 [1, 2, 3, 4]，长度为 4
 * 
 * 解题思路：
 * 1. 哈希集合法：将所有数放入集合，只从"起点"开始计数
 * 2. 动态规划/并查集：更复杂但也能达到 O(n)
 */

#pragma once

#include <vector>
#include <unordered_set>

class Solution {
public:
    /**
     * @brief 方法一：哈希集合法
     * 
     * 核心思想：
     * 1. 将所有数字放入哈希集合（去重 + O(1) 查找）
     * 2. 对于每个数字，判断是否是序列的起点（num-1 不存在）
     * 3. 如果是起点，向后查找连续序列的长度
     * 
     * 为什么是 O(n)?
     * - 每个数字最多被访问两次：
     *   - 一次判断是否是起点
     *   - 一次在序列中计数
     * 
     * 时间复杂度：O(n)
     * 空间复杂度：O(n)
     */
    int longestConsecutive(std::vector<int>& nums);
    
    /**
     * @brief 方法二：哈希集合 + 预处理优化
     * 
     * 核心思想：
     * 与方法一类似，但可以进一步优化查找过程
     */
    int longestConsecutiveOptimized(std::vector<int>& nums);
};

// 测试函数
void testLongestConsecutive();
