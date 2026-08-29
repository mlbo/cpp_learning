#ifndef DAY30_LC0102_SOLUTION_H
#define DAY30_LC0102_SOLUTION_H

#include <vector>

struct TreeNode {
    explicit TreeNode(int value) : val(value) {}

    int val;
    TreeNode* left{nullptr};
    TreeNode* right{nullptr};
};

class Solution {
public:
    std::vector<std::vector<int>> levelOrder(TreeNode* root) const;
};

#endif  // DAY30_LC0102_SOLUTION_H
