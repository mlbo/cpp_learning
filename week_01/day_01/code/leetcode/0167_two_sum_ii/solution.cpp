/**
 * @file solution.cpp
 * @brief LeetCode 167. 两数之和 II - 输入有序数组
 */

#include <vector>
#include "solution.h"
#include "../../../../common/integer_contracts.h"

namespace LC0167 {

std::vector<int> Solution::twoSum(std::vector<int>& numbers, int target) {
    std::size_t left = 0;
    std::size_t right = numbers.size();  // 半开区间，避免空数组执行 size()-1
    
    // right 是排他端点。只有候选区间至少还有两个元素时，左右下标才不同。
    while (right - left >= 2) {
        const std::size_t right_index = right - 1;
        const auto sum = static_cast<std::int64_t>(numbers[left]) +
                         numbers[right_index];
        
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

} // namespace LC0167
