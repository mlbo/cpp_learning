/**
 * @file solution.cpp
 * @brief LeetCode 11: 盛最多水的容器 - 实现
 */

#include "solution.h"
#include <iostream>
#include <cstdint>
#include <stdexcept>

#include "../../../../common/integer_contracts.h"

namespace leetcode_0011 {

namespace {

void validate_input(const std::vector<int>& height) {
    (void)week01::checked_index(height.size());
    if (std::any_of(height.begin(), height.end(), [](int value) { return value < 0; })) {
        throw std::invalid_argument("container heights must be nonnegative");
    }
}

}  // namespace

int Solution::maxArea(std::vector<int>& height) {
    validate_input(height);
    if (height.size() < 2) {
        return 0;
    }
    std::size_t left = 0;
    std::size_t right = height.size() - 1;
    std::int64_t max_water = 0;
    
    while (left < right) {
        // 计算当前容器的面积
        // 高度取较小值（短板效应）
        const int h = std::min(height[left], height[right]);
        const auto width = static_cast<std::int64_t>(right - left);
        const auto area = static_cast<std::int64_t>(h) * width;
        
        // 更新最大面积
        max_water = std::max(max_water, area);
        
        // 移动较矮的边
        // 为什么？假设左边界较矮，如果移动右边界：
        // - 宽度减小
        // - 高度即使增加，也被左边界限制
        // - 所以面积不可能增加
        // 只有移动左边界，高度才可能增加
        if (height[left] < height[right]) {
            left++;
        } else {
            right--;
        }
    }
    
    return week01::checked_result(max_water);
}

int Solution::maxAreaBruteForce(std::vector<int>& height) {
    validate_input(height);
    std::int64_t max_water = 0;
    
    // 枚举所有可能的容器
    for (std::size_t i = 0; i < height.size(); ++i) {
        for (std::size_t j = i + 1; j < height.size(); ++j) {
            const int h = std::min(height[i], height[j]);
            const auto width = static_cast<std::int64_t>(j - i);
            max_water = std::max(max_water, static_cast<std::int64_t>(h) * width);
        }
    }
    
    return week01::checked_result(max_water);
}

} // namespace leetcode_0011
