/**
 * @file test.cpp
 * @brief LeetCode 26. 删除有序数组中的重复项 - 测试用例
 */

#include "solution.h"
#include <iostream>
#include <vector>
#include <cassert>
#include <iomanip>

// 辅助函数：打印数组
void printArray(const std::vector<int>& nums, int length) {
    std::cout << "[";
    for (int i = 0; i < length && i < static_cast<int>(nums.size()); ++i) {
        std::cout << nums[i];
        if (i < length - 1 && i < static_cast<int>(nums.size()) - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]";
}

// 辅助函数：打印分隔线
void printSeparator() {
    std::cout << std::string(50, '-') << "\n";
}

/**
 * @brief 测试用例1：基本测试
 */
void testBasic() {
    std::cout << "\n【测试用例1：基本测试】\n";
    printSeparator();
    
    Solution solution;
    
    std::vector<int> nums = {1, 1, 2};
    std::cout << "输入: nums = ";
    printArray(nums, nums.size());
    std::cout << "\n";
    
    int newLength = solution.removeDuplicates(nums);
    
    std::cout << "输出: " << newLength << ", nums = ";
    printArray(nums, newLength);
    std::cout << "\n";
    
    // 验证结果
    assert(newLength == 2);
    assert(nums[0] == 1);
    assert(nums[1] == 2);
    
    std::cout << "✓ 测试通过\n";
}

/**
 * @brief 测试用例2：多个重复
 */
void testMultipleDuplicates() {
    std::cout << "\n【测试用例2：多个重复】\n";
    printSeparator();
    
    Solution solution;
    
    std::vector<int> nums = {0, 0, 1, 1, 1, 2, 2, 3, 3, 4};
    std::cout << "输入: nums = ";
    printArray(nums, nums.size());
    std::cout << "\n";
    
    int newLength = solution.removeDuplicates(nums);
    
    std::cout << "输出: " << newLength << ", nums = ";
    printArray(nums, newLength);
    std::cout << "\n";
    
    // 验证结果
    assert(newLength == 5);
    std::vector<int> expected = {0, 1, 2, 3, 4};
    for (int i = 0; i < newLength; ++i) {
        assert(nums[i] == expected[i]);
    }
    
    std::cout << "✓ 测试通过\n";
}

/**
 * @brief 测试用例3：空数组
 */
void testEmpty() {
    std::cout << "\n【测试用例3：空数组】\n";
    printSeparator();
    
    Solution solution;
    
    std::vector<int> nums;
    std::cout << "输入: nums = []\n";
    
    int newLength = solution.removeDuplicates(nums);
    
    std::cout << "输出: " << newLength << ", nums = []\n";
    
    // 验证结果
    assert(newLength == 0);
    
    std::cout << "✓ 测试通过\n";
}

/**
 * @brief 测试用例4：单元素
 */
void testSingleElement() {
    std::cout << "\n【测试用例4：单元素】\n";
    printSeparator();
    
    Solution solution;
    
    std::vector<int> nums = {1};
    std::cout << "输入: nums = [1]\n";
    
    int newLength = solution.removeDuplicates(nums);
    
    std::cout << "输出: " << newLength << ", nums = [1]\n";
    
    // 验证结果
    assert(newLength == 1);
    assert(nums[0] == 1);
    
    std::cout << "✓ 测试通过\n";
}

/**
 * @brief 测试用例5：无重复
 */
void testNoDuplicates() {
    std::cout << "\n【测试用例5：无重复元素】\n";
    printSeparator();
    
    Solution solution;
    
    std::vector<int> nums = {1, 2, 3, 4, 5};
    std::cout << "输入: nums = ";
    printArray(nums, nums.size());
    std::cout << "\n";
    
    int newLength = solution.removeDuplicates(nums);
    
    std::cout << "输出: " << newLength << ", nums = ";
    printArray(nums, newLength);
    std::cout << "\n";
    
    // 验证结果
    assert(newLength == 5);
    
    std::cout << "✓ 测试通过\n";
}

/**
 * @brief 测试用例6：全部相同
 */
void testAllSame() {
    std::cout << "\n【测试用例6：全部相同元素】\n";
    printSeparator();
    
    Solution solution;
    
    std::vector<int> nums = {1, 1, 1, 1, 1};
    std::cout << "输入: nums = ";
    printArray(nums, nums.size());
    std::cout << "\n";
    
    int newLength = solution.removeDuplicates(nums);
    
    std::cout << "输出: " << newLength << ", nums = ";
    printArray(nums, newLength);
    std::cout << "\n";
    
    // 验证结果
    assert(newLength == 1);
    assert(nums[0] == 1);
    
    std::cout << "✓ 测试通过\n";
}

/**
 * @brief 测试递归方法
 */
void testRecursive() {
    std::cout << "\n【测试用例7：递归方法测试】\n";
    printSeparator();
    
    Solution solution;
    
    std::vector<int> nums = {0, 0, 1, 1, 2, 3, 3};
    std::cout << "输入: nums = ";
    printArray(nums, nums.size());
    std::cout << "\n";
    
    int newLength = solution.removeDuplicatesRecursive(nums);
    
    std::cout << "输出: " << newLength << ", nums = ";
    printArray(nums, newLength);
    std::cout << "\n";
    
    // 验证结果
    assert(newLength == 4);
    std::vector<int> expected = {0, 1, 2, 3};
    for (int i = 0; i < newLength; ++i) {
        assert(nums[i] == expected[i]);
    }
    
    std::cout << "✓ 测试通过\n";
}

/**
 * @brief 性能测试
 */
void testPerformance() {
    std::cout << "\n【性能测试】\n";
    printSeparator();
    
    Solution solution;
    
    // 创建一个大数组
    std::vector<int> nums;
    for (int i = 0; i < 10000; ++i) {
        // 每个数字重复 3 次
        nums.push_back(i / 3 + 1);
    }
    
    std::cout << "数组大小: " << nums.size() << "\n";
    
    int newLength = solution.removeDuplicates(nums);
    
    std::cout << "去重后大小: " << newLength << "\n";
    std::cout << "压缩比: " << std::fixed << std::setprecision(2) 
              << (1.0 - static_cast<double>(newLength) / nums.size()) * 100 << "%\n";
    
    std::cout << "✓ 性能测试完成\n";
}

/**
 * @brief 演示快慢指针的工作过程
 */
void demonstrateProcess() {
    std::cout << "\n【快慢指针过程演示】\n";
    printSeparator();
    
    std::vector<int> nums = {1, 1, 2, 2, 3};
    
    std::cout << "初始数组: ";
    printArray(nums, nums.size());
    std::cout << "\n\n";
    
    int slow = 0;
    std::cout << "步骤详解:\n";
    std::cout << "  初始: slow=0, fast=1\n";
    
    for (int fast = 1; fast < static_cast<int>(nums.size()); ++fast) {
        std::cout << "  fast=" << fast << ": nums[" << fast << "]=" << nums[fast];
        
        if (nums[fast] != nums[slow]) {
            ++slow;
            nums[slow] = nums[fast];
            std::cout << " != nums[" << slow-1 << "] → 复制到位置 " << slow;
        } else {
            std::cout << " == nums[" << slow << "] → 跳过";
        }
        
        std::cout << ", 数组: ";
        printArray(nums, nums.size());
        std::cout << "\n";
    }
    
    std::cout << "\n最终结果: 长度=" << slow + 1 << ", 数组=";
    printArray(nums, slow + 1);
    std::cout << "\n";
}

/**
 * @brief main 函数
 */
int main() {
    std::cout << std::string(50, '=') << "\n";
    std::cout << "  LeetCode 26. 删除有序数组中的重复项 - 测试\n";
    std::cout << std::string(50, '=') << "\n";
    
    // 运行测试
    testBasic();
    testMultipleDuplicates();
    testEmpty();
    testSingleElement();
    testNoDuplicates();
    testAllSame();
    testRecursive();
    testPerformance();
    demonstrateProcess();
    
    std::cout << "\n" << std::string(50, '=') << "\n";
    std::cout << "所有测试通过！\n";
    std::cout << std::string(50, '=') << "\n";
    
    return 0;
}
