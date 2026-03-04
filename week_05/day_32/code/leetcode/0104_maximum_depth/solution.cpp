/**
 * LeetCode 104: 二叉树的最大深度
 */

#include <iostream>
#include <algorithm>

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

class Solution {
public:
    // 递归DFS
    int maxDepth(TreeNode* root) {
        if (root == nullptr) return 0;
        return 1 + std::max(maxDepth(root->left), maxDepth(root->right));
    }
};

int main() {
    std::cout << "=== LeetCode 104: 二叉树最大深度 ===" << std::endl;
    
    TreeNode* root = new TreeNode(3);
    root->left = new TreeNode(9);
    root->right = new TreeNode(20);
    root->right->left = new TreeNode(15);
    root->right->right = new TreeNode(7);
    
    Solution sol;
    std::cout << "最大深度: " << sol.maxDepth(root) << std::endl;
    
    return 0;
}
