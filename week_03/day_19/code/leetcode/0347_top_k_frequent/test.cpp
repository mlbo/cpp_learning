#include "leetcode/0347_top_k_frequent/solution.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <vector>

namespace {

bool sameElements(const std::vector<int>& actual, const std::vector<int>& expected) {
    return actual.size() == expected.size() &&
           std::unordered_set<int>(actual.begin(), actual.end()) ==
               std::unordered_set<int>(expected.begin(), expected.end());
}

} // namespace

int main() {
    leetcode_0347::Solution solution;
    int failures = 0;

    const auto check = [&solution, &failures](std::vector<int> nums, int k,
                                               const std::vector<int>& expected,
                                               const std::string& name) {
        const auto actual = solution.topKFrequent(nums, k);
        const bool passed = sameElements(actual, expected);
        std::cout << name << ": " << (passed ? "通过" : "失败") << '\n';
        if (!passed) ++failures;
    };

    check({1, 1, 1, 2, 2, 3}, 2, {1, 2}, "不同频率的前2名");
    check({1}, 1, {1}, "单元素");
    check({-1, -1, -2, -2, -2, 3}, 2, {-2, -1}, "负数元素");
    check({4, 4, 5, 5, 6, 6}, 3, {4, 5, 6}, "k等于不同元素数");

    for (int invalidK : {0, 4}) {
        std::vector<int> nums = {1, 1, 2, 3};
        bool threw = false;
        try {
            (void)solution.topKFrequent(nums, invalidK);
        } catch (const std::invalid_argument&) {
            threw = true;
        }
        std::cout << "非法k=" << invalidK << ": " << (threw ? "通过" : "失败") << '\n';
        if (!threw) ++failures;
    }

    return failures == 0 ? 0 : 1;
}
