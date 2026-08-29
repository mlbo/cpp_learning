#include "solution.h"

#include <stack>

std::vector<int> Solution::inorderTraversal(TreeNode* root) const {
    std::vector<int> result;
    inorder(root, result);
    return result;
}

std::vector<int> Solution::inorderIterative(TreeNode* root) const {
    std::vector<int> result;
    std::stack<TreeNode*> ancestors;
    TreeNode* current = root;
    while (current != nullptr || !ancestors.empty()) {
        while (current != nullptr) {
            ancestors.push(current);
            current = current->left;
        }
        current = ancestors.top();
        ancestors.pop();
        result.push_back(current->val);
        current = current->right;
    }
    return result;
}

void Solution::inorder(TreeNode* root, std::vector<int>& result) {
    if (root == nullptr) {
        return;
    }
    inorder(root->left, result);
    result.push_back(root->val);
    inorder(root->right, result);
}
