/**
 * Day 27 主程序入口
 * 字符串专题演示
 */

#include <iostream>
#include <string>
#include <vector>
#include <climits>

using namespace std;

// 函数声明
void demonstrateStringOperations();
void demonstrateSlidingWindow();
void demonstrateStringMatching();
void demonstrateMinimumWindow();
void demonstratePermutationInString();

// 打印分隔线
void printSeparator(const string& title) {
    cout << "\n";
    cout << "========================================" << endl;
    cout << "  " << title << endl;
    cout << "========================================" << endl;
}

// 打印子标题
void printSubTitle(const string& title) {
    cout << "\n--- " << title << " ---" << endl;
}

int main() {
    cout << R"(
╔════════════════════════════════════════╗
║       Day 27: 字符串专题               ║
║                                        ║
║  • 字符串基础与操作                    ║
║  • 字符串匹配算法                      ║
║  • 滑动窗口技术                        ║
║  • LeetCode 经典题目                   ║
╚════════════════════════════════════════╝
)" << endl;

    // 1. 字符串基础操作演示
    printSeparator("1. 字符串基础操作");
    demonstrateStringOperations();

    // 2. 滑动窗口算法演示
    printSeparator("2. 滑动窗口算法");
    demonstrateSlidingWindow();

    // 3. 字符串匹配算法演示
    printSeparator("3. 字符串匹配算法");
    demonstrateStringMatching();

    // 4. LeetCode 76 演示
    printSeparator("4. LeetCode 76: 最小覆盖子串");
    demonstrateMinimumWindow();

    // 5. LeetCode 567 演示
    printSeparator("5. LeetCode 567: 字符串的排列");
    demonstratePermutationInString();

    // 学习总结
    printSeparator("学习总结");
    cout << R"(
今天我们学习了字符串处理的核心技能：

1. 【字符串基础】
   - std::string 的初始化、容量操作、元素访问
   - 字符串查找、截取、修改等常用操作
   - 短字符串优化(SSO)和移动语义

2. 【字符串匹配】
   - 暴力匹配：简单直观，O(n*m) 复杂度
   - KMP算法：利用已匹配信息，O(n+m) 复杂度
   - Rabin-Karp：哈希思想，适合多模式匹配

3. 【滑动窗口】
   - 核心思想：维护可变/固定长度的窗口
   - 适用场景：子串问题、连续区间问题
   - 技巧：增量式更新，避免重复计算

4. 【LeetCode 实战】
   - LC 76：可变窗口，寻找最小覆盖子串
   - LC 567：固定窗口，判断排列是否存在

继续练习，巩固滑动窗口技巧！
)" << endl;

    return 0;
}

// 演示字符串操作
void demonstrateStringOperations() {
    printSubTitle("字符串初始化");
    
    // 多种初始化方式
    string s1;                      // 空字符串
    string s2 = "Hello";            // 从C字符串构造
    string s3(5, 'a');              // 5个'a'
    string s4(s2);                  // 拷贝构造
    string s5 = s2 + " World";      // 拼接
    
    cout << "s1 (空字符串): \"" << s1 << "\"" << endl;
    cout << "s2 = \"Hello\": \"" << s2 << "\"" << endl;
    cout << "s3(5, 'a'): \"" << s3 << "\"" << endl;
    cout << "s4(s2): \"" << s4 << "\"" << endl;
    cout << "s2 + \" World\": \"" << s5 << "\"" << endl;
    
    printSubTitle("容量操作");
    cout << "s5.size() = " << s5.size() << endl;
    cout << "s5.empty() = " << (s5.empty() ? "true" : "false") << endl;
    cout << "s1.empty() = " << (s1.empty() ? "true" : "false") << endl;
    
    printSubTitle("元素访问");
    cout << "s2[0] = '" << s2[0] << "'" << endl;
    cout << "s2.at(1) = '" << s2.at(1) << "'" << endl;
    cout << "s2.front() = '" << s2.front() << "'" << endl;
    cout << "s2.back() = '" << s2.back() << "'" << endl;
    
    printSubTitle("查找操作");
    string text = "Hello World, Hello C++";
    cout << "文本: \"" << text << "\"" << endl;
    cout << "find(\"Hello\"): " << text.find("Hello") << endl;
    cout << "rfind(\"Hello\"): " << text.rfind("Hello") << endl;
    cout << "find_first_of(\"aeiou\"): " << text.find_first_of("aeiou") << endl;
    
    printSubTitle("修改操作");
    string s = "Hello World";
    cout << "原始: \"" << s << "\"" << endl;
    s.replace(6, 5, "C++");
    cout << "replace后: \"" << s << "\"" << endl;
    s.erase(5, 1);
    cout << "erase后: \"" << s << "\"" << endl;
    s.insert(5, " Beautiful");
    cout << "insert后: \"" << s << "\"" << endl;
}

// 演示滑动窗口概念
void demonstrateSlidingWindow() {
    printSubTitle("滑动窗口基本概念");
    cout << R"(
滑动窗口是一种在字符串/数组上维护连续区间的技巧：
- 窗口：由左右边界[left, right)定义的区间
- 扩展：right向右移动，窗口变大
- 收缩：left向右移动，窗口变小
- 核心思想：增量更新，避免重复计算
)" << endl;
    
    printSubTitle("简单示例：最长无重复字符子串");
    string s = "abcabcbb";
    cout << "字符串: \"" << s << "\"" << endl;
    
    // 滑动窗口实现
    vector<int> charIndex(256, -1);  // 记录字符最后出现位置
    int maxLen = 0, start = 0;
    int left = 0;
    
    for (int right = 0; right < s.size(); right++) {
        char c = s[right];
        // 如果字符已在窗口内，移动左边界
        if (charIndex[c] >= left) {
            left = charIndex[c] + 1;
        }
        charIndex[c] = right;
        
        if (right - left + 1 > maxLen) {
            maxLen = right - left + 1;
            start = left;
        }
    }
    
    cout << "最长无重复字符子串: \"" << s.substr(start, maxLen) << "\"" << endl;
    cout << "长度: " << maxLen << endl;
}

