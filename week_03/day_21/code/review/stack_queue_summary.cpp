/**
 * 栈和队列知识点复习
 */

#include "stack_queue_summary.h"
#include <iostream>
#include <stack>
#include <queue>

void stackQueueSummary() {
    std::cout << "╔══════════════════════════════════════╗" << std::endl;
    std::cout << "║        栈和队列知识总结              ║" << std::endl;
    std::cout << "╚══════════════════════════════════════╝" << std::endl;
    
    // ========== 栈的特点 ==========
    std::cout << "\n--- 栈 Stack ---" << std::endl;
    std::cout << "  特点：LIFO（后进先出）" << std::endl;
    std::cout << "  操作：push, pop, top, empty, size" << std::endl;
    
    std::stack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);
    
    std::cout << "  入栈 1, 2, 3 后出栈顺序: ";
    while (!s.empty()) {
        std::cout << s.top() << " ";
        s.pop();
    }
    std::cout << std::endl;
    
    std::cout << "\n  栈的应用：" << std::endl;
    std::cout << "    - 括号匹配（LC 20）" << std::endl;
    std::cout << "    - 表达式求值（LC 150）" << std::endl;
    std::cout << "    - 函数调用栈" << std::endl;
    std::cout << "    - 单调栈（LC 739, 496, 84, 42）" << std::endl;
    
    // ========== 队列的特点 ==========
    std::cout << "\n--- 队列 Queue ---" << std::endl;
    std::cout << "  特点：FIFO（先进先出）" << std::endl;
    std::cout << "  操作：push, pop, front, back, empty, size" << std::endl;
    
    std::queue<int> q;
    q.push(1);
    q.push(2);
    q.push(3);
    
    std::cout << "  入队 1, 2, 3 后出队顺序: ";
    while (!q.empty()) {
        std::cout << q.front() << " ";
        q.pop();
    }
    std::cout << std::endl;
    
    std::cout << "\n  队列的应用：" << std::endl;
    std::cout << "    - BFS层序遍历（LC 102, 107）" << std::endl;
    std::cout << "    - 任务调度" << std::endl;
    std::cout << "    - 消息队列" << std::endl;
    
    // ========== 栈与队列转换 ==========
    std::cout << "\n--- 栈与队列转换 ---" << std::endl;
    std::cout << "  用两个栈实现队列（LC 232）" << std::endl;
    std::cout << "    - 入队栈 + 出队栈" << std::endl;
    std::cout << "    - 均摊O(1)时间复杂度" << std::endl;
    std::cout << "  用一个队列实现栈（LC 225）" << std::endl;
    std::cout << "    - 出队时把前n-1个元素移到队尾" << std::endl;
    
    // ========== 单调栈 ==========
    std::cout << "\n--- 单调栈 ---" << std::endl;
    std::cout << "  不要脱离扫描方向死记“递增找更大/递减找更小”。" << std::endl;
    std::cout << "  写模板前固定回答四个问题：" << std::endl;
    std::cout << "    1. 从左到右还是从右到左扫描？" << std::endl;
    std::cout << "    2. 栈中存值还是下标？需要距离或回填位置时通常存下标。" << std::endl;
    std::cout << "    3. 栈底到栈顶对应值保持什么顺序，是否允许相等？" << std::endl;
    std::cout << "    4. 当前值满足什么严格/非严格条件时弹栈并结算答案？" << std::endl;
    std::cout << "  例：从左到右找右侧第一个严格更大值时，" << std::endl;
    std::cout << "      栈存未结算下标，对应值从栈底到栈顶非递增，" << std::endl;
    std::cout << "      current > value[top] 时弹栈。" << std::endl;
    std::cout << "  时间复杂度：O(n)" << std::endl;
    std::cout << "  模板要点：" << std::endl;
    std::cout << "    while (!stk.empty() && condition) {" << std::endl;
    std::cout << "      // 处理栈顶" << std::endl;
    std::cout << "      stk.pop();" << std::endl;
    std::cout << "    }" << std::endl;
    std::cout << "    stk.push(i);" << std::endl;
    
    // ========== 复杂度总结 ==========
    std::cout << "\n--- 时间复杂度总结 ---" << std::endl;
    std::cout << "  栈操作：push, pop, top 都是 O(1)" << std::endl;
    std::cout << "  队列操作：push, pop, front 都是 O(1)" << std::endl;
    std::cout << "  单调栈：整体 O(n)，每个元素最多入栈出栈各一次" << std::endl;
    std::cout << "  BFS：O(V+E)，V是节点数，E是边数" << std::endl;
}
