/**
 * @file test.cpp
 * @brief LeetCode 88. 合并两个有序数组 - 测试用例
 */

#include "solution.h"
#include <iostream>
#include <vector>
#include <cassert>

namespace leetcode_0088 {

void print_vector(const std::vector<int>& v) {
    std::cout << "[";
    for (size_t i = 0; i < v.size(); ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << v[i];
    }
    std::cout << "]";
}

void run_test(const std::string& name,
              std::vector<int> nums1, int m,
              std::vector<int> nums2, int n,
              const std::vector<int>& expected) {

    std::cout << "测试 " << name << ":\n";
    std::cout << "  输入: nums1 = ";
    print_vector(std::vector<int>(nums1.begin(), nums1.begin() + m));
    std::cout << ", m = " << m << ", nums2 = ";
    print_vector(nums2);
    std::cout << ", n = " << n << "\n";

    Solution sol;
    sol.merge(nums1, m, nums2, n);

    std::cout << "  输出: ";
    print_vector(nums1);
    std::cout << "\n";

    std::cout << "  期望: ";
    print_vector(expected);
    std::cout << "\n";

    if (nums1 == expected) {
        std::cout << "  结果: ✓ 通过\n\n";
    } else {
        std::cout << "  结果: ✗ 失败\n\n";
    }
}

void run_tests() {
    std::cout << "╔══════════════════════════════════════════════════════════╗\n";
    std::cout << "║        LeetCode 88: 合并两个有序数组 - 测试              ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════╝\n\n";

    // 测试 1：基本示例
    run_test("基本示例",
             {1, 2, 3, 0, 0, 0}, 3,
             {2, 5, 6}, 3,
             {1, 2, 2, 3, 5, 6});

    // 测试 2：nums2 为空
    run_test("nums2 为空",
             {1, 2, 3}, 3,
             {}, 0,
             {1, 2, 3});

    // 测试 3：nums1 有效部分为空
    run_test("nums1 有效部分为空",
             {0, 0, 0}, 0,
             {1, 2, 3}, 3,
             {1, 2, 3});

    // 测试 4：两个单元素数组
    run_test("两个单元素数组",
             {1, 0}, 1,
             {2}, 1,
             {1, 2});

    // 测试 5：nums2 全部小于 nums1
    run_test("nums2 全部小于 nums1",
             {4, 5, 6, 0, 0, 0}, 3,
             {1, 2, 3}, 3,
             {1, 2, 3, 4, 5, 6});

    // 测试 6：nums2 全部大于 nums1
    run_test("nums2 全部大于 nums1",
             {1, 2, 3, 0, 0, 0}, 3,
             {4, 5, 6}, 3,
             {1, 2, 3, 4, 5, 6});

    // 测试 7：有重复元素
    run_test("有重复元素",
             {1, 1, 3, 0, 0, 0}, 3,
             {1, 2, 2}, 3,
             {1, 1, 1, 2, 2, 3});

    // 测试 8：负数
    run_test("包含负数",
             {-3, -1, 2, 0, 0, 0}, 3,
             {-2, 0, 1}, 3,
             {-3, -2, -1, 0, 1, 2});

    std::cout << "══════════════════════════════════════════════════════════\n";
    std::cout << "算法复杂度分析：\n";
    std::cout << "  时间复杂度: O(m + n) - 每个元素最多被访问一次\n";
    std::cout << "  空间复杂度: O(1) - 原地操作，无需额外空间\n";
    std::cout << "══════════════════════════════════════════════════════════\n";
}

} // namespace leetcode_0088
