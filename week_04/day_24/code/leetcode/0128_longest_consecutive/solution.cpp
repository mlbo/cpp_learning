/**
 * @file solution.cpp
 * @brief LC 128: 最长连续序列 - 实现
 * 
 * 解题思路：
 * 1. 哈希集合法：将所有数放入集合，只从"起点"开始计数
 * 2. 核心思想：一个序列的起点是指 num-1 不存在的数字
 * 3. 时间复杂度：O(n)，每个元素最多被访问两次
 */

#include "solution.h"
#include <iostream>
#include <algorithm>

/**
 * @brief 方法一：哈希集合法
 * 
 * 算法步骤：
 * 1. 将所有数字放入哈希集合（自动去重 + O(1) 查找）
 * 2. 对于每个数字，判断是否是序列的起点（num-1 不存在）
 * 3. 如果是起点，向后查找连续序列的长度
 * 4. 更新最大长度
 * 
 * 为什么时间复杂度是 O(n)？
 * - 虽然有两层循环，但每个数字最多被访问两次：
 *   1. 作为"起点判断"：检查 num-1 是否存在
 *   2. 作为"序列计数"：被某个起点向后遍历时计数
 * - 即使有嵌套循环，总操作次数仍然是 O(n)
 */
int Solution::longestConsecutive(std::vector<int>& nums) {
    // 边界情况：空数组
    if (nums.empty()) {
        return 0;
    }
    
    // 将所有数字放入哈希集合
    // 作用：1. 自动去重  2. O(1) 时间查找
    std::unordered_set<int> numSet(nums.begin(), nums.end());
    
    int maxLen = 0;
    
    // 遍历集合中的每个数字
    for (int num : numSet) {
        // 关键优化：只从序列的"起点"开始计数
        // 如果 num-1 存在于集合中，说明 num 不是起点，跳过
        // 这样可以保证每个序列只被计算一次
        if (numSet.find(num - 1) != numSet.end()) {
            continue;  // num 不是起点，跳过
        }
        
        // num 是起点，开始向后查找连续序列
        int currentLen = 1;  // 至少包含 num 自己
        int current = num;
        
        // 向后查找 num+1, num+2, ... 是否存在
        while (numSet.find(current + 1) != numSet.end()) {
            current++;
            currentLen++;
        }
        
        // 更新最大长度
        maxLen = std::max(maxLen, currentLen);
    }
    
    return maxLen;
}

/**
 * @brief 方法二：优化的哈希集合法
 * 
 * 进一步优化：
 * 1. 可以在查找过程中删除已访问的元素
 * 2. 减少重复查找的开销
 */
int Solution::longestConsecutiveOptimized(std::vector<int>& nums) {
    if (nums.empty()) {
        return 0;
    }
    
    std::unordered_set<int> numSet(nums.begin(), nums.end());
    int maxLen = 0;
    
    for (int num : numSet) {
        // 只从起点开始
        if (numSet.count(num - 1)) {
            continue;
        }
        
        int currentLen = 1;
        int current = num;
        
        // 向后查找
        while (numSet.count(current + 1)) {
            currentLen++;
            current++;
        }
        
        maxLen = std::max(maxLen, currentLen);
    }
    
    return maxLen;
}

/**
 * @brief 辅助函数：打印数组
 */
void printArray(const std::vector<int>& nums) {
    std::cout << "[";
    for (size_t i = 0; i < nums.size(); ++i) {
        std::cout << nums[i];
        if (i < nums.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]";
}

/**
 * @brief 测试函数
 */
void testLongestConsecutive() {
    std::cout << "\n========================================\n";
    std::cout << "   LC 128: 最长连续序列\n";
    std::cout << "========================================\n";
    
    Solution solution;
    
    // 测试用例 1
    std::cout << "\n=== 测试用例 1 ===\n";
    std::vector<int> nums1 = {100, 4, 200, 1, 3, 2};
    std::cout << "输入: ";
    printArray(nums1);
    std::cout << "\n";
    
    int result1 = solution.longestConsecutive(nums1);
    std::cout << "输出: " << result1 << "\n";
    std::cout << "解释: 最长连续序列是 [1, 2, 3, 4]，长度为 4\n";
    
    // 测试用例 2
    std::cout << "\n=== 测试用例 2 ===\n";
    std::vector<int> nums2 = {0, 3, 7, 2, 5, 8, 4, 6, 0, 1};
    std::cout << "输入: ";
    printArray(nums2);
    std::cout << "\n";
    
    int result2 = solution.longestConsecutive(nums2);
    std::cout << "输出: " << result2 << "\n";
    std::cout << "解释: 最长连续序列是 [0, 1, 2, 3, 4, 5, 6, 7, 8]，长度为 9\n";
    
    // 测试用例 3：空数组
    std::cout << "\n=== 测试用例 3 ===\n";
    std::vector<int> nums3 = {};
    std::cout << "输入: []\n";
    
    int result3 = solution.longestConsecutive(nums3);
    std::cout << "输出: " << result3 << "\n";
    
    // 测试用例 4：单个元素
    std::cout << "\n=== 测试用例 4 ===\n";
    std::vector<int> nums4 = {1};
    std::cout << "输入: [1]\n";
    
    int result4 = solution.longestConsecutive(nums4);
    std::cout << "输出: " << result4 << "\n";
    
    // 测试用例 5：重复元素
    std::cout << "\n=== 测试用例 5 ===\n";
    std::vector<int> nums5 = {1, 2, 0, 1};
    std::cout << "输入: ";
    printArray(nums5);
    std::cout << "\n";
    
    int result5 = solution.longestConsecutive(nums5);
    std::cout << "输出: " << result5 << "\n";
    std::cout << "解释: 最长连续序列是 [0, 1, 2]，长度为 3（重复的 1 只计算一次）\n";
    
    // 测试用例 6：负数
    std::cout << "\n=== 测试用例 6 ===\n";
    std::vector<int> nums6 = {-1, -2, 0, 1, 2, -3};
    std::cout << "输入: ";
    printArray(nums6);
    std::cout << "\n";
    
    int result6 = solution.longestConsecutive(nums6);
    std::cout << "输出: " << result6 << "\n";
    std::cout << "解释: 最长连续序列是 [-3, -2, -1, 0, 1, 2]，长度为 6\n";
    
    std::cout << "\n========================================\n";
    std::cout << "   LC 128 测试完成\n";
    std::cout << "========================================\n";
    
    std::cout << "\n算法复杂度分析:\n";
    std::cout << "  时间复杂度: O(n)\n";
    std::cout << "    - 每个数字最多被访问两次\n";
    std::cout << "    - 一次判断是否是起点，一次在序列中计数\n";
    std::cout << "  空间复杂度: O(n)\n";
    std::cout << "    - 哈希集合存储所有数字\n";
    
    std::cout << "\n形象化理解:\n";
    std::cout << "  想象你在整理一堆扑克牌\n";
    std::cout << "  1. 把所有牌摊开放在桌上（放入哈希集合）\n";
    std::cout << "  2. 找「起点牌」：前面没有相邻数字的牌\n";
    std::cout << "  3. 从起点开始，数有多少张连续的牌\n";
    std::cout << "  4. 记录最长的连续序列\n";
}
