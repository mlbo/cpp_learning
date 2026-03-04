/**
 * @file tree_node.cpp
 * @brief 二叉树节点操作实现
 * @author C++ Tutorial
 * @date Day 20
 */

#include "tree_node.h"
#include <stack>
#include <sstream>
#include <iomanip>
#include <algorithm>

namespace tree_ops {

// ==================== 创建与销毁 ====================

TreeNode* createTree(const std::vector<std::optional<int>>& values) {
    if (values.empty() || !values[0].has_value()) {
        return nullptr;
    }
    
    TreeNode* root = new TreeNode(values[0].value());
    std::queue<TreeNode*> q;
    q.push(root);
    
    size_t i = 1;
    while (!q.empty() && i < values.size()) {
        TreeNode* node = q.front();
        q.pop();
        
        // 左孩子
        if (i < values.size()) {
            if (values[i].has_value()) {
                node->left = new TreeNode(values[i].value());
                q.push(node->left);
            }
            ++i;
        }
        
        // 右孩子
        if (i < values.size()) {
            if (values[i].has_value()) {
                node->right = new TreeNode(values[i].value());
                q.push(node->right);
            }
            ++i;
        }
    }
    
    return root;
}

TreeNode* createTreeSimple(const std::vector<int>& values, int nullValue) {
    if (values.empty() || values[0] == nullValue) {
        return nullptr;
    }
    
    TreeNode* root = new TreeNode(values[0]);
    std::queue<TreeNode*> q;
    q.push(root);
    
    size_t i = 1;
    while (!q.empty() && i < values.size()) {
        TreeNode* node = q.front();
        q.pop();
        
        // 左孩子
        if (i < values.size()) {
            if (values[i] != nullValue) {
                node->left = new TreeNode(values[i]);
                q.push(node->left);
            }
            ++i;
        }
        
        // 右孩子
        if (i < values.size()) {
            if (values[i] != nullValue) {
                node->right = new TreeNode(values[i]);
                q.push(node->right);
            }
            ++i;
        }
    }
    
    return root;
}

void deleteTree(TreeNode* root) {
    if (root == nullptr) return;
    
    // 使用后序遍历释放
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

TreeNode* copyTree(const TreeNode* root) {
    if (root == nullptr) return nullptr;
    
    TreeNode* newNode = new TreeNode(root->val);
    newNode->left = copyTree(root->left);
    newNode->right = copyTree(root->right);
    
    return newNode;
}

// ==================== 遍历操作 ====================

void printLevelOrder(const TreeNode* root, std::ostream& os) {
    if (root == nullptr) {
        os << "[]" << std::endl;
        return;
    }
    
    std::queue<const TreeNode*> q;
    q.push(root);
    
    os << "[";
    bool first = true;
    
    while (!q.empty()) {
        const TreeNode* node = q.front();
        q.pop();
        
        if (!first) os << ", ";
        first = false;
        
        if (node) {
            os << node->val;
            q.push(node->left);
            q.push(node->right);
        } else {
            os << "null";
        }
    }
    
    os << "]" << std::endl;
}

void preOrder(const TreeNode* root, std::function<void(const TreeNode*)> visit) {
    if (root == nullptr) return;
    visit(root);
    preOrder(root->left, visit);
    preOrder(root->right, visit);
}

void inOrder(const TreeNode* root, std::function<void(const TreeNode*)> visit) {
    if (root == nullptr) return;
    inOrder(root->left, visit);
    visit(root);
    inOrder(root->right, visit);
}

void postOrder(const TreeNode* root, std::function<void(const TreeNode*)> visit) {
    if (root == nullptr) return;
    postOrder(root->left, visit);
    postOrder(root->right, visit);
    visit(root);
}

std::vector<int> preOrderValues(const TreeNode* root) {
    std::vector<int> result;
    preOrder(root, [&result](const TreeNode* node) {
        result.push_back(node->val);
    });
    return result;
}

std::vector<int> inOrderValues(const TreeNode* root) {
    std::vector<int> result;
    inOrder(root, [&result](const TreeNode* node) {
        result.push_back(node->val);
    });
    return result;
}

std::vector<std::vector<int>> levelOrderValues(const TreeNode* root) {
    std::vector<std::vector<int>> result;
    if (root == nullptr) return result;
    
    std::queue<const TreeNode*> q;
    q.push(root);
    
    while (!q.empty()) {
        int levelSize = static_cast<int>(q.size());
        std::vector<int> currentLevel;
        
        for (int i = 0; i < levelSize; ++i) {
            const TreeNode* node = q.front();
            q.pop();
            
            currentLevel.push_back(node->val);
            
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        
        result.push_back(currentLevel);
    }
    
    return result;
}

// ==================== 属性查询 ====================

int getNodeCount(const TreeNode* root) {
    if (root == nullptr) return 0;
    return 1 + getNodeCount(root->left) + getNodeCount(root->right);
}

int getHeight(const TreeNode* root) {
    if (root == nullptr) return 0;
    return 1 + std::max(getHeight(root->left), getHeight(root->right));
}

int getMaxWidth(const TreeNode* root) {
    if (root == nullptr) return 0;
    
    int maxWidth = 0;
    std::queue<const TreeNode*> q;
    q.push(root);
    
    while (!q.empty()) {
        int levelSize = static_cast<int>(q.size());
        maxWidth = std::max(maxWidth, levelSize);
        
        for (int i = 0; i < levelSize; ++i) {
            const TreeNode* node = q.front();
            q.pop();
            
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
    }
    
    return maxWidth;
}

// ==================== 查找操作 ====================

TreeNode* findNode(TreeNode* root, int val) {
    if (root == nullptr) return nullptr;
    if (root->val == val) return root;
    
    TreeNode* leftResult = findNode(root->left, val);
    if (leftResult) return leftResult;
    
    return findNode(root->right, val);
}

const TreeNode* findNode(const TreeNode* root, int val) {
    if (root == nullptr) return nullptr;
    if (root->val == val) return root;
    
    const TreeNode* leftResult = findNode(root->left, val);
    if (leftResult) return leftResult;
    
    return findNode(root->right, val);
}

bool contains(const TreeNode* root, int val) {
    return findNode(root, val) != nullptr;
}

// ==================== 可视化 ====================

// 辅助函数：打印树结构
static void printTreeHelper(const TreeNode* root, std::ostream& os, 
                           const std::string& prefix, bool isLeft, bool hasSibling) {
    if (root == nullptr) return;
    
    os << prefix;
    
    if (hasSibling) {
        os << (isLeft ? "├── " : "└── ");
    } else {
        os << (isLeft ? "┌── " : "└── ");
    }
    
    os << root->val << std::endl;
    
    bool hasLeft = root->left != nullptr;
    bool hasRight = root->right != nullptr;
    
    if (hasLeft || hasRight) {
        std::string newPrefix = prefix;
        if (hasSibling) {
            newPrefix += (isLeft ? "│   " : "    ");
        } else {
            newPrefix += (isLeft ? "    " : "    ");
        }
        
        if (hasLeft) {
            printTreeHelper(root->left, os, newPrefix, true, hasRight);
        }
        if (hasRight) {
            printTreeHelper(root->right, os, newPrefix, false, false);
        }
    }
}

void printTree(const TreeNode* root, std::ostream& os) {
    if (root == nullptr) {
        os << "(空树)" << std::endl;
        return;
    }
    
    os << root->val << std::endl;
    
    bool hasLeft = root->left != nullptr;
    bool hasRight = root->right != nullptr;
    
    if (hasLeft) {
        printTreeHelper(root->left, os, "", true, hasRight);
    }
    if (hasRight) {
        printTreeHelper(root->right, os, "", false, false);
    }
}

// ==================== 辅助函数 ====================

std::string levelOrderToString(const std::vector<std::vector<int>>& levels) {
    std::ostringstream oss;
    oss << "[";
    
    for (size_t i = 0; i < levels.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << "[";
        for (size_t j = 0; j < levels[i].size(); ++j) {
            if (j > 0) oss << ", ";
            oss << levels[i][j];
        }
        oss << "]";
    }
    
    oss << "]";
    return oss.str();
}

bool isSameTree(const TreeNode* t1, const TreeNode* t2) {
    // 两者都为空
    if (t1 == nullptr && t2 == nullptr) return true;
    // 一个为空，一个不为空
    if (t1 == nullptr || t2 == nullptr) return false;
    // 值不同
    if (t1->val != t2->val) return false;
    
    // 递归比较子树
    return isSameTree(t1->left, t2->left) && isSameTree(t1->right, t2->right);
}

} // namespace tree_ops
