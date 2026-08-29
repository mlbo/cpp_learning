#include "cpp11_features/lambda_capture_demo.h"
#include "cpp11_features/lambda_demo.h"
#include "data_structure/stack_demo.h"
#include "emcpp/item31_avoid_default_capture.h"
#include "leetcode/0020_valid_parentheses/solution.h"
#include "leetcode/1047_remove_adjacent_duplicates/solution.h"

int main() {
    stackDemo();
    lambdaBasicsDemo();
    lambdaCaptureDemo();
    item31Demo();

    lc20::Solution parentheses;
    lc1047::Solution duplicates;
    return parentheses.isValid("([])") && duplicates.removeDuplicates("abbaca") == "ca" ? 0 : 1;
}
