/**
 * @file bst_demo.cpp
 * @brief 二叉搜索树演示程序
 * 
 * 本文件演示：
 * 1. BST 的定义和性质
 * 2. BST 的查找操作
 * 3. BST 的插入操作
 * 4. BST 的删除操作
 */

#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <iomanip>

// ========================================
// 树节点定义
// ========================================
struct BSTNode {
    int val;
    BSTNode* left;
    BSTNode* right;
    
    BSTNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// ========================================
// BST 类定义
// ========================================
class BinarySearchTree {
private:
    BSTNode* root;
    
    // 私有递归插入
    BSTNode* insertHelper(BSTNode* node, int val) {
        if (node == nullptr) {
            return new BSTNode(val);
        }
        
        if (val < node->val) {
            node->left = insertHelper(node->left, val);
        } else if (val > node->val) {
            node->right = insertHelper(node->right, val);
        }
        // 如果 val == node->val，不插入重复值
        
        return node;
    }
    
    // 私有递归查找
    BSTNode* searchHelper(BSTNode* node, int val) {
        if (node == nullptr || node->val == val) {
            return node;
        }
        
        if (val < node->val) {
            return searchHelper(node->left, val);
        } else {
            return searchHelper(node->right, val);
        }
    }
    
    // 找到最小节点
    BSTNode* findMin(BSTNode* node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }
    
    // 找到最大节点
    BSTNode* findMax(BSTNode* node) {
        while (node->right != nullptr) {
            node = node->right;
        }
        return node;
    }
    
    // 私有递归删除
    BSTNode* deleteHelper(BSTNode* node, int val) {
        if (node == nullptr) {
            return nullptr;
        }
        
        if (val < node->val) {
            // 目标值在左子树
            node->left = deleteHelper(node->left, val);
        } else if (val > node->val) {
            // 目标值在右子树
            node->right = deleteHelper(node->right, val);
        } else {
            // 找到目标节点
            // 情况1：叶子节点或只有一个孩子
            if (node->left == nullptr) {
                BSTNode* rightChild = node->right;
                delete node;
                return rightChild;
            } else if (node->right == nullptr) {
                BSTNode* leftChild = node->left;
                delete node;
                return leftChild;
            }
            
            // 情况2：有两个孩子
            // 找到右子树的最小节点（中序后继）
            BSTNode* successor = findMin(node->right);
            node->val = successor->val;  // 用后继值替换当前节点值
            node->right = deleteHelper(node->right, successor->val);  // 删除后继
        }
        
        return node;
    }
    
    // 中序遍历
    void inorderHelper(BSTNode* node, std::vector<int>& result) {
        if (node == nullptr) return;
        inorderHelper(node->left, result);
        result.push_back(node->val);
        inorderHelper(node->right, result);
    }
    
    // 打印树结构（辅助）
    void printTreeHelper(BSTNode* node, int depth) {
        if (node == nullptr) return;
        
        printTreeHelper(node->right, depth + 1);
        
        std::cout << std::string(depth * 4, ' ') << node->val << std::endl;
        
        printTreeHelper(node->left, depth + 1);
    }
    
    // 销毁树
    void destroy(BSTNode* node) {
        if (node == nullptr) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }
    
public:
    BinarySearchTree() : root(nullptr) {}
    
    ~BinarySearchTree() {
        destroy(root);
    }
    
    // 插入值
    void insert(int val) {
        root = insertHelper(root, val);
    }
    
    // 查找值
    bool search(int val) {
        return searchHelper(root, val) != nullptr;
    }
    
    // 删除值
    void remove(int val) {
        root = deleteHelper(root, val);
    }
    
    // 获取中序遍历结果
    std::vector<int> inorder() {
        std::vector<int> result;
        inorderHelper(root, result);
        return result;
    }
    
    // 打印树结构
    void printTree() {
        std::cout << "树结构（右-根-左）:\n";
        printTreeHelper(root, 0);
    }
    
