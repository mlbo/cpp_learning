#ifndef SOLUTION_102_H
#define SOLUTION_102_H

#include <vector>

// 二叉树节点定义
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

class Solution {
public:
    std::vector<std::vector<int>> levelOrder(TreeNode* root);
};

void testLevelOrder();

#endif // SOLUTION_102_H
