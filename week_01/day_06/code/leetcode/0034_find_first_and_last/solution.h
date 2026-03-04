/**
 * @file solution.h
 * @brief LeetCode 34. 在排序数组中查找元素的第一个和最后一个位置
 * 
 * 题目描述：
 * 给你一个按照非递减顺序排列的整数数组 nums，和一个目标值 target。
 * 请你找出给定目标值在数组中的开始位置和结束位置。
 * 如果数组中不存在目标值 target，返回 [-1, -1]。
 * 
 * 你必须设计并实现时间复杂度为 O(log n) 的算法解决此问题。
 * 
 * 示例 1：
 * 输入：nums = [5,7,7,8,8,10], target = 8
 * 输出：[3,4]
 * 
 * 示例 2：
 * 输入：nums = [5,7,7,8,8,10], target = 6
 * 输出：[-1,-1]
 * 
 * 示例 3：
 * 输入：nums = [], target = 0
 * 输出：[-1,-1]
 */

#ifndef SOLUTION_34_H
#define SOLUTION_34_H

#include <vector>

class Solution34 {
public:
    /**
     * @brief 查找目标值的起始和结束位置
     * @param nums 非递减顺序排列的整数数组
     * @param target 目标值
     * @return {起始位置, 结束位置}，不存在则返回 {-1, -1}
     */
    std::vector<int> searchRange(std::vector<int>& nums, int target);
    
    /**
     * @brief 查找左边界
     * @param nums 已排序数组
     * @param target 目标值
     * @return 第一个等于 target 的索引，不存在返回 -1
     */
    int findLeftBound(std::vector<int>& nums, int target);
    
    /**
     * @brief 查找右边界
     * @param nums 已排序数组
     * @param target 目标值
     * @return 最后一个等于 target 的索引，不存在返回 -1
     */
    int findRightBound(std::vector<int>& nums, int target);

private:
    /**
     * @brief 二分查找左边界（内部实现）
     */
    int binarySearchLeft(std::vector<int>& nums, int target);
    
    /**
     * @brief 二分查找右边界（内部实现）
     */
    int binarySearchRight(std::vector<int>& nums, int target);
};

#endif // SOLUTION_34_H
