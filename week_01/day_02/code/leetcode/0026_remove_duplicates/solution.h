/**
 * @file solution.h
 * @brief LeetCode 26. 删除有序数组中的重复项
 * 
 * 题目描述：
 * 给你一个有序数组 nums ，请你原地删除重复出现的元素，
 * 使每个元素只出现一次，返回删除后数组的新长度。
 * 
 * 要求：
 * - 不要使用额外的数组空间
 * - 必须在原地修改输入数组
 * - O(1) 额外空间
 */

#ifndef LEETCODE_0026_SOLUTION_H
#define LEETCODE_0026_SOLUTION_H

#include <vector>

class Solution {
public:
    /**
     * @brief 使用快慢指针删除重复元素（迭代法）
     * @param nums 有序数组
     * @return 删除重复元素后的新长度
     * 
     * 时间复杂度: O(n)
     * 空间复杂度: O(1)
     */
    int removeDuplicates(std::vector<int>& nums);
    
    /**
     * @brief 使用递归删除重复元素
     * @param nums 有序数组
     * @return 删除重复元素后的新长度
     * 
     * 时间复杂度: O(n)
     * 空间复杂度: O(n) - 递归栈
     * 
     * 注意：本题不推荐使用递归，仅作为算法演示
     */
    int removeDuplicatesRecursive(std::vector<int>& nums);
    
private:
    /**
     * @brief 递归辅助函数
     */
    int removeDuplicatesHelper(std::vector<int>& nums, int slow, int fast);
};

#endif // LEETCODE_0026_SOLUTION_H
