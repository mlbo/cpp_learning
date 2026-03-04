/**
 * LeetCode 76: 最小覆盖子串 - 实现文件
 */

#include "solution.h"
#include <iostream>
#include <unordered_map>

using namespace std;

// ==================== 主函数实现 ====================

string Solution::minWindow(string s, string t) {
    if (s.empty() || t.empty() || s.size() < t.size()) {
        return "";
    }
    
    // 使用数组代替哈希表
    vector<int> need(128, 0);
    vector<int> window(128, 0);
    
    for (char c : t) {
        need[c]++;
    }
    
    int needCount = 0;
    for (int i = 0; i < 128; i++) {
        if (need[i] > 0) needCount++;
    }
    
    int left = 0, right = 0;
    int valid = 0;
    int minLen = INT_MAX;
    int minStart = 0;
    
    while (right < (int)s.size()) {
        char c = s[right];
        right++;
        
        if (need[c] > 0) {
            window[c]++;
            if (window[c] == need[c]) {
                valid++;
            }
        }
        
        while (valid == needCount) {
            if (right - left < minLen) {
                minLen = right - left;
                minStart = left;
            }
            
            char d = s[left];
            left++;
            
            if (need[d] > 0) {
                if (window[d] == need[d]) {
                    valid--;
                }
                window[d]--;
            }
        }
    }
    
    return minLen == INT_MAX ? "" : s.substr(minStart, minLen);
}

// ==================== 算法演示函数 ====================

/**
 * 演示滑动窗口的执行过程
 * 用于理解算法的每一步操作
 */
void demonstrateAlgorithm(const string& s, const string& t) {
    cout << "\n演示: s = \"" << s << "\", t = \"" << t << "\"" << endl;
    cout << "----------------------------------------" << endl;
    
    unordered_map<char, int> need;
    for (char c : t) need[c]++;
    
    unordered_map<char, int> window;
    int left = 0, right = 0;
    int valid = 0;
    int minLen = INT_MAX;
    int minStart = 0;
    
    int step = 0;
    while (right < (int)s.size()) {
        char c = s[right];
        right++;
        
        if (need.count(c)) {
            window[c]++;
            if (window[c] == need[c]) {
                valid++;
            }
        }
        
        while (valid == (int)need.size()) {
            if (right - left < minLen) {
                minLen = right - left;
                minStart = left;
            }
            
            cout << "步骤" << ++step << ": ";
            cout << "窗口[" << left << "," << right-1 << "] = \"";
            cout << s.substr(left, right - left) << "\"";
            cout << " -> 当前最小: \"" << s.substr(minStart, minLen) << "\"" << endl;
            
            char d = s[left];
            left++;
            
            if (need.count(d)) {
                if (window[d] == need[d]) {
                    valid--;
                }
                window[d]--;
            }
        }
    }
    
    string result = minLen == INT_MAX ? "" : s.substr(minStart, minLen);
    cout << "\n最终结果: \"" << result << "\"" << endl;
}
