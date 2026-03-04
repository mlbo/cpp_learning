#ifndef SOLUTION_107_H
#define SOLUTION_107_H

#include <vector>

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

class Solution {
public:
    std::vector<std::vector<int>> levelOrderBottom(TreeNode* root);
};

void testLevelOrderBottom();

#endif // SOLUTION_107_H
