/**
 * LeetCode 567: 字符串的排列
 * 
 * 给你两个字符串 s1 和 s2，写一个函数来判断 s2 是否包含 s1 的排列。
 * 如果是，返回 true；否则，返回 false。
 * 
 * 换句话说，s1 的排列之一是 s2 的子串。
 */

#ifndef SOLUTION_H
#define SOLUTION_H

#include <string>
#include <vector>

using namespace std;

class Solution {
public:
    /**
     * 固定窗口滑动解法
     * 
     * 核心思想：
     * 1. 窗口大小固定为 s1 的长度
     * 2. 维护窗口内字符计数与 s1 的字符计数比较
     * 3. 窗口滑动时增量更新计数
     * 
     * 时间复杂度：O(n)，n 是 s2 的长度
     * 空间复杂度：O(1)，因为字符集大小固定
     */
    bool checkInclusion(string s1, string s2);
    
    /**
     * 优化版本：使用匹配字符数避免数组比较
     * 
     * 核心优化：
     * 维护一个变量 diff 表示当前窗口与目标窗口的字符计数差异
     * 当 diff == 0 时，说明完全匹配
     */
    bool checkInclusionOptimized(string s1, string s2);
};

/**
 * 演示滑动窗口的执行过程
 * 用于理解算法的每一步操作
 */
void demonstratePermutationAlgorithm(const string& s1, const string& s2);

#endif // SOLUTION_H
