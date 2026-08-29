#pragma once

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <vector>

namespace day34::construct_tree {

struct TreeNode {
    explicit TreeNode(int node_value) : value{node_value} {}

    int value;
    std::unique_ptr<TreeNode> left;
    std::unique_ptr<TreeNode> right;
};

class TreeBuilder {
public:
    std::unique_ptr<TreeNode> build(
        const std::vector<int>& preorder,
        const std::vector<int>& inorder) {
        if (preorder.size() != inorder.size()) {
            throw std::invalid_argument("preorder and inorder sizes differ");
        }
        positions_.clear();
        for (std::size_t index = 0; index < inorder.size(); ++index) {
            const auto [iterator, inserted] = positions_.emplace(inorder[index], index);
            static_cast<void>(iterator);
            if (!inserted) {
                throw std::invalid_argument("duplicate values make reconstruction ambiguous");
            }
        }
        return build_range(preorder, 0U, preorder.size(), 0U, inorder.size());
    }

private:
    std::unique_ptr<TreeNode> build_range(
        const std::vector<int>& preorder,
        std::size_t preorder_begin,
        std::size_t preorder_end,
        std::size_t inorder_begin,
        std::size_t inorder_end) const {
        if (preorder_begin == preorder_end) {
            if (inorder_begin != inorder_end) {
                throw std::invalid_argument("traversal ranges are inconsistent");
            }
            return nullptr;
        }
        if (preorder_end - preorder_begin != inorder_end - inorder_begin) {
            throw std::invalid_argument("traversal ranges are inconsistent");
        }

        const int root_value = preorder[preorder_begin];
        const auto position = positions_.find(root_value);
        if (position == positions_.end() || position->second < inorder_begin ||
            position->second >= inorder_end) {
            throw std::invalid_argument("traversals do not describe the same tree");
        }

        const std::size_t left_size = position->second - inorder_begin;
        const std::size_t right_preorder_begin = preorder_begin + 1U + left_size;
        if (right_preorder_begin > preorder_end) {
            throw std::invalid_argument("traversal ranges are inconsistent");
        }

        auto root = std::make_unique<TreeNode>(root_value);
        root->left = build_range(
            preorder,
            preorder_begin + 1U,
            right_preorder_begin,
            inorder_begin,
            position->second);
        root->right = build_range(
            preorder,
            right_preorder_begin,
            preorder_end,
            position->second + 1U,
            inorder_end);
        return root;
    }

    std::unordered_map<int, std::size_t> positions_;
};

inline void collect_preorder(const TreeNode* node, std::vector<int>& output) {
    if (node == nullptr) {
        return;
    }
    output.push_back(node->value);
    collect_preorder(node->left.get(), output);
    collect_preorder(node->right.get(), output);
}

inline void collect_inorder(const TreeNode* node, std::vector<int>& output) {
    if (node == nullptr) {
        return;
    }
    collect_inorder(node->left.get(), output);
    output.push_back(node->value);
    collect_inorder(node->right.get(), output);
}

}  // namespace day34::construct_tree
