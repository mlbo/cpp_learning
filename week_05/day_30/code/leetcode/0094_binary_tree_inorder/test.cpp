#include "solution.h"

#include <iostream>
#include <vector>

namespace {
void deleteTree(TreeNode* root) {
    if (root == nullptr) {
        return;
    }
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}
}  // namespace

int main() {
    TreeNode* root = new TreeNode(1);
    root->right = new TreeNode(2);
    root->right->left = new TreeNode(3);

    const Solution solution;
    const std::vector<int> expected{1, 3, 2};
    const bool passed = solution.inorderTraversal(root) == expected &&
                        solution.inorderIterative(root) == expected &&
                        solution.inorderTraversal(nullptr).empty();
    deleteTree(root);

    std::cout << (passed ? "LC 94 checks passed\n" : "LC 94 checks failed\n");
    return passed ? 0 : 1;
}
