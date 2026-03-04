# LeetCode 26. 删除有序数组中的重复项

## 题目描述

给你一个 **升序排列** 的数组 `nums`，请你**原地**删除重复出现的元素，使每个元素只出现一次，返回删除后数组的新长度。元素的相对顺序应该保持一致。

**要求：**
- 不要使用额外的数组空间
- 必须在原地修改输入数组
- 使用 O(1) 额外空间

## 示例

### 示例 1
```
输入：nums = [1,1,2]
输出：2, nums = [1,2,_]
解释：函数应该返回新的长度 2，原数组 nums 的前两个元素被修改为 1, 2。
不需要考虑数组中超出新长度后面的元素。
```

### 示例 2
```
输入：nums = [0,0,1,1,1,2,2,3,3,4]
输出：5, nums = [0,1,2,3,4,_,_,_,_,_]
解释：函数应该返回新的长度 5，原数组 nums 的前五个元素被修改为 0, 1, 2, 3, 4。
```

## 解题思路

### 方法一：快慢指针（推荐）

**核心思想：**
利用数组有序的特点，使用两个指针：
- **慢指针 `slow`**：指向当前有效序列的末尾
- **快指针 `fast`**：遍历整个数组

当发现新元素（不同于 `nums[slow]`）时，将其放到 `slow+1` 的位置。

```
初始状态：
[1, 1, 2, 2, 3]
 s  f

发现重复，fast 前进：
[1, 1, 2, 2, 3]
 s     f

发现新元素，复制并前进 slow：
[1, 2, 2, 2, 3]
    s  f

继续...
```

**复杂度分析：**
- 时间复杂度：O(n)，只需遍历一次数组
- 空间复杂度：O(1)，只使用两个指针变量

### 方法二：递归

递归实现同样的逻辑，但会增加递归栈的空间消耗。

**复杂度分析：**
- 时间复杂度：O(n)
- 空间复杂度：O(n)，递归栈深度

## 代码实现

### 迭代实现

```cpp
int removeDuplicates(vector<int>& nums) {
    if (nums.empty()) return 0;
    
    int slow = 0;
    for (int fast = 1; fast < nums.size(); ++fast) {
        if (nums[fast] != nums[slow]) {
            ++slow;
            nums[slow] = nums[fast];
        }
    }
    return slow + 1;
}
```

### 递归实现

```cpp
int removeDuplicatesHelper(vector<int>& nums, int slow, int fast) {
    if (fast >= nums.size()) return slow + 1;
    
    if (nums[fast] != nums[slow]) {
        ++slow;
        nums[slow] = nums[fast];
    }
    
    return removeDuplicatesHelper(nums, slow, fast + 1);
}

int removeDuplicatesRecursive(vector<int>& nums) {
    if (nums.empty()) return 0;
    return removeDuplicatesHelper(nums, 0, 1);
}
```

## 关键点

1. **数组有序**：这是本题的关键前提，保证了重复元素必定相邻
2. **原地修改**：不需要额外空间，直接在原数组上操作
3. **返回长度**：返回新长度，数组前 `length` 个元素是去重后的结果

## 扩展思考

### 如果数组无序怎么办？

需要先排序，或者使用哈希表记录已出现的元素。

```cpp
// 方法：先排序，再使用双指针
int removeDuplicatesUnsorted(vector<int>& nums) {
    sort(nums.begin(), nums.end());
    return removeDuplicates(nums);
}
```

### 如果允许最多出现 k 次？

修改比较条件，比较 `nums[fast] != nums[slow - k + 1]`。

## 相关题目

- LeetCode 27. 移除元素
- LeetCode 80. 删除有序数组中的重复项 II
- LeetCode 283. 移动零
