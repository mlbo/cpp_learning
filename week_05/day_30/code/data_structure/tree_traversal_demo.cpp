/**
 * 树遍历演示
 */

#include <iostream>
#include <vector>
#include <stack>
#include <queue>

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// 迭代前序
std::vector<int> preorderIterative(TreeNode* root) {
    std::vector<int> result;
    if (!root) return result;
    
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

// 迭代中序
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

void treeTraversalDemo() {
    std::cout << "=== 树遍历演示 ===" << std::endl;
    
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);
    
    auto pre = preorderIterative(root);
    auto in = inorderIterative(root);
    
    std::cout << "迭代前序: ";
    for (int v : pre) std::cout << v << " ";
    std::cout << "\n迭代中序: ";
    for (int v : in) std::cout << v << " ";
    std::cout << std::endl;
}

int main() {
    treeTraversalDemo();
    return 0;
}
