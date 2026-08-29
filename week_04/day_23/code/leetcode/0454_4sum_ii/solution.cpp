/**
 * @file solution.cpp
 * @brief LeetCode 454: 四数相加 II (4Sum II)
 * 
 * 题目描述：
 * 给定四个包含整数的数组 nums1, nums2, nums3, nums4，
 * 计算有多少个元组 (i, j, k, l) 使得：
 * nums1[i] + nums2[j] + nums3[k] + nums4[l] = 0
 * 
 * 示例：
 * 输入：
 * nums1 = [1, 2], nums2 = [-2, -1], nums3 = [-1, 2], nums4 = [0, 2]
 * 输出：2
 * 解释：
 * 两个元组如下：
 * 1. (0, 0, 0, 1) -> nums1[0] + nums2[0] + nums3[0] + nums4[1] = 1 + (-2) + (-1) + 2 = 0
 * 2. (1, 1, 0, 0) -> nums1[1] + nums2[1] + nums3[0] + nums4[0] = 2 + (-1) + (-1) + 0 = 0
 * 
 * 形象化提示：
 * 想象你有四个篮子，每个篮子里装着一些数字卡片。
 * 你需要从每个篮子各取一张卡片，使得四张卡片上的数字之和为 0。
 * 
 * 朴素方法：从四个篮子各取一张卡片，检查所有组合 → 四重循环 → O(n⁴)
 * 优化方法：分成两组，先计算前两个篮子的所有组合和，再计算后两个篮子的组合和
 * 
 * 难度：中等
 * 标签：数组、哈希表
 */

#include <vector>
#include <unordered_map>
#include <iostream>
#include <limits>

namespace leetcode::lc0454 {

class Solution {
public:
    /**
     * @brief 分组哈希表解法 - O(n²) 时间复杂度
     * 
     * 思路：
     * 将四个数组分成两组：
     * - 组1: nums1 + nums2
     * - 组2: nums3 + nums4
     * 
     * 1. 计算 nums1 和 nums2 所有元素对的和，存入哈希表（记录出现次数）
     * 2. 计算 nums3 和 nums4 所有元素对的和，在哈希表中查找相反数
     * 
     * 数学原理：
     * 如果 a + b + c + d = 0，则 (a + b) = -(c + d)
     */
    int fourSumCount(std::vector<int>& nums1, std::vector<int>& nums2,
                     std::vector<int>& nums3, std::vector<int>& nums4) {
        // 哈希表：两数之和 -> 出现次数
        std::unordered_map<long long, int> sumCount;
        
        // 第一阶段：计算 nums1 + nums2 的所有和
        for (int a : nums1) {
            for (int b : nums2) {
                const long long pairSum = static_cast<long long>(a) + b;
                ++sumCount[pairSum];
            }
        }
        
        int result = 0;
        
        // 第二阶段：查找 nums3 + nums4 的相反数
        for (int c : nums3) {
            for (int d : nums4) {
                const long long target = -(static_cast<long long>(c) + d);
                // 如果找到相反数，累加出现次数
                const auto found = sumCount.find(target);
                if (found != sumCount.end()) {
                    result += found->second;
                }
            }
        }
        
        return result;
    }
    
