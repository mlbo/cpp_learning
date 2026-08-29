#include <array>
#include <iostream>
#include <string_view>

namespace {

template <std::size_t Count>
bool valid_manifest(const std::array<std::string_view, Count>& modules,
                    std::string_view prefix) {
    for (std::size_t index = 0; index < modules.size(); ++index) {
        if (modules[index].empty() || modules[index].find(prefix) != 0U) {
            return false;
        }
        for (std::size_t other = index + 1U; other < modules.size(); ++other) {
            if (modules[index] == modules[other]) {
                return false;
            }
        }
    }
    return true;
}

}  // namespace

int main() {
    constexpr std::array<std::string_view, 5> modules{
        "day31_bst_demo",
        "day31_condition_variable",
        "day31_item38",
        "day31_lc0098",
        "day31_lc0700",
    };

    std::cout << "Day 31 工程动作：由 CTest 验证以下模块：\n";
    for (const std::string_view module : modules) {
        std::cout << "  - " << module << '\n';
    }

    const bool contains_contract_targets = modules[0] == "day31_bst_demo" &&
                                           modules[1] == "day31_condition_variable" &&
                                           modules[2] == "day31_item38";
    return valid_manifest(modules, "day31_") && contains_contract_targets ? 0 : 1;
}
