/**
 * 二叉树数据结构演示
 * 展示二叉树的基本概念和操作
 */

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

// 二叉树节点定义
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode* right) 
        : val(x), left(left), right(right) {}
};

// 二叉树操作类
class BinaryTree {
public:
    // 前序遍历：根 -> 左 -> 右
    static std::vector<int> preorderTraversal(TreeNode* root) {
        std::vector<int> result;
        preorderHelper(root, result);
        return result;
    }
    
    // 中序遍历：左 -> 根 -> 右
    static std::vector<int> inorderTraversal(TreeNode* root) {
        std::vector<int> result;
        inorderHelper(root, result);
        return result;
    }
    
    // 后序遍历：左 -> 右 -> 根
    static std::vector<int> postorderTraversal(TreeNode* root) {
        std::vector<int> result;
        postorderHelper(root, result);
        return result;
    }
    
    // 层序遍历
    static std::vector<std::vector<int>> levelOrder(TreeNode* root) {
        std::vector<std::vector<int>> result;
        if (root == nullptr) return result;
        
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
    
    // 计算树的高度
    static int getHeight(TreeNode* root) {
        if (root == nullptr) return 0;
        return 1 + std::max(getHeight(root->left), getHeight(root->right));
    }
    
    // 统计节点数量
    static int countNodes(TreeNode* root) {
        if (root == nullptr) return 0;
        return 1 + countNodes(root->left) + countNodes(root->right);
    }
    
    // 判断是否为完全二叉树
    static bool isCompleteTree(TreeNode* root) {
        if (root == nullptr) return true;
        
        std::queue<TreeNode*> q;
        q.push(root);
        bool seenNull = false;
        
        while (!q.empty()) {
            TreeNode* node = q.front();
            q.pop();
            
            if (node == nullptr) {
                seenNull = true;
            } else {
                if (seenNull) return false;
                q.push(node->left);
                q.push(node->right);
            }
        }
        return true;
    }
    
    // 释放树内存
    static void deleteTree(TreeNode* root) {
        if (root == nullptr) return;
        deleteTree(root->left);
        deleteTree(root->right);
        delete root;
    }

private:
    static void preorderHelper(TreeNode* root, std::vector<int>& result) {
        if (root == nullptr) return;
        result.push_back(root->val);
        preorderHelper(root->left, result);
        preorderHelper(root->right, result);
    }
    
    static void inorderHelper(TreeNode* root, std::vector<int>& result) {
        if (root == nullptr) return;
        inorderHelper(root->left, result);
        result.push_back(root->val);
        inorderHelper(root->right, result);
    }
    
    static void postorderHelper(TreeNode* root, std::vector<int>& result) {
        if (root == nullptr) return;
        postorderHelper(root->left, result);
        postorderHelper(root->right, result);
        result.push_back(root->val);
    }
};

// 演示函数
void binaryTreeDemo() {
    std::cout << "=== 二叉树数据结构演示 ===" << std::endl;
    
    // 构建测试树
    /*
     *       1
     *      / \
     *     2   3
     *    / \
     *   4   5
     */
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);
    
    // 遍历演示
    auto preorder = BinaryTree::preorderTraversal(root);
    auto inorder = BinaryTree::inorderTraversal(root);
    auto postorder = BinaryTree::postorderTraversal(root);
    auto levelorder = BinaryTree::levelOrder(root);
    
    std::cout << "前序遍历: ";
    for (int v : preorder) std::cout << v << " ";
    std::cout << std::endl;
    
    std::cout << "中序遍历: ";
    for (int v : inorder) std::cout << v << " ";
    std::cout << std::endl;
    
    std::cout << "后序遍历: ";
    for (int v : postorder) std::cout << v << " ";
    std::cout << std::endl;
    
    std::cout << "层序遍历: " << std::endl;
    for (const auto& level : levelorder) {
        std::cout << "  [";
        for (size_t i = 0; i < level.size(); ++i) {
            std::cout << level[i];
            if (i < level.size() - 1) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    }
    
    // 树属性
    std::cout << "树高度: " << BinaryTree::getHeight(root) << std::endl;
    std::cout << "节点数: " << BinaryTree::countNodes(root) << std::endl;
    std::cout << "是否完全二叉树: " << (BinaryTree::isCompleteTree(root) ? "是" : "否") << std::endl;
    
    // 清理
    BinaryTree::deleteTree(root);
}

int main() {
    binaryTreeDemo();
    return 0;
}
