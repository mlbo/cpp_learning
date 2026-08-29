/**
 * LeetCode 232. 用栈实现队列
 * 
 * 请你仅使用两个栈实现一个队列，并支持队列的四种操作：
 * - push(x) -- 将一个元素放入队列的尾部
 * - pop() -- 从队列首部移除元素
 * - peek() -- 返回队列首部的元素
 * - empty() -- 返回队列是否为空
 */

#include "solution.h"
#include <iostream>
#include <stdexcept>

MyQueue::MyQueue() {}

void MyQueue::transfer() {
    // 将入队栈的元素全部倒入出队栈
    while (!inStack.empty()) {
        outStack.push(inStack.top());
        inStack.pop();
    }
}

void MyQueue::push(int x) {
    inStack.push(x);
}

int MyQueue::pop() {
    if (outStack.empty()) {
        transfer();
    }
    if (outStack.empty()) {
        throw std::underflow_error("不能从空 MyQueue 弹出元素");
    }
    int val = outStack.top();
    outStack.pop();
    return val;
}

int MyQueue::peek() {
    if (outStack.empty()) {
        transfer();
    }
    if (outStack.empty()) {
        throw std::underflow_error("空 MyQueue 没有队首元素");
    }
    return outStack.top();
}

bool MyQueue::empty() const noexcept {
    return inStack.empty() && outStack.empty();
}

bool testMyQueue() {
    std::cout << "LeetCode 232. 用栈实现队列 测试结果：" << std::endl;
    
    MyQueue q;
    
    // 测试序列
    std::cout << "  push(1), push(2), push(3)" << std::endl;
    q.push(1);
    q.push(2);
    q.push(3);
    
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

    checkInt("peek()", q.peek(), 1);
    checkInt("pop()", q.pop(), 1);
    checkBool("empty()", q.empty(), false);
    
    std::cout << "  push(4)" << std::endl;
    q.push(4);
    
    checkInt("pop()", q.pop(), 2);
    checkInt("pop()", q.pop(), 3);
    checkInt("pop()", q.pop(), 4);
    checkBool("empty()", q.empty(), true);
    
    std::cout << "\n  解题思路：" << std::endl;
    std::cout << "  1. 使用两个栈：inStack（入队）和outStack（出队）" << std::endl;
    std::cout << "  2. push：直接压入inStack" << std::endl;
    std::cout << "  3. pop/peek：如果outStack为空，把inStack倒入outStack" << std::endl;
    std::cout << "  4. 每个元素最多被转移一次，均摊O(1)" << std::endl;

    return allPassed;
}
