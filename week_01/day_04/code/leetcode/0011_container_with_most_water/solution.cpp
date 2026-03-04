/**
 * @file solution.cpp
 * @brief LeetCode 11: 盛最多水的容器 - 实现
 */

#include "solution.h"
#include <iostream>

int Solution::maxArea(std::vector<int>& height) {
    int left = 0;
    int right = static_cast<int>(height.size()) - 1;
    int maxWater = 0;
    
    while (left < right) {
        // 计算当前容器的面积
        // 高度取较小值（短板效应）
        int h = std::min(height[left], height[right]);
        int width = right - left;
        int area = h * width;
        
        // 更新最大面积
        maxWater = std::max(maxWater, area);
        
        // 移动较矮的边
        // 为什么？假设左边界较矮，如果移动右边界：
        // - 宽度减小
        // - 高度即使增加，也被左边界限制
        // - 所以面积不可能增加
        // 只有移动左边界，高度才可能增加
        if (height[left] < height[right]) {
            left++;
        } else {
            right--;
        }
    }
    
    return maxWater;
}

int Solution::maxAreaBruteForce(std::vector<int>& height) {
    int maxWater = 0;
    int n = static_cast<int>(height.size());
    
    // 枚举所有可能的容器
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            int h = std::min(height[i], height[j]);
            int width = j - i;
            maxWater = std::max(maxWater, h * width);
        }
    }
    
    return maxWater;
}
