/**
 * LeetCode 242. 有效的字母异位词
 * 实现文件
 * 
 * 题目描述：
 * 给定两个字符串 s 和 t，判断 t 是否是 s 的字母异位词。
 * 字母异位词：两个字符串包含相同的字母，但顺序可能不同。
 * 
 * 示例：
 * 输入: s = "anagram", t = "nagaram"
 * 输出: true
 * 
 * 输入: s = "rat", t = "car"
 * 输出: false
 */

#include "solution.h"
#include <iostream>
#include <unordered_map>
#include <algorithm>

/**
 * 方法一：哈希表解法
 * 
 * 思路：
 * 1. 如果两个字符串长度不同，直接返回false
 * 2. 使用哈希表统计s中每个字符的出现次数
 * 3. 遍历t，减少对应字符的计数
 * 4. 如果所有字符计数都为0，则是字母异位词
 */
bool isAnagram_hash(const std::string& s, const std::string& t) {
    // 长度不同，一定不是字母异位词
    if (s.length() != t.length()) {
        return false;
    }
    
    std::unordered_map<char, int> count;
    
    // 统计s中每个字符的出现次数
    for (char c : s) {
        count[c]++;
    }
    
    // 遍历t，减少对应字符的计数
    for (char c : t) {
        count[c]--;
        // 如果计数小于0，说明t中该字符比s中多
        if (count[c] < 0) {
            return false;
        }
    }
    
    return true;
}

/**
 * 方法二：数组解法（最优解）
 * 
 * 思路：
 * 1. 题目说明只有小写字母，可以用长度26的数组代替哈希表
 * 2. 一次遍历同时统计两个字符串
 * 3. 检查数组所有元素是否为0
 * 
 * 优点：
 * - 数组访问比哈希表更快
 * - 空间复杂度为O(1)
 */
bool isAnagram_array(const std::string& s, const std::string& t) {
    // 长度不同，直接返回false
    if (s.length() != t.length()) {
        return false;
    }
    
    // 使用固定大小数组统计字符频率
    int count[26] = {0};
    
    // 一次遍历完成统计
    for (size_t i = 0; i < s.length(); ++i) {
        count[s[i] - 'a']++;  // s中的字符加
        count[t[i] - 'a']--;  // t中的字符减
    }
    
    // 检查是否都为0
    for (int c : count) {
        if (c != 0) {
            return false;
        }
    }
    
    return true;
}

/**
 * 方法三：排序解法
 * 
 * 思路：
 * 1. 将两个字符串排序
 * 2. 比较排序后的字符串是否相等
 * 
 * 缺点：
 * - 时间复杂度较高 O(n log n)
 * - 可能需要额外空间
 */
bool isAnagram_sort(std::string s, std::string t) {
    if (s.length() != t.length()) {
        return false;
    }
    
    std::sort(s.begin(), s.end());
    std::sort(t.begin(), t.end());
    
    return s == t;
}

/**
 * 测试函数
 */
void testValidAnagram() {
    std::cout << "========== LeetCode 242: 有效的字母异位词 ==========" << std::endl;
    
    // 测试用例
    struct TestCase {
        std::string s;
        std::string t;
        bool expected;
    };
    
    TestCase testCases[] = {
        {"anagram", "nagaram", true},
        {"rat", "car", false},
        {"a", "a", true},
        {"ab", "ba", true},
        {"ab", "abc", false},
        {"", "", true},
        {"aacc", "ccac", false}  // a:2,c:2 vs c:3,a:1 不是字母异位词
    };
    
    std::cout << "\n方法一：哈希表解法" << std::endl;
    for (const auto& tc : testCases) {
        bool result = isAnagram_hash(tc.s, tc.t);
        std::cout << "  s = \"" << tc.s << "\", t = \"" << tc.t << "\"";
        std::cout << " -> " << (result ? "true" : "false");
        std::cout << " (期望: " << (tc.expected ? "true" : "false") << ")";
        std::cout << (result == tc.expected ? " ✓" : " ✗") << std::endl;
    }
    
    std::cout << "\n方法二：数组解法（最优）" << std::endl;
    for (const auto& tc : testCases) {
        bool result = isAnagram_array(tc.s, tc.t);
        std::cout << "  s = \"" << tc.s << "\", t = \"" << tc.t << "\"";
        std::cout << " -> " << (result ? "true" : "false");
        std::cout << " (期望: " << (tc.expected ? "true" : "false") << ")";
        std::cout << (result == tc.expected ? " ✓" : " ✗") << std::endl;
    }
    
    std::cout << "\n方法三：排序解法" << std::endl;
    for (const auto& tc : testCases) {
        bool result = isAnagram_sort(tc.s, tc.t);
        std::cout << "  s = \"" << tc.s << "\", t = \"" << tc.t << "\"";
        std::cout << " -> " << (result ? "true" : "false");
        std::cout << " (期望: " << (tc.expected ? "true" : "false") << ")";
        std::cout << (result == tc.expected ? " ✓" : " ✗") << std::endl;
    }
    
    std::cout << "\n复杂度分析：" << std::endl;
    std::cout << "  哈希表法: O(n)时间, O(k)空间" << std::endl;
    std::cout << "  数组法:   O(n)时间, O(1)空间（推荐）" << std::endl;
    std::cout << "  排序法:   O(n log n)时间, O(1)或O(n)空间" << std::endl;
}
