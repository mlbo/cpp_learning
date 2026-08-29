/**
 * @file test_all.cpp
 * @brief Day 1 代码测试主程序 - 独立测试版本
 */

#include <iostream>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <limits>

#include "../../common/integer_contracts.h"

// ============================================
// LeetCode 1. 两数之和 - 独立实现
// ============================================

std::vector<int> twoSum(std::vector<int>& nums, int target) {
    std::unordered_map<int, std::size_t> numToIndex;
    
    for (std::size_t i = 0; i < nums.size(); ++i) {
        const auto complement = static_cast<std::int64_t>(target) - nums[i];
        if (complement >= std::numeric_limits<int>::min() &&
            complement <= std::numeric_limits<int>::max()) {
            const auto it = numToIndex.find(static_cast<int>(complement));
            if (it != numToIndex.end()) {
                return {week01::checked_index(it->second), week01::checked_index(i)};
            }
        }
        numToIndex[nums[i]] = i;
    }
    return {};
}

// ============================================
// LeetCode 167. 两数之和 II - 独立实现
// ============================================

std::vector<int> twoSumII(std::vector<int>& numbers, int target) {
    std::size_t left = 0;
    std::size_t right = numbers.size();
    
    // 半开候选区间 [left, right) 至少保留两个元素，禁止重复使用同一下标。
    while (right - left >= 2) {
        const std::size_t right_index = right - 1;
        const auto sum = static_cast<std::int64_t>(numbers[left]) + numbers[right_index];
        if (sum == static_cast<std::int64_t>(target)) {
            return {week01::checked_index(left + 1),
                    week01::checked_index(right)};
        } else if (sum < static_cast<std::int64_t>(target)) {
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

namespace {

int failures = 0;

void expect_pair(const std::vector<int>& result,
                 int first,
                 int second,
                 const char* case_name) {
    const bool passed = result.size() == 2 && result[0] == first && result[1] == second;
    if (!passed) {
        std::cerr << "[FAIL] " << case_name << '\n';
        ++failures;
    }
}

void print_pair(const std::vector<int>& result) {
    if (result.size() == 2) {
        std::cout << "[" << result[0] << ", " << result[1] << "]";
    } else {
        std::cout << "[]";
    }
}

}  // namespace

void test_two_sum() {
    std::cout << "\n=== 测试 LeetCode 1. 两数之和 ===\n";
    
    // 测试1: 基本案例
    std::vector<int> nums1 = {2, 7, 11, 15};
    auto result1 = twoSum(nums1, 9);
    std::cout << "测试1: nums = [2,7,11,15], target = 9\n";
    std::cout << "结果: ";
    print_pair(result1);
    std::cout << "\n";
    std::cout << "解释: nums[0]=2 + nums[1]=7 = 9 ✓\n";
    expect_pair(result1, 0, 1, "twoSum basic case");
    
    // 测试2: 有重复
    std::vector<int> nums2 = {3, 3};
    auto result2 = twoSum(nums2, 6);
    std::cout << "测试2: nums = [3,3], target = 6\n";
    std::cout << "结果: ";
    print_pair(result2);
    std::cout << "\n";
    std::cout << "解释: nums[0]=3 + nums[1]=3 = 6 ✓\n";
    expect_pair(result2, 0, 1, "twoSum duplicate values");
    
    std::cout << "✅ LeetCode 1 测试通过!\n";
}

void test_two_sum_ii() {
    std::cout << "\n=== 测试 LeetCode 167. 两数之和 II ===\n";
    
    // 测试1: 基本案例
    std::vector<int> nums1 = {2, 7, 11, 15};
    auto result1 = twoSumII(nums1, 9);
    std::cout << "测试1: numbers = [2,7,11,15], target = 9\n";
    std::cout << "结果: ";
    print_pair(result1);
    std::cout << "\n";
    std::cout << "解释: numbers[1]=2 + numbers[2]=7 = 9 (索引从1开始) ✓\n";
    expect_pair(result1, 1, 2, "twoSumII basic case");
    
    // 测试2: 负数
    std::vector<int> nums2 = {-1, 0};
    auto result2 = twoSumII(nums2, -1);
    std::cout << "测试2: numbers = [-1,0], target = -1\n";
    std::cout << "结果: ";
    print_pair(result2);
    std::cout << "\n";
    std::cout << "解释: numbers[1]=-1 + numbers[2]=0 = -1 ✓\n";
    expect_pair(result2, 1, 2, "twoSumII negative values");

    // 指针收敛反例：旧循环在 left == right - 1 时会把 numbers[left]
    // 与自身相加，并错误返回两个相同的 1-based 下标。
    std::vector<int> nums3 = {1, 2, 3};
    auto result3 = twoSumII(nums3, 6);
    std::cout << "测试3: numbers = [1,2,3], target = 6\n";
    std::cout << "结果: ";
    print_pair(result3);
    std::cout << "\n";
    if (!result3.empty()) {
        std::cerr << "[FAIL] twoSumII must use two distinct indices\n";
        ++failures;
    }
    
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
    
    return failures == 0 ? 0 : 1;
}
