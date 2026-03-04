/**
 * LeetCode 42. 接雨水
 * 
 * 给定 n 个非负整数表示每个宽度为 1 的柱子的高度图，
 * 计算按此排列的柱子，下雨之后能接多少雨水。
 */

#include "solution.h"
#include <iostream>
#include <stack>
#include <algorithm>

// 方法1：单调栈
int trapStack(std::vector<int>& height) {
    std::stack<int> stk;
    int water = 0;
    
    for (int i = 0; i < height.size(); ++i) {
        while (!stk.empty() && height[i] > height[stk.top()]) {
            int mid = stk.top();
            stk.pop();
            
            if (stk.empty()) break;
            
            int left = stk.top();
            int h = std::min(height[left], height[i]) - height[mid];
            int w = i - left - 1;
            water += h * w;
        }
        stk.push(i);
    }
    
    return water;
}

// 方法2：双指针
int trapTwoPointers(std::vector<int>& height) {
    int left = 0, right = height.size() - 1;
    int leftMax = 0, rightMax = 0;
    int water = 0;
    
    while (left < right) {
        if (height[left] < height[right]) {
            if (height[left] >= leftMax) {
                leftMax = height[left];
            } else {
                water += leftMax - height[left];
            }
            left++;
        } else {
            if (height[right] >= rightMax) {
                rightMax = height[right];
            } else {
                water += rightMax - height[right];
            }
            right--;
        }
    }
    
    return water;
}

int Solution::trap(std::vector<int>& height) {
    return trapStack(height);
}

void testTrap() {
    Solution sol;
    
    std::cout << "LeetCode 42. 接雨水 测试结果：" << std::endl;
    
    // 测试用例1
    std::vector<int> height1 = {0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1};
    int result1 = sol.trap(height1);
    std::cout << "  输入: [0,1,0,2,1,0,1,3,2,1,2,1]" << std::endl;
    std::cout << "  输出: " << result1 << " (期望: 6)" << std::endl;
    
    // 测试用例2
    std::vector<int> height2 = {4, 2, 0, 3, 2, 5};
    int result2 = sol.trap(height2);
    std::cout << "\n  输入: [4,2,0,3,2,5]" << std::endl;
    std::cout << "  输出: " << result2 << " (期望: 9)" << std::endl;
    
    // 图解
    std::cout << "\n  接雨水图解：" << std::endl;
    std::cout << "    对于每个位置，能接的雨水量 = min(左边最高, 右边最高) - 当前高度" << std::endl;
    std::cout << "    位置5（高度0）：左边最高=2，右边最高=3" << std::endl;
    std::cout << "                   min(2,3) - 0 = 2" << std::endl;
    
    std::cout << "\n  两种解法：" << std::endl;
    std::cout << "    1. 单调栈：O(n)时间，O(n)空间" << std::endl;
    std::cout << "    2. 双指针：O(n)时间，O(1)空间" << std::endl;
    
    // 比较两种方法
    std::vector<int> height3 = {0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1};
    std::cout << "\n  双指针法结果: " << trapTwoPointers(height3) << std::endl;
}
