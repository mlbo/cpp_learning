/**
 * @file solution.h
 * @brief LeetCode 438: 找到字符串中所有字母异位词 - 头文件
 * 
 * 题目描述：
 * 给定两个字符串 s 和 p，找到 s 中所有 p 的字母异位词的起始索引。
 * 字母异位词指由相同字母重新排列形成的字符串。
 * 
 * 解题思路：
 * 1. 固定大小滑动窗口
 * 2. 使用计数数组比较字符频率
 * 3. 优化：使用diff计数避免完整比较
 */

#ifndef LEETCODE_0438_FIND_ANAGRAMS_H
#define LEETCODE_0438_FIND_ANAGRAMS_H

#include <string>
#include <vector>

class Solution {
public:
    /**
     * @brief 使用计数数组的方法
     * 
     * 核心思想：
     * 1. 统计p中各字符的出现次数
     * 2. 维护一个与p长度相同的滑动窗口
     * 3. 比较窗口内字符计数与p的计数是否相同
     * 
     * @param s 源字符串
     * @param p 目标模式字符串
     * @return std::vector<int> 异位词起始索引列表
     * 
     * 时间复杂度：O(n)
     * 空间复杂度：O(1)，固定大小的计数数组
     */
    std::vector<int> findAnagrams(std::string s, std::string p);
    
    /**
     * @brief 优化版本：使用差值计数
     * 
     * 核心思想：
     * 维护一个diff计数，表示窗口与p的差异字符数
     * 当diff为0时，说明窗口是p的异位词
     * 
     * @param s 源字符串
     * @param p 目标模式字符串
     * @return std::vector<int> 异位词起始索引列表
     * 
     * 时间复杂度：O(n)
     * 空间复杂度：O(1)
     */
    std::vector<int> findAnagramsOptimized(std::string s, std::string p);

private:
    /**
     * @brief 检查计数数组是否全为0
     * @param count 计数数组
     * @return true 如果全为0
     */
    bool allZero(const std::vector<int>& count);
};

#endif // LEETCODE_0438_FIND_ANAGRAMS_H
