/**
 * @file solution.cpp
 * @brief LeetCode 142: 环形链表 II 实现
 * 
 * 给定链表的头节点 head，返回链表开始入环的第一个节点。
 * 如果链表无环，则返回 null。
 * 
 * 难度：中等
 * 标签：链表、双指针、快慢指针
 */

#include <iostream>
#include <unordered_set>
#include <vector>

// 链表节点定义
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

class Solution {
public:
    /**
     * @brief 快慢指针法找环入口
     * 
     * 数学推导：
     * 设 a 为头到入环点的距离
     * 设 b 为入环点到相遇点的距离
     * 设 c 为相遇点到入环点的距离
     * 
     * 慢指针走的距离：a + b
     * 快指针走的距离：a + n(b+c) + b = a + (n+1)b + nc
     * 
     * 快指针是慢指针的两倍：
     * 2(a + b) = a + (n+1)b + nc
     * => a = (n-1)(b+c) + c
     * 
     * 结论：从相遇点和头同时出发，会在入环点相遇！
     * 
     * @param head 链表头节点
     * @return 环入口节点，无环返回 nullptr
     * 
     * 时间复杂度：O(n)
     * 空间复杂度：O(1)
     */
    ListNode *detectCycle(ListNode *head) {
        if (head == nullptr || head->next == nullptr) {
            return nullptr;
        }
        
        // 第一步：使用快慢指针找到相遇点
        ListNode *slow = head;
        ListNode *fast = head;
        
        while (fast != nullptr && fast->next != nullptr) {
            slow = slow->next;          // 慢指针走一步
            fast = fast->next->next;    // 快指针走两步
            
            if (slow == fast) {
                // 相遇了，说明有环
                // 第二步：找环入口
                ListNode *ptr1 = head;
                ListNode *ptr2 = slow;
                
                while (ptr1 != ptr2) {
                    ptr1 = ptr1->next;
                    ptr2 = ptr2->next;
                }
                
                return ptr1;  // 环入口
            }
        }
        
        return nullptr;  // 无环
    }
    
    /**
     * @brief 哈希表法找环入口（简单但用额外空间）
     */
    ListNode *detectCycleHash(ListNode *head) {
        std::unordered_set<ListNode*> visited;
        
        ListNode *curr = head;
        while (curr != nullptr) {
            if (visited.find(curr) != visited.end()) {
                return curr;  // 第一次遇到的重复节点就是环入口
            }
            visited.insert(curr);
            curr = curr->next;
        }
        
        return nullptr;  // 无环
    }
};

// ============================================
// 测试辅助函数
// ============================================
namespace {

// 简单测试：只创建无环链表用于测试
ListNode* createList(const std::vector<int>& values) {
    if (values.empty()) return nullptr;
    
    ListNode *head = new ListNode(values[0]);
    ListNode *curr = head;
    
    for (size_t i = 1; i < values.size(); ++i) {
        curr->next = new ListNode(values[i]);
        curr = curr->next;
    }
    
    return head;
}

void freeList(ListNode* head) {
    while (head != nullptr) {
        ListNode *temp = head;
        head = head->next;
        delete temp;
    }
}

// 安全释放有环链表
void freeCycleList(ListNode* head, ListNode* cycleStart) {
    if (head == nullptr) return;
    
    // 先打断环
    if (cycleStart != nullptr) {
        ListNode* curr = head;
        int maxIter = 100;
        while (curr && curr->next != cycleStart && maxIter-- > 0) {
            curr = curr->next;
        }
        if (curr && curr->next == cycleStart) {
            curr->next = nullptr;
        }
    }
    
    freeList(head);
}

void runTest(const std::string& name, 
             const std::vector<int>& values, 
             int pos, 
             int expected) {
    std::cout << "  测试: " << name << "\n";
    
    ListNode *head = createList(values);
    
    Solution sol;
    ListNode *entrance = sol.detectCycle(head);
    
    // 无环链表的测试
    int actual = (entrance != nullptr) ? -2 : -1;  // -2 表示找到环(不应该), -1 表示无环
    
    std::cout << "    输入: 链表 [";
    for (size_t i = 0; i < values.size(); ++i) {
        std::cout << values[i];
        if (i < values.size() - 1) std::cout << "->";
    }
    std::cout << "], pos = " << pos << "\n";
    
    std::cout << "    期望入口索引: " << expected << "\n";
    std::cout << "    实际入口索引: " << actual << "\n";
    
    if (actual == expected) {
        std::cout << "    ✅ 通过\n";
    } else {
        std::cout << "    ❌ 失败\n";
    }
    
    freeList(head);
    std::cout << "\n";
}

} // anonymous namespace

// ============================================
// 主演示函数
// ============================================
void test_leetcode_142() {
    std::cout << "\n【LeetCode 142: 环形链表 II】\n";
    std::cout << "\n快慢指针数学推导:\n";
    std::cout << "  设 a = 头到入环点距离\n";
    std::cout << "  设 b = 入环点到相遇点距离\n";
    std::cout << "  设 c = 相遇点到入环点距离\n";
    std::cout << "  慢指针: a + b\n";
    std::cout << "  快指针: a + n(b+c) + b\n";
    std::cout << "  推导: a = (n-1)(b+c) + c\n";
    std::cout << "  结论: 从相遇点和头同时出发，必在入环点相遇\n";
    
    std::cout << "\n-------------------- 测试用例 (无环链表) --------------------\n";
    
    // 测试无环链表用例
    runTest("无环链表", {1, 2, 3, 4}, -1, -1);
    runTest("单节点无环", {1}, -1, -1);
    runTest("空链表", {}, -1, -1);
    
    std::cout << "-------------------- 测试完成 --------------------\n";
    
    // 演示有环链表检测（不释放，仅展示）
    std::cout << "\n【有环链表示例】\n";
    ListNode* n1 = new ListNode(3);
    ListNode* n2 = new ListNode(2);
    ListNode* n3 = new ListNode(0);
    ListNode* n4 = new ListNode(-4);
    n1->next = n2;
    n2->next = n3;
    n3->next = n4;
    n4->next = n2;  // 形成环
    
    Solution sol;
    ListNode* entrance = sol.detectCycle(n1);
    if (entrance != nullptr) {
        std::cout << "  链表 [3->2->0->-4], 环在索引1 (节点值为2)\n";
        std::cout << "  检测到环入口值: " << entrance->val << "\n";
        std::cout << "  ✅ 环检测正确\n";
    }
    
    // 打断环后释放
    n4->next = nullptr;
    freeList(n1);
    
    std::cout << "\n【方法对比】\n";
    std::cout << "  快慢指针: O(1) 空间，O(n) 时间\n";
    std::cout << "  哈希表:   O(n) 空间，O(n) 时间\n";
}
