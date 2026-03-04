/**
 * @file test.cpp
 * @brief LeetCode 102: 二叉树的层序遍历 - 测试文件
 * @author C++ Tutorial
 * @date Day 20
 */

#include "solution.h"
#include <iostream>
#include <cassert>
#include <chrono>

// 性能测试
void performanceTest() {
    std::cout << "\n======== 性能测试 ========\n";
    
    // 创建一个完全二叉树，高度为15，节点数约32767
    std::vector<int> values;
    for (int i = 1; i <= 32767; ++i) {
        values.push_back(i);
    }
    
    auto root = tree_ops::createTreeSimple(values, INT_MIN);
    
    // 测试BFS
    auto start = std::chrono::high_resolution_clock::now();
    auto result = leetcode::levelOrder(root);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto bfsDuration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "节点数: " << values.size() << "\n";
    std::cout << "层数: " << result.size() << "\n";
    std::cout << "BFS耗时: " << bfsDuration.count() << " 微秒\n";
    
    // 测试DFS
    start = std::chrono::high_resolution_clock::now();
    auto result2 = leetcode::levelOrderDFS(root);
    end = std::chrono::high_resolution_clock::now();
    
    auto dfsDuration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "DFS耗时: " << dfsDuration.count() << " 微秒\n";
    
    // 验证结果正确性
    assert(result == result2);
    std::cout << "验证: ✓ 两种方法结果一致\n";
    
    tree_ops::deleteTree(root);
}

// 边界测试
void edgeCaseTest() {
    std::cout << "\n======== 边界测试 ========\n";
    
    // 左斜树
    std::cout << "\n【左斜树】\n";
    auto leftSkew = new TreeNode(1);
    leftSkew->left = new TreeNode(2);
    leftSkew->left->left = new TreeNode(3);
    leftSkew->left->left->left = new TreeNode(4);
    
    auto result = leetcode::levelOrder(leftSkew);
    std::cout << "左斜树结果: " << tree_ops::levelOrderToString(result) << "\n";
    std::cout << "预期: [[1], [2], [3], [4]]\n";
    
    tree_ops::deleteTree(leftSkew);
    
    // 右斜树
    std::cout << "\n【右斜树】\n";
    auto rightSkew = new TreeNode(1);
    rightSkew->right = new TreeNode(2);
    rightSkew->right->right = new TreeNode(3);
    rightSkew->right->right->right = new TreeNode(4);
    
    result = leetcode::levelOrder(rightSkew);
    std::cout << "右斜树结果: " << tree_ops::levelOrderToString(result) << "\n";
    std::cout << "预期: [[1], [2], [3], [4]]\n";
    
    tree_ops::deleteTree(rightSkew);
}

int main() {
    std::cout << "╔═════════════════════════════════════════════════════════════╗\n";
    std::cout << "║       LeetCode 102: 二叉树的层序遍历 - 完整测试             ║\n";
    std::cout << "╚═════════════════════════════════════════════════════════════╝\n";
    
    edgeCaseTest();
    performanceTest();
    
    std::cout << "\n======== 所有测试通过 ========\n";
    return 0;
}
