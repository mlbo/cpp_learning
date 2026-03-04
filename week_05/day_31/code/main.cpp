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

// 前向声明
namespace bst {
    void run_demo();
}

namespace condition_var {
    void run_demo();
}

namespace thread_handle {
    void run_demo();
}

namespace lc98 {
    void run_tests();
}

namespace lc700 {
    void run_tests();
}

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
    
    // 1. BST 基本操作演示
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "📚 模块一：二叉搜索树基本操作\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";
    bst::run_demo();
    
    // 2. 条件变量演示
    std::cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "📚 模块二：C++11 条件变量\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";
    condition_var::run_demo();
    
    // 3. 线程句柄析构行为
    std::cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "📚 模块三：EMC++ Item 38 - 线程句柄析构\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";
    thread_handle::run_demo();
    
    // 4. LeetCode 98 验证
    std::cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "📚 模块四：LeetCode 98 - 验证二叉搜索树\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";
    lc98::run_tests();
    
    // 5. LeetCode 700 验证
    std::cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "📚 模块五：LeetCode 700 - BST 中的搜索\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";
    lc700::run_tests();
    
    std::cout << "\n╔════════════════════════════════════════════╗\n";
    std::cout << "║          Day 31 学习完成！🎉               ║\n";
    std::cout << "╚════════════════════════════════════════════╝\n";
    
    return 0;
}
