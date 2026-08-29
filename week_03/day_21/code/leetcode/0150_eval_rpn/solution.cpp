/**
 * LeetCode 150. 逆波兰表达式求值
 * 
 * 根据 逆波兰表示法，求表达式的值。
 */

#include "solution.h"
#include <iostream>
#include <cstdint>
#include <limits>
#include <stack>
#include <stdexcept>
#include <string>

namespace leetcode_0150 {

int Solution::evalRPN(const std::vector<std::string>& tokens) {
    std::stack<int> stk;
    
    for (const std::string& token : tokens) {
        if (token == "+" || token == "-" || token == "*" || token == "/") {
            if (stk.size() < 2) {
                throw std::invalid_argument("运算符前缺少两个操作数");
            }
            // 弹出两个操作数
            int b = stk.top(); stk.pop();
            int a = stk.top(); stk.pop();
            
            // 先提升到足以容纳任意两个 int 运算结果的 int64_t，再检查范围，
            // 避免在 int 上先发生有符号溢出未定义行为。
            std::int64_t wideResult = 0;
            if (token == "+") {
                wideResult = static_cast<std::int64_t>(a) + static_cast<std::int64_t>(b);
            } else if (token == "-") {
                wideResult = static_cast<std::int64_t>(a) - static_cast<std::int64_t>(b);
            } else if (token == "*") {
                wideResult = static_cast<std::int64_t>(a) * static_cast<std::int64_t>(b);
            } else {
                if (b == 0) {
                    throw std::domain_error("逆波兰表达式不能除以0");
                }
                wideResult = static_cast<std::int64_t>(a) / static_cast<std::int64_t>(b);
            }

            if (wideResult < std::numeric_limits<int>::min() ||
                wideResult > std::numeric_limits<int>::max()) {
                throw std::overflow_error("逆波兰表达式的中间结果超出int范围");
            }
            const int result = static_cast<int>(wideResult);
            
            stk.push(result);
        } else {
            // 数字入栈
            std::size_t parsed = 0;
            const int value = std::stoi(token, &parsed);
            if (parsed != token.size()) {
                throw std::invalid_argument("token不是完整整数: " + token);
            }
            stk.push(value);
        }
    }

    if (stk.size() != 1) {
        throw std::invalid_argument("表达式结束后必须恰好剩余一个结果");
    }
    return stk.top();
}

} // namespace leetcode_0150

void testEvalRPN() {
    leetcode_0150::Solution sol;
    
    std::cout << "LeetCode 150. 逆波兰表达式求值 测试结果：" << std::endl;
    
    // 测试用例1
    std::vector<std::string> tokens1 = {"2", "1", "+", "3", "*"};
    int result1 = sol.evalRPN(tokens1);
    
    std::cout << "  输入: [\"2\", \"1\", \"+\", \"3\", \"*\"]" << std::endl;
    std::cout << "  计算: (2 + 1) * 3 = " << result1 << " (期望: 9)" << std::endl;
    
    // 测试用例2
    std::vector<std::string> tokens2 = {"4", "13", "5", "/", "+"};
    int result2 = sol.evalRPN(tokens2);
    
    std::cout << "\n  输入: [\"4\", \"13\", \"5\", \"/\", \"+\"]" << std::endl;
    std::cout << "  计算: 4 + (13 / 5) = " << result2 << " (期望: 6)" << std::endl;
    
    // 测试用例3
    std::vector<std::string> tokens3 = {"10", "6", "9", "3", "+", "-11", "*", "/", "*", "17", "+", "5", "+"};
    int result3 = sol.evalRPN(tokens3);
    
    std::cout << "\n  复杂表达式: " << result3 << " (期望: 22)" << std::endl;
    
    // 解题思路
    std::cout << "\n  解题思路：" << std::endl;
    std::cout << "    1. 遍历tokens" << std::endl;
    std::cout << "    2. 遇到数字 -> 入栈" << std::endl;
    std::cout << "    3. 遇到运算符 -> 弹出两个操作数计算，结果入栈" << std::endl;
    std::cout << "    4. 注意：先弹出的是右操作数" << std::endl;
    
    // 图解
    std::cout << "\n  图解 [\"2\", \"1\", \"+\", \"3\", \"*\"]：" << std::endl;
    std::cout << "    读入2: 栈=[2]" << std::endl;
    std::cout << "    读入1: 栈=[2, 1]" << std::endl;
    std::cout << "    读入+: 弹出1和2，计算2+1=3，栈=[3]" << std::endl;
    std::cout << "    读入3: 栈=[3, 3]" << std::endl;
    std::cout << "    读入*: 弹出3和3，计算3*3=9，栈=[9]" << std::endl;
    std::cout << "    结果: 9" << std::endl;
}
