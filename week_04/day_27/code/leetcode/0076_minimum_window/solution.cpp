/**
 * LeetCode 76: 最小覆盖子串 - 实现文件
 */

#include "solution.h"
#include <iostream>
#include <limits>
#include <unordered_map>
#include <vector>

using namespace std;

namespace day27::lc0076 {

// ==================== 主函数实现 ====================

string Solution::minWindow(string s, string t) {
    if (s.empty() || t.empty() || s.size() < t.size()) {
        return "";
    }
    
    // 使用数组代替哈希表
    // 按“字节”统计，转成 unsigned char 后再做下标。
    // 直接用可能为负的 char 下标会越界；这里也不把 UTF-8 的一个字节误称为一个字符。
    vector<int> need(256, 0);
    vector<int> window(256, 0);
    
    for (char character : t) {
        const auto byte = static_cast<unsigned char>(character);
        ++need[byte];
    }
    
    size_t needCount = 0;
    for (size_t i = 0; i < need.size(); ++i) {
        if (need[i] > 0) ++needCount;
    }
    
    size_t left = 0;
    size_t right = 0;
    size_t valid = 0;
    size_t minLen = string::npos;
    size_t minStart = 0;
    
    while (right < s.size()) {
        const unsigned char c = static_cast<unsigned char>(s[right]);
        ++right;
        
        if (need[c] > 0) {
            window[c]++;
            if (window[c] == need[c]) {
                ++valid;
            }
        }
        
        while (valid == needCount) {
            if (right - left < minLen) {
                minLen = right - left;
                minStart = left;
            }
            
            const unsigned char d = static_cast<unsigned char>(s[left]);
            left++;
            
            if (need[d] > 0) {
                if (window[d] == need[d]) {
                    --valid;
                }
                --window[d];
            }
        }
    }
    
    return minLen == string::npos ? "" : s.substr(minStart, minLen);
}

// ==================== 算法演示函数 ====================

/**
 * 演示滑动窗口的执行过程
 * 用于理解算法的每一步操作
 */
void demonstrateAlgorithm(const string& s, const string& t) {
    cout << "\n演示: s = \"" << s << "\", t = \"" << t << "\"" << endl;
    cout << "----------------------------------------" << endl;
    if (t.empty()) {
        cout << "空目标按接口约定返回空串；窗口不进入收缩循环。" << endl;
        return;
    }
    if (s.empty() || s.size() < t.size()) {
        cout << "源串为空或短于目标，不存在覆盖窗口。" << endl;
        return;
    }
    
    unordered_map<char, int> need;
    for (char c : t) need[c]++;
    
    unordered_map<char, int> window;
    size_t left = 0;
    size_t right = 0;
    size_t valid = 0;
    size_t minLen = string::npos;
    size_t minStart = 0;
    
    size_t step = 0;
    while (right < s.size()) {
        const char c = s[right];
        ++right;
        
        if (need.count(c)) {
            window[c]++;
            if (window[c] == need[c]) {
                valid++;
            }
        }
        
        while (valid == need.size()) {
            if (right - left < minLen) {
                minLen = right - left;
                minStart = left;
            }
            
            cout << "步骤" << ++step << ": ";
            cout << "窗口[" << left << "," << right-1 << "] = \"";
            cout << s.substr(left, right - left) << "\"";
            cout << " -> 当前最小: \"" << s.substr(minStart, minLen) << "\"" << endl;
            
            const char d = s[left];
            ++left;
            
            if (need.count(d)) {
                if (window[d] == need[d]) {
                    --valid;
                }
                --window[d];
            }
        }
    }
    
    const string result = minLen == string::npos ? "" : s.substr(minStart, minLen);
    cout << "\n最终结果: \"" << result << "\"" << endl;
}

} // namespace day27::lc0076
