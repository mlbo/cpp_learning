#include "leetcode/0150_eval_rpn/solution.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

bool check(std::vector<std::string> tokens, int expected, const std::string& name) {
    leetcode_0150::Solution solution;
    const int actual = solution.evalRPN(tokens);
    const bool passed = actual == expected;
    std::cout << name << ": " << (passed ? "通过" : "失败") << '\n';
    return passed;
}

template<typename Exception>
bool checkThrows(std::vector<std::string> tokens, const std::string& name) {
    leetcode_0150::Solution solution;
    try {
        (void)solution.evalRPN(tokens);
    } catch (const Exception&) {
        std::cout << name << ": 通过\n";
        return true;
    } catch (...) {
    }
    std::cout << name << ": 失败\n";
    return false;
}

} // namespace

int main() {
    int failures = 0;
    if (!check({"2", "1", "+", "3", "*"}, 9, "标准表达式")) ++failures;
    if (!check({"4", "13", "5", "/", "+"}, 6, "整数除法")) ++failures;
    if (!check({"5", "2", "-"}, 3, "左右操作数顺序")) ++failures;
    if (!check({"7", "-3", "/"}, -2, "负数除法向0截断")) ++failures;
    if (!check({"42"}, 42, "单个操作数")) ++failures;

    if (!checkThrows<std::invalid_argument>({}, "空表达式")) ++failures;
    if (!checkThrows<std::invalid_argument>({"+"}, "操作数不足")) ++failures;
    if (!checkThrows<std::invalid_argument>({"1", "2"}, "结果不唯一")) ++failures;
    if (!checkThrows<std::invalid_argument>({"12x"}, "非法数字token")) ++failures;
    if (!checkThrows<std::domain_error>({"1", "0", "/"}, "除以0")) ++failures;
    if (!checkThrows<std::overflow_error>({"2147483647", "1", "+"},
                                          "加法溢出受检")) ++failures;
    if (!checkThrows<std::overflow_error>({"50000", "50000", "*"},
                                          "乘法溢出受检")) ++failures;
    if (!checkThrows<std::overflow_error>({"-2147483648", "-1", "/"},
                                          "INT_MIN除以-1受检")) ++failures;

    return failures == 0 ? 0 : 1;
}
