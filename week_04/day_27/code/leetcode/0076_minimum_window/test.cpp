/**
 * LeetCode 76: 最小覆盖子串 - 测试文件
 */

#include "solution.cpp"
#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

// 测试用例结构
struct TestCase {
    string s;
    string t;
    string expected;
    string description;
};

// 运行测试用例
void runTest(const TestCase& tc, int testNum) {
    Solution sol;
    string result = sol.minWindow(tc.s, tc.t);
    
    bool passed = (result == tc.expected);
    
    cout << "测试 " << testNum << ": " << tc.description << endl;
    cout << "  s = \"" << tc.s << "\"" << endl;
    cout << "  t = \"" << tc.t << "\"" << endl;
    cout << "  期望: \"" << tc.expected << "\"" << endl;
    cout << "  结果: \"" << result << "\"" << endl;
    cout << "  状态: " << (passed ? "✓ 通过" : "✗ 失败") << endl;
    cout << endl;
    
    if (!passed) {
        cerr << "断言失败！" << endl;
    }
}

int main() {
    cout << R"(
╔════════════════════════════════════════╗
║  LeetCode 76: 最小覆盖子串             ║
╚════════════════════════════════════════╝
)" << endl;

    vector<TestCase> testCases = {
        {
            "ADOBECODEBANC",
            "ABC",
            "BANC",
            "标准测试用例"
        },
        {
            "a",
            "a",
            "a",
            "单字符匹配"
        },
        {
            "a",
            "aa",
            "",
            "s长度不足"
        },
        {
            "aa",
            "aa",
            "aa",
            "完全匹配"
        },
        {
            "abc",
            "d",
            "",
            "不存在匹配"
        },
        {
            "aaaaaaaaaaaabbbbbcdd",
            "abcdd",
            "abbbbbcdd",
            "包含重复字符"
        },
        {
            "cabwefgewcwaefgcf",
            "cae",
            "cwae",
            "复杂情况"
        }
    };

    int passed = 0;
    for (size_t i = 0; i < testCases.size(); i++) {
        Solution sol;
        string result = sol.minWindow(testCases[i].s, testCases[i].t);
        if (result == testCases[i].expected) {
            passed++;
        }
        runTest(testCases[i], i + 1);
    }

    cout << "========================================" << endl;
    cout << "测试结果: " << passed << "/" << testCases.size() << " 通过" << endl;
    
    // 演示算法执行过程
    cout << "\n========== 算法执行演示 ==========" << endl;
    demonstrateAlgorithm("ADOBECODEBANC", "ABC");

    return passed == (int)testCases.size() ? 0 : 1;
}
