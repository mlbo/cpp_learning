/**
 * @file test.cpp
 * @brief LeetCode 704. 二分查找 - 测试用例
 */

#include <iostream>
#include <vector>
#include <iomanip>
#include "solution.h"

// 测试用例结构
struct TestCase {
    std::vector<int> nums;
    int target;
    int expected;
    std::string description;
};

// 打印测试结果
void printResult(int testNum, const TestCase& tc, int result) {
    std::cout << "测试 " << testNum << ": " << tc.description << "\n";
    std::cout << "  数组: [";
    for (size_t i = 0; i < tc.nums.size(); ++i) {
        std::cout << tc.nums[i];
        if (i < tc.nums.size() - 1) std::cout << ", ";
    }
    std::cout << "]\n";
    std::cout << "  目标: " << tc.target << "\n";
    std::cout << "  期望: " << tc.expected << "\n";
    std::cout << "  结果: " << result << "\n";
    
    if (result == tc.expected) {
        std::cout << "  状态: ✓ 通过\n";
    } else {
        std::cout << "  状态: ✗ 失败\n";
    }
    std::cout << "\n";
}

int main() {
    std::cout << "========================================\n";
    std::cout << "  LeetCode 704: 二分查找 - 测试\n";
    std::cout << "========================================\n\n";
    
    Solution704 solution;
    
    // 定义测试用例
    std::vector<TestCase> testCases = {
        // 基本测试
        {{-1, 0, 3, 5, 9, 12}, 9, 4, "基本测试 - 目标存在"},
        {{-1, 0, 3, 5, 9, 12}, 2, -1, "基本测试 - 目标不存在"},
        
        // 边界测试
        {{5}, 5, 0, "单元素 - 目标存在"},
        {{5}, 3, -1, "单元素 - 目标不存在"},
        
        // 首尾测试
        {{1, 2, 3, 4, 5}, 1, 0, "查找第一个元素"},
        {{1, 2, 3, 4, 5}, 5, 4, "查找最后一个元素"},
        
        // 边界外测试
        {{1, 2, 3, 4, 5}, 0, -1, "目标小于所有元素"},
        {{1, 2, 3, 4, 5}, 6, -1, "目标大于所有元素"},
        
        // 负数测试
        {{-5, -3, -1, 0, 2, 4}, -3, 1, "负数数组 - 目标存在"},
        {{-5, -3, -1, 0, 2, 4}, -4, -1, "负数数组 - 目标不存在"},
        
        // 较大数组测试
        {{1, 3, 5, 7, 9, 11, 13, 15, 17, 19}, 11, 5, "10元素数组"},
    };
    
    int passed = 0;
    int total = static_cast<int>(testCases.size());
    
    // 运行测试
    for (int i = 0; i < total; ++i) {
        int result = solution.search(testCases[i].nums, testCases[i].target);
        printResult(i + 1, testCases[i], result);
        if (result == testCases[i].expected) {
            passed++;
        }
    }
    
    // 打印总结
    std::cout << "========================================\n";
    std::cout << "  测试总结: " << passed << "/" << total << " 通过\n";
    std::cout << "========================================\n";
    
    // 测试递归版本
    std::cout << "\n--- 递归版本测试 ---\n";
    std::vector<int> nums = {-1, 0, 3, 5, 9, 12};
    int target = 9;
    int result = solution.searchRecursive(nums, target);
    std::cout << "递归查找 " << target << ": 索引 = " << result << "\n";
    
    return (passed == total) ? 0 : 1;
}
