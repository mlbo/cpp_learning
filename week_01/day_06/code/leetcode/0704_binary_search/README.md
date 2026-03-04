# LeetCode 704. 二分查找

## 题目描述

给定一个 `n` 个元素有序的（升序）整型数组 `nums` 和一个目标值 `target`，写一个函数搜索 `nums` 中的 `target`，如果目标值存在返回下标，否则返回 `-1`。

## 示例

### 示例 1
```
输入: nums = [-1,0,3,5,9,12], target = 9
输出: 4
解释: 9 出现在 nums 中并且下标为 4
```

### 示例 2
```
输入: nums = [-1,0,3,5,9,12], target = 2
输出: -1
解释: 2 不存在 nums 中因此返回 -1
```

## 提示

1. 你可以假设 `nums` 中的所有元素是不重复的
2. `n` 将在 `[1, 10000]` 之间
3. `nums` 的每个元素都将在 `[-9999, 9999]` 之间

## 解题思路

### 方法：二分查找

这道题是二分查找的经典应用，使用标准二分查找模板即可解决。

**算法步骤**：
1. 初始化左右边界 `left = 0`, `right = n - 1`
2. 循环条件：`left <= right`
3. 计算中间位置 `mid = left + (right - left) / 2`（防溢出）
4. 比较 `nums[mid]` 与 `target`：
   - 相等：返回 `mid`
   - 小于：`left = mid + 1`
   - 大于：`right = mid - 1`
5. 循环结束未找到，返回 `-1`

## 代码实现

```cpp
int search(vector<int>& nums, int target) {
    int left = 0, right = nums.size() - 1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;  // 防溢出
        
        if (nums[mid] == target) {
            return mid;
        } else if (nums[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    return -1;
}
```

## 复杂度分析

| 复杂度 | 值 | 说明 |
|--------|-----|------|
| 时间复杂度 | O(log n) | 每次迭代将搜索范围缩小一半 |
| 空间复杂度 | O(1) | 只使用常数额外空间 |

## 关键点

1. **循环条件**：`left <= right`，使用闭区间
2. **mid计算**：使用 `left + (right - left) / 2` 防止整数溢出
3. **边界更新**：`left = mid + 1` 和 `right = mid - 1`，确保每次搜索范围都在缩小

## 运行测试

```bash
cd week_01/day_06
./build_and_run.sh
```

## 相关题目

- [35. 搜索插入位置](https://leetcode.cn/problems/search-insert-position/)
- [69. x 的平方根](https://leetcode.cn/problems/sqrtx/)
- [367. 有效的完全平方数](https://leetcode.cn/problems/valid-perfect-square/)
