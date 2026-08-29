/**
 * 单调栈算法演示
 * 找下一个更大/更小元素的经典算法
 */

#include "monotonic_stack_demo.h"
#include <iostream>
#include <stack>
#include <vector>
#include <unordered_map>

// 找每个元素右边第一个更大的元素
std::vector<int> findNextGreater(const std::vector<int>& nums) {
    std::vector<int> result(nums.size(), -1);  // 默认-1表示没有
    std::stack<std::size_t> stk;  // 存储下标
    
    for (std::size_t i = 0; i < nums.size(); ++i) {
        // 当前元素比栈顶大，说明找到了栈顶元素的答案
        while (!stk.empty() && nums[i] > nums[stk.top()]) {
            result[stk.top()] = nums[i];
            stk.pop();
        }
        stk.push(i);
    }
    
    return result;
}

// 找每个元素右边第一个更小的元素
std::vector<int> findNextSmaller(const std::vector<int>& nums) {
    std::vector<int> result(nums.size(), -1);
    std::stack<std::size_t> stk;
    
    for (std::size_t i = 0; i < nums.size(); ++i) {
        while (!stk.empty() && nums[i] < nums[stk.top()]) {
            result[stk.top()] = nums[i];
            stk.pop();
        }
        stk.push(i);
    }
    
    return result;
}

// 找每个元素左边第一个更大的元素
std::vector<int> findPrevGreater(const std::vector<int>& nums) {
    std::vector<int> result(nums.size(), -1);
    std::stack<std::size_t> stk;
    
    for (std::size_t i = nums.size(); i-- > 0;) {
        while (!stk.empty() && nums[i] > nums[stk.top()]) {
            result[stk.top()] = nums[i];
            stk.pop();
        }
        stk.push(i);
    }
    
    return result;
}

void monotonicStackDemo() {
    std::cout << "╔══════════════════════════════════════╗" << std::endl;
    std::cout << "║        单调栈算法 Monotonic Stack     ║" << std::endl;
    std::cout << "╚══════════════════════════════════════╝" << std::endl;
    
    // ========== 1. 找下一个更大元素 ==========
    std::cout << "\n--- 1. 找下一个更大元素 ---" << std::endl;
    
    std::vector<int> nums1 = {2, 1, 2, 4, 3};
    std::cout << "  输入: [2, 1, 2, 4, 3]" << std::endl;
    
    auto result1 = findNextGreater(nums1);
    
    std::cout << "  下一个更大: [";
    for (std::size_t i = 0; i < result1.size(); ++i) {
        std::cout << result1[i];
        if (i + 1 < result1.size()) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
    
    std::cout << "  解释:" << std::endl;
    std::cout << "    2 -> 下一个更大是4" << std::endl;
    std::cout << "    1 -> 下一个更大是2" << std::endl;
    std::cout << "    2 -> 下一个更大是4" << std::endl;
    std::cout << "    4 -> 没有更大的，返回-1" << std::endl;
    std::cout << "    3 -> 没有更大的，返回-1" << std::endl;
    
    // ========== 2. 找下一个更小元素 ==========
    std::cout << "\n--- 2. 找下一个更小元素 ---" << std::endl;
    
    std::vector<int> nums2 = {5, 3, 4, 2, 6};
    std::cout << "  输入: [5, 3, 4, 2, 6]" << std::endl;
    
    auto result2 = findNextSmaller(nums2);
    
    std::cout << "  下一个更小: [";
    for (std::size_t i = 0; i < result2.size(); ++i) {
        std::cout << result2[i];
        if (i + 1 < result2.size()) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
    
    // ========== 3. 单调栈工作过程演示 ==========
    std::cout << "\n--- 3. 单调栈工作过程 ---" << std::endl;
    
    std::vector<int> nums3 = {3, 1, 4, 2};
    std::stack<std::size_t> demoStack;
    
    std::cout << "  处理 [3, 1, 4, 2]，找下一个更大" << std::endl;
    
    for (std::size_t i = 0; i < nums3.size(); ++i) {
        std::cout << "\n  处理nums[" << i << "] = " << nums3[i] << std::endl;
        
        while (!demoStack.empty() && nums3[i] > nums3[demoStack.top()]) {
            const std::size_t top = demoStack.top();
            std::cout << "    " << nums3[top] << " < " << nums3[i] 
                      << "，弹出" << nums3[top] << "，记录答案" << nums3[i] << std::endl;
            demoStack.pop();
        }
        
        demoStack.push(i);
        std::cout << "    入栈 " << nums3[i] << std::endl;
        std::cout << "    栈: [";
        std::stack<std::size_t> temp = demoStack;
        std::vector<int> stackContent;
        while (!temp.empty()) {
            stackContent.push_back(nums3[temp.top()]);
            temp.pop();
        }
        for (auto it = stackContent.rbegin(); it != stackContent.rend(); ++it) {
            if (it != stackContent.rbegin()) std::cout << ", ";
            std::cout << *it;
        }
        std::cout << "]" << std::endl;
    }
    
    // ========== 4. 时间复杂度分析 ==========
    std::cout << "\n--- 4. 时间复杂度分析 ---" << std::endl;
    std::cout << "  暴力解法: O(n²) - 对每个元素遍历后面的所有元素" << std::endl;
    std::cout << "  单调栈: O(n) - 每个元素最多入栈出栈各一次" << std::endl;
    std::cout << "  空间复杂度: O(n) - 栈的最大大小" << std::endl;
    std::cout << "  本例不变量: 栈存尚未找到答案的下标，对应值从栈底到栈顶非递增" << std::endl;
}
