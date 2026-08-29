#include <iostream>
#include <stdexcept>
#include "solution.h"

namespace LC0167 {

void test_two_sum_ii() {
    Solution sol;
    
    std::cout << "\n--- 测试 LeetCode 167. 两数之和 II ---\n";
    
    std::vector<int> nums = {2, 7, 11, 15};
    auto result = sol.twoSum(nums, 9);
    if (result != std::vector<int>({1, 2})) {
        throw std::runtime_error("LeetCode 167 基本用例失败");
    }
    std::cout << "  输入: [2,7,11,15], target=9\n";
    std::cout << "  结果: [" << result[0] << ", " << result[1] << "]\n";
    std::cout << "  ✓ 通过\n";

    std::vector<int> converged = {1, 2, 3};
    if (!sol.twoSum(converged, 6).empty()) {
        throw std::runtime_error("LeetCode 167 不能重复使用收敛后的同一下标");
    }

    std::vector<int> single = {3};
    if (!sol.twoSum(single, 6).empty()) {
        throw std::runtime_error("LeetCode 167 单元素输入不能组成答案");
    }
}

} // namespace LC0167
