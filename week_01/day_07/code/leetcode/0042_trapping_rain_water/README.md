# LeetCode 42: 接雨水 (Trapping Rain Water)

## 问题描述

给定 `n` 个非负整数表示每个宽度为 `1` 的柱子的高度图，计算按此排列的柱子，下雨之后能接多少雨水。

### 示例

```
输入: height = [0,1,0,2,1,0,1,3,2,1,2,1]
输出: 6
```

可视化：
```
        ■
    ■   ■ ■   ■
■   ■ ■ ■ ■ ■ ■ ■
0 1 0 2 1 0 1 3 2 1 2 1
```

蓝色部分为可接雨水，共6个单位。

---

## 解法分析

### 解法一：双指针法 ⭐推荐

**核心思想**：从两端向中间遍历，维护左右两侧的最大高度。

```cpp
int trap(vector<int>& height) {
    int left = 0, right = height.size() - 1;
    int left_max = 0, right_max = 0;
    int water = 0;
    
    while (left < right) {
        if (height[left] < height[right]) {
            if (height[left] >= left_max)
                left_max = height[left];
            else
                water += left_max - height[left];
            ++left;
        } else {
            if (height[right] >= right_max)
                right_max = height[right];
            else
                water += right_max - height[right];
            --right;
        }
    }
    return water;
}
```

**为什么可以这样做？**

关键洞察：对于位置 `i`，它能接的水量取决于 `min(left_max, right_max) - height[i]`。

- 如果 `height[left] < height[right]`，那么 `left_max < right_max`（或者至少 `left_max <= right_max`）
- 这意味着位置 `left` 处的储水量只取决于 `left_max`（因为右边有更高的柱子）
- 同理，如果 `height[right] <= height[left]`，位置 `right` 的储水量只取决于 `right_max`

**复杂度**：
- 时间：O(n)
- 空间：O(1)

---

### 解法二：动态规划

**核心思想**：预处理每个位置左右两侧的最大高度。

```cpp
int trap(vector<int>& height) {
    int n = height.size();
    vector<int> left_max(n), right_max(n);
    
    left_max[0] = height[0];
    for (int i = 1; i < n; ++i)
        left_max[i] = max(left_max[i-1], height[i]);
    
    right_max[n-1] = height[n-1];
    for (int i = n-2; i >= 0; --i)
        right_max[i] = max(right_max[i+1], height[i]);
    
    int water = 0;
    for (int i = 0; i < n; ++i)
        water += min(left_max[i], right_max[i]) - height[i];
    
    return water;
}
```

**复杂度**：
- 时间：O(n)
- 空间：O(n)

---

### 解法三：单调栈

**核心思想**：使用单调递减栈，按"层"计算储水量。

```cpp
int trap(vector<int>& height) {
    stack<int> st;
    int water = 0;
    
    for (int i = 0; i < height.size(); ++i) {
        while (!st.empty() && height[i] > height[st.top()]) {
            int bottom = st.top();
            st.pop();
            
            if (st.empty()) break;
            
            int distance = i - st.top() - 1;
            int bounded_height = min(height[i], height[st.top()]) - height[bottom];
            water += distance * bounded_height;
        }
        st.push(i);
    }
    return water;
}
```

**工作原理**：

1. 维护单调递减栈（存储下标）
2. 当遇到比栈顶高的柱子时，说明可能形成凹槽
3. 计算凹槽的面积：宽度 × 高度差

**复杂度**：
- 时间：O(n) - 每个元素最多入栈出栈一次
- 空间：O(n)

---

## 方法对比

| 方法 | 时间复杂度 | 空间复杂度 | 难度 | 面试推荐度 |
|------|-----------|-----------|------|-----------|
| 双指针 | O(n) | O(1) | ⭐⭐ | ⭐⭐⭐⭐⭐ |
| 动态规划 | O(n) | O(n) | ⭐⭐ | ⭐⭐⭐⭐ |
| 单调栈 | O(n) | O(n) | ⭐⭐⭐ | ⭐⭐⭐ |

---

## 扩展思考

### 变体问题

1. **接雨水 II (3D版)**：LeetCode 407
2. **容器盛水问题**：给定容器边界
3. **最大矩形**：LeetCode 84（单调栈应用）

### 相关知识点

- 双指针技巧
- 动态规划预处理
- 单调栈应用
- 空间优化技巧

---

## 运行测试

```bash
# 编译
g++ -std=c++17 -O2 test.cpp -o test_42

# 运行
./test_42
```

---

## 参考资料

- [LeetCode 官方题解](https://leetcode.cn/problems/trapping-rain-water/solution/)
- [双指针图解](https://leetcode.cn/problems/trapping-rain-water/solutions/9112/jie-yu-shui-by-leetcode/)
