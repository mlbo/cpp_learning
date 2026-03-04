# LeetCode 215. 数组中的第K个最大元素

## 题目描述

给定整数数组 `nums` 和整数 `k`，请返回数组中第 `k` 个最大的元素。

请注意，你需要找的是数组排序后的第 `k` 个最大的元素，而不是第 `k` 个不同的元素。

## 示例

### 示例 1
```
输入: [3,2,1,5,6,4], k = 2
输出: 5
解释: 排序后为 [6,5,4,3,2,1]，第2大的是5
```

### 示例 2
```
输入: [3,2,3,1,2,4,5,5,6], k = 4
输出: 4
解释: 排序后为 [6,5,5,4,3,3,2,2,1]，第4大的是4
```

## 提示

- `1 <= k <= nums.length <= 10^5`
- `-10^4 <= nums[i] <= 10^4`

---

## 形象化理解

### 🏆 颁奖台比喻

想象一个运动会颁奖台：

```
     🥇 第一名 (最大)
   🥈 第二名    🥉 第三名
```

找第K大 = 找站在第K级领奖台上的运动员

### 🎫 VIP候车室比喻

维护一个**只能容纳K个人的VIP候车室**：

1. 候车室里有K个候选人
2. 新来的人要和房间里"最弱"的人（堆顶）比较
3. 如果更强，就挤掉最弱的人进入房间
4. 最后房间里"最弱"的人就是第K名！

```
初始：房间为空

加入3：房间 [3]
加入2：房间 [2, 3]（小顶堆，2在堆顶）
加入1：1 < 堆顶2，被拒绝
加入5：5 > 堆顶2，挤掉2，房间变成 [3, 5]
加入6：6 > 堆顶3，挤掉3，房间变成 [5, 6]
加入4：4 < 堆顶5，被拒绝

最终堆顶 = 5，即第2大元素
```

---

## 解题思路

### 方法一：小顶堆法（推荐）

**核心思想**：维护大小为K的小顶堆，堆顶就是第K大

**为什么用小顶堆？**

```
小顶堆特性：堆顶是最小值

如果堆中维护前K大的元素：
- 堆中恰好有K个"最大的候选人"
- 堆顶是这K个中最小的
- 堆顶 = 第K大！
```

**算法步骤**：

1. 创建一个大小为K的小顶堆
2. 遍历数组：
   - 如果堆未满，直接加入
   - 如果堆已满且当前元素 > 堆顶，替换堆顶
3. 返回堆顶元素

**复杂度分析**：
- 时间复杂度：O(n log k)，每个元素最多进行一次堆操作
- 空间复杂度：O(k)

```cpp
int findKthLargest(vector<int>& nums, int k) {
    // 小顶堆
    priority_queue<int, vector<int>, greater<int>> pq;

    for (int num : nums) {
        if (pq.size() < k) {
            pq.push(num);
        } else if (num > pq.top()) {
            pq.pop();
            pq.push(num);
        }
    }

    return pq.top();
}
```

---

### 方法二：排序法

**核心思想**：直接排序，取第k-1个元素

**复杂度分析**：
- 时间复杂度：O(n log n)
- 空间复杂度：O(1)

```cpp
int findKthLargest_sort(vector<int>& nums, int k) {
    sort(nums.begin(), nums.end(), greater<int>());
    return nums[k - 1];
}
```

---

### 方法三：快速选择法

**核心思想**：利用快速排序的partition思想

**形象化理解**：分组淘汰赛

```
1. 选一个人当"标准"（pivot）
2. 比他强的站左边，比他弱的站右边
3. 看标准人的位置：
   - 如果正好是第K个位置，找到答案！
   - 如果在第K个位置的左边，去右边继续找
   - 如果在第K个位置的右边，去左边继续找
```

**复杂度分析**：
- 时间复杂度：平均O(n)，最坏O(n²)
- 空间复杂度：O(1)

```cpp
int findKthLargest_quickselect(vector<int>& nums, int k) {
    int n = nums.size();
    return quickSelect(nums, 0, n - 1, n - k);
}

int quickSelect(vector<int>& nums, int left, int right, int k) {
    if (left == right) return nums[left];

    int pivotIndex = partition(nums, left, right);

    if (pivotIndex == k) {
        return nums[pivotIndex];
    } else if (pivotIndex < k) {
        return quickSelect(nums, pivotIndex + 1, right, k);
    } else {
        return quickSelect(nums, left, pivotIndex - 1, k);
    }
}
```

---

## 方法对比

| 方法 | 时间复杂度 | 空间复杂度 | 适用场景 |
|------|-----------|-----------|---------|
| 小顶堆 | O(n log k) | O(k) | k << n 时最优 |
| 排序 | O(n log n) | O(1) | 代码简单，k接近n时 |
| 快速选择 | O(n) 平均 | O(1) | 面试加分项 |

**选择建议**：
- 面试首选：小顶堆法（代码简洁，时间复杂度优秀）
- 加分项：快速选择（需要理解partition）
- 快速编码：排序法（代码最短）

---

## 关键点

1. **为什么找第K大用小顶堆？**
   - 小顶堆堆顶是堆中最小值
   - 维护前K大的元素，堆顶就是第K大

2. **堆大小限制**
   - 保持堆大小为K，超过K时弹出堆顶

3. **快速选择的随机化**
   - 随机选择pivot可以避免最坏情况

---

## 相关题目

- [LeetCode 215. 数组中的第K个最大元素](https://leetcode.cn/problems/kth-largest-element-in-an-array/)
- [LeetCode 347. 前K个高频元素](https://leetcode.cn/problems/top-k-frequent-elements/)
- [LeetCode 703. 数据流中的第K大元素](https://leetcode.cn/problems/kth-largest-element-in-a-stream/)

---

## 运行测试

```bash
cd build
./test_leetcode215
```
