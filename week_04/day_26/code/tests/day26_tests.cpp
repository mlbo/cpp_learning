#include "data_structure/memory_alignment_demo.h"
#include "leetcode/0005_longest_palindrome/solution.h"
#include "leetcode/0647_palindromic_substrings/solution.h"

#include <cstdint>
#include <iostream>
#include <string>

namespace {

int failures = 0;

void expect(bool condition, const char* caseName) {
    if (condition) {
        return;
    }
    ++failures;
    std::cerr << "[FAIL] " << caseName << '\n';
}

bool isPalindrome(const std::string& text) {
    for (std::size_t left = 0, right = text.size(); left < right; ++left) {
        --right;
        if (text[left] != text[right]) {
            return false;
        }
    }
    return true;
}

void testLongestPalindromeAlgorithm() {
    Solution solution;
    const auto result = solution.longestPalindrome("babad");
    expect(result.size() == 3 && isPalindrome(result), "LC5 accepts bab or aba");
    expect(solution.longestPalindrome("cbbd") == "bb", "LC5 even-length center");
    expect(solution.longestPalindrome("").empty(), "LC5 empty input");
    expect(solution.longestPalindrome("aaaa") == "aaaa", "LC5 full palindrome");
}

void testPalindromeCount() {
    Solution647 solution;
    expect(solution.countSubstrings("") == 0, "LC647 empty input");
    expect(solution.countSubstrings("abc") == 3, "LC647 single-character palindromes");
    expect(solution.countSubstrings("aaa") == 6, "LC647 overlapping palindromes");
    expect(solution.countSubstrings("abba") == 6, "LC647 even palindrome");
}

void testAlignmentContracts() {
    alignas(32) double value = 0.0;
    expect(reinterpret_cast<std::uintptr_t>(&value) % 32 == 0, "alignas(32) contract");
    expect(sizeof(DefaultAligned) % alignof(DefaultAligned) == 0, "struct size respects alignment");
    demoSafeByteAccess();
}

} // namespace

int main() {
    testLongestPalindromeAlgorithm();
    testPalindromeCount();
    testAlignmentContracts();
    if (failures != 0) {
        std::cerr << failures << " Day 26 test(s) failed\n";
        return 1;
    }
    std::cout << "Day 26 tests passed\n";
    return 0;
}
