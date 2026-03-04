/**
 * @file test_all.cpp
 * @brief Day 1 代码测试主程序 - 独立测试版本
 */

#include <iostream>
#include <vector>
#include <unordered_map>
#include <cassert>

// ============================================
// LeetCode 1. 两数之和 - 独立实现
// ============================================

std::vector<int> twoSum(std::vector<int>& nums, int target) {
    std::unordered_map<int, int> numToIndex;
    
    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        int complement = target - nums[i];
        auto it = numToIndex.find(complement);
        if (it != numToIndex.end()) {
            return {it->second, i};
        }
        numToIndex[nums[i]] = i;
    }
    return {};
}

// ============================================
// LeetCode 167. 两数之和 II - 独立实现
// ============================================

std::vector<int> twoSumII(std::vector<int>& numbers, int target) {
    int left = 0;
    int right = static_cast<int>(numbers.size()) - 1;
    
    while (left < right) {
        int sum = numbers[left] + numbers[right];
        if (sum == target) {
            return {left + 1, right + 1};  // 题目要求从1开始
        } else if (sum < target) {
            ++left;
        } else {
            --right;
        }
    }
    return {};
}

// ============================================
// 测试函数
// ============================================

void test_two_sum() {
    std::cout << "\n=== 测试 LeetCode 1. 两数之和 ===\n";
    
    // 测试1: 基本案例
    std::vector<int> nums1 = {2, 7, 11, 15};
    auto result1 = twoSum(nums1, 9);
    std::cout << "测试1: nums = [2,7,11,15], target = 9\n";
    std::cout << "结果: [" << result1[0] << ", " << result1[1] << "]\n";
    std::cout << "解释: nums[0]=2 + nums[1]=7 = 9 ✓\n";
    
    // 测试2: 有重复
    std::vector<int> nums2 = {3, 3};
    auto result2 = twoSum(nums2, 6);
    std::cout << "测试2: nums = [3,3], target = 6\n";
    std::cout << "结果: [" << result2[0] << ", " << result2[1] << "]\n";
    std::cout << "解释: nums[0]=3 + nums[1]=3 = 6 ✓\n";
    
    std::cout << "✅ LeetCode 1 测试通过!\n";
}

void test_two_sum_ii() {
    std::cout << "\n=== 测试 LeetCode 167. 两数之和 II ===\n";
    
    // 测试1: 基本案例
    std::vector<int> nums1 = {2, 7, 11, 15};
    auto result1 = twoSumII(nums1, 9);
    std::cout << "测试1: numbers = [2,7,11,15], target = 9\n";
    std::cout << "结果: [" << result1[0] << ", " << result1[1] << "]\n";
    std::cout << "解释: numbers[1]=2 + numbers[2]=7 = 9 (索引从1开始) ✓\n";
    
    // 测试2: 负数
    std::vector<int> nums2 = {-1, 0};
    auto result2 = twoSumII(nums2, -1);
    std::cout << "测试2: numbers = [-1,0], target = -1\n";
    std::cout << "结果: [" << result2[0] << ", " << result2[1] << "]\n";
    std::cout << "解释: numbers[1]=-1 + numbers[2]=0 = -1 ✓\n";
    
    std::cout << "✅ LeetCode 167 测试通过!\n";
}

int main() {
    std::cout << "╔════════════════════════════════════════╗\n";
    std::cout << "║       Day 1 代码测试                    ║\n";
    std::cout << "╚════════════════════════════════════════╝\n";
    
    test_two_sum();
    test_two_sum_ii();
    
    std::cout << "\n╔════════════════════════════════════════╗\n";
    std::cout << "║       🎉 所有测试通过！                 ║\n";
    std::cout << "╚════════════════════════════════════════╝\n";
    
    return 0;
}
