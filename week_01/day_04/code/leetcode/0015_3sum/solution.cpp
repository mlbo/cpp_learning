/**
 * @file solution.cpp
 * @brief LeetCode 15: 三数之和 - 实现
 */

#include "solution.h"

std::vector<std::vector<int>> Solution::threeSum(std::vector<int>& nums) {
    std::vector<std::vector<int>> result;
    int n = static_cast<int>(nums.size());
    
    // 边界情况：少于3个元素
    if (n < 3) {
        return result;
    }
    
    // 排序：为双指针和去重做准备
    std::sort(nums.begin(), nums.end());
    
    // 遍历第一个数
    for (int i = 0; i < n - 2; ++i) {
        // ========== 去重1: 跳过相同的第一个数 ==========
        // 如果当前数与前一个数相同，跳过（避免重复三元组）
        if (i > 0 && nums[i] == nums[i - 1]) {
            continue;
        }
        
        // ========== 剪枝优化 ==========
        // 如果第一个数已经大于0，后面的数更大，不可能和为0
        if (nums[i] > 0) {
            break;
        }
        
        // 双指针找另外两个数
        int left = i + 1;
        int right = n - 1;
        
        while (left < right) {
            long long sum = static_cast<long long>(nums[i]) + 
                           static_cast<long long>(nums[left]) + 
                           static_cast<long long>(nums[right]);
            
            if (sum == 0) {
                // 找到一个有效的三元组
                result.push_back({nums[i], nums[left], nums[right]});
                
                // ========== 去重2: 跳过相同的第二个数 ==========
                while (left < right && nums[left] == nums[left + 1]) {
                    left++;
                }
                
                // ========== 去重3: 跳过相同的第三个数 ==========
                while (left < right && nums[right] == nums[right - 1]) {
                    right--;
                }
                
                // 移动指针
                left++;
                right--;
                
            } else if (sum < 0) {
                // 和太小，需要增大，移动left
                left++;
            } else {
                // 和太大，需要减小，移动right
                right--;
            }
        }
    }
    
    return result;
}
