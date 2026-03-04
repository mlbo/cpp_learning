/**
 * LeetCode 647. 回文子串
 * 
 * 给你一个字符串 s ，请你统计并返回这个字符串中回文子串的数目。
 * 
 * 解题方法：中心扩展法
 * 时间复杂度：O(n²)
 * 空间复杂度：O(1)
 * 
 * 核心思想：
 * 与LC 5类似，也是使用中心扩展法
 * 不同的是：LC 5找最长，LC 647统计个数
 * 每次扩展成功，就找到一个回文子串，计数加一
 */

#include "solution.h"
#include <iostream>

int Solution647::countSubstrings(std::string s) {
    int n = s.length();
    int count = 0;
    
    // 中心扩展函数
    // 每次扩展成功就找到一个回文子串
    auto extendPalindrome = [&](int left, int right) {
        // 从中心向两边扩展
        while (left >= 0 && right < n && s[left] == s[right]) {
            count++;   // 找到一个回文子串
            left--;    // 继续向左扩展
            right++;   // 继续向右扩展
        }
        // 扩展结束后，[left+1, right-1] 是一个回文子串
        // 但我们在循环中已经计数了所有可能的回文
    };
    
    // 遍历每个可能的中心点
    for (int i = 0; i < n; ++i) {
        // 以 s[i] 为中心的奇数长度回文
        // 会找到：s[i], s[i-1..i+1], s[i-2..i+2], ...
        extendPalindrome(i, i);
        
        // 以 s[i] 和 s[i+1] 之间为中心的偶数长度回文
        // 会找到：s[i..i+1], s[i-1..i+2], ...
        extendPalindrome(i, i + 1);
    }
    
    return count;
}

void testPalindromicSubstrings() {
    std::cout << "LeetCode 647. 回文子串 测试结果：" << std::endl;
    
    Solution647 sol;
    
    // 测试用例1
    std::string s1 = "abc";
    int result1 = sol.countSubstrings(s1);
    std::cout << "\n  测试用例1:" << std::endl;
    std::cout << "    输入: \"" << s1 << "\"" << std::endl;
    std::cout << "    输出: " << result1 << std::endl;
    std::cout << "    回文子串: \"a\", \"b\", \"c\" (共3个)" << std::endl;
    
    // 测试用例2
    std::string s2 = "aaa";
    int result2 = sol.countSubstrings(s2);
    std::cout << "\n  测试用例2:" << std::endl;
    std::cout << "    输入: \"" << s2 << "\"" << std::endl;
    std::cout << "    输出: " << result2 << std::endl;
    std::cout << "    回文子串: \"a\", \"a\", \"a\", \"aa\", \"aa\", \"aaa\" (共6个)" << std::endl;
    
    // 测试用例3
    std::string s3 = "aba";
    int result3 = sol.countSubstrings(s3);
    std::cout << "\n  测试用例3:" << std::endl;
    std::cout << "    输入: \"" << s3 << "\"" << std::endl;
    std::cout << "    输出: " << result3 << std::endl;
    std::cout << "    回文子串: \"a\", \"b\", \"a\", \"aba\" (共4个)" << std::endl;
    
    // 测试用例4
    std::string s4 = "abba";
    int result4 = sol.countSubstrings(s4);
    std::cout << "\n  测试用例4:" << std::endl;
    std::cout << "    输入: \"" << s4 << "\"" << std::endl;
    std::cout << "    输出: " << result4 << std::endl;
    std::cout << "    回文子串: \"a\", \"b\", \"b\", \"a\", \"bb\", \"abba\" (共6个)" << std::endl;
    
    std::cout << "\n  解题思路（中心扩展法）：" << std::endl;
    std::cout << "    1. 遍历每个字符作为可能的回文中心" << std::endl;
    std::cout << "    2. 从中心向两边扩展" << std::endl;
    std::cout << "    3. 每次扩展成功（左右字符相等），计数加一" << std::endl;
    std::cout << "    4. 继续扩展直到不满足条件" << std::endl;
    
    std::cout << "\n  形象化理解：" << std::endl;
    std::cout << "    把每个字符和每两个字符之间都看作可能的\"镜子中心\"" << std::endl;
    std::cout << "    从镜面向两边扩展，看看能照出多长的对称图像" << std::endl;
    std::cout << "    每扩展成功一次，就发现一个新的回文子串" << std::endl;
    
    std::cout << "\n  LC 5 vs LC 647 对比：" << std::endl;
    std::cout << "    ┌─────────────┬──────────────┬──────────────┐" << std::endl;
    std::cout << "    │    题目     │    LC 5      │    LC 647    │" << std::endl;
    std::cout << "    ├─────────────┼──────────────┼──────────────┤" << std::endl;
    std::cout << "    │    目标     │ 找最长回文   │ 统计回文数   │" << std::endl;
    std::cout << "    │    返回值   │   字符串     │    整数      │" << std::endl;
    std::cout << "    │ 核心方法    │  中心扩展    │  中心扩展    │" << std::endl;
    std::cout << "    │ 时间复杂度  │    O(n²)     │    O(n²)     │" << std::endl;
    std::cout << "    │ 空间复杂度  │    O(1)      │    O(1)      │" << std::endl;
    std::cout << "    └─────────────┴──────────────┴──────────────┘" << std::endl;
}
