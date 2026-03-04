/**
 * LeetCode 242. 有效的字母异位词
 * 头文件
 */

#ifndef LC0242_VALID_ANAGRAM_H
#define LC0242_VALID_ANAGRAM_H

#include <string>

/**
 * 测试函数
 */
void testValidAnagram();

/**
 * 方法一：哈希表解法
 * 时间复杂度: O(n)
 * 空间复杂度: O(k), k为字符集大小
 */
bool isAnagram_hash(const std::string& s, const std::string& t);

/**
 * 方法二：数组解法（最优）
 * 时间复杂度: O(n)
 * 空间复杂度: O(1) - 固定26大小数组
 */
bool isAnagram_array(const std::string& s, const std::string& t);

/**
 * 方法三：排序解法
 * 时间复杂度: O(n log n)
 * 空间复杂度: O(1) 或 O(n) 取决于排序实现
 */
bool isAnagram_sort(std::string s, std::string t);

#endif // LC0242_VALID_ANAGRAM_H
