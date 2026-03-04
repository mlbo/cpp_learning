/**
 * @file solution.cpp
 * @brief LeetCode 209: 长度最小的子数组 - 实现文件
 */

#include "solution.h"
#include <algorithm>

/**
 * 方法一：滑动窗口
 * 
 * 核心思想：
 * 1. 使用双指针left和right维护一个窗口
 * 2. right指针向右扩展窗口，增加和
 * 3. 当sum >= target时，left指针向右收缩窗口，尝试找到更小的满足条件的子数组
 * 4. 窗口大小即为子数组长度
 * 
 * 滑动窗口正确性保证：
 * - 数组元素均为正数，所以sum随right增大而增大，随left增大而减小
 * - 这保证了单调性，使得滑动窗口算法正确
 */
int Solution::minSubArrayLen(int target, vector<int>& nums) {
    int n = nums.size();
    int left = 0;
    int sum = 0;
    int minLen = INT_MAX;
    
    // 遍历右边界
    for (int right = 0; right < n; ++right) {
        sum += nums[right];  // 扩展窗口
        
        // 当窗口满足条件时，尝试收缩
        while (sum >= target) {
            minLen = min(minLen, right - left + 1);  // 更新最小长度
            sum -= nums[left++];  // 收缩左边界
        }
    }
    
    return minLen == INT_MAX ? 0 : minLen;
}

/**
 * 方法二：前缀和 + 二分查找
 * 
 * 核心思想：
 * 1. 计算前缀和数组prefix，其中prefix[i]表示nums[0..i-1]的和
 * 2. 对于每个起始位置i，二分查找最小的结束位置j，使得prefix[j] - prefix[i] >= target
 * 3. prefix数组是单调递增的（因为元素都是正数），所以可以二分查找
 * 
 * 适用场景：
 * - 当需要多次查询不同target时效率更高
 * - 可以处理负数的情况（但需要更复杂的处理）
 */
int Solution::minSubArrayLenBinarySearch(int target, vector<int>& nums) {
    int n = nums.size();
    int minLen = INT_MAX;
    
    // 计算前缀和
    vector<int> prefix(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        prefix[i + 1] = prefix[i] + nums[i];
    }
    
    // 对每个起始位置，二分查找结束位置
    for (int i = 0; i < n; ++i) {
        int need = prefix[i] + target;  // 需要找到prefix[j] >= need
        
        // 二分查找
        int left = i + 1, right = n + 1;
        while (left < right) {
            int mid = left + (right - left) / 2;
            if (prefix[mid] < need) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }
        
        // 检查是否找到有效的结束位置
        if (left <= n) {
            minLen = min(minLen, left - i);
        }
    }
    
    return minLen == INT_MAX ? 0 : minLen;
}

/**
 * 方法三：暴力法
 * 
 * 枚举所有可能的子数组，计算其和
 * 用于验证滑动窗口算法的正确性
 */
int Solution::minSubArrayLenBruteForce(int target, vector<int>& nums) {
    int n = nums.size();
    int minLen = INT_MAX;
    
    for (int i = 0; i < n; ++i) {
        int sum = 0;
        for (int j = i; j < n; ++j) {
            sum += nums[j];
            if (sum >= target) {
                minLen = min(minLen, j - i + 1);
                break;  // 找到最短的以i开头的子数组
            }
        }
    }
    
    return minLen == INT_MAX ? 0 : minLen;
}
