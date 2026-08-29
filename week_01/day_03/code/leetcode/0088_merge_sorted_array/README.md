# LeetCode 88: 合并两个有序数组

## 题目描述

给你两个按**非递减顺序**排列的整数数组 `nums1` 和 `nums2`，另有两个整数 `m` 和 `n` ，分别表示 `nums1` 和 `nums2` 中的元素数目。

请你**合并** `nums2` 到 `nums1` 中，使 `nums1` 成为一个有序数组。

### 初始条件
- `nums1` 的长度为 `m + n`，前 `m` 个元素为有效元素，后 `n` 个元素为 0（应忽略）
- `nums2` 的长度为 `n`

### 要求
- 时间复杂度 O(m + n)
- 空间复杂度 O(1)（原地操作）

---

## 解题思路

### 方法一：逆向双指针（推荐）

#### 核心思想
利用 `nums1` 尾部的 `n` 个空位，从后往前填充，避免覆盖未处理的元素。

#### 算法流程

```
初始化:
  先验证 m、n 非负且不超过两个缓冲区
  p1 = m        // nums1 未处理区间 [0, p1)
  p2 = n        // nums2 未处理区间 [0, p2)
  output = m+n  // 待填区间 [0, output)

循环 (p1 > 0 && p2 > 0):
  如果 nums1[p1-1] > nums2[p2-1]:
    p1--, output--, nums1[output] = nums1[p1]
  否则:
    p2--, output--, nums1[output] = nums2[p2]

处理剩余:
  当 p2 > 0:
    先缩短 p2/output，再复制 nums2 剩余元素到 nums1 前面
```

#### 为什么逆向？
- 正向合并需要额外空间存储 `nums1` 的原始数据
- 逆向合并利用 `nums1` 后面的空位，实现原地操作

### 方法二：正向双指针 + 额外空间

#### 思路
1. 复制 `nums1` 的前 `m` 个元素
2. 使用双指针从前往后合并

#### 复杂度
- 时间：O(m + n)
- 空间：O(m)

---

## 代码实现

```cpp
void merge(vector<int>& nums1, int m, const vector<int>& nums2, int n) {
    const auto [first_size, second_size] =
        validate_lengths(nums1, m, nums2, n);
    size_t p1 = first_size;                   // 未处理区间 [0, p1)
    size_t p2 = second_size;                  // 未处理区间 [0, p2)
    size_t output = first_size + second_size; // 待填充区间 [0, output)

    // 从后往前比较填充
    while (p1 > 0 && p2 > 0) {
        if (nums1[p1 - 1] > nums2[p2 - 1]) {
            --p1;
            --output;
            nums1[output] = nums1[p1];
        } else {
            --p2;
            --output;
            nums1[output] = nums2[p2];
        }
    }

    // 处理 nums2 剩余元素
    while (p2 > 0) {
        --p2;
        --output;
        nums1[output] = nums2[p2];
    }
}
```

`validate_lengths` 先拒绝负长度，并验证 `m`、`n` 没有超出两个输入缓冲区。内部用“剩余元素数量”而不是 `-1` 哨兵表示空区间，因此空输入不依赖有符号下标，也不会先计算可能溢出的 `m + n - 1`。

---

## 图解示例

### 示例：nums1 = [1,2,3,0,0,0], m = 3; nums2 = [2,5,6], n = 3

```
初始状态:
nums1: [1, 2, 3, 0, 0, 0]
              p1        p
nums2: [2, 5, 6]
              p2

步骤 1: 3 < 6 → 放入 6
nums1: [1, 2, 3, 0, 0, 6]
              p1     p
nums2: [2, 5, 6]
           p2

步骤 2: 3 < 5 → 放入 5
nums1: [1, 2, 3, 0, 5, 6]
              p1  p
nums2: [2, 5, 6]
        p2

步骤 3: 3 > 2 → 放入 3
nums1: [1, 2, 3, 3, 5, 6]
           p1 p
nums2: [2, 5, 6]
        p2

步骤 4: 2 = 2 → 放入 2 (nums2 的)
nums1: [1, 2, 2, 3, 5, 6]
        p1 p
nums2: [2, 5, 6]
       p2 (已完成)

步骤 5: nums2 完成，nums1 剩余元素已在正确位置
结果: [1, 2, 2, 3, 5, 6]
```

---

## 复杂度分析

| 指标 | 逆向双指针 | 正向+额外空间 |
|------|-----------|--------------|
| 时间复杂度 | O(m + n) | O(m + n) |
| 空间复杂度 | O(1) | O(m) |

---

## 关键点

1. **逆向思维**：从后往前填充，避免覆盖问题
2. **边界处理**：只需要处理 `nums2` 剩余元素
3. **优化空间**：原地操作，空间复杂度 O(1)

---

## 相关题目

- LeetCode 21: 合并两个有序链表
- LeetCode 23: 合并 K 个有序链表
- LeetCode 977: 有序数组的平方
