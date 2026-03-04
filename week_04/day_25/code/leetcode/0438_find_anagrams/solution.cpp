/**
 * @file solution.cpp
 * @brief LeetCode 438: 找到字符串中所有字母异位词
 * 
 * 题目描述：
 * 给定两个字符串 s 和 p，找到 s 中所有 p 的字母异位词的起始索引。
 * 字母异位词指由相同字母重新排列形成的字符串。
 * 
 * 示例：
 * 输入: s = "cbaebabacd", p = "abc"
 * 输出: [0, 6]
 * 解释: 起始索引为0的子串"cba"和起始索引为6的子串"bac"都是"abc"的异位词。
 * 
 * 解题思路：
 * 1. 固定大小滑动窗口
 * 2. 使用计数数组比较字符频率
 * 3. 优化：使用diff计数避免完整比较
 * 
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)，固定大小的计数数组
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// ==================== 解决方案类 ====================

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
     */
    std::vector<int> findAnagrams(std::string s, std::string p) {
        std::vector<int> result;
        
        // 边界检查
        if (s.size() < p.size()) {
            return result;
        }
        
        // 使用数组记录字符计数（假设只有小写字母）
        std::vector<int> pCount(26, 0);
        std::vector<int> windowCount(26, 0);
        
        // 统计p的字符
        for (char c : p) {
            pCount[c - 'a']++;
        }
        
        int windowSize = p.size();
        
        // 滑动窗口
        for (int i = 0; i < static_cast<int>(s.size()); ++i) {
            // 添加新字符到窗口
            windowCount[s[i] - 'a']++;
            
            // 移除窗口外的字符
            if (i >= windowSize) {
                windowCount[s[i - windowSize] - 'a']--;
            }
            
            // 窗口形成后，比较计数
            if (i >= windowSize - 1) {
                if (windowCount == pCount) {
                    result.push_back(i - windowSize + 1);
                }
            }
        }
        
        return result;
    }
    
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
     */
    std::vector<int> findAnagramsOptimized(std::string s, std::string p) {
        std::vector<int> result;
        
        if (s.size() < p.size()) {
            return result;
        }
        
        // 使用数组记录差值
        std::vector<int> diff(26, 0);
        
        // 初始化：p中字符计数增加，窗口初始字符计数减少
        for (size_t i = 0; i < p.size(); ++i) {
            diff[p[i] - 'a']++;
            diff[s[i] - 'a']--;
        }
        
        // 检查初始窗口
        if (allZero(diff)) {
            result.push_back(0);
        }
        
        // 滑动窗口
        for (size_t i = p.size(); i < s.size(); ++i) {
            // 移除左边字符（增加计数）
            diff[s[i - p.size()] - 'a']++;
            // 添加右边字符（减少计数）
            diff[s[i] - 'a']--;
            
            // 检查是否匹配
            if (allZero(diff)) {
                result.push_back(i - p.size() + 1);
            }
        }
        
        return result;
    }
    
private:
    /**
     * @brief 检查计数数组是否全为0
     */
    bool allZero(const std::vector<int>& count) {
        for (int c : count) {
            if (c != 0) return false;
        }
        return true;
    }
};

// ==================== 演示函数 ====================

/**
 * @brief 可视化滑动窗口过程
 */
void visualizeFindAnagrams(const std::string& s, const std::string& p) {
    std::cout << "\n可视化滑动窗口过程:\n";
    std::cout << "字符串 s: \"" << s << "\"\n";
    std::cout << "模式 p: \"" << p << "\" (窗口大小 = " << p.size() << ")\n\n";
    
    std::vector<int> pCount(26, 0);
    std::vector<int> windowCount(26, 0);
    
    for (char c : p) {
        pCount[c - 'a']++;
    }
    
    std::cout << "p的字符计数: ";
    for (int i = 0; i < 26; ++i) {
        if (pCount[i] > 0) {
            std::cout << (char)('a' + i) << ":" << pCount[i] << " ";
        }
    }
    std::cout << "\n\n";
    
    int windowSize = p.size();
    
    for (int i = 0; i < static_cast<int>(s.size()); ++i) {
        windowCount[s[i] - 'a']++;
        
        if (i >= windowSize) {
            windowCount[s[i - windowSize] - 'a']--;
        }
        
        if (i >= windowSize - 1) {
            int start = i - windowSize + 1;
            std::cout << "窗口[" << start << "-" << i << "]: \"";
            for (int j = start; j <= i; ++j) {
                std::cout << s[j];
            }
            std::cout << "\"";
            
            if (windowCount == pCount) {
                std::cout << " ← 匹配！";
            }
            std::cout << "\n";
        }
    }
}

/**
 * @brief 解释字母异位词概念
 */
