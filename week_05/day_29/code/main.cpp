#include <future>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

struct TreeNode {
    explicit TreeNode(int value) : val(value) {}

    int val;
    std::unique_ptr<TreeNode> left;
    std::unique_ptr<TreeNode> right;
};

void preorder(const TreeNode* root, std::vector<int>& result) {
    if (root == nullptr) {
        return;
    }
    result.push_back(root->val);
    preorder(root->left.get(), result);
    preorder(root->right.get(), result);
}

int main() {
    auto root = std::make_unique<TreeNode>(1);
    root->left = std::make_unique<TreeNode>(2);
    root->right = std::make_unique<TreeNode>(3);

    std::vector<int> traversal;
    preorder(root.get(), traversal);

    int threadResult = 0;
    std::thread worker([&threadResult] { threadResult = 6 * 6; });
    worker.join();

    auto task = std::async(std::launch::async, [] { return 7 * 7; });
    const int taskResult = task.get();

    const bool passed = traversal == std::vector<int>({1, 2, 3}) &&
                        threadResult == 36 && taskResult == 49;
    std::cout << "Day 29 integrates owned trees, joined threads, and observed futures.\n";
    std::cout << (passed ? "day29 integration passed\n" : "day29 integration failed\n");
    return passed ? 0 : 1;
}
