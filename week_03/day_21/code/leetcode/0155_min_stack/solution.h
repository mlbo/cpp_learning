#ifndef SOLUTION_155_H
#define SOLUTION_155_H

class MinStack {
public:
    MinStack();
    void push(int val);
    void pop();
    int top();
    int getMin();
    
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
