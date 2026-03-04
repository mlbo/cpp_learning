/**
 * LeetCode 102: 层序遍历
 */

#include <iostream>
#include <vector>
#include <queue>

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

class Solution {
public:
    std::vector<std::vector<int>> levelOrder(TreeNode* root) {
        std::vector<std::vector<int>> result;
        if (!root) return result;
        
        std::queue<TreeNode*> q;
        q.push(root);
        
        while (!q.empty()) {
            int size = q.size();
            std::vector<int> level;
            for (int i = 0; i < size; ++i) {
                TreeNode* node = q.front();
                q.pop();
                level.push_back(node->val);
                if (node->left) q.push(node->left);
                if (node->right) q.push(node->right);
            }
            result.push_back(level);
        }
        return result;
    }
};

int main() {
    std::cout << "=== LeetCode 102: 层序遍历 ===" << std::endl;
    
    TreeNode* root = new TreeNode(3);
    root->left = new TreeNode(9);
    root->right = new TreeNode(20);
    root->right->left = new TreeNode(15);
    root->right->right = new TreeNode(7);
    
    Solution sol;
    auto result = sol.levelOrder(root);
    
    for (size_t i = 0; i < result.size(); ++i) {
        std::cout << "层" << i << ": ";
        for (int v : result[i]) std::cout << v << " ";
        std::cout << std::endl;
    }
    
    return 0;
}
