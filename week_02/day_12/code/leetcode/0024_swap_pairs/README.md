# LeetCode 24. 两两交换链表中的节点

## 题目描述

给定一个链表，两两交换其中相邻的节点，并返回交换后的链表。

**你不能只是单纯的改变节点内部的值，而是需要实际的进行节点交换。**

## 示例

### 示例 1
```
输入: 1->2->3->4
输出: 2->1->4->3
```

### 示例 2
```
输入: 1->2->3
输出: 2->1->3
```

### 示例 3
```
输入: []
输出: []
```

## 解题思路

### 方法1：递归法

**思路**：
1. 交换前两个节点
2. 递归处理后续链表
3. 连接交换后的部分

**代码**：
```cpp
ListNode* swapPairs_recursive(ListNode* head) {
    // 基本情况
    if (head == nullptr || head->next == nullptr) {
        return head;
    }
    
    ListNode* newHead = head->next;
    head->next = swapPairs_recursive(newHead->next);
    newHead->next = head;
    
    return newHead;
}
```

**复杂度**：
- 时间：O(n)
- 空间：O(n) 递归栈

---

### 方法2：迭代法（推荐）

**思路**：
1. 使用虚拟头节点简化边界处理
2. 成对交换节点
3. 更新指针继续处理

**交换过程图解**：
```
交换前: prev -> first -> second -> next_pair

步骤1: prev->next = second
步骤2: first->next = second->next  
步骤3: second->next = first

结果: prev -> second -> first -> next_pair
```

**代码**：
```cpp
ListNode* swapPairs_iterative(ListNode* head) {
    ListNode dummy(0);
    dummy.next = head;
    ListNode* prev = &dummy;
    
    while (prev->next && prev->next->next) {
        ListNode* first = prev->next;
        ListNode* second = prev->next->next;
        
        prev->next = second;
        first->next = second->next;
        second->next = first;
        
        prev = first;
    }
    
    return dummy.next;
}
```

**复杂度**：
- 时间：O(n)
- 空间：O(1)

## 关键点

1. **虚拟头节点**：简化边界条件处理
2. **指针操作顺序**：先保存再修改，避免丢失引用
3. **终止条件**：`prev->next && prev->next->next`
4. **奇数节点**：最后一个节点不处理

## 扩展

- 如果允许修改节点值，可以直接交换 val，但题目要求交换节点
- 类似题目：25题 K 个一组翻转链表
