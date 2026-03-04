/**
 * LeetCode 1047. 删除字符串中的所有相邻重复项
 */

#include "solution.h"
#include <iostream>
#include <stack>

std::string Solution::removeDuplicates(std::string s) {
    std::stack<char> stk;
    
    for (char c : s) {
        if (stk.empty() || stk.top() != c) {
            stk.push(c);
        } else {
            stk.pop();
        }
    }
    
    std::string result;
    while (!stk.empty()) {
        result = stk.top() + result;
        stk.pop();
    }
    
    return result;
}

void testRemoveDuplicates() {
    Solution sol;
    
    std::cout << "LeetCode 1047. 删除相邻重复项 测试：" << std::endl;
    
    std::vector<std::pair<std::string, std::string>> tests = {
        {"abbaca", "ca"},
        {"azxxzy", "ay"},
        {"aababaab", "ba"}
    };
    
    for (const auto& [input, expected] : tests) {
        std::string result = sol.removeDuplicates(input);
        std::cout << "  \"" << input << "\" -> \"" << result 
                  << "\" (" << (result == expected ? "✓" : "✗") << ")" << std::endl;
    }
}
