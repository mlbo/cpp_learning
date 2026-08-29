#include "leetcode/0084_largest_rectangle/solution.h"

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
        {{2, 1, 5, 6, 2, 3}, 10, "标准示例"},
        {{2, 4}, 4, "两根递增柱"},
        {{1, 2, 3, 4}, 6, "严格递增"},
        {{4, 3, 2, 1}, 6, "严格递减"},
        {{2, 2, 2}, 6, "相同高度"},
        {{0}, 0, "单个零高度"}
    };

    leetcode_0084::Solution solution;
    int failures = 0;

    for (const auto& test : cases) {
        auto input = test.heights;
        const int actual = solution.largestRectangleArea(input);
        const bool passed = actual == test.expected && input == test.heights;
        std::cout << test.name << ": " << (passed ? "通过" : "失败") << '\n';
        if (!passed) {
            std::cerr << "  期望面积 " << test.expected << "，实际 " << actual
                      << "；算法不应修改输入\n";
            ++failures;
        }
    }

    return failures == 0 ? 0 : 1;
}
