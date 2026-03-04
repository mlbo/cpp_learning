# LeetCode 160: 相交链表

## 题目描述

给你两个单链表的头节点 `headA` 和 `headB`，请你找出并返回两个单链表相交的起始节点。如果两个链表不存在相交节点，返回 `null`。

图示两个链表在节点 `c1` 开始相交：

```
A:          a1 → a2
                   ↘
                     c1 → c2 → c3
                   ↗
B:    b1 → b2 → b3
```

## 解题思路

### 方法一：双指针交叉遍历（推荐）

**核心思想**：
- 指针 A 遍历完链表 A 后，跳到链表 B 继续
- 指针 B 遍历完链表 B 后，跳到链表 A 继续
- 两指针最终会在相交点相遇（或同时到达末尾 nullptr）

**数学原理**：
```
设：链表A独立部分长度 = a
    链表B独立部分长度 = b
    公共部分长度 = c

指针A走：a + c + b
指针B走：b + c + a
两者相等，必会在交点相遇！
```

**代码实现**：
```cpp
ListNode* getIntersectionNode(ListNode* headA, ListNode* headB) {
    if (!headA || !headB) return nullptr;
    
    ListNode* pA = headA;
    ListNode* pB = headB;
    
    while (pA != pB) {
        pA = (pA == nullptr) ? headB : pA->next;
        pB = (pB == nullptr) ? headA : pB->next;
    }
    
    return pA;  // 相交点或 nullptr
}
```

**复杂度分析**：
- 时间复杂度：O(m + n)
- 空间复杂度：O(1)

### 方法二：长度差法

**核心思想**：
1. 计算两个链表的长度
2. 让较长的链表先走长度差步
3. 两个指针同时前进，相遇即为交点

```cpp
ListNode* getIntersectionNodeByLength(ListNode* headA, ListNode* headB) {
    int lenA = getLength(headA);
    int lenB = getLength(headB);
    
    // 对齐起点
    ListNode* pA = headA;
    ListNode* pB = headB;
    
    if (lenA > lenB) {
        for (int i = 0; i < lenA - lenB; ++i) pA = pA->next;
    } else {
        for (int i = 0; i < lenB - lenA; ++i) pB = pB->next;
    }
    
    // 同时前进寻找交点
    while (pA && pB) {
        if (pA == pB) return pA;
        pA = pA->next;
        pB = pB->next;
    }
    
    return nullptr;
}
```

### 方法三：哈希集合法

**核心思想**：将链表 A 的所有节点地址存入哈希集合，遍历链表 B 时查找。

```cpp
ListNode* getIntersectionNodeByHash(ListNode* headA, ListNode* headB) {
    unordered_set<ListNode*> visited;
    
    while (headA) {
        visited.insert(headA);
        headA = headA->next;
    }
    
    while (headB) {
        if (visited.count(headB)) return headB;
        headB = headB->next;
    }
    
    return nullptr;
}
```

**复杂度分析**：
- 时间复杂度：O(m + n)
- 空间复杂度：O(m) 或 O(n)

## 三种方法对比

| 方法 | 时间复杂度 | 空间复杂度 | 推荐程度 |
|------|-----------|-----------|---------|
| 双指针交叉 | O(m+n) | O(1) | ⭐⭐⭐⭐⭐ |
| 长度差法 | O(m+n) | O(1) | ⭐⭐⭐⭐ |
| 哈希集合 | O(m+n) | O(m) | ⭐⭐⭐ |

## 关键技巧

1. **双指针的思想**：将两个不同长度的链表问题转化为相同长度问题
2. **边界处理**：注意空链表、无交点的情况
3. **指针比较**：比较的是节点地址（指针值），而不是节点值

## 测试用例

```cpp
// 测试1：有交点
// A: 4 -> 1 -> 8 -> 4 -> 5
// B: 5 -> 6 -> 1 -> 8 -> 4 -> 5
// 交点值: 8

// 测试2：无交点
// A: 2 -> 6 -> 4
// B: 1 -> 5
// 结果: nullptr

// 测试3：交点在头节点
// A: 0 -> 1 -> 2 -> 3
// B: 5 -> 1 -> 2 -> 3
// 交点值: 1
```

## 相关题目

- LeetCode 141: 环形链表
- LeetCode 142: 环形链表 II
- LeetCode 21: 合并两个有序链表
