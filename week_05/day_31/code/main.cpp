/**
 * @file main.cpp
 * @brief Day 31 主程序入口 - 二叉搜索树专题
 * 
 * 本程序演示以下内容：
 * 1. 二叉搜索树（BST）的基本操作：插入、查找、删除
 * 2. C++11 条件变量（condition_variable）的使用
 * 3. EMC++ Item 38：线程句柄的析构行为
 * 4. LeetCode 98 和 700 题解验证
 */

#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

// ========================================
// 树节点定义（用于所有演示）
// ========================================
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    
    // 辅助函数：创建新节点
    static TreeNode* create(int x) {
        return new TreeNode(x);
    }
    
    // 辅助函数：释放树
    static void destroy(TreeNode* root) {
        if (root == nullptr) return;
        destroy(root->left);
        destroy(root->right);
        delete root;
    }
};

// ========================================
// 主函数
// ========================================
int main() {
    std::cout << "╔════════════════════════════════════════════╗\n";
    std::cout << "║     Day 31: 二叉搜索树 (BST) 专题          ║\n";
    std::cout << "╚════════════════════════════════════════════╝\n\n";
    
    std::cout << "本日各模块请分别运行对应可执行文件：\n";
    std::cout << "  - day31_bst_demo\n";
    std::cout << "  - day31_condition_variable\n";
    std::cout << "  - day31_item38\n";
    std::cout << "  - day31_lc0098\n";
    std::cout << "  - day31_lc0700\n";
    
    std::cout << "\n╔════════════════════════════════════════════╗\n";
    std::cout << "║          Day 31 学习完成！🎉               ║\n";
    std::cout << "╚════════════════════════════════════════════╝\n";
    
    return 0;
}
