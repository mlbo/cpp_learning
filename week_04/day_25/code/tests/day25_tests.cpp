#include "cpp11_features/perfect_forwarding_demo.h"
#include "leetcode/0003_longest_substring/solution.h"
#include "leetcode/0438_find_anagrams/solution.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

bool verify_item29_move_assumptions_contract();
bool verify_item30_forwarding_boundaries_contract();
bool verify_std_forward_demo_contract();

namespace {

int failures = 0;

template<typename Actual, typename Expected>
void expectEqual(const Actual& actual, const Expected& expected, const char* caseName) {
    if (actual == expected) {
        return;
    }
    ++failures;
    std::cerr << "[FAIL] " << caseName << '\n';
}

template<typename Callable>
void expectInvalidArgument(Callable action, const char* caseName) {
    try {
        action();
    } catch (const std::invalid_argument&) {
        return;
    } catch (...) {
        ++failures;
        std::cerr << "[FAIL] " << caseName << " (异常类型错误)\n";
        return;
    }
    ++failures;
    std::cerr << "[FAIL] " << caseName << " (未抛出异常)\n";
}

void testForwarding() {
    std::string text = "cache-value";
    const std::string constText = "read-only";
    expectEqual(cpp11_features::forwardingRoute(text),
                cpp11_features::ForwardingRoute::Lvalue,
                "real forwarding module preserves lvalue");
    expectEqual(cpp11_features::forwardingRoute(constText),
                cpp11_features::ForwardingRoute::Lvalue,
                "real forwarding module preserves const lvalue");
    expectEqual(cpp11_features::forwardingRoute(std::string{"temporary"}),
                cpp11_features::ForwardingRoute::Rvalue,
                "real forwarding module preserves rvalue");
}

void testEmcppContracts() {
    expectEqual(verify_item29_move_assumptions_contract(), true,
                "Item29 uses observed copy/move and real forwarding routes");
    expectEqual(verify_item30_forwarding_boundaries_contract(), true,
                "Item30 success repairs reach real target overloads");
    expectEqual(verify_std_forward_demo_contract(), true,
                "std::move type/moved-from reuse and DeferredTask ownership contracts");
}

void testLongestSubstring() {
    leetcode::lc0003::Solution solution;
    const std::vector<std::pair<std::string, int>> cases{
        {"", 0},
        {"abcabcbb", 3},
        {"bbbbb", 1},
        {"pwwkew", 3},
        {"abba", 2},
        {std::string{"\xFF" "a" "\xFF", 3}, 2},
    };

    for (const auto& [input, expected] : cases) {
        expectEqual(solution.lengthOfLongestSubstring(input), expected, "LC3 hash-table version");
        expectEqual(solution.lengthOfLongestSubstringArray(input), expected, "LC3 byte-array version");
    }
}

void testFindAnagrams() {
    leetcode::lc0438::Solution solution;
    const std::vector<std::pair<std::pair<std::string, std::string>, std::vector<int>>> cases{
        {{"cbaebabacd", "abc"}, {0, 6}},
        {{"abab", "ab"}, {0, 1, 2}},
        {{"abcdefg", "xyz"}, {}},
        {{"a", "ab"}, {}},
        {{"abc", ""}, {}},
    };

    for (const auto& [input, expected] : cases) {
        expectEqual(solution.findAnagrams(input.first, input.second), expected, "LC438 basic version");
        expectEqual(solution.findAnagramsOptimized(input.first, input.second), expected, "LC438 optimized version");
    }

    expectInvalidArgument(
        [&solution] { static_cast<void>(solution.findAnagrams("aB", "ab")); },
        "LC438 basic rejects uppercase source byte");
    expectInvalidArgument(
        [&solution] { static_cast<void>(solution.findAnagramsOptimized("ab", "aB")); },
        "LC438 optimized rejects uppercase pattern byte");
    expectInvalidArgument(
        [&solution] { static_cast<void>(solution.findAnagrams("A", "abc")); },
        "LC438 validates before short-input return");
    expectInvalidArgument(
        [&solution] { static_cast<void>(solution.findAnagramsOptimized("A", "abc")); },
        "LC438 optimized validates before short-input return");
}

} // namespace

int main() {
    testForwarding();
    testEmcppContracts();
    testLongestSubstring();
    testFindAnagrams();

    if (failures != 0) {
        std::cerr << failures << " Day 25 test(s) failed\n";
        return 1;
    }
    std::cout << "Day 25 tests passed\n";
    return 0;
}
