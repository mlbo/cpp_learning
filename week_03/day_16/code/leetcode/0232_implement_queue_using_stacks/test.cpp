/**
 * @file test.cpp
 * @brief LeetCode 232. 用栈实现队列 - 测试
 */

#include "leetcode/0232_implement_queue_using_stacks/solution.h"
#include <cstddef>
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <stdexcept>

// 测试用例结构
struct TestCase {
    std::vector<std::string> operations;  // 操作序列
    std::vector<std::vector<int>> args;   // 参数
    std::vector<std::string> expected;    // 期望结果
    std::string description;               // 测试描述
};

bool runTests() {
    std::cout << "=== LeetCode 232. 用栈实现队列 测试 ===\n\n";
    
    // 测试用例数组
    std::vector<TestCase> testCases = {
        {
            {"MyQueue", "push", "push", "peek", "pop", "empty"},
            {{}, {1}, {2}, {}, {}, {}},
            {"null", "null", "null", "1", "1", "false"},
            "基本操作测试"
        },
        {
            {"MyQueue", "push", "push", "push", "pop", "pop", "peek", "pop", "empty"},
            {{}, {1}, {2}, {3}, {}, {}, {}, {}, {}},
            {"null", "null", "null", "null", "1", "2", "3", "3", "true"},
            "多元素操作测试"
        },
        {
            {"MyQueue", "push", "peek", "push", "peek", "pop", "peek"},
            {{}, {5}, {}, {10}, {}, {}, {}},
            {"null", "null", "5", "null", "5", "5", "10"},
            "交替操作测试"
        },
        {
            {"MyQueue", "empty"},
            {{}, {}},
            {"null", "true"},
            "空队列测试"
        }
    };
    
    std::size_t passed = 0;
    const std::size_t total = testCases.size();
    
    for (std::size_t i = 0; i < total; ++i) {
        const auto& tc = testCases[i];
        std::cout << "测试 " << i + 1 << ": " << tc.description << "\n";
        
        std::unique_ptr<MyQueue> queue;
        bool testPassed = true;
        
        for (std::size_t j = 0; j < tc.operations.size(); ++j) {
            const std::string& op = tc.operations[j];
            
            if (op == "MyQueue") {
                queue = std::make_unique<MyQueue>();
                std::cout << "  创建队列\n";
            } else if (op == "push") {
                queue->push(tc.args[j][0]);
                std::cout << "  push(" << tc.args[j][0] << ") -> null\n";
            } else if (op == "pop") {
                int result = queue->pop();
                std::string expected = tc.expected[j];
                std::cout << "  pop() -> " << result << " (期望: " << expected << ")\n";
                if (std::to_string(result) != expected) {
                    testPassed = false;
                }
            } else if (op == "peek") {
                int result = queue->peek();
                std::string expected = tc.expected[j];
                std::cout << "  peek() -> " << result << " (期望: " << expected << ")\n";
                if (std::to_string(result) != expected) {
                    testPassed = false;
                }
            } else if (op == "empty") {
                bool result = queue->empty();
                std::string expected = tc.expected[j];
                std::cout << "  empty() -> " << (result ? "true" : "false") 
                          << " (期望: " << expected << ")\n";
                if ((result ? "true" : "false") != expected) {
                    testPassed = false;
                }
            }
        }
        
        if (testPassed) {
            std::cout << "  结果: ✅ 通过\n";
            passed++;
        } else {
            std::cout << "  结果: ❌ 失败\n";
        }
        std::cout << "\n";
        
    }
    
    std::cout << "=== 测试总结 ===\n";
    std::cout << "通过: " << passed << "/" << total << "\n";
    
    if (passed == total) {
        std::cout << "🎉 所有测试用例通过！\n";
    } else {
        std::cout << "⚠️ 有 " << (total - passed) << " 个测试用例失败\n";
    }

    MyQueue emptyQueue;
    bool popThrew = false;
    bool peekThrew = false;
    try {
        (void)emptyQueue.pop();
    } catch (const std::underflow_error&) {
        popThrew = true;
    }
    try {
        (void)emptyQueue.peek();
    } catch (const std::underflow_error&) {
        peekThrew = true;
    }
    std::cout << "空队列 pop 契约: " << (popThrew ? "✅ 通过" : "❌ 失败") << "\n";
    std::cout << "空队列 peek 契约: " << (peekThrew ? "✅ 通过" : "❌ 失败") << "\n";

    return passed == total && popThrew && peekThrew;
}

// 详细演示
void detailedDemo() {
    std::cout << "\n=== 详细演示：双栈实现队列原理 ===\n\n";
    
    MyQueue q;
    
    std::cout << "【步骤演示】\n\n";
    
    std::cout << "1. 入队 1, 2, 3:\n";
    std::cout << "   入队栈: [1, 2, 3] (栈顶为3)\n";
    std::cout << "   出队栈: []\n";
    q.push(1);
    q.push(2);
    q.push(3);
    
    std::cout << "\n2. peek() 查看队头:\n";
    std::cout << "   需要从入队栈转移到出队栈\n";
    std::cout << "   入队栈: []\n";
    std::cout << "   出队栈: [3, 2, 1] (栈顶为1，即队头!)\n";
    std::cout << "   peek() = " << q.peek() << "\n";
    
    std::cout << "\n3. pop() 出队:\n";
    std::cout << "   pop() = " << q.pop() << "\n";
    std::cout << "   出队栈: [3, 2] (栈顶为2)\n";
    
    std::cout << "\n4. 继续入队 4, 5:\n";
    std::cout << "   入队栈: [4, 5]\n";
    std::cout << "   出队栈: [3, 2] (不变)\n";
    q.push(4);
    q.push(5);
    
    std::cout << "\n5. 继续pop()直到为空:\n";
    while (!q.empty()) {
        std::cout << "   pop() = " << q.pop() << "\n";
    }
    
    std::cout << "\n【关键理解】\n";
    std::cout << "• 栈A只负责接收新元素（入队）\n";
    std::cout << "• 栈B只负责弹出元素（出队）\n";
    std::cout << "• 栈B为空时，从栈A转移所有元素\n";
    std::cout << "• 单次转移可能是 O(n)，所以 pop/peek 的单次最坏复杂度是 O(n)\n";
    std::cout << "• 每个元素只会进入栈A一次、转移到栈B一次、从栈B弹出一次\n";
    std::cout << "• 一串 n 次入队和出队的总栈操作数是 O(n)，因此均摊 O(1)\n";
}

// 性能测试
void performanceTest() {
    std::cout << "\n=== 性能测试 ===\n";
    
    MyQueue q;
    const int N = 100000;
    
    std::cout << "执行 " << N << " 次操作...\n";
    
    // 大量入队
    for (int i = 0; i < N; ++i) {
        q.push(i);
    }
    
    // 交替操作
    for (int i = 0; i < N; ++i) {
        q.push(i);
        q.pop();
    }
    
    // 清空
    while (!q.empty()) {
        q.pop();
    }
    
    std::cout << "性能测试完成 ✓\n";
}

int main() {
    const bool passed = runTests();
    detailedDemo();
    performanceTest();
    return passed ? 0 : 1;
}
