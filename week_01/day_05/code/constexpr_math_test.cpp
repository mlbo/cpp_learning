#ifdef NDEBUG
#undef NDEBUG
#endif

#include "constexpr_math.h"

#include <cassert>
#include <iostream>
#include <stdexcept>

static_assert(day05::factorial(0) == 1);
static_assert(day05::factorial(5) == 120);
static_assert(day05::factorial(12) == 479001600);
static_assert(day05::factorial_fits_int(12));
static_assert(!day05::factorial_fits_int(13));
static_assert(day05::square(46340) == 2147395600);
static_assert(day05::sum_to_n(65535) == 2147450880);

void expect_out_of_range(int n) {
    bool caught = false;
    try {
        (void)day05::factorial(n);
    } catch (const std::out_of_range&) {
        caught = true;
    }
    assert(caught);
}

int main() {
    assert(day05::factorial(12) == 479001600);
    expect_out_of_range(-1);
    expect_out_of_range(13);
    bool overflow_caught = false;
    try {
        (void)day05::square(46341);
    } catch (const std::overflow_error&) {
        overflow_caught = true;
    }
    assert(overflow_caught);
    std::cout << "factorial int 边界通过：[0, 12] 可计算，-1/13 被拒绝。\n";
    return 0;
}
