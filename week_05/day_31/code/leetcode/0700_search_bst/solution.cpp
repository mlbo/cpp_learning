#include <iostream>
#include <memory>

struct TreeNode {
    explicit TreeNode(int value_in) : value(value_in) {}

    int value;
    std::unique_ptr<TreeNode> left;
    std::unique_ptr<TreeNode> right;
};

class Solution {
public:
    [[nodiscard]] const TreeNode* search_recursive(const TreeNode* root, int target) const {
        if (root == nullptr || root->value == target) {
            return root;
        }
        return target < root->value ? search_recursive(root->left.get(), target)
                                    : search_recursive(root->right.get(), target);
    }

    [[nodiscard]] const TreeNode* search_iterative(const TreeNode* root, int target) const {
        const TreeNode* current = root;
        while (current != nullptr && current->value != target) {
            current = target < current->value ? current->left.get() : current->right.get();
        }
        return current;
    }
};

int main() {
    int failures = 0;
    const auto expect = [&failures](bool condition, const char* message) {
        if (!condition) {
            std::cerr << "FAIL: " << message << '\n';
            ++failures;
        }
    };

    auto root = std::make_unique<TreeNode>(4);
    root->left = std::make_unique<TreeNode>(2);
    root->right = std::make_unique<TreeNode>(7);
    root->left->left = std::make_unique<TreeNode>(1);
    root->left->right = std::make_unique<TreeNode>(3);

    const Solution solution;
    const TreeNode* recursive = solution.search_recursive(root.get(), 2);
    const TreeNode* iterative = solution.search_iterative(root.get(), 2);
    expect(recursive != nullptr && recursive->value == 2, "递归搜索应找到目标节点");
    expect(iterative == recursive, "递归和迭代搜索应返回同一子树根");
    expect(solution.search_iterative(root.get(), 5) == nullptr, "不存在的目标应返回 nullptr");
    expect(solution.search_recursive(nullptr, 1) == nullptr, "空树搜索应返回 nullptr");

    std::cout << "LC 700 递归与迭代搜索测试完成。\n";
    return failures == 0 ? 0 : 1;
}
