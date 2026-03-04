/**
 * @file solution.cpp
 * @brief LeetCode 1. 两数之和
 * 
 * 题目描述：
 * 给定一个整数数组 nums 和一个整数目标值 target，
 * 找出数组中和为目标值的两个整数，返回它们的索引。
 * 
 * 难度：Easy
 * 标签：数组、哈希表
 */

#include <vector>
#include <unordered_map>
#include <iostream>
#include "solution.h"

namespace LC0001 {

// ============================================
// 解法1：暴力法 O(n²)
// ============================================

std::vector<int> Solution::twoSum_bruteForce(std::vector<int>& nums, int target) {
    int n = static_cast<int>(nums.size());
    for (int i = 0; i < n - 1; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (nums[i] + nums[j] == target) {
                return {i, j};
            }
        }
    }
    return {};  // 未找到
}

// ============================================
// 解法2：哈希表 O(n) - 推荐
// ============================================

std::vector<int> Solution::twoSum(std::vector<int>& nums, int target) {
    std::unordered_map<int, int> numToIndex;  // 值 -> 索引
    
    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        int complement = target - nums[i];
        
        // 查找补数是否已在哈希表中
        auto it = numToIndex.find(complement);
        if (it != numToIndex.end()) {
            return {it->second, i};  // 返回补数的索引和当前索引
        }
        
        // 将当前值存入哈希表
        numToIndex[nums[i]] = i;
    }
    
    return {};  // 未找到（题目保证有解）
}

// ============================================
// 解法3：一次遍历优化版
// ============================================

std::vector<int> Solution::twoSum_optimized(std::vector<int>& nums, int target) {
    // 预留空间减少rehash
    std::unordered_map<int, int> map;
    map.reserve(nums.size());
    
    for (size_t i = 0; i < nums.size(); ++i) {
        // C++11写法（注释）:
        // std::pair<std::unordered_map<int,int>::iterator, bool> ret =
        //     map.insert(std::make_pair(nums[i], static_cast<int>(i)));
        // auto it = ret.first; bool inserted = ret.second;
        auto [it, inserted] = map.insert({nums[i], static_cast<int>(i)});
        if (!inserted && nums[i] * 2 == target) {
            // 处理相同元素的情况 [3, 3], target = 6
            return {it->second, static_cast<int>(i)};
        }
    }
    
    // 第二次遍历查找
    for (size_t i = 0; i < nums.size(); ++i) {
        int complement = target - nums[i];
        auto it = map.find(complement);
        if (it != map.end() && it->second != static_cast<int>(i)) {
            return {static_cast<int>(i), it->second};
        }
    }
    
    return {};
}

// ============================================
// 复杂度分析
// ============================================

/*
 * 暴力法：
 * - 时间复杂度：O(n²) - 两层循环
 * - 空间复杂度：O(1) - 只用常数空间
 * 
 * 哈希表法：
 * - 时间复杂度：O(n) - 一次遍历，每次查找O(1)
 * - 空间复杂度：O(n) - 哈希表存储
 * 
 * 优化版：
 * - 时间复杂度：O(n) - 两次遍历
 * - 空间复杂度：O(n) - 哈希表存储
 */

} // namespace LC0001
