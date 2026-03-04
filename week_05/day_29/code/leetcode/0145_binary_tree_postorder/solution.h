/**
 * LeetCode 145: 二叉树的后序遍历
 * 
 * 题目描述：
 * 给你二叉树的根节点 root，返回它节点值的后序遍历。
 * 
 * 后序遍历：左 -> 右 -> 根
 */

#ifndef SOLUTION_H
#define SOLUTION_H

#include <vector>
#include <stack>
#include <algorithm>

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
    std::vector<int> postorderTraversal(TreeNode* root) {
        std::vector<int> result;
        postorderRecursive(root, result);
        return result;
    }
    
    // 方法2：迭代实现（反转法）
    // 思路：前序是"根-左-右"，改成"根-右-左"，再反转得到"左-右-根"
    std::vector<int> postorderIterative(TreeNode* root) {
        std::vector<int> result;
        if (root == nullptr) return result;
        
        std::stack<TreeNode*> stk;
        stk.push(root);
        
        while (!stk.empty()) {
            TreeNode* node = stk.top();
            stk.pop();
            result.push_back(node->val);  // 访问根
            
            // 注意顺序：左先入栈（后出）
            if (node->left) stk.push(node->left);
            if (node->right) stk.push(node->right);
        }
        
        // 反转得到后序遍历结果
        std::reverse(result.begin(), result.end());
        return result;
    }

private:
    void postorderRecursive(TreeNode* root, std::vector<int>& result) {
        if (root == nullptr) return;
        postorderRecursive(root->left, result);   // 遍历左子树
        postorderRecursive(root->right, result);  // 遍历右子树
        result.push_back(root->val);              // 访问根
    }
};

#endif // SOLUTION_H
