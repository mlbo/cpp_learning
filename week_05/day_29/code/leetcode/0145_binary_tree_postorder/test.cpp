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
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);

    Solution solution;
    bool passed = true;
    const std::vector<int> expected{4, 5, 2, 3, 1};
    passed = expectEqual("recursive", solution.postorderTraversal(root), expected) && passed;
    passed = expectEqual("reverse preorder", solution.postorderIterative(root), expected) && passed;
    passed = expectEqual("classic stack", postorderIterativeClassic(root), expected) && passed;
    passed = expectEqual("empty", solution.postorderTraversal(nullptr), {}) && passed;

    deleteTree(root);
    std::cout << (passed ? "LC 145 checks passed\n" : "LC 145 checks failed\n");
    return passed ? 0 : 1;
}
