#include <cstdint>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <vector>

int get_first(const std::vector<int>& arr);
int add(int a, int b);
long long sum(const std::vector<int>& arr);

namespace {

int failures = 0;

void expect(bool condition, const char* name) {
    if (condition) {
        std::cout << "[PASS] " << name << '\n';
    } else {
        std::cerr << "[FAIL] " << name << '\n';
        ++failures;
    }
}

template<typename Exception, typename Operation>
void expect_throw(Operation operation, const char* name) {
    try {
        operation();
        expect(false, name);
    } catch (const Exception&) {
        expect(true, name);
    } catch (...) {
        expect(false, name);
    }
}

}  // namespace

int main() {
    expect(get_first({42}) == 42, "get_first returns the first value");
    expect_throw<std::out_of_range>([] { (void)get_first({}); },
                                    "get_first rejects an empty vector");
    expect(add(20, 22) == 42, "add returns an in-range sum");
    expect_throw<std::overflow_error>([] {
        (void)add(std::numeric_limits<int>::max(), 1);
    }, "add rejects positive int overflow");
    expect_throw<std::overflow_error>([] {
        (void)add(std::numeric_limits<int>::min(), -1);
    }, "add rejects negative int overflow");
    expect(sum({std::numeric_limits<int>::max(),
                std::numeric_limits<int>::max()}) ==
               static_cast<long long>(std::numeric_limits<int>::max()) * 2,
           "sum accumulates beyond int without overflow");

    return failures == 0 ? 0 : 1;
}
