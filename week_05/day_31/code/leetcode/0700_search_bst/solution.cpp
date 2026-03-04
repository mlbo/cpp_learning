/**
 * LeetCode 700: 二叉搜索树中的搜索
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
    TreeNode* searchBST(TreeNode* root, int val) {
        if (root == nullptr) return nullptr;
        
        if (val == root->val) {
            return root;
        } else if (val < root->val) {
            return searchBST(root->left, val);
        } else {
            return searchBST(root->right, val);
        }
    }
    
    // 迭代版本
    TreeNode* searchBSTIterative(TreeNode* root, int val) {
        while (root != nullptr && root->val != val) {
            if (val < root->val) {
                root = root->left;
            } else {
                root = root->right;
            }
        }
        return root;
    }
};

int main() {
    std::cout << "=== LeetCode 700: BST搜索 ===" << std::endl;
    
    TreeNode* root = new TreeNode(4);
    root->left = new TreeNode(2);
    root->right = new TreeNode(7);
    root->left->left = new TreeNode(1);
    root->left->right = new TreeNode(3);
    
    Solution sol;
    TreeNode* result = sol.searchBST(root, 2);
    
    if (result) {
        std::cout << "找到节点，值为: " << result->val << std::endl;
    } else {
        std::cout << "未找到节点" << std::endl;
    }
    
    return 0;
}
