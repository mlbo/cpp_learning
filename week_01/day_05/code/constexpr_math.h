#ifndef DAY05_CONSTEXPR_MATH_H
#define DAY05_CONSTEXPR_MATH_H

#include <cstdint>
#include <limits>
#include <stdexcept>

namespace day05 {

inline constexpr int max_factorial_input = 12;

constexpr int checked_int(std::int64_t value) {
    return value >= static_cast<std::int64_t>(std::numeric_limits<int>::min()) &&
                   value <= static_cast<std::int64_t>(std::numeric_limits<int>::max())
        ? static_cast<int>(value)
        : throw std::overflow_error("constexpr integer result does not fit in int");
}

constexpr int checked_add(int lhs, int rhs) {
    return checked_int(static_cast<std::int64_t>(lhs) + rhs);
}

constexpr int checked_subtract(int lhs, int rhs) {
    return checked_int(static_cast<std::int64_t>(lhs) - rhs);
}

constexpr int checked_multiply(int lhs, int rhs) {
    return checked_int(static_cast<std::int64_t>(lhs) * rhs);
}

constexpr int square(int value) {
    return checked_multiply(value, value);
}

constexpr int sum_to_n(int n) {
    return n >= 0
        ? checked_int((static_cast<std::int64_t>(n) * (n + std::int64_t{1})) / 2)
        : throw std::invalid_argument("sum_to_n(int) requires n >= 0");
}

constexpr bool factorial_fits_int(int n) noexcept {
    return n >= 0 && n <= max_factorial_input;
}

constexpr int factorial(int n) {
    return factorial_fits_int(n)
        ? (n <= 1 ? 1 : n * factorial(n - 1))
        : throw std::out_of_range("factorial(int) requires 0 <= n <= 12");
}

}  // namespace day05

#endif
