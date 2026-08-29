# LeetCode 11: 盛最多水的容器

## 问题描述

给定一个长度为 n 的整数数组 `height`，有 n 条垂线，第 i 条线的两个端点是 `(i, 0)` 和 `(i, height[i])`。

找出其中的两条线，使得它们与 x 轴共同构成的容器可以容纳最多的水。

返回容器可以储存的最大水量。

## 示例

```
输入: height = [1,8,6,2,5,4,8,3,7]
输出: 49

图示:
     8 |    █        █         █
     7 |    █        █         █────█
     6 |    █  █     █         █    █
     5 |    █  █     █  █      █    █
     4 |    █  █     █  █  █   █    █
     3 |    █  █     █  █  █   █  █ █
     2 |    █  █  █  █  █  █   █  █ █
     1 |█   █  █  █  █  █  █   █  █ █
       └─────────────────────────────
        0  1  2  3  4  5  6  7  8

选择索引1和8，面积 = min(8,7) × (8-1) = 49
```

## 解法

### 方法一：双指针贪心

```cpp
int maxArea(vector<int>& height) {
    validate_input(height);  // 高度非负，且长度可由公开 int 接口表示
    if (height.size() < 2) return 0;
    size_t left = 0;
    size_t right = height.size() - 1;
    int64_t max_water = 0;
    
    while (left < right) {
        const int64_t h = min(height[left], height[right]);
        const int64_t width = static_cast<int64_t>(right - left);
        max_water = max(max_water, h * width);
        
        // 移动较矮的边
        if (height[left] < height[right]) {
            ++left;
        } else {
            --right;
        }
    }
    
    return week01::checked_result(max_water);
}
```

这里先验证再处理短输入，因此 `{-1}` 不会被误当成普通空结果。下标使用容器的 `size_type`，面积则在 `int64_t` 中计算；若数学结果超出题目规定的 `int` 返回范围，`checked_result` 会抛出 `overflow_error`，而不是先发生有符号整数溢出。

### 核心思想

1. **双指针从两端开始**：初始时宽度最大
2. **贪心移动策略**：每次移动较矮的边
3. **原因**：
   - 移动较高边：宽度减小，高度仍不会超过固定的矮边，所以不可能超过当前面积
   - 移动较矮边：宽度减小，但高度可能增加，面积可能增加

### 复杂度分析

| 方法 | 时间复杂度 | 空间复杂度 |
|------|-----------|-----------|
| 双指针 | O(n) | O(1) |
| 暴力法 | O(n²) | O(1) |

## 关键要点

1. **短板效应**：容器高度由较短的边决定
2. **贪心正确性**：移动较矮边不会错过最优解
3. **双指针技巧**：常用于需要从两端向中间收敛的问题
4. **接口契约**：负高度是无效输入；长度或结果无法由公开 `int` 接口表示时显式报告异常

## 相关题目

- LeetCode 42: 接雨水
- LeetCode 15: 三数之和（也使用双指针）
