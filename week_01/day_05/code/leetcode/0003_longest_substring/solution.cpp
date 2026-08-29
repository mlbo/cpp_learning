/**
 * @file solution.cpp
 * @brief LeetCode 3: 无重复字符的最长子串 - 实现文件
 */

#include "solution.h"
#include <algorithm>
#include <array>

#include "../../../../common/integer_contracts.h"

namespace leetcode_0003 {

/**
 * 方法一：滑动窗口 + 哈希集合
 * 
 * 核心思想：
 * 1. 使用双指针left和right维护一个窗口
 * 2. right指针向右扩展窗口，加入新字符
 * 3. 当新字符已在窗口中时，left指针向右收缩窗口直到无重复
 * 4. 使用哈希集合记录窗口内的字符，实现O(1)查找
 */
int Solution::lengthOfLongestSubstring(const std::string& s) {
    (void)week01::checked_index(s.size());
    std::unordered_set<char> window;  // 窗口内的字符集合
    std::size_t left = 0;
    std::size_t max_len = 0;
    
    for (std::size_t right = 0; right < s.size(); ++right) {
        // 当新字符在窗口中时，收缩左边界
        while (window.count(s[right])) {
            window.erase(s[left]);
            ++left;
        }
        
        // 扩展窗口
        window.insert(s[right]);
        max_len = std::max(max_len, right - left + 1);
    }
    
    return week01::checked_index(max_len);
}

/**
 * 方法二：滑动窗口 + 数组优化
 * 
 * 核心思想：
 * 使用固定大小的数组代替哈希集合
 * - 以单字节为单位时共有256种可能值，可以用bool数组标记
 * - 数组访问比哈希集合更快
 */
int Solution::lengthOfLongestSubstringArray(const std::string& s) {
    (void)week01::checked_index(s.size());
    std::array<bool, 256> in_window{};  // 记录字节是否在窗口中
    std::size_t left = 0;
    std::size_t max_len = 0;
    
    for (std::size_t right = 0; right < s.size(); ++right) {
        const unsigned char byte = static_cast<unsigned char>(s[right]);
        
        // 当字符在窗口中时，收缩左边界
        while (in_window[byte]) {
            in_window[static_cast<unsigned char>(s[left])] = false;
            ++left;
        }
        
        // 扩展窗口
        in_window[byte] = true;
        max_len = std::max(max_len, right - left + 1);
    }
    
    return week01::checked_index(max_len);
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
int Solution::lengthOfLongestSubstringOptimized(const std::string& s) {
    (void)week01::checked_index(s.size());
    std::unordered_map<char, std::size_t> last_pos;  // 字符上次位置 + 1
    std::size_t left = 0;
    std::size_t max_len = 0;
    
    for (std::size_t right = 0; right < s.size(); ++right) {
        const char c = s[right];
        
        // 如果字符出现过，且在当前窗口内，更新左边界
        const auto found = last_pos.find(c);
        if (found != last_pos.end()) {
            left = std::max(left, found->second);
        }
        
        last_pos[c] = right + 1;  // 记录位置+1
        max_len = std::max(max_len, right - left + 1);
    }
    
    return week01::checked_index(max_len);
}

/**
 * 方法四：暴力法
 * 
 * 枚举所有子串，检查是否有重复字符
 * 用于验证其他方法的正确性
 */
int Solution::lengthOfLongestSubstringBruteForce(const std::string& s) {
    (void)week01::checked_index(s.size());
    std::size_t max_len = 0;
    
    for (std::size_t i = 0; i < s.size(); ++i) {
        std::unordered_set<char> seen;
        for (std::size_t j = i; j < s.size(); ++j) {
            if (seen.count(s[j])) {
                break;  // 遇到重复字符，停止扩展
            }
            seen.insert(s[j]);
            max_len = std::max(max_len, j - i + 1);
        }
    }
    
    return week01::checked_index(max_len);
}

} // namespace leetcode_0003
