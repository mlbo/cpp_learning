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

MyStack::MyStack() {}

void MyStack::push(int x) {
    q.push(x);
}

int MyStack::pop() {
    // 把前n-1个元素移到队尾
    int n = q.size();
    for (int i = 0; i < n - 1; ++i) {
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

bool MyStack::empty() {
    return q.empty();
}

void testMyStack() {
    std::cout << "LeetCode 225. 用队列实现栈 测试结果：" << std::endl;
    
    MyStack s;
    
    // 测试序列
    std::cout << "  push(1), push(2), push(3)" << std::endl;
    s.push(1);
    s.push(2);
    s.push(3);
    
    std::cout << "  top() = " << s.top() << " (期望: 3)" << std::endl;
    std::cout << "  pop() = " << s.pop() << " (期望: 3)" << std::endl;
    std::cout << "  empty() = " << (s.empty() ? "true" : "false") << " (期望: false)" << std::endl;
    
    std::cout << "  push(4)" << std::endl;
    s.push(4);
    
    std::cout << "  pop() = " << s.pop() << " (期望: 4)" << std::endl;
    std::cout << "  pop() = " << s.pop() << " (期望: 2)" << std::endl;
    std::cout << "  pop() = " << s.pop() << " (期望: 1)" << std::endl;
    std::cout << "  empty() = " << (s.empty() ? "true" : "false") << " (期望: true)" << std::endl;
    
    std::cout << "\n  解题思路：" << std::endl;
    std::cout << "  1. 使用一个队列" << std::endl;
    std::cout << "  2. push：直接入队" << std::endl;
    std::cout << "  3. pop/top：把前n-1个元素移到队尾，最后的就是" << std::endl;
    std::cout << "  4. 时间复杂度：push O(1)，pop/top O(n)" << std::endl;
}
