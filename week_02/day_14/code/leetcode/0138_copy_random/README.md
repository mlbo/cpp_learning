# LeetCode 138. 随机链表的复制

## 题目描述

给你一个长度为 `n` 的链表，每个节点包含一个额外增加的随机指针 `random`，该指针可以指向链表中的任何节点或空节点。请构造这个链表的深拷贝。

## 解题思路

输入的 `next` 链必须无环，`random` 可以为空、指向自身或指向链中的任意节点。深拷贝要求副本节点地址全部不同于原节点，并保持 `next/random` 指向关系；返回后调用者拥有一条独立链，原链结构必须保持不变。

### 方法一：哈希表法

**思路：**
1. 第一遍遍历：创建所有新节点，建立原节点到新节点的映射
2. 第二遍遍历：设置新节点的 `next` 和 `random` 指针

**复杂度：**
- 时间复杂度：O(n)
- 空间复杂度：O(n)

实现先用 `unique_ptr` 临时拥有全部副本，只有两遍连接都成功后才统一交付裸指针结果；分配或哈希表操作抛异常时，局部所有者会自动清理已创建节点。

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
Node* copyRandomList(Node* head) {
    if (!head) return nullptr;

    // 第一步：穿插副本；构造失败时恢复已修改前缀。
    Node* curr = head;
    try {
        while (curr) {
            auto copy = std::make_unique<Node>(curr->val);
            copy->next = curr->next;
            curr->next = copy.release();
            curr = curr->next->next;
        }
    } catch (...) {
        Node* rollback = head;
        while (rollback != curr) {
            Node* copy = rollback->next;
            rollback->next = copy->next;
            delete copy;
            rollback = rollback->next;
        }
        throw;
    }

    // 第二步：原节点 X 的副本就是 X->next。
    curr = head;
    while (curr) {
        if (curr->random) {
            curr->next->random = curr->random->next;
        }
        curr = curr->next->next;
    }

    // 第三步只做不抛的指针重连，同时恢复原链并分离副本链。
    Node* result = head->next;
    curr = head;
    while (curr) {
        Node* copy = curr->next;
        curr->next = copy->next;
        copy->next = curr->next ? curr->next->next : nullptr;
        curr = curr->next;
    }

    return result;
}
```

三阶段不变量分别是：已处理前缀呈 `原节点 -> 副本` 交错结构；每个副本的 `random` 指向目标原节点之后的副本；拆分时已处理前缀中的原链和副本链都已恢复，未处理后缀仍保持交错。不要在第三阶段再构造可能抛异常的哨兵，否则会重新打开“原链已修改但尚未恢复”的失败窗口。

## 关键点

1. **哈希表法**简单直观，但需要 O(n) 额外空间
2. **节点拆分法**巧妙地利用原链表结构，实现 O(1) 空间复杂度
3. 注意处理 `random` 指针时，复制节点的 `random` 应指向对应的复制节点
4. 节点穿插法的 `O(1)` 只指辅助空间，不计算必须返回的 `n` 个副本节点
5. 分配失败时必须释放已创建副本并恢复原链，不能只讨论成功路径

## 边界与验证

- 空链返回空；单节点分别覆盖 `random == nullptr` 与 `random == self`。
- 多节点覆盖向前、向后、交叉和重复指向同一目标的 `random`。
- 验证“值相等”还不够：应逐节点断言副本地址与原地址不同，并检查两种边的拓扑对应关系。
- 在副本创建中途注入异常，验证原链 `next/random` 未改变且已创建节点全部销毁。
