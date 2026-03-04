# LeetCode 15: 三数之和

## 问题描述

给定一个整数数组 `nums`，判断是否存在三元组 `[nums[i], nums[j], nums[k]]` 满足 `i != j != k` 且 `nums[i] + nums[j] + nums[k] == 0`。

返回所有不重复的三元组。

## 示例

```
输入: nums = [-1,0,1,2,-1,-4]
输出: [[-1,-1,2],[-1,0,1]]

解释:
排序后: [-4, -1, -1, 0, 1, 2]

i=0: -4, 找两数之和为4
     left=1, right=5: -1+2=1 < 4, left++
     left=2, right=5: -1+2=1 < 4, left++
     left=3, right=5: 0+2=2 < 4, left++
     left=4, right=5: 1+2=3 < 4, left++
     left=5, right=5: 结束

i=1: -1, 找两数之和为1
     left=2, right=5: -1+2=1 ✓ 找到 [-1,-1,2]
     去重后 left=3, right=4
     left=3, right=4: 0+1=1 ✓ 找到 [-1,0,1]
     left=4, right=3: 结束

i=2: -1, 与i=1相同，跳过（去重）
```

## 解法

### 方法：排序 + 双指针 + 去重

```cpp
vector<vector<int>> threeSum(vector<int>& nums) {
    vector<vector<int>> result;
    sort(nums.begin(), nums.end());
    
    for (int i = 0; i < nums.size() - 2; i++) {
        // 去重1: 跳过相同的第一个数
        if (i > 0 && nums[i] == nums[i-1]) continue;
        
        // 剪枝
        if (nums[i] > 0) break;
        
        int left = i + 1, right = nums.size() - 1;
        while (left < right) {
            int sum = nums[i] + nums[left] + nums[right];
            
            if (sum == 0) {
                result.push_back({nums[i], nums[left], nums[right]});
                
                // 去重2: 跳过相同的第二个数
                while (left < right && nums[left] == nums[left+1]) left++;
                // 去重3: 跳过相同的第三个数
                while (left < right && nums[right] == nums[right-1]) right--;
                
                left++;
                right--;
            } else if (sum < 0) {
                left++;
            } else {
                right--;
            }
        }
    }
    
    return result;
}
```

## 去重技巧

### 三层去重策略

```
排序后数组: [-4, -1, -1, 0, 0, 1, 1, 2, 2]

去重1 - 第一个数 (i):
┌───┬───┬───┬───┬───┬───┬───┬───┬───┐
│ -4│ -1│ -1│ 0 │ 0 │ 1 │ 1 │ 2 │ 2 │
└───┴───┴───┴───┴───┴───┴───┴───┴───┘
      ↑   ↑
      i  跳过(nums[i]==nums[i-1])

去重2 - 第二个数 (left):
当找到 [-1, 0, 1] 后，跳过重复的0
┌───┬───┬───┬───┬───┬───┬───┐
│ -1│ 0 │ 0 │ 0 │ 1 │ 1 │ 2 │
└───┴───┴───┴───┴───┴───┴───┘
          ↑       ↑
        left    跳过

去重3 - 第三个数 (right):
同样跳过重复的值
```

### 为什么必须先排序？

1. **双指针需要有序数组**：才能根据sum决定移动方向
2. **去重需要相同元素相邻**：才能高效跳过重复
3. **剪枝优化**：`nums[i] > 0` 时可以提前终止

## 复杂度分析

| 操作 | 时间复杂度 |
|------|-----------|
| 排序 | O(n log n) |
| 外层循环 | O(n) |
| 内层双指针 | O(n) |
| **总计** | **O(n²)** |

空间复杂度: O(1)（不考虑输出）

## 常见错误

### 错误1: 去重位置不对

```cpp
// ❌ 错误: 在记录结果前去重
if (nums[left] == nums[left+1]) left++;  // 可能漏掉有效解

// ✅ 正确: 在记录结果后去重
result.push_back(...);
while (left < right && nums[left] == nums[left+1]) left++;
```

### 错误2: 没有检查边界

```cpp
// ❌ 错误: 没有检查数组大小
for (int i = 0; i < nums.size() - 2; i++)  // size < 2时下溢

// ✅ 正确: 先检查边界
if (nums.size() < 3) return {};
```

### 错误3: 溢出问题

```cpp
// ❌ 可能溢出
int sum = nums[i] + nums[left] + nums[right];

// ✅ 使用更大的类型
long long sum = (long long)nums[i] + nums[left] + nums[right];
```

## 相关题目

- LeetCode 1: 两数之和
- LeetCode 16: 最接近的三数之和
- LeetCode 18: 四数之和
