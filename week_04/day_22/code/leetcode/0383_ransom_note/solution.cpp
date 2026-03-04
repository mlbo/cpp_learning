/**
 * LeetCode 383. 赎金信
 * 实现文件
 * 
 * 题目描述：
 * 给定一个赎金信字符串 ransomNote 和一个杂志字符串 magazine，
 * 判断 ransomNote 能否由 magazine 中的字符构成。
 * magazine 中的每个字符只能使用一次。
 * 
 * 示例：
 * 输入: ransomNote = "a", magazine = "b"
 * 输出: false
 * 
 * 输入: ransomNote = "aa", magazine = "ab"
 * 输出: false
 * 
 * 输入: ransomNote = "aa", magazine = "aab"
 * 输出: true
 */

#include "solution.h"
#include <iostream>
#include <unordered_map>

/**
 * 方法一：哈希表解法
 * 
 * 思路：
 * 1. 先统计magazine中每个字符的数量
 * 2. 遍历ransomNote，消耗对应字符
 * 3. 如果字符不足，返回false
 */
bool canConstruct_hash(const std::string& ransomNote, const std::string& magazine) {
    // 统计magazine中每个字符的数量
    std::unordered_map<char, int> count;
    for (char c : magazine) {
        count[c]++;
    }
    
    // 检查ransomNote的需求能否被满足
    for (char c : ransomNote) {
        // 如果字符不存在或数量不足
        if (count[c] <= 0) {
            return false;
        }
        count[c]--;
    }
    
    return true;
}

/**
 * 方法二：数组解法（最优解）
 * 
 * 思路：
 * 1. 题目说明只有小写字母，用长度26的数组
 * 2. 先统计magazine的字符
 * 3. 再检查ransomNote的需求
 */
bool canConstruct_array(const std::string& ransomNote, const std::string& magazine) {
    // 统计magazine中每个字符的数量
    int count[26] = {0};
    for (char c : magazine) {
        count[c - 'a']++;
    }
    
    // 检查ransomNote的需求
    for (char c : ransomNote) {
        if (count[c - 'a'] <= 0) {
            return false;
        }
        count[c - 'a']--;
    }
    
    return true;
}

/**
 * 测试函数
 */
void testRansomNote() {
    std::cout << "========== LeetCode 383: 赎金信 ==========" << std::endl;
    
    // 测试用例
    struct TestCase {
        std::string ransomNote;
        std::string magazine;
        bool expected;
    };
    
    TestCase testCases[] = {
        {"a", "b", false},
        {"aa", "ab", false},
        {"aa", "aab", true},
        {"hello", "helloworld", true},
        {"aaaa", "aaab", false},
        {"", "a", true},
        {"a", "", false}
    };
    
    std::cout << "\n方法一：哈希表解法" << std::endl;
    for (const auto& tc : testCases) {
        bool result = canConstruct_hash(tc.ransomNote, tc.magazine);
        std::cout << "  ransomNote = \"" << tc.ransomNote << "\"";
        std::cout << ", magazine = \"" << tc.magazine << "\"";
        std::cout << " -> " << (result ? "true" : "false");
        std::cout << " (期望: " << (tc.expected ? "true" : "false") << ")";
        std::cout << (result == tc.expected ? " ✓" : " ✗") << std::endl;
    }
    
    std::cout << "\n方法二：数组解法（最优）" << std::endl;
    for (const auto& tc : testCases) {
        bool result = canConstruct_array(tc.ransomNote, tc.magazine);
        std::cout << "  ransomNote = \"" << tc.ransomNote << "\"";
        std::cout << ", magazine = \"" << tc.magazine << "\"";
        std::cout << " -> " << (result ? "true" : "false");
        std::cout << " (期望: " << (tc.expected ? "true" : "false") << ")";
        std::cout << (result == tc.expected ? " ✓" : " ✗") << std::endl;
    }
    
    std::cout << "\n解题思路总结：" << std::endl;
    std::cout << "  1. 这是典型的字符计数问题" << std::endl;
    std::cout << "  2. 与LC 242类似，都是用哈希表/数组统计字符频率" << std::endl;
    std::cout << "  3. 区别：LC 242是判断两个字符串字符是否相同" << std::endl;
    std::cout << "         LC 383是判断一个字符串是否包含另一个" << std::endl;
    std::cout << "\n复杂度分析：" << std::endl;
    std::cout << "  时间复杂度: O(m + n)" << std::endl;
    std::cout << "  空间复杂度: O(1) - 固定26大小数组" << std::endl;
}
