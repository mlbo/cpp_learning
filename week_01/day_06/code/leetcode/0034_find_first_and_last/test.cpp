/**
 * @file test.cpp
 * @brief LeetCode 34. 在排序数组中查找元素的第一个和最后一个位置 - 测试用例
 */

#include <iostream>
#include <vector>
#include <iomanip>
#include "solution.h"

// 测试用例结构
struct TestCase {
    std::vector<int> nums;
    int target;
    std::vector<int> expected;
    std::string description;
};

// 打印数组
void printArray(const std::vector<int>& arr) {
    std::cout << "[";
    for (size_t i = 0; i < arr.size(); ++i) {
        std::cout << arr[i];
        if (i < arr.size() - 1) std::cout << ", ";
    }
    std::cout << "]";
}

// 打印测试结果
void printResult(int testNum, const TestCase& tc, const std::vector<int>& result) {
    std::cout << "测试 " << testNum << ": " << tc.description << "\n";
    std::cout << "  数组: ";
    printArray(tc.nums);
    std::cout << "\n";
    std::cout << "  目标: " << tc.target << "\n";
    std::cout << "  期望: ";
    printArray(tc.expected);
    std::cout << "\n";
    std::cout << "  结果: ";
    printArray(result);
    std::cout << "\n";
    
    if (result == tc.expected) {
        std::cout << "  状态: ✓ 通过\n";
    } else {
        std::cout << "  状态: ✗ 失败\n";
    }
    std::cout << "\n";
}

int main() {
    std::cout << "========================================\n";
    std::cout << "  LeetCode 34: 查找元素的第一个和\n";
    std::cout << "              最后一个位置 - 测试\n";
    std::cout << "========================================\n\n";
    
    Solution34 solution;
    
    // 定义测试用例
    std::vector<TestCase> testCases = {
        // 基本测试
        {{5, 7, 7, 8, 8, 10}, 8, {3, 4}, "基本测试 - 目标存在多次"},
        {{5, 7, 7, 8, 8, 10}, 6, {-1, -1}, "基本测试 - 目标不存在"},
        
        // 边界测试
        {{}, 0, {-1, -1}, "空数组"},
        {{5}, 5, {0, 0}, "单元素 - 目标存在"},
        {{5}, 3, {-1, -1}, "单元素 - 目标不存在"},
        
        // 全相同元素
        {{2, 2, 2, 2, 2}, 2, {0, 4}, "全部相同元素"},
        {{2, 2, 2, 2, 2}, 3, {-1, -1}, "全部相同元素 - 目标不存在"},
        
        // 目标在边界
        {{1, 2, 3, 4, 5}, 1, {0, 0}, "目标在第一个位置"},
        {{1, 2, 3, 4, 5}, 5, {4, 4}, "目标在最后一个位置"},
        
        // 目标出现一次
        {{1, 2, 3, 4, 5}, 3, {2, 2}, "目标出现一次"},
        
        // 目标出现多次
        {{1, 2, 2, 2, 3, 4, 4, 4, 5}, 2, {1, 3}, "目标出现多次 - 2"},
        {{1, 2, 2, 2, 3, 4, 4, 4, 5}, 4, {5, 7}, "目标出现多次 - 4"},
        
        // 负数测试
        {{-5, -3, -3, -3, 0, 2, 2}, -3, {1, 3}, "负数数组 - 目标存在多次"},
        {{-5, -3, -3, -3, 0, 2, 2}, 2, {5, 6}, "负数数组 - 目标在末尾"},
    };
    
    int passed = 0;
    int total = static_cast<int>(testCases.size());
    
    // 运行测试
    for (int i = 0; i < total; ++i) {
        std::vector<int> result = solution.searchRange(testCases[i].nums, testCases[i].target);
        printResult(i + 1, testCases[i], result);
        if (result == testCases[i].expected) {
            passed++;
        }
    }
    
    // 打印总结
    std::cout << "========================================\n";
    std::cout << "  测试总结: " << passed << "/" << total << " 通过\n";
    std::cout << "========================================\n";
    
    // 单独测试左边界和右边界查找
    std::cout << "\n--- 单独测试边界查找 ---\n";
    std::vector<int> nums = {5, 7, 7, 8, 8, 8, 10};
    int target = 8;
    
    std::cout << "数组: ";
    printArray(nums);
    std::cout << ", 目标: " << target << "\n";
    std::cout << "左边界: " << solution.findLeftBound(nums, target) << "\n";
    std::cout << "右边界: " << solution.findRightBound(nums, target) << "\n";
    
    return (passed == total) ? 0 : 1;
}
