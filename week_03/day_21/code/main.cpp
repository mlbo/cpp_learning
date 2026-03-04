/**
 * Day 21: 第三周复习
 * 主程序入口
 */

#include <iostream>
#include "review/stack_queue_summary.h"
#include "review/lambda_summary.h"
#include "review/emcpp_review.h"
#include "leetcode/0155_min_stack/solution.h"
#include "leetcode/0150_eval_rpn/solution.h"

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  Day 21: 第三周复习" << std::endl;
    std::cout << "========================================" << std::endl;

    // 1. 栈队列复习
    std::cout << "\n【1. 栈和队列复习】" << std::endl;
    stackQueueSummary();

    // 2. Lambda复习
    std::cout << "\n【2. Lambda表达式复习】" << std::endl;
    lambdaSummary();

    // 3. EMC++复习
    std::cout << "\n【3. EMC++条款复习】" << std::endl;
    emcppReview();

    // 4. LeetCode 155 测试
    std::cout << "\n【4. LeetCode 155: 最小栈】" << std::endl;
    testMinStack();

    // 5. LeetCode 150 测试
    std::cout << "\n【5. LeetCode 150: 逆波兰表达式求值】" << std::endl;
    testEvalRPN();

    std::cout << "\n========================================" << std::endl;
    std::cout << "  第三周学习完成！" << std::endl;
    std::cout << "  下周预告：哈希表、移动语义、完美转发" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}
