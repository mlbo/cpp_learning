#include <iostream>
#include <memory>
#include <stdexcept>

#include "solution.h"

int main() {
    using day35::max_path::MaxPathSum;
    using day35::max_path::TreeNode;

    auto root = std::make_unique<TreeNode>(-10);
    root->add_left(9);
    TreeNode* twenty = root->add_right(20);
    twenty->add_left(15);
    twenty->add_right(7);

    MaxPathSum solve;
    const std::int64_t classic = solve(root.get());

    auto negative = std::make_unique<TreeNode>(-3);
    negative->add_left(-8);
    const std::int64_t all_negative = solve(negative.get());

    bool rejects_empty = false;
    try {
        static_cast<void>(solve(nullptr));
    } catch (const std::invalid_argument&) {
        rejects_empty = true;
    }

    std::cout << "最大路径和: " << classic << "（15 + 20 + 7）\n";
    return classic == 42 && all_negative == -3 && rejects_empty ? 0 : 1;
}
