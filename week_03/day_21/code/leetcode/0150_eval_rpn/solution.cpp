/**
 * LeetCode 150. 逆波兰表达式求值
 * 
 * 根据 逆波兰表示法，求表达式的值。
 */

#include "solution.h"
#include <iostream>
#include <stack>
#include <string>

int Solution::evalRPN(std::vector<std::string>& tokens) {
    std::stack<int> stk;
    
    for (const std::string& token : tokens) {
        if (token == "+" || token == "-" || token == "*" || token == "/") {
            // 弹出两个操作数
            int b = stk.top(); stk.pop();
            int a = stk.top(); stk.pop();
            
            int result;
            if (token == "+") result = a + b;
            else if (token == "-") result = a - b;
            else if (token == "*") result = a * b;
            else result = a / b;  // 整数除法
            
            stk.push(result);
        } else {
            // 数字入栈
            stk.push(std::stoi(token));
        }
    }
    
    return stk.top();
}

void testEvalRPN() {
    Solution sol;
    
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
