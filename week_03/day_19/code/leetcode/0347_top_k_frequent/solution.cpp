/**
 * LeetCode 347. 前K个高频元素
 * 
 * 给你一个整数数组 nums 和一个整数 k，请你返回其中出现频率前 k 高的元素。
 */

#include "solution.h"
#include <iostream>
#include <queue>
#include <unordered_map>
#include <utility>

std::vector<int> Solution::topKFrequent(std::vector<int>& nums, int k) {
    // 1. 统计频率
    std::unordered_map<int, int> freq;
    for (int num : nums) {
        freq[num]++;
    }
    
    // 2. 小顶堆，按频率排序
    auto cmp = [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        return a.second > b.second;  // 频率小的排后面
    };
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, decltype(cmp)> minHeap(cmp);
    
    // 3. 维护前K个高频元素
    for (auto& [num, count] : freq) {
        minHeap.push({num, count});
        if (minHeap.size() > k) {
            minHeap.pop();
        }
    }
    
    // 4. 收集结果
    std::vector<int> result;
    while (!minHeap.empty()) {
        result.push_back(minHeap.top().first);
        minHeap.pop();
    }
    
    return result;
}

void testTopKFrequent() {
    Solution sol;
    
    std::cout << "LeetCode 347. 前K个高频元素 测试结果：" << std::endl;
    
    // 测试用例1
    std::vector<int> nums1 = {1, 1, 1, 2, 2, 3};
    auto result1 = sol.topKFrequent(nums1, 2);
    std::cout << "  输入: [1,1,1,2,2,3], k = 2" << std::endl;
    std::cout << "  输出: [";
    for (int i = 0; i < result1.size(); ++i) {
        std::cout << result1[i];
        if (i < result1.size() - 1) std::cout << ", ";
    }
    std::cout << "] (期望: [1, 2])" << std::endl;
    
    // 测试用例2
    std::vector<int> nums2 = {1};
    auto result2 = sol.topKFrequent(nums2, 1);
    std::cout << "\n  输入: [1], k = 1" << std::endl;
    std::cout << "  输出: [";
    for (int i = 0; i < result2.size(); ++i) {
        std::cout << result2[i];
        if (i < result2.size() - 1) std::cout << ", ";
    }
    std::cout << "] (期望: [1])" << std::endl;
    
    // 解题思路
    std::cout << "\n  解题思路：" << std::endl;
    std::cout << "    1. 用哈希表统计每个数字出现的频率" << std::endl;
    std::cout << "    2. 用小顶堆维护前K个高频元素" << std::endl;
    std::cout << "    3. 堆中存储 (数字, 频率) 对" << std::endl;
    std::cout << "    4. 堆的大小超过K时，弹出频率最小的" << std::endl;
    
    // 图解
    std::cout << "\n  图解（找前2高频）：" << std::endl;
    std::cout << "    频率统计: 1->3次, 2->2次, 3->1次" << std::endl;
    std::cout << "    堆变化:" << std::endl;
    std::cout << "      加入(1,3): 堆=[(1,3)]" << std::endl;
    std::cout << "      加入(2,2): 堆=[(2,2), (1,3)]" << std::endl;
    std::cout << "      加入(3,1): 堆满，弹出(3,1)，堆=[(2,2), (1,3)]" << std::endl;
    std::cout << "    结果: [1, 2]" << std::endl;
}
