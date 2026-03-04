# LeetCode 34. 在排序数组中查找元素的第一个和最后一个位置

## 题目描述

给你一个按照非递减顺序排列的整数数组 `nums`，和一个目标值 `target`。请你找出给定目标值在数组中的开始位置和结束位置。

如果数组中不存在目标值 `target`，返回 `[-1, -1]`。

**要求**：必须设计并实现时间复杂度为 O(log n) 的算法解决此问题。

## 示例

### 示例 1
```
输入：nums = [5,7,7,8,8,10], target = 8
输出：[3,4]
```

### 示例 2
```
输入：nums = [5,7,7,8,8,10], target = 6
输出：[-1,-1]
```

### 示例 3
```
输入：nums = [], target = 0
输出：[-1,-1]
```

## 提示

- `0 <= nums.length <= 10^5`
- `-10^9 <= nums[i] <= 10^9`
- `nums` 是一个非递减数组
- `-10^9 <= target <= 10^9`

## 解题思路

### 方法：两次二分查找

这道题需要找到目标值的左右边界，可以使用两次二分查找：
1. 第一次二分查找：找左边界（第一个等于 target 的位置）
2. 第二次二分查找：找右边界（最后一个等于 target 的位置）

### 查找左边界

```
使用左闭右开区间 [left, right)
找到第一个 >= target 的位置
检查该位置是否等于 target
```

### 查找右边界

```
使用左闭右开区间 [left, right)
找到第一个 > target 的位置
返回 left - 1（即最后一个 <= target 的位置）
检查该位置是否等于 target
```

## 代码实现

```cpp
vector<int> searchRange(vector<int>& nums, int target) {
    // 查找左边界
    int leftBound = findLeft(nums, target);
    if (leftBound == -1) {
        return {-1, -1};
    }
    
    // 查找右边界
    int rightBound = findRight(nums, target);
    
    return {leftBound, rightBound};
}

int findLeft(vector<int>& nums, int target) {
    int left = 0, right = nums.size();
    
    while (left < right) {
        int mid = left + (right - left) / 2;
        
        if (nums[mid] >= target) {
            right = mid;
        } else {
            left = mid + 1;
        }
    }
    
    if (left == nums.size() || nums[left] != target) {
        return -1;
    }
    return left;
}

int findRight(vector<int>& nums, int target) {
    int left = 0, right = nums.size();
    
    while (left < right) {
        int mid = left + (right - left) / 2;
        
        if (nums[mid] <= target) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    
    if (left == 0 || nums[left - 1] != target) {
        return -1;
    }
    return left - 1;
}
```

## 复杂度分析

| 复杂度 | 值 | 说明 |
|--------|-----|------|
| 时间复杂度 | O(log n) | 两次二分查找，每次 O(log n) |
| 空间复杂度 | O(1) | 只使用常数额外空间 |

## 图解算法

```
示例：nums = [5,7,7,8,8,8,10], target = 8

查找左边界：
┌───┬───┬───┬───┬───┬───┬───┐
│ 5 │ 7 │ 7 │ 8 │ 8 │ 8 │ 10│
└───┴───┴───┴───┴───┴───┴───┘
  0   1   2   3   4   5   6
                  ↑
              第一个 >= 8 的位置
              返回 3

查找右边界：
┌───┬───┬───┬───┬───┬───┬───┐
│ 5 │ 7 │ 7 │ 8 │ 8 │ 8 │ 10│
└───┴───┴───┴───┴───┴───┴───┘
  0   1   2   3   4   5   6
                          ↑
                  第一个 > 8 的位置
                  返回 6-1=5

最终结果：[3, 5]
```

## 关键点

1. **左右边界使用不同模板**
   - 左边界：找第一个 >= target 的位置
   - 右边界：找第一个 > target 的位置，然后减1

2. **边界检查**
   - 左边界：检查是否超出数组范围
   - 右边界：检查是否为0（数组第一个元素之前）

3. **存在性检查**
   - 找到位置后，需要检查该位置的值是否等于 target

## 运行测试

```bash
cd week_01/day_06
./build_and_run.sh
```

## 相关题目

- [704. 二分查找](https://leetcode.cn/problems/binary-search/)
- [35. 搜索插入位置](https://leetcode.cn/problems/search-insert-position/)
- [69. x 的平方根](https://leetcode.cn/problems/sqrtx/)
