#ifndef WEEK05_DAY32_LC0111_SOLUTION_H
#define WEEK05_DAY32_LC0111_SOLUTION_H

#include <algorithm>
#include <memory>

namespace lc0111 {

struct TreeNode {
    explicit TreeNode(int value_in) : value(value_in) {}

    int value;
    std::unique_ptr<TreeNode> left;
    std::unique_ptr<TreeNode> right;
};

class Solution {
public:
    [[nodiscard]] int min_depth(const TreeNode* root) const {
        if (root == nullptr) {
            return 0;
        }
        if (root->left == nullptr) {
            return 1 + min_depth(root->right.get());
        }
        if (root->right == nullptr) {
            return 1 + min_depth(root->left.get());
        }
        return 1 + std::min(min_depth(root->left.get()), min_depth(root->right.get()));
    }
};

}  // namespace lc0111

#endif
