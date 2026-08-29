#pragma once

#include <algorithm>
#include <cstdint>
#include <limits>
#include <memory>
#include <stdexcept>

namespace day35::max_path {

struct TreeNode {
    explicit TreeNode(std::int64_t node_value) : value{node_value} {}

    TreeNode* add_left(std::int64_t node_value) {
        left = std::make_unique<TreeNode>(node_value);
        return left.get();
    }

    TreeNode* add_right(std::int64_t node_value) {
        right = std::make_unique<TreeNode>(node_value);
        return right.get();
    }

    std::int64_t value;
    std::unique_ptr<TreeNode> left;
    std::unique_ptr<TreeNode> right;
};

class MaxPathSum {
public:
    std::int64_t operator()(const TreeNode* root) const {
        if (root == nullptr) {
            throw std::invalid_argument("maximum path requires a non-empty tree");
        }
        std::int64_t best = std::numeric_limits<std::int64_t>::lowest();
        static_cast<void>(gain(root, best));
        return best;
    }

private:
    static std::int64_t checked_add(std::int64_t left, std::int64_t right) {
        const std::int64_t maximum = std::numeric_limits<std::int64_t>::max();
        const std::int64_t minimum = std::numeric_limits<std::int64_t>::lowest();
        if ((right > 0 && left > maximum - right) ||
            (right < 0 && left < minimum - right)) {
            throw std::overflow_error("maximum path sum is outside int64_t");
        }
        return left + right;
    }

    static std::int64_t gain(const TreeNode* node, std::int64_t& best) {
        if (node == nullptr) {
            return 0;
        }
        const std::int64_t left_gain = std::max<std::int64_t>(0, gain(node->left.get(), best));
        const std::int64_t right_gain =
            std::max<std::int64_t>(0, gain(node->right.get(), best));
        const std::int64_t through_node =
            checked_add(checked_add(node->value, left_gain), right_gain);
        best = std::max(best, through_node);
        return checked_add(node->value, std::max(left_gain, right_gain));
    }
};

}  // namespace day35::max_path
