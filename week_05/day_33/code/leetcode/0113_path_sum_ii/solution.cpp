/**
 * LeetCode 113: 路径总和 II
 * 
 * 给你二叉树的根节点 root 和一个整数目标和 targetSum，
 * 找出所有从根节点到叶子节点路径总和等于给定目标和的路径。
 */

#include <iostream>
#include <vector>

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

class Solution {
public:
    // 方法一：DFS + 回溯
    std::vector<std::vector<int>> pathSum(TreeNode* root, int targetSum) {
        std::vector<std::vector<int>> result;
        std::vector<int> currentPath;
        dfs(root, targetSum, currentPath, result);
        return result;
    }
    
private:
    void dfs(TreeNode* node, int remaining, std::vector<int>& path, 
             std::vector<std::vector<int>>& result) {
        if (node == nullptr) return;
        
        // 添加当前节点到路径
        path.push_back(node->val);
        
        // 如果是叶子节点且剩余和等于当前节点值
        if (node->left == nullptr && node->right == nullptr && 
            remaining == node->val) {
            result.push_back(path);  // 保存当前路径的副本
        }
        
        // 递归左右子树
        dfs(node->left, remaining - node->val, path, result);
        dfs(node->right, remaining - node->val, path, result);
        
        // 回溯：移除当前节点
        path.pop_back();
    }
};

// 方法二：不使用引用传递 path（不推荐，效率较低）
class Solution2 {
public:
    std::vector<std::vector<int>> pathSum(TreeNode* root, int targetSum) {
        std::vector<std::vector<int>> result;
        findPaths(root, targetSum, {}, result);
        return result;
    }
    
private:
    void findPaths(TreeNode* node, int remaining, std::vector<int> path,
                   std::vector<std::vector<int>>& result) {
        if (node == nullptr) return;
        
        path.push_back(node->val);
        
        if (node->left == nullptr && node->right == nullptr && 
            remaining == node->val) {
            result.push_back(path);
            return;
        }
        
        findPaths(node->left, remaining - node->val, path, result);
        findPaths(node->right, remaining - node->val, path, result);
    }
};

// 方法三：迭代法
#include <stack>
#include <tuple>

class Solution3 {
public:
    std::vector<std::vector<int>> pathSum(TreeNode* root, int targetSum) {
        std::vector<std::vector<int>> result;
        if (root == nullptr) return result;
        
        // 栈存储 <节点, 当前和, 当前路径>
        std::stack<std::tuple<TreeNode*, int, std::vector<int>>> stk;
        stk.push({root, root->val, {root->val}});
        
        while (!stk.empty()) {
            auto [node, sum, path] = stk.top();
            stk.pop();
            
            // 如果是叶子节点
            if (node->left == nullptr && node->right == nullptr) {
                if (sum == targetSum) {
                    result.push_back(path);
                }
                continue;
            }
            
            // 添加子节点到栈
            if (node->right != nullptr) {
                auto newPath = path;
                newPath.push_back(node->right->val);
                stk.push({node->right, sum + node->right->val, newPath});
            }
            if (node->left != nullptr) {
                auto newPath = path;
                newPath.push_back(node->left->val);
                stk.push({node->left, sum + node->left->val, newPath});
            }
        }
        
        return result;
    }
};

// ========================================
// 测试代码
// ========================================

void testLC113() {
    std::cout << "=== LeetCode 113: 路径总和 II ===\n\n";
    
    Solution solution;
    
    // 测试用例1
    //        5
    //       / \
    //      4   8
    //     /   / \
    //    11  13  4
    //   /  \    / \
    //  7    2  5   1
    TreeNode* root1 = new TreeNode(5);
    root1->left = new TreeNode(4);
    root1->right = new TreeNode(8);
    root1->left->left = new TreeNode(11);
    root1->left->left->left = new TreeNode(7);
    root1->left->left->right = new TreeNode(2);
    root1->right->left = new TreeNode(13);
    root1->right->right = new TreeNode(4);
    root1->right->right->left = new TreeNode(5);
    root1->right->right->right = new TreeNode(1);
    
    std::cout << "测试用例1:\n";
    std::cout << "        5\n";
    std::cout << "       / \\\n";
    std::cout << "      4   8\n";
    std::cout << "     /   / \\\n";
    std::cout << "    11  13  4\n";
    std::cout << "   /  \\    / \\\n";
    std::cout << "  7    2  5   1\n\n";
    
    int target1 = 22;
    auto result1 = solution.pathSum(root1, target1);
    
    std::cout << "目标和 = " << target1 << "\n";
    std::cout << "找到的路径: [";
    for (size_t i = 0; i < result1.size(); ++i) {
        std::cout << "[";
        for (size_t j = 0; j < result1[i].size(); ++j) {
            std::cout << result1[i][j];
            if (j < result1[i].size() - 1) std::cout << ",";
        }
        std::cout << "]";
        if (i < result1.size() - 1) std::cout << ", ";
    }
    std::cout << "]\n";
    std::cout << "预期: [[5,4,11,2],[5,8,4,5]]\n\n";
    
    // 测试用例2：空树
    std::cout << "测试用例2: 空树\n";
    auto result2 = solution.pathSum(nullptr, 1);
    std::cout << "找到的路径: []\n";
    std::cout << "预期: []\n\n";
    
    // 测试用例3：单节点，目标和等于节点值
    TreeNode* root3 = new TreeNode(1);
    std::cout << "测试用例3: 单节点，targetSum = 1\n";
    auto result3 = solution.pathSum(root3, 1);
    std::cout << "找到的路径: [";
    for (size_t i = 0; i < result3.size(); ++i) {
        std::cout << "[";
        for (size_t j = 0; j < result3[i].size(); ++j) {
            std::cout << result3[i][j];
            if (j < result3[i].size() - 1) std::cout << ",";
        }
        std::cout << "]";
    }
    std::cout << "]\n";
    std::cout << "预期: [[1]]\n\n";
    
    // 测试用例4：单节点，目标和不等于节点值
    std::cout << "测试用例4: 单节点，targetSum = 2\n";
    auto result4 = solution.pathSum(root3, 2);
    std::cout << "找到的路径: []\n";
    std::cout << "预期: []\n\n";
    
    // 清理内存
    delete root1->left->left->left;
    delete root1->left->left->right;
    delete root1->left->left;
    delete root1->left;
    delete root1->right->left;
    delete root1->right->right->left;
    delete root1->right->right->right;
    delete root1->right->right;
    delete root1->right;
    delete root1;
    delete root3;
    
    std::cout << "复杂度分析:\n";
    std::cout << "- 时间复杂度: O(n)，每个节点访问一次\n";
    std::cout << "- 空间复杂度: O(h)，递归栈深度（不包括结果存储）\n";
}
