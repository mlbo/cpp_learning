/**
 * 滑动窗口算法演示
 * 展示滑动窗口的核心思想和典型应用
 */

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <climits>

using namespace std;

// 打印标题
void printTitle(const string& title) {
    cout << "\n========== " << title << " ==========" << endl;
}

// 打印分隔线
void printLine() {
    cout << "----------------------------------------" << endl;
}

// ==================== 基础滑动窗口模板 ====================

/**
 * 滑动窗口基本框架演示
 * 适用于寻找满足特定条件的子串/子数组
 */
void demonstrateBasicFramework() {
    printTitle("滑动窗口基本框架");
    
    cout << R"(
滑动窗口核心思想：
┌─────────────────────────────────────────┐
│  while (right < n) {                    │
│      // 1. 扩展窗口：加入右边界的元素    │
│      char c = s[right++];               │
│      // 更新窗口状态...                 │
│                                         │
│      // 2. 判断是否需要收缩             │
│      while (窗口需要收缩) {             │
│          // 3. 更新结果                 │
│          // 4. 收缩窗口：移除左边界元素  │
│          char d = s[left++];            │
│          // 更新窗口状态...             │
│      }                                  │
│  }                                      │
└─────────────────────────────────────────┘
)" << endl;
}

// ==================== 经典题目1：最长无重复字符子串 ====================

/**
 * LeetCode 3: 无重复字符的最长子串
 * 给定字符串，找出不含有重复字符的最长子串长度
 */
int lengthOfLongestSubstring(const string& s) {
    // 使用哈希集合记录窗口中的字符
    unordered_set<char> window;
    int left = 0, maxLen = 0;
    
    for (int right = 0; right < (int)s.size(); right++) {
        char c = s[right];
        
        // 如果字符已在窗口中，收缩左边界直到无重复
        while (window.count(c)) {
            window.erase(s[left]);
            left++;
        }
        
        // 加入新字符
        window.insert(c);
        
        // 更新最大长度
        maxLen = max(maxLen, right - left + 1);
    }
    
    return maxLen;
}

void demoLongestSubstring() {
    printTitle("题目1：最长无重复字符子串");
    
    cout << R"(
问题描述：
给定一个字符串 s，找出其中不含有重复字符的最长子串的长度。

算法思路：
1. 用哈希集合记录当前窗口中的字符
2. 遇到重复字符时，收缩左边界
3. 维护最大长度
)" << endl;
    
    vector<string> testCases = {
        "abcabcbb",
        "bbbbb",
        "pwwkew"
    };
    
    for (const string& s : testCases) {
        cout << "\n输入: \"" << s << "\"" << endl;
        
        // 详细演示过程
        unordered_set<char> window;
        int left = 0;
        int maxLen = 0, maxStart = 0;
        
        cout << "滑动过程:" << endl;
        for (int right = 0; right < (int)s.size(); right++) {
            char c = s[right];
            
            while (window.count(c)) {
                window.erase(s[left]);
                left++;
            }
            
            window.insert(c);
            
            if (right - left + 1 > maxLen) {
                maxLen = right - left + 1;
                maxStart = left;
            }
            
            cout << "  right=" << right << " ('" << c << "'), ";
            cout << "窗口: [" << left << "," << right << "] = \"";
            cout << s.substr(left, right - left + 1) << "\"" << endl;
        }
        
        cout << "最长无重复子串: \"" << s.substr(maxStart, maxLen) << "\"" << endl;
        cout << "长度: " << maxLen << endl;
    }
}

// ==================== 经典题目2：最小长度子数组和 ====================

/**
 * LeetCode 209: 长度最小的子数组
 * 给定正整数数组和正整数s，找出和≥s的最小连续子数组长度
 */
int minSubArrayLen(int target, const vector<int>& nums) {
    int left = 0, sum = 0;
    int minLen = INT_MAX;
    
    for (int right = 0; right < (int)nums.size(); right++) {
        sum += nums[right];
        
        // 当和>=target时，尝试收缩窗口
        while (sum >= target) {
            minLen = min(minLen, right - left + 1);
            sum -= nums[left];
            left++;
        }
    }
    
    return minLen == INT_MAX ? 0 : minLen;
}

void demoMinSubArrayLen() {
    printTitle("题目2：最小长度子数组和");
    
    cout << R"(
问题描述：
给定一个含有n个正整数的数组和一个正整数target，
找出该数组中满足其和≥target的长度最小的连续子数组长度。

算法思路：
1. 扩展窗口直到和>=target
2. 尝试收缩窗口，寻找更短的满足条件的子数组
3. 记录最小长度
)" << endl;
    
    vector<int> nums = {2, 3, 1, 2, 4, 3};
    int target = 7;
    
    cout << "数组: [";
    for (size_t i = 0; i < nums.size(); i++) {
        cout << nums[i];
        if (i < nums.size() - 1) cout << ", ";
    }
    cout << "]" << endl;
    cout << "target: " << target << endl;
    
    // 详细演示过程
    int left = 0, sum = 0;
    int minLen = INT_MAX;
    
    cout << "\n滑动过程:" << endl;
    for (int right = 0; right < (int)nums.size(); right++) {
        sum += nums[right];
        
        cout << "  right=" << right << " (+" << nums[right] << "), sum=" << sum;
        
        while (sum >= target) {
            int len = right - left + 1;
            cout << " -> 满足条件！长度=" << len;
            
            if (len < minLen) {
                minLen = len;
                cout << " (更新最小值)";
            }
            
            sum -= nums[left];
            left++;
            cout << ", 收缩后sum=" << sum;
        }
        cout << endl;
    }
    
    cout << "\n最小长度: " << (minLen == INT_MAX ? 0 : minLen) << endl;
}

// ==================== 经典题目3：至多包含K个不同字符的子串 ====================

