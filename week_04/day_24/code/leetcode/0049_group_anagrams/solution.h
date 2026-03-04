/**
 * @file solution.h
 * @brief LC 49: 字母异位词分组
 * 
 * 题目描述：
 * 给定一个字符串数组，将字母异位词分组在一起。可以按任意顺序返回结果。
 * 字母异位词是由相同字母重新排列形成的字符串。
 * 
 * 示例：
 * 输入: strs = ["eat", "tea", "tan", "ate", "nat", "bat"]
 * 输出: [["bat"], ["nat", "tan"], ["ate", "eat", "tea"]]
 * 
 * 解题思路：
 * 1. 哈希表法：将排序后的字符串作为键
 * 2. 计数法：统计每个字母出现的次数作为键
 */

#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

class Solution {
public:
    /**
     * @brief 方法一：排序哈希法
     * 
     * 核心思想：
     * 字母异位词排序后相同，因此可以将排序后的字符串作为哈希表的键。
     * 
     * 时间复杂度：O(N × K log K)
     *   - N 是字符串数量
     *   - K 是字符串最大长度
     *   - 每个字符串排序需要 O(K log K)
     * 
     * 空间复杂度：O(N × K)
     *   - 存储所有字符串
     */
    std::vector<std::vector<std::string>> groupAnagrams(
        std::vector<std::string>& strs);
    
    /**
     * @brief 方法二：计数哈希法
     * 
     * 核心思想：
     * 字母异位词中每个字母出现的次数相同，可以用计数作为键。
     * 
     * 时间复杂度：O(N × K)
     *   - N 是字符串数量
     *   - K 是字符串最大长度
     *   - 计数遍历是 O(K)，优于排序
     * 
     * 空间复杂度：O(N × K)
     */
    std::vector<std::vector<std::string>> groupAnagramsCount(
        std::vector<std::string>& strs);
};

// 测试函数
void testGroupAnagrams();
