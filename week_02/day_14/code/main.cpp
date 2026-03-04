/**
 * @file main.cpp
 * @brief Day 14: 第二周复习与综合练习 - 主程序
 *
 * 本程序整合：
 * 1. 第二周知识点总结
 * 2. EMC++条款17-22复习
 * 3. 综合练习题
 * 4. 线程安全链表项目
 * 5. LeetCode 234 & 138 题解演示
 */

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>

// 复习模块
#include "review/week2_summary.cpp"
#include "review/emcpp_review.cpp"
#include "review/exercises.cpp"

// 项目模块
#include "project/thread_safe_list.h"

// LeetCode 模块
#include "leetcode/0234_palindrome/solution.h"
#include "leetcode/0138_copy_random/solution.h"

// ============================================================
// 线程安全链表演示
// ============================================================

void demo_thread_safe_list() {
    std::cout << "\n=== 线程安全链表演示 ===\n";

    ThreadSafeList<int> list;

    // 多线程插入
    const int num_threads = 4;
    const int items_per_thread = 100;

    std::vector<std::thread> threads;
    for (int t = 0; t < num_threads; ++t) {
        threads.emplace_back([&list, t, items_per_thread]() {
            for (int i = 0; i < items_per_thread; ++i) {
                list.push_front(t * 1000 + i);
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "After concurrent inserts, size: " << list.size() << "\n";

    // 遍历
    int count = 0;
    list.for_each([&count](int val) {
        count++;
    });
    std::cout << "Counted " << count << " elements\n";

    // 查找
    auto found = list.find_first_if([](int val) {
        return val == 0;
    });
    if (found) {
        std::cout << "Found value: " << *found << "\n";
    }

    // 删除
    size_t removed = list.remove_if([](int val) {
        return val % 2 == 0;
    });
    std::cout << "Removed " << removed << " even numbers\n";
    std::cout << "After removal, size: " << list.size() << "\n";
}

// ============================================================
// LeetCode 234 回文链表演示
// ============================================================

void demo_leetcode_234() {
    std::cout << "\n=== LeetCode 234: 回文链表演示 ===\n";

    Solution solution;

    // 测试用例1: 1->2->2->1 (回文)
    ListNode* test1 = new ListNode(1);
    test1->next = new ListNode(2);
    test1->next->next = new ListNode(2);
    test1->next->next->next = new ListNode(1);

    std::cout << "Test case 1: [1,2,2,1]\n";
    std::cout << "Result (快慢指针法): " << (solution.isPalindrome(test1) ? "true" : "false") << "\n";
    std::cout << "Result (数组法): " << (solution.isPalindrome_array(test1) ? "true" : "false") << "\n";

    // 释放内存
    while (test1) {
        ListNode* temp = test1;
        test1 = test1->next;
        delete temp;
    }

    // 测试用例2: 1->2 (非回文)
    ListNode* test2 = new ListNode(1);
    test2->next = new ListNode(2);

    std::cout << "\nTest case 2: [1,2]\n";
    std::cout << "Result: " << (solution.isPalindrome(test2) ? "true" : "false") << "\n";

    while (test2) {
        ListNode* temp = test2;
        test2 = test2->next;
        delete temp;
    }

    // 测试用例3: 单节点
    ListNode* test3 = new ListNode(1);
    std::cout << "\nTest case 3: [1]\n";
    std::cout << "Result: " << (solution.isPalindrome(test3) ? "true" : "false") << "\n";
    delete test3;
}

// ============================================================
// LeetCode 138 随机链表复制演示
// ============================================================

void demo_leetcode_138() {
    std::cout << "\n=== LeetCode 138: 随机链表复制演示 ===\n";

    // 创建测试链表
    // 节点关系: 7 -> 13 -> 11 -> 10 -> 1
    // random: 7.random=null, 13.random=7, 11.random=1, 10.random=11, 1.random=7

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

    std::cout << "Original list created\n";

    Solution solution;

    // 方法1: 哈希表法
    Node* copy1 = solution.copyRandomList(node7);
    std::cout << "Copy created (hash map method)\n";
    if (copy1 && copy1->next && copy1->next->random) {
        std::cout << "Verification: copy->next->random->val = " << copy1->next->random->val << " (expected 7)\n";
    }

    // 方法2: 节点拆分法
    Node* copy2 = solution.copyRandomList_optimized(node7);
    std::cout << "Copy created (optimized method, O(1) space)\n";
    if (copy2 && copy2->next && copy2->next->random) {
        std::cout << "Verification: copy->next->random->val = " << copy2->next->random->val << " (expected 7)\n";
    }

    // 清理原链表
    Node* curr = node7;
    while (curr) {
        Node* temp = curr;
        curr = curr->next;
        delete temp;
    }

    // 清理复制链表
    curr = copy1;
    while (curr) {
        Node* temp = curr;
        curr = curr->next;
        delete temp;
    }

    curr = copy2;
    while (curr) {
        Node* temp = curr;
        curr = curr->next;
        delete temp;
    }
}

// ============================================================
// 主菜单
// ============================================================

void print_menu() {
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════════╗\n";
    std::cout << "║        Day 14: 第二周复习与综合练习                     ║\n";
    std::cout << "╠══════════════════════════════════════════════════════╣\n";
    std::cout << "║  1. 第二周知识点总结                                   ║\n";
    std::cout << "║  2. EMC++条款17-22复习                                 ║\n";
    std::cout << "║  3. 综合练习题                                         ║\n";
    std::cout << "║  4. 线程安全链表演示                                   ║\n";
    std::cout << "║  5. LeetCode 234: 回文链表                            ║\n";
    std::cout << "║  6. LeetCode 138: 随机链表复制                        ║\n";
    std::cout << "║  7. 运行全部演示                                       ║\n";
    std::cout << "║  0. 退出                                               ║\n";
    std::cout << "╚══════════════════════════════════════════════════════╝\n";
    std::cout << "请选择 [0-7]: ";
}

void run_all() {
    std::cout << "\n╔══════════════════════════════════════════════════════╗\n";
    std::cout << "║             运行全部演示                               ║\n";
    std::cout << "╚══════════════════════════════════════════════════════╝\n";

    week2::run_week2_summary();
    emcpp::run_emcpp_review();
    exercises::run_exercises();
    demo_thread_safe_list();
    demo_leetcode_234();
    demo_leetcode_138();

    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════════╗\n";
    std::cout << "║            恭喜完成第二周复习！                         ║\n";
    std::cout << "╚══════════════════════════════════════════════════════╝\n";
}

int main(int argc, char* argv[]) {
    // 如果有命令行参数，直接运行全部
    if (argc > 1 && std::string(argv[1]) == "--all") {
        run_all();
        return 0;
    }

    int choice;
    while (true) {
        print_menu();
        std::cin >> choice;

        switch (choice) {
            case 1:
                week2::run_week2_summary();
                break;
            case 2:
                emcpp::run_emcpp_review();
                break;
            case 3:
                exercises::run_exercises();
                break;
            case 4:
                demo_thread_safe_list();
                break;
            case 5:
                demo_leetcode_234();
                break;
            case 6:
                demo_leetcode_138();
                break;
            case 7:
                run_all();
                break;
            case 0:
                std::cout << "\n感谢使用，再见！\n";
                return 0;
            default:
                std::cout << "无效选择，请重试。\n";
        }
    }

    return 0;
}
