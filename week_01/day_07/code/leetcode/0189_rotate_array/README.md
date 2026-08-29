# LeetCode 189: 轮转数组 (Rotate Array)

## 问题描述

给定一个整数数组 `nums`，将数组中的元素向右轮转 `k` 个位置。LeetCode 原题给出非负 `k`，本课程还把负 `k` 明确定义为向左轮转。

### 示例

```
输入: nums = [1,2,3,4,5,6,7], k = 3
输出: [5,6,7,1,2,3,4]

解释:
向右轮转 1 步: [7,1,2,3,4,5,6]
向右轮转 2 步: [6,7,1,2,3,4,5]
向右轮转 3 步: [5,6,7,1,2,3,4]
```

---

## 解法分析

四种解法共用同一个归一化契约：空数组的位移量为 `0`，其他输入先检查长度可以用 `int` 接口表示，再在 `int64_t` 中取模并把负余数换算成向右位移。

```cpp
size_t normalized_shift(const vector<int>& nums, int k) {
    if (nums.empty()) return 0;
    const int n = week01::checked_index(nums.size());
    int64_t shift = static_cast<int64_t>(k) % n;
    if (shift < 0) shift += n;
    return static_cast<size_t>(shift);
}
```

### 解法一：数组翻转法 ⭐推荐

**核心思想**：通过三次翻转实现轮转。

```
原始数组:      [1, 2, 3, 4, 5, 6, 7]
翻转整个数组:   [7, 6, 5, 4, 3, 2, 1]
翻转前k个:     [5, 6, 7, 4, 3, 2, 1]
翻转剩余:      [5, 6, 7, 1, 2, 3, 4]
```

```cpp
void rotate(vector<int>& nums, int k) {
    const size_t shift = normalized_shift(nums, k);
    if (shift == 0) return;
    const auto offset = static_cast<vector<int>::difference_type>(shift);
    
    reverse(nums.begin(), nums.end());
    reverse(nums.begin(), nums.begin() + offset);
    reverse(nums.begin() + offset, nums.end());
}
```

**为什么可以这样做？**

设原数组分为两部分：`A = [1,2,3,4]`, `B = [5,6,7]`
- 原数组：`A + B`
- 目标：`B + A`
- 翻转：`(A + B)^T = B^T + A^T`
- 再翻转：`(B^T)^T + (A^T)^T = B + A`

**复杂度**：
- 时间：O(n)
- 空间：O(1)

---

### 解法二：额外数组法

**核心思想**：创建临时数组，直接计算新位置。

```cpp
void rotate(vector<int>& nums, int k) {
    const size_t shift = normalized_shift(nums, k);
    if (shift == 0) return;
    vector<int> temp(nums.size());
    
    for (size_t i = 0; i < nums.size(); ++i) {
        temp[(i + shift) % nums.size()] = nums[i];
    }
    
    nums = std::move(temp);
}
```

**位置映射**：
- 原位置 `i` → 新位置 `(i + shift) % n`

**复杂度**：
- 时间：O(n)
- 空间：O(n)

---

### 解法三：环形替换法

**核心思想**：原地交换，沿着环移动元素。

```cpp
void rotate(vector<int>& nums, int k) {
    const size_t shift = normalized_shift(nums, k);
    if (shift == 0) return;
    size_t count = 0;
    
    for (size_t start = 0; count < nums.size(); ++start) {
        size_t current = start;
        int prev = nums[start];
        
        do {
            const size_t next = (current + shift) % nums.size();
            swap(nums[next], prev);
            current = next;
            ++count;
        } while (current != start);
    }
}
```

**工作原理**：

- 每次从一个起点开始，沿着 `(i + shift) % n` 的路径移动
- 直到回到起点，完成一个环
- 需要从多个起点开始，覆盖所有元素

**环的数量**：`gcd(n, shift)` 个

**复杂度**：
- 时间：O(n)
- 空间：O(1)

---

### 解法四：std::rotate

```cpp
void rotate(vector<int>& nums, int k) {
    const size_t shift = normalized_shift(nums, k);
    if (shift == 0) return;
    const auto offset = static_cast<vector<int>::difference_type>(shift);
    std::rotate(nums.begin(), nums.end() - offset, nums.end());
}
```

**复杂度**：
- 时间：O(n)
- 空间：O(1)

---

## 方法对比

| 方法 | 时间复杂度 | 空间复杂度 | 难度 | 面试推荐度 |
|------|-----------|-----------|------|-----------|
| 翻转法 | O(n) | O(1) | ⭐⭐ | ⭐⭐⭐⭐⭐ |
| 额外数组 | O(n) | O(n) | ⭐ | ⭐⭐⭐ |
| 环形替换 | O(n) | O(1) | ⭐⭐⭐ | ⭐⭐⭐ |
| std::rotate | O(n) | O(1) | ⭐ | ⭐⭐ |

---

## 边界情况

1. **|k| > n**：需要在 `int64_t` 中取模并归一化
2. **k == 0 或 k == n**：无需操作
3. **单元素或空数组**：直接返回
4. **k 是 n 的倍数**：数组不变

---

## 扩展思考

### 向左轮转

```cpp
// 向左轮转 k 位，等价于向右轮转 n-k 位
void rotateLeft(vector<int>& nums, int k) {
    if (nums.empty()) return;
    const int64_t n = week01::checked_index(nums.size());
    int64_t shift = static_cast<int64_t>(k) % n;
    if (shift < 0) shift += n;
    const auto offset = static_cast<vector<int>::difference_type>(shift);
    std::rotate(nums.begin(), nums.begin() + offset, nums.end());
}
```

### 相关问题

1. **反转字符串**：LeetCode 344
2. **反转字符串中的单词**：LeetCode 151
3. **旋转链表**：LeetCode 61

---

## 运行测试

```bash
cd week_01/day_07
./build_and_run.sh
```

---

## 参考资料

- [LeetCode 官方题解](https://leetcode.cn/problems/rotate-array/solution/)
- [数组翻转原理](https://leetcode.cn/problems/rotate-array/solutions/1224075/xuan-zhuan-shu-zu-yuan-di-zhuan-zi-fu-chuan-by-ari/)
