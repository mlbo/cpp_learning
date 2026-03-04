/**
 * LeetCode 155. 最小栈
 * 
 * 设计一个支持 push，pop，top 操作，并能在常数时间内检索到最小元素的栈。
 */

#include "solution.h"
#include <iostream>
#include <stack>
#include <climits>

// 方法1：使用链表节点存储最小值
MinStack::MinStack() : head(nullptr) {}

void MinStack::push(int val) {
    if (head == nullptr) {
        head = new Node(val, val, nullptr);
    } else {
        head = new Node(val, std::min(val, head->minVal), head);
    }
}

void MinStack::pop() {
    Node* temp = head;
    head = head->next;
    delete temp;
}

int MinStack::top() {
    return head->val;
}

int MinStack::getMin() {
    return head->minVal;
}

// 方法2：使用双栈（更直观）
class MinStack2 {
private:
    std::stack<int> dataStack;
    std::stack<int> minStack;
    
public:
    MinStack2() {
        minStack.push(INT_MAX);
    }
    
    void push(int val) {
        dataStack.push(val);
        minStack.push(std::min(val, minStack.top()));
    }
    
    void pop() {
        dataStack.pop();
        minStack.pop();
    }
    
    int top() {
        return dataStack.top();
    }
    
    int getMin() {
        return minStack.top();
    }
};

void testMinStack() {
    std::cout << "LeetCode 155. 最小栈 测试结果：" << std::endl;
    
    MinStack2 ms;
    
    std::cout << "  操作序列：" << std::endl;
    
    ms.push(-2);
    std::cout << "    push(-2), min = " << ms.getMin() << std::endl;
    
    ms.push(0);
    std::cout << "    push(0), min = " << ms.getMin() << std::endl;
    
    ms.push(-3);
    std::cout << "    push(-3), min = " << ms.getMin() << std::endl;
    
    std::cout << "    getMin() = " << ms.getMin() << " (期望: -3)" << std::endl;
    
    ms.pop();
    std::cout << "    pop(), min = " << ms.getMin() << std::endl;
    
    std::cout << "    top() = " << ms.top() << " (期望: 0)" << std::endl;
    std::cout << "    getMin() = " << ms.getMin() << " (期望: -2)" << std::endl;
    
    std::cout << "\n  解题思路：" << std::endl;
    std::cout << "    1. 使用辅助栈同步记录每个状态的最小值" << std::endl;
    std::cout << "    2. push时，minStack push当前最小值" << std::endl;
    std::cout << "    3. pop时，两个栈同时pop" << std::endl;
    std::cout << "    4. getMin直接返回minStack栈顶" << std::endl;
}
