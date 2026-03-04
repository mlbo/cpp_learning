/**
 * @file test.cpp
 * @brief LeetCode 146: LRU缓存机制 - 测试文件
 */

#include <iostream>
#include <cassert>
#include "solution.cpp"

namespace lru_cache_test {

/**
 * @brief 打印测试结果
 */
void printResult(bool passed, const std::string& testName) {
    if (passed) {
        std::cout << "  ✓ " << testName << " 通过\n";
    } else {
        std::cout << "  ✗ " << testName << " 失败\n";
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
        
        cache.put(4, 4);  // 淘汰key=1（因为key=1最近被访问过，但put(3,3)时key=2被淘汰，现在key=1是最久未使用）
        printResult(cache.get(1) == -1, "第二次淘汰正确");
        printResult(cache.get(3) == 3, "key=3存在");
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
    
    std::cout << "所有LRU缓存测试完成！\n";
}

} // namespace lru_cache_test

// 如果直接运行此文件
int main() {
    lru_cache_test::test();
    return 0;
}
