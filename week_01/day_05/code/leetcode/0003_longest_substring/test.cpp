/**
 * @file test.cpp
 * @brief LeetCode 3: 无重复字符的最长子串 - 测试文件
 */

#include "solution.h"
#include <iostream>
#include <string>
#include <chrono>

using namespace std;

// 测试用例结构
struct TestCase {
    string input;
    int expected;
    string description;
};

// 运行测试
void runTests() {
    Solution solution;
    
    // 测试用例
    vector<TestCase> testCases = {
        {"abcabcbb", 3, "标准用例 - abc"},
        {"bbbbb", 1, "全部相同字符 - b"},
        {"pwwkew", 3, "中间重复 - wke"},
        {"", 0, "空字符串"},
        {"a", 1, "单字符"},
        {"au", 2, "两个不同字符"},
        {"dvdf", 3, "跳过重复 - vdf"},
        {"abba", 2, "回文字符串 - ab/ba"},
        {"tmmzuxt", 5, "复杂情况 - mzuxt"},
        {"abcdefghijklmnopqrstuvwxyz", 26, "全部不同"},
        {"aab", 2, "开头重复"},
        {"abcdeafbdgc", 7, "多重复"},
    };
    
    cout << "╔════════════════════════════════════════════════════════════╗\n";
    cout << "║        LeetCode 3: 无重复字符的最长子串                     ║\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n\n";
    
    int passed = 0;
    int total = testCases.size();
    
    for (size_t i = 0; i < testCases.size(); ++i) {
        const auto& tc = testCases[i];
        
        cout << "测试用例 " << i + 1 << ": " << tc.description << "\n";
        cout << "  输入: \"" << tc.input << "\"\n";
        cout << "  期望输出: " << tc.expected << "\n";
        
        // 测试方法一
        auto start = chrono::high_resolution_clock::now();
        int result = solution.lengthOfLongestSubstring(tc.input);
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
        int result2 = solution.lengthOfLongestSubstringArray(tc.input);
        int result3 = solution.lengthOfLongestSubstringOptimized(tc.input);
        int result4 = solution.lengthOfLongestSubstringBruteForce(tc.input);
        
        cout << "  其他方法: 数组=" << result2 
             << ", 优化=" << result3 
             << ", 暴力=" << result4 << "\n";
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
    
    string s = "abcabcbb";
    
    cout << "输入: \"" << s << "\"\n\n";
    cout << "滑动窗口执行过程:\n";
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    
    unordered_set<char> window;
    int left = 0;
    int maxLen = 0;
    
    for (int right = 0; right < s.size(); ++right) {
        cout << "right=" << right << " ('" << s[right] << "'): ";
        
        // 处理重复
        if (window.count(s[right])) {
            cout << "发现重复，收缩窗口: ";
            while (window.count(s[right])) {
                cout << "移除 '" << s[left] << "' ";
                window.erase(s[left++]);
            }
            cout << "-> ";
        } else {
            cout << "无重复，扩展窗口: ";
        }
        
        window.insert(s[right]);
        int len = right - left + 1;
        maxLen = max(maxLen, len);
        
        cout << "窗口=\"";
        for (int i = left; i <= right; ++i) {
            cout << s[i];
        }
        cout << "\", 长度=" << len;
        if (len == maxLen) {
            cout << " (最大)";
        }
        cout << "\n";
    }
    
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    cout << "最终结果: " << maxLen << "\n";
}

// 展示优化版本的过程
void demonstrateOptimizedVersion() {
    cout << "\n╔════════════════════════════════════════════════════════════╗\n";
    cout << "║           优化版本演示（直接跳转）                          ║\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n\n";
    
    string s = "abba";
    
    cout << "输入: \"" << s << "\"\n\n";
    cout << "优化版本执行过程（使用位置映射）:\n";
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    
    unordered_map<char, int> lastPos;
    int left = 0;
    int maxLen = 0;
    
    for (int right = 0; right < s.size(); ++right) {
        char c = s[right];
        
        cout << "right=" << right << " ('" << c << "'): ";
        
        if (lastPos.count(c) && lastPos[c] > left) {
            cout << "上次位置=" << (lastPos[c] - 1) 
                 << ", 跳转left从" << left << "到" << lastPos[c];
            left = lastPos[c];
        } else {
            cout << "无重复或不在窗口内";
        }
        
        lastPos[c] = right + 1;
        int len = right - left + 1;
        maxLen = max(maxLen, len);
        
        cout << ", 窗口长度=" << len << "\n";
    }
    
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    cout << "最终结果: " << maxLen << "\n";
}

// 复杂度分析
void analyzeComplexity() {
    cout << "\n╔════════════════════════════════════════════════════════════╗\n";
    cout << "║                    复杂度分析                               ║\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n\n";
    
    cout << "┌───────────────────────┬──────────────┬─────────────────┐\n";
    cout << "│        方法           │  时间复杂度  │   空间复杂度    │\n";
    cout << "├───────────────────────┼──────────────┼─────────────────┤\n";
    cout << "│   滑动窗口+哈希集合   │    O(n)      │  O(min(m,n))    │\n";
    cout << "│   滑动窗口+数组       │    O(n)      │     O(m)        │\n";
    cout << "│   滑动窗口+哈希映射   │    O(n)      │  O(min(m,n))    │\n";
    cout << "│       暴力法          │    O(n³)     │  O(min(m,n))    │\n";
    cout << "└───────────────────────┴──────────────┴─────────────────┘\n";
    cout << "\n注: m为字符集大小（ASCII为128），n为字符串长度\n\n";
    
    cout << "优化技巧:\n";
    cout << "  1. 使用数组代替哈希集合（常数优化）\n";
    cout << "  2. 记录字符位置，直接跳转（减少移动次数）\n";
    cout << "  3. 使用right+1记录位置，避免0的歧义\n";
}

// 方法对比
void compareMethods() {
    cout << "\n╔════════════════════════════════════════════════════════════╗\n";
    cout << "║                    方法对比                                 ║\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n\n";
    
    cout << "方法一：滑动窗口 + 哈希集合\n";
    cout << "  优点：思路清晰，代码简洁\n";
    cout << "  缺点：重复时需要逐个移动left\n";
    cout << "  适用：一般情况\n\n";
    
    cout << "方法二：滑动窗口 + 数组\n";
    cout << "  优点：访问速度快，常数级优化\n";
    cout << "  缺点：需要知道字符集大小\n";
    cout << "  适用：字符集已知且较小（如ASCII）\n\n";
    
    cout << "方法三：滑动窗口 + 哈希映射\n";
    cout << "  优点：直接跳转，减少移动次数\n";
    cout << "  缺点：需要额外空间存储位置\n";
    cout << "  适用：追求最优时间性能\n";
}

int main() {
    runTests();
    demonstrateSlidingWindow();
    demonstrateOptimizedVersion();
    analyzeComplexity();
    compareMethods();
    
    return 0;
}
