/**
 * LeetCode 105: 从前序与中序遍历构造二叉树
 */

#include <iostream>
#include <vector>
#include <unordered_map>

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

class Solution {
public:
    TreeNode* buildTree(std::vector<int>& preorder, std::vector<int>& inorder) {
        for (int i = 0; i < inorder.size(); ++i) {
            inorderMap_[inorder[i]] = i;
        }
        return build(preorder, 0, preorder.size() - 1, 0, inorder.size() - 1);
    }

private:
    std::unordered_map<int, int> inorderMap_;
    
    TreeNode* build(std::vector<int>& preorder, int preL, int preR, int inL, int inR) {
        if (preL > preR) return nullptr;
        
        int rootVal = preorder[preL];
        TreeNode* root = new TreeNode(rootVal);
        
        int rootPos = inorderMap_[rootVal];
        int leftSize = rootPos - inL;
        
        root->left = build(preorder, preL + 1, preL + leftSize, inL, rootPos - 1);
        root->right = build(preorder, preL + leftSize + 1, preR, rootPos + 1, inR);
        
        return root;
    }
};

int main() {
    std::cout << "=== LeetCode 105: 构造二叉树 ===" << std::endl;
    
    std::vector<int> preorder = {3, 9, 20, 15, 7};
    std::vector<int> inorder = {9, 3, 15, 20, 7};
    
    Solution sol;
    TreeNode* root = sol.buildTree(preorder, inorder);
    
    std::cout << "构造完成，根节点: " << root->val << std::endl;
    
    return 0;
}