void explainAnagram() {
    std::cout << "【字母异位词概念解释】\n\n";
    
    std::cout << "定义：字母异位词(Anagram)是指由相同字母重新排列形成的字符串。\n\n";
    
    std::cout << "例子：\n";
    std::cout << "  \"abc\" 的异位词有：\"abc\", \"acb\", \"bac\", \"bca\", \"cab\", \"cba\"\n";
    std::cout << "  \"aab\" 的异位词有：\"aab\", \"aba\", \"baa\"\n\n";
    
    std::cout << "特点：\n";
    std::cout << "  1. 两个字符串长度相同\n";
    std::cout << "  2. 包含完全相同的字符\n";
    std::cout << "  3. 每个字符出现次数相同\n";
    std::cout << "  4. 字符顺序可以不同\n\n";
    
    std::cout << "判断方法：\n";
    std::cout << "  方法1：排序后比较 - O(n log n)\n";
    std::cout << "  方法2：计数比较 - O(n)\n";
    std::cout << "  本题使用计数比较法\n";
}

namespace leetcode {

void lc0438Demo() {
    std::cout << "【LeetCode 438: 找到字符串中所有字母异位词】\n\n";
    
    // 先解释概念
    explainAnagram();
    
    Solution solution;
    
    // 测试用例1
    std::string s1 = "cbaebabacd";
    std::string p1 = "abc";
    std::cout << "\n测试用例1:\n";
    std::cout << "  s = \"" << s1 << "\"\n";
    std::cout << "  p = \"" << p1 << "\"\n";
    
    auto result1 = solution.findAnagrams(s1, p1);
    std::cout << "  输出: [";
    for (size_t i = 0; i < result1.size(); ++i) {
        std::cout << result1[i];
        if (i < result1.size() - 1) std::cout << ", ";
    }
    std::cout << "]\n";
    std::cout << "  预期: [0, 6]\n";
    
    visualizeFindAnagrams(s1, p1);
    
    // 测试用例2
    std::string s2 = "abab";
    std::string p2 = "ab";
    std::cout << "\n测试用例2:\n";
    std::cout << "  s = \"" << s2 << "\"\n";
    std::cout << "  p = \"" << p2 << "\"\n";
    
    auto result2 = solution.findAnagrams(s2, p2);
    std::cout << "  输出: [";
    for (size_t i = 0; i < result2.size(); ++i) {
        std::cout << result2[i];
        if (i < result2.size() - 1) std::cout << ", ";
    }
    std::cout << "]\n";
    std::cout << "  预期: [0, 1, 2]\n";
    
    // 测试用例3：无匹配
    std::string s3 = "abcdefg";
    std::string p3 = "xyz";
    std::cout << "\n测试用例3:\n";
    std::cout << "  s = \"" << s3 << "\"\n";
    std::cout << "  p = \"" << p3 << "\"\n";
    
    auto result3 = solution.findAnagrams(s3, p3);
    std::cout << "  输出: [";
    for (size_t i = 0; i < result3.size(); ++i) {
        std::cout << result3[i];
        if (i < result3.size() - 1) std::cout << ", ";
    }
    std::cout << "]\n";
    std::cout << "  预期: []\n";
    
    // 测试用例4：s比p短
    std::string s4 = "a";
    std::string p4 = "ab";
    std::cout << "\n测试用例4:\n";
    std::cout << "  s = \"" << s4 << "\" (比p短)\n";
    std::cout << "  p = \"" << p4 << "\"\n";
    
    auto result4 = solution.findAnagrams(s4, p4);
    std::cout << "  输出: []\n";
    std::cout << "  预期: []\n";
    
    // 性能对比
    std::cout << "\n性能对比:\n";
    std::cout << "  基础版本结果: [";
    for (size_t i = 0; i < result1.size(); ++i) {
        std::cout << result1[i];
        if (i < result1.size() - 1) std::cout << ", ";
    }
    std::cout << "]\n";
    
    auto resultOpt = solution.findAnagramsOptimized(s1, p1);
    std::cout << "  优化版本结果: [";
    for (size_t i = 0; i < resultOpt.size(); ++i) {
        std::cout << resultOpt[i];
        if (i < resultOpt.size() - 1) std::cout << ", ";
    }
    std::cout << "]\n";
    
    std::cout << "\n算法要点总结:\n";
    std::cout << "  1. 使用固定大小的滑动窗口\n";
    std::cout << "  2. 用计数数组记录字符频率\n";
    std::cout << "  3. 窗口滑动时只需更新两个字符的计数\n";
    std::cout << "  4. 比较两个计数数组是否相等\n";
    std::cout << "  5. 时间复杂度O(n)，空间复杂度O(1)\n";
}

} // namespace leetcode

// ==================== 主函数 ====================

#ifdef STANDALONE_TEST
int main() {
    leetcode::lc0438Demo();
    return 0;
}
#endif
