/**
 * LeetCode 124: 二叉树最大路径和
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
private:
    int maxSum = -1000000;  // INT_MIN approximation
    
    int maxGain(TreeNode* node) {
        if (!node) return 0;
        
        int leftGain = std::max(maxGain(node->left), 0);
        int rightGain = std::max(maxGain(node->right), 0);
        
        int pathSum = node->val + leftGain + rightGain;
        maxSum = std::max(maxSum, pathSum);
        
        return node->val + std::max(leftGain, rightGain);
    }
    
public:
    int maxPathSum(TreeNode* root) {
        maxGain(root);
        return maxSum;
    }
};

int main() {
    std::cout << "=== LeetCode 124: 最大路径和 ===" << std::endl;
    
    TreeNode* root = new TreeNode(-10);
    root->left = new TreeNode(9);
    root->right = new TreeNode(20);
    root->right->left = new TreeNode(15);
    root->right->right = new TreeNode(7);
    
    Solution sol;
    std::cout << "最大路径和: " << sol.maxPathSum(root) << std::endl;
    
    return 0;
}
