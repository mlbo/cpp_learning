/**
 * 树路径问题演示
 */

#include <iostream>
#include <vector>
#include <string>

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// 所有根到叶子路径
void findAllPaths(TreeNode* root, std::vector<int>& path, std::vector<std::vector<int>>& result) {
    if (root == nullptr) return;
    
    path.push_back(root->val);
    
    if (root->left == nullptr && root->right == nullptr) {
        result.push_back(path);
    } else {
        findAllPaths(root->left, path, result);
        findAllPaths(root->right, path, result);
    }
    
    path.pop_back();
}

// 路径总和
void pathSum(TreeNode* root, int target, std::vector<int>& path, std::vector<std::vector<int>>& result) {
    if (root == nullptr) return;
    
    path.push_back(root->val);
    
    if (root->left == nullptr && root->right == nullptr && target == root->val) {
        result.push_back(path);
    } else {
        pathSum(root->left, target - root->val, path, result);
        pathSum(root->right, target - root->val, path, result);
    }
    
    path.pop_back();
}

void treePathDemo() {
    std::cout << "=== 树路径问题演示 ===" << std::endl;
    
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);
    
    // 所有路径
    std::vector<std::vector<int>> paths;
    std::vector<int> path;
    findAllPaths(root, path, paths);
    
    std::cout << "所有根到叶子路径:" << std::endl;
    for (const auto& p : paths) {
        std::cout << "  ";
        for (size_t i = 0; i < p.size(); ++i) {
            std::cout << p[i];
            if (i < p.size() - 1) std::cout << "->";
        }
        std::cout << std::endl;
    }
    
    // 路径总和
    std::vector<std::vector<int>> sumPaths;
    path.clear();
    pathSum(root, 7, path, sumPaths);
    
    std::cout << "\n和为7的路径:" << std::endl;
    for (const auto& p : sumPaths) {
        std::cout << "  ";
        for (int v : p) std::cout << v << " ";
        std::cout << std::endl;
    }
}

int main() {
    treePathDemo();
    return 0;
}
