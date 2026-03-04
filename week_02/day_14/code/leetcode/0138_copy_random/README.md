# LeetCode 138. 随机链表的复制

## 题目描述

给你一个长度为 `n` 的链表，每个节点包含一个额外增加的随机指针 `random`，该指针可以指向链表中的任何节点或空节点。请构造这个链表的深拷贝。

## 解题思路

### 方法一：哈希表法

**思路：**
1. 第一遍遍历：创建所有新节点，建立原节点到新节点的映射
2. 第二遍遍历：设置新节点的 `next` 和 `random` 指针

**复杂度：**
- 时间复杂度：O(n)
- 空间复杂度：O(n)

### 方法二：节点拆分法（推荐）

**思路：**
1. 在每个原节点后插入复制节点
2. 设置复制节点的 `random` 指针
3. 拆分链表

**复杂度：**
- 时间复杂度：O(n)
- 空间复杂度：O(1)（不计返回结果）

## 代码示例

```cpp
// 方法二：节点拆分法
Node* copyRandomList(Node* head) {
    if (!head) return nullptr;

    // 第一步：插入复制节点
    Node* curr = head;
    while (curr) {
        Node* copy = new Node(curr->val);
        copy->next = curr->next;
        curr->next = copy;
        curr = copy->next;
    }

    // 第二步：设置random指针
    curr = head;
    while (curr) {
        if (curr->random) {
            curr->next->random = curr->random->next;
        }
        curr = curr->next->next;
    }

    // 第三步：拆分链表
    Node dummy(0);
    Node* copyCurr = &dummy;
    curr = head;
    while (curr) {
        copyCurr->next = curr->next;
        copyCurr = copyCurr->next;
        curr->next = curr->next->next;
        curr = curr->next;
    }

    return dummy.next;
}
```

## 关键点

1. **哈希表法**简单直观，但需要 O(n) 额外空间
2. **节点拆分法**巧妙地利用原链表结构，实现 O(1) 空间复杂度
3. 注意处理 `random` 指针时，复制节点的 `random` 应指向对应的复制节点
