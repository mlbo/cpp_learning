#include "solution.h"

#include <iostream>
#include <string>
#include <vector>

namespace {
void deleteTree(TreeNode* root) {
    if (root == nullptr) {
        return;
    }
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

bool expectEqual(const std::string& name,
                 const std::vector<int>& actual,
                 const std::vector<int>& expected) {
    if (actual == expected) {
        return true;
    }
    std::cerr << name << " failed\n";
    return false;
}
}  // namespace

int main() {
    TreeNode* root = new TreeNode(1);
    root->right = new TreeNode(2);
    root->right->left = new TreeNode(3);

    Solution solution;
    bool passed = true;
    const std::vector<int> expected{1, 2, 3};
    passed = expectEqual("recursive", solution.preorderTraversal(root), expected) && passed;
    passed = expectEqual("stack", solution.preorderIterative(root), expected) && passed;
    passed = expectEqual("cursor stack", preorderTraversalIterative(root), expected) && passed;
    passed = expectEqual("empty", solution.preorderTraversal(nullptr), {}) && passed;

    deleteTree(root);
    std::cout << (passed ? "LC 144 checks passed\n" : "LC 144 checks failed\n");
    return passed ? 0 : 1;
}
