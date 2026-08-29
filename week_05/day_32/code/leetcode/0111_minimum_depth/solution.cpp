#include "solution.h"

#include <iostream>

int main() {
    auto root = std::make_unique<lc0111::TreeNode>(3);
    root->left = std::make_unique<lc0111::TreeNode>(9);
    root->right = std::make_unique<lc0111::TreeNode>(20);
    root->right->left = std::make_unique<lc0111::TreeNode>(15);
    root->right->right = std::make_unique<lc0111::TreeNode>(7);

    const lc0111::Solution solution;
    const int depth = solution.min_depth(root.get());
    std::cout << "LC 111 sample minimum depth: " << depth << '\n';
    return depth == 2 ? 0 : 1;
}
