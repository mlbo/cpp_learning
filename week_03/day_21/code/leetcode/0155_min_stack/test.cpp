/**
 * @file test.cpp
 * @brief LC 155. 最小栈 - 测试文件
 */

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include "solution.h"

// 测试辅助函数
void printTest(const std::string& testName, bool passed) {
    std::cout << "  " << testName << ": ";
    if (passed) {
        std::cout << "✅ 通过";
    } else {
        std::cout << "❌ 失败";
    }
    std::cout << "\n";
}

// 测试方法一：辅助栈法
void testMethod1() {
    std::cout << "方法一：辅助栈法\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    
    MinStack minStack;
    
    // 测试用例来自LeetCode
    // ["MinStack","push","push","push","getMin","pop","top","getMin"]
    // [[],[-2],[0],[-3],[],[],[],[]]
    
    minStack.push(-2);
    minStack.push(0);
    minStack.push(-3);
    
    printTest("getMin() 应返回 -3", minStack.getMin() == -3);
    
    minStack.pop();
    printTest("pop() 后 top() 应返回 0", minStack.top() == 0);
    printTest("getMin() 应返回 -2", minStack.getMin() == -2);
    
    std::cout << "\n";
}

// 测试方法二：单栈pair法
void testMethod2() {
    std::cout << "方法二：单栈pair法\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    
    MinStackPair minStack;
    
    minStack.push(-2);
    minStack.push(0);
    minStack.push(-3);
    
    printTest("getMin() 应返回 -3", minStack.getMin() == -3);
    
    minStack.pop();
    printTest("pop() 后 top() 应返回 0", minStack.top() == 0);
    printTest("getMin() 应返回 -2", minStack.getMin() == -2);
    
    std::cout << "\n";
}

// 测试方法三：差值法
void testMethod3() {
    std::cout << "方法三：差值法\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    
    MinStackDiff minStack;
    
    minStack.push(-2);
    minStack.push(0);
    minStack.push(-3);
    
    printTest("getMin() 应返回 -3", minStack.getMin() == -3);
    
    minStack.pop();
    printTest("pop() 后 top() 应返回 0", minStack.top() == 0);
    printTest("getMin() 应返回 -2", minStack.getMin() == -2);
    
    std::cout << "\n";
}

// 可视化演示
void visualizeOperation() {
    std::cout << "可视化演示：辅助栈的工作原理\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";
    
    MinStack ms;
    
    std::cout << "操作序列演示:\n\n";
    
    // push(5)
    std::cout << "push(5):\n";
    ms.push(5);
    std::cout << "  主栈: [5]           最小栈: [5]     min=5\n";
    std::cout << "  └───┘               └───┘\n\n";
    
    // push(3)
    std::cout << "push(3):\n";
    ms.push(3);
    std::cout << "  主栈: [3, 5]        最小栈: [3, 5]   min=3\n";
    std::cout << "        └───┘               └───┘\n\n";
    
    // push(7)
    std::cout << "push(7):\n";
    ms.push(7);
    std::cout << "  主栈: [7, 3, 5]     最小栈: [3, 3, 5] min=3\n";
    std::cout << "        └───┘               └───┘\n";
    std::cout << "  注意：7比当前最小3大，所以最小栈继续压入3\n\n";
    
    // getMin()
    std::cout << "getMin(): " << ms.getMin() << " ✓ O(1)获取\n\n";
    
    // pop()
    std::cout << "pop(): 弹出7\n";
    ms.pop();
    std::cout << "  主栈: [3, 5]        最小栈: [3, 5]   min=3\n";
    std::cout << "        └───┘               └───┘\n";
    std::cout << "  最小栈同步弹出，min仍为3\n\n";
    
    // push(1)
    std::cout << "push(1):\n";
    ms.push(1);
    std::cout << "  主栈: [1, 3, 5]     最小栈: [1, 3, 5] min=1\n";
    std::cout << "        └───┘               └───┘\n";
    std::cout << "  1比当前最小3更小，更新min为1\n\n";
    
    std::cout << "getMin(): " << ms.getMin() << " ✓ 新的最小值\n\n";
    
    // pop()
    std::cout << "pop(): 弹出1\n";
    ms.pop();
    std::cout << "  最小值恢复为: " << ms.getMin() << " ✓ 自动恢复\n\n";
}

// 复杂度分析
void printComplexity() {
    std::cout << "复杂度分析\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";
    
    std::cout << "方法一（辅助栈法）:\n";
    std::cout << "┌────────────┬──────────┬──────────┐\n";
    std::cout << "│   操作     │ 时间     │ 空间     │\n";
    std::cout << "├────────────┼──────────┼──────────┤\n";
    std::cout << "│  push      │  O(1)    │  O(1)    │\n";
    std::cout << "│  pop       │  O(1)    │  O(1)    │\n";
    std::cout << "│  top       │  O(1)    │  O(1)    │\n";
    std::cout << "│  getMin    │  O(1)    │  O(1)    │\n";
    std::cout << "├────────────┼──────────┼──────────┤\n";
    std::cout << "│  总空间    │    -     │  O(n)    │\n";
    std::cout << "└────────────┴──────────┴──────────┘\n\n";
    
    std::cout << "💡 关键洞察：每个元素\"记住\"它成为栈顶时的最小值\n";
    std::cout << "   出栈时，新的栈顶自然带着它当时的最小值\n\n";
}

// 主测试入口
void testMinStack() {
    std::cout << R"(
    ╔═══════════════════════════════════════════════════════════════╗
    ║               LC 155. 最小栈 - 测试                            ║
    ╠═══════════════════════════════════════════════════════════════╣
    ║                                                               ║
    ║   设计一个栈，支持O(1)时间获取最小元素                        ║
    ║                                                               ║
    ║   核心思想：辅助栈同步记录每个时刻的最小值                    ║
    ║                                                               ║
    ╚═══════════════════════════════════════════════════════════════╝
    )" << "\n";
    
    testMethod1();
    testMethod2();
    testMethod3();
    visualizeOperation();
    printComplexity();
}
