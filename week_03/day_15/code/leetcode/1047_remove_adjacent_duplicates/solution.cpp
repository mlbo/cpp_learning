/**
 * LeetCode 1047. 删除字符串中的所有相邻重复项
 */

#include "solution.h"
#include <iostream>
#include <vector>

namespace lc1047 {

std::string Solution::removeDuplicates(const std::string& s) {
    // std::string 的 back/push_back/pop_back 正好提供栈顶操作，
    // 并且避免把字符反复插到字符串头部造成 O(n^2) 搬移。
    std::string result;
    result.reserve(s.size());

    for (char c : s) {
        if (!result.empty() && result.back() == c) {
            result.pop_back();
        } else {
            result.push_back(c);
        }
    }

    return result;
}

bool testRemoveDuplicates() {
    Solution sol;

    std::cout << "LeetCode 1047. 删除相邻重复项 测试：" << std::endl;

    const std::vector<std::pair<std::string, std::string>> tests = {
        {"", ""},
        {"abc", "abc"},
        {"aaaa", ""},
        {"abbaca", "ca"},
        {"azxxzy", "ay"},
        {"aababaab", "ba"}
    };

    bool allPassed = true;
    for (const auto& [input, expected] : tests) {
        const std::string result = sol.removeDuplicates(input);
        const bool passed = result == expected;
        allPassed = allPassed && passed;
        std::cout << "  \"" << input << "\" -> \"" << result
                  << "\" (" << (passed ? "✓" : "✗") << ")" << std::endl;
    }

    return allPassed;
}

}  // namespace lc1047
