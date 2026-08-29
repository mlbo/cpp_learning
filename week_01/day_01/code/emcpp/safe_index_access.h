#ifndef DAY01_SAFE_INDEX_ACCESS_H
#define DAY01_SAFE_INDEX_ACCESS_H

#include <cstddef>
#include <stdexcept>
#include <type_traits>

// 下标访问通常返回容器元素的引用，因此只接受左值容器。
// const 左值仍可调用；临时容器不能绑定到 Container&，从接口上阻止引用逃逸。
template<typename Container, typename Index>
decltype(auto) get_element(Container& container, Index index) {
    static_assert(std::is_integral<Index>::value, "index must be an integer type");
    if constexpr (std::is_signed<Index>::value) {
        if (index < 0) {
            throw std::out_of_range("negative index");
        }
    }
    return container.at(static_cast<std::size_t>(index));
}

#endif
