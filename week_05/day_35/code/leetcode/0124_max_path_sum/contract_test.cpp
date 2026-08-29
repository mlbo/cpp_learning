#include <cstdint>
#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>

#include "solution.h"

int main() {
    using day35::max_path::MaxPathSum;
    using day35::max_path::TreeNode;

    const MaxPathSum solve;
    auto maximum = std::make_unique<TreeNode>(std::numeric_limits<std::int64_t>::max());
    const bool maximum_single_node =
        solve(maximum.get()) == std::numeric_limits<std::int64_t>::max();
    maximum->add_left(1);

    bool overflow_rejected = false;
    try {
        static_cast<void>(solve(maximum.get()));
    } catch (const std::overflow_error&) {
        overflow_rejected = true;
    }

    if (!maximum_single_node || !overflow_rejected) {
        std::cerr << "maximum path int64_t overflow contract test failed\n";
        return 1;
    }

    std::cout << "maximum path sum reports int64_t overflow instead of executing UB\n";
    return 0;
}
