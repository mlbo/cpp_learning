/**
 * @file test.cpp
 * @brief LeetCode 15 测试用例
 */

#include "solution.h"
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

void printResult(const std::vector<std::vector<int>>& result) {
    std::cout << "[";
    for (size_t i = 0; i < result.size(); ++i) {
        std::cout << "[";
        for (size_t j = 0; j < result[i].size(); ++j) {
            std::cout << result[i][j];
            if (j < result[i].size() - 1) std::cout << ",";
        }
        std::cout << "]";
        if (i < result.size() - 1) std::cout << ",";
    }
    std::cout << "]";
}

bool compareResults(std::vector<std::vector<int>> a, std::vector<std::vector<int>> b) {
    // 排序每个三元组内部
    for (auto& triplet : a) {
        std::sort(triplet.begin(), triplet.end());
    }
    for (auto& triplet : b) {
        std::sort(triplet.begin(), triplet.end());
    }
    
    // 排序结果集
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    
    return a == b;
}

void runTest(const std::vector<int>& nums, 
             const std::vector<std::vector<int>>& expected, 
             const char* description) {
    Solution sol;
    std::vector<int> input = nums;
    std::vector<std::vector<int>> result = sol.threeSum(input);
    
    std::cout << "测试: " << description << "\n";
    std::cout << "  输入: [";
    for (size_t i = 0; i < nums.size(); ++i) {
        std::cout << nums[i];
        if (i < nums.size() - 1) std::cout << ",";
    }
    std::cout << "]\n";
    
    std::cout << "  结果: ";
    printResult(result);
    std::cout << "\n";
    
    std::cout << "  预期: ";
    printResult(expected);
    std::cout << "\n";
    
    if (compareResults(result, expected)) {
        std::cout << "  ✅ 通过\n\n";
    } else {
        std::cout << "  ❌ 失败\n\n";
    }
}

void explainDeduplication() {
    std::cout << "═══════════════════════════════════════════════════════════════\n";
    std::cout << "去重技巧详解\n";
    std::cout << "═══════════════════════════════════════════════════════════════\n\n";
    
    std::cout << "关键: 在三个层面进行去重\n\n";
    
    std::cout << "1. 第一个数去重 (i):\n";
    std::cout << "   if (i > 0 && nums[i] == nums[i-1]) continue;\n\n";
    std::cout << "   示例: [-1, -1, 0, 1]\n";
    std::cout << "   i=0: nums[0]=-1\n";
    std::cout << "   i=1: nums[1]=-1, 跳过（与nums[0]相同）\n\n";
    
    std::cout << "2. 第二个数去重 (left):\n";
    std::cout << "   while (left < right && nums[left] == nums[left+1]) left++;\n\n";
    std::cout << "   示例: [-2, 0, 0, 2, 2]\n";
    std::cout << "   当找到 [-2, 0, 2] 后\n";
    std::cout << "   跳过第二个0，避免重复\n\n";
    
    std::cout << "3. 第三个数去重 (right):\n";
    std::cout << "   while (left < right && nums[right] == nums[right-1]) right--;\n\n";
    std::cout << "   同上例，跳过重复的2\n\n";
    
    std::cout << "图解:\n";
    std::cout << "   排序后: [-4, -1, -1, 0, 1, 2]\n";
    std::cout << "             i   left      right\n\n";
    std::cout << "   i=1 (nums[1]=-1):\n";
    std::cout << "   - left=2, right=5: -1+(-1)+2=0 → 找到 [-1,-1,2]\n";
    std::cout << "   - 检查去重后 left=3, right=4\n";
    std::cout << "   - left=3, right=4: -1+0+1=0 → 找到 [-1,0,1]\n\n";
    
    std::cout << "注意: i=2 时 nums[2]=-1 与 nums[1] 相同，直接跳过\n\n";
}

void explainAlgorithm() {
    std::cout << "═══════════════════════════════════════════════════════════════\n";
    std::cout << "算法流程\n";
    std::cout << "═══════════════════════════════════════════════════════════════\n\n";
    
    std::cout << "步骤:\n\n";
    
    std::cout << "1. 排序数组\n";
    std::cout << "   - 使双指针技术可行\n";
    std::cout << "   - 方便去重\n\n";
    
    std::cout << "2. 固定第一个数 nums[i]\n";
    std::cout << "   - 从左到右遍历\n";
    std::cout << "   - 跳过重复值\n";
    std::cout << "   - 剪枝: nums[i] > 0 时终止\n\n";
    
    std::cout << "3. 双指针找另外两数\n";
    std::cout << "   - left = i + 1, right = n - 1\n";
    std::cout << "   - 根据sum调整指针:\n";
    std::cout << "     sum < 0: left++ (需要更大的数)\n";
    std::cout << "     sum > 0: right-- (需要更小的数)\n";
    std::cout << "     sum = 0: 记录结果，两边都移动\n\n";
    
    std::cout << "4. 结果去重\n";
    std::cout << "   - 找到有效三元组后\n";
    std::cout << "   - 跳过left重复值\n";
    std::cout << "   - 跳过right重复值\n\n";
    
    std::cout << "复杂度分析:\n";
    std::cout << "  - 时间: O(n²) = O(nlogn)排序 + O(n²)双指针\n";
    std::cout << "  - 空间: O(1) (不考虑输出)\n\n";
}

void run_leetcode15_tests() {
    std::cout << "═══════════════════════════════════════════════════════════════\n";
    std::cout << "LeetCode 15: 三数之和\n";
    std::cout << "═══════════════════════════════════════════════════════════════\n\n";
    
    explainDeduplication();
    explainAlgorithm();
    
    std::cout << "───────────────────────────────────────────────────────────────\n";
    std::cout << "测试用例\n";
    std::cout << "───────────────────────────────────────────────────────────────\n\n";
    
    // 测试用例1: 示例
    runTest({-1, 0, 1, 2, -1, -4}, {{-1, -1, 2}, {-1, 0, 1}}, "示例用例");
    
    // 测试用例2: 空结果
    runTest({0, 1, 1}, {}, "无解情况");
    
    // 测试用例3: 全0
    runTest({0, 0, 0, 0}, {{0, 0, 0}}, "全零情况");
    
    // 测试用例4: 空数组
    runTest({}, {}, "空数组");
    
    // 测试用例5: 两个元素
    runTest({1, 2}, {}, "元素不足");
    
    // 测试用例6: 多个解
    runTest({-2, 0, 1, 1, 2}, {{-2, 0, 2}, {-2, 1, 1}}, "多个解");
    
    // 测试用例7: 大量重复
    runTest({-1, -1, -1, 2, 2, 2}, {{-1, -1, 2}}, "大量重复元素");
    
    // 测试用例8: 正数
    runTest({1, 2, 3, 4, 5}, {}, "全正数");
    
    // 测试用例9: 负数
    runTest({-5, -4, -3, -2, -1}, {}, "全负数");
    
    // 测试用例10: 复杂情况
    runTest({-1, 0, 1, 2, -1, -4, -2, -3, 3, 0, 4}, 
            {{-4, 0, 4}, {-4, 1, 3}, {-3, -1, 4}, {-3, 0, 3}, 
             {-3, 1, 2}, {-2, -1, 3}, {-2, 0, 2}, {-1, -1, 2}, {-1, 0, 1}}, 
            "复杂情况");
    
    std::cout << "═══════════════════════════════════════════════════════════════\n";
    std::cout << "所有测试完成！\n";
    std::cout << "═══════════════════════════════════════════════════════════════\n\n";
}
