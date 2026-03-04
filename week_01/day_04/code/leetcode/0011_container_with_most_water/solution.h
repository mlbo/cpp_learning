/**
 * @file solution.h
 * @brief LeetCode 11: 盛最多水的容器 (Container With Most Water)
 * 
 * 给定一个长度为 n 的整数数组 height，有 n 条垂线，
 * 第 i 条线的两个端点是 (i, 0) 和 (i, height[i])。
 * 
 * 找出其中的两条线，使得它们与 x 轴共同构成的容器可以容纳最多的水。
 * 返回容器可以储存的最大水量。
 * 
 * 示例:
 * 输入: height = [1,8,6,2,5,4,8,3,7]
 * 输出: 49
 * 解释: 选中索引 1 和 8 的两条线，最大水量 = min(8,7) * (8-1) = 49
 */

#ifndef SOLUTION_H
#define SOLUTION_H

#include <vector>
#include <algorithm>

class Solution {
public:
    /**
     * @brief 计算盛最多水的容器能容纳的水量
     * 
     * 方法：双指针贪心算法
     * 
     * 核心思想:
     * 1. 使用左右两个指针，从两端向中间移动
     * 2. 每次计算当前容器的面积，更新最大值
     * 3. 移动较矮的边，因为只有移动较矮边才可能获得更大的面积
     * 4. 如果移动较高的边，宽度减小，高度不可能增加，面积必然减小
     * 
     * @param height 高度数组
     * @return 最大水量
     * 
     * 时间复杂度: O(n)
     * 空间复杂度: O(1)
     */
    int maxArea(std::vector<int>& height);
    
    /**
     * @brief 暴力解法（用于对比）
     * 
     * 时间复杂度: O(n²)
     */
    int maxAreaBruteForce(std::vector<int>& height);
};

#endif // SOLUTION_H
