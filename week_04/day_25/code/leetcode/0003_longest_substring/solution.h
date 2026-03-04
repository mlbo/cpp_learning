/**
 * @file solution.h
 * @brief LeetCode 3: 无重复字符的最长子串 - 头文件
 * 
 * 题目描述：
 * 给定一个字符串 s，请你找出其中不含有重复字符的最长子串的长度。
 * 
 * 解题思路：
 * 1. 滑动窗口法
 * 2. 使用哈希表记录字符最后出现的位置
 * 3. 维护窗口左右边界
 */

#ifndef LEETCODE_0003_LONGEST_SUBSTRING_H
#define LEETCODE_0003_LONGEST_SUBSTRING_H

#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

class Solution {
public:
    /**
     * @brief 滑动窗口法求最长无重复子串
     * 
     * 核心思想：
     * 维护一个滑动窗口 [left, right]，窗口内的字符都不重复。
     * 当遇到重复字符时，移动左边界到重复字符的下一个位置。
     * 
     * @param s 输入字符串
     * @return int 最长无重复子串的长度
     * 
     * 时间复杂度：O(n)，每个字符最多被访问两次
     * 空间复杂度：O(min(m, n))，m为字符集大小
     */
    int lengthOfLongestSubstring(std::string s);
    
    /**
     * @brief 使用数组优化的版本
     * 
     * 当字符集较小（如ASCII）时，使用数组比哈希表更快
     * 
     * @param s 输入字符串
     * @return int 最长无重复子串的长度
     * 
     * 时间复杂度：O(n)
     * 空间复杂度：O(1)，固定大小的数组
     */
    int lengthOfLongestSubstringArray(std::string s);
};

#endif // LEETCODE_0003_LONGEST_SUBSTRING_H
