#ifndef SOLUTION_155_H
#define SOLUTION_155_H

class MinStack {
public:
    MinStack();
    ~MinStack();

    MinStack(const MinStack&) = delete;
    MinStack& operator=(const MinStack&) = delete;
    MinStack(MinStack&&) = delete;
    MinStack& operator=(MinStack&&) = delete;

    void push(int val);
    void pop();
    int top() const;
    int getMin() const;
    bool empty() const noexcept;
    
private:
    struct Node {
        int val;
        int minVal;
        Node* next;
        Node(int v, int m, Node* n) : val(v), minVal(m), next(n) {}
    };
    Node* head;
};

void testMinStack();

#endif // SOLUTION_155_H
