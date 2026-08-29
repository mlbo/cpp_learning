/**
 * LeetCode 76: 最小覆盖子串
 * 
 * 给你一个字符串 s 和一个字符串 t。
 * 返回 s 中涵盖 t 所有字符（包括重复字符）的最小子串。
 * 如果不存在这样的子串，返回空字符串。
 */

#ifndef WEEK04_DAY27_LC0076_SOLUTION_H
#define WEEK04_DAY27_LC0076_SOLUTION_H

#include <string>

namespace day27::lc0076 {

class Solution {
public:
    /**
     * 滑动窗口解法
     * 
     * 核心思想：
     * 1. 使用两个指针left和right维护一个可变窗口
     * 2. 扩展right直到窗口包含所有需要的字符
     * 3. 收缩left寻找最小满足条件的窗口
     * 4. 记录过程中的最小窗口
     * 
     * 时间复杂度：O(n)，n是s的长度
     * 空间复杂度：O(1)，固定统计 256 种字节值
     * 输入按字节处理，先转 unsigned char 再做下标；空 t 返回空串。
     */
    std::string minWindow(std::string s, std::string t);
};

/**
 * 演示滑动窗口的执行过程
 * 用于理解算法的每一步操作；空 t 会直接说明空窗口契约并返回。
 */
void demonstrateAlgorithm(const std::string& s, const std::string& t);

} // namespace day27::lc0076

#endif
