/**
 * @file list_operations.cpp
 * @brief 链表基本操作实现
 * @author C++ Tutorial
 * @date Day 8
 */

#include "list_node.h"
#include <cassert>
#include <algorithm>

namespace list_ops {

// ==================== 创建与销毁 ====================

ListNode* createList(const std::vector<int>& values) {
    if (values.empty()) return nullptr;
    
    ListNode* head = new ListNode(values[0]);
    ListNode* curr = head;
    
    for (size_t i = 1; i < values.size(); ++i) {
        curr->next = new ListNode(values[i]);
        curr = curr->next;
    }
    
    return head;
}

ListNode* createList(std::initializer_list<int> values) {
    return createList(std::vector<int>(values));
}

ListNode* createListWithDummy(const std::vector<int>& values) {
    ListNode* dummy = new ListNode(0);  // 虚拟头节点
    
    if (!values.empty()) {
        dummy->next = createList(values);
    }
    
    return dummy;
}

void deleteList(ListNode* head) {
    while (head != nullptr) {
        ListNode* next = head->next;
        delete head;
        head = next;
    }
}

ListNode* copyList(const ListNode* head) {
    if (!head) return nullptr;
    
    ListNode* newHead = new ListNode(head->val);
    ListNode* curr = newHead;
    head = head->next;
    
    while (head) {
        curr->next = new ListNode(head->val);
        curr = curr->next;
        head = head->next;
    }
    
    return newHead;
}

// ==================== 遍历与访问 ====================

void printList(const ListNode* head, std::ostream& os) {
    os << "List: ";
    while (head != nullptr) {
        os << head->val;
        if (head->next != nullptr) {
            os << " -> ";
        }
        head = head->next;
    }
    os << " -> nullptr" << std::endl;
}

int getLength(const ListNode* head) {
    int len = 0;
    while (head != nullptr) {
        ++len;
        head = head->next;
    }
    return len;
}

ListNode* getNode(ListNode* head, int n) {
    if (n < 0) return nullptr;
    
    int index = 0;
    while (head != nullptr && index < n) {
        head = head->next;
        ++index;
    }
    
    return head;
}

const ListNode* getNode(const ListNode* head, int n) {
    if (n < 0) return nullptr;
    
    int index = 0;
    while (head != nullptr && index < n) {
        head = head->next;
        ++index;
    }
    
    return head;
}

ListNode* getLastNode(ListNode* head) {
    if (!head) return nullptr;
    
    while (head->next != nullptr) {
        head = head->next;
    }
    
    return head;
}

std::vector<int> toVector(const ListNode* head) {
    std::vector<int> result;
    while (head != nullptr) {
        result.push_back(head->val);
        head = head->next;
    }
    return result;
}

// ==================== 插入操作 ====================

ListNode* insertHead(ListNode*& head, int val) {
    ListNode* newNode = new ListNode(val, head);
    head = newNode;
    return head;
}

ListNode* insertTail(ListNode*& head, int val) {
    ListNode* newNode = new ListNode(val);
    
    if (head == nullptr) {
        head = newNode;
        return head;
    }
    
    ListNode* last = getLastNode(head);
    last->next = newNode;
    return head;
}

ListNode* insertAfter(ListNode* node, int val) {
    if (!node) return nullptr;
    
    ListNode* newNode = new ListNode(val, node->next);
    node->next = newNode;
    return newNode;
}

ListNode* insertAt(ListNode*& head, int index, int val) {
    if (index <= 0) {
        return insertHead(head, val);
    }
    
    ListNode* prev = getNode(head, index - 1);
    if (!prev) {
        return insertTail(head, val);
    }
    
    return insertAfter(prev, val);
}

// ==================== 删除操作 ====================

ListNode* deleteHead(ListNode*& head) {
    if (!head) return nullptr;
    
    ListNode* oldHead = head;
    head = head->next;
    delete oldHead;
    
    return head;
}

ListNode* deleteTail(ListNode*& head) {
    if (!head) return nullptr;
    
    // 只有一个节点
    if (head->next == nullptr) {
        delete head;
        head = nullptr;
        return nullptr;
    }
    
    // 找到倒数第二个节点
    ListNode* curr = head;
    while (curr->next->next != nullptr) {
        curr = curr->next;
    }
    
    delete curr->next;
    curr->next = nullptr;
    
    return head;
}

int deleteAfter(ListNode* node) {
    if (!node || !node->next) return -1;
    
    ListNode* toDelete = node->next;
    int val = toDelete->val;
    node->next = toDelete->next;
    delete toDelete;
    
    return val;
}

bool deleteFirst(ListNode*& head, int val) {
    if (!head) return false;
    
    // 删除头节点
    if (head->val == val) {
        ListNode* toDelete = head;
        head = head->next;
        delete toDelete;
        return true;
    }
    
    // 查找前驱节点
    ListNode* prev = head;
    while (prev->next && prev->next->val != val) {
        prev = prev->next;
    }
    
    if (prev->next) {
        ListNode* toDelete = prev->next;
        prev->next = toDelete->next;
        delete toDelete;
        return true;
    }
    
    return false;
}

int deleteAll(ListNode*& head, int val) {
    int count = 0;
    
    // 删除头部的所有目标值节点
    while (head && head->val == val) {
        ListNode* toDelete = head;
        head = head->next;
        delete toDelete;
        ++count;
    }
    
    if (!head) return count;
    
    // 删除中间和尾部的目标值节点
    ListNode* curr = head;
    while (curr->next) {
        if (curr->next->val == val) {
            ListNode* toDelete = curr->next;
            curr->next = toDelete->next;
            delete toDelete;
            ++count;
        } else {
            curr = curr->next;
        }
    }
    
    return count;
}

// ==================== 查找操作 ====================

ListNode* find(ListNode* head, int val) {
    while (head != nullptr) {
        if (head->val == val) return head;
        head = head->next;
    }
    return nullptr;
}

const ListNode* find(const ListNode* head, int val) {
    while (head != nullptr) {
        if (head->val == val) return head;
        head = head->next;
    }
    return nullptr;
}

bool contains(const ListNode* head, int val) {
    return find(head, val) != nullptr;
}

ListNode* findPrev(ListNode* head, int val) {
    if (!head || !head->next) return nullptr;
    
    while (head->next) {
        if (head->next->val == val) return head;
        head = head->next;
    }
    
    return nullptr;
}

// ==================== 反转操作 ====================

ListNode* reverse(ListNode* head) {
    ListNode* prev = nullptr;
    ListNode* curr = head;
    
    while (curr != nullptr) {
        ListNode* next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    
    return prev;
}

ListNode* reverseRecursive(ListNode* head) {
    // 基础情况：空链表或只有一个节点
    if (!head || !head->next) {
        return head;
    }
    
    // 递归反转剩余部分
    ListNode* newHead = reverseRecursive(head->next);
    
    // 将当前节点接到反转后的链表末尾
    head->next->next = head;
    head->next = nullptr;
    
    return newHead;
}

ListNode* reverseN(ListNode* head, int n) {
    if (n <= 0 || !head) return head;
    
    ListNode* prev = nullptr;
    ListNode* curr = head;
    int count = 0;
    
    while (curr && count < n) {
        ListNode* next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
        ++count;
    }
    
    // 将原链表的剩余部分接到反转后的尾部
    head->next = curr;
    
    return prev;
}

ListNode* reverseBetween(ListNode* head, int m, int n) {
    if (m >= n || !head) return head;
    
    // 使用虚拟头节点简化边界处理
    ListNode* dummy = new ListNode(0, head);
    ListNode* prev = dummy;
    
    // 找到第m-1个节点
    for (int i = 1; i < m && prev->next; ++i) {
        prev = prev->next;
    }
    
    if (!prev->next) {
        delete dummy;
        return head;
    }
    
    // 反转从m到n的节点
    ListNode* curr = prev->next;
    ListNode* tail = curr;  // 反转后的尾部
    ListNode* newPrev = nullptr;
    
    int count = 0;
    int range = n - m + 1;
    
    while (curr && count < range) {
        ListNode* next = curr->next;
        curr->next = newPrev;
        newPrev = curr;
        curr = next;
        ++count;
    }
    
    // 连接反转部分
    prev->next = newPrev;
    tail->next = curr;
    
    ListNode* result = dummy->next;
    delete dummy;
    
    return result;
}

// ==================== 合并与拆分 ====================

ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
    ListNode* dummy = new ListNode(0);
    ListNode* tail = dummy;
    
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
    
    // 连接剩余部分
    tail->next = l1 ? l1 : l2;
    
    ListNode* result = dummy->next;
    delete dummy;
    
    return result;
}

ListNode* findMiddle(ListNode* head) {
    if (!head) return nullptr;
    
    ListNode* slow = head;
    ListNode* fast = head;
    
    // 快慢指针找中点
    while (fast->next && fast->next->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    
    return slow;
}

// ==================== 环检测 ====================

bool hasCycle(const ListNode* head) {
    if (!head || !head->next) return false;
    
    const ListNode* slow = head;
    const ListNode* fast = head;
    
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        
        if (slow == fast) {
            return true;
        }
    }
    
    return false;
}

ListNode* detectCycle(ListNode* head) {
    if (!head || !head->next) return nullptr;
    
    ListNode* slow = head;
    ListNode* fast = head;
    
    // 第一阶段：检测是否有环
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        
        if (slow == fast) {
            // 第二阶段：找到环入口
            ListNode* ptr1 = head;
            ListNode* ptr2 = slow;
            
            while (ptr1 != ptr2) {
                ptr1 = ptr1->next;
                ptr2 = ptr2->next;
            }
            
            return ptr1;
        }
    }
    
    return nullptr;
}

// ==================== 辅助函数 ====================

bool isEqual(const ListNode* l1, const ListNode* l2) {
    while (l1 && l2) {
        if (l1->val != l2->val) return false;
        l1 = l1->next;
        l2 = l2->next;
    }
    
    return l1 == nullptr && l2 == nullptr;
}

} // namespace list_ops

// ============================================================
// 现代C++风格的链表实现
// ============================================================

namespace modern_list {

std::unique_ptr<UniqueListNode> createUniqueList(const std::vector<int>& values) {
    if (values.empty()) return nullptr;
    
    auto head = std::make_unique<UniqueListNode>(values[0]);
    UniqueListNode* curr = head.get();
    
    for (size_t i = 1; i < values.size(); ++i) {
        curr->next = std::make_unique<UniqueListNode>(values[i]);
        curr = curr->next.get();
    }
    
    return head;
}

void printUniqueList(const UniqueListNode* head) {
    std::cout << "UniqueList: ";
    while (head) {
        std::cout << head->val;
        if (head->next) {
            std::cout << " -> ";
        }
        head = head->next.get();
    }
    std::cout << " -> nullptr" << std::endl;
}

} // namespace modern_list
