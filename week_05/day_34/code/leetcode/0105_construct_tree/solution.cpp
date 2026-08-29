#include <iostream>
#include <vector>

#include "solution.h"

int main() {
    using day34::construct_tree::TreeBuilder;
    using day34::construct_tree::collect_inorder;
    using day34::construct_tree::collect_preorder;

    const std::vector<int> preorder{3, 9, 20, 15, 7};
    const std::vector<int> inorder{9, 3, 15, 20, 7};
    TreeBuilder builder;
    const auto root = builder.build(preorder, inorder);
    std::vector<int> rebuilt_preorder;
    std::vector<int> rebuilt_inorder;
    collect_preorder(root.get(), rebuilt_preorder);
    collect_inorder(root.get(), rebuilt_inorder);

    const bool passed = rebuilt_preorder == preorder && rebuilt_inorder == inorder;
    std::cout << "构造完成，根节点: " << (root == nullptr ? -1 : root->value) << '\n';
    return passed ? 0 : 1;
}
