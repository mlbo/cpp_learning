/**
 * @file test.cpp
 * @brief LeetCode 138. 随机链表的复制 - 测试用例
 */

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "solution.h"

using leetcode_0138::Node;
using leetcode_0138::Solution;

// 辅助函数：创建测试链表
Node* createTestList() {
    // 创建链表: 7 -> 13 -> 11 -> 10 -> 1
    // random: null, 7, 1, 11, 7

    auto node7 = std::make_unique<Node>(7);
    auto node13 = std::make_unique<Node>(13);
    auto node11 = std::make_unique<Node>(11);
    auto node10 = std::make_unique<Node>(10);
    auto node1 = std::make_unique<Node>(1);

    node7->next = node13.get();
    node13->next = node11.get();
    node11->next = node10.get();
    node10->next = node1.get();

    node13->random = node7.get();
    node11->random = node1.get();
    node10->random = node11.get();
    node1->random = node7.get();

    (void)node13.release();
    (void)node11.release();
    (void)node10.release();
    (void)node1.release();
    return node7.release();
}

struct LinkSnapshot {
    Node* node;
    Node* next;
    Node* random;
};

std::vector<LinkSnapshot> snapshotLinks(Node* head) {
    std::vector<LinkSnapshot> snapshot;
    for (Node* current = head; current; current = current->next) {
        snapshot.push_back({current, current->next, current->random});
    }
    return snapshot;
}

bool linksUnchanged(const std::vector<LinkSnapshot>& snapshot) {
    for (const auto& link : snapshot) {
        if (link.node->next != link.next || link.node->random != link.random) {
            return false;
        }
    }
    return true;
}

// 辅助函数：验证复制结果
bool verifyCopy(Node* original, Node* copy) {
    std::vector<Node*> originalNodes;
    std::vector<Node*> copyNodes;

    for (Node* current = original; current; current = current->next) {
        originalNodes.push_back(current);
    }
    for (Node* current = copy; current; current = current->next) {
        copyNodes.push_back(current);
    }

    if (originalNodes.size() != copyNodes.size()) {
        return false;
    }

    std::unordered_map<Node*, std::size_t> originalIndex;
    std::unordered_map<Node*, std::size_t> copyIndex;
    std::unordered_set<Node*> originalSet(originalNodes.begin(), originalNodes.end());

    for (std::size_t i = 0; i < originalNodes.size(); ++i) {
        originalIndex.emplace(originalNodes[i], i);
        copyIndex.emplace(copyNodes[i], i);
    }

    for (std::size_t i = 0; i < originalNodes.size(); ++i) {
        Node* originalNode = originalNodes[i];
        Node* copyNode = copyNodes[i];

        if (originalNode->val != copyNode->val || originalSet.count(copyNode) != 0) {
            return false;
        }

        if (originalNode->random == nullptr) {
            if (copyNode->random != nullptr) {
                return false;
            }
            continue;
        }

        const auto originalTarget = originalIndex.find(originalNode->random);
        const auto copyTarget = copyIndex.find(copyNode->random);
        if (originalTarget == originalIndex.end() || copyTarget == copyIndex.end() ||
            originalTarget->second != copyTarget->second) {
            return false;
        }
    }

    return true;
}

bool expect(bool condition, const std::string& description, int& failures) {
    std::cout << "  " << description << ": " << (condition ? "通过" : "失败") << '\n';
    if (!condition) {
        ++failures;
    }
    return condition;
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
    int failures = 0;

    // 测试1：哈希表法
    {
        std::cout << "测试1: 哈希表法\n";
        Node* original = createTestList();
        Node* copy = solution.copyRandomList(original);

        expect(verifyCopy(original, copy), "值、next/random 拓扑和节点独立性", failures);

        deleteList(original);
        deleteList(copy);
    }

    // 测试2：节点拆分法
    {
        std::cout << "测试2: 节点拆分法 (O(1)空间)\n";
        Node* original = createTestList();
        const auto original_links = snapshotLinks(original);
        Node* copy = solution.copyRandomList_optimized(original);

        expect(verifyCopy(original, copy) && linksUnchanged(original_links),
               "深拷贝且原链表的next/random地址均已恢复", failures);

        deleteList(original);
        deleteList(copy);
    }

    // 测试3：递归法
    {
        std::cout << "测试3: 递归法\n";
        Node* original = createTestList();
        Node* copy = solution.copyRandomList_recursive(original);

        expect(verifyCopy(original, copy), "递归复制可处理 random 形成的环", failures);

        deleteList(original);
        deleteList(copy);
    }

    // 测试4：空链表
    {
        std::cout << "测试4: 空链表\n";
        Node* copy = solution.copyRandomList(nullptr);
        expect(copy == nullptr, "空链表仍为空", failures);
    }

    // 测试5：单节点
    {
        std::cout << "测试5: 单节点\n";
        Node* single = new Node(42);
        single->random = single;

        Node* copy = solution.copyRandomList(single);
        const bool singleCopied = copy != nullptr && copy != single && copy->val == 42 &&
                                  copy->next == nullptr && copy->random == copy;
        expect(singleCopied, "单节点自引用被正确深拷贝", failures);

        delete single;
        delete copy;
    }

    // 测试6：节点分配/构造失败时，穿插法必须回滚原链。
    {
        std::cout << "测试6: bad_alloc回滚\n";
        Node* original = createTestList();
        const auto original_links = snapshotLinks(original);
        bool caught = false;
        Node::failAfterConstructionsForTest(2);
        try {
            (void)solution.copyRandomList_optimized(original);
        } catch (const std::bad_alloc&) {
            caught = true;
        }
        Node::disableConstructionFailureForTest();

        expect(caught && linksUnchanged(original_links),
               "部分穿插后分配失败必须释放副本并恢复全部next/random", failures);
        deleteList(original);
    }

    // 测试7：恰好完成所有副本构造后，拆分提交阶段不得再有可抛节点构造。
    {
        std::cout << "测试7: 最后一次构造后的提交窗口\n";
        Node* original = createTestList();
        const auto original_links = snapshotLinks(original);
        Node::failAfterConstructionsForTest(5);
        Node* copy = nullptr;
        bool threw = false;
        try {
            copy = solution.copyRandomList_optimized(original);
        } catch (...) {
            threw = true;
        }
        Node::disableConstructionFailureForTest();

        expect(!threw && verifyCopy(original, copy) && linksUnchanged(original_links),
               "全部可抛构造完成后只用指针重连提交并恢复输入", failures);
        deleteList(original);
        deleteList(copy);
    }

    if (failures == 0) {
        std::cout << "\n所有测试通过!\n";
        return 0;
    }

    std::cerr << "\n共有 " << failures << " 项检查失败。\n";
    return 1;
}
