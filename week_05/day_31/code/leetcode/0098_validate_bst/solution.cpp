/**
 * LeetCode 98: 验证二叉搜索树
 */

#include <iostream>
#include <climits>

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

class Solution {
public:
    bool isValidBST(TreeNode* root) {
        return validate(root, LONG_MIN, LONG_MAX);
    }
    
private:
    bool validate(TreeNode* node, long minVal, long maxVal) {
        if (node == nullptr) return true;
        
        if (node->val <= minVal || node->val >= maxVal) {
            return false;
        }
        
        return validate(node->left, minVal, node->val) &&
               validate(node->right, node->val, maxVal);
    }
};

int main() {
    std::cout << "=== LeetCode 98: 验证二叉搜索树 ===" << std::endl;
    
    TreeNode* root = new TreeNode(2);
    root->left = new TreeNode(1);
    root->right = new TreeNode(3);
    
    Solution sol;
    std::cout << "是否有效BST: " << (sol.isValidBST(root) ? "是" : "否") << std::endl;
    
    return 0;
}
