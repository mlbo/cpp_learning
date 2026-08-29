#include <iostream>
#include <stdexcept>
#include <vector>

#include "solution.h"

template <typename Function>
bool throws_invalid_argument(Function&& function) {
    try {
        function();
    } catch (const std::invalid_argument&) {
        return true;
    }
    return false;
}

int main() {
    day34::construct_tree::TreeBuilder builder;
    const auto empty = builder.build({}, {});
    const bool rejects_size_mismatch = throws_invalid_argument([&builder] {
        static_cast<void>(builder.build({1}, {}));
    });
    const bool rejects_duplicates = throws_invalid_argument([&builder] {
        static_cast<void>(builder.build({1, 1}, {1, 1}));
    });
    const bool rejects_inconsistent = throws_invalid_argument([&builder] {
        static_cast<void>(builder.build({1, 2, 3}, {3, 1, 2}));
    });

    if (empty != nullptr || !rejects_size_mismatch || !rejects_duplicates ||
        !rejects_inconsistent) {
        std::cerr << "tree reconstruction contract test failed\n";
        return 1;
    }
    return 0;
}
