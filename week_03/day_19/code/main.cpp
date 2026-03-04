/**
 * Day 19: 堆与优先队列
 * 主程序入口
 */

#include <iostream>
#include "data_structure/heap_demo.h"
#include "data_structure/priority_queue_demo.h"
#include "leetcode/0215_kth_largest/solution.h"
#include "leetcode/0347_top_k_frequent/solution.h"

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  Day 19: 堆与优先队列" << std::endl;
    std::cout << "========================================" << std::endl;

    // 1. 堆演示
    std::cout << "\n【1. 堆数据结构演示】" << std::endl;
    heapDemo();

    // 2. 优先队列演示
    std::cout << "\n【2. 优先队列演示】" << std::endl;
    priorityQueueDemo();

    // 3. LeetCode 215 测试
    std::cout << "\n【3. LeetCode 215: 第K个最大元素】" << std::endl;
    testFindKthLargest();

    // 4. LeetCode 347 测试
    std::cout << "\n【4. LeetCode 347: 前K个高频元素】" << std::endl;
    testTopKFrequent();

    std::cout << "\n========================================" << std::endl;
    std::cout << "  Day 19 学习完成！" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}
