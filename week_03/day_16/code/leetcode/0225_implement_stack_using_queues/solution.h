#ifndef SOLUTION_225_H
#define SOLUTION_225_H

#include <queue>

class MyStack {
private:
    std::queue<int> q;
    
public:
    MyStack();
    void push(int x);
    int pop();
    int top();
    bool empty() const noexcept;
};

bool testMyStack();

#endif // SOLUTION_225_H
