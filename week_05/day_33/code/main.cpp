/**
 * Day 33 综合烟雾测试：借用树节点，复制结果路径，并验证回溯状态恢复。
 */

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

void collectRootToLeafPaths(const TreeNode* node, Path& current, Paths& result) {
    if (node == nullptr) {
        return;
    }

    current.push_back(node->value);
    if (node->left == nullptr && node->right == nullptr) {
        result.push_back(current);
    } else {
        collectRootToLeafPaths(node->left.get(), current, result);
        collectRootToLeafPaths(node->right.get(), current, result);
    }
    current.pop_back();
}

int main() {
    auto root = std::make_unique<TreeNode>(1);
    root->left = std::make_unique<TreeNode>(2);
    root->right = std::make_unique<TreeNode>(3);
    root->left->right = std::make_unique<TreeNode>(5);

    Path scratch;
    Paths actual;
    collectRootToLeafPaths(root.get(), scratch, actual);

    const Paths expected{{1, 2, 5}, {1, 3}};
    if (actual != expected || !scratch.empty()) {
        std::cerr << "Day 33 综合测试失败：路径结果错误或回溯状态未恢复\n";
        return 1;
    }

    std::cout << "Day 33 综合测试通过：得到 2 条根到叶子路径\n";
    return 0;
}
