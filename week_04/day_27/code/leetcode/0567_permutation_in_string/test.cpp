/**
 * LeetCode 567: 字符串的排列 - 测试文件
 */

#include "leetcode/0567_permutation_in_string/solution.h"
#include <iostream>
#include <vector>

using namespace std;

// 测试用例结构
struct TestCase {
    string s1;
    string s2;
    bool expected;
    string description;
};

// 运行测试用例
void runTest(const TestCase& tc, size_t testNum) {
    day27::lc0567::Solution sol;
    bool result = sol.checkInclusion(tc.s1, tc.s2);
    
    bool passed = (result == tc.expected);
    
    cout << "测试 " << testNum << ": " << tc.description << endl;
    cout << "  s1 = \"" << tc.s1 << "\"" << endl;
    cout << "  s2 = \"" << tc.s2 << "\"" << endl;
    cout << "  期望: " << (tc.expected ? "true" : "false") << endl;
    cout << "  结果: " << (result ? "true" : "false") << endl;
    cout << "  状态: " << (passed ? "✓ 通过" : "✗ 失败") << endl;
    cout << endl;
    
    if (!passed) {
        cerr << "断言失败！" << endl;
    }
}

int main() {
    cout << R"(
╔════════════════════════════════════════╗
║  LeetCode 567: 字符串的排列            ║
╚════════════════════════════════════════╝
)" << endl;

    vector<TestCase> testCases = {
        {
            "ab",
            "eidbaooo",
            true,
            "标准测试用例 - 存在排列 'ba'"
        },
        {
            "ab",
            "eidboaoo",
            false,
            "不存在排列"
        },
        {
            "a",
            "a",
            true,
            "单字符匹配"
        },
        {
            "a",
            "b",
            false,
            "单字符不匹配"
        },
        {
            "abc",
            "ccccbbbbaaaa",
            false,
            "字符数量不匹配"
        },
        {
            "adc",
            "dcda",
            true,
            "排列在开头"
        },
        {
            "hello",
            "ooolleoooleh",
            false,
            "复杂情况"
        },
        {
            "ab",
            "ab",
            true,
            "完全匹配"
        },
        {
            "abc",
            "abcabc",
            true,
            "排列在开头"
        },
        {
            "trinitrophenylmethylnitramine",
            "dinitrophenylhydrazinetrinitrophenylmethylnitramine",
            true,
            "长字符串测试"
        },
        {
            "",
            "anything",
            true,
            "空模式按数学约定匹配空窗口"
        },
        {
            "Ab",
            "xxbAyy",
            true,
            "大小写敏感且支持一般字节"
        },
        {
            string("\xFF\x01", 2),
            string("z\x01\xFFz", 4),
            true,
            "高位字节不会形成负下标"
        }
    };

    size_t passed = 0;
    for (size_t i = 0; i < testCases.size(); i++) {
        day27::lc0567::Solution sol;
        bool result = sol.checkInclusion(testCases[i].s1, testCases[i].s2);
        bool optimized = sol.checkInclusionOptimized(testCases[i].s1, testCases[i].s2);
        if (result == testCases[i].expected && optimized == testCases[i].expected) {
            ++passed;
        }
        runTest(testCases[i], i + 1);
    }

    cout << "========================================" << endl;
    cout << "测试结果: " << passed << "/" << testCases.size() << " 通过" << endl;
    
    // 测试优化版本
    cout << "\n========== 测试优化版本 ==========" << endl;
    day27::lc0567::Solution sol;
    bool optimizedResult = sol.checkInclusionOptimized("ab", "eidbaooo");
    cout << "checkInclusionOptimized(\"ab\", \"eidbaooo\") = " 
         << (optimizedResult ? "true" : "false") << endl;
    
    // 演示算法执行过程
    cout << "\n========== 算法执行演示 ==========" << endl;
    day27::lc0567::demonstratePermutationAlgorithm("ab", "eidbaooo");
    
    cout << "\n========== 演示不匹配情况 ==========" << endl;
    day27::lc0567::demonstratePermutationAlgorithm("ab", "eidboaoo");
    day27::lc0567::demonstratePermutationAlgorithm("", "anything");
    day27::lc0567::demonstratePermutationAlgorithm(
        string("\xFF\x01", 2), string("z\x01\xFFz", 4));

    return passed == testCases.size() ? 0 : 1;
}
