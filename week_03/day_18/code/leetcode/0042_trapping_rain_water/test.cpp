#include "leetcode/0042_trapping_rain_water/solution.h"

#include <iostream>
#include <string>
#include <vector>

namespace {

struct TestCase {
    std::vector<int> heights;
    int expected;
    std::string name;
};

} // namespace

int main() {
    const std::vector<TestCase> cases = {
        {{}, 0, "空输入"},
        {{1}, 0, "单柱"},
        {{1, 2}, 0, "两柱"},
        {{0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1}, 6, "标准示例一"},
        {{4, 2, 0, 3, 2, 5}, 9, "标准示例二"},
        {{1, 2, 3, 4}, 0, "单调递增"},
        {{4, 3, 2, 1}, 0, "单调递减"},
        {{2, 0, 2}, 2, "单个凹槽"}
    };

    leetcode_0042::Solution solution;
    int failures = 0;

    for (const auto& test : cases) {
        auto input = test.heights;
        const int actual = solution.trap(input);
        const bool passed = actual == test.expected && input == test.heights;
        std::cout << test.name << ": " << (passed ? "通过" : "失败") << '\n';
        if (!passed) {
            std::cerr << "  期望水量 " << test.expected << "，实际 " << actual << '\n';
            ++failures;
        }
    }

    return failures == 0 ? 0 : 1;
}
