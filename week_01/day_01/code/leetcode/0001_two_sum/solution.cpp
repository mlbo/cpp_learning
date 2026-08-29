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
#include "../../../../common/integer_contracts.h"

namespace LC0001 {

// ============================================
// 解法1：暴力法 O(n²)
// ============================================

std::vector<int> Solution::twoSum_bruteForce(std::vector<int>& nums, int target) {
    for (std::size_t i = 0; i < nums.size(); ++i) {
        for (std::size_t j = i + 1; j < nums.size(); ++j) {
            const auto sum = static_cast<std::int64_t>(nums[i]) + nums[j];
            if (sum == static_cast<std::int64_t>(target)) {
                return {week01::checked_index(i), week01::checked_index(j)};
            }
        }
    }
    return {};  // 未找到
}

// ============================================
// 解法2：哈希表 O(n) - 推荐
// ============================================

std::vector<int> Solution::twoSum(std::vector<int>& nums, int target) {
    std::unordered_map<int, std::size_t> numToIndex;  // 值 -> 索引
    numToIndex.reserve(nums.size());
    
    for (std::size_t i = 0; i < nums.size(); ++i) {
        const auto complement = static_cast<std::int64_t>(target) - nums[i];
        
        // 补数超出 int 时，数组中不可能存在该值；不要先做有符号溢出的 int 减法。
        if (complement >= std::numeric_limits<int>::min() &&
            complement <= std::numeric_limits<int>::max()) {
            const auto it = numToIndex.find(static_cast<int>(complement));
            if (it != numToIndex.end()) {
                return {week01::checked_index(it->second), week01::checked_index(i)};
            }
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
    std::unordered_map<int, std::size_t> map;
    map.reserve(nums.size());
    
    for (std::size_t i = 0; i < nums.size(); ++i) {
        // C++11写法（注释）:
        // std::pair<std::unordered_map<int,int>::iterator, bool> ret =
        //     map.insert(std::make_pair(nums[i], static_cast<int>(i)));
        // auto it = ret.first; bool inserted = ret.second;
        map.try_emplace(nums[i], i);  // 保留第一次出现的位置
    }
    
    // 第二次遍历查找
    for (std::size_t i = 0; i < nums.size(); ++i) {
        const auto complement = static_cast<std::int64_t>(target) - nums[i];
        if (complement < std::numeric_limits<int>::min() ||
            complement > std::numeric_limits<int>::max()) {
            continue;
        }
        const auto it = map.find(static_cast<int>(complement));
        if (it != map.end() && it->second != i) {
            return {week01::checked_index(i), week01::checked_index(it->second)};
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
 * - 时间复杂度：平均 O(n)；哈希冲突严重时可能退化
 * - 空间复杂度：O(n) - 哈希表存储
 * 
 * 优化版：
 * - 时间复杂度：平均 O(n)；哈希冲突严重时可能退化
 * - 空间复杂度：O(n) - 哈希表存储
 */

} // namespace LC0001
