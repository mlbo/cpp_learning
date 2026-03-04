/**
 * Day 20: BFS广度优先搜索
 * 主程序入口
 */

#include <iostream>
#include "algorithm/bfs_template.h"

void testLevelOrder();
void testLevelOrderBottom();

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  Day 20: BFS广度优先搜索" << std::endl;
    std::cout << "========================================" << std::endl;

    // 1. BFS模板演示
    std::cout << "\n【1. BFS模板演示】" << std::endl;
    bfsTemplateDemo();

    // 2. LeetCode 102 测试
    std::cout << "\n【2. LeetCode 102: 层序遍历】" << std::endl;
    testLevelOrder();

    // 3. LeetCode 107 测试
    std::cout << "\n【3. LeetCode 107: 自底向上层序遍历】" << std::endl;
    testLevelOrderBottom();

    std::cout << "\n========================================" << std::endl;
    std::cout << "  Day 20 学习完成！" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}
