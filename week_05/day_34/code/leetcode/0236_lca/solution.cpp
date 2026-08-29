#include <iostream>

#include "solution.h"

int main() {
    using day34::lca::TreeNode;
    using day34::lca::lowest_common_ancestor;

    auto root = std::make_unique<TreeNode>(3);
    TreeNode* five = root->add_left(5);
    TreeNode* one = root->add_right(1);
    five->add_left(6);
    TreeNode* two = five->add_right(2);
    two->add_left(7);
    TreeNode* four = two->add_right(4);
    one->add_left(0);
    one->add_right(8);

    const TreeNode* first = lowest_common_ancestor(root.get(), five, one);
    const TreeNode* second = lowest_common_ancestor(root.get(), five, four);
    const bool passed = first == root.get() && second == five;
    std::cout << "LCA(5, 1) = " << (first == nullptr ? -1 : first->value) << '\n';
    std::cout << "LCA(5, 4) = " << (second == nullptr ? -1 : second->value) << '\n';
    return passed ? 0 : 1;
}
