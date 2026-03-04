/**
 * 队列数据结构演示
 * 展示队列的基本操作和使用场景
 */

#include "queue_demo.h"
#include <iostream>
#include <queue>
#include <deque>
#include <list>
#include <string>

// 使用链表实现一个简单的队列
template<typename T>
class MyQueue {
private:
    std::list<T> data;
    
public:
    // 入队
    void push(const T& x) {
        data.push_back(x);
        std::cout << "  入队: " << x << std::endl;
    }
    
    // 出队
    void pop() {
        if (data.empty()) {
            std::cout << "  队列为空，无法出队！" << std::endl;
            return;
        }
        std::cout << "  出队: " << data.front() << std::endl;
        data.pop_front();
    }
    
    // 查看队首
    T front() {
        if (data.empty()) {
            std::cout << "  队列为空！" << std::endl;
            return T();
        }
        return data.front();
    }
    
    // 查看队尾
    T back() {
        if (data.empty()) {
            std::cout << "  队列为空！" << std::endl;
            return T();
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
    
    // 打印队列内容（从队首到队尾）
    void print() {
        std::cout << "  队列内容（首->尾）: [";
        bool first = true;
        for (const T& item : data) {
            if (!first) std::cout << ", ";
            std::cout << item;
            first = false;
        }
        std::cout << "]" << std::endl;
    }
};

// 演示STL queue
void demoSTLQueue() {
    std::cout << "\n--- STL queue 使用 ---" << std::endl;
    
    std::queue<int> q;
    
    // 入队
    q.push(1);
    q.push(2);
    q.push(3);
    std::cout << "  入队 1, 2, 3" << std::endl;
    
    // 队首和队尾
    std::cout << "  队首元素: " << q.front() << std::endl;
    std::cout << "  队尾元素: " << q.back() << std::endl;
    
    // 大小
    std::cout << "  队列大小: " << q.size() << std::endl;
    
    // 出队
    std::cout << "  出队元素: " << q.front() << std::endl;
    q.pop();
    
    std::cout << "  出队后队首: " << q.front() << std::endl;
    
    // 清空队列
    std::cout << "  清空队列..." << std::endl;
    while (!q.empty()) {
        std::cout << "    出队: " << q.front() << std::endl;
        q.pop();
    }
    std::cout << "  队列是否为空: " << (q.empty() ? "是" : "否") << std::endl;
}

// 演示自定义队列
void demoMyQueue() {
    std::cout << "\n--- 自定义队列演示 ---" << std::endl;
    
    MyQueue<std::string> mq;
    
    mq.push("小明");
    mq.push("小红");
    mq.push("小刚");
    mq.print();
    
    std::cout << "  队首: " << mq.front() << std::endl;
    std::cout << "  队尾: " << mq.back() << std::endl;
    std::cout << "  大小: " << mq.size() << std::endl;
    
    mq.pop();
    mq.print();
    
    mq.pop();
    mq.pop();
    std::cout << "  队列是否为空: " << (mq.empty() ? "是" : "否") << std::endl;
}

// 演示双端队列 deque
void demoDeque() {
    std::cout << "\n--- 双端队列 deque ---" << std::endl;
    
    std::deque<int> dq;
    
    // 两端都可以添加
    dq.push_back(2);
    dq.push_front(1);
    dq.push_back(3);
    dq.push_front(0);
    
    std::cout << "  双端队列: ";
    for (int n : dq) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
    
    std::cout << "  首元素: " << dq.front() << std::endl;
    std::cout << "  尾元素: " << dq.back() << std::endl;
    
    // 两端都可以删除
    dq.pop_front();
    dq.pop_back();
    
    std::cout << "  删除首尾后: ";
    for (int n : dq) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
}

// 演示队列应用：模拟打印任务
void printerQueueDemo() {
    std::cout << "\n--- 打印队列模拟 ---" << std::endl;
    
    std::queue<std::string> printQueue;
    
    // 提交打印任务
    printQueue.push("文档1.pdf");
    printQueue.push("图片.png");
    printQueue.push("报告.docx");
    std::cout << "  已提交3个打印任务" << std::endl;
    
    // 处理打印任务（先进先出）
    int jobNum = 1;
    while (!printQueue.empty()) {
        std::cout << "  正在打印任务" << jobNum++ << ": " << printQueue.front() << std::endl;
        printQueue.pop();
    }
    std::cout << "  所有打印任务完成！" << std::endl;
}

void queueDemo() {
    std::cout << "╔══════════════════════════════════════╗" << std::endl;
    std::cout << "║        队列 Queue 数据结构           ║" << std::endl;
    std::cout << "║      First In First Out (FIFO)       ║" << std::endl;
    std::cout << "╚══════════════════════════════════════╝" << std::endl;
    
    demoSTLQueue();
    demoMyQueue();
    demoDeque();
    printerQueueDemo();
    
    std::cout << "\n队列的特点：" << std::endl;
    std::cout << "  1. 先进先出 (FIFO)" << std::endl;
    std::cout << "  2. 队尾入队，队首出队" << std::endl;
    std::cout << "  3. 时间复杂度: push/pop/front/back 都是 O(1)" << std::endl;
}
