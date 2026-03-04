/**
 * @file solution.cpp
 * @brief LeetCode 1: 两数之和 (Two Sum)
 * 
 * 题目描述：
 * 给定一个整数数组 nums 和一个整数目标值 target，
 * 在数组中找出和为目标值的那两个整数，并返回它们的数组下标。
 * 
 * 你可以假设每种输入只会对应一个答案，且同一个元素在答案里不能重复出现。
 * 
 * 示例：
 * 输入：nums = [2, 7, 11, 15], target = 9
 * 输出：[0, 1]
 * 解释：因为 nums[0] + nums[1] == 9，返回 [0, 1]
 * 
 * 形象化提示：
 * 想象你在玩配对游戏，有一排卡片，每张写着一个数字。
 * 裁判喊出一个目标数字，你需要找出哪两张卡片加起来正好等于目标。
 * 如果一张卡片是 2，目标减去 2 是 7，你就需要找那张写着 7 的卡片。
 * 
 * 难度：简单
 * 标签：数组、哈希表
 */

#include <vector>
#include <unordered_map>
#include <iostream>

class Solution {
public:
    /**
     * @brief 哈希表解法 - O(n) 时间复杂度
     * 
     * 思路：
     * 1. 遍历数组，对于每个元素 nums[i]
     * 2. 计算 complement = target - nums[i]
     * 3. 在哈希表中查找 complement
     *    - 如果找到，说明之前见过这个补数，返回两个索引
     *    - 如果没找到，将当前元素存入哈希表
     * 
     * 为什么这样可行？
     * 当我们遍历到位置 j 时，如果存在 i < j 使得 nums[i] + nums[j] = target，
     * 那么 nums[i] 一定已经被存入哈希表，我们可以找到它。
     */
    std::vector<int> twoSum(std::vector<int>& nums, int target) {
        // 哈希表：值 -> 索引
        std::unordered_map<int, int> seen;
        
        for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
            int complement = target - nums[i];
            
            // 查找补数是否已经在哈希表中
            auto it = seen.find(complement);
            if (it != seen.end()) {
                // 找到了！返回两个索引
                return {it->second, i};
            }
            
            // 没找到，将当前元素存入哈希表
            seen[nums[i]] = i;
        }
        
        // 题目保证有解，这行理论上不会执行
        return {};
    }
    
    /**
     * @brief 暴力解法 - O(n²) 时间复杂度
     * 
     * 用于对比理解：
     * 双重循环检查所有配对
     */
    std::vector<int> twoSumBruteForce(std::vector<int>& nums, int target) {
        for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
            for (int j = i + 1; j < static_cast<int>(nums.size()); ++j) {
                if (nums[i] + nums[j] == target) {
                    return {i, j};
                }
            }
        }
        return {};
    }
};

/**
 * @brief 测试函数
 */
void testTwoSum() {
    Solution sol;
    
    std::cout << "=== LeetCode 1: 两数之和 测试 ===\n\n";
    
    // 测试用例 1
    std::vector<int> nums1 = {2, 7, 11, 15};
    int target1 = 9;
    auto result1 = sol.twoSum(nums1, target1);
    std::cout << "测试1: nums = [2, 7, 11, 15], target = 9\n";
    std::cout << "结果: [" << result1[0] << ", " << result1[1] << "]\n";
    std::cout << "验证: nums[" << result1[0] << "] + nums[" << result1[1] << "] = "
              << nums1[result1[0]] << " + " << nums1[result1[1]] << " = "
              << nums1[result1[0]] + nums1[result1[1]] << "\n\n";
    
    // 测试用例 2
    std::vector<int> nums2 = {3, 2, 4};
    int target2 = 6;
    auto result2 = sol.twoSum(nums2, target2);
    std::cout << "测试2: nums = [3, 2, 4], target = 6\n";
    std::cout << "结果: [" << result2[0] << ", " << result2[1] << "]\n";
    std::cout << "验证: nums[" << result2[0] << "] + nums[" << result2[1] << "] = "
              << nums2[result2[0]] << " + " << nums2[result2[1]] << " = "
              << nums2[result2[0]] + nums2[result2[1]] << "\n\n";
    
    // 测试用例 3
    std::vector<int> nums3 = {3, 3};
    int target3 = 6;
    auto result3 = sol.twoSum(nums3, target3);
    std::cout << "测试3: nums = [3, 3], target = 6\n";
    std::cout << "结果: [" << result3[0] << ", " << result3[1] << "]\n";
    std::cout << "验证: nums[" << result3[0] << "] + nums[" << result3[1] << "] = "
              << nums3[result3[0]] << " + " << nums3[result3[1]] << " = "
              << nums3[result3[0]] + nums3[result3[1]] << "\n\n";
    
    // 性能对比
    std::cout << "=== 算法复杂度对比 ===\n";
    std::cout << "暴力解法: O(n²) 时间, O(1) 空间\n";
    std::cout << "哈希表解法: O(n) 时间, O(n) 空间\n";
    std::cout << "\n哈希表通过空间换时间，将查找从 O(n) 降到 O(1)！\n";
}

/**
 * @brief 形象化演示哈希表查找过程
 */
void visualizeHashMap() {
    std::cout << "\n=== 哈希表查找过程可视化 ===\n\n";
    
    std::vector<int> nums = {2, 7, 11, 15};
    int target = 9;
    
    std::unordered_map<int, int> seen;
    
    std::cout << "目标: 找到两个数，和为 " << target << "\n\n";
    
    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        int complement = target - nums[i];
        
        std::cout << "步骤 " << i + 1 << ": 当前元素 = " << nums[i] << "\n";
        std::cout << "  需要找的补数 = " << target << " - " << nums[i] << " = " << complement << "\n";
        
        auto it = seen.find(complement);
        if (it != seen.end()) {
            std::cout << "  在哈希表中找到 " << complement << "! 索引为 " << it->second << "\n";
            std::cout << "  答案: [" << it->second << ", " << i << "]\n";
            break;
        } else {
            std::cout << "  哈希表中没有 " << complement << "\n";
            seen[nums[i]] = i;
            std::cout << "  将 " << nums[i] << " 存入哈希表 (索引 " << i << ")\n";
        }
        std::cout << "\n";
    }
}

// 导出测试函数供 main.cpp 调用
void run_two_sum_test() {
    testTwoSum();
    visualizeHashMap();
}
