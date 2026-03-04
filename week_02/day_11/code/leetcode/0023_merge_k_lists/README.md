# LeetCode 23: 合并K个升序链表

## 题目描述

给你一个链表数组，每个链表都已经按升序排列。请将所有链表合并到一个升序链表中，返回合并后的链表。

## 示例

### 示例 1
```
输入：lists = [[1,4,5],[1,3,4],[2,6]]
输出：[1,1,2,3,4,4,5,6]
解释：链表数组如下：
[
  1->4->5,
  1->3->4,
  2->6
]
将它们合并到一个有序链表中得到。
1->1->2->3->4->4->5->6
```

### 示例 2
```
输入：lists = []
输出：[]
```

### 示例 3
```
输入：lists = [[]]
输出：[]
```

## 解题思路

### 方法一：分治合并（推荐）

**思路**：
1. 将K个链表分成两半
2. 递归合并每一半
3. 最后合并两个结果

**复杂度分析**：
- 时间复杂度：O(N × log K)，其中 N 是所有节点总数，K 是链表数量
- 空间复杂度：O(log K)，递归栈深度

**代码**：
```cpp
ListNode* mergeKLists(vector<ListNode*>& lists) {
    if (lists.empty()) return nullptr;
    return merge(lists, 0, lists.size() - 1);
}

ListNode* merge(vector<ListNode*>& lists, int left, int right) {
    if (left == right) return lists[left];
    int mid = left + (right - left) / 2;
    ListNode* l1 = merge(lists, left, mid);
    ListNode* l2 = merge(lists, mid + 1, right);
    return mergeTwoLists(l1, l2);
}
```

### 方法二：优先队列（最小堆）

**思路**：
1. 将所有链表的头节点放入最小堆
2. 每次取出最小节点，加入结果链表
3. 如果该节点有 next，将 next 放入堆

**复杂度分析**：
- 时间复杂度：O(N × log K)
- 空间复杂度：O(K)，优先队列大小

**代码**：
```cpp
ListNode* mergeKLists(vector<ListNode*>& lists) {
    auto cmp = [](ListNode* a, ListNode* b) { return a->val > b->val; };
    priority_queue<ListNode*, vector<ListNode*>, decltype(cmp)> pq(cmp);

    for (auto list : lists) {
        if (list) pq.push(list);
    }

    ListNode dummy;
    ListNode* tail = &dummy;

    while (!pq.empty()) {
        ListNode* node = pq.top(); pq.pop();
        tail->next = node;
        tail = tail->next;
        if (node->next) pq.push(node->next);
    }

    return dummy.next;
}
```

### 方法三：顺序合并

**思路**：逐个合并链表

**复杂度分析**：
- 时间复杂度：O(K² × N)
- 空间复杂度：O(1)

**适用场景**：链表数量较少时简单直观

## 方法比较

| 方法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| 分治合并 | O(N×log K) | O(log K) | 递归，空间效率高 |
| 优先队列 | O(N×log K) | O(K) | 非递归，思路直观 |
| 顺序合并 | O(K²×N) | O(1) | 简单，但效率低 |

## 关键点

1. **合并两个有序链表**是基础操作
2. 分治法将问题规模减半
3. 优先队列自动维护最小值
4. 哑节点（dummy node）简化边界处理

## 扩展练习

1. LeetCode 21: 合并两个有序链表
2. LeetCode 148: 排序链表
3. LeetCode 2: 两数相加

## 编译运行

```bash
cd /home/z/my-project/download/week_02/day_11
./build_and_run.sh leetcode23
```
