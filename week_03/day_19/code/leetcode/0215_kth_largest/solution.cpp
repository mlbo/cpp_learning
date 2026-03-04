/**
 * LeetCode 215. 数组中的第K个最大元素
 * 
 * 给定整数数组 nums 和整数 k，请返回数组中第 k 个最大的元素。
 */

#include "solution.h"
#include <iostream>
#include <queue>
#include <algorithm>

// 方法1：小顶堆
int findKthLargestHeap(std::vector<int>& nums, int k) {
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;
    
    for (int num : nums) {
        minHeap.push(num);
        if (minHeap.size() > k) {
            minHeap.pop();  // 弹出最小的
        }
    }
    
    return minHeap.top();
}

// 方法2：快速选择
int quickSelect(std::vector<int>& nums, int left, int right, int k) {
    if (left == right) return nums[left];
    
    int pivot = nums[right];
    int i = left;
    
    // 分区：大的放左边
    for (int j = left; j < right; ++j) {
        if (nums[j] > pivot) {
            std::swap(nums[i], nums[j]);
            i++;
        }
    }
    std::swap(nums[i], nums[right]);
    
    // i是pivot的最终位置
    if (i == k - 1) {
        return nums[i];  // 找到第k大
    } else if (i < k - 1) {
        return quickSelect(nums, i + 1, right, k);
    } else {
        return quickSelect(nums, left, i - 1, k);
    }
}

int Solution::findKthLargest(std::vector<int>& nums, int k) {
    return findKthLargestHeap(nums, k);
}

void testFindKthLargest() {
    Solution sol;
    
    std::cout << "LeetCode 215. 第K个最大元素 测试结果：" << std::endl;
    
    // 测试用例1
    std::vector<int> nums1 = {3, 2, 1, 5, 6, 4};
    int result1 = sol.findKthLargest(nums1, 2);
    std::cout << "  输入: [3,2,1,5,6,4], k = 2" << std::endl;
    std::cout << "  输出: " << result1 << " (期望: 5)" << std::endl;
    
    // 测试用例2
    std::vector<int> nums2 = {3, 2, 3, 1, 2, 4, 5, 5, 6};
    int result2 = sol.findKthLargest(nums2, 4);
    std::cout << "\n  输入: [3,2,3,1,2,4,5,5,6], k = 4" << std::endl;
    std::cout << "  输出: " << result2 << " (期望: 4)" << std::endl;
    
    // 方法对比
    std::cout << "\n  方法对比：" << std::endl;
    std::cout << "    1. 排序: O(n log n)" << std::endl;
    std::cout << "    2. 小顶堆: O(n log k)" << std::endl;
    std::cout << "    3. 快速选择: 平均O(n), 最坏O(n²)" << std::endl;
    
    // 图解
    std::cout << "\n  小顶堆图解（找第2大）：" << std::endl;
    std::cout << "    处理3: 堆=[3]" << std::endl;
    std::cout << "    处理2: 堆=[2,3]" << std::endl;
    std::cout << "    处理1: 堆满，1<2，跳过" << std::endl;
    std::cout << "    处理5: 5>2，弹出2，压入5，堆=[3,5]" << std::endl;
    std::cout << "    处理6: 6>3，弹出3，压入6，堆=[5,6]" << std::endl;
    std::cout << "    处理4: 4<5，跳过" << std::endl;
    std::cout << "    结果: 堆顶=5" << std::endl;
}
