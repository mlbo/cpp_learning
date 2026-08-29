#ifndef WEEK01_COMMON_INTEGER_CONTRACTS_H
#define WEEK01_COMMON_INTEGER_CONTRACTS_H

#include <cstddef>
#include <cstdint>
#include <limits>
#include <stdexcept>

namespace week01 {

// Week 1 的 LeetCode 风格接口按题意返回 int，但容器下标使用 size_t。
// 转换前统一检查，避免大输入在窄化时得到错误下标或实现相关结果。
inline int checked_index(std::size_t index) {
    if (index > static_cast<std::size_t>(std::numeric_limits<int>::max())) {
        throw std::length_error("container index does not fit in int");
    }
    return static_cast<int>(index);
}

// 中间算术先提升到 int64_t；公开接口必须返回 int 时在边界统一失败。
inline int checked_result(std::int64_t value) {
    if (value < static_cast<std::int64_t>(std::numeric_limits<int>::min()) ||
        value > static_cast<std::int64_t>(std::numeric_limits<int>::max())) {
        throw std::overflow_error("mathematical result does not fit in int");
    }
    return static_cast<int>(value);
}

}  // namespace week01

#endif  // WEEK01_COMMON_INTEGER_CONTRACTS_H
