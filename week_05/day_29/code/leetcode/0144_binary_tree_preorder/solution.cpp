/**
 * LeetCode 144: 二叉树的前序遍历 - 实现
 */

#include "solution.h"

// 额外的迭代实现（统一风格）
std::vector<int> preorderTraversalIterative(TreeNode* root) {
    std::vector<int> result;
    std::stack<TreeNode*> stk;
    TreeNode* curr = root;
    
    while (curr != nullptr || !stk.empty()) {
        // 访问当前节点并一路向左
        while (curr != nullptr) {
            result.push_back(curr->val);  // 前序：先访问
            stk.push(curr);
            curr = curr->left;
        }
        
        // 回溯到父节点，转向右子树
        curr = stk.top();
        stk.pop();
        curr = curr->right;
    }
    
    return result;
}
