/**
 * @file test.cpp
 * @brief LeetCode 11 测试用例
 */

#include "solution.h"
#include <iostream>
#include <vector>
#include <cassert>

void printVector(const std::vector<int>& v) {
    std::cout << "[";
    for (size_t i = 0; i < v.size(); ++i) {
        std::cout << v[i];
        if (i < v.size() - 1) std::cout << ",";
    }
    std::cout << "]";
}

void runTest(const std::vector<int>& height, int expected, const char* description) {
    Solution sol;
    std::vector<int> h = height;  // 复制，因为maxArea接受非const引用
    int result = sol.maxArea(h);
    
    std::cout << "测试: " << description << "\n";
    std::cout << "  输入: ";
    printVector(height);
    std::cout << "\n";
    std::cout << "  预期: " << expected << "\n";
    std::cout << "  结果: " << result << "\n";
    
    if (result == expected) {
        std::cout << "  ✅ 通过\n\n";
    } else {
        std::cout << "  ❌ 失败\n\n";
    }
}

void visualizeExample() {
    std::cout << "═══════════════════════════════════════════════════════════════\n";
    std::cout << "图解示例\n";
    std::cout << "═══════════════════════════════════════════════════════════════\n\n";
    
    std::cout << "height = [1,8,6,2,5,4,8,3,7]\n\n";
    std::cout << "     8 |    █        █         █\n";
    std::cout << "     7 |    █        █         █────█\n";
    std::cout << "     6 |    █  █     █         █    █\n";
    std::cout << "     5 |    █  █     █  █      █    █\n";
    std::cout << "     4 |    █  █     █  █  █   █    █\n";
    std::cout << "     3 |    █  █     █  █  █   █  █ █\n";
    std::cout << "     2 |    █  █  █  █  █  █   █  █ █\n";
    std::cout << "     1 |█   █  █  █  █  █  █   █  █ █\n";
    std::cout << "       └─────────────────────────────\n";
    std::cout << "        0  1  2  3  4  5  6  7  8\n\n";
    
    std::cout << "选择索引1和8的两条线:\n";
    std::cout << "  高度 = min(8, 7) = 7\n";
    std::cout << "  宽度 = 8 - 1 = 7\n";
    std::cout << "  面积 = 7 × 7 = 49\n\n";
}

static void explainAlgorithm() {
    std::cout << "═══════════════════════════════════════════════════════════════\n";
    std::cout << "算法详解\n";
    std::cout << "═══════════════════════════════════════════════════════════════\n\n";
    
    std::cout << "双指针贪心策略:\n\n";
    
    std::cout << "1. 初始化:\n";
    std::cout << "   left = 0, right = n-1\n\n";
    
    std::cout << "2. 计算当前面积:\n";
    std::cout << "   面积 = min(height[left], height[right]) × (right - left)\n\n";
    
    std::cout << "3. 移动策略:\n";
    std::cout << "   if (height[left] < height[right]):\n";
    std::cout << "       left++  // 移动较矮的边\n";
    std::cout << "   else:\n";
    std::cout << "       right--\n\n";
    
    std::cout << "4. 为什么这样做是正确的？\n\n";
    std::cout << "   假设 height[left] < height[right]:\n";
    std::cout << "   - 如果移动 right (较高的边):\n";
    std::cout << "     - 宽度减小 (right - left 变小)\n";
    std::cout << "     - 高度最大是 height[left] (已被限制)\n";
    std::cout << "     - 面积不可能增加\n\n";
    std::cout << "   - 如果移动 left (较矮的边):\n";
    std::cout << "     - 宽度减小\n";
    std::cout << "     - 但高度可能增加 (新的 height[left] 可能更大)\n";
    std::cout << "     - 面积可能增加\n\n";
    
    std::cout << "5. 复杂度:\n";
    std::cout << "   - 时间: O(n) - 每个元素最多访问一次\n";
    std::cout << "   - 空间: O(1) - 只使用常数变量\n\n";
}

void run_leetcode11_tests() {
    std::cout << "═══════════════════════════════════════════════════════════════\n";
    std::cout << "LeetCode 11: 盛最多水的容器\n";
    std::cout << "═══════════════════════════════════════════════════════════════\n\n";
    
    visualizeExample();
    explainAlgorithm();
    
    std::cout << "───────────────────────────────────────────────────────────────\n";
    std::cout << "测试用例\n";
    std::cout << "───────────────────────────────────────────────────────────────\n\n";
    
    // 测试用例1: 示例
    runTest({1, 8, 6, 2, 5, 4, 8, 3, 7}, 49, "示例用例");
    
    // 测试用例2: 简单情况
    runTest({1, 1}, 1, "两个元素");
    
    // 测试用例3: 递增序列
    runTest({1, 2, 3, 4, 5}, 6, "递增序列");
    
    // 测试用例4: 递减序列
    runTest({5, 4, 3, 2, 1}, 6, "递减序列");
    
    // 测试用例5: 相同高度
    runTest({4, 4, 4, 4}, 12, "相同高度");
    
    // 测试用例6: 较大数组
    runTest({2, 3, 4, 5, 18, 17, 6}, 17, "复杂情况");
    
    // 测试用例7: 包含0
    runTest({0, 2, 0, 3, 0}, 4, "包含0的高度");
    
    std::cout << "═══════════════════════════════════════════════════════════════\n";
    std::cout << "所有测试完成！\n";
    std::cout << "═══════════════════════════════════════════════════════════════\n\n";
}
