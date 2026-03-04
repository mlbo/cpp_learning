/**
 * @file main.cpp
 * @brief Day 5 主程序：constexpr与滑动窗口算法演示
 */

#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <climits>
#include <chrono>

using namespace std;

// ==================== constexpr示例 ====================

// constexpr变量
constexpr int MAX_SIZE = 100;
constexpr double PI = 3.14159265359;

// constexpr函数
constexpr int square(int x) {
    return x * x;
}

constexpr int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

// C++14: constexpr函数可以更复杂
constexpr int sum_to_n(int n) {
    int result = 0;
    for (int i = 1; i <= n; ++i) {
        result += i;
    }
    return result;
}

// constexpr类
class Point {
    int x_, y_;
public:
    constexpr Point(int x, int y) : x_(x), y_(y) {}
    constexpr int x() const { return x_; }
    constexpr int y() const { return y_; }
    constexpr int distance_squared() const { return x_ * x_ + y_ * y_; }
};

// ==================== 滑动窗口模板 ====================

// 滑动窗口：长度最小的子数组
int minSubArrayLen(int target, vector<int>& nums) {
    int left = 0, sum = 0, minLen = INT_MAX;
    
    for (int right = 0; right < nums.size(); ++right) {
        sum += nums[right];
        
        while (sum >= target) {
            minLen = min(minLen, right - left + 1);
            sum -= nums[left++];
        }
    }
    
    return minLen == INT_MAX ? 0 : minLen;
}

// 滑动窗口：无重复字符的最长子串
int lengthOfLongestSubstring(string s) {
    unordered_set<char> window;
    int left = 0, maxLen = 0;
    
    for (int right = 0; right < s.size(); ++right) {
        while (window.count(s[right])) {
            window.erase(s[left++]);
        }
        window.insert(s[right]);
        maxLen = max(maxLen, right - left + 1);
    }
    
    return maxLen;
}

// ==================== 主函数 ====================

int main() {
    cout << "╔════════════════════════════════════════════════════════════╗\n";
    cout << "║     Day 5: constexpr详解与滑动窗口算法                      ║\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n\n";
    
    // ========== Part 1: constexpr演示 ==========
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    cout << "▶ Part 1: constexpr 演示\n";
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";
    
    // constexpr变量
    cout << "【constexpr变量】\n";
    cout << "  MAX_SIZE = " << MAX_SIZE << " (编译期常量)\n";
    cout << "  PI = " << PI << " (编译期常量)\n";
    cout << "  数组大小可以使用: int arr[" << MAX_SIZE << "]\n\n";
    
    // constexpr函数
    cout << "【constexpr函数】\n";
    constexpr int sq5 = square(5);      // 编译期计算
    constexpr int fact5 = factorial(5); // 编译期计算
    constexpr int sum10 = sum_to_n(10); // C++14编译期计算
    
    cout << "  square(5) = " << sq5 << " (编译期计算)\n";
    cout << "  factorial(5) = " << fact5 << " (编译期计算)\n";
    cout << "  sum_to_n(10) = " << sum10 << " (C++14编译期计算)\n";
    
    // 运行时调用
    int n;
    cout << "\n  constexpr函数也可以运行时调用:\n";
    cout << "  输入n计算factorial(n): ";
    if (cin >> n) {
        auto start = chrono::high_resolution_clock::now();
        int runtime_fact = factorial(n);
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        cout << "  factorial(" << n << ") = " << runtime_fact 
             << " (耗时: " << duration.count() << " μs)\n";
    }
    
    // constexpr对象
    cout << "\n【constexpr对象】\n";
    constexpr Point p(3, 4);
    constexpr int dist = p.distance_squared();
    cout << "  constexpr Point p(3, 4)\n";
    cout << "  p.x() = " << p.x() << ", p.y() = " << p.y() << "\n";
    cout << "  p.distance_squared() = " << dist << " (编译期计算)\n";
    
    // const vs constexpr
    cout << "\n【const vs constexpr对比】\n";
    const int const_val = 100;          // 可以是编译期或运行期
    constexpr int constexpr_val = 100;  // 必须是编译期
    
    cout << "  const int const_val = 100;        // 可能运行期\n";
    cout << "  constexpr int constexpr_val = 100; // 必须编译期\n";
    cout << "  都可用于数组大小: int arr1[" << const_val << "], arr2[" << constexpr_val << "]\n";
    
    // ========== Part 2: 滑动窗口演示 ==========
    cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    cout << "▶ Part 2: 滑动窗口算法演示\n";
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";
    
    // LeetCode 209
    cout << "【LeetCode 209: 长度最小的子数组】\n";
    vector<int> nums = {2, 3, 1, 2, 4, 3};
    int target = 7;
    cout << "  输入: target = " << target << ", nums = [2,3,1,2,4,3]\n";
    
    cout << "\n  滑动窗口过程:\n";
    int left = 0, sum = 0, minLen = INT_MAX;
    for (int right = 0; right < nums.size(); ++right) {
        sum += nums[right];
        cout << "  right=" << right << ", 窗口=[";
        for (int i = left; i <= right; ++i) {
            cout << nums[i] << (i < right ? "," : "");
        }
        cout << "], sum=" << sum;
        
        if (sum >= target) {
            cout << " ≥ " << target << " ✓";
            while (sum >= target) {
                minLen = min(minLen, right - left + 1);
                cout << " (len=" << right - left + 1 << ")";
                sum -= nums[left++];
            }
        }
        cout << "\n";
    }
    cout << "  结果: 最小长度 = " << (minLen == INT_MAX ? 0 : minLen) << "\n";
    
    // LeetCode 3
    cout << "\n【LeetCode 3: 无重复字符的最长子串】\n";
    string s = "abcabcbb";
    cout << "  输入: s = \"" << s << "\"\n";
    
    cout << "\n  滑动窗口过程:\n";
    unordered_set<char> window;
    left = 0;
    int maxLen2 = 0;
    for (int right = 0; right < s.size(); ++right) {
        while (window.count(s[right])) {
            window.erase(s[left++]);
        }
        window.insert(s[right]);
        maxLen2 = max(maxLen2, right - left + 1);
        cout << "  right=" << right << ", 字符='" << s[right] 
             << "', 窗口=\"" << s.substr(left, right - left + 1) 
             << "\", 长度=" << right - left + 1 << "\n";
    }
    cout << "  结果: 最长子串长度 = " << maxLen2 << "\n";
    
    // ========== 总结 ==========
    cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    cout << "▶ 总结\n";
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";
    
    cout << "【constexpr要点】\n";
    cout << "  1. constexpr变量：编译期常量，必须立即初始化\n";
    cout << "  2. constexpr函数：可编译期求值，也可运行时调用\n";
    cout << "  3. constexpr构造函数：使类成为字面量类型\n";
    cout << "  4. if constexpr：编译期条件分支（C++17）\n\n";
    
    cout << "【滑动窗口要点】\n";
    cout << "  1. 双指针left/right定义窗口边界\n";
    cout << "  2. right单调递增，left单调不减\n";
    cout << "  3. 时间复杂度O(n)，每个元素最多处理两次\n";
    cout << "  4. 适用于连续区间、子数组、子串问题\n";
    
    cout << "\n╔════════════════════════════════════════════════════════════╗\n";
    cout << "║     Day 5 完成！继续加油！                                  ║\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n";
    
    return 0;
}
