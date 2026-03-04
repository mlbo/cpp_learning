/**
 * Day 26: CPU缓存与内存对齐
 * 主程序入口
 */

#include <iostream>
#include "data_structure/cpu_cache_demo.h"
#include "data_structure/memory_alignment_demo.h"
#include "data_structure/cache_friendly_demo.h"

void testLongestPalindrome();
void testPalindromicSubstrings();

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  Day 26: CPU缓存与内存对齐" << std::endl;
    std::cout << "========================================" << std::endl;

    // 1. CPU缓存演示
    std::cout << "\n【1. CPU缓存演示】" << std::endl;
    cpuCacheDemo();

    // 2. 内存对齐演示
    std::cout << "\n【2. 内存对齐演示】" << std::endl;
    memoryAlignmentDemo();

    // 3. 缓存友好编程演示
    std::cout << "\n【3. 缓存友好编程演示】" << std::endl;
    cacheFriendlyDemo();

    // 4. LeetCode 5 测试
    std::cout << "\n【4. LeetCode 5: 最长回文子串】" << std::endl;
    testLongestPalindrome();

    // 5. LeetCode 647 测试
    std::cout << "\n【5. LeetCode 647: 回文子串】" << std::endl;
    testPalindromicSubstrings();

    std::cout << "\n========================================" << std::endl;
    std::cout << "  Day 26 学习完成！" << std::endl;
    std::cout << "  核心要点：" << std::endl;
    std::cout << "  - CPU缓存分层：L1/L2/L3" << std::endl;
    std::cout << "  - 缓存行大小：通常64字节" << std::endl;
    std::cout << "  - 内存对齐提升访问效率" << std::endl;
    std::cout << "  - 缓存友好编程：连续访问优先" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}
