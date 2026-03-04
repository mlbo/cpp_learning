/**
 * LeetCode 496. 下一个更大元素 I
 * 
 * 给你两个没有重复元素的数组 nums1 和 nums2，其中 nums1 是 nums2 的子集。
 * 请你找出 nums1 中每个元素在 nums2 中的下一个更大元素。
 */

#include "solution.h"
#include <iostream>
#include <stack>
#include <unordered_map>

std::vector<int> Solution::nextGreaterElement(std::vector<int>& nums1, std::vector<int>& nums2) {
    // Step 1: 使用单调栈找出nums2中每个元素的下一个更大元素
    std::unordered_map<int, int> nextGreater;
    std::stack<int> stk;
    
    for (int num : nums2) {
        while (!stk.empty() && num > stk.top()) {
            nextGreater[stk.top()] = num;
            stk.pop();
        }
        stk.push(num);
    }
    
    // 栈中剩余元素没有下一个更大元素
    while (!stk.empty()) {
        nextGreater[stk.top()] = -1;
        stk.pop();
    }
    
    // Step 2: 查询nums1中每个元素的结果
    std::vector<int> result;
    for (int num : nums1) {
        result.push_back(nextGreater[num]);
    }
    
    return result;
}

void testNextGreaterElement() {
    Solution sol;
    
    std::cout << "LeetCode 496. 下一个更大元素 I 测试结果：" << std::endl;
    
    // 测试用例1
    std::vector<int> nums1_1 = {4, 1, 2};
    std::vector<int> nums2_1 = {1, 3, 4, 2};
    auto result1 = sol.nextGreaterElement(nums1_1, nums2_1);
    
    std::cout << "  nums1 = [4, 1, 2], nums2 = [1, 3, 4, 2]" << std::endl;
    std::cout << "  输出: [";
    for (int i = 0; i < result1.size(); ++i) {
        std::cout << result1[i];
        if (i < result1.size() - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
    std::cout << "  期望: [-1, 3, -1]" << std::endl;
    
    // 测试用例2
    std::vector<int> nums1_2 = {2, 4};
    std::vector<int> nums2_2 = {1, 2, 3, 4};
    auto result2 = sol.nextGreaterElement(nums1_2, nums2_2);
    
    std::cout << "\n  nums1 = [2, 4], nums2 = [1, 2, 3, 4]" << std::endl;
    std::cout << "  输出: [";
    for (int i = 0; i < result2.size(); ++i) {
        std::cout << result2[i];
        if (i < result2.size() - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
    std::cout << "  期望: [3, -1]" << std::endl;
    
    std::cout << "\n  解题思路：" << std::endl;
    std::cout << "    1. 先用单调栈求出nums2中所有元素的下一个更大元素" << std::endl;
    std::cout << "    2. 用哈希表存储映射关系" << std::endl;
    std::cout << "    3. 查询nums1中每个元素的结果" << std::endl;
}
