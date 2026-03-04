/**
 * @file fast_slow_pointer.cpp
 * @brief 快慢指针技巧详解
 * 
 * 快慢指针是链表操作中最优雅的技巧之一，主要应用：
 * 1. 找链表中点
 * 2. 判断链表是否有环
 * 3. 找环的入口点
 */

#include <iostream>
#include <vector>

// 链表节点定义
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* n) : val(x), next(n) {}
};

// ============================================================
// 应用1: 找链表中点
// ============================================================

/**
 * @brief 使用快慢指针找链表中点
 * 
 * 原理：
 * - 慢指针每次走1步
 * - 快指针每次走2步
 * - 当快指针到达末尾时，慢指针正好在中间
 * 
 * @param head 链表头节点
 * @return 中点节点（偶数长度返回靠后的中点）
 */
ListNode* findMiddle(ListNode* head) {
    if (!head) return nullptr;
    
    ListNode* slow = head;
    ListNode* fast = head;
    
    // 快指针走两步，慢指针走一步
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    
    return slow;
}

/**
 * @brief 找链表中点（返回靠前的中点）
 * 
 * 用于链表归并排序时分割链表
 */
ListNode* findMiddleForMerge(ListNode* head) {
    if (!head || !head->next) return head;
    
    ListNode* slow = head;
    ListNode* fast = head->next;  // 快指针先走一步
    
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    
    return slow;  // 返回靠前的中点
}

// ============================================================
// 应用2: 判断链表是否有环
// ============================================================

/**
 * @brief 使用快慢指针判断链表是否有环
 * 
 * 原理（Floyd判圈算法）：
 * - 如果有环，快指针最终会追上慢指针
 * - 如果无环，快指针会先到达末尾
 * 
 * 证明：
 * 设环长为L，当慢指针进入环后：
 * - 快指针相对于慢指针每步多走1步
 * - 最多经过L步，快指针就会追上慢指针
 * 
 * @param head 链表头节点
 * @return 是否有环
 */
bool hasCycle(ListNode* head) {
    if (!head || !head->next) return false;
    
    ListNode* slow = head;
    ListNode* fast = head;
    
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        
        if (slow == fast) {
            return true;  // 相遇，有环
        }
    }
    
    return false;  // 快指针到达末尾，无环
}

// ============================================================
// 应用3: 找环的入口
// ============================================================

/**
 * @brief 找环的入口节点
 * 
 * 原理（数学推导）：
 * - 设从起点到环入口距离为a，环长为b
 * - 相遇时，慢指针走了 s = a + m*b + x
 * - 快指针走了 2s = a + n*b + x
 * - 所以 a + x = k*b（k为某个整数）
 * - 即：从起点走a步 = 从相遇点走a步（都会到环入口）
 * 
 * @param head 链表头节点
 * @return 环入口节点，无环返回nullptr
 */
ListNode* detectCycle(ListNode* head) {
    if (!head || !head->next) return nullptr;
    
    ListNode* slow = head;
    ListNode* fast = head;
    
    // 第一步：判断是否有环，并找到相遇点
    bool hasCycle = false;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        
        if (slow == fast) {
            hasCycle = true;
            break;
        }
    }
    
    if (!hasCycle) return nullptr;
    
    // 第二步：找环入口
    // 从头节点和相遇点同时出发，相遇处即为入口
    ListNode* ptr1 = head;
    ListNode* ptr2 = slow;  // 或fast，因为它们在相遇点
    
    while (ptr1 != ptr2) {
        ptr1 = ptr1->next;
        ptr2 = ptr2->next;
    }
    
    return ptr1;
}

// ============================================================
// 辅助函数
// ============================================================

// 创建链表
static ListNode* createList(const std::vector<int>& vals) {
    if (vals.empty()) return nullptr;
    
    ListNode* head = new ListNode(vals[0]);
    ListNode* cur = head;
    for (size_t i = 1; i < vals.size(); ++i) {
        cur->next = new ListNode(vals[i]);
        cur = cur->next;
    }
    return head;
}

// 创建带环链表
static ListNode* createListWithCycle(const std::vector<int>& vals, int pos) {
    if (vals.empty()) return nullptr;
    
    ListNode* head = new ListNode(vals[0]);
    ListNode* cur = head;
    ListNode* cycleEntry = nullptr;
    
    for (size_t i = 1; i < vals.size(); ++i) {
        cur->next = new ListNode(vals[i]);
        cur = cur->next;
        if (static_cast<int>(i) == pos) {
            cycleEntry = cur;
        }
    }
    
    // 创建环
    if (pos >= 0 && pos < static_cast<int>(vals.size())) {
        // 找到环入口
        ListNode* entry = head;
        for (int i = 0; i < pos; ++i) {
            entry = entry->next;
        }
        cur->next = entry;
    }
    
    return head;
}

// 打印链表（最多打印前20个节点）
void printList(ListNode* head, int maxNodes = 20) {
    int count = 0;
    while (head && count < maxNodes) {
        std::cout << head->val << " -> ";
        head = head->next;
        ++count;
    }
    if (head) {
        std::cout << "...(更多节点)";
    } else {
        std::cout << "nullptr";
    }
    std::cout << "\n";
}

// 释放链表内存
void deleteList(ListNode* head) {
    while (head) {
        ListNode* tmp = head;
        head = head->next;
        delete tmp;
    }
}

// ============================================================
// 演示函数
// ============================================================

void demoFastSlowPointer() {
    // ----- 演示找中点 -----
    std::cout << "【演示1：找链表中点】\n\n";
    
    // 奇数长度
    ListNode* list1 = createList({1, 2, 3, 4, 5});
    std::cout << "链表(奇数长度): ";
    printList(list1);
    std::cout << "中点: " << findMiddle(list1)->val << "\n";
    deleteList(list1);
    
    std::cout << "\n";
    
    // 偶数长度
    ListNode* list2 = createList({1, 2, 3, 4, 5, 6});
    std::cout << "链表(偶数长度): ";
    printList(list2);
    std::cout << "中点(靠后): " << findMiddle(list2)->val << "\n";
    std::cout << "中点(靠前): " << findMiddleForMerge(list2)->val << "\n";
    deleteList(list2);
    
    // ----- 演示判断环 -----
    std::cout << "\n【演示2：判断链表是否有环】\n\n";
    
    // 无环链表
    ListNode* list3 = createList({1, 2, 3, 4, 5});
    std::cout << "无环链表: ";
    printList(list3);
    std::cout << "是否有环: " << (hasCycle(list3) ? "是" : "否") << "\n";
    deleteList(list3);
    
    std::cout << "\n";
    
    // 有环链表（pos=2表示环入口在第3个节点）
    ListNode* list4 = createListWithCycle({1, 2, 3, 4, 5}, 2);
    std::cout << "有环链表: 1 -> 2 -> 3 -> 4 -> 5 -> 3(环入口)\n";
    std::cout << "是否有环: " << (hasCycle(list4) ? "是" : "否") << "\n";
    
    // ----- 演示找环入口 -----
    std::cout << "\n【演示3：找环的入口】\n\n";
    
    ListNode* entry = detectCycle(list4);
    if (entry) {
        std::cout << "环入口节点值: " << entry->val << "\n";
    }
    
    // 简单清理（带环链表需要特殊处理）
    // 这里为了演示简单，不进行完整清理
    // 实际应用中应该断开环后再删除
    
    std::cout << "\n";
}
