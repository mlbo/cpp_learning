#include <algorithm>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

namespace leetcode::p0026 {
int removeDuplicates(std::vector<int>& nums);
}

namespace leetcode::p0027 {
int removeElement(std::vector<int>& nums, int val);
int removeElementOptimized(std::vector<int>& nums, int val);
}

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

bool prefix_equals(const std::vector<int>& actual,
                   int length,
                   const std::vector<int>& expected) {
    if (length < 0 || static_cast<std::size_t>(length) > actual.size()) {
        return false;
    }

    return static_cast<std::size_t>(length) == expected.size() &&
           std::equal(expected.begin(), expected.end(), actual.begin());
}

void test_remove_duplicates() {
    {
        std::vector<int> nums;
        const int length = leetcode::p0026::removeDuplicates(nums);
        expect(length == 0, "26 empty input");
    }
    {
        std::vector<int> nums{1, 1, 2};
        const int length = leetcode::p0026::removeDuplicates(nums);
        expect(prefix_equals(nums, length, {1, 2}), "26 basic duplicates");
    }
    {
        std::vector<int> nums{0, 0, 1, 1, 1, 2, 2, 3, 3, 4};
        const int length = leetcode::p0026::removeDuplicates(nums);
        expect(prefix_equals(nums, length, {0, 1, 2, 3, 4}),
               "26 multiple duplicate groups");
    }
}

void test_remove_element_stable() {
    {
        std::vector<int> nums;
        const int length = leetcode::p0027::removeElement(nums, 1);
        expect(length == 0, "27 stable empty input");
    }
    {
        std::vector<int> nums{0, 1, 2, 2, 3, 0, 4, 2};
        const int length = leetcode::p0027::removeElement(nums, 2);
        expect(prefix_equals(nums, length, {0, 1, 3, 0, 4}),
               "27 stable keeps relative order");
    }
}

void test_remove_element_unstable() {
    std::vector<int> nums{0, 1, 2, 2, 3, 0, 4, 2};
    const int length = leetcode::p0027::removeElementOptimized(nums, 2);

    std::vector<int> prefix(nums.begin(), nums.begin() + length);
    std::sort(prefix.begin(), prefix.end());
    expect(length == 5 && prefix == std::vector<int>({0, 0, 1, 3, 4}),
           "27 unstable keeps the correct multiset");
}

}  // namespace

int main() {
    test_remove_duplicates();
    test_remove_element_stable();
    test_remove_element_unstable();

    if (failures != 0) {
        std::cerr << failures << " Day 2 algorithm test(s) failed\n";
        return 1;
    }

    std::cout << "All Day 2 algorithm tests passed\n";
    return 0;
}
