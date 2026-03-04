# LeetCode 3: 无重复字符的最长子串

## 题目描述

给定一个字符串 `s`，请你找出其中不含有重复字符的 **最长子串** 的长度。

## 示例

**示例 1:**
```
输入: s = "abcabcbb"
输出: 3
解释: 因为无重复字符的最长子串是 "abc"，所以其长度为 3。
```

**示例 2:**
```
输入: s = "bbbbb"
输出: 1
解释: 因为无重复字符的最长子串是 "b"，所以其长度为 1。
```

**示例 3:**
```
输入: s = "pwwkew"
输出: 3
解释: 因为无重复字符的最长子串是 "wke"，所以其长度为 3。
     请注意，你的答案必须是 子串 的长度，"pwke" 是一个子序列，不是子串。
```

## 解题思路

### 方法一：滑动窗口 + 哈希集合

```
┌─────────────────────────────────────────────────────────────┐
│                     滑动窗口算法流程                          │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  初始化: left = 0, window = {}, maxLen = 0                  │
│                                                             │
│  ┌─────────────────────────────────────────────────────┐   │
│  │  for right in 0..n-1:                               │   │
│  │      while s[right] in window:  // 重复字符          │   │
│  │          window.remove(s[left])                      │   │
│  │          left++                                      │   │
│  │                                                     │   │
│  │      window.add(s[right])     // 扩展窗口            │   │
│  │      maxLen = max(maxLen, right - left + 1)          │   │
│  └─────────────────────────────────────────────────────┘   │
│                                                             │
│  返回: maxLen                                               │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

**示例执行过程（s = "abcabcbb"）：**

```
Step 1: right=0, 'a' 不在窗口
        窗口 = "a", 长度 = 1, maxLen = 1

Step 2: right=1, 'b' 不在窗口
        窗口 = "ab", 长度 = 2, maxLen = 2

Step 3: right=2, 'c' 不在窗口
        窗口 = "abc", 长度 = 3, maxLen = 3

Step 4: right=3, 'a' 在窗口中
        收缩: 移除 'a', 窗口 = "bc"
        加入 'a': 窗口 = "bca", 长度 = 3

Step 5: right=4, 'b' 在窗口中
        收缩: 移除 'b','c', 窗口 = "a"
        加入 'b': 窗口 = "ab", 长度 = 2

...继续处理

最终结果: maxLen = 3
```

### 方法二：滑动窗口 + 数组优化

使用固定大小的数组代替哈希集合：

```cpp
bool inWindow[128] = {false};  // ASCII字符集

for (int right = 0; right < s.size(); ++right) {
    while (inWindow[s[right]]) {
        inWindow[s[left++]] = false;
    }
    inWindow[s[right]] = true;
    maxLen = max(maxLen, right - left + 1);
}
```

### 方法三：滑动窗口 + 哈希映射（优化版）

直接记录字符位置，避免逐个移动：

```cpp
unordered_map<char, int> lastPos;  // 字符最后出现的位置+1

for (int right = 0; right < s.size(); ++right) {
    char c = s[right];
    
    // 如果字符出现过且在窗口内，直接跳转
    if (lastPos.count(c) && lastPos[c] > left) {
        left = lastPos[c];
    }
    
    lastPos[c] = right + 1;  // 记录位置+1
    maxLen = max(maxLen, right - left + 1);
}
```

## 代码实现

```cpp
// 方法一：滑动窗口 + 哈希集合
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
```

## 复杂度分析

| 方法 | 时间复杂度 | 空间复杂度 |
|------|-----------|-----------|
| 滑动窗口+集合 | O(n) | O(min(m,n)) |
| 滑动窗口+数组 | O(n) | O(m) |
| 滑动窗口+映射 | O(n) | O(min(m,n)) |
| 暴力法 | O(n³) | O(min(m,n)) |

> m 为字符集大小，n 为字符串长度

## 关键点

1. **子串 vs 子序列**：
   - 子串：连续的字符序列
   - 子序列：可以不连续

2. **滑动窗口适用条件**：
   - 窗口内的元素需要满足某种性质
   - 窗口扩展/收缩的方向一致

3. **优化技巧**：
   - 使用数组代替哈希集合（字符集固定时）
   - 记录位置直接跳转（减少移动次数）

## 边界情况

- 空字符串 → 返回 0
- 单字符 → 返回 1
- 全部相同字符 → 返回 1
- 全部不同字符 → 返回字符串长度

## 相关题目

- LeetCode 209: 长度最小的子数组
- LeetCode 76: 最小覆盖子串
- LeetCode 438: 找到字符串中所有字母异位词
- LeetCode 567: 字符串的排列