    // 层序遍历
    void levelOrder() {
        if (root == nullptr) return;
        
        std::queue<BSTNode*> q;
        q.push(root);
        
        std::cout << "层序遍历: ";
        while (!q.empty()) {
            BSTNode* node = q.front();
            q.pop();
            
            std::cout << node->val << " ";
            
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        std::cout << std::endl;
    }
    
    // 获取树高
    int getHeight(BSTNode* node) {
        if (node == nullptr) return 0;
        return 1 + std::max(getHeight(node->left), getHeight(node->right));
    }
    
    int height() {
        return getHeight(root);
    }
};

// ========================================
// 演示函数
// ========================================
namespace bst {

void demo_properties() {
    std::cout << "📋 BST 核心性质\n";
    std::cout << "────────────────────────────────────\n";
    std::cout << "1. 左子树所有节点值 < 根节点值\n";
    std::cout << "2. 右子树所有节点值 > 根节点值\n";
    std::cout << "3. 左右子树本身也是 BST\n";
    std::cout << "4. 中序遍历得到有序序列\n\n";
    
    std::cout << "⏱️ 时间复杂度分析:\n";
    std::cout << "   - 查找: 平均 O(log n), 最坏 O(n)\n";
    std::cout << "   - 插入: 平均 O(log n), 最坏 O(n)\n";
    std::cout << "   - 删除: 平均 O(log n), 最坏 O(n)\n\n";
}

void demo_insert_and_search() {
    std::cout << "📋 演示：插入和查找\n";
    std::cout << "────────────────────────────────────\n";
    
    BinarySearchTree bst;
    
    // 插入序列
    std::vector<int> values = {8, 3, 10, 1, 6, 14, 4, 7, 13};
    std::cout << "插入序列: ";
    for (int v : values) {
        std::cout << v << " ";
        bst.insert(v);
    }
    std::cout << "\n\n";
    
    // 打印树结构
    bst.printTree();
    std::cout << "\n";
    
    // 层序遍历
    bst.levelOrder();
    
    // 中序遍历
    std::cout << "中序遍历: ";
    for (int v : bst.inorder()) {
        std::cout << v << " ";
    }
    std::cout << "（有序序列）\n\n";
    
    // 查找演示
    std::cout << "查找演示:\n";
    std::vector<int> searchValues = {6, 13, 5, 100};
    for (int v : searchValues) {
        bool found = bst.search(v);
        std::cout << "   查找 " << std::setw(3) << v << ": "
                  << (found ? "✓ 找到" : "✗ 未找到") << "\n";
    }
    std::cout << "\n";
}

void demo_delete() {
    std::cout << "📋 演示：删除操作\n";
    std::cout << "────────────────────────────────────\n";
    
    // 重建树
    BinarySearchTree bst;
    std::vector<int> values = {8, 3, 10, 1, 6, 14, 4, 7, 13};
    for (int v : values) {
        bst.insert(v);
    }
    
    std::cout << "原始树:\n";
    bst.printTree();
    std::cout << "\n中序遍历: ";
    for (int v : bst.inorder()) {
        std::cout << v << " ";
    }
    std::cout << "\n\n";
    
    // 情况1：删除叶子节点
    std::cout << "【情况1】删除叶子节点 1:\n";
    bst.remove(1);
    bst.printTree();
    std::cout << "中序遍历: ";
    for (int v : bst.inorder()) {
        std::cout << v << " ";
    }
    std::cout << "\n\n";
    
    // 情况2：删除只有一个孩子的节点
    std::cout << "【情况2】删除单孩子节点 14（只有左孩子 13）:\n";
    bst.remove(14);
    bst.printTree();
    std::cout << "中序遍历: ";
    for (int v : bst.inorder()) {
        std::cout << v << " ";
    }
    std::cout << "\n\n";
    
    // 情况3：删除有两个孩子的节点
    std::cout << "【情况3】删除双孩子节点 3（孩子: 6）:\n";
    bst.remove(3);
    bst.printTree();
    std::cout << "中序遍历: ";
    for (int v : bst.inorder()) {
        std::cout << v << " ";
    }
    std::cout << "\n\n";
    
    std::cout << "💡 删除要点:\n";
    std::cout << "   - 叶子节点: 直接删除\n";
    std::cout << "   - 单孩子: 用孩子替代\n";
    std::cout << "   - 双孩子: 用后继/前驱替代，再删除后继/前驱\n\n";
}

void run_demo() {
    demo_properties();
    demo_insert_and_search();
    demo_delete();
}

} // namespace bst

// ========================================
// 独立程序入口
// ========================================
int main() {
    bst::run_demo();
    return 0;
}
