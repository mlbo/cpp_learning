/**
 * LeetCode 257：二叉树的所有路径。
 * 接口借用 root，不保存节点指针；返回值拥有自己的路径字符串。
 */

#include <algorithm>
#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <utility>
#include <vector>

struct TreeNode {
    explicit TreeNode(int node_value) : val(node_value) {}

    int val;
    TreeNode* left{nullptr};
    TreeNode* right{nullptr};
};

class Solution {
public:
    std::vector<std::string> binaryTreePaths(TreeNode* root) const {
        std::vector<std::string> result;
        if (root != nullptr) {
            dfs(root, std::string{}, result);
        }
        return result;
    }

private:
    static void dfs(const TreeNode* node,
                    std::string path,
                    std::vector<std::string>& result) {
        if (!path.empty()) {
            path += "->";
        }
        path += std::to_string(node->val);

        if (node->left == nullptr && node->right == nullptr) {
            result.push_back(std::move(path));
            return;
        }
        if (node->left != nullptr) {
            dfs(node->left, path, result);
        }
        if (node->right != nullptr) {
            dfs(node->right, std::move(path), result);
        }
    }
};

class Solution2 {
public:
    std::vector<std::string> binaryTreePaths(TreeNode* root) const {
        std::vector<std::string> result;
        std::vector<int> path;
        dfs(root, path, result);
        return result;
    }

private:
    static void dfs(const TreeNode* node,
                    std::vector<int>& path,
                    std::vector<std::string>& result) {
        if (node == nullptr) {
            return;
        }

        path.push_back(node->val);
        if (node->left == nullptr && node->right == nullptr) {
            result.push_back(buildPathString(path));
        } else {
            dfs(node->left, path, result);
            dfs(node->right, path, result);
        }
        path.pop_back();
    }

    static std::string buildPathString(const std::vector<int>& path) {
        std::string result;
        for (std::size_t index = 0; index < path.size(); ++index) {
            if (index != 0U) {
                result += "->";
            }
            result += std::to_string(path[index]);
        }
        return result;
    }
};

class Solution3 {
public:
    std::vector<std::string> binaryTreePaths(TreeNode* root) const {
        std::vector<std::string> result;
        if (root == nullptr) {
            return result;
        }

        std::queue<std::pair<const TreeNode*, std::string>> pending;
        pending.emplace(root, std::to_string(root->val));
        while (!pending.empty()) {
            auto [node, path] = std::move(pending.front());
            pending.pop();

            if (node->left == nullptr && node->right == nullptr) {
                result.push_back(std::move(path));
                continue;
            }
            if (node->left != nullptr) {
                pending.emplace(node->left,
                                path + "->" + std::to_string(node->left->val));
            }
            if (node->right != nullptr) {
                pending.emplace(node->right,
                                path + "->" + std::to_string(node->right->val));
            }
        }
        return result;
    }
};

class TreeArena final {
public:
    TreeNode* make(int value) {
        nodes_.push_back(std::make_unique<TreeNode>(value));
        return nodes_.back().get();
    }

private:
    std::vector<std::unique_ptr<TreeNode>> nodes_;
};

std::vector<std::string> sortedPaths(std::vector<std::string> paths) {
    std::sort(paths.begin(), paths.end());
    return paths;
}

int main() {
    TreeArena arena;
    TreeNode* root = arena.make(1);
    root->left = arena.make(2);
    root->right = arena.make(3);
    root->left->right = arena.make(5);

    const std::vector<std::string> expected{"1->2->5", "1->3"};
    const Solution recursive_string;
    const Solution2 backtracking;
    const Solution3 breadth_first;
    if (sortedPaths(recursive_string.binaryTreePaths(root)) != expected ||
        sortedPaths(backtracking.binaryTreePaths(root)) != expected ||
        sortedPaths(breadth_first.binaryTreePaths(root)) != expected) {
        std::cerr << "LC 257 基本用例失败\n";
        return 1;
    }

    if (!backtracking.binaryTreePaths(nullptr).empty()) {
        std::cerr << "LC 257 空树用例失败\n";
        return 1;
    }

    TreeNode* single = arena.make(-7);
    const std::vector<std::string> expected_single{"-7"};
    if (breadth_first.binaryTreePaths(single) != expected_single) {
        std::cerr << "LC 257 单节点负值用例失败\n";
        return 1;
    }

    std::cout << "LC 257 测试通过：递归字符串、回溯与 BFS 三种实现一致\n";
    return 0;
}
