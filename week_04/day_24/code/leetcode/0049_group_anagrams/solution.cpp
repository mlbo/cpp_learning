/**
 * @file solution.cpp
 * @brief LC 49: 字母异位词分组 - 实现
 */

#include "solution.h"
#include <iostream>
#include <sstream>

/**
 * @brief 方法一：排序哈希法
 * 
 * 算法步骤：
 * 1. 遍历每个字符串
 * 2. 将字符串排序后作为键
 * 3. 将原字符串加入对应的分组
 * 4. 收集所有分组
 */
std::vector<std::vector<std::string>> Solution::groupAnagrams(
    std::vector<std::string>& strs) {
    
    // 哈希表：键是排序后的字符串，值是原字符串列表
    std::unordered_map<std::string, std::vector<std::string>> groups;
    
    for (const std::string& s : strs) {
        // 将字符串排序作为键
        // 排序后，所有字母异位词都会变成相同的字符串
        std::string key = s;
        std::sort(key.begin(), key.end());
        
        // 将原字符串加入对应的分组
        groups[key].push_back(s);
    }
    
    // 收集所有分组
    std::vector<std::vector<std::string>> result;
    result.reserve(groups.size());
    
    for (auto& pair : groups) {
        // 使用 std::move 避免不必要的拷贝
        result.push_back(std::move(pair.second));
    }
    
    return result;
}

/**
 * @brief 方法二：计数哈希法
 * 
 * 算法步骤：
 * 1. 统计每个字符串中各字母出现的次数
 * 2. 将计数编码为字符串作为键（如 "a1b2c3"）
 * 3. 将原字符串加入对应的分组
 * 
 * 这种方法避免了排序，时间复杂度更优
 */
std::vector<std::vector<std::string>> Solution::groupAnagramsCount(
    std::vector<std::string>& strs) {
    
    // 哈希表：键是计数编码字符串，值是原字符串列表
    std::unordered_map<std::string, std::vector<std::string>> groups;
    
    for (const std::string& s : strs) {
        // 统计每个字母出现的次数
        int count[26] = {0};
        for (char c : s) {
            count[c - 'a']++;
        }
        
        // 将计数编码为字符串
        // 例如：abbcc → "a1b2c2"
        std::stringstream ss;
        for (int i = 0; i < 26; ++i) {
            if (count[i] != 0) {
                ss << static_cast<char>('a' + i) << count[i];
            }
        }
        std::string key = ss.str();
        
        // 将原字符串加入对应的分组
        groups[key].push_back(s);
    }
    
    // 收集所有分组
    std::vector<std::vector<std::string>> result;
    result.reserve(groups.size());
    
    for (auto& pair : groups) {
        result.push_back(std::move(pair.second));
    }
    
    return result;
}

/**
 * @brief 辅助函数：打印分组结果
 */
void printGroups(const std::vector<std::vector<std::string>>& groups) {
    std::cout << "分组结果:\n";
    for (size_t i = 0; i < groups.size(); ++i) {
        std::cout << "  分组 " << i + 1 << ": [";
        for (size_t j = 0; j < groups[i].size(); ++j) {
            std::cout << "\"" << groups[i][j] << "\"";
            if (j < groups[i].size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "]\n";
    }
}

/**
 * @brief 测试函数
 */
void testGroupAnagrams() {
    std::cout << "\n========================================\n";
    std::cout << "   LC 49: 字母异位词分组\n";
    std::cout << "========================================\n";
    
    Solution solution;
    
    // 测试用例 1
    std::cout << "\n=== 测试用例 1 ===\n";
    std::vector<std::string> strs1 = {"eat", "tea", "tan", "ate", "nat", "bat"};
    std::cout << "输入: [\"eat\", \"tea\", \"tan\", \"ate\", \"nat\", \"bat\"]\n\n";
    
    std::cout << "方法一：排序哈希法\n";
    auto result1 = solution.groupAnagrams(strs1);
    printGroups(result1);
    
    std::cout << "\n方法二：计数哈希法\n";
    auto result2 = solution.groupAnagramsCount(strs1);
    printGroups(result2);
    
    // 测试用例 2
    std::cout << "\n=== 测试用例 2 ===\n";
    std::vector<std::string> strs2 = {""};
    std::cout << "输入: [\"\"]\n\n";
    
    auto result3 = solution.groupAnagrams(strs2);
    printGroups(result3);
    
    // 测试用例 3
    std::cout << "\n=== 测试用例 3 ===\n";
    std::vector<std::string> strs3 = {"a"};
    std::cout << "输入: [\"a\"]\n\n";
    
    auto result4 = solution.groupAnagrams(strs3);
    printGroups(result4);
    
    // 测试用例 4：更多例子
    std::cout << "\n=== 测试用例 4 ===\n";
    std::vector<std::string> strs4 = {"abc", "bca", "cab", "xyz", "zyx", "yxz", "aaa"};
    std::cout << "输入: [\"abc\", \"bca\", \"cab\", \"xyz\", \"zyx\", \"yxz\", \"aaa\"]\n\n";
    
    auto result5 = solution.groupAnagrams(strs4);
    printGroups(result5);
    
    std::cout << "\n========================================\n";
    std::cout << "   LC 49 测试完成\n";
    std::cout << "========================================\n";
    
    std::cout << "\n算法复杂度分析:\n";
    std::cout << "  排序法: O(N × K log K) 时间, O(N × K) 空间\n";
    std::cout << "  计数法: O(N × K) 时间, O(N × K) 空间\n";
    std::cout << "  其中 N 是字符串数量，K 是字符串最大长度\n";
}
