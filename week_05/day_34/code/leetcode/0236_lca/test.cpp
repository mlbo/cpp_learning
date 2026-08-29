#include <iostream>
#include <memory>

#include "solution.h"

int main() {
    using day34::lca::TreeNode;
    using day34::lca::lowest_common_ancestor;

    auto root = std::make_unique<TreeNode>(10);
    TreeNode* left = root->add_left(5);
    TreeNode* right = root->add_right(15);
    TreeNode outside{99};

    const bool same_node = lowest_common_ancestor(root.get(), left, left) == left;
    const bool missing_node = lowest_common_ancestor(root.get(), left, &outside) == nullptr;
    const bool null_node = lowest_common_ancestor(root.get(), nullptr, right) == nullptr;
    if (!same_node || !missing_node || !null_node) {
        std::cerr << "LCA interface contract test failed\n";
        return 1;
    }
    return 0;
}
