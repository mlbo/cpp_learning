/**
 * @file test.cpp
 * @brief LeetCode 209: 长度最小的子数组 - 测试文件
 */

#include "solution.h"
#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

// 测试用例结构
struct TestCase {
    int target;
    vector<int> nums;
    int expected;
    string description;
};

// 打印数组
void printArray(const vector<int>& arr) {
    cout << "[";
    for (size_t i = 0; i < arr.size(); ++i) {
        cout << arr[i];
        if (i < arr.size() - 1) cout << ", ";
    }
    cout << "]";
}

// 运行测试
void runTests() {
    Solution solution;
    
    // 测试用例
    vector<TestCase> testCases = {
        {7, {2, 3, 1, 2, 4, 3}, 2, "标准用例"},
        {4, {1, 4, 4}, 1, "单个元素满足"},
        {11, {1, 1, 1, 1, 1, 1, 1, 1}, 0, "无法满足"},
        {11, {1, 2, 3, 4, 5}, 3, "恰好等于"},
        {15, {1, 2, 3, 4, 5}, 5, "全部元素"},
        {100, {1, 2, 3}, 0, "目标过大"},
        {1, {1}, 1, "单个元素"},
        {3, {1, 1, 1, 1, 1}, 3, "多个相同元素"},
        {6, {10, 2, 3}, 1, "第一个元素就满足"},
        {5, {2, 3, 1, 1, 1, 1, 1}, 2, "有更优解"}
    };
    
    cout << "╔════════════════════════════════════════════════════════════╗\n";
    cout << "║        LeetCode 209: 长度最小的子数组                       ║\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n\n";
    
    int passed = 0;
    int total = testCases.size();
    
    for (size_t i = 0; i < testCases.size(); ++i) {
        const auto& tc = testCases[i];
        
        cout << "测试用例 " << i + 1 << ": " << tc.description << "\n";
        cout << "  输入: target = " << tc.target << ", nums = ";
        printArray(tc.nums);
        cout << "\n";
        cout << "  期望输出: " << tc.expected << "\n";
        
        // 测试滑动窗口方法
        auto start = chrono::high_resolution_clock::now();
        int result = solution.minSubArrayLen(tc.target, const_cast<vector<int>&>(tc.nums));
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        
        cout << "  实际输出: " << result;
        
        if (result == tc.expected) {
            cout << " ✅ 通过";
            passed++;
        } else {
            cout << " ❌ 失败";
        }
        
        cout << " (耗时: " << duration.count() << " μs)\n";
        
        // 验证其他方法
        int result2 = solution.minSubArrayLenBinarySearch(tc.target, const_cast<vector<int>&>(tc.nums));
        int result3 = solution.minSubArrayLenBruteForce(tc.target, const_cast<vector<int>&>(tc.nums));
        
        cout << "  其他方法: 二分查找=" << result2 << ", 暴力法=" << result3 << "\n";
        cout << "\n";
    }
    
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    cout << "测试结果: " << passed << "/" << total << " 通过\n";
    
    if (passed == total) {
        cout << "🎉 所有测试用例通过！\n";
    }
}

// 滑动窗口过程演示
void demonstrateSlidingWindow() {
    cout << "\n╔════════════════════════════════════════════════════════════╗\n";
    cout << "║              滑动窗口过程演示                               ║\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n\n";
    
    vector<int> nums = {2, 3, 1, 2, 4, 3};
    int target = 7;
    
    cout << "输入: target = " << target << ", nums = ";
    printArray(nums);
    cout << "\n\n";
    
    cout << "滑动窗口执行过程:\n";
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    
    int left = 0, sum = 0, minLen = INT_MAX;
    
    for (int right = 0; right < nums.size(); ++right) {
        sum += nums[right];
        
        // 打印当前窗口
        cout << "right=" << right << ": 窗口 = [";
        for (int i = left; i <= right; ++i) {
            cout << nums[i];
            if (i < right) cout << ", ";
        }
        cout << "], sum = " << sum;
        
        if (sum >= target) {
            cout << " >= " << target << " ✓";
            
            while (sum >= target) {
                int len = right - left + 1;
                if (len < minLen) {
                    minLen = len;
                    cout << " -> 更新最小长度 = " << minLen;
                }
                sum -= nums[left++];
            }
            cout << ", 收缩后 left=" << left;
        }
        
        cout << "\n";
    }
    
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    cout << "最终结果: " << (minLen == INT_MAX ? 0 : minLen) << "\n";
}

// 复杂度分析
void analyzeComplexity() {
    cout << "\n╔════════════════════════════════════════════════════════════╗\n";
    cout << "║                    复杂度分析                               ║\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n\n";
    
    cout << "┌─────────────────┬──────────────┬──────────────┐\n";
    cout << "│     方法        │  时间复杂度  │  空间复杂度  │\n";
    cout << "├─────────────────┼──────────────┼──────────────┤\n";
    cout << "│   滑动窗口      │    O(n)      │    O(1)      │\n";
    cout << "│ 前缀和+二分     │  O(n log n)  │    O(n)      │\n";
    cout << "│    暴力法       │    O(n²)     │    O(1)      │\n";
    cout << "└─────────────────┴──────────────┴──────────────┘\n\n";
    
    cout << "滑动窗口优化原理:\n";
    cout << "  - 每个元素最多被访问两次（right和left各一次）\n";
    cout << "  - 时间复杂度从O(n²)降低到O(n)\n";
    cout << "  - 关键在于数组元素全为正数，保证单调性\n";
}

int main() {
    runTests();
    demonstrateSlidingWindow();
    analyzeComplexity();
    
    return 0;
}
