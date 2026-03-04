/**
 * Day 29: 二叉树入门 - 主程序
 * 
 * 本程序演示二叉树数据结构、std::thread基础和EMC++ Item 35
 */

#include <iostream>
#include <vector>
#include <thread>
#include <future>

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

// 前向声明
void binaryTreeDemo();
void threadDemo();
void item35Demo();

// 二叉树遍历函数
void preorderTraversal(TreeNode* root, std::vector<int>& result) {
    if (root == nullptr) return;
    result.push_back(root->val);
    preorderTraversal(root->left, result);
    preorderTraversal(root->right, result);
}

void inorderTraversal(TreeNode* root, std::vector<int>& result) {
    if (root == nullptr) return;
    inorderTraversal(root->left, result);
    result.push_back(root->val);
    inorderTraversal(root->right, result);
}

void postorderTraversal(TreeNode* root, std::vector<int>& result) {
    if (root == nullptr) return;
    postorderTraversal(root->left, result);
    postorderTraversal(root->right, result);
    result.push_back(root->val);
}

// 辅助函数：打印向量
void printVector(const std::string& name, const std::vector<int>& vec) {
    std::cout << name << ": [";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i];
        if (i < vec.size() - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
}

// 辅助函数：创建测试树
TreeNode* createTestTree() {
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
    return root;
}

// 释放树内存
void deleteTree(TreeNode* root) {
    if (root == nullptr) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    std::cout << "=== Day 29: 二叉树入门 ===" << std::endl;
    std::cout << std::endl;
    
    // 1. 二叉树数据结构演示
    std::cout << "--- 1. 二叉树数据结构 ---" << std::endl;
    TreeNode* root = createTestTree();
    
    std::vector<int> preorder, inorder, postorder;
    preorderTraversal(root, preorder);
    inorderTraversal(root, inorder);
    postorderTraversal(root, postorder);
    
    printVector("前序遍历(根-左-右)", preorder);
    printVector("中序遍历(左-根-右)", inorder);
    printVector("后序遍历(左-右-根)", postorder);
    
    deleteTree(root);
    std::cout << std::endl;
    
    // 2. std::thread 基础演示
    std::cout << "--- 2. std::thread 基础 ---" << std::endl;
    threadDemo();
    std::cout << std::endl;
    
    // 3. EMC++ Item 35 演示
    std::cout << "--- 3. EMC++ Item 35: 任务优先于线程 ---" << std::endl;
    item35Demo();
    std::cout << std::endl;
    
    std::cout << "=== Day 29 学习完成 ===" << std::endl;
    return 0;
}

// 线程演示函数
void threadDemo() {
    // 方式1：使用函数指针
    std::thread t1([]() {
        std::cout << "  线程1: 使用Lambda表达式" << std::endl;
    });
    t1.join();
    
    // 方式2：使用函数对象
    struct Worker {
        void operator()() const {
            std::cout << "  线程2: 使用函数对象" << std::endl;
        }
    };
    std::thread t2{Worker{}};
    t2.join();
    
    // 方式3：多线程并行
    std::vector<std::thread> threads;
    for (int i = 0; i < 3; ++i) {
        threads.emplace_back([i]() {
            std::cout << "  工作线程 " << i << " 正在执行" << std::endl;
        });
    }
    for (auto& t : threads) {
        t.join();
    }
}

// Item 35 演示：基于任务 vs 基于线程
void item35Demo() {
    // 计算函数
    auto compute = [](int n) {
        return n * n;
    };
    
    // 基于线程的方式（不推荐）
    int result1 = 0;
    std::thread t([&result1, compute]() {
        result1 = compute(10);
    });
    t.join();
    std::cout << "  基于线程结果: " << result1 << std::endl;
    
    // 基于任务的方式（推荐）
    auto future = std::async(std::launch::async, compute, 10);
    int result2 = future.get();
    std::cout << "  基于任务结果: " << result2 << std::endl;
    std::cout << "  结论: 基于任务更简洁、更安全！" << std::endl;
}
