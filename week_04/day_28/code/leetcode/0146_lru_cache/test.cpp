/**
 * @file test.cpp
 * @brief LeetCode 146: LRU缓存机制 - 测试文件
 */

#include <iostream>
#include "solution.h"

namespace lru_cache_test {

/**
 * @brief 打印测试结果
 */
int failures = 0;

void printResult(bool passed, const std::string& testName) {
    if (passed) {
        std::cout << "  ✓ " << testName << " 通过\n";
    } else {
        std::cout << "  ✗ " << testName << " 失败\n";
        ++failures;
    }
}

/**
 * @brief 运行所有测试用例
 */
void test() {
    std::cout << "=== LRU缓存测试 ===\n\n";
    
    // 测试1：基本操作
    {
        std::cout << "测试1：基本操作\n";
        LRUCache cache(2);
        
        cache.put(1, 1);
        cache.put(2, 2);
        printResult(cache.get(1) == 1, "put和get基本操作");
        
        cache.put(3, 3);  // 淘汰key=2
        printResult(cache.get(2) == -1, "淘汰最久未使用的键");
        printResult(cache.get(1) == 1, "访问过的键仍存在");
        
        cache.put(4, 4);  // 第二次 get(1) 后，key=3 才是最久未使用
        printResult(cache.get(1) == 1, "最近再次访问的key=1保留");
        printResult(cache.get(3) == -1, "第二次淘汰key=3");
        printResult(cache.get(4) == 4, "key=4存在");
        std::cout << "\n";
    }
    
    // 测试2：更新已存在的键
    {
        std::cout << "测试2：更新已存在的键\n";
        LRUCache cache(2);
        
        cache.put(1, 1);
        cache.put(2, 2);
        cache.put(1, 10);  // 更新key=1的值
        printResult(cache.get(1) == 10, "更新已存在键的值");
        
        cache.put(3, 3);  // 淘汰key=2
        printResult(cache.get(2) == -1, "更新后淘汰顺序正确");
        printResult(cache.get(1) == 10, "更新的值保留");
        std::cout << "\n";
    }
    
    // 测试3：get操作更新访问顺序
    {
        std::cout << "测试3：get操作更新访问顺序\n";
        LRUCache cache(3);
        
        cache.put(1, 1);
        cache.put(2, 2);
        cache.put(3, 3);
        
        // 访问key=1，使其变为最近使用
        cache.get(1);
        
        cache.put(4, 4);  // 淘汰key=2（最久未使用）
        printResult(cache.get(1) == 1, "get后key=1仍存在");
        printResult(cache.get(2) == -1, "key=2被淘汰");
        printResult(cache.get(3) == 3, "key=3仍存在");
        printResult(cache.get(4) == 4, "key=4存在");
        std::cout << "\n";
    }
    
    // 测试4：容量为1
    {
        std::cout << "测试4：容量为1\n";
        LRUCache cache(1);
        
        cache.put(1, 1);
        printResult(cache.get(1) == 1, "容量1时put");
        
        cache.put(2, 2);
        printResult(cache.get(1) == -1, "key=1被淘汰");
        printResult(cache.get(2) == 2, "key=2存在");
        std::cout << "\n";
    }
    
    // 测试5：LeetCode官方示例
    {
        std::cout << "测试5：LeetCode官方示例\n";
        LRUCache cache(2);
        
        cache.put(1, 1);
        cache.put(2, 2);
        printResult(cache.get(1) == 1, "get(1) = 1");
        
        cache.put(3, 3);
        printResult(cache.get(2) == -1, "get(2) = -1");
        
        cache.put(4, 4);
        printResult(cache.get(1) == -1, "get(1) = -1");
        printResult(cache.get(3) == 3, "get(3) = 3");
        printResult(cache.get(4) == 4, "get(4) = 4");
        std::cout << "\n";
    }
    
    // 测试6：边界条件
    {
        std::cout << "测试6：边界条件\n";
        LRUCache cache(2);
        
        printResult(cache.get(0) == -1, "空缓存get返回-1");
        
        cache.put(1, 1);
        cache.put(1, 1);  // 重复put
        printResult(cache.size() == 1, "重复put不增加大小");
        std::cout << "\n";
    }

    // 测试7：零容量和负容量都按禁用缓存处理
    {
        std::cout << "测试7：非正容量\n";
        LRUCache zero(0);
        zero.put(1, 1);
        printResult(zero.size() == 0 && zero.get(1) == -1, "容量0不保存条目");

        LRUCache negative(-3);
        negative.put(2, 2);
        printResult(negative.capacity() == 0 && negative.size() == 0,
                    "负容量被规范化为0");
        std::cout << "\n";
    }
    
    std::cout << "所有LRU缓存测试完成！\n";
}

} // namespace lru_cache_test

// 如果直接运行此文件
int main() {
    lru_cache_test::test();
    return lru_cache_test::failures == 0 ? 0 : 1;
}
