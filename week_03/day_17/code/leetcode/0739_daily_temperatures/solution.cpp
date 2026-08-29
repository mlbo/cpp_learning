/**
 * LeetCode 739. 每日温度
 * 
 * 给定一个整数数组 temperatures，表示每天的温度，
 * 返回一个数组 answer，其中 answer[i] 是指对于第 i 天，
 * 下一个更高温度出现在几天后。
 */

#include "solution.h"
#include <iostream>
#include <stack>

namespace lc739 {

std::vector<int> Solution::dailyTemperatures(const std::vector<int>& temperatures) {
    std::vector<int> answer(temperatures.size(), 0);
    std::stack<std::size_t> stk;  // 存储下标
    
    for (std::size_t i = 0; i < temperatures.size(); ++i) {
        // 当前温度比栈顶温度高，找到了栈顶天数的答案
        while (!stk.empty() && temperatures[i] > temperatures[stk.top()]) {
            const std::size_t prev = stk.top();
            stk.pop();
            answer[prev] = static_cast<int>(i - prev);  // 题目结果类型为int
        }
        stk.push(i);
    }
    
    // 栈中剩余的天数没有更高温度，answer已经初始化为0
    
    return answer;
}

bool testDailyTemperatures() {
    Solution sol;

    std::cout << "LeetCode 739. 每日温度 测试结果：" << std::endl;

    struct TestCase {
        std::vector<int> input;
        std::vector<int> expected;
        const char* description;
    };
    const std::vector<TestCase> tests = {
        {{}, {}, "空输入"},
        {{73, 74, 75, 71, 69, 72, 76, 73}, {1, 1, 4, 2, 1, 1, 0, 0}, "标准案例"},
        {{30, 40, 50, 60}, {1, 1, 1, 0}, "严格递增"},
        {{60, 50, 40, 30}, {0, 0, 0, 0}, "严格递减"},
        {{42, 42, 42}, {0, 0, 0}, "全部相等"}
    };

    bool allPassed = true;
    for (const auto& test : tests) {
        const auto result = sol.dailyTemperatures(test.input);
        const bool passed = result == test.expected;
        allPassed = allPassed && passed;
        std::cout << "  " << test.description << ": " << (passed ? "✓" : "✗") << std::endl;
    }
    
    // 图解过程
    std::cout << "\n  单调栈过程（输入: [73, 74, 75, 71, 69, 72, 76, 73]）：" << std::endl;
    std::cout << "    i=0: 73入栈, 栈=[0]" << std::endl;
    std::cout << "    i=1: 74>73, 弹出0, ans[0]=1, 74入栈, 栈=[1]" << std::endl;
    std::cout << "    i=2: 75>74, 弹出1, ans[1]=1, 75入栈, 栈=[2]" << std::endl;
    std::cout << "    i=3: 71<75, 71入栈, 栈=[2,3]" << std::endl;
    std::cout << "    i=4: 69<71, 69入栈, 栈=[2,3,4]" << std::endl;
    std::cout << "    i=5: 72>69, 弹出4, ans[4]=1" << std::endl;
    std::cout << "         72>71, 弹出3, ans[3]=2" << std::endl;
    std::cout << "         72<75, 72入栈, 栈=[2,5]" << std::endl;
    std::cout << "    i=6: 76>72, 弹出5, ans[5]=1" << std::endl;
    std::cout << "         76>75, 弹出2, ans[2]=4" << std::endl;
    std::cout << "         76入栈, 栈=[6]" << std::endl;
    std::cout << "    i=7: 73<76, 73入栈, 栈=[6,7]" << std::endl;
    std::cout << "    结束: ans[6]=0, ans[7]=0" << std::endl;

    return allPassed;
}

}  // namespace lc739
