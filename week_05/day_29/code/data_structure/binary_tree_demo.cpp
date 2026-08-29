#include <algorithm>
#include <cstddef>
#include <iostream>
#include <memory>
#include <queue>
#include <utility>
#include <vector>

struct TreeNode {
    explicit TreeNode(int value) : val(value) {}

    int val;
    std::unique_ptr<TreeNode> left;
    std::unique_ptr<TreeNode> right;
};

class BinaryTree {
public:
    static std::vector<int> preorderTraversal(const TreeNode* root) {
        std::vector<int> result;
        preorderHelper(root, result);
        return result;
    }

    static std::vector<int> inorderTraversal(const TreeNode* root) {
        std::vector<int> result;
        inorderHelper(root, result);
        return result;
    }

    static std::vector<int> postorderTraversal(const TreeNode* root) {
        std::vector<int> result;
        postorderHelper(root, result);
        return result;
    }

    static std::vector<std::vector<int>> levelOrder(const TreeNode* root) {
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

    static int getHeight(const TreeNode* root) {
        if (root == nullptr) {
            return 0;
        }
        return 1 + std::max(getHeight(root->left.get()), getHeight(root->right.get()));
    }

    static int countNodes(const TreeNode* root) {
        if (root == nullptr) {
            return 0;
        }
        return 1 + countNodes(root->left.get()) + countNodes(root->right.get());
    }

    static bool isCompleteTree(const TreeNode* root) {
        if (root == nullptr) {
            return true;
        }

        std::queue<const TreeNode*> pending;
        pending.push(root);
        bool seenMissingChild = false;
        while (!pending.empty()) {
            const TreeNode* node = pending.front();
            pending.pop();
            if (node == nullptr) {
                seenMissingChild = true;
                continue;
            }
            if (seenMissingChild) {
                return false;
            }
            pending.push(node->left.get());
            pending.push(node->right.get());
        }
        return true;
    }

private:
    static void preorderHelper(const TreeNode* root, std::vector<int>& result) {
        if (root == nullptr) {
            return;
        }
        result.push_back(root->val);
        preorderHelper(root->left.get(), result);
        preorderHelper(root->right.get(), result);
    }

    static void inorderHelper(const TreeNode* root, std::vector<int>& result) {
        if (root == nullptr) {
            return;
        }
        inorderHelper(root->left.get(), result);
        result.push_back(root->val);
        inorderHelper(root->right.get(), result);
    }

    static void postorderHelper(const TreeNode* root, std::vector<int>& result) {
        if (root == nullptr) {
            return;
        }
        postorderHelper(root->left.get(), result);
        postorderHelper(root->right.get(), result);
        result.push_back(root->val);
    }
};

int main() {
    auto root = std::make_unique<TreeNode>(1);
    root->left = std::make_unique<TreeNode>(2);
    root->right = std::make_unique<TreeNode>(3);
    root->left->left = std::make_unique<TreeNode>(4);
    root->left->right = std::make_unique<TreeNode>(5);

    const auto preorder = BinaryTree::preorderTraversal(root.get());
    const auto inorder = BinaryTree::inorderTraversal(root.get());
    const auto postorder = BinaryTree::postorderTraversal(root.get());
    const auto levels = BinaryTree::levelOrder(root.get());

    const bool passed =
        preorder == std::vector<int>({1, 2, 4, 5, 3}) &&
        inorder == std::vector<int>({4, 2, 5, 1, 3}) &&
        postorder == std::vector<int>({4, 5, 2, 3, 1}) &&
        levels == std::vector<std::vector<int>>({{1}, {2, 3}, {4, 5}}) &&
        BinaryTree::getHeight(root.get()) == 3 &&
        BinaryTree::countNodes(root.get()) == 5 &&
        BinaryTree::isCompleteTree(root.get());

    std::cout << "二叉树所有权：根节点独占整棵树，离开作用域时递归释放。\n";
    std::cout << (passed ? "binary tree checks passed\n" : "binary tree checks failed\n");
    return passed ? 0 : 1;
}
