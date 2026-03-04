/**
 * LeetCode 76: 最小覆盖子串
 * 
 * 给你一个字符串 s 和一个字符串 t。
 * 返回 s 中涵盖 t 所有字符（包括重复字符）的最小子串。
 * 如果不存在这样的子串，返回空字符串。
 */

#ifndef SOLUTION_H
#define SOLUTION_H

#include <string>
#include <vector>
#include <climits>

using namespace std;

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
     * 空间复杂度：O(k)，k是字符集大小
     */
    string minWindow(string s, string t);
};

/**
 * 演示滑动窗口的执行过程
 * 用于理解算法的每一步操作
 */
void demonstrateAlgorithm(const string& s, const string& t);

#endif // SOLUTION_H
