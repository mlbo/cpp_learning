#ifdef NDEBUG
#undef NDEBUG
#endif

#include "safe_index_access.h"

#include <cassert>
#include <iostream>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

template<typename Container, typename = void>
struct can_get_element : std::false_type {};

template<typename Container>
struct can_get_element<Container, std::void_t<
    decltype(get_element(std::declval<Container>(), std::size_t{0}))
>> : std::true_type {};

static_assert(can_get_element<std::vector<int>&>::value);
static_assert(can_get_element<const std::vector<int>&>::value);
static_assert(!can_get_element<std::vector<int>&&>::value,
              "元素引用不能从临时 vector 中逃逸");

// 仅供 AddressSanitizer 负例验证。正常测试不会调用这个包装器。
template<typename Container, typename Index>
decltype(auto) unsafe_get_element(Container&& container, Index index) {
    return std::forward<Container>(container)[index];
}

int main(int argc, char* argv[]) {
    if (argc == 2 && std::string_view(argv[1]) == "--trigger-asan") {
        decltype(auto) escaped =
            unsafe_get_element(std::vector<int>{10, 20, 30}, std::size_t{1});
        std::cout << "悬空引用读取（预期由 ASan 拦截）: " << escaped << '\n';
        return 0;
    }

    std::vector<int> values{10, 20, 30};
    static_assert(std::is_same_v<decltype(get_element(values, 0)), int&>);
    get_element(values, 1) = 42;
    assert(values[1] == 42);

    const std::vector<int>& read_only = values;
    static_assert(std::is_same_v<decltype(get_element(read_only, 0)), const int&>);
    assert(get_element(read_only, 0) == 10);

    bool negative_rejected = false;
    try {
        (void)get_element(values, -1);
    } catch (const std::out_of_range&) {
        negative_rejected = true;
    }
    assert(negative_rejected);

    bool upper_bound_rejected = false;
    try {
        (void)get_element(values, values.size());
    } catch (const std::out_of_range&) {
        upper_bound_rejected = true;
    }
    assert(upper_bound_rejected);

    std::cout << "lvalue/const lvalue 元素访问通过；负下标和上界越界已拒绝；临时容器调用已在编译期拒绝。\n";
    return 0;
}
