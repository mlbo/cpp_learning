#include <array>
#include <iostream>
#include <string_view>

namespace {

template <std::size_t Count>
bool valid_manifest(const std::array<std::string_view, Count>& modules) {
    bool has_dfs = false;
    bool has_atomic = false;
    bool has_depth_contract_test = false;
    for (std::size_t index = 0; index < modules.size(); ++index) {
        const std::string_view module = modules[index];
        if (module.empty() || module.find("day32_") != 0U) {
            return false;
        }
        has_dfs = has_dfs || module == "day32_dfs_demo";
        has_atomic = has_atomic || module == "day32_atomic_demo";
        has_depth_contract_test = has_depth_contract_test ||
                                  module == "day32_lc0111_test";
        for (std::size_t other = index + 1U; other < modules.size(); ++other) {
            if (module == modules[other]) {
                return false;
            }
        }
    }
    return has_dfs && has_atomic && has_depth_contract_test;
}

}  // namespace

int main() {
    constexpr std::array<std::string_view, 7> modules{
        "day32_dfs_demo",
        "day32_atomic_demo",
        "day32_item39_40",
        "day32_lc0104_demo",
        "day32_lc0104_test",
        "day32_lc0111_demo",
        "day32_lc0111_test",
    };

    std::cout << "Day 32 工程动作：由 CTest 验证以下模块：\n";
    for (const std::string_view module : modules) {
        std::cout << "  - " << module << '\n';
    }

    return valid_manifest(modules) ? 0 : 1;
}
