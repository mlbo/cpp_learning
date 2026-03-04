/**
 * @file test.cpp
 * @brief LeetCode 138. 随机链表的复制 - 测试用例
 */

#include <iostream>
#include <cassert>
#include "solution.h"

// 辅助函数：创建测试链表
Node* createTestList() {
    // 创建链表: 7 -> 13 -> 11 -> 10 -> 1
    // random: null, 7, 1, 11, 7

    Node* node7 = new Node(7);
    Node* node13 = new Node(13);
    Node* node11 = new Node(11);
    Node* node10 = new Node(10);
    Node* node1 = new Node(1);

    node7->next = node13;
    node13->next = node11;
    node11->next = node10;
    node10->next = node1;

    node13->random = node7;
    node11->random = node1;
    node10->random = node11;
    node1->random = node7;

    return node7;
}

// 辅助函数：验证复制结果
bool verifyCopy(Node* original, Node* copy) {
    // 检查是否是不同的节点
    if (original == copy) return false;

    // 遍历比较
    while (original && copy) {
        // 值必须相同
        if (original->val != copy->val) return false;

        // 检查random指针
        if (original->random) {
            if (!copy->random) return false;
            if (original->random->val != copy->random->val) return false;
            if (original->random == copy->random) return false;  // 不能指向原节点
        } else {
            if (copy->random) return false;
        }

        original = original->next;
        copy = copy->next;
    }

    return original == nullptr && copy == nullptr;
}

// 辅助函数：释放链表
void deleteList(Node* head) {
    while (head) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    std::cout << "=== LeetCode 138: 随机链表复制测试 ===\n\n";

    Solution solution;

    // 测试1：哈希表法
    {
        std::cout << "测试1: 哈希表法\n";
        Node* original = createTestList();
        Node* copy = solution.copyRandomList(original);

        assert(verifyCopy(original, copy));
        std::cout << "  通过!\n";

        deleteList(original);
        deleteList(copy);
    }

    // 测试2：节点拆分法
    {
        std::cout << "测试2: 节点拆分法 (O(1)空间)\n";
        Node* original = createTestList();
        Node* copy = solution.copyRandomList_optimized(original);

        assert(verifyCopy(original, copy));
        std::cout << "  通过!\n";

        deleteList(original);
        deleteList(copy);
    }

    // 测试3：递归法
    {
        std::cout << "测试3: 递归法\n";
        Node* original = createTestList();
        Node* copy = solution.copyRandomList_recursive(original);

        assert(verifyCopy(original, copy));
        std::cout << "  通过!\n";

        deleteList(original);
        deleteList(copy);
    }

    // 测试4：空链表
    {
        std::cout << "测试4: 空链表\n";
        Node* copy = solution.copyRandomList(nullptr);
        assert(copy == nullptr);
        std::cout << "  通过!\n";
    }

    // 测试5：单节点
    {
        std::cout << "测试5: 单节点\n";
        Node* single = new Node(42);
        single->random = single;

        Node* copy = solution.copyRandomList(single);
        assert(copy != nullptr);
        assert(copy->val == 42);
        assert(copy->random == copy);  // 指向自己
        std::cout << "  通过!\n";

        delete single;
        delete copy;
    }

    std::cout << "\n所有测试通过!\n";
    return 0;
}
