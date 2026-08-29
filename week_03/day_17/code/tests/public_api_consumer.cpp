#include "algorithm/monotonic_stack_demo.h"
#include "cpp11_features/bind_demo.h"
#include "cpp11_features/function_demo.h"
#include "emcpp/item34_lambda_vs_bind.h"
#include "leetcode/0496_next_greater_element/solution.h"
#include "leetcode/0739_daily_temperatures/solution.h"

#include <vector>

int main() {
    monotonicStackDemo();
    functionDemo();
    bindDemo();
    item34Demo();

    const std::vector<int> values = {2, 1, 2, 4, 3};
    if (findNextGreater(values) != std::vector<int>({4, 2, 4, -1, -1})) {
        return 1;
    }

    lc739::Solution temperatures;
    lc496::Solution greater;
    return temperatures.dailyTemperatures({30, 40, 50, 60}) ==
                   std::vector<int>({1, 1, 1, 0}) &&
                   greater.nextGreaterElement({2, 4}, {1, 2, 3, 4}) ==
                       std::vector<int>({3, -1})
               ? 0
               : 1;
}
