#include "cpp11_features/universal_reference_demo.h"
#include "leetcode/0049_group_anagrams/solution.h"
#include "leetcode/0128_longest_consecutive/solution.h"

#include <algorithm>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

std::vector<std::vector<std::string>> normalize(
    std::vector<std::vector<std::string>> groups) {
    for (auto& group : groups) {
        std::sort(group.begin(), group.end());
    }
    std::sort(groups.begin(), groups.end());
    return groups;
}

template<typename Callable>
bool throwsInvalidArgument(Callable action) {
    try {
        action();
    } catch (const std::invalid_argument&) {
        return true;
    } catch (...) {
        return false;
    }
    return false;
}

} // namespace

int main() {
    int failures = 0;
    const auto expect = [&failures](bool condition, const char* name) {
        std::cout << name << ": " << (condition ? "通过" : "失败") << '\n';
        if (!condition) {
            ++failures;
        }
    };

    using LRef = int&;
    using RRef = int&&;
    static_assert(std::is_same_v<LRef&&, int&>);
    static_assert(std::is_same_v<RRef&&, int&&>);
    static_assert(std::is_same_v<RRef&, int&>);

    int value = 1;
    short shortIndex = 7;
    using day24::universal_reference_lesson::OverloadRoute;
    using day24::universal_reference_lesson::ValueCategoryRoute;
    using day24::universal_reference_lesson::forwardingCall;
    using day24::universal_reference_lesson::selectOverload;

    expect(selectOverload(42) == OverloadRoute::Integer,
           "int精确匹配时非模板重载优先");
    expect(selectOverload(shortIndex) == OverloadRoute::ForwardingTemplate,
           "short左值被转发引用精确匹配");
    expect(forwardingCall(value) == ValueCategoryRoute::Lvalue &&
           forwardingCall(2) == ValueCategoryRoute::Rvalue,
           "std::forward真实到达目标重载");

    day24::lc49::Solution anagrams;
    std::vector<std::string> words = {"eat", "tea", "tan", "ate", "nat", "bat"};
    const std::vector<std::vector<std::string>> expected = {
        {"ate", "eat", "tea"}, {"bat"}, {"nat", "tan"}};
    expect(normalize(anagrams.groupAnagrams(words)) == normalize(expected),
           "LC49排序哈希分组");
    expect(normalize(anagrams.groupAnagramsCount(words)) == normalize(expected),
           "LC49计数哈希分组");
    std::vector<std::string> invalidWords = {"eat", "A"};
    expect(throwsInvalidArgument(
               [&] { static_cast<void>(anagrams.groupAnagrams(invalidWords)); }),
           "LC49排序法拒绝非小写输入");
    expect(throwsInvalidArgument(
               [&] { static_cast<void>(anagrams.groupAnagramsCount(invalidWords)); }),
           "LC49计数法与排序法保持同一输入域");

    day24::lc128::Solution consecutive;
    std::vector<int> ordinary = {100, 4, 200, 1, 3, 2};
    expect(consecutive.longestConsecutive(ordinary) == 4,
           "LC128普通序列");
    expect(consecutive.longestConsecutiveOptimized(ordinary) == 4,
           "LC128消耗式集合与起点法结果一致");
    std::vector<int> minBoundary = {
        std::numeric_limits<int>::min(),
        std::numeric_limits<int>::min() + 1,
        std::numeric_limits<int>::max()};
    expect(consecutive.longestConsecutive(minBoundary) == 2 &&
           consecutive.longestConsecutiveOptimized(minBoundary) == 2,
           "LC128避免INT_MIN减一溢出");
    std::vector<int> maxBoundary = {
        std::numeric_limits<int>::max() - 1,
        std::numeric_limits<int>::max()};
    expect(consecutive.longestConsecutive(maxBoundary) == 2 &&
           consecutive.longestConsecutiveOptimized(maxBoundary) == 2,
           "LC128避免INT_MAX加一溢出");

    return failures == 0 ? 0 : 1;
}
