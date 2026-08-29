#ifndef SOLUTION_150_H
#define SOLUTION_150_H

#include <vector>
#include <string>

namespace leetcode_0150 {

class Solution {
public:
    // 非法表达式抛出 invalid_argument，除以 0 抛出 domain_error；
    // 若任一步结果超出 int（包括 INT_MIN / -1），抛出 overflow_error。
    int evalRPN(const std::vector<std::string>& tokens);
};

} // namespace leetcode_0150

void testEvalRPN();

#endif // SOLUTION_150_H
