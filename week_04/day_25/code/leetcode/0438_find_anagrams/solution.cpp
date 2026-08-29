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

#include "solution.h"

#include <iostream>
#include <algorithm>
#include <limits>
#include <stdexcept>

// ==================== 解决方案类 ====================

namespace leetcode::lc0438 {

namespace {

std::size_t letterIndex(char character) {
    const auto byte = static_cast<unsigned char>(character);
    const auto first = static_cast<unsigned char>('a');
    const auto last = static_cast<unsigned char>('z');
    if (byte < first || byte > last) {
        throw std::invalid_argument("LC438 教学实现只接受小写英文字母");
    }
    return static_cast<std::size_t>(byte - first);
}

int toProblemIndex(std::size_t index) {
    if (index > static_cast<std::size_t>(std::numeric_limits<int>::max())) {
        throw std::overflow_error("异位词下标超出题目 int 返回类型的范围");
    }
    return static_cast<int>(index);
}

void validateLowercaseInput(const std::string& s, const std::string& p) {
    for (char character : s) {
        static_cast<void>(letterIndex(character));
    }
    for (char character : p) {
        static_cast<void>(letterIndex(character));
    }
}

} // namespace

std::vector<int> Solution::findAnagrams(std::string s, std::string p) {
    std::vector<int> result;
    validateLowercaseInput(s, p);
    if (p.empty() || s.size() < p.size()) {
        return result;
    }

    std::vector<int> pCount(26, 0);
    std::vector<int> windowCount(26, 0);
    for (char c : p) {
        ++pCount[letterIndex(c)];
    }

    const std::size_t windowSize = p.size();
    for (std::size_t i = 0; i < s.size(); ++i) {
        ++windowCount[letterIndex(s[i])];
        if (i >= windowSize) {
            --windowCount[letterIndex(s[i - windowSize])];
        }
        if (i + 1U >= windowSize && windowCount == pCount) {
            result.push_back(toProblemIndex(i + 1U - windowSize));
        }
    }

    return result;
}

std::vector<int> Solution::findAnagramsOptimized(std::string s, std::string p) {
    std::vector<int> result;
    validateLowercaseInput(s, p);
    if (p.empty() || s.size() < p.size()) {
        return result;
    }

    std::vector<int> diff(26, 0);
    for (std::size_t i = 0; i < p.size(); ++i) {
        ++diff[letterIndex(p[i])];
        --diff[letterIndex(s[i])];
    }

    if (allZero(diff)) {
        result.push_back(0);
    }

    for (std::size_t i = p.size(); i < s.size(); ++i) {
        ++diff[letterIndex(s[i - p.size()])];
        --diff[letterIndex(s[i])];
        if (allZero(diff)) {
            result.push_back(toProblemIndex(i - p.size() + 1U));
        }
    }

    return result;
}

bool Solution::allZero(const std::vector<int>& count) const {
    return std::all_of(count.begin(), count.end(), [](int value) {
        return value == 0;
    });
}

} // namespace leetcode::lc0438

// ==================== 演示函数 ====================

/**
 * @brief 可视化滑动窗口过程
 */
namespace leetcode::lc0438 {

void visualizeFindAnagrams(const std::string& s, const std::string& p) {
    std::cout << "\n可视化滑动窗口过程:\n";
    std::cout << "字符串 s: \"" << s << "\"\n";
    std::cout << "模式 p: \"" << p << "\" (窗口大小 = " << p.size() << ")\n\n";
    
    std::vector<int> pCount(26, 0);
    std::vector<int> windowCount(26, 0);
    
    for (char c : p) {
        ++pCount[letterIndex(c)];
    }
    
    std::cout << "p的字符计数: ";
    for (std::size_t i = 0; i < pCount.size(); ++i) {
        if (pCount[i] > 0) {
            const char letter = static_cast<char>('a' + static_cast<int>(i));
            std::cout << letter << ":" << pCount[i] << " ";
        }
    }
    std::cout << "\n\n";
    
    const std::size_t windowSize = p.size();
    
    for (std::size_t i = 0; i < s.size(); ++i) {
        ++windowCount[letterIndex(s[i])];
        
        if (i >= windowSize) {
            --windowCount[letterIndex(s[i - windowSize])];
        }
        
        if (i + 1U >= windowSize) {
            const std::size_t start = i + 1U - windowSize;
            std::cout << "窗口[" << start << "-" << i << "]: \"";
            for (std::size_t j = start; j <= i; ++j) {
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

} // namespace leetcode::lc0438

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
    
    lc0438::Solution solution;
    
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
    
    lc0438::visualizeFindAnagrams(s1, p1);
    
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
