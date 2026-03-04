/**
 * @file solution.h
 * @brief LeetCode 1. 两数之和 - 头文件
 */

#ifndef LEETCODE_0001_TWO_SUM_H
#define LEETCODE_0001_TWO_SUM_H

#include <vector>

namespace LC0001 {

class Solution {
public:
    // 主解法：哈希表
    std::vector<int> twoSum(std::vector<int>& nums, int target);
    
    // 暴力解法：用于对比
    std::vector<int> twoSum_bruteForce(std::vector<int>& nums, int target);
    
    // 优化解法
    std::vector<int> twoSum_optimized(std::vector<int>& nums, int target);
};

#endif // LEETCODE_0001_TWO_SUM_H

} // namespace LC0001
