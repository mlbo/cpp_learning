/**
 * @file solution.h
 * @brief LeetCode 209: 长度最小的子数组
 * 
 * 题目描述：
 * 给定一个含有 n 个正整数的数组和一个正整数 target。
 * 找出该数组中满足其和 ≥ target 的长度最小的连续子数组，
 * 并返回其长度。如果不存在符合条件的子数组，返回 0。
 * 
 * 示例：
 * 输入: target = 7, nums = [2,3,1,2,4,3]
 * 输出: 2
 * 解释: 子数组 [4,3] 是该条件下的长度最小的子数组
 * 
 * 难度：中等
 * 标签：数组、二分查找、前缀和、滑动窗口
 */

#ifndef SOLUTION_0209_H
#define SOLUTION_0209_H

#include <vector>
#include <climits>

using namespace std;

class Solution {
public:
    /**
     * @brief 方法一：滑动窗口
     * @param target 目标和
     * @param nums 输入数组
     * @return 最小子数组长度
     * 
     * 时间复杂度: O(n)
     * 空间复杂度: O(1)
     */
    int minSubArrayLen(int target, vector<int>& nums);
    
    /**
     * @brief 方法二：前缀和 + 二分查找
     * @param target 目标和
     * @param nums 输入数组
     * @return 最小子数组长度
     * 
     * 时间复杂度: O(n log n)
     * 空间复杂度: O(n)
     */
    int minSubArrayLenBinarySearch(int target, vector<int>& nums);
    
    /**
     * @brief 方法三：暴力法（用于对比）
     * @param target 目标和
     * @param nums 输入数组
     * @return 最小子数组长度
     * 
     * 时间复杂度: O(n²)
     * 空间复杂度: O(1)
     */
    int minSubArrayLenBruteForce(int target, vector<int>& nums);
};

#endif // SOLUTION_0209_H
