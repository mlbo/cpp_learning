/**
 * LeetCode 144: 二叉树的前序遍历
 * 
 * 题目描述：
 * 给你二叉树的根节点 root，返回它节点值的前序遍历。
 * 
 * 前序遍历：根 -> 左 -> 右
 */

#ifndef SOLUTION_H
#define SOLUTION_H

#include <vector>
#include <stack>

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode* right) 
        : val(x), left(left), right(right) {}
};

class Solution {
public:
    // 方法1：递归实现
    std::vector<int> preorderTraversal(TreeNode* root) {
        std::vector<int> result;
        preorderRecursive(root, result);
        return result;
    }
    
    // 方法2：迭代实现
    std::vector<int> preorderIterative(TreeNode* root) {
        std::vector<int> result;
        if (root == nullptr) return result;
        
        std::stack<TreeNode*> stk;
        stk.push(root);
        
        while (!stk.empty()) {
            TreeNode* node = stk.top();
            stk.pop();
            result.push_back(node->val);
            
            // 右子节点先入栈（后出）
            if (node->right) stk.push(node->right);
            // 左子节点后入栈（先出）
            if (node->left) stk.push(node->left);
        }
        return result;
    }

private:
    void preorderRecursive(TreeNode* root, std::vector<int>& result) {
        if (root == nullptr) return;
        result.push_back(root->val);           // 访问根
        preorderRecursive(root->left, result);  // 遍历左子树
        preorderRecursive(root->right, result); // 遍历右子树
    }
};

#endif // SOLUTION_H
