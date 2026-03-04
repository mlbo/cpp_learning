/**
 * @file solution.cpp
 * @brief LeetCode 167. 两数之和 II - 输入有序数组
 */

#include <vector>
#include "solution.h"

namespace LC0167 {

std::vector<int> Solution::twoSum(std::vector<int>& numbers, int target) {
    int left = 0;
    int right = static_cast<int>(numbers.size()) - 1;
    
    while (left < right) {
        int sum = numbers[left] + numbers[right];
        
        if (sum == target) {
            return {left + 1, right + 1};
        } else if (sum < target) {
            ++left;
        } else {
            --right;
        }
    }
    
    return {};
}

} // namespace LC0167
