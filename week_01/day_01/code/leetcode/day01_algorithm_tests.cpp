#include "0001_two_sum/solution.h"
#include "0167_two_sum_ii/solution.h"

#include <iostream>
#include <cstdint>
#include <limits>
#include <string>
#include <vector>

namespace {

int failures = 0;

void expect(bool condition, const std::string& case_name) {
    if (condition) {
        std::cout << "[PASS] " << case_name << '\n';
    } else {
        std::cerr << "[FAIL] " << case_name << '\n';
        ++failures;
    }
}

bool is_two_sum_answer(const std::vector<int>& nums,
                       int target,
                       const std::vector<int>& answer) {
    if (answer.size() != 2 || answer[0] == answer[1]) {
        return false;
    }

    const int first = answer[0];
    const int second = answer[1];
    if (first < 0 || second < 0 ||
        first >= static_cast<int>(nums.size()) ||
        second >= static_cast<int>(nums.size())) {
        return false;
    }
    return static_cast<std::int64_t>(nums[static_cast<std::size_t>(first)]) +
               nums[static_cast<std::size_t>(second)] ==
           static_cast<std::int64_t>(target);
}

void test_two_sum() {
    LC0001::Solution solution;

    {
        std::vector<int> nums{2, 7, 11, 15};
        expect(is_two_sum_answer(nums, 9, solution.twoSum(nums, 9)),
               "LeetCode 1 hash-table solution");
        expect(is_two_sum_answer(nums, 9, solution.twoSum_bruteForce(nums, 9)),
               "LeetCode 1 brute-force solution");
        expect(is_two_sum_answer(nums, 9, solution.twoSum_optimized(nums, 9)),
               "LeetCode 1 optimized solution");
    }
    {
        std::vector<int> nums{3, 3};
        expect(is_two_sum_answer(nums, 6, solution.twoSum(nums, 6)),
               "LeetCode 1 duplicate values");
    }
    {
        std::vector<int> nums{1, 2, 3};
        expect(solution.twoSum(nums, 100).empty(),
               "LeetCode 1 no solution");
    }
    {
        std::vector<int> nums{std::numeric_limits<int>::min(), 0};
        expect(solution.twoSum(nums, std::numeric_limits<int>::max()).empty() &&
                   solution.twoSum_bruteForce(nums, std::numeric_limits<int>::max()).empty() &&
                   solution.twoSum_optimized(nums, std::numeric_limits<int>::max()).empty(),
               "LeetCode 1 complement arithmetic stays defined");
    }
}

void test_two_sum_ii() {
    LC0167::Solution solution;

    {
        std::vector<int> numbers{2, 7, 11, 15};
        expect(solution.twoSum(numbers, 9) == std::vector<int>({1, 2}),
               "LeetCode 167 basic sorted input");
    }
    {
        std::vector<int> numbers{-3, -1, 0, 4, 8};
        expect(solution.twoSum(numbers, 7) == std::vector<int>({2, 5}),
               "LeetCode 167 negative values");
    }
    {
        std::vector<int> numbers{1, 2, 3};
        expect(solution.twoSum(numbers, 100).empty(),
               "LeetCode 167 no solution");
    }
    {
        std::vector<int> numbers{1, 2, 3};
        expect(solution.twoSum(numbers, 6).empty(),
               "LeetCode 167 never reuses the converged index");
    }
    {
        std::vector<int> numbers{3};
        expect(solution.twoSum(numbers, 6).empty(),
               "LeetCode 167 requires two distinct indices");
    }
    {
        std::vector<int> numbers{std::numeric_limits<int>::max(),
                                 std::numeric_limits<int>::max()};
        expect(solution.twoSum(numbers, -2).empty(),
               "LeetCode 167 sum arithmetic stays defined");
    }
}

}  // namespace

int main() {
    test_two_sum();
    test_two_sum_ii();

    if (failures != 0) {
        std::cerr << failures << " Day 1 algorithm test(s) failed\n";
        return 1;
    }

    std::cout << "All Day 1 algorithm tests passed\n";
    return 0;
}
