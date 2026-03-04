# LeetCode 21: 合并两个有序链表

## 问题描述

将两个升序链表合并为一个新的**升序**链表并返回。新链表是通过拼接给定的两个链表的所有节点组成的。

## 示例

```
输入：l1 = [1,2,4], l2 = [1,3,4]
输出：[1,1,2,3,4,4]

输入：l1 = [], l2 = []
输出：[]

输入：l1 = [], l2 = [0]
输出：[0]
```

## 解题思路

### 方法一：迭代（使用虚拟头节点）

**核心思想**：使用虚拟头节点统一处理边界条件

```
步骤：
1. 创建虚拟头节点 dummy，tail 指针指向 dummy
2. 比较 list1 和 list2 的当前节点值
3. 将较小的节点接到 tail 后面
4. 移动相应指针
5. 处理剩余节点
```

**图示**：
```
初始状态：
dummy -> tail
list1: 1 -> 2 -> 4
list2: 1 -> 3 -> 4

步骤1（1 <= 1）：
dummy -> 1 -> tail
list1: 2 -> 4
list2: 1 -> 3 -> 4

步骤2（1 < 2）：
dummy -> 1 -> 1 -> tail
list1: 2 -> 4
list2: 3 -> 4

最终：
dummy -> 1 -> 1 -> 2 -> 3 -> 4 -> 4
```

### 方法二：递归

**核心思想**：分治思想，每次选择较小的头节点

```
merge(list1, list2):
    if list1 为空：返回 list2
    if list2 为空：返回 list1
    if list1.val <= list2.val:
        list1.next = merge(list1.next, list2)
        return list1
    else:
        list2.next = merge(list1, list2.next)
        return list2
```

## 代码实现

### 迭代解法

```cpp
ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
    ListNode dummy(0);  // 虚拟头节点
    ListNode* tail = &dummy;
    
    while (list1 && list2) {
        if (list1->val <= list2->val) {
            tail->next = list1;
            list1 = list1->next;
        } else {
            tail->next = list2;
            list2 = list2->next;
        }
        tail = tail->next;
    }
    
    tail->next = list1 ? list1 : list2;
    return dummy.next;
}
```

### 递归解法

```cpp
ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
    if (!list1) return list2;
    if (!list2) return list1;
    
    if (list1->val <= list2->val) {
        list1->next = mergeTwoLists(list1->next, list2);
        return list1;
    } else {
        list2->next = mergeTwoLists(list1, list2->next);
        return list2;
    }
}
```

## 复杂度分析

| 解法 | 时间复杂度 | 空间复杂度 |
|------|-----------|-----------|
| 迭代 | O(n+m) | O(1) |
| 递归 | O(n+m) | O(n+m) 递归栈 |

## 关键技巧

1. **虚拟头节点**：避免对空链表的特殊处理
2. **指针移动**：正确更新 tail 和 list1/list2
3. **边界处理**：剩余节点直接连接

## 拓展

- LeetCode 23：合并K个有序链表
- LeetCode 148：排序链表（归并排序）
