/**
 * 栈数据结构演示
 * 展示栈的基本操作和使用场景
 */

#include "stack_demo.h"
#include <iostream>
#include <stack>
#include <vector>
#include <string>

// 使用数组实现一个简单的栈
class MyStack {
private:
    std::vector<int> data;
    
public:
    // 入栈
    void push(int x) {
        data.push_back(x);
        std::cout << "  入栈: " << x << std::endl;
    }
    
    // 出栈
    void pop() {
        if (data.empty()) {
            std::cout << "  栈为空，无法出栈！" << std::endl;
            return;
        }
        std::cout << "  出栈: " << data.back() << std::endl;
        data.pop_back();
    }
    
    // 查看栈顶
    int top() {
        if (data.empty()) {
            std::cout << "  栈为空！" << std::endl;
            return -1;
        }
        return data.back();
    }
    
    // 判空
    bool empty() {
        return data.empty();
    }
    
    // 大小
    size_t size() {
        return data.size();
    }
    
    // 打印栈内容（从底到顶）
    void print() {
        std::cout << "  栈内容（底->顶）: [";
        for (size_t i = 0; i < data.size(); ++i) {
            std::cout << data[i];
            if (i < data.size() - 1) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    }
};

// 演示STL stack
void demoSTLStack() {
    std::cout << "\n--- STL stack 使用 ---" << std::endl;
    
    std::stack<int> s;
    
    // 入栈
    s.push(1);
    s.push(2);
    s.push(3);
    std::cout << "  入栈 1, 2, 3" << std::endl;
    
    // 栈顶
    std::cout << "  栈顶元素: " << s.top() << std::endl;
    
    // 大小
    std::cout << "  栈大小: " << s.size() << std::endl;
    
    // 出栈
    std::cout << "  出栈元素: " << s.top() << std::endl;
    s.pop();
    
    std::cout << "  出栈后栈顶: " << s.top() << std::endl;
    
    // 清空栈
    std::cout << "  清空栈..." << std::endl;
    while (!s.empty()) {
        std::cout << "    弹出: " << s.top() << std::endl;
        s.pop();
    }
    std::cout << "  栈是否为空: " << (s.empty() ? "是" : "否") << std::endl;
}

// 演示自定义栈
void demoMyStack() {
    std::cout << "\n--- 自定义栈演示 ---" << std::endl;
    
    MyStack ms;
    
    ms.push(10);
    ms.push(20);
    ms.push(30);
    ms.print();
    
    std::cout << "  栈顶: " << ms.top() << std::endl;
    std::cout << "  大小: " << ms.size() << std::endl;
    
    ms.pop();
    ms.print();
    
    ms.pop();
    ms.pop();
    std::cout << "  栈是否为空: " << (ms.empty() ? "是" : "否") << std::endl;
}

// 演示栈的应用：反转字符串
void reverseString(const std::string& str) {
    std::cout << "\n--- 使用栈反转字符串 ---" << std::endl;
    std::cout << "  原字符串: " << str << std::endl;
    
    std::stack<char> s;
    
    // 所有字符入栈
    for (char c : str) {
        s.push(c);
    }
    
    // 依次出栈
    std::string reversed;
    while (!s.empty()) {
        reversed += s.top();
        s.pop();
    }
    
    std::cout << "  反转后: " << reversed << std::endl;
}

// 演示栈的应用：浏览器历史记录
void browserHistoryDemo() {
    std::cout << "\n--- 浏览器历史记录模拟 ---" << std::endl;
    
    std::stack<std::string> history;
    
    // 访问页面
    history.push("首页");
    std::cout << "  访问: 首页" << std::endl;
    
    history.push("新闻");
    std::cout << "  访问: 新闻" << std::endl;
    
    history.push("科技");
    std::cout << "  访问: 科技" << std::endl;
    
    // 后退
    std::cout << "\n  点击后退..." << std::endl;
    std::cout << "  从 " << history.top() << " 后退" << std::endl;
    history.pop();
    std::cout << "  当前页面: " << history.top() << std::endl;
    
    std::cout << "  再后退..." << std::endl;
    std::cout << "  从 " << history.top() << " 后退" << std::endl;
    history.pop();
    std::cout << "  当前页面: " << history.top() << std::endl;
}

void stackDemo() {
    std::cout << "╔══════════════════════════════════════╗" << std::endl;
    std::cout << "║        栈 Stack 数据结构             ║" << std::endl;
    std::cout << "║      Last In First Out (LIFO)        ║" << std::endl;
    std::cout << "╚══════════════════════════════════════╝" << std::endl;
    
    demoSTLStack();
    demoMyStack();
    reverseString("Hello World!");
    browserHistoryDemo();
    
    std::cout << "\n栈的特点：" << std::endl;
    std::cout << "  1. 后进先出 (LIFO)" << std::endl;
    std::cout << "  2. 只能在栈顶操作" << std::endl;
    std::cout << "  3. 时间复杂度: push/pop/top 都是 O(1)" << std::endl;
}
