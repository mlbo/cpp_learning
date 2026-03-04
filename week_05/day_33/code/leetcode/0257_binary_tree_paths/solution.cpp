/**
 * LeetCode 257: 二叉树的所有路径
 * 
 * 给定一个二叉树的根节点 root，按任意顺序返回所有从根节点到叶子节点的路径。
 */

#include <iostream>
#include <vector>
#include <string>

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
    // 方法一：DFS + 回溯（推荐）
    std::vector<std::string> binaryTreePaths(TreeNode* root) {
        std::vector<std::string> result;
        if (root == nullptr) return result;
        
        dfs(root, "", result);
        return result;
    }
    
private:
    void dfs(TreeNode* node, std::string path, std::vector<std::string>& result) {
        // 构建当前路径
        if (!path.empty()) {
            path += "->";
        }
        path += std::to_string(node->val);
        
        // 如果是叶子节点，添加到结果
        if (node->left == nullptr && node->right == nullptr) {
            result.push_back(path);
            return;
        }
        
        // 递归左右子树
        if (node->left != nullptr) {
            dfs(node->left, path, result);
        }
        if (node->right != nullptr) {
            dfs(node->right, path, result);
        }
    }
};

// 方法二：使用 vector 记录路径（更清晰的回溯）
class Solution2 {
public:
    std::vector<std::string> binaryTreePaths(TreeNode* root) {
        std::vector<std::string> result;
        std::vector<int> path;
        dfs(root, path, result);
        return result;
    }
    
private:
    void dfs(TreeNode* node, std::vector<int>& path, std::vector<std::string>& result) {
        if (node == nullptr) return;
        
        // 添加当前节点到路径
        path.push_back(node->val);
        
        // 如果是叶子节点
        if (node->left == nullptr && node->right == nullptr) {
            result.push_back(buildPathString(path));
        } else {
            // 递归左右子树
            dfs(node->left, path, result);
            dfs(node->right, path, result);
        }
        
        // 回溯：移除当前节点
        path.pop_back();
    }
    
    std::string buildPathString(const std::vector<int>& path) {
        std::string result;
        for (size_t i = 0; i < path.size(); ++i) {
            result += std::to_string(path[i]);
            if (i < path.size() - 1) {
                result += "->";
            }
        }
        return result;
    }
};

// 方法三：迭代法（BFS）
#include <queue>
#include <utility>

class Solution3 {
public:
    std::vector<std::string> binaryTreePaths(TreeNode* root) {
        std::vector<std::string> result;
        if (root == nullptr) return result;
        
        // 队列存储 <节点, 路径字符串>
        std::queue<std::pair<TreeNode*, std::string>> q;
        q.push({root, std::to_string(root->val)});
        
        while (!q.empty()) {
            // C++11写法（注释）:
            // std::pair<TreeNode*, std::string> cur = q.front();
            // TreeNode* node = cur.first;
            // std::string path = cur.second;
            auto [node, path] = q.front();
            q.pop();
            
            // 如果是叶子节点
            if (node->left == nullptr && node->right == nullptr) {
                result.push_back(path);
                continue;
            }
            
            // 添加子节点到队列
            if (node->left != nullptr) {
                q.push({node->left, path + "->" + std::to_string(node->left->val)});
            }
            if (node->right != nullptr) {
                q.push({node->right, path + "->" + std::to_string(node->right->val)});
            }
        }
        
        return result;
    }
};

// ========================================
// 测试代码
// ========================================

void testLC257() {
    std::cout << "=== LeetCode 257: 二叉树的所有路径 ===\n\n";
    
    Solution solution;
    
    // 测试用例1
    //     1
    //    / \
    //   2   3
    //    \
    //     5
    TreeNode* root1 = new TreeNode(1);
    root1->left = new TreeNode(2);
    root1->right = new TreeNode(3);
    root1->left->right = new TreeNode(5);
    
    std::cout << "测试用例1:\n";
    std::cout << "     1\n";
    std::cout << "    / \\\n";
    std::cout << "   2   3\n";
    std::cout << "    \\\n";
    std::cout << "     5\n\n";
    
    auto result1 = solution.binaryTreePaths(root1);
    std::cout << "所有路径: [";
    for (size_t i = 0; i < result1.size(); ++i) {
        std::cout << "\"" << result1[i] << "\"";
        if (i < result1.size() - 1) std::cout << ", ";
    }
    std::cout << "]\n";
    std::cout << "预期: [\"1->2->5\", \"1->3\"]\n\n";
    
    // 测试用例2：单节点
    TreeNode* root2 = new TreeNode(1);
    std::cout << "测试用例2: 单节点树\n";
    auto result2 = solution.binaryTreePaths(root2);
    std::cout << "所有路径: [";
    for (size_t i = 0; i < result2.size(); ++i) {
        std::cout << "\"" << result2[i] << "\"";
        if (i < result2.size() - 1) std::cout << ", ";
    }
    std::cout << "]\n";
    std::cout << "预期: [\"1\"]\n\n";
    
    // 清理内存
    delete root1->left->right;
    delete root1->left;
    delete root1->right;
    delete root1;
    delete root2;
    
    std::cout << "复杂度分析:\n";
    std::cout << "- 时间复杂度: O(n)，每个节点访问一次\n";
    std::cout << "- 空间复杂度: O(h)，递归栈深度\n";
}
