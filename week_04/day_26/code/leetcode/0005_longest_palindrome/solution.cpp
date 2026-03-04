/**
 * LeetCode 5. 最长回文子串
 * 
 * 给你一个字符串 s，找到 s 中最长的回文子串。
 * 
 * 解题方法：中心扩展法
 * 时间复杂度：O(n²)
 * 空间复杂度：O(1)
 * 
 * 核心思想：
 * 回文串是中心对称的，可以从每个可能的中心向两边扩展
 * 回文中心有两种情况：
 * 1. 单个字符（奇数长度回文，如"aba"）
 * 2. 两个字符之间（偶数长度回文，如"abba"）
 */

#include "solution.h"
#include <iostream>
#include <algorithm>

std::string Solution::longestPalindrome(std::string s) {
    int n = s.length();
    if (n < 2) return s;
    
    int start = 0;   // 最长回文子串的起始位置
    int maxLen = 1;  // 最长回文子串的长度
    
    // 中心扩展函数（使用lambda）
    // left, right 为中心的左右边界
    // 返回扩展后的回文长度
    auto expandAroundCenter = [&](int left, int right) {
        // 当左右都在边界内且字符相等时，继续扩展
        while (left >= 0 && right < n && s[left] == s[right]) {
            left--;
            right++;
        }
        // 循环结束时，left和right是多扩展了一步的
        // 实际回文范围是 [left+1, right-1]
        // 回文长度 = right - left - 1
        int len = right - left - 1;
        if (len > maxLen) {
            maxLen = len;
            start = left + 1;  // 更新起始位置
        }
    };
    
    // 遍历每个可能的中心点
    for (int i = 0; i < n; ++i) {
        // 情况1：以s[i]为中心的奇数长度回文
        // 例如：在"aba"中，以'b'为中心
        expandAroundCenter(i, i);
        
        // 情况2：以s[i]和s[i+1]之间为中心的偶数长度回文
        // 例如：在"abba"中，以两个'b'之间为中心
        expandAroundCenter(i, i + 1);
    }
    
    // 返回最长回文子串
    return s.substr(start, maxLen);
}

void testLongestPalindrome() {
    std::cout << "LeetCode 5. 最长回文子串 测试结果：" << std::endl;
    
    Solution sol;
    
    // 测试用例1
    std::string s1 = "babad";
    std::string result1 = sol.longestPalindrome(s1);
    std::cout << "\n  测试用例1:" << std::endl;
    std::cout << "    输入: \"" << s1 << "\"" << std::endl;
    std::cout << "    输出: \"" << result1 << "\"" << std::endl;
    std::cout << "    说明: \"bab\" 和 \"aba\" 都是正确答案" << std::endl;
    
    // 测试用例2
    std::string s2 = "cbbd";
    std::string result2 = sol.longestPalindrome(s2);
    std::cout << "\n  测试用例2:" << std::endl;
    std::cout << "    输入: \"" << s2 << "\"" << std::endl;
    std::cout << "    输出: \"" << result2 << "\"" << std::endl;
    std::cout << "    说明: 最长回文子串是 \"bb\"" << std::endl;
    
    // 测试用例3
    std::string s3 = "a";
    std::string result3 = sol.longestPalindrome(s3);
    std::cout << "\n  测试用例3:" << std::endl;
    std::cout << "    输入: \"" << s3 << "\"" << std::endl;
    std::cout << "    输出: \"" << result3 << "\"" << std::endl;
    std::cout << "    说明: 单个字符本身就是回文" << std::endl;
    
    // 测试用例4
    std::string s4 = "aaaa";
    std::string result4 = sol.longestPalindrome(s4);
    std::cout << "\n  测试用例4:" << std::endl;
    std::cout << "    输入: \"" << s4 << "\"" << std::endl;
    std::cout << "    输出: \"" << result4 << "\"" << std::endl;
    std::cout << "    说明: 整个字符串都是回文" << std::endl;
    
    std::cout << "\n  解题思路（中心扩展法）：" << std::endl;
    std::cout << "    1. 遍历每个字符作为可能的回文中心" << std::endl;
    std::cout << "    2. 从中心向两边扩展，直到字符不相等" << std::endl;
    std::cout << "    3. 记录过程中发现的最长回文子串" << std::endl;
    std::cout << "    4. 注意处理奇数长度和偶数长度两种情况" << std::endl;
    
    std::cout << "\n  形象化理解：" << std::endl;
    std::cout << "    回文就像一面镜子，中心是镜面" << std::endl;
    std::cout << "    从镜面向两边看，左边的镜像和右边一样" << std::endl;
    std::cout << "    例如 \"aba\"：以 b 为镜面，a|b|a 对称" << std::endl;
    std::cout << "    例如 \"abba\"：以 bb 中间为镜面，ab|ba 对称" << std::endl;
}
