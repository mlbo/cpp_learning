/**
 * @file solution.cpp
 * @brief LeetCode 189: 轮转数组 - 解法实现
 */

#include "solution.h"
#include <algorithm>
#include <numeric>

namespace leetcode {

// ============================================================================
// 解法1: 数组翻转法 (推荐)
// ============================================================================

void Solution::rotate_reverse(std::vector<int>& nums, int k) {
    int n = static_cast<int>(nums.size());
    if (n == 0) return;
    
    k = k % n;  // 处理 k > n 的情况
    if (k == 0) return;  // 不需要轮转
    
    // 步骤1: 翻转整个数组
    // [1,2,3,4,5,6,7] -> [7,6,5,4,3,2,1]
    std::reverse(nums.begin(), nums.end());
    
    // 步骤2: 翻转前 k 个元素
    // [7,6,5,4,3,2,1] -> [5,6,7,4,3,2,1]
    std::reverse(nums.begin(), nums.begin() + k);
    
    // 步骤3: 翻转剩余元素
    // [5,6,7,4,3,2,1] -> [5,6,7,1,2,3,4]
    std::reverse(nums.begin() + k, nums.end());
}

// ============================================================================
// 解法2: 额外数组法
// ============================================================================

void Solution::rotate_extra_array(std::vector<int>& nums, int k) {
    int n = static_cast<int>(nums.size());
    if (n == 0) return;
    
    k = k % n;
    if (k == 0) return;
    
    // 创建临时数组
    std::vector<int> temp(n);
    
    // 计算新位置: 原位置 i -> 新位置 (i + k) % n
    for (int i = 0; i < n; ++i) {
        temp[(i + k) % n] = nums[i];
    }
    
    // 移动回去
    nums = std::move(temp);
}

// ============================================================================
// 解法3: 环形替换法
// ============================================================================

void Solution::rotate_cycle(std::vector<int>& nums, int k) {
    int n = static_cast<int>(nums.size());
    if (n == 0) return;
    
    k = k % n;
    if (k == 0) return;
    
    int count = 0;  // 已处理的元素数量
    
    for (int start = 0; count < n; ++start) {
        int current = start;
        int prev = nums[start];
        
        do {
            int next = (current + k) % n;
            int temp = nums[next];
            nums[next] = prev;
            prev = temp;
            current = next;
            ++count;
        } while (current != start);
    }
}

// ============================================================================
// 解法4: 使用 std::rotate
// ============================================================================

void Solution::rotate_std(std::vector<int>& nums, int k) {
    int n = static_cast<int>(nums.size());
    if (n == 0) return;
    
    k = k % n;
    if (k == 0) return;
    
    // std::rotate 将 [first, middle) 和 [middle, last) 交换
    // 向右轮转 k 位，等价于从 n-k 位置开始
    std::rotate(nums.begin(), nums.end() - k, nums.end());
}

} // namespace leetcode
