#include "solution.h"

#include <iostream>

int main() {
    auto root = std::make_unique<lc0104::TreeNode>(3);
    root->left = std::make_unique<lc0104::TreeNode>(9);
    root->right = std::make_unique<lc0104::TreeNode>(20);
    root->right->left = std::make_unique<lc0104::TreeNode>(15);
    root->right->right = std::make_unique<lc0104::TreeNode>(7);

    const lc0104::Solution solution;
    const int depth = solution.max_depth(root.get());
    std::cout << "LC 104 sample maximum depth: " << depth << '\n';
    return depth == 3 ? 0 : 1;
}
