#include "solution.h"

#include <iostream>
#include <vector>

using leetcode_0141::ListNode;
using leetcode_0141::Solution;

namespace {

bool checkCase(const std::vector<int>& values, int cycleEntry, bool expected) {
    Solution solution;
    ListNode* head = cycleEntry < 0
        ? solution.createList(values)
        : solution.createListWithCycle(values, cycleEntry);
    const bool actual = solution.hasCycle(head);
    solution.deleteList(head);

    if (actual != expected) {
        std::cerr << "判环结果不符合预期，入口索引=" << cycleEntry << "\n";
        return false;
    }
    return true;
}

}  // namespace

int main() {
    bool passed = true;
    passed = checkCase({}, -1, false) && passed;
    passed = checkCase({1}, -1, false) && passed;
    passed = checkCase({1}, 0, true) && passed;
    passed = checkCase({3, 2, 0, -4}, 1, true) && passed;
    passed = checkCase({1, 2, 3, 4, 5}, -1, false) && passed;

    std::cout << (passed ? "LeetCode 141 全部测试通过\n"
                         : "LeetCode 141 存在失败用例\n");
    return passed ? 0 : 1;
}
