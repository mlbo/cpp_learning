/**
 * Day 16: 队列入门与Lambda进阶
 * 主程序入口
 */

#include <iostream>
#include "data_structure/queue_demo.h"
#include "cpp11_features/lambda_generic_demo.h"
#include "cpp11_features/lambda_init_capture_demo.h"
#include "emcpp/item32_item33.h"

void testMyQueue();
void testMyStack();

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  Day 16: 队列入门与Lambda进阶" << std::endl;
    std::cout << "========================================" << std::endl;

    // 1. 队列演示
    std::cout << "\n【1. 队列数据结构演示】" << std::endl;
    queueDemo();

    // 2. 泛型Lambda
    std::cout << "\n【2. 泛型Lambda】" << std::endl;
    lambdaGenericDemo();

    // 3. 初始化捕获
    std::cout << "\n【3. Lambda初始化捕获】" << std::endl;
    lambdaInitCaptureDemo();

    // 4. EMC++ Item 32-33
    std::cout << "\n【4. EMC++ Item 32-33】" << std::endl;
    item32Item33Demo();

    // 5. LeetCode 232 测试
    std::cout << "\n【5. LeetCode 232: 用栈实现队列】" << std::endl;
    testMyQueue();

    // 6. LeetCode 225 测试
    std::cout << "\n【6. LeetCode 225: 用队列实现栈】" << std::endl;
    testMyStack();

    std::cout << "\n========================================" << std::endl;
    std::cout << "  Day 16 学习完成！" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}
