/**
 * @file solution.cpp
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
 * 解题思路：
 * 1. 滑动窗口法
 * 2. 使用哈希表记录字符最后出现的位置
 * 3. 维护窗口左右边界
 * 
 * 时间复杂度：O(n)
 * 空间复杂度：O(min(m, n))，m为字符集大小
 */

#include <iostream>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <vector>

// ==================== 解决方案类 ====================

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
     */
    int lengthOfLongestSubstring(std::string s) {
        // 使用哈希表记录字符最后出现的位置
        std::unordered_map<char, int> charIndex;
        int maxLen = 0;
        int left = 0;  // 窗口左边界
        
        // 遍历字符串，right为窗口右边界
        for (int right = 0; right < static_cast<int>(s.size()); ++right) {
            char c = s[right];
            
            // 如果字符已在窗口中出现
            if (charIndex.find(c) != charIndex.end() && charIndex[c] >= left) {
                // 移动左边界到重复字符的下一个位置
                left = charIndex[c] + 1;
            }
            
            // 更新字符位置
            charIndex[c] = right;
            
            // 更新最大长度
            maxLen = std::max(maxLen, right - left + 1);
        }
        
        return maxLen;
    }
    
    /**
     * @brief 使用数组优化的版本
     * 
     * 当字符集较小（如ASCII）时，使用数组比哈希表更快
     * 
     * @param s 输入字符串
     * @return int 最长无重复子串的长度
     */
    int lengthOfLongestSubstringArray(std::string s) {
        // 使用数组记录字符最后出现的位置，初始化为-1
        std::vector<int> charIndex(128, -1);
        int maxLen = 0;
        int left = 0;
        
        for (int right = 0; right < static_cast<int>(s.size()); ++right) {
            char c = s[right];
            
            // 如果字符已在窗口中，更新左边界
            if (charIndex[c] >= left) {
                left = charIndex[c] + 1;
            }
            
            charIndex[c] = right;
            maxLen = std::max(maxLen, right - left + 1);
        }
        
        return maxLen;
    }
};

// ==================== 演示函数 ====================

/**
 * @brief 可视化滑动窗口过程
 */
void visualizeSlidingWindow(const std::string& s) {
    std::cout << "\n可视化滑动窗口过程:\n";
    std::cout << "字符串: \"" << s << "\"\n\n";
    
    std::unordered_map<char, int> charIndex;
    int maxLen = 0;
    int left = 0;
    int maxLeft = 0;
    
    for (int right = 0; right < static_cast<int>(s.size()); ++right) {
        char c = s[right];
        
        std::cout << "步骤 " << right + 1 << ": 处理 '" << c << "'\n";
        
        if (charIndex.find(c) != charIndex.end() && charIndex[c] >= left) {
            std::cout << "  发现重复！'" << c << "' 上次在位置 " << charIndex[c] << "\n";
            left = charIndex[c] + 1;
            std::cout << "  左边界移动到 " << left << "\n";
        }
        
        charIndex[c] = right;
        int currentLen = right - left + 1;
        
        // 打印当前窗口
        std::cout << "  当前窗口: [";
        for (int i = left; i <= right; ++i) {
            std::cout << s[i];
            if (i < right) std::cout << ", ";
        }
        std::cout << "] 长度 = " << currentLen << "\n";
        
        if (currentLen > maxLen) {
            maxLen = currentLen;
            maxLeft = left;
        }
        
        std::cout << "  当前最大长度: " << maxLen << "\n\n";
    }
    
    std::cout << "最长无重复子串: \"";
    for (int i = maxLeft; i < maxLeft + maxLen; ++i) {
        std::cout << s[i];
    }
    std::cout << "\" 长度 = " << maxLen << "\n";
}

namespace leetcode {

void lc0003Demo() {
    std::cout << "【LeetCode 3: 无重复字符的最长子串】\n\n";
    
    Solution solution;
    
    // 测试用例1
    std::string s1 = "abcabcbb";
    std::cout << "测试用例1:\n";
    std::cout << "  输入: \"" << s1 << "\"\n";
    std::cout << "  输出: " << solution.lengthOfLongestSubstring(s1) << "\n";
    std::cout << "  预期: 3\n";
    visualizeSlidingWindow(s1);
    
    // 测试用例2
    std::string s2 = "bbbbb";
    std::cout << "\n测试用例2:\n";
    std::cout << "  输入: \"" << s2 << "\"\n";
    std::cout << "  输出: " << solution.lengthOfLongestSubstring(s2) << "\n";
    std::cout << "  预期: 1\n";
    
    // 测试用例3
    std::string s3 = "pwwkew";
    std::cout << "\n测试用例3:\n";
    std::cout << "  输入: \"" << s3 << "\"\n";
    std::cout << "  输出: " << solution.lengthOfLongestSubstring(s3) << "\n";
    std::cout << "  预期: 3\n";
    
    // 测试用例4：空字符串
    std::string s4 = "";
    std::cout << "\n测试用例4:\n";
    std::cout << "  输入: \"" << s4 << "\"\n";
    std::cout << "  输出: " << solution.lengthOfLongestSubstring(s4) << "\n";
    std::cout << "  预期: 0\n";
    
    // 测试用例5：所有字符都不同
    std::string s5 = "abcdefghijklmnopqrstuvwxyz";
    std::cout << "\n测试用例5:\n";
    std::cout << "  输入: \"" << s5 << "\"\n";
    std::cout << "  输出: " << solution.lengthOfLongestSubstring(s5) << "\n";
    std::cout << "  预期: 26\n";
    
    // 性能对比
    std::cout << "\n性能对比:\n";
    std::cout << "  哈希表版本: " << solution.lengthOfLongestSubstring(s3) << "\n";
    std::cout << "  数组版本: " << solution.lengthOfLongestSubstringArray(s3) << "\n";
    
    std::cout << "\n算法要点总结:\n";
    std::cout << "  1. 滑动窗口维护一个无重复字符的区间\n";
    std::cout << "  2. 哈希表记录字符最后出现的位置\n";
    std::cout << "  3. 遇到重复字符时快速移动左边界\n";
    std::cout << "  4. 时间复杂度O(n)，每个字符最多访问两次\n";
}

} // namespace leetcode

// ==================== 主函数 ====================

#ifdef STANDALONE_TEST
int main() {
    leetcode::lc0003Demo();
    return 0;
}
#endif
