/**
 * LeetCode 383. 赎金信
 * 头文件
 */

#ifndef LC0383_RANSOM_NOTE_H
#define LC0383_RANSOM_NOTE_H

#include <string>

/**
 * 测试函数
 */
void testRansomNote();

/**
 * 方法一：哈希表解法
 * 时间复杂度: O(m + n), m为ransomNote长度, n为magazine长度
 * 空间复杂度: O(k), k为字符集大小
 */
bool canConstruct_hash(const std::string& ransomNote, const std::string& magazine);

/**
 * 方法二：数组解法（最优）
 * 时间复杂度: O(m + n)
 * 空间复杂度: O(1) - 固定26大小数组
 */
bool canConstruct_array(const std::string& ransomNote, const std::string& magazine);

#endif // LC0383_RANSOM_NOTE_H
