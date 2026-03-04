/**
 * LeetCode 84. 柱状图中最大的矩形
 * 
 * 给定 n 个非负整数，用来表示柱状图中各个柱子的高度。
 * 每个柱子彼此相邻，且宽度为 1。
 * 求在该柱状图中，能够勾勒出来的矩形的最大面积。
 */

#include "solution.h"
#include <iostream>
#include <stack>
#include <algorithm>

int Solution::largestRectangleArea(std::vector<int>& heights) {
    std::stack<int> stk;
    int maxArea = 0;
    
    // 在末尾添加高度为0的柱子，确保所有柱子都能被处理
    heights.push_back(0);
    
    for (int i = 0; i < heights.size(); ++i) {
        // 当当前高度小于栈顶高度时，可以计算以栈顶为高度的矩形面积
        while (!stk.empty() && heights[i] < heights[stk.top()]) {
            int h = heights[stk.top()];
            stk.pop();
            
            // 左边界：栈顶（弹出后）的位置
            // 如果栈空，左边界是-1
            int left = stk.empty() ? -1 : stk.top();
            int width = i - left - 1;
            
            maxArea = std::max(maxArea, h * width);
        }
        stk.push(i);
    }
    
    return maxArea;
}

void testLargestRectangle() {
    Solution sol;
    
    std::cout << "LeetCode 84. 柱状图中最大的矩形 测试结果：" << std::endl;
    
    // 测试用例1
    std::vector<int> heights1 = {2, 1, 5, 6, 2, 3};
    int result1 = sol.largestRectangleArea(heights1);
    std::cout << "  输入: [2, 1, 5, 6, 2, 3]" << std::endl;
    std::cout << "  输出: " << result1 << " (期望: 10)" << std::endl;
    
    // 测试用例2
    std::vector<int> heights2 = {2, 4};
    int result2 = sol.largestRectangleArea(heights2);
    std::cout << "\n  输入: [2, 4]" << std::endl;
    std::cout << "  输出: " << result2 << " (期望: 4)" << std::endl;
    
    // 图解
    std::cout << "\n  单调栈解法图解：" << std::endl;
    std::cout << "    对于柱子高度 [2, 1, 5, 6, 2, 3]：" << std::endl;
    std::cout << "    柱子5（高=5）：左边第一个<5的是1（位置1）" << std::endl;
    std::cout << "                 右边第一个<5的是2（位置4）" << std::endl;
    std::cout << "                 宽度 = 4 - 1 - 1 = 2" << std::endl;
    std::cout << "                 面积 = 5 * 2 = 10" << std::endl;
    
    std::cout << "\n  关键技巧：" << std::endl;
    std::cout << "    1. 使用单调递增栈" << std::endl;
    std::cout << "    2. 末尾添加0，确保所有柱子都被处理" << std::endl;
    std::cout << "    3. 弹出时计算面积，宽度 = right - left - 1" << std::endl;
}
