/**
 * LeetCode 236: 最近公共祖先
 */

#include <iostream>

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        if (!root || root == p || root == q) return root;
        
        TreeNode* left = lowestCommonAncestor(root->left, p, q);
        TreeNode* right = lowestCommonAncestor(root->right, p, q);
        
        if (left && right) return root;
        return left ? left : right;
    }
};

int main() {
    std::cout << "=== LeetCode 236: 最近公共祖先 ===" << std::endl;
    
    TreeNode* root = new TreeNode(3);
    root->left = new TreeNode(5);
    root->right = new TreeNode(1);
    root->left->left = new TreeNode(6);
    root->left->right = new TreeNode(2);
    root->right->left = new TreeNode(0);
    root->right->right = new TreeNode(8);
    
    Solution sol;
    TreeNode* lca = sol.lowestCommonAncestor(root, root->left, root->right);
    std::cout << "LCA(5, 1) = " << lca->val << std::endl;
    
    return 0;
}
