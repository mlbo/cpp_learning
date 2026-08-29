#include "leetcode/0215_kth_largest/solution.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

struct TestCase {
    std::vector<int> nums;
    int k;
    int expected;
    std::string name;
};

} // namespace

int main() {
    const std::vector<TestCase> cases = {
        {{3, 2, 1, 5, 6, 4}, 2, 5, "标准示例"},
        {{3, 2, 3, 1, 2, 4, 5, 5, 6}, 4, 4, "包含重复值"},
        {{-1, -5, -3, -2}, 1, -1, "k=1找最大值"},
        {{-1, -5, -3, -2}, 4, -5, "k=n找最小值"},
        {{2, 2, 2, 2, 2}, 3, 2, "全部相等"}
    };

    leetcode_0215::Solution solution;
    int failures = 0;

    for (const auto& test : cases) {
        auto input = test.nums;
        const int actual = solution.findKthLargest(input, test.k);
        const bool passed = actual == test.expected;
        std::cout << test.name << ": " << (passed ? "通过" : "失败") << '\n';
        if (!passed) {
            std::cerr << "  期望 " << test.expected << "，实际 " << actual << '\n';
            ++failures;
        }
    }

    for (int invalidK : {0, 4}) {
        std::vector<int> nums = {1, 2, 3};
        bool threw = false;
        try {
            (void)solution.findKthLargest(nums, invalidK);
        } catch (const std::invalid_argument&) {
            threw = true;
        }
        std::cout << "非法k=" << invalidK << ": " << (threw ? "通过" : "失败") << '\n';
        if (!threw) ++failures;
    }

    return failures == 0 ? 0 : 1;
}
