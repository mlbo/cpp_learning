#include <cstdint>
#include <iostream>
#include <limits>
#include <memory>

struct TreeNode {
    explicit TreeNode(int value_in) : value(value_in) {}

    int value;
    std::unique_ptr<TreeNode> left;
    std::unique_ptr<TreeNode> right;
};

class Solution {
public:
    [[nodiscard]] bool is_valid_bst(const TreeNode* root) const {
        return validate(root,
                        std::numeric_limits<std::int64_t>::lowest(),
                        std::numeric_limits<std::int64_t>::max());
    }

private:
    [[nodiscard]] static bool validate(const TreeNode* node,
                                       std::int64_t lower,
                                       std::int64_t upper) {
        if (node == nullptr) {
            return true;
        }
        const std::int64_t value = node->value;
        if (value <= lower || value >= upper) {
            return false;
        }
        return validate(node->left.get(), lower, value) &&
               validate(node->right.get(), value, upper);
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
    const Solution solution;

    auto valid = std::make_unique<TreeNode>(2);
    valid->left = std::make_unique<TreeNode>(1);
    valid->right = std::make_unique<TreeNode>(3);
    expect(solution.is_valid_bst(valid.get()), "合法 BST 应通过验证");

    auto globally_invalid = std::make_unique<TreeNode>(10);
    globally_invalid->left = std::make_unique<TreeNode>(5);
    globally_invalid->right = std::make_unique<TreeNode>(15);
    globally_invalid->left->right = std::make_unique<TreeNode>(12);
    expect(!solution.is_valid_bst(globally_invalid.get()),
           "只满足父子大小关系但越过祖先上界的树必须失败");

    auto duplicate = std::make_unique<TreeNode>(2);
    duplicate->left = std::make_unique<TreeNode>(2);
    expect(!solution.is_valid_bst(duplicate.get()), "严格 BST 契约不允许重复值");

    auto limits = std::make_unique<TreeNode>(0);
    limits->left = std::make_unique<TreeNode>(std::numeric_limits<int>::min());
    limits->right = std::make_unique<TreeNode>(std::numeric_limits<int>::max());
    expect(solution.is_valid_bst(limits.get()), "int 极值不应与哨兵冲突");
    expect(solution.is_valid_bst(nullptr), "空树应是合法 BST");

    std::cout << "LC 98 全局开区间、重复值和整数边界测试完成。\n";
    return failures == 0 ? 0 : 1;
}
