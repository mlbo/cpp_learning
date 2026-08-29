# LeetCode 234. 回文链表

## 题目描述

给你一个单链表的头节点 `head`，请你判断该链表是否为回文链表。如果是，返回 `true`；否则，返回 `false`。

## 示例

### 示例 1
```
输入：head = [1,2,2,1]
输出：true
```

### 示例 2
```
输入：head = [1,2]
输出：false
```

## 提示

- 链表中节点数目在范围 `[1, 10^5]` 内
- `0 <= Node.val <= 9`

## 进阶

你能否用 O(n) 时间复杂度和 O(1) 空间复杂度解决此题？

---

## 解题思路

本教程把它定义为查询接口：允许临时反转后半段，但返回前必须恢复原链拓扑。输入链表必须无环；算法不取得节点所有权，也不创建或释放节点。

### 方法一：快慢指针 + 反转链表（推荐）

**核心思想**：将链表分成两半，反转后半部分，然后比较。

**算法步骤**：
1. 使用快慢指针找到链表中点
2. 反转后半部分链表
3. 同时遍历前后两半进行比较
4. 恢复链表原始结构，兑现查询接口不修改输入拓扑的后置条件

**复杂度**：
- 时间：O(n)
- 空间：O(1)

```cpp
bool isPalindrome(ListNode* head) {
    if (!head || !head->next) return true;

    // 1. 快慢指针找中点
    ListNode *slow = head, *fast = head;
    while (fast->next && fast->next->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    // 2. 反转后半部分
    ListNode* second = reverseList(slow->next);

    // 3. 比较
    ListNode *p1 = head, *p2 = second;
    bool result = true;
    while (p2) {
        if (p1->val != p2->val) {
            result = false;
            break;
        }
        p1 = p1->next;
        p2 = p2->next;
    }

    // 4. 恢复，保证查询接口不改变输入结构
    slow->next = reverseList(second);

    return result;
}
```

### 方法二：数组存储法

**核心思想**：将链表值复制到数组，然后双指针比较。

**复杂度**：
- 时间：O(n)
- 空间：O(n)

```cpp
bool isPalindrome_array(ListNode* head) {
    vector<int> vals;
    while (head) {
        vals.push_back(head->val);
        head = head->next;
    }

    size_t left = 0;
    size_t right = vals.size(); // 半开区间 [left, right)
    while (left < right) {
        --right;
        if (vals[left] != vals[right]) {
            return false;
        }
        ++left;
    }
    return true;
}
```

### 方法三：递归法

**核心思想**：利用递归的栈来反向遍历链表。

**复杂度**：
- 时间：O(n)
- 空间：O(n) - 递归栈深度

```cpp
class Solution {
    ListNode* front;
public:
    bool isPalindrome(ListNode* head) {
        front = head;
        return check(head);
    }

    bool check(ListNode* back) {
        if (!back) return true;
        if (!check(back->next)) return false;
        if (front->val != back->val) return false;
        front = front->next;
        return true;
    }
};
```

---

## 关键点

1. **快慢指针技巧**：
   - 慢指针每次走一步，快指针每次走两步
   - 当快指针走到末尾，慢指针正好在中点

2. **链表反转**：
   - 需要熟练掌握迭代反转方法
   - 注意保存 next 指针

3. **边界处理**：
   - 空链表和单节点链表直接返回 true
   - 奇偶长度链表的处理

4. **链表恢复**：
   - 本教程把 `isPalindrome` 定义为查询接口，因此无论比较成功还是失败都必须恢复

---

## 相关题目

- [206. 反转链表](https://leetcode.cn/problems/reverse-linked-list/)
- [876. 链表的中间结点](https://leetcode.cn/problems/middle-of-the-linked-list/)
- [143. 重排链表](https://leetcode.cn/problems/reorder-list/)

---

## 运行测试

```bash
cd week_02/day_14
cmake -S . -B build-manual -DCMAKE_BUILD_TYPE=Debug
cmake --build build-manual --target test_leetcode234
./build-manual/test_leetcode234
```
