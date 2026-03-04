/**
 * @file test.cpp
 * @brief LeetCode 283. 移动零 - 测试用例
 */

#include "solution.h"
#include <iostream>
#include <vector>
#include <string>

namespace leetcode_0283 {

void print_vector(const std::vector<int>& v) {
    std::cout << "[";
    for (size_t i = 0; i < v.size(); ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << v[i];
    }
    std::cout << "]";
}

std::string check_result(const std::vector<int>& result,
                         const std::vector<int>& expected) {
    if (result == expected) {
        return "✓ 通过";
    }
    return "✗ 失败";
}

void run_test(const std::string& name,
              std::vector<int> nums,
              const std::vector<int>& expected) {

    std::cout << "测试 " << name << ":\n";
    std::cout << "  输入:   ";
    print_vector(nums);
    std::cout << "\n";

    Solution sol;
    sol.moveZeroes(nums);

    std::cout << "  输出:   ";
    print_vector(nums);
    std::cout << "\n";

    std::cout << "  期望:   ";
    print_vector(expected);
    std::cout << "\n";

    std::cout << "  结果:   " << check_result(nums, expected) << "\n\n";
}

void compare_methods(const std::vector<int>& original) {
    std::cout << "方法对比:\n";

    std::vector<int> nums1 = original;
    std::vector<int> nums2 = original;
    std::vector<int> nums3 = original;

    Solution sol;

    std::cout << "  原始数组: ";
    print_vector(original);
    std::cout << "\n";

    sol.moveZeroes(nums1);
    std::cout << "  双指针交换: ";
    print_vector(nums1);
    std::cout << "\n";

    sol.moveZeroes_two_pass(nums2);
    std::cout << "  两次遍历:   ";
    print_vector(nums2);
    std::cout << "\n";

    sol.moveZeroes_optimized(nums3);
    std::cout << "  优化版:     ";
    print_vector(nums3);
    std::cout << "\n";
}

void run_tests() {
    std::cout << "╔══════════════════════════════════════════════════════════╗\n";
    std::cout << "║          LeetCode 283: 移动零 - 测试                     ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════╝\n\n";

    // 测试 1：基本示例
    run_test("基本示例",
             {0, 1, 0, 3, 12},
             {1, 3, 12, 0, 0});

    // 测试 2：全是零
    run_test("全是零",
             {0, 0, 0, 0},
             {0, 0, 0, 0});

    // 测试 3：没有零
    run_test("没有零",
             {1, 2, 3, 4},
             {1, 2, 3, 4});

    // 测试 4：全是非零
    run_test("全是非零",
             {1, 2, 3},
             {1, 2, 3});

    // 测试 5：零在开头
    run_test("零在开头",
             {0, 0, 1, 2, 3},
             {1, 2, 3, 0, 0});

    // 测试 6：零在末尾
    run_test("零在末尾",
             {1, 2, 3, 0, 0},
             {1, 2, 3, 0, 0});

    // 测试 7：单个元素 - 零
    run_test("单个元素（零）",
             {0},
             {0});

    // 测试 8：单个元素 - 非零
    run_test("单个元素（非零）",
             {5},
             {5});

    // 测试 9：交替零和非零
    run_test("交替零和非零",
             {0, 1, 0, 2, 0, 3},
             {1, 2, 3, 0, 0, 0});

    // 测试 10：负数
    run_test("包含负数",
             {0, -1, 0, -3, 12},
             {-1, -3, 12, 0, 0});

    // 方法对比演示
    std::cout << "══════════════════════════════════════════════════════════\n";
    std::cout << "方法对比演示:\n";
    std::cout << "══════════════════════════════════════════════════════════\n\n";
    compare_methods({0, 1, 0, 3, 12});

    // 复杂度分析
    std::cout << "══════════════════════════════════════════════════════════\n";
    std::cout << "算法复杂度分析:\n";
    std::cout << "══════════════════════════════════════════════════════════\n";
    std::cout << "双指针交换法:\n";
    std::cout << "  时间复杂度: O(n) - 一次遍历\n";
    std::cout << "  空间复杂度: O(1) - 原地操作\n";
    std::cout << "\n";
    std::cout << "两次遍历法:\n";
    std::cout << "  时间复杂度: O(n) - 两次遍历\n";
    std::cout << "  空间复杂度: O(1) - 原地操作\n";
    std::cout << "\n";
    std::cout << "优化版:\n";
    std::cout << "  时间复杂度: O(n) - 一次遍历\n";
    std::cout << "  空间复杂度: O(1) - 原地操作\n";
    std::cout << "  特点: 减少了不必要的自交换\n";
    std::cout << "══════════════════════════════════════════════════════════\n";
}

} // namespace leetcode_0283
