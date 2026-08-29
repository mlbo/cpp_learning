#include "solution.h"

#include <iostream>
#include <memory>

int main() {
    int failures = 0;
    const auto expect = [&failures](bool condition, const char* message) {
        if (!condition) {
            std::cerr << "FAIL: " << message << '\n';
            ++failures;
        }
    };
    const lc0111::Solution solution;

    expect(solution.min_depth(nullptr) == 0, "空树最小深度应为 0");
    auto one_sided = std::make_unique<lc0111::TreeNode>(1);
    one_sided->right = std::make_unique<lc0111::TreeNode>(2);
    one_sided->right->right = std::make_unique<lc0111::TreeNode>(3);
    expect(solution.min_depth(one_sided.get()) == 3,
           "缺失孩子不是叶子，不能把空分支深度错误计为 0");

    auto branched = std::make_unique<lc0111::TreeNode>(1);
    branched->left = std::make_unique<lc0111::TreeNode>(2);
    branched->right = std::make_unique<lc0111::TreeNode>(3);
    branched->left->left = std::make_unique<lc0111::TreeNode>(4);
    expect(solution.min_depth(branched.get()) == 2, "应选择最近的真实叶子");

    return failures == 0 ? 0 : 1;
}