    /**
     * @brief 暴力解法 - O(n⁴) 时间复杂度
     * 
     * 用于对比理解：
     * 四重循环检查所有组合
     */
    int fourSumCountBruteForce(std::vector<int>& nums1, std::vector<int>& nums2,
                               std::vector<int>& nums3, std::vector<int>& nums4) {
        int count = 0;

        // 范围 for 不但避开有符号下标问题，也正确支持四个数组长度不同的情况。
        for (int a : nums1) {
            for (int b : nums2) {
                for (int c : nums3) {
                    for (int d : nums4) {
                        const long long sum = static_cast<long long>(a) + b + c + d;
                        if (sum == 0) {
                            ++count;
                        }
                    }
                }
            }
        }
        
        return count;
    }
};

/**
 * @brief 测试函数
 */
void testFourSumII() {
    Solution sol;
    
    std::cout << "=== LeetCode 454: 四数相加 II 测试 ===\n\n";
    
    // 测试用例 1
    std::vector<int> nums1_1 = {1, 2};
    std::vector<int> nums2_1 = {-2, -1};
    std::vector<int> nums3_1 = {-1, 2};
    std::vector<int> nums4_1 = {0, 2};
    
    int result1 = sol.fourSumCount(nums1_1, nums2_1, nums3_1, nums4_1);
    std::cout << "测试1:\n";
    std::cout << "  nums1 = [1, 2]\n";
    std::cout << "  nums2 = [-2, -1]\n";
    std::cout << "  nums3 = [-1, 2]\n";
    std::cout << "  nums4 = [0, 2]\n";
    std::cout << "结果: " << result1 << " 个满足条件的元组\n\n";
    
    // 测试用例 2
    std::vector<int> nums1_2 = {0};
    std::vector<int> nums2_2 = {0};
    std::vector<int> nums3_2 = {0};
    std::vector<int> nums4_2 = {0};
    
    int result2 = sol.fourSumCount(nums1_2, nums2_2, nums3_2, nums4_2);
    std::cout << "测试2:\n";
    std::cout << "  nums1 = [0], nums2 = [0], nums3 = [0], nums4 = [0]\n";
    std::cout << "结果: " << result2 << " 个满足条件的元组\n\n";
    
    // 测试用例 3
    std::vector<int> nums1_3 = {-1, -1};
    std::vector<int> nums2_3 = {-1, 1};
    std::vector<int> nums3_3 = {-1, 1};
    std::vector<int> nums4_3 = {1, -1};
    
    int result3 = sol.fourSumCount(nums1_3, nums2_3, nums3_3, nums4_3);
    std::cout << "测试3:\n";
    std::cout << "  nums1 = [-1, -1], nums2 = [-1, 1]\n";
    std::cout << "  nums3 = [-1, 1], nums4 = [1, -1]\n";
    std::cout << "结果: " << result3 << " 个满足条件的元组\n\n";
    
    // 性能对比
    std::cout << "=== 算法复杂度对比 ===\n";
    std::cout << "暴力解法: O(n⁴) 时间, O(1) 空间\n";
    std::cout << "分组哈希表解法: O(n²) 时间, O(n²) 空间\n";
    constexpr long long n = 100;
    constexpr long long onePairStage = n * n;
    constexpr long long groupedPairEnumerations = 2 * onePairStage;
    constexpr long long bruteTupleEnumerations = onePairStage * onePairStage;
    constexpr long long enumerationRatio =
        bruteTupleEnumerations / groupedPairEnumerations;
    std::cout << "\n当 n = " << n << " 时，只按循环枚举次数估算：\n";
    std::cout << "  暴力四重循环 n⁴ = " << bruteTupleEnumerations << " 次四元组枚举\n";
    std::cout << "  分组法第一阶段 n² = " << onePairStage << " 次数对枚举\n";
    std::cout << "  分组法第二阶段 n² = " << onePairStage << " 次数对枚举\n";
    std::cout << "  两阶段合计约 2 * n² = " << groupedPairEnumerations << " 次数对枚举\n";
    std::cout << "  枚举数量比约 " << enumerationRatio
              << " 倍；真实耗时还受哈希、分配和缓存影响\n";
}

/**
 * @brief 形象化演示分组哈希表过程
 */
void visualizeFourSumII() {
    std::cout << "\n=== 分组哈希表过程可视化 ===\n\n";
    
    std::vector<int> nums1 = {1, 2};
    std::vector<int> nums2 = {-2, -1};
    std::vector<int> nums3 = {-1, 2};
    std::vector<int> nums4 = {0, 2};
    
    std::cout << "数组:\n";
    std::cout << "  nums1 = [1, 2]\n";
    std::cout << "  nums2 = [-2, -1]\n";
    std::cout << "  nums3 = [-1, 2]\n";
    std::cout << "  nums4 = [0, 2]\n\n";
    
    // 第一阶段：计算 nums1 + nums2
    std::cout << "第一阶段：计算 nums1 + nums2 的所有和\n";
    std::unordered_map<long long, int> sumCount;
    
    for (int a : nums1) {
        for (int b : nums2) {
            const long long sum = static_cast<long long>(a) + b;
            ++sumCount[sum];
            std::cout << "  " << a << " + " << b << " = " << sum << "\n";
        }
    }
    
    std::cout << "\n哈希表内容:\n";
    for (const auto& [sum, count] : sumCount) {
        std::cout << "  和 " << sum << " 出现 " << count << " 次\n";
    }
    
    // 第二阶段：查找 nums3 + nums4 的相反数
    std::cout << "\n第二阶段：查找 nums3 + nums4 的相反数\n";
    int result = 0;
    
    for (int c : nums3) {
        for (int d : nums4) {
            const long long sum = static_cast<long long>(c) + d;
            const long long target = -sum;
            std::cout << "  " << c << " + " << d << " = " << sum;
            std::cout << ", 需要找 " << target;
            
            const auto found = sumCount.find(target);
            if (found != sumCount.end()) {
                std::cout << " → 找到! 贡献 " << found->second << " 个元组\n";
                result += found->second;
            } else {
                std::cout << " → 未找到\n";
            }
        }
    }
    
    std::cout << "\n最终结果: " << result << " 个满足条件的元组\n";
}

/**
 * @brief 展示分治思想
 */
void explainDivideAndConquer() {
    std::cout << "\n=== 分治思想解析 ===\n\n";
    
    std::cout << "问题：四个数组各取一个数，使和为 0\n\n";
    
    std::cout << "朴素思路：四重循环遍历所有组合\n";
    std::cout << "  时间复杂度: O(n⁴)\n\n";
    
    std::cout << "分治思路：将四个数组分成两组\n";
    std::cout << "  组1: nums1 + nums2 → 计算所有两数之和\n";
    std::cout << "  组2: nums3 + nums4 → 计算所有两数之和\n";
    std::cout << "  找满足条件的配对\n";
    std::cout << "  时间复杂度: O(n²)\n\n";
    
    std::cout << "数学原理：\n";
    std::cout << "  若 a + b + c + d = 0\n";
    std::cout << "  则 (a + b) = -(c + d)\n\n";
    
    std::cout << "这就是「空间换时间」的经典应用！\n";
    std::cout << "用 O(n²) 空间存储中间结果，将 O(n⁴) 降到 O(n²)\n";
}

} // namespace leetcode::lc0454

bool verify_lc0454_wide_pair_sum_contract() {
    leetcode::lc0454::Solution solution;
    std::vector<int> first{std::numeric_limits<int>::max()};
    std::vector<int> second{1};
    std::vector<int> third{std::numeric_limits<int>::min()};
    std::vector<int> fourth{0};
    return solution.fourSumCount(first, second, third, fourth) == 1 &&
           solution.fourSumCountBruteForce(first, second, third, fourth) == 1;
}

// 导出测试函数供 main.cpp 调用
void run_four_sum_ii_test() {
    leetcode::lc0454::testFourSumII();
    leetcode::lc0454::visualizeFourSumII();
    leetcode::lc0454::explainDivideAndConquer();
}
