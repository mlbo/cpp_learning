/**
 * LeetCode 107. 二叉树的层序遍历 II
 * 
 * 给定二叉树的根节点 root，返回其节点值自底向上的层序遍历。
 */

#include "solution.h"
#include <iostream>
#include <queue>
#include <algorithm>

std::vector<std::vector<int>> Solution::levelOrderBottom(TreeNode* root) {
    std::vector<std::vector<int>> result;
    if (!root) return result;
    
    std::queue<TreeNode*> q;
    q.push(root);
    
    while (!q.empty()) {
        int levelSize = q.size();
        std::vector<int> level;
        
        for (int i = 0; i < levelSize; ++i) {
            TreeNode* node = q.front();
            q.pop();
            
            level.push_back(node->val);
            
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        
        result.push_back(level);
    }
    
    // 反转结果，自底向上
    std::reverse(result.begin(), result.end());
    
    return result;
}

void testLevelOrderBottom() {
    Solution sol;
    
    std::cout << "LeetCode 107. 自底向上层序遍历 测试结果：" << std::endl;
    
    // 构建测试树（同102题）
    TreeNode* root = new TreeNode(3);
    root->left = new TreeNode(9);
    root->right = new TreeNode(20);
    root->right->left = new TreeNode(15);
    root->right->right = new TreeNode(7);
    
    std::cout << "  树结构：" << std::endl;
    std::cout << "        3" << std::endl;
    std::cout << "       / \\" << std::endl;
    std::cout << "      9  20" << std::endl;
    std::cout << "        /  \\" << std::endl;
    std::cout << "       15   7" << std::endl;
    
    auto result = sol.levelOrderBottom(root);
    
    std::cout << "\n  自底向上层序遍历结果：" << std::endl;
    std::cout << "    [";
    for (int i = 0; i < result.size(); ++i) {
        std::cout << "[";
        for (int j = 0; j < result[i].size(); ++j) {
            std::cout << result[i][j];
            if (j < result[i].size() - 1) std::cout << ", ";
        }
        std::cout << "]";
        if (i < result.size() - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
    std::cout << "  期望: [[15, 7], [9, 20], [3]]" << std::endl;
    
    // 清理内存
    delete root->right->right;
    delete root->right->left;
    delete root->right;
    delete root->left;
    delete root;
    
    std::cout << "\n  解题要点：" << std::endl;
    std::cout << "    1. 与102题相同的BFS层序遍历" << std::endl;
    std::cout << "    2. 最后反转结果数组" << std::endl;
    std::cout << "    3. 或者使用 insert 在头部插入" << std::endl;
}
