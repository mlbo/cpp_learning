#include "leetcode/0150_eval_rpn/solution.h"
#include "leetcode/0155_min_stack/solution.h"
#include "review/emcpp_review.h"
#include "review/lambda_summary.h"
#include "review/stack_queue_summary.h"

#include <string>
#include <vector>

int main() {
    stackQueueSummary();
    lambdaSummary();
    emcppReview();

    MinStack stack;
    stack.push(3);
    stack.push(1);

    const std::vector<std::string> tokens = {"2", "1", "+", "3", "*"};
    leetcode_0150::Solution rpn;
    return stack.getMin() == 1 && rpn.evalRPN(tokens) == 9 ? 0 : 1;
}
