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
    TreeNode* root = new TreeNode(3);
    root->left = new TreeNode(9);
    root->right = new TreeNode(20);
    root->right->left = new TreeNode(15);
    root->right->right = new TreeNode(7);

    const Solution solution;
    const std::vector<std::vector<int>> expected{{3}, {9, 20}, {15, 7}};
    const bool passed = solution.levelOrder(root) == expected &&
                        solution.levelOrder(nullptr).empty();
    deleteTree(root);

    std::cout << (passed ? "LC 102 checks passed\n" : "LC 102 checks failed\n");
    return passed ? 0 : 1;
}
