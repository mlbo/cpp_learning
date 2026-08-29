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

namespace lc496 {

std::vector<int> Solution::nextGreaterElement(const std::vector<int>& nums1,
                                               const std::vector<int>& nums2) {
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
    result.reserve(nums1.size());
    for (int num : nums1) {
        const auto found = nextGreater.find(num);
        result.push_back(found == nextGreater.end() ? -1 : found->second);
    }
    
    return result;
}

bool testNextGreaterElement() {
    Solution sol;

    std::cout << "LeetCode 496. 下一个更大元素 I 测试结果：" << std::endl;

    struct TestCase {
        std::vector<int> nums1;
        std::vector<int> nums2;
        std::vector<int> expected;
        const char* description;
    };
    const std::vector<TestCase> tests = {
        {{4, 1, 2}, {1, 3, 4, 2}, {-1, 3, -1}, "标准案例"},
        {{2, 4}, {1, 2, 3, 4}, {3, -1}, "递增序列"},
        {{}, {1, 2}, {}, "空查询"},
        {{5}, {1, 2, 3}, {-1}, "工程边界：查询值缺失"}
    };

    bool allPassed = true;
    for (const auto& test : tests) {
        const auto result = sol.nextGreaterElement(test.nums1, test.nums2);
        const bool passed = result == test.expected;
        allPassed = allPassed && passed;
        std::cout << "  " << test.description << ": " << (passed ? "✓" : "✗") << std::endl;
    }
    
    std::cout << "\n  解题思路：" << std::endl;
    std::cout << "    1. 先用单调栈求出nums2中所有元素的下一个更大元素" << std::endl;
    std::cout << "    2. 用哈希表存储映射关系" << std::endl;
    std::cout << "    3. 查询nums1中每个元素的结果" << std::endl;

    return allPassed;
}

}  // namespace lc496
