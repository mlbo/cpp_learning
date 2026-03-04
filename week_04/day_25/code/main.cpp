/**
 * @file main.cpp
 * @brief Day 25: 完美转发 - 主程序入口
 * 
 * 本程序演示：
 * 1. 完美转发的概念和应用
 * 2. std::forward的工作原理
 * 3. EMC++ Item 29-30: 完美转发的细节与失败案例
 * 4. LeetCode 3: 无重复字符的最长子串
 * 5. LeetCode 438: 找到字符串中所有字母异位词
 */

#include <iostream>
#include <string>

// 声明各个演示函数
namespace cpp11_features {
    void perfectForwardingDemo();
    void stdForwardDemo();
}

namespace emcpp {
    void item29StdForwardDetail();
    void item30PerfectForwardFail();
}

namespace leetcode {
    void lc0003Demo();
    void lc0438Demo();
}

// 打印分隔线
void printSeparator(const std::string& title) {
    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "  " << title << "\n";
    std::cout << "========================================\n\n";
}

int main() {
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║             Day 25: 完美转发                               ║\n";
    std::cout << "║     Perfect Forwarding & Sliding Window Algorithms         ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";

    // 1. 完美转发演示
    printSeparator("知识点一：完美转发");
    cpp11_features::perfectForwardingDemo();

    // 2. std::forward详解
    printSeparator("知识点二：std::forward详解");
    cpp11_features::stdForwardDemo();

    // 3. EMC++ Item 29
    printSeparator("EMC++ Item 29: 理解std::forward");
    emcpp::item29StdForwardDetail();

    // 4. EMC++ Item 30
    printSeparator("EMC++ Item 30: 完美转发失败案例");
    emcpp::item30PerfectForwardFail();

    // 5. LeetCode 3
    printSeparator("LeetCode 3: 无重复字符的最长子串");
    leetcode::lc0003Demo();

    // 6. LeetCode 438
    printSeparator("LeetCode 438: 找到字符串中所有字母异位词");
    leetcode::lc0438Demo();

    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                  Day 25 学习完成！                          ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║  今日要点：                                                ║\n";
    std::cout << "║  1. 完美转发保持参数的值类别（左值/右值）                    ║\n";
    std::cout << "║  2. std::forward是条件性转换工具                            ║\n";
    std::cout << "║  3. 了解完美转发失败的情况                                  ║\n";
    std::cout << "║  4. 滑动窗口解决子串问题                                    ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";

    return 0;
}
