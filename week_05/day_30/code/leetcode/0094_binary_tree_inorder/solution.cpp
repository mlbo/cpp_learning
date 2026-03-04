/**
 * LeetCode 94: 二叉树中序遍历
 */

#include <iostream>
#include <vector>
#include <stack>

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

class Solution {
public:
    // 递归
    std::vector<int> inorderTraversal(TreeNode* root) {
        std::vector<int> result;
        inorder(root, result);
        return result;
    }
    
    // 迭代
    std::vector<int> inorderIterative(TreeNode* root) {
        std::vector<int> result;
        std::stack<TreeNode*> stk;
        TreeNode* curr = root;
        
        while (curr || !stk.empty()) {
            while (curr) {
                stk.push(curr);
                curr = curr->left;
            }
            curr = stk.top();
            stk.pop();
            result.push_back(curr->val);
            curr = curr->right;
        }
        return result;
    }

private:
    void inorder(TreeNode* root, std::vector<int>& result) {
        if (!root) return;
        inorder(root->left, result);
        result.push_back(root->val);
        inorder(root->right, result);
    }
};

int main() {
    std::cout << "=== LeetCode 94: 中序遍历 ===" << std::endl;
    
    TreeNode* root = new TreeNode(1);
    root->right = new TreeNode(2);
    root->right->left = new TreeNode(3);
    
    Solution sol;
    auto result = sol.inorderTraversal(root);
    
    std::cout << "结果: ";
    for (int v : result) std::cout << v << " ";
    std::cout << std::endl;
    
    return 0;
}
