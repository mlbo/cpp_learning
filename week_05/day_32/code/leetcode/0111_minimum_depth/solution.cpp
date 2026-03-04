/**
 * LeetCode 111: 二叉树的最小深度
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
    int minDepth(TreeNode* root) {
        if (root == nullptr) return 0;
        if (root->left == nullptr) return minDepth(root->right) + 1;
        if (root->right == nullptr) return minDepth(root->left) + 1;
        return 1 + std::min(minDepth(root->left), minDepth(root->right));
    }
};

int main() {
    std::cout << "=== LeetCode 111: 二叉树最小深度 ===" << std::endl;
    
    TreeNode* root = new TreeNode(3);
    root->left = new TreeNode(9);
    root->right = new TreeNode(20);
    root->right->left = new TreeNode(15);
    root->right->right = new TreeNode(7);
    
    Solution sol;
    std::cout << "最小深度: " << sol.minDepth(root) << std::endl;
    
    return 0;
}
