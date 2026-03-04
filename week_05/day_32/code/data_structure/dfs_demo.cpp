/**
 * DFS算法演示
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

// 递归DFS
void dfsRecursive(TreeNode* root, std::vector<int>& result) {
    if (root == nullptr) return;
    result.push_back(root->val);
    dfsRecursive(root->left, result);
    dfsRecursive(root->right, result);
}

// 迭代DFS
std::vector<int> dfsIterative(TreeNode* root) {
    std::vector<int> result;
    if (root == nullptr) return result;
    
    std::stack<TreeNode*> stk;
    stk.push(root);
    
    while (!stk.empty()) {
        TreeNode* node = stk.top();
        stk.pop();
        result.push_back(node->val);
        
        if (node->right) stk.push(node->right);
        if (node->left) stk.push(node->left);
    }
    return result;
}

void dfsDemo() {
    std::cout << "=== DFS算法演示 ===" << std::endl;
    
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);
    
    std::vector<int> result1, result2;
    dfsRecursive(root, result1);
    result2 = dfsIterative(root);
    
    std::cout << "递归DFS: ";
    for (int v : result1) std::cout << v << " ";
    std::cout << std::endl;
    
    std::cout << "迭代DFS: ";
    for (int v : result2) std::cout << v << " ";
    std::cout << std::endl;
}

int main() {
    dfsDemo();
    return 0;
}
