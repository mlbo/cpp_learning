#include <algorithm>
#include <iostream>
#include <memory>
#include <queue>
#include <stack>
#include <utility>
#include <vector>

struct TreeNode {
    explicit TreeNode(int value) : val(value) {}

    int val;
    std::unique_ptr<TreeNode> left;
    std::unique_ptr<TreeNode> right;
};

std::vector<int> preorderIterative(const TreeNode* root) {
    std::vector<int> result;
    if (root == nullptr) {
        return result;
    }

    std::stack<const TreeNode*> pending;
    pending.push(root);
    while (!pending.empty()) {
        const TreeNode* node = pending.top();
        pending.pop();
        result.push_back(node->val);
        if (node->right != nullptr) {
            pending.push(node->right.get());
        }
        if (node->left != nullptr) {
            pending.push(node->left.get());
        }
    }
    return result;
}

std::vector<int> inorderIterative(const TreeNode* root) {
    std::vector<int> result;
    std::stack<const TreeNode*> ancestors;
    const TreeNode* current = root;
    while (current != nullptr || !ancestors.empty()) {
        while (current != nullptr) {
            ancestors.push(current);
            current = current->left.get();
        }
        current = ancestors.top();
        ancestors.pop();
        result.push_back(current->val);
        current = current->right.get();
    }
    return result;
}

std::vector<int> postorderIterative(const TreeNode* root) {
    std::vector<int> result;
    if (root == nullptr) {
        return result;
    }

    std::stack<const TreeNode*> pending;
    pending.push(root);
    while (!pending.empty()) {
        const TreeNode* node = pending.top();
        pending.pop();
        result.push_back(node->val);
        if (node->left != nullptr) {
            pending.push(node->left.get());
        }
        if (node->right != nullptr) {
            pending.push(node->right.get());
        }
    }
    std::reverse(result.begin(), result.end());
    return result;
}

std::vector<std::vector<int>> levelOrder(const TreeNode* root) {
    std::vector<std::vector<int>> result;
    if (root == nullptr) {
        return result;
    }

    std::queue<const TreeNode*> pending;
    pending.push(root);
    while (!pending.empty()) {
        const std::size_t levelSize = pending.size();
        std::vector<int> level;
        level.reserve(levelSize);
        for (std::size_t i = 0; i < levelSize; ++i) {
            const TreeNode* node = pending.front();
            pending.pop();
            level.push_back(node->val);
            if (node->left != nullptr) {
                pending.push(node->left.get());
            }
            if (node->right != nullptr) {
                pending.push(node->right.get());
            }
        }
        result.push_back(std::move(level));
    }
    return result;
}

int main() {
    auto root = std::make_unique<TreeNode>(1);
    root->left = std::make_unique<TreeNode>(2);
    root->right = std::make_unique<TreeNode>(3);
    root->left->left = std::make_unique<TreeNode>(4);
    root->left->right = std::make_unique<TreeNode>(5);

    const bool passed =
        preorderIterative(root.get()) == std::vector<int>({1, 2, 4, 5, 3}) &&
        inorderIterative(root.get()) == std::vector<int>({4, 2, 5, 1, 3}) &&
        postorderIterative(root.get()) == std::vector<int>({4, 5, 2, 3, 1}) &&
        levelOrder(root.get()) == std::vector<std::vector<int>>({{1}, {2, 3}, {4, 5}}) &&
        preorderIterative(nullptr).empty();

    std::cout << (passed ? "tree traversal checks passed\n"
                         : "tree traversal checks failed\n");
    return passed ? 0 : 1;
}
