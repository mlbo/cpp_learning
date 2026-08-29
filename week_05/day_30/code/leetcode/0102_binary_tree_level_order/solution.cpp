#include "solution.h"

#include <cstddef>
#include <queue>
#include <utility>

std::vector<std::vector<int>> Solution::levelOrder(TreeNode* root) const {
    std::vector<std::vector<int>> result;
    if (root == nullptr) {
        return result;
    }

    std::queue<TreeNode*> pending;
    pending.push(root);
    while (!pending.empty()) {
        const std::size_t levelSize = pending.size();
        std::vector<int> level;
        level.reserve(levelSize);
        for (std::size_t i = 0; i < levelSize; ++i) {
            TreeNode* node = pending.front();
            pending.pop();
            level.push_back(node->val);
            if (node->left != nullptr) {
                pending.push(node->left);
            }
            if (node->right != nullptr) {
                pending.push(node->right);
            }
        }
        result.push_back(std::move(level));
    }
    return result;
}
