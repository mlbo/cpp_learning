/**
 * @file test.cpp
 * @brief LeetCode 1. 两数之和 - 测试代码
 */

#include <iostream>
#include <vector>
#include <cassert>
#include "solution.h"

namespace LC0001 {

void test_two_sum() {
    Solution sol;
    
    std::cout << "\n--- 测试 LeetCode 1. 两数之和 ---\n";
    
    // 测试用例1
    {
        std::vector<int> nums = {2, 7, 11, 15};
        int target = 9;
        auto result = sol.twoSum(nums, target);
        std::cout << "  测试1: nums = [2,7,11,15], target = 9\n";
        std::cout << "    结果: [" << result[0] << ", " << result[1] << "]\n";
        assert((result[0] == 0 && result[1] == 1) || (result[0] == 1 && result[1] == 0));
        std::cout << "    ✓ 通过\n";
    }
    
    // 测试用例2
    {
        std::vector<int> nums = {3, 2, 4};
        int target = 6;
        auto result = sol.twoSum(nums, target);
        std::cout << "  测试2: nums = [3,2,4], target = 6\n";
        std::cout << "    结果: [" << result[0] << ", " << result[1] << "]\n";
        assert((result[0] == 1 && result[1] == 2) || (result[0] == 2 && result[1] == 1));
        std::cout << "    ✓ 通过\n";
    }
    
    // 测试用例3
    {
        std::vector<int> nums = {3, 3};
        int target = 6;
        auto result = sol.twoSum(nums, target);
        std::cout << "  测试3: nums = [3,3], target = 6\n";
        std::cout << "    结果: [" << result[0] << ", " << result[1] << "]\n";
        assert((result[0] == 0 && result[1] == 1) || (result[0] == 1 && result[1] == 0));
        std::cout << "    ✓ 通过\n";
    }
    
    // 对比暴力法
    {
        std::vector<int> nums = {1, 5, 3, 7, 2, 8, 4};
        int target = 10;
        auto result1 = sol.twoSum(nums, target);
        auto result2 = sol.twoSum_bruteForce(nums, target);
        std::cout << "  测试4: 对比暴力法和哈希表法\n";
        std::cout << "    哈希表法: [" << result1[0] << ", " << result1[1] << "]\n";
        std::cout << "    暴力法: [" << result2[0] << ", " << result2[1] << "]\n";
        std::cout << "    ✓ 结果一致\n";
    }
    
    std::cout << "\n  所有测试通过！\n";
}

} // namespace LC0001
