#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

#include "joining_thread_group.h"

struct TreeNode {
    explicit TreeNode(int value) : val(value) {}

    int val;
    std::unique_ptr<TreeNode> left;
    std::unique_ptr<TreeNode> right;
};

void inorder(const TreeNode* root, std::vector<int>& result) {
    if (root == nullptr) {
        return;
    }
    inorder(root->left.get(), result);
    result.push_back(root->val);
    inorder(root->right.get(), result);
}

std::vector<std::vector<int>> levelOrder(const TreeNode* root) {
    std::vector<std::vector<int>> levels;
    if (root == nullptr) {
        return levels;
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
        levels.push_back(std::move(level));
    }
    return levels;
}

int main() {
    auto root = std::make_unique<TreeNode>(1);
    root->left = std::make_unique<TreeNode>(2);
    root->right = std::make_unique<TreeNode>(3);

    std::vector<int> in;
    inorder(root.get(), in);
    const auto levels = levelOrder(root.get());

    std::mutex mutex;
    int counter = 0;
    auto increment = [&mutex, &counter] {
        for (int i = 0; i < 1'000; ++i) {
            const std::lock_guard<std::mutex> lock(mutex);
            ++counter;
        }
    };
    week5::JoiningThreadGroup threads;
    threads.start(increment);
    threads.start(increment);
    threads.join_all();

    const bool passed = in == std::vector<int>({2, 1, 3}) &&
                        levels == std::vector<std::vector<int>>({{1}, {2, 3}}) &&
                        counter == 2'000;
    std::cout << (passed ? "day30 integration passed\n" : "day30 integration failed\n");
    return passed ? 0 : 1;
}
