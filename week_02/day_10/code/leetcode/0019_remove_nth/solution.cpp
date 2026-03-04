/**
 * @file solution.cpp
 * @brief LeetCode 19: 删除链表的倒数第N个节点 实现
 * 
 * 给你一个链表，删除链表的倒数第 n 个结点，并且返回链表的头结点。
 * 
 * 难度：中等
 * 标签：链表、双指针
 */

#include <iostream>
#include <vector>
#include <string>

// 链表节点定义
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
public:
    /**
     * @brief 双指针法删除倒数第N个节点
     * 
     * 思路：
     * 1. 使用虚拟头节点简化边界处理
     * 2. 快指针先走 n 步
     * 3. 快慢指针同时移动，直到快指针到达末尾
     * 4. 此时慢指针指向待删除节点的前一个节点
     * 
     * @param head 链表头节点
     * @param n 倒数第n个
     * @return 删除后的链表头节点
     * 
     * 时间复杂度：O(n)，只需一次遍历
     * 空间复杂度：O(1)
     */
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        // 使用虚拟头节点，统一处理删除头节点的情况
        ListNode *dummy = new ListNode(0, head);
        
        ListNode *fast = dummy;
        ListNode *slow = dummy;
        
        // 快指针先走 n 步
        for (int i = 0; i < n; ++i) {
            fast = fast->next;
        }
        
        // 快慢指针同时移动
        // 当快指针到达最后一个节点时，慢指针在倒数第 n+1 个节点
        while (fast->next != nullptr) {
            fast = fast->next;
            slow = slow->next;
        }
        
        // 删除慢指针的下一个节点
        ListNode *toDelete = slow->next;
        slow->next = slow->next->next;
        delete toDelete;
        
        // 返回真正的头节点
        ListNode *result = dummy->next;
        delete dummy;
        
        return result;
    }
    
    /**
     * @brief 计算长度法（需要两次遍历）
     */
    ListNode* removeNthFromEndByLength(ListNode* head, int n) {
        // 第一次遍历：计算链表长度
        int length = 0;
        ListNode *curr = head;
        while (curr != nullptr) {
            ++length;
            curr = curr->next;
        }
        
        // 创建虚拟头节点
        ListNode *dummy = new ListNode(0, head);
        curr = dummy;
        
        // 找到倒数第 n+1 个节点（即正数第 length-n 个节点）
        for (int i = 0; i < length - n; ++i) {
            curr = curr->next;
        }
        
        // 删除节点
        ListNode *toDelete = curr->next;
        curr->next = curr->next->next;
        delete toDelete;
        
        ListNode *result = dummy->next;
        delete dummy;
        
        return result;
    }
};

// ============================================
// 测试辅助函数
// ============================================
namespace {

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

std::vector<int> listToVector(ListNode* head) {
    std::vector<int> result;
    while (head != nullptr) {
        result.push_back(head->val);
        head = head->next;
    }
    return result;
}

void freeList(ListNode* head) {
    while (head != nullptr) {
        ListNode *temp = head;
        head = head->next;
        delete temp;
    }
}

std::string listToString(ListNode* head) {
    std::string result = "[";
    bool first = true;
    while (head != nullptr) {
        if (!first) result += "->";
        result += std::to_string(head->val);
        head = head->next;
        first = false;
    }
    result += "]";
    return result;
}

void runTest(const std::string& name,
             const std::vector<int>& inputValues,
             int n,
             const std::vector<int>& expectedValues) {
    std::cout << "  测试: " << name << "\n";
    
    ListNode *head = createList(inputValues);
    std::cout << "    输入: " << listToString(head) << ", n = " << n << "\n";
    
    Solution sol;
    ListNode *result = sol.removeNthFromEnd(head, n);
    
    std::vector<int> actual = listToVector(result);
    
    std::string expectedStr = "[";
    for (size_t i = 0; i < expectedValues.size(); ++i) {
        if (i > 0) expectedStr += "->";
        expectedStr += std::to_string(expectedValues[i]);
    }
    expectedStr += "]";
    
    std::cout << "    期望: " << expectedStr << "\n";
    std::cout << "    实际: " << listToString(result) << "\n";
    
    if (actual == expectedValues) {
        std::cout << "    ✅ 通过\n";
    } else {
        std::cout << "    ❌ 失败\n";
    }
    
    freeList(result);
    std::cout << "\n";
}

} // anonymous namespace

// ============================================
// 主演示函数
// ============================================
void test_leetcode_19() {
    std::cout << "\n【LeetCode 19: 删除链表的倒数第N个节点】\n";
    
    std::cout << "\n双指针技巧:\n";
    std::cout << "  1. 虚拟头节点处理边界情况\n";
    std::cout << "  2. 快指针先走 n 步\n";
    std::cout << "  3. 快慢指针同时移动\n";
    std::cout << "  4. 慢指针停在倒数第 n+1 个节点\n";
    std::cout << "  5. 删除慢指针的下一个节点\n";
    
    std::cout << "\n-------------------- 测试用例 --------------------\n";
    
    // 测试用例
    runTest("删除中间节点", {1, 2, 3, 4, 5}, 2, {1, 2, 3, 5});
    runTest("删除头节点", {1, 2, 3, 4, 5}, 5, {2, 3, 4, 5});
    runTest("删除尾节点", {1, 2, 3, 4, 5}, 1, {1, 2, 3, 4});
    runTest("单节点链表", {1}, 1, {});
    runTest("两节点删第一个", {1, 2}, 2, {2});
    runTest("两节点删第二个", {1, 2}, 1, {1});
    
    std::cout << "-------------------- 测试完成 --------------------\n";
    
    // 可视化演示
    std::cout << "\n【双指针移动可视化】\n";
    std::cout << "  链表: 1->2->3->4->5, n=2\n";
    std::cout << "  初始: dummy->1->2->3->4->5\n";
    std::cout << "        slow  fast\n";
    std::cout << "\n";
    std::cout << "  快指针走2步:\n";
    std::cout << "        dummy->1->2->3->4->5\n";
    std::cout << "              slow    fast\n";
    std::cout << "\n";
    std::cout << "  同时移动到末尾:\n";
    std::cout << "        dummy->1->2->3->4->5\n";
    std::cout << "                    slow fast\n";
    std::cout << "\n";
    std::cout << "  slow在倒数第3个节点，删除slow->next即可\n";
    
    std::cout << "\n【方法对比】\n";
    std::cout << "  双指针法:   一次遍历 O(n)\n";
    std::cout << "  计算长度法: 两次遍历 O(n) + O(n)\n";
    std::cout << "  空间复杂度: 都是 O(1)\n";
}
