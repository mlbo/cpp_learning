#include <iostream>
#include <cassert>
#include "solution.h"

namespace LC0167 {

void test_two_sum_ii() {
    Solution sol;
    
    std::cout << "\n--- 测试 LeetCode 167. 两数之和 II ---\n";
    
    std::vector<int> nums = {2, 7, 11, 15};
    auto result = sol.twoSum(nums, 9);
    std::cout << "  输入: [2,7,11,15], target=9\n";
    std::cout << "  结果: [" << result[0] << ", " << result[1] << "]\n";
    assert(result[0] == 1 && result[1] == 2);
    std::cout << "  ✓ 通过\n";
}

} // namespace LC0167
