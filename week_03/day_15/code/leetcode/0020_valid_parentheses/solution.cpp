/**
 * LeetCode 20. 有效的括号
 */

#include "solution.h"
#include <iostream>
#include <stack>
#include <unordered_map>

bool Solution::isValid(std::string s) {
    std::stack<char> stk;
    
    std::unordered_map<char, char> pairs = {
        {')', '('},
        {']', '['},
        {'}', '{'}
    };
    
    for (char c : s) {
        if (pairs.count(c)) {
            if (stk.empty() || stk.top() != pairs[c]) {
                return false;
            }
            stk.pop();
        } else {
            stk.push(c);
        }
    }
    
    return stk.empty();
}

void testValidParentheses() {
    Solution sol;
    
    std::cout << "LeetCode 20. 有效的括号 测试：" << std::endl;
    
    std::vector<std::pair<std::string, bool>> tests = {
        {"()", true},
        {"()[]{}", true},
        {"(]", false},
        {"([)]", false},
        {"{[]}", true}
    };
    
    for (const auto& [input, expected] : tests) {
        bool result = sol.isValid(input);
        std::cout << "  \"" << input << "\" -> " 
                  << (result ? "true" : "false")
                  << " (" << (result == expected ? "✓" : "✗") << ")" << std::endl;
    }
}
