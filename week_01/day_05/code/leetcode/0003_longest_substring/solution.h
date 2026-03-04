/**
 * @file solution.h
 * @brief LeetCode 3: 无重复字符的最长子串
 * 
 * 题目描述：
 * 给定一个字符串 s，请你找出其中不含有重复字符的最长子串的长度。
 * 
 * 示例：
 * 输入: s = "abcabcbb"
 * 输出: 3
 * 解释: 因为无重复字符的最长子串是 "abc"，所以其长度为 3。
 * 
 * 难度：中等
 * 标签：哈希表、字符串、滑动窗口
 */

#ifndef SOLUTION_0003_H
#define SOLUTION_0003_H

#include <string>
#include <unordered_set>
#include <unordered_map>

using namespace std;

class Solution {
public:
    /**
     * @brief 方法一：滑动窗口 + 哈希集合
     * @param s 输入字符串
     * @return 最长子串长度
     * 
     * 时间复杂度: O(n)，最坏情况每个字符被访问两次
     * 空间复杂度: O(min(m, n))，m为字符集大小
     */
    int lengthOfLongestSubstring(string s);
    
    /**
     * @brief 方法二：滑动窗口 + 数组优化
     * @param s 输入字符串
     * @return 最长子串长度
     * 
     * 时间复杂度: O(n)
     * 空间复杂度: O(m)，m为字符集大小（固定128）
     */
    int lengthOfLongestSubstringArray(string s);
    
    /**
     * @brief 方法三：滑动窗口 + 哈希映射（优化版本）
     * @param s 输入字符串
     * @return 最长子串长度
     * 
     * 时间复杂度: O(n)，每个字符只被访问一次
     * 空间复杂度: O(min(m, n))
     */
    int lengthOfLongestSubstringOptimized(string s);
    
    /**
     * @brief 方法四：暴力法（用于对比验证）
     * @param s 输入字符串
     * @return 最长子串长度
     * 
     * 时间复杂度: O(n³)
     * 空间复杂度: O(min(m, n))
     */
    int lengthOfLongestSubstringBruteForce(string s);
};

#endif // SOLUTION_0003_H
