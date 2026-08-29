#ifndef SOLUTION_232_H
#define SOLUTION_232_H

#include <stack>

class MyQueue {
private:
    std::stack<int> inStack;
    std::stack<int> outStack;
    
    void transfer();
    
public:
    MyQueue();
    void push(int x);
    int pop();
    int peek();
    bool empty() const noexcept;
};

bool testMyQueue();

#endif // SOLUTION_232_H