/**
 * LeetCode 340: 至多包含K个不同字符的最长子串
 * 给定字符串和整数k，找出至多包含k个不同字符的最长子串长度
 */
int lengthOfLongestSubstringKDistinct(const string& s, int k) {
    unordered_map<char, int> charCount;  // 字符计数
    int left = 0, maxLen = 0;
    int distinct = 0;  // 不同字符数量
    
    for (int right = 0; right < (int)s.size(); right++) {
        char c = s[right];
        
        if (charCount[c] == 0) {
            distinct++;
        }
        charCount[c]++;
        
        // 当不同字符超过k时收缩
        while (distinct > k) {
            char d = s[left];
            charCount[d]--;
            if (charCount[d] == 0) {
                distinct--;
            }
            left++;
        }
        
        maxLen = max(maxLen, right - left + 1);
    }
    
    return maxLen;
}

void demoKDistinct() {
    printTitle("题目3：至多K个不同字符的最长子串");
    
    cout << R"(
问题描述：
给定一个字符串s和一个整数k，找出至多包含k个不同字符的最长子串长度。

算法思路：
1. 用哈希表记录窗口中各字符的数量
2. 当不同字符数>k时收缩窗口
3. 维护最大长度
)" << endl;
    
    string s = "eceba";
    int k = 2;
    
    cout << "字符串: \"" << s << "\"" << endl;
    cout << "k: " << k << endl;
    
    unordered_map<char, int> charCount;
    int left = 0, maxLen = 0, maxStart = 0;
    int distinct = 0;
    
    cout << "\n滑动过程:" << endl;
    for (int right = 0; right < (int)s.size(); right++) {
        char c = s[right];
        
        if (charCount[c] == 0) distinct++;
        charCount[c]++;
        
        while (distinct > k) {
            char d = s[left];
            charCount[d]--;
            if (charCount[d] == 0) distinct--;
            left++;
        }
        
        cout << "  right=" << right << " ('" << c << "'), ";
        cout << "distinct=" << distinct << ", ";
        cout << "窗口: \"" << s.substr(left, right - left + 1) << "\"" << endl;
        
        if (right - left + 1 > maxLen) {
            maxLen = right - left + 1;
            maxStart = left;
        }
    }
    
    cout << "\n最长子串: \"" << s.substr(maxStart, maxLen) << "\"" << endl;
    cout << "长度: " << maxLen << endl;
}

// ==================== 固定窗口大小示例 ====================

/**
 * 固定窗口大小演示
 * 在固定大小的窗口中统计信息
 */
void demoFixedWindow() {
    printTitle("固定窗口大小示例");
    
    cout << R"(
固定窗口特点：
- 窗口大小固定为k
- 每次移动一个位置：加入新元素，移除旧元素
- 适用于：平均值、统计频率等场景
)" << endl;
    
    vector<int> nums = {1, 3, -1, -3, 5, 3, 6, 7};
    int k = 3;
    
    cout << "数组: [";
    for (size_t i = 0; i < nums.size(); i++) {
        cout << nums[i];
        if (i < nums.size() - 1) cout << ", ";
    }
    cout << "]" << endl;
    cout << "窗口大小: " << k << endl;
    
    cout << "\n计算每个窗口的最大值（滑动窗口最大值）:" << endl;
    
    // 使用双端队列求解滑动窗口最大值
    vector<int> result;
    vector<int> dq;  // 存储下标
    
    for (int i = 0; i < (int)nums.size(); i++) {
        // 移除超出窗口的元素
        while (!dq.empty() && dq.front() <= i - k) {
            dq.erase(dq.begin());
        }
        
        // 移除比当前元素小的元素（它们不可能是最大值）
        while (!dq.empty() && nums[dq.back()] < nums[i]) {
            dq.pop_back();
        }
        
        dq.push_back(i);
        
        if (i >= k - 1) {
            result.push_back(nums[dq.front()]);
            cout << "  窗口 [" << i - k + 1 << "," << i << "]: 最大值=" << nums[dq.front()] << endl;
        }
    }
}

// ==================== 滑动窗口技巧总结 ====================

void summarizeTechniques() {
    printTitle("滑动窗口技巧总结");
    
    cout << R"(
┌─────────────────────────────────────────────────────────┐
│                   滑动窗口核心要点                       │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  1. 窗口类型                                            │
│     • 可变窗口：左右边界都可移动                         │
│     • 固定窗口：窗口大小固定，整体滑动                   │
│                                                         │
│  2. 数据结构选择                                        │
│     • 哈希集合：判断元素是否存在                         │
│     • 哈希表：统计元素出现次数                           │
│     • 双端队列：维护窗口最大/最小值                      │
│                                                         │
│  3. 关键操作                                            │
│     • 扩展：right++，加入新元素                          │
│     • 收缩：left++，移除旧元素                           │
│     • 更新状态：维护窗口内信息                           │
│                                                         │
│  4. 适用场景                                            │
│     • 子串/子数组问题                                   │
│     • 连续区间统计                                      │
│     • 满足条件的最优区间                                 │
│                                                         │
│  5. 时间复杂度                                          │
│     • O(n)：每个元素最多被访问两次                       │
│     • 空间复杂度取决于使用的数据结构                     │
│                                                         │
└─────────────────────────────────────────────────────────┘
)" << endl;
}

int main() {
    cout << R"(
╔════════════════════════════════════════╗
║       滑动窗口算法演示                 ║
╚════════════════════════════════════════╝
)" << endl;

    demonstrateBasicFramework();
    demoLongestSubstring();
    demoMinSubArrayLen();
    demoKDistinct();
    demoFixedWindow();
    summarizeTechniques();

    cout << "\n========== 演示完成 ==========" << endl;
    return 0;
}
