/**
 * C++ priority_queue 演示
 * STL优先队列的使用
 */

#include "priority_queue_demo.h"
#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <functional>

// 自定义类型
struct Task {
    std::string name;
    int priority;
    
    Task(const std::string& n, int p) : name(n), priority(p) {}
};

// 自定义比较器（优先级高的先出）
struct TaskComparator {
    bool operator()(const Task& a, const Task& b) const {
        return a.priority < b.priority;  // 优先级小的排后面
    }
};

void priorityQueueDemo() {
    std::cout << "╔══════════════════════════════════════╗" << std::endl;
    std::cout << "║        priority_queue 优先队列       ║" << std::endl;
    std::cout << "╚══════════════════════════════════════╝" << std::endl;
    
    // ========== 1. 大顶堆（默认） ==========
    std::cout << "\n--- 1. 大顶堆（默认） ---" << std::endl;
    
    std::priority_queue<int> maxHeap;
    
    maxHeap.push(3);
    maxHeap.push(1);
    maxHeap.push(4);
    maxHeap.push(1);
    maxHeap.push(5);
    
    std::cout << "  入队: 3, 1, 4, 1, 5" << std::endl;
    std::cout << "  出队顺序: ";
    while (!maxHeap.empty()) {
        std::cout << maxHeap.top() << " ";
        maxHeap.pop();
    }
    std::cout << std::endl;
    
    // ========== 2. 小顶堆 ==========
    std::cout << "\n--- 2. 小顶堆 ---" << std::endl;
    
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;
    
    minHeap.push(3);
    minHeap.push(1);
    minHeap.push(4);
    minHeap.push(1);
    minHeap.push(5);
    
    std::cout << "  入队: 3, 1, 4, 1, 5" << std::endl;
    std::cout << "  出队顺序: ";
    while (!minHeap.empty()) {
        std::cout << minHeap.top() << " ";
        minHeap.pop();
    }
    std::cout << std::endl;
    
    // ========== 3. 自定义比较器 ==========
    std::cout << "\n--- 3. 自定义比较器（任务调度） ---" << std::endl;
    
    std::priority_queue<Task, std::vector<Task>, TaskComparator> taskQueue;
    
    taskQueue.push(Task("低优先级任务", 1));
    taskQueue.push(Task("高优先级任务", 10));
    taskQueue.push(Task("中优先级任务", 5));
    taskQueue.push(Task("紧急任务", 100));
    
    std::cout << "  任务执行顺序:" << std::endl;
    while (!taskQueue.empty()) {
        std::cout << "    " << taskQueue.top().name 
                  << " (优先级: " << taskQueue.top().priority << ")" << std::endl;
        taskQueue.pop();
    }
    
    // ========== 4. Lambda比较器 ==========
    std::cout << "\n--- 4. Lambda比较器 ---" << std::endl;
    
    auto cmp = [](int a, int b) { return a > b; };  // 小顶堆
    std::priority_queue<int, std::vector<int>, decltype(cmp)> customHeap(cmp);
    
    customHeap.push(5);
    customHeap.push(2);
    customHeap.push(8);
    
    std::cout << "  Lambda小顶堆: ";
    while (!customHeap.empty()) {
        std::cout << customHeap.top() << " ";
        customHeap.pop();
    }
    std::cout << std::endl;
    
    // ========== 5. 常用操作 ==========
    std::cout << "\n--- 5. 常用操作 ---" << std::endl;
    
    std::priority_queue<int> pq;
    
    pq.push(10);
    pq.push(20);
    pq.push(30);
    
    std::cout << "  top(): " << pq.top() << std::endl;
    std::cout << "  size(): " << pq.size() << std::endl;
    std::cout << "  empty(): " << (pq.empty() ? "true" : "false") << std::endl;
    
    // ========== 6. 使用场景 ==========
    std::cout << "\n--- 6. 使用场景 ---" << std::endl;
    std::cout << "  1. Top K问题：维护K个大小的堆" << std::endl;
    std::cout << "  2. 任务调度：按优先级执行" << std::endl;
    std::cout << "  3. 合并有序链表：多路归并" << std::endl;
    std::cout << "  4. 中位数：双堆法" << std::endl;
}
