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
    const lc0104::Solution solution;

    expect(solution.max_depth(nullptr) == 0, "空树深度应为 0");
    auto single = std::make_unique<lc0104::TreeNode>(1);
    expect(solution.max_depth(single.get()) == 1, "单节点树深度应为 1");
    single->right = std::make_unique<lc0104::TreeNode>(2);
    single->right->right = std::make_unique<lc0104::TreeNode>(3);
    expect(solution.max_depth(single.get()) == 3, "退化树深度应等于节点数");

    return failures == 0 ? 0 : 1;
}
