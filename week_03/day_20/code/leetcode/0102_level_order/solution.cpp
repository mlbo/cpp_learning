/**
 * LeetCode 102. 二叉树的层序遍历
 * 
 * 给你二叉树的根节点 root，返回其节点值的层序遍历。
 */

#include "solution.h"
#include <iostream>
#include <queue>

std::vector<std::vector<int>> Solution::levelOrder(TreeNode* root) {
    std::vector<std::vector<int>> result;
    if (!root) return result;
    
    std::queue<TreeNode*> q;
    q.push(root);
    
    while (!q.empty()) {
        int levelSize = q.size();  // 当前层的节点数
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
    
    return result;
}

void testLevelOrder() {
    Solution sol;
    
    std::cout << "LeetCode 102. 二叉树层序遍历 测试结果：" << std::endl;
    
    // 构建测试树
    //        3
    //       / \
    //      9  20
    //        /  \
    //       15   7
    
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
    
    auto result = sol.levelOrder(root);
    
    std::cout << "\n  层序遍历结果：" << std::endl;
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
    std::cout << "  期望: [[3], [9, 20], [15, 7]]" << std::endl;
    
    // 清理内存
    delete root->right->right;
    delete root->right->left;
    delete root->right;
    delete root->left;
    delete root;
    
    std::cout << "\n  解题要点：" << std::endl;
    std::cout << "    1. 使用队列实现BFS" << std::endl;
    std::cout << "    2. 每轮记录当前层的大小" << std::endl;
    std::cout << "    3. 处理该数量的节点，收集结果" << std::endl;
}
