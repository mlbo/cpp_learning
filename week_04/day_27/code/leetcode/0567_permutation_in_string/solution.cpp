/**
 * LeetCode 567: 字符串的排列 - 实现文件
 */

#include "solution.h"
#include <iostream>

using namespace std;

// ==================== 主函数实现 ====================

bool Solution::checkInclusion(string s1, string s2) {
    int n1 = s1.size();
    int n2 = s2.size();
    
    // 边界条件：s1 比 s2 长，不可能存在排列
    if (n1 > n2) {
        return false;
    }
    
    // 使用数组统计字符计数（假设只有小写字母）
    vector<int> count1(26, 0);  // s1 的字符计数
    vector<int> count2(26, 0);  // 窗口的字符计数
    
    // 统计 s1 的字符
    for (char c : s1) {
        count1[c - 'a']++;
    }
    
    // 初始化窗口：统计前 n1 个字符
    for (int i = 0; i < n1; i++) {
        count2[s2[i] - 'a']++;
    }
    
    // 检查初始窗口是否匹配
    if (count1 == count2) {
        return true;
    }
    
    // 滑动窗口
    for (int i = n1; i < n2; i++) {
        // 加入新字符
        count2[s2[i] - 'a']++;
        // 移除旧字符（窗口左边界）
        count2[s2[i - n1] - 'a']--;
        
        // 检查是否匹配
        if (count1 == count2) {
            return true;
        }
    }
    
    return false;
}

// ==================== 优化版本实现 ====================

bool Solution::checkInclusionOptimized(string s1, string s2) {
    int n1 = s1.size();
    int n2 = s2.size();
    
    if (n1 > n2) {
        return false;
    }
    
    // 使用数组统计字符计数差异
    // count[c] = 窗口中字符c的数量 - s1中字符c的数量
    vector<int> count(26, 0);
    
    // 初始化：窗口前 n1 个字符与 s1 的差异
    for (int i = 0; i < n1; i++) {
        count[s1[i] - 'a']--;
        count[s2[i] - 'a']++;
    }
    
    // 统计有多少种字符的数量不匹配
    int diff = 0;
    for (int i = 0; i < 26; i++) {
        if (count[i] != 0) {
            diff++;
        }
    }
    
    // 如果初始窗口就匹配
    if (diff == 0) {
        return true;
    }
    
    // 滑动窗口
    for (int i = n1; i < n2; i++) {
        // 加入的新字符
        int in = s2[i] - 'a';
        // 移除的旧字符
        int out = s2[i - n1] - 'a';
        
        // 处理新字符的加入
        if (count[in] == 0) {
            diff++;  // 原本匹配，现在不匹配了
        }
        count[in]++;
        if (count[in] == 0) {
            diff--;  // 现在匹配了
        }
        
        // 处理旧字符的移除
        if (count[out] == 0) {
            diff++;  // 原本匹配，现在不匹配了
        }
        count[out]--;
        if (count[out] == 0) {
            diff--;  // 现在匹配了
        }
        
        // 检查是否完全匹配
        if (diff == 0) {
            return true;
        }
    }
    
    return false;
}

// ==================== 算法演示函数 ====================

/**
 * 演示滑动窗口的执行过程
 * 用于理解算法的每一步操作
 */
void demonstratePermutationAlgorithm(const string& s1, const string& s2) {
    cout << "\n演示: s1 = \"" << s1 << "\", s2 = \"" << s2 << "\"" << endl;
    cout << "----------------------------------------" << endl;
    
    int n1 = s1.size();
    int n2 = s2.size();
    
    if (n1 > n2) {
        cout << "s1 比 s2 长，不可能存在排列" << endl;
        return;
    }
    
    // 统计 s1 的字符
    vector<int> count1(26, 0);
    for (char c : s1) {
        count1[c - 'a']++;
    }
    
    cout << "s1 字符统计: ";
    for (int i = 0; i < 26; i++) {
        if (count1[i] > 0) {
            cout << (char)('a' + i) << ":" << count1[i] << " ";
        }
    }
    cout << endl;
    
    // 初始化窗口
    vector<int> count2(26, 0);
    for (int i = 0; i < n1; i++) {
        count2[s2[i] - 'a']++;
    }
    
    cout << "\n初始窗口 [0," << n1 - 1 << "]: \"" << s2.substr(0, n1) << "\"" << endl;
    cout << "窗口字符统计: ";
    for (int i = 0; i < 26; i++) {
        if (count2[i] > 0) {
            cout << (char)('a' + i) << ":" << count2[i] << " ";
        }
    }
    cout << endl;
    
    if (count1 == count2) {
        cout << ">>> 匹配成功！返回 true" << endl;
        return;
    }
    cout << "不匹配，继续滑动..." << endl;
    
    // 滑动窗口
    for (int i = n1; i < n2; i++) {
        // 加入新字符，移除旧字符
        count2[s2[i] - 'a']++;
        count2[s2[i - n1] - 'a']--;
        
        cout << "\n窗口 [" << i - n1 + 1 << "," << i << "]: \"";
        cout << s2.substr(i - n1 + 1, n1) << "\"" << endl;
        
        cout << "窗口字符统计: ";
        for (int j = 0; j < 26; j++) {
            if (count2[j] > 0) {
                cout << (char)('a' + j) << ":" << count2[j] << " ";
            }
        }
        cout << endl;
        
        if (count1 == count2) {
            cout << ">>> 匹配成功！返回 true" << endl;
            return;
        }
        cout << "不匹配，继续滑动..." << endl;
    }
    
    cout << "\n遍历完成，未找到匹配。返回 false" << endl;
}
