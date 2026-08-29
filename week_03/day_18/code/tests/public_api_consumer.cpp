#include "cpp11_features/enum_class_demo.h"
#include "cpp11_features/enum_vs_enum_class.h"
#include "emcpp/item10_enum_class.h"
#include "leetcode/0042_trapping_rain_water/solution.h"
#include "leetcode/0084_largest_rectangle/solution.h"
#include "low_level/call_stack_demo.h"
#include "low_level/stack_frame.h"

#include <vector>

int main() {
    callStackDemo();
    run_stack_frame_demo();
    enumClassDemo();
    run_enum_vs_enum_class_demo();
    item10Demo();

    const std::vector<int> histogram = {2, 1, 5, 6, 2, 3};
    const std::vector<int> heights = {2, 0, 2};
    leetcode_0084::Solution rectangle;
    leetcode_0042::Solution rain;
    return rectangle.largestRectangleArea(histogram) == 10 && rain.trap(heights) == 2 ? 0 : 1;
}
