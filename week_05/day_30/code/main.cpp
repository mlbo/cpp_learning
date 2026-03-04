/**
 * Day 30: 树遍历 - 主程序
 */

#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <mutex>
#include <thread>
#include <future>

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// 前序遍历
void preorder(TreeNode* root, std::vector<int>& result) {
    if (!root) return;
    result.push_back(root->val);
    preorder(root->left, result);
    preorder(root->right, result);
}

// 中序遍历
void inorder(TreeNode* root, std::vector<int>& result) {
    if (!root) return;
    inorder(root->left, result);
    result.push_back(root->val);
    inorder(root->right, result);
}

// 后序遍历
void postorder(TreeNode* root, std::vector<int>& result) {
    if (!root) return;
    postorder(root->left, result);
    postorder(root->right, result);
    result.push_back(root->val);
}

// 层序遍历
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

void deleteTree(TreeNode* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    std::cout << "=== Day 30: 树遍历 ===" << std::endl;
    
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);
    
    // 遍历演示
    std::vector<int> pre, in, post;
    preorder(root, pre);
    inorder(root, in);
    postorder(root, post);
    
    std::cout << "\n前序遍历: ";
    for (int v : pre) std::cout << v << " ";
    std::cout << "\n中序遍历: ";
    for (int v : in) std::cout << v << " ";
    std::cout << "\n后序遍历: ";
    for (int v : post) std::cout << v << " ";
    
    std::cout << "\n\n层序遍历:" << std::endl;
    auto levels = levelOrder(root);
    for (size_t i = 0; i < levels.size(); ++i) {
        std::cout << "  层" << i << ": ";
        for (int v : levels[i]) std::cout << v << " ";
        std::cout << std::endl;
    }
    
    // mutex演示
    std::cout << "\n--- mutex演示 ---" << std::endl;
    std::mutex mtx;
    int counter = 0;
    
    std::thread t1([&]() {
        for (int i = 0; i < 1000; ++i) {
            std::lock_guard<std::mutex> lock(mtx);
            counter++;
        }
    });
    std::thread t2([&]() {
        for (int i = 0; i < 1000; ++i) {
            std::lock_guard<std::mutex> lock(mtx);
            counter++;
        }
    });
    
    t1.join();
    t2.join();
    std::cout << "计数器结果: " << counter << std::endl;
    
    deleteTree(root);
    std::cout << "\n=== Day 30 学习完成 ===" << std::endl;
    return 0;
}
