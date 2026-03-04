/**
 * 知识体系总结演示
 */

#include <iostream>
#include <vector>
#include <string>

void dataStructureSummary() {
    std::cout << "=== 数据结构知识体系 ===" << std::endl;
    
    std::cout << "\n数组 (Array):" << std::endl;
    std::cout << "  - 连续内存存储，O(1)随机访问" << std::endl;
    std::cout << "  - 插入删除O(n)，需要移动元素" << std::endl;
    
    std::cout << "\n链表 (Linked List):" << std::endl;
    std::cout << "  - 指针连接，O(1)插入删除" << std::endl;
    std::cout << "  - O(n)随机访问" << std::endl;
    
    std::cout << "\n栈 (Stack):" << std::endl;
    std::cout << "  - LIFO，O(1)入栈出栈" << std::endl;
    std::cout << "  - 应用：表达式求值、括号匹配" << std::endl;
    
    std::cout << "\n队列 (Queue):" << std::endl;
    std::cout << "  - FIFO，O(1)入队出队" << std::endl;
    std::cout << "  - 应用：BFS、任务调度" << std::endl;
    
    std::cout << "\n哈希表 (Hash Table):" << std::endl;
    std::cout << "  - 平均O(1)查找插入删除" << std::endl;
    std::cout << "  - 不支持有序遍历" << std::endl;
    
    std::cout << "\n树 (Tree):" << std::endl;
    std::cout << "  - 层次结构，递归定义" << std::endl;
    std::cout << "  - BST: O(log n)查找" << std::endl;
}

int main() {
    dataStructureSummary();
    return 0;
}
