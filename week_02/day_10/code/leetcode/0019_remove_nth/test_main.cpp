#include <iostream>

namespace leetcode_0019 {
bool test_leetcode_19();
}

int main() {
    const bool passed = leetcode_0019::test_leetcode_19();
    std::cout << (passed ? "LeetCode 19 全部测试通过\n"
                         : "LeetCode 19 存在失败用例\n");
    return passed ? 0 : 1;
}
