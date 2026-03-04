/**
 * @file solution.cpp
 * @brief LeetCode 3: 无重复字符的最长子串 - 实现文件
 */

#include "solution.h"
#include <algorithm>

/**
 * 方法一：滑动窗口 + 哈希集合
 * 
 * 核心思想：
 * 1. 使用双指针left和right维护一个窗口
 * 2. right指针向右扩展窗口，加入新字符
 * 3. 当新字符已在窗口中时，left指针向右收缩窗口直到无重复
 * 4. 使用哈希集合记录窗口内的字符，实现O(1)查找
 */
int Solution::lengthOfLongestSubstring(string s) {
    unordered_set<char> window;  // 窗口内的字符集合
    int left = 0;
    int maxLen = 0;
    
    for (int right = 0; right < s.size(); ++right) {
        // 当新字符在窗口中时，收缩左边界
        while (window.count(s[right])) {
            window.erase(s[left]);
            ++left;
        }
        
        // 扩展窗口
        window.insert(s[right]);
        maxLen = max(maxLen, right - left + 1);
    }
    
    return maxLen;
}

/**
 * 方法二：滑动窗口 + 数组优化
 * 
 * 核心思想：
 * 使用固定大小的数组代替哈希集合
 * - ASCII字符集大小为128，可以用bool数组标记字符是否在窗口中
 * - 数组访问比哈希集合更快
 */
int Solution::lengthOfLongestSubstringArray(string s) {
    bool inWindow[128] = {false};  // 记录字符是否在窗口中
    int left = 0;
    int maxLen = 0;
    
    for (int right = 0; right < s.size(); ++right) {
        char c = s[right];
        
        // 当字符在窗口中时，收缩左边界
        while (inWindow[static_cast<unsigned char>(c)]) {
            inWindow[static_cast<unsigned char>(s[left])] = false;
            ++left;
        }
        
        // 扩展窗口
        inWindow[static_cast<unsigned char>(c)] = true;
        maxLen = max(maxLen, right - left + 1);
    }
    
    return maxLen;
}

/**
 * 方法三：滑动窗口 + 哈希映射（优化版本）
 * 
 * 核心思想：
 * 1. 使用哈希映射记录每个字符最后出现的位置
 * 2. 当遇到重复字符时，直接将left移动到重复字符位置的下一个位置
 * 3. 避免了逐个移动left的低效操作
 * 
 * 关键点：
 * - 使用 right + 1 记录位置，避免0的歧义
 * - left只能向右移动，不能回退
 */
int Solution::lengthOfLongestSubstringOptimized(string s) {
    unordered_map<char, int> lastPos;  // 字符最后出现的位置
    int left = 0;
    int maxLen = 0;
    
    for (int right = 0; right < s.size(); ++right) {
        char c = s[right];
        
        // 如果字符出现过，且在当前窗口内，更新左边界
        if (lastPos.count(c) && lastPos[c] > left) {
            left = lastPos[c];
        }
        
        lastPos[c] = right + 1;  // 记录位置+1
        maxLen = max(maxLen, right - left + 1);
    }
    
    return maxLen;
}

/**
 * 方法四：暴力法
 * 
 * 枚举所有子串，检查是否有重复字符
 * 用于验证其他方法的正确性
 */
int Solution::lengthOfLongestSubstringBruteForce(string s) {
    int n = s.size();
    int maxLen = 0;
    
    for (int i = 0; i < n; ++i) {
        unordered_set<char> seen;
        for (int j = i; j < n; ++j) {
            if (seen.count(s[j])) {
                break;  // 遇到重复字符，停止扩展
            }
            seen.insert(s[j]);
            maxLen = max(maxLen, j - i + 1);
        }
    }
    
    return maxLen;
}
