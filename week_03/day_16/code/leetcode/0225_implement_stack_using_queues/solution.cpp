/**
 * LeetCode 225. 用队列实现栈
 * 
 * 请你仅使用两个队列实现一个栈，并支持栈的四种操作：
 * - push(x) -- 元素 x 入栈
 * - pop() -- 移除栈顶元素
 * - top() -- 获取栈顶元素
 * - empty() -- 返回栈是否为空
 */

#include "solution.h"
#include <iostream>
#include <stdexcept>

MyStack::MyStack() {}

void MyStack::push(int x) {
    q.push(x);
}

int MyStack::pop() {
    if (q.empty()) {
        throw std::underflow_error("不能从空 MyStack 弹出元素");
    }

    // 把栈顶之前的元素移到队尾；判空后减一不会发生无符号下溢。
    const std::size_t rotations = q.size() - 1;
    for (std::size_t i = 0; i < rotations; ++i) {
        q.push(q.front());
        q.pop();
    }
    
    // 现在队首就是"栈顶"
    int val = q.front();
    q.pop();
    return val;
}

int MyStack::top() {
    int val = pop();
    q.push(val);  // 放回去
    return val;
}

bool MyStack::empty() const noexcept {
    return q.empty();
}

bool testMyStack() {
    std::cout << "LeetCode 225. 用队列实现栈 测试结果：" << std::endl;
    
    MyStack s;
    
    // 测试序列
    std::cout << "  push(1), push(2), push(3)" << std::endl;
    s.push(1);
    s.push(2);
    s.push(3);
    
    bool allPassed = true;
    auto checkInt = [&allPassed](const char* operation, int actual, int expected) {
        const bool passed = actual == expected;
        allPassed = allPassed && passed;
        std::cout << "  " << operation << " = " << actual << " (期望: " << expected
                  << ", " << (passed ? "✓" : "✗") << ")" << std::endl;
    };
    auto checkBool = [&allPassed](const char* operation, bool actual, bool expected) {
        const bool passed = actual == expected;
        allPassed = allPassed && passed;
        std::cout << "  " << operation << " = " << (actual ? "true" : "false")
                  << " (期望: " << (expected ? "true" : "false")
                  << ", " << (passed ? "✓" : "✗") << ")" << std::endl;
    };
    auto checkUnderflow = [&allPassed](const char* operation, auto&& action) {
        bool threw = false;
        try {
            action();
        } catch (const std::underflow_error&) {
            threw = true;
        }
        allPassed = allPassed && threw;
        std::cout << "  " << operation << " (" << (threw ? "✓" : "✗") << ")" << std::endl;
    };

    checkInt("top()", s.top(), 3);
    checkInt("top() 再次调用", s.top(), 3);
    checkInt("pop()", s.pop(), 3);
    checkBool("empty()", s.empty(), false);
    
    std::cout << "  push(4)" << std::endl;
    s.push(4);
    
    checkInt("pop()", s.pop(), 4);
    checkInt("pop()", s.pop(), 2);
    checkInt("pop()", s.pop(), 1);
    checkBool("empty()", s.empty(), true);
    checkUnderflow("空栈 pop() 抛出 underflow_error", [&s]() { (void)s.pop(); });
    checkUnderflow("空栈 top() 抛出 underflow_error", [&s]() { (void)s.top(); });
    
    std::cout << "\n  解题思路：" << std::endl;
    std::cout << "  1. 使用一个队列" << std::endl;
    std::cout << "  2. push：直接入队" << std::endl;
    std::cout << "  3. pop/top：把前n-1个元素移到队尾，最后的就是" << std::endl;
    std::cout << "  4. 时间复杂度：push O(1)，pop/top O(n)" << std::endl;

    return allPassed;
}
