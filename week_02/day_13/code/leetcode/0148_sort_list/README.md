# LeetCode 148: 排序链表

## 题目描述

给你链表的头结点 `head`，将其按**升序**排列并返回**排序后的链表**。

**进阶**：你可以在 O(n log n) 时间复杂度和常数级空间复杂度下完成吗？

## 示例

```
输入: head = [4,2,1,3]
输出: [1,2,3,4]

输入: head = [-1,5,3,4,0]
输出: [-1,0,3,4,5]
```

## 解题思路

### 为什么选择归并排序？

| 排序算法 | 时间复杂度 | 空间复杂度 | 是否适合链表 |
|---------|-----------|-----------|------------|
| 快速排序 | O(n log n) | O(log n) | ❌ 需要随机访问 |
| 堆排序 | O(n log n) | O(1) | ❌ 需要随机访问 |
| 归并排序 | O(n log n) | O(1) | ✅ 顺序访问友好 |

**归并排序的优势**：
1. 不需要随机访问元素
2. 链表合并操作简单高效
3. 空间复杂度可优化为 O(1)

### 方法一：归并排序（自顶向下）

**核心步骤**：
```
1. 找中点（快慢指针）
2. 断开链表
3. 递归排序两部分
4. 合并有序链表
```

**图解**：
```
原始: 4 -> 2 -> 1 -> 3

第一步：找中点并分割
    [4, 2]  |  [1, 3]

第二步：递归排序
    [2, 4]  |  [1, 3]

第三步：合并
    [1, 2, 3, 4]
```

**代码实现**：
```cpp
ListNode* sortList(ListNode* head) {
    if (head == nullptr || head->next == nullptr) {
        return head;
    }
    
    // 找中点
    ListNode* slow = head;
    ListNode* fast = head;
    ListNode* prev = nullptr;
    
    while (fast && fast->next) {
        prev = slow;
        slow = slow->next;
        fast = fast->next->next;
    }
    
    // 断开链表
    ListNode* rightHead = slow;
    prev->next = nullptr;
    
    // 递归排序
    ListNode* left = sortList(head);
    ListNode* right = sortList(rightHead);
    
    // 合并
    return merge(left, right);
}

ListNode* merge(ListNode* l1, ListNode* l2) {
    ListNode dummy(0);
    ListNode* tail = &dummy;
    
    while (l1 && l2) {
        if (l1->val <= l2->val) {
            tail->next = l1;
            l1 = l1->next;
        } else {
            tail->next = l2;
            l2 = l2->next;
        }
        tail = tail->next;
    }
    
    tail->next = l1 ? l1 : l2;
    return dummy.next;
}
```

**复杂度分析**：
- 时间复杂度：O(n log n)
- 空间复杂度：O(log n) — 递归栈空间

### 方法二：归并排序（自底向上）

**核心思想**：
- 从长度为 1 的子链表开始，两两合并
- 逐步增加子链表长度：1 → 2 → 4 → 8 → ...

**图解**：
```
原始: 4 -> 2 -> 1 -> 3

step=1: 合并相邻长度为1的段
    [4] [2] -> [2, 4]
    [1] [3] -> [1, 3]
    结果: 2 -> 4 -> 1 -> 3

step=2: 合并相邻长度为2的段
    [2, 4] [1, 3] -> [1, 2, 3, 4]
    结果: 1 -> 2 -> 3 -> 4
```

**代码实现**：
```cpp
ListNode* sortListBottomUp(ListNode* head) {
    if (!head || !head->next) return head;
    
    // 计算长度
    int length = 0;
    ListNode* node = head;
    while (node) {
        ++length;
        node = node->next;
    }
    
    ListNode dummy(0);
    dummy.next = head;
    
    // 逐步增大步长
    for (int step = 1; step < length; step *= 2) {
        ListNode* prev = &dummy;
        ListNode* curr = dummy.next;
        
        while (curr) {
            ListNode* left = curr;
            ListNode* right = split(left, step);
            curr = split(right, step);
            
            ListNode* merged = merge(left, right);
            prev->next = merged;
            
            while (prev->next) {
                prev = prev->next;
            }
        }
    }
    
    return dummy.next;
}
```

**复杂度分析**：
- 时间复杂度：O(n log n)
- 空间复杂度：O(1) — 真正的常数空间

## 两种方法对比

| 特性 | 自顶向下 | 自底向上 |
|------|---------|---------|
| 实现难度 | 较简单 | 较复杂 |
| 空间复杂度 | O(log n) | O(1) |
| 代码可读性 | 好 | 一般 |
| 推荐程度 | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐ |

## 关键技巧总结

### 1. 快慢指针找中点

```cpp
ListNode* slow = head;
ListNode* fast = head;
ListNode* prev = nullptr;

while (fast && fast->next) {
    prev = slow;        // 记录中点前一个节点
    slow = slow->next;  // 慢指针走一步
    fast = fast->next->next;  // 快指针走两步
}
// slow 指向中点（偶数时指向后一半第一个）
// prev 指向中点前一个节点
```

### 2. 合并有序链表

```cpp
ListNode* merge(ListNode* l1, ListNode* l2) {
    ListNode dummy(0);
    ListNode* tail = &dummy;
    
    while (l1 && l2) {
        if (l1->val <= l2->val) {
            tail->next = l1;
            l1 = l1->next;
        } else {
            tail->next = l2;
            l2 = l2->next;
        }
        tail = tail->next;
    }
    
    tail->next = l1 ? l1 : l2;
    return dummy.next;
}
```

### 3. 虚拟头节点技巧

使用虚拟头节点可以避免处理头节点为空的边界情况。

## 测试用例

```cpp
// 测试1: 普通乱序
输入: [4,2,1,3]
输出: [1,2,3,4]

// 测试2: 含负数
输入: [-1,5,3,4,0]
输出: [-1,0,3,4,5]

// 测试3: 空链表
输入: []
输出: []

// 测试4: 单节点
输入: [1]
输出: [1]

// 测试5: 有重复
输入: [3,1,2,3,1]
输出: [1,1,2,3,3]
```

## 相关题目

- LeetCode 21: 合并两个有序链表
- LeetCode 23: 合并K个有序链表
- LeetCode 147: 对链表进行插入排序
- LeetCode 148: 排序链表
