/**
 * LeetCode 145: 二叉树的后序遍历 - 实现
 */

#include "solution.h"

// 方法3：迭代实现（经典方法，使用prev指针）
std::vector<int> postorderIterativeClassic(TreeNode* root) {
    std::vector<int> result;
    if (root == nullptr) return result;
    
    std::stack<TreeNode*> stk;
    TreeNode* prev = nullptr;
    
    while (root != nullptr || !stk.empty()) {
        // 一路向左，入栈
        while (root != nullptr) {
            stk.push(root);
            root = root->left;
        }
        
        root = stk.top();
        
        // 如果右子树为空或已访问，则访问当前节点
        if (root->right == nullptr || root->right == prev) {
            result.push_back(root->val);
            stk.pop();
            prev = root;
            root = nullptr;  // 重要：避免重复访问左子树
        } else {
            // 否则转向右子树
            root = root->right;
        }
    }
    
    return result;
}