// 演示字符串匹配
void demonstrateStringMatching() {
    printSubTitle("暴力匹配算法");
    string text = "ABABABCABABCABAB";
    string pattern = "ABABCABAB";
    
    cout << "文本: \"" << text << "\"" << endl;
    cout << "模式: \"" << pattern << "\"" << endl;
    
    // 暴力匹配
    int found = -1;
    for (int i = 0; i <= (int)(text.size() - pattern.size()); i++) {
        int j = 0;
        while (j < (int)pattern.size() && text[i + j] == pattern[j]) {
            j++;
        }
        if (j == (int)pattern.size()) {
            found = i;
            break;
        }
    }
    
    if (found != -1) {
        cout << "找到匹配位置: " << found << endl;
        cout << "匹配子串: \"" << text.substr(found, pattern.size()) << "\"" << endl;
    } else {
        cout << "未找到匹配" << endl;
    }
    
    printSubTitle("KMP算法思想");
    cout << R"(
KMP算法的核心思想：
1. 当匹配失败时，利用已匹配信息跳过不必要的比较
2. 构建next数组，记录模式串的"部分匹配"信息
3. 时间复杂度从O(n*m)优化到O(n+m)

next数组含义：对于模式串位置i，next[i]表示
以i结尾的子串中，最长的相等前后缀长度。
)" << endl;
    
    // 计算next数组演示
    cout << "模式串 \"" << pattern << "\" 的部分匹配分析：" << endl;
    vector<int> next(pattern.size(), 0);
    int j = 0;
    for (int i = 1; i < (int)pattern.size(); i++) {
        while (j > 0 && pattern[i] != pattern[j]) {
            j = next[j - 1];
        }
        if (pattern[i] == pattern[j]) {
            j++;
        }
        next[i] = j;
    }
    
    cout << "next数组: [";
    for (int i = 0; i < (int)next.size(); i++) {
        cout << next[i];
        if (i < (int)next.size() - 1) cout << ", ";
    }
    cout << "]" << endl;
}

// 演示最小覆盖子串
void demonstrateMinimumWindow() {
    printSubTitle("问题描述");
    cout << R"(
给定字符串s和t，在s中找到涵盖t所有字符的最小子串。
如果不存在，返回空字符串。

示例：
s = "ADOBECODEBANC", t = "ABC"
最小覆盖子串 = "BANC"
)" << endl;
    
    printSubTitle("算法演示");
    string s = "ADOBECODEBANC";
    string t = "ABC";
    
    cout << "s = \"" << s << "\"" << endl;
    cout << "t = \"" << t << "\"" << endl;
    
    // 滑动窗口求解
    vector<int> need(128, 0);
    vector<int> window(128, 0);
    
    for (char c : t) need[c]++;
    
    int left = 0, right = 0;
    int valid = 0;  // 窗口中满足需求的字符种类数
    int needCount = 0;  // t中不同字符的种类数
    for (int i = 0; i < 128; i++) {
        if (need[i] > 0) needCount++;
    }
    
    int minLen = INT_MAX;
    int minStart = 0;
    
    while (right < (int)s.size()) {
        // 扩展窗口
        char c = s[right];
        right++;
        
        if (need[c] > 0) {
            window[c]++;
            if (window[c] == need[c]) {
                valid++;
            }
        }
        
        // 收缩窗口
        while (valid == needCount) {
            // 更新结果
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
    
    if (minLen != INT_MAX) {
        cout << "\n最小覆盖子串: \"" << s.substr(minStart, minLen) << "\"" << endl;
        cout << "起始位置: " << minStart << ", 长度: " << minLen << endl;
    } else {
        cout << "\n不存在覆盖子串" << endl;
    }
}

// 演示字符串的排列
void demonstratePermutationInString() {
    printSubTitle("问题描述");
    cout << R"(
判断s2是否包含s1的某个排列。
即：s1的某种排列是否是s2的子串。

示例：
s1 = "ab", s2 = "eidbaooo"
返回 true（因为"ba"是s1的排列，且是s2的子串）
)" << endl;
    
    printSubTitle("算法演示");
    string s1 = "ab";
    string s2 = "eidbaooo";
    
    cout << "s1 = \"" << s1 << "\"" << endl;
    cout << "s2 = \"" << s2 << "\"" << endl;
    
    // 固定窗口滑动
    vector<int> count1(26, 0);
    vector<int> count2(26, 0);
    
    for (char c : s1) count1[c - 'a']++;
    
    int windowSize = s1.size();
    bool found = false;
    int foundPos = -1;
    
    for (int i = 0; i < (int)s2.size(); i++) {
        // 加入新字符
        count2[s2[i] - 'a']++;
        
        // 移除超出窗口的字符
        if (i >= windowSize) {
            count2[s2[i - windowSize] - 'a']--;
        }
        
        // 检查是否匹配
        if (i >= windowSize - 1) {
            if (count1 == count2) {
                found = true;
                foundPos = i - windowSize + 1;
                break;
            }
        }
    }
    
    if (found) {
        cout << "\n找到排列子串: \"" << s2.substr(foundPos, windowSize) << "\"" << endl;
        cout << "起始位置: " << foundPos << endl;
    } else {
        cout << "\n未找到排列子串" << endl;
    }
}
