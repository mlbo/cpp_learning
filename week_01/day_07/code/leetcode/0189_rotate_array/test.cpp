/**
 * @file test.cpp
 * @brief LeetCode 189: 轮转数组 - 测试代码
 */

#include "solution.cpp"
#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>

using namespace leetcode;

// 打印数组
void print_array(const std::vector<int>& nums) {
    std::cout << "[";
    for (std::size_t i = 0; i < nums.size(); ++i) {
        std::cout << nums[i];
        if (i < nums.size() - 1) std::cout << ", ";
    }
    std::cout << "]";
}

// 测试用例结构
struct TestCase {
    std::vector<int> nums;
    int k;
    std::vector<int> expected;
    std::string description;
};

// 验证数组是否相等
bool arrays_equal(const std::vector<int>& a, const std::vector<int>& b) {
    return a == b;
}

// 运行测试
void run_test(const TestCase& tc, 
              void (*method)(std::vector<int>&, int),
              const std::string& method_name) {
    
    std::vector<int> nums = tc.nums;  // 复制
    method(nums, tc.k);
    
    bool passed = arrays_equal(nums, tc.expected);
    
    std::cout << "    [" << std::setw(12) << method_name << "] ";
    if (passed) {
        std::cout << "✅ PASS";
    } else {
        std::cout << "❌ FAIL";
        std::cout << " - expected: ";
        print_array(tc.expected);
        std::cout << ", got: ";
        print_array(nums);
    }
    std::cout << "\n";
}

// 可视化轮转过程
void visualize_rotation(const std::vector<int>& original, int k) {
    int n = static_cast<int>(original.size());
    std::cout << "\n    原始数组: ";
    print_array(original);
    std::cout << ", k = " << k << "\n";
    
    std::cout << "\n    翻转过程:\n";
    std::cout << "    1. 翻转整个数组:\n       ";
    
    std::vector<int> temp = original;
    std::reverse(temp.begin(), temp.end());
    print_array(temp);
    std::cout << "\n";
    
    k = k % n;
    
    std::cout << "    2. 翻转前 " << k << " 个元素:\n       ";
    std::reverse(temp.begin(), temp.begin() + k);
    print_array(temp);
    std::cout << "\n";
    
    std::cout << "    3. 翻转剩余元素:\n       ";
    std::reverse(temp.begin() + k, temp.end());
    print_array(temp);
    std::cout << "\n";
}

// 性能测试
void performance_test() {
    std::cout << "\n  【性能测试】\n";
    
    // 创建大数组
    std::vector<int> large_array(1000000);
    for (int i = 0; i < 1000000; ++i) {
        large_array[i] = i;
    }
    int k = 123456;
    
    std::cout << "  数组大小: " << large_array.size() << ", k = " << k << "\n\n";
    
    std::vector<int> temp;
    
    // 测试翻转法
    temp = large_array;
    auto start = std::chrono::high_resolution_clock::now();
    Solution::rotate_reverse(temp, k);
    auto end = std::chrono::high_resolution_clock::now();
    auto reverse_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // 测试额外数组法
    temp = large_array;
    start = std::chrono::high_resolution_clock::now();
    Solution::rotate_extra_array(temp, k);
    end = std::chrono::high_resolution_clock::now();
    auto extra_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // 测试环形替换法
    temp = large_array;
    start = std::chrono::high_resolution_clock::now();
    Solution::rotate_cycle(temp, k);
    end = std::chrono::high_resolution_clock::now();
    auto cycle_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // 测试 std::rotate
    temp = large_array;
    start = std::chrono::high_resolution_clock::now();
    Solution::rotate_std(temp, k);
    end = std::chrono::high_resolution_clock::now();
    auto std_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "    翻转法:     " << std::setw(6) << reverse_time.count() << " μs (推荐)\n";
    std::cout << "    额外数组:   " << std::setw(6) << extra_time.count() << " μs\n";
    std::cout << "    环形替换:   " << std::setw(6) << cycle_time.count() << " μs\n";
    std::cout << "    std::rotate:" << std::setw(6) << std_time.count() << " μs\n";
}

int main() {
    std::cout << R"(
╔══════════════════════════════════════════════════════════════╗
║            LeetCode 189: 轮转数组 (Rotate Array)             ║
╚══════════════════════════════════════════════════════════════╝
)" << std::endl;

    // 测试用例
    std::vector<TestCase> test_cases = {
        {{1, 2, 3, 4, 5, 6, 7}, 3, {5, 6, 7, 1, 2, 3, 4}, "标准用例"},
        {{-1, -100, 3, 99}, 2, {3, 99, -1, -100}, "负数"},
        {{1, 2, 3, 4, 5, 6}, 6, {1, 2, 3, 4, 5, 6}, "k等于n"},
        {{1, 2, 3, 4, 5, 6}, 12, {1, 2, 3, 4, 5, 6}, "k是n的倍数"},
        {{1}, 1, {1}, "单元素"},
        {{1, 2}, 1, {2, 1}, "两元素"},
        {{1, 2, 3}, 0, {1, 2, 3}, "k=0"},
        {{1, 2, 3, 4, 5}, 2, {4, 5, 1, 2, 3}, "奇数长度"},
        {{}, 1, {}, "空数组"},
    };
    
    std::cout << "【测试用例】\n";
    std::cout << "-------------------------------------------\n";
    
    for (const auto& tc : test_cases) {
        std::cout << "\n  测试: " << tc.description << "\n";
        std::cout << "    输入: ";
        print_array(tc.nums);
        std::cout << ", k = " << tc.k << "\n";
        
        run_test(tc, Solution::rotate_reverse, "翻转法");
        run_test(tc, Solution::rotate_extra_array, "额外数组");
        run_test(tc, Solution::rotate_cycle, "环形替换");
        run_test(tc, Solution::rotate_std, "std::rotate");
    }
    
    // 可视化演示
    std::cout << "\n-------------------------------------------\n";
    std::cout << "【翻转法可视化】\n";
    visualize_rotation({1, 2, 3, 4, 5, 6, 7}, 3);
    
    // 性能测试
    std::cout << "\n-------------------------------------------\n";
    performance_test();
    
    std::cout << "\n-------------------------------------------\n";
    std::cout << "✅ 所有测试完成!\n";
    
    return 0;
}
