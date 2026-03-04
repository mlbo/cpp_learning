/**
 * LeetCode 145 测试文件
 */

#include <iostream>
#include <vector>
#include "solution.h"

// 辅助函数
TreeNode* createTree1() {
    //   1
    //    \
    //     2
    //    /
    //   3
    TreeNode* root = new TreeNode(1);
    root->right = new TreeNode(2);
    root->right->left = new TreeNode(3);
    return root;
}

TreeNode* createTree2() {
    //     1
    //    / \
    //   2   3
    //  / \
    // 4   5
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);
    return root;
}

void deleteTree(TreeNode* root) {
    if (root == nullptr) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

void printVector(const std::string& name, const std::vector<int>& vec) {
    std::cout << name << ": [";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i];
        if (i < vec.size() - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
}

int main() {
    std::cout << "=== LeetCode 145: 二叉树后序遍历 ===" << std::endl;
    
    Solution sol;
    
    // 测试用例1
    std::cout << "\n测试用例1:" << std::endl;
    TreeNode* root1 = createTree1();
    auto result1_recursive = sol.postorderTraversal(root1);
    auto result1_iterative = sol.postorderIterative(root1);
    printVector("递归结果", result1_recursive);
    printVector("迭代结果", result1_iterative);
    deleteTree(root1);
    
    // 测试用例2
    std::cout << "\n测试用例2:" << std::endl;
    TreeNode* root2 = createTree2();
    auto result2_recursive = sol.postorderTraversal(root2);
    auto result2_iterative = sol.postorderIterative(root2);
    printVector("递归结果", result2_recursive);
    printVector("迭代结果", result2_iterative);
    deleteTree(root2);
    
    // 测试用例3：空树
    std::cout << "\n测试用例3 (空树):" << std::endl;
    auto result3 = sol.postorderTraversal(nullptr);
    printVector("结果", result3);
    
    std::cout << "\n测试完成!" << std::endl;
    return 0;
}
