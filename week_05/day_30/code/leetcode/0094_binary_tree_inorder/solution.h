#ifndef DAY30_LC0094_SOLUTION_H
#define DAY30_LC0094_SOLUTION_H

#include <vector>

struct TreeNode {
    explicit TreeNode(int value) : val(value) {}

    int val;
    TreeNode* left{nullptr};
    TreeNode* right{nullptr};
};

class Solution {
public:
    std::vector<int> inorderTraversal(TreeNode* root) const;
    std::vector<int> inorderIterative(TreeNode* root) const;

private:
    static void inorder(TreeNode* root, std::vector<int>& result);
};

#endif  // DAY30_LC0094_SOLUTION_H
