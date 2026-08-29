/**
 * LeetCode 113：路径总和 II。
 * 只有根到叶子路径才算答案；返回结果拥有路径副本。
 */

#include <cstdint>
#include <iostream>
#include <memory>
#include <stack>
#include <tuple>
#include <utility>
#include <vector>

struct TreeNode {
    explicit TreeNode(int node_value) : val(node_value) {}

    int val;
    TreeNode* left{nullptr};
    TreeNode* right{nullptr};
};

using Path = std::vector<int>;
using Paths = std::vector<Path>;

class Solution {
public:
    Paths pathSum(TreeNode* root, int target_sum) const {
        Paths result;
        Path current;
        dfs(root, static_cast<std::int64_t>(target_sum), current, result);
        return result;
    }

private:
    static void dfs(const TreeNode* node,
                    std::int64_t remaining,
                    Path& current,
                    Paths& result) {
        if (node == nullptr) {
            return;
        }

        current.push_back(node->val);
        const std::int64_t next_remaining = remaining - node->val;
        if (node->left == nullptr && node->right == nullptr) {
            if (next_remaining == 0) {
                result.push_back(current);
            }
        } else {
            dfs(node->left, next_remaining, current, result);
            dfs(node->right, next_remaining, current, result);
        }
        current.pop_back();
    }
};

class Solution2 {
public:
    Paths pathSum(TreeNode* root, int target_sum) const {
        Paths result;
        findPaths(root, static_cast<std::int64_t>(target_sum), {}, result);
        return result;
    }

private:
    static void findPaths(const TreeNode* node,
                          std::int64_t remaining,
                          Path path,
                          Paths& result) {
        if (node == nullptr) {
            return;
        }

        path.push_back(node->val);
        const std::int64_t next_remaining = remaining - node->val;
        if (node->left == nullptr && node->right == nullptr) {
            if (next_remaining == 0) {
                result.push_back(std::move(path));
            }
            return;
        }
        findPaths(node->left, next_remaining, path, result);
        findPaths(node->right, next_remaining, std::move(path), result);
    }
};

class Solution3 {
public:
    Paths pathSum(TreeNode* root, int target_sum) const {
        Paths result;
        if (root == nullptr) {
            return result;
        }

        using State = std::tuple<const TreeNode*, std::int64_t, Path>;
        std::stack<State> pending;
        pending.emplace(root, static_cast<std::int64_t>(root->val), Path{root->val});
        while (!pending.empty()) {
            auto [node, sum, path] = std::move(pending.top());
            pending.pop();

            if (node->left == nullptr && node->right == nullptr) {
                if (sum == target_sum) {
                    result.push_back(std::move(path));
                }
                continue;
            }
            if (node->right != nullptr) {
                Path right_path = path;
                right_path.push_back(node->right->val);
                pending.emplace(node->right, sum + node->right->val, std::move(right_path));
            }
            if (node->left != nullptr) {
                path.push_back(node->left->val);
                pending.emplace(node->left, sum + node->left->val, std::move(path));
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

TreeNode* makeExampleTree(TreeArena& arena) {
    TreeNode* root = arena.make(5);
    root->left = arena.make(4);
    root->right = arena.make(8);
    root->left->left = arena.make(11);
    root->left->left->left = arena.make(7);
    root->left->left->right = arena.make(2);
    root->right->left = arena.make(13);
    root->right->right = arena.make(4);
    root->right->right->left = arena.make(5);
    root->right->right->right = arena.make(1);
    return root;
}

int main() {
    TreeArena arena;
    TreeNode* root = makeExampleTree(arena);
    const Paths expected{{5, 4, 11, 2}, {5, 8, 4, 5}};

    const Solution backtracking;
    const Solution2 value_semantics;
    const Solution3 iterative;
    if (backtracking.pathSum(root, 22) != expected ||
        value_semantics.pathSum(root, 22) != expected ||
        iterative.pathSum(root, 22) != expected) {
        std::cerr << "LC 113 基本用例失败\n";
        return 1;
    }

    if (!backtracking.pathSum(nullptr, 0).empty()) {
        std::cerr << "LC 113 空树用例失败\n";
        return 1;
    }

    TreeNode* negative_root = arena.make(-2);
    negative_root->right = arena.make(-3);
    const Paths expected_negative{{-2, -3}};
    if (iterative.pathSum(negative_root, -5) != expected_negative ||
        !value_semantics.pathSum(negative_root, -2).empty()) {
        std::cerr << "LC 113 负值或必须到叶子的边界用例失败\n";
        return 1;
    }

    std::cout << "LC 113 测试通过：回溯、值语义与迭代三种实现一致\n";
    return 0;
}
