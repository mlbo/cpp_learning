/**
 * @file solution.h
 * @brief LC 128: 最长连续序列
 * 
 * 题目描述：
 * 给定一个未排序的整数数组 nums，找出数字连续的最长序列的长度。
 * 题目要求线性算法；哈希实现按平均 O(1) 查找得到期望 O(n)。
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

namespace day24::lc128 {

class Solution {
public:
    /**
     * @brief 方法一：哈希集合法
     * 
     * 核心思想：
     * 1. 将所有数字放入哈希集合（去重 + 平均 O(1) 查找）
     * 2. 对于每个数字，先检查 INT_MIN，再判断 num-1 是否存在
     * 3. 如果是起点，向后查找连续序列的长度
     * 
     * 为什么是 O(n)?
     * - 每个数字最多被访问两次：
     *   - 一次判断是否是起点
     *   - 一次在序列中计数
     * 
     * 期望时间复杂度：O(n)，最坏情况受哈希冲突影响
     * 空间复杂度：O(n)
     */
    int longestConsecutive(std::vector<int>& nums);
    
    /**
     * @brief 方法二：消耗式哈希集合
     *
     * 从任意未处理数字同时向两侧扩展，并把访问过的数字从局部集合删除。
     * 每个不同数字只会被某一次扩展消费；输入 vector 本身不会被修改。
     * 这是一种不同的不变量组织方式，不承诺比“只从起点扩展”更快。
     */
    int longestConsecutiveOptimized(std::vector<int>& nums);
};

} // namespace day24::lc128

// 测试函数
void testLongestConsecutive();
