#pragma once

#include <memory>

namespace day34::lca {

struct TreeNode {
    explicit TreeNode(int node_value) : value{node_value} {}

    TreeNode* add_left(int node_value) {
        left = std::make_unique<TreeNode>(node_value);
        return left.get();
    }

    TreeNode* add_right(int node_value) {
        right = std::make_unique<TreeNode>(node_value);
        return right.get();
    }

    int value;
    std::unique_ptr<TreeNode> left;
    std::unique_ptr<TreeNode> right;
};

inline bool contains(const TreeNode* root, const TreeNode* target) {
    if (root == nullptr || target == nullptr) {
        return false;
    }
    return root == target || contains(root->left.get(), target) ||
           contains(root->right.get(), target);
}

inline const TreeNode* lowest_common_ancestor_unchecked(
    const TreeNode* root,
    const TreeNode* first,
    const TreeNode* second) {
    if (root == nullptr || root == first || root == second) {
        return root;
    }

    const TreeNode* left =
        lowest_common_ancestor_unchecked(root->left.get(), first, second);
    const TreeNode* right =
        lowest_common_ancestor_unchecked(root->right.get(), first, second);
    if (left != nullptr && right != nullptr) {
        return root;
    }
    return left != nullptr ? left : right;
}

// Contract: root owns the tree; first and second are non-owning observers into
// that same tree. Return nullptr when either observer is null or outside root.
inline const TreeNode* lowest_common_ancestor(
    const TreeNode* root,
    const TreeNode* first,
    const TreeNode* second) {
    if (!contains(root, first) || !contains(root, second)) {
        return nullptr;
    }
    return lowest_common_ancestor_unchecked(root, first, second);
}

}  // namespace day34::lca
