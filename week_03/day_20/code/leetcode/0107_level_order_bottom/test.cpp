/**
 * @file test.cpp
 * @brief LeetCode 107: 二叉树的层序遍历 II - 测试文件
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
    
    // 创建一个完全二叉树
    std::vector<int> values;
    for (int i = 1; i <= 32767; ++i) {
        values.push_back(i);
    }
    
    auto root = tree_ops::createTreeSimple(values, INT_MIN);
    
    // 测试方法一：BFS + 反转
    auto start = std::chrono::high_resolution_clock::now();
    auto result1 = leetcode::levelOrderBottom(root);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // 测试方法二：deque
    start = std::chrono::high_resolution_clock::now();
    auto result2 = leetcode::levelOrderBottomDeque(root);
    end = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // 测试方法三：DFS
    start = std::chrono::high_resolution_clock::now();
    auto result3 = leetcode::levelOrderBottomDFS(root);
    end = std::chrono::high_resolution_clock::now();
    auto duration3 = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "节点数: " << values.size() << "\n";
    std::cout << "层数: " << result1.size() << "\n\n";
    
    std::cout << "方法一 (BFS + 反转): " << duration1.count() << " 微秒\n";
    std::cout << "方法二 (deque):       " << duration2.count() << " 微秒\n";
    std::cout << "方法三 (DFS):         " << duration3.count() << " 微秒\n";
    
    // 验证所有方法结果一致
    assert(result1 == result2);
    assert(result2 == result3);
    std::cout << "\n验证: ✓ 所有方法结果一致\n";
    
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
    
    auto result = leetcode::levelOrderBottom(leftSkew);
    std::cout << "左斜树结果: " << tree_ops::levelOrderToString(result) << "\n";
    std::cout << "预期: [[4],[3],[2],[1]]\n";
    
    tree_ops::deleteTree(leftSkew);
    
    // 右斜树
    std::cout << "\n【右斜树】\n";
    auto rightSkew = new TreeNode(1);
    rightSkew->right = new TreeNode(2);
    rightSkew->right->right = new TreeNode(3);
    rightSkew->right->right->right = new TreeNode(4);
    
    result = leetcode::levelOrderBottom(rightSkew);
    std::cout << "右斜树结果: " << tree_ops::levelOrderToString(result) << "\n";
    std::cout << "预期: [[4],[3],[2],[1]]\n";
    
    tree_ops::deleteTree(rightSkew);
    
    // 不平衡树
    std::cout << "\n【不平衡树】\n";
    auto unbalanced = new TreeNode(1);
    unbalanced->left = new TreeNode(2);
    unbalanced->left->left = new TreeNode(3);
    unbalanced->left->left->left = new TreeNode(4);
    unbalanced->right = new TreeNode(5);
    
    result = leetcode::levelOrderBottom(unbalanced);
    std::cout << "不平衡树结果: " << tree_ops::levelOrderToString(result) << "\n";
    std::cout << "预期: [[4],[3],[2,5],[1]]\n";
    
    tree_ops::deleteTree(unbalanced);
}

int main() {
    std::cout << "╔═════════════════════════════════════════════════════════════╗\n";
    std::cout << "║      LeetCode 107: 二叉树的层序遍历 II - 完整测试          ║\n";
    std::cout << "╚═════════════════════════════════════════════════════════════╝\n";
    
    edgeCaseTest();
    performanceTest();
    
    std::cout << "\n======== 所有测试通过 ========\n";
    return 0;
}
