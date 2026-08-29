#ifndef WEEK05_DAY32_LC0104_SOLUTION_H
#define WEEK05_DAY32_LC0104_SOLUTION_H

#include <algorithm>
#include <memory>

namespace lc0104 {

struct TreeNode {
    explicit TreeNode(int value_in) : value(value_in) {}

    int value;
    std::unique_ptr<TreeNode> left;
    std::unique_ptr<TreeNode> right;
};

class Solution {
public:
    [[nodiscard]] int max_depth(const TreeNode* root) const {
        if (root == nullptr) {
            return 0;
        }
        return 1 + std::max(max_depth(root->left.get()), max_depth(root->right.get()));
    }
};

}  // namespace lc0104

#endif
