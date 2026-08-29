/**
 * LeetCode 20. 有效的括号
 */

#include "solution.h"
#include <iostream>
#include <stack>
#include <unordered_map>
#include <vector>

namespace lc20 {

bool Solution::isValid(const std::string& s) {
    std::stack<char> stk;

    const std::unordered_map<char, char> pairs = {
        {')', '('},
        {']', '['},
        {'}', '{'}
    };

    for (char c : s) {
        const auto closing = pairs.find(c);
        if (closing != pairs.end()) {
            if (stk.empty() || stk.top() != closing->second) {
                return false;
            }
            stk.pop();
        } else if (c == '(' || c == '[' || c == '{') {
            stk.push(c);
        } else {
            // 题目只允许六种括号；工程代码显式拒绝越界输入。
            return false;
        }
    }

    return stk.empty();
}

bool testValidParentheses() {
    Solution sol;

    std::cout << "LeetCode 20. 有效的括号 测试：" << std::endl;

    const std::vector<std::pair<std::string, bool>> tests = {
        {"", true},
        {"()", true},
        {"()[]{}", true},
        {"(]", false},
        {"([)]", false},
        {"{[]}", true},
        {"]", false},
        {"(", false},
        {"a", false}
    };

    bool allPassed = true;
    for (const auto& [input, expected] : tests) {
        const bool result = sol.isValid(input);
        const bool passed = result == expected;
        allPassed = allPassed && passed;
        std::cout << "  \"" << input << "\" -> "
                  << (result ? "true" : "false")
                  << " (" << (passed ? "✓" : "✗") << ")" << std::endl;
    }

    return allPassed;
}

}  // namespace lc20
