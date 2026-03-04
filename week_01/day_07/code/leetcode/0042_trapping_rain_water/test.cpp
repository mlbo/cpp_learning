/**
 * @file test.cpp
 * @brief LeetCode 42: 接雨水 - 测试代码
 */

#include "solution.cpp"  // 包含实现
#include <iostream>
#include <vector>
#include <chrono>
#include <cassert>
#include <iomanip>

using namespace leetcode;

// 测试用例结构
struct TestCase {
    std::vector<int> height;
    int expected;
    std::string description;
};

// 运行单个测试
void run_test(Solution& sol, const TestCase& tc, 
              int (*method)(std::vector<int>&),
              const std::string& method_name) {
    
    std::vector<int> height = tc.height;  // 复制，避免修改
    int result = method(height);
    
    bool passed = (result == tc.expected);
    
    std::cout << "    [" << method_name << "] ";
    if (passed) {
        std::cout << "✅ ";
    } else {
        std::cout << "❌ ";
    }
    std::cout << "expected: " << tc.expected 
              << ", got: " << result;
    
    if (!tc.description.empty()) {
        std::cout << " (" << tc.description << ")";
    }
    std::cout << "\n";
}

// 打印高度图
void print_height_map(const std::vector<int>& height) {
    if (height.empty()) return;
    
    int max_h = *std::max_element(height.begin(), height.end());
    
    std::cout << "\n    高度图:\n";
    for (int h = max_h; h > 0; --h) {
        std::cout << "    ";
        for (int val : height) {
            if (val >= h) {
                std::cout << "■ ";
            } else {
                std::cout << "  ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "    ";
    for (int val : height) {
        std::cout << val << " ";
    }
    std::cout << "\n";
}

// 性能测试
void performance_test(Solution& sol, const std::vector<int>& large_input) {
    std::cout << "\n  【性能测试】\n";
    std::cout << "  输入规模: " << large_input.size() << " 个元素\n\n";
    
    std::vector<int> input;
    
    // 测试双指针
    input = large_input;
    auto start = std::chrono::high_resolution_clock::now();
    sol.trap_two_pointers(input);
    auto end = std::chrono::high_resolution_clock::now();
    auto two_ptr_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // 测试动态规划
    input = large_input;
    start = std::chrono::high_resolution_clock::now();
    sol.trap_dp(input);
    end = std::chrono::high_resolution_clock::now();
    auto dp_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // 测试单调栈
    input = large_input;
    start = std::chrono::high_resolution_clock::now();
    sol.trap_stack(input);
    end = std::chrono::high_resolution_clock::now();
    auto stack_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "    双指针法:   " << std::setw(6) << two_ptr_time.count() << " μs (推荐)\n";
    std::cout << "    动态规划:   " << std::setw(6) << dp_time.count() << " μs\n";
    std::cout << "    单调栈:     " << std::setw(6) << stack_time.count() << " μs\n";
}

int main() {
    std::cout << R"(
╔══════════════════════════════════════════════════════════════╗
║            LeetCode 42: 接雨水 (Trapping Rain Water)         ║
╚══════════════════════════════════════════════════════════════╝
)" << std::endl;

    Solution sol;
    
    // 测试用例
    std::vector<TestCase> test_cases = {
        {{0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1}, 6, "标准测试用例"},
        {{4, 2, 0, 3, 2, 5}, 9, "中间凹槽"},
        {{0, 0, 0, 0}, 0, "全零"},
        {{1, 2, 3, 4, 5}, 0, "递增序列"},
        {{5, 4, 3, 2, 1}, 0, "递减序列"},
        {{5, 5, 5, 5}, 0, "所有柱子等高"},
        {{}, 0, "空数组"},
        {{1}, 0, "单个元素"},
        {{2, 0, 2}, 2, "简单凹槽"},
        {{5, 4, 1, 2}, 1, "复杂凹槽"},
    };
    
    std::cout << "【测试用例】\n";
    std::cout << "-------------------------------------------\n";
    
    for (const auto& tc : test_cases) {
        std::cout << "\n  测试: " << tc.description << "\n";
        
        if (tc.height.size() <= 15) {
            print_height_map(tc.height);
        }
        
        run_test(sol, tc, &Solution::trap_two_pointers, "双指针");
        run_test(sol, tc, &Solution::trap_dp, "动态规划");
        run_test(sol, tc, &Solution::trap_stack, "单调栈");
        
        // 验证结果一致
        std::vector<int> h1 = tc.height, h2 = tc.height, h3 = tc.height;
        int r1 = sol.trap_two_pointers(h1);
        int r2 = sol.trap_dp(h2);
        int r3 = sol.trap_stack(h3);
        
        if (r1 == r2 && r2 == r3 && r1 == tc.expected) {
            std::cout << "    所有方法结果一致 ✅\n";
        } else {
            std::cout << "    ⚠️ 方法结果不一致!\n";
        }
    }
    
    // 性能测试
    std::cout << "\n-------------------------------------------\n";
    std::vector<int> large_input(10000);
    for (int i = 0; i < 10000; ++i) {
        large_input[i] = i % 100;
    }
    performance_test(sol, large_input);
    
    std::cout << "\n-------------------------------------------\n";
    std::cout << "✅ 所有测试完成!\n";
    
    return 0;
}
