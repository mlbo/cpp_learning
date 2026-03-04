/**
 * @file list_node.h
 * @brief 链表节点定义和辅助函数（公共头文件）
 */

#ifndef LIST_NODE_H
#define LIST_NODE_H

#include <vector>
#include <string>

// 链表节点定义
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* n) : val(x), next(n) {}
};

// 辅助函数：创建链表
inline ListNode* createList(const std::vector<int>& values) {
    if (values.empty()) return nullptr;
    ListNode* head = new ListNode(values[0]);
    ListNode* cur = head;
    for (size_t i = 1; i < values.size(); ++i) {
        cur->next = new ListNode(values[i]);
        cur = cur->next;
    }
    return head;
}

// 辅助函数：打印链表
inline std::string listToString(ListNode* head) {
    std::string result;
    while (head) {
        result += std::to_string(head->val);
        if (head->next) result += " -> ";
        head = head->next;
    }
    return result.empty() ? "nullptr" : result;
}

// 辅助函数：释放链表
inline void deleteList(ListNode* head) {
    while (head) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

// 辅助函数：释放链表直到某个节点
inline void deleteListUntil(ListNode* head, ListNode* stop) {
    while (head && head != stop) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

#endif // LIST_NODE_H
