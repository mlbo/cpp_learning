#include "leetcode/0102_level_order/solution.h"

#include <iostream>
#include <string>
#include <vector>

namespace {

void deleteTree(TreeNode* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

bool check(TreeNode* root, const std::vector<std::vector<int>>& expected,
           const std::string& name) {
    leetcode_0102::Solution solution;
    const auto actual = solution.levelOrder(root);
    const bool passed = actual == expected;
    std::cout << name << ": " << (passed ? "通过" : "失败") << '\n';
    deleteTree(root);
    return passed;
}

} // namespace

int main() {
    int failures = 0;

    if (!check(nullptr, {}, "空树")) ++failures;
    if (!check(new TreeNode(1), {{1}}, "单节点")) ++failures;

    auto* standard = new TreeNode(3);
    standard->left = new TreeNode(9);
    standard->right = new TreeNode(20);
    standard->right->left = new TreeNode(15);
    standard->right->right = new TreeNode(7);
    if (!check(standard, {{3}, {9, 20}, {15, 7}}, "标准三层树")) ++failures;

    auto* skewed = new TreeNode(1);
    skewed->left = new TreeNode(2);
    skewed->left->left = new TreeNode(3);
    skewed->left->left->left = new TreeNode(4);
    if (!check(skewed, {{1}, {2}, {3}, {4}}, "左斜树")) ++failures;

    auto* unbalanced = new TreeNode(1);
    unbalanced->left = new TreeNode(2);
    unbalanced->right = new TreeNode(3);
    unbalanced->left->left = new TreeNode(4);
    unbalanced->right->right = new TreeNode(5);
    if (!check(unbalanced, {{1}, {2, 3}, {4, 5}}, "不平衡树")) ++failures;

    return failures == 0 ? 0 : 1;
}
