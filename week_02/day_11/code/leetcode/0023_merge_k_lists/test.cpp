/**
 * @file test.cpp
 * @brief LeetCode 23 测试文件
 */

#include "solution.h"

#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

using namespace leetcode_0023;

namespace {

struct TestCase {
    std::vector<std::vector<int>> input;
    std::vector<int> expected;
    std::string description;
};

struct Method {
    const char* name;
    std::function<ListNode*(Solution&, std::vector<ListNode*>&)> run;
};

std::vector<ListNode*> createLists(const std::vector<std::vector<int>>& input) {
    std::vector<ListNode*> lists;
    lists.reserve(input.size());
    try {
        for (const auto& values : input) {
            lists.push_back(createList(values));
        }
    } catch (...) {
        for (ListNode* head : lists) {
            deleteList(head);
        }
        throw;
    }
    return lists;
}

const std::vector<Method>& methods() {
    static const std::vector<Method> all_methods = {
        {"分治", [](Solution& solution, std::vector<ListNode*>& lists) {
             return solution.mergeKLists(lists);
         }},
        {"最小堆", [](Solution& solution, std::vector<ListNode*>& lists) {
             return solution.mergeKListsPriorityQueue(lists);
         }},
        {"顺序合并", [](Solution& solution, std::vector<ListNode*>& lists) {
             return solution.mergeKListsSequential(lists);
         }},
    };
    return all_methods;
}

bool runTest(Solution& solution, const TestCase& test_case, int test_number) {
    std::cout << "测试 " << test_number << ": " << test_case.description << '\n';
    bool passed = true;

    for (const auto& method : methods()) {
        auto lists = createLists(test_case.input);
        ListNode* result = method.run(solution, lists);
        const std::vector<int> actual = listToVector(result);
        const bool method_passed = actual == test_case.expected;

        std::cout << "  " << method.name << ": "
                  << (method_passed ? "通过" : "失败") << '\n';
        if (!method_passed) {
            std::cout << "    实际结果: ";
            printList(result);
            std::cout << '\n';
            passed = false;
        }

        deleteList(result);
    }

    return passed;
}

bool compareMethods() {
    std::cout << "\n=== 三种方法对比（计时只用于观察，不作为性能结论） ===\n";
    Solution solution;

    std::vector<std::vector<int>> input;
    std::vector<int> expected;
    for (int list_index = 0; list_index < 100; ++list_index) {
        std::vector<int> values;
        for (int item = 0; item < 100; ++item) {
            const int value = item * 100 + list_index;
            values.push_back(value);
            expected.push_back(value);
        }
        input.push_back(std::move(values));
    }
    std::sort(expected.begin(), expected.end());

    bool all_correct = true;
    for (const auto& method : methods()) {
        auto lists = createLists(input);
        const auto start = std::chrono::steady_clock::now();
        ListNode* result = method.run(solution, lists);
        const auto end = std::chrono::steady_clock::now();

        const std::vector<int> actual = listToVector(result);
        const bool correct = actual == expected && std::is_sorted(actual.begin(), actual.end());
        const auto duration =
            std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "  " << method.name << ": " << duration.count() << " us, "
                  << (correct ? "结果正确" : "结果错误") << '\n';

        all_correct = all_correct && correct;
        deleteList(result);
    }
    return all_correct;
}

} // namespace

int main() {
    std::cout << "=== LeetCode 23: 合并K个升序链表 ===\n\n";
    Solution solution;

    const std::vector<TestCase> test_cases = {
        {{{1, 4, 5}, {1, 3, 4}, {2, 6}},
         {1, 1, 2, 3, 4, 4, 5, 6},
         "标准情况"},
        {{}, {}, "K=0：空数组"},
        {{{}}, {}, "K=1：单个空链表"},
        {{{}, {}, {}, {}}, {}, "很多空链表"},
        {{{1, 2, 3}}, {1, 2, 3}, "单个非空链表"},
        {{{1, 2}, {3, 4}, {5, 6}}, {1, 2, 3, 4, 5, 6}, "值域不重叠"},
        {{{-10, -5, 0}, {-8, -3, 2}, {-6, 1, 7}},
         {-10, -8, -6, -5, -3, 0, 1, 2, 7},
         "包含负数"},
        {{{1}, {0}, {-1}}, {-1, 0, 1}, "单节点链表"},
        {{{1, 1, 1}, {}, {1, 1}, {1}}, {1, 1, 1, 1, 1, 1}, "重复值夹杂空链表"},
    };

    int failures = 0;
    for (std::size_t i = 0; i < test_cases.size(); ++i) {
        if (!runTest(solution, test_cases[i], static_cast<int>(i + 1))) {
            ++failures;
        }
    }

    if (!compareMethods()) {
        ++failures;
    }

    if (failures != 0) {
        std::cerr << "\n共有 " << failures << " 组检查失败\n";
        return 1;
    }

    std::cout << "\n所有方法与边界测试通过！\n";
    return 0;
}
