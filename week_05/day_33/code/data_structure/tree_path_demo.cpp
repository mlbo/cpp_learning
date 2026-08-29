/**
 * 树路径演示：树拥有节点，DFS 只借用节点，结果保存路径副本。
 */

#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

struct TreeNode {
    explicit TreeNode(int node_value) : value(node_value) {}

    int value;
    std::unique_ptr<TreeNode> left;
    std::unique_ptr<TreeNode> right;
};

using Path = std::vector<int>;
using Paths = std::vector<Path>;

class PathEntry final {
public:
    PathEntry(Path& path, int value) : path_(path) {
        path_.push_back(value);
    }

    ~PathEntry() {
        path_.pop_back();
    }

    PathEntry(const PathEntry&) = delete;
    PathEntry& operator=(const PathEntry&) = delete;

private:
    Path& path_;
};

void findAllPaths(const TreeNode* node, Path& current, Paths& result) {
    if (node == nullptr) {
        return;
    }

    const PathEntry entry(current, node->value);
    if (node->left == nullptr && node->right == nullptr) {
        result.push_back(current);
        return;
    }

    findAllPaths(node->left.get(), current, result);
    findAllPaths(node->right.get(), current, result);
}

void findPathSums(const TreeNode* node,
                  std::int64_t remaining,
                  Path& current,
                  Paths& result) {
    if (node == nullptr) {
        return;
    }

    const PathEntry entry(current, node->value);
    const std::int64_t next_remaining = remaining - node->value;
    if (node->left == nullptr && node->right == nullptr) {
        if (next_remaining == 0) {
            result.push_back(current);
        }
        return;
    }

    findPathSums(node->left.get(), next_remaining, current, result);
    findPathSums(node->right.get(), next_remaining, current, result);
}

std::unique_ptr<TreeNode> makeExampleTree() {
    auto root = std::make_unique<TreeNode>(5);
    root->left = std::make_unique<TreeNode>(4);
    root->right = std::make_unique<TreeNode>(8);
    root->left->left = std::make_unique<TreeNode>(11);
    root->left->left->left = std::make_unique<TreeNode>(7);
    root->left->left->right = std::make_unique<TreeNode>(2);
    root->right->left = std::make_unique<TreeNode>(13);
    root->right->right = std::make_unique<TreeNode>(4);
    root->right->right->left = std::make_unique<TreeNode>(5);
    root->right->right->right = std::make_unique<TreeNode>(1);
    return root;
}

int main() {
    const auto root = makeExampleTree();
    Path scratch;

    Paths all_paths;
    findAllPaths(root.get(), scratch, all_paths);
    const Paths expected_all{
        {5, 4, 11, 7},
        {5, 4, 11, 2},
        {5, 8, 13},
        {5, 8, 4, 5},
        {5, 8, 4, 1},
    };
    if (all_paths != expected_all || !scratch.empty()) {
        std::cerr << "所有路径测试失败\n";
        return 1;
    }

    Paths sum_paths;
    findPathSums(root.get(), 22, scratch, sum_paths);
    const Paths expected_sum{{5, 4, 11, 2}, {5, 8, 4, 5}};
    if (sum_paths != expected_sum || !scratch.empty()) {
        std::cerr << "路径和测试失败\n";
        return 1;
    }

    Paths empty_paths;
    findAllPaths(nullptr, scratch, empty_paths);
    if (!empty_paths.empty() || !scratch.empty()) {
        std::cerr << "空树边界测试失败\n";
        return 1;
    }

    std::cout << "树路径测试通过：5 条路径，其中 2 条路径和为 22\n";
    return 0;
}
