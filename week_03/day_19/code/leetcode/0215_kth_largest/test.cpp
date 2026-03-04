/**
 * @file test.cpp
 * @brief LeetCode 215. 数组中的第K个最大元素 - 测试
 */

#include "leetcode/0215_kth_largest/solution.h"
#include <iostream>
#include <vector>
#include <cassert>

void testFindKthLargest() {
    Solution sol;

    std::cout << "========== LeetCode 215 测试 ==========\n" << std::endl;

    // 测试用例1
    {
        std::cout << "测试用例 1：" << std::endl;
        std::vector<int> nums = {3, 2, 1, 5, 6, 4};
        int k = 2;
        int expected = 5;

        std::cout << "  输入: nums = [3,2,1,5,6,4], k = 2" << std::endl;

        // 方法1：小顶堆
        std::vector<int> copy1 = nums;
        int result1 = sol.findKthLargest_heap(copy1, k);
        std::cout << "  小顶堆法: " << result1 << " (期望: " << expected << ")" << std::endl;
        assert(result1 == expected);

        // 方法2：排序
        std::vector<int> copy2 = nums;
        int result2 = sol.findKthLargest_sort(copy2, k);
        std::cout << "  排序法: " << result2 << " (期望: " << expected << ")" << std::endl;
        assert(result2 == expected);

        // 方法3：快速选择
        std::vector<int> copy3 = nums;
        int result3 = sol.findKthLargest_quickselect(copy3, k);
        std::cout << "  快速选择法: " << result3 << " (期望: " << expected << ")" << std::endl;
        assert(result3 == expected);

        std::cout << "  ✓ 通过\n" << std::endl;
    }

    // 测试用例2
    {
        std::cout << "测试用例 2：" << std::endl;
        std::vector<int> nums = {3, 2, 3, 1, 2, 4, 5, 5, 6};
        int k = 4;
        int expected = 4;

        std::cout << "  输入: nums = [3,2,3,1,2,4,5,5,6], k = 4" << std::endl;

        std::vector<int> copy1 = nums;
        int result1 = sol.findKthLargest_heap(copy1, k);
        std::cout << "  小顶堆法: " << result1 << " (期望: " << expected << ")" << std::endl;
        assert(result1 == expected);

        std::vector<int> copy2 = nums;
        int result2 = sol.findKthLargest_sort(copy2, k);
        std::cout << "  排序法: " << result2 << " (期望: " << expected << ")" << std::endl;
        assert(result2 == expected);

        std::vector<int> copy3 = nums;
        int result3 = sol.findKthLargest_quickselect(copy3, k);
        std::cout << "  快速选择法: " << result3 << " (期望: " << expected << ")" << std::endl;
        assert(result3 == expected);

        std::cout << "  ✓ 通过\n" << std::endl;
    }

    // 测试用例3：边界情况 k=1
    {
        std::cout << "测试用例 3 (k=1，找最大值)：" << std::endl;
        std::vector<int> nums = {1};
        int k = 1;
        int expected = 1;

        std::cout << "  输入: nums = [1], k = 1" << std::endl;

        std::vector<int> copy1 = nums;
        int result1 = sol.findKthLargest_heap(copy1, k);
        std::cout << "  小顶堆法: " << result1 << " (期望: " << expected << ")" << std::endl;
        assert(result1 == expected);

        std::vector<int> copy3 = nums;
        int result3 = sol.findKthLargest_quickselect(copy3, k);
        std::cout << "  快速选择法: " << result3 << " (期望: " << expected << ")" << std::endl;
        assert(result3 == expected);

        std::cout << "  ✓ 通过\n" << std::endl;
    }

    // 测试用例4：有重复元素
    {
        std::cout << "测试用例 4 (有重复元素)：" << std::endl;
        std::vector<int> nums = {2, 2, 2, 2, 2};
        int k = 3;
        int expected = 2;

        std::cout << "  输入: nums = [2,2,2,2,2], k = 3" << std::endl;

        std::vector<int> copy1 = nums;
        int result1 = sol.findKthLargest_heap(copy1, k);
        std::cout << "  小顶堆法: " << result1 << " (期望: " << expected << ")" << std::endl;
        assert(result1 == expected);

        std::vector<int> copy3 = nums;
        int result3 = sol.findKthLargest_quickselect(copy3, k);
        std::cout << "  快速选择法: " << result3 << " (期望: " << expected << ")" << std::endl;
        assert(result3 == expected);

        std::cout << "  ✓ 通过\n" << std::endl;
    }

    std::cout << "========== 所有测试通过！ ==========\n" << std::endl;
}

int main() {
    testFindKthLargest();
    return 0;
}
