/**
 * @file test.cpp
 * @brief LeetCode 460: LFU缓存 - 测试文件
 */

#include <iostream>
#include <cassert>
#include "solution.cpp"

namespace lfu_cache_test {

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
    std::cout << "=== LFU缓存测试 ===\n\n";
    
    // 测试1：基本操作
    {
        std::cout << "测试1：基本操作\n";
        LFUCache cache(2);
        
        cache.put(1, 1);
        cache.put(2, 2);
        printResult(cache.get(1) == 1, "put和get基本操作");
        
        cache.put(3, 3);  // 淘汰key=2（频率最低）
        printResult(cache.get(2) == -1, "淘汰频率最低的键");
        printResult(cache.get(1) == 1, "高频键保留");
        printResult(cache.get(3) == 3, "新键存在");
        std::cout << "\n";
    }
    
    // 测试2：频率更新
    {
        std::cout << "测试2：频率更新\n";
        LFUCache cache(3);
        
        cache.put(1, 1);
        cache.put(2, 2);
        cache.put(3, 3);
        
        // key=1访问2次，key=2访问1次，key=3访问1次
        cache.get(1);
        cache.get(1);
        cache.get(2);
        
        cache.put(4, 4);  // 淘汰key=3（频率最低，且最久未使用）
        printResult(cache.get(1) == 1, "key=1存在（频率高）");
        printResult(cache.get(2) == 2, "key=2存在");
        printResult(cache.get(3) == -1, "key=3被淘汰");
        printResult(cache.get(4) == 4, "key=4存在");
        std::cout << "\n";
    }
    
    // 测试3：同频率时LRU淘汰
    {
        std::cout << "测试3：同频率时LRU淘汰\n";
        LFUCache cache(2);
        
        cache.put(1, 1);  // freq=1
        cache.put(2, 2);  // freq=1
        // key=1和key=2频率相同，key=1更久未使用
        
        cache.put(3, 3);  // 淘汰key=1（同频率下最久未使用）
        printResult(cache.get(1) == -1, "key=1被淘汰（同频率LRU）");
        printResult(cache.get(2) == 2, "key=2保留");
        printResult(cache.get(3) == 3, "key=3存在");
        std::cout << "\n";
    }
    
    // 测试4：更新已存在的键
    {
        std::cout << "测试4：更新已存在的键\n";
        LFUCache cache(2);
        
        cache.put(1, 1);
        cache.put(2, 2);
        cache.put(1, 10);  // 更新值并增加频率
        
        cache.put(3, 3);   // 淘汰key=2（频率最低）
        printResult(cache.get(1) == 10, "更新后的值正确");
        printResult(cache.get(2) == -1, "key=2被淘汰");
        std::cout << "\n";
    }
    
    // 测试5：LeetCode官方示例
    {
        std::cout << "测试5：LeetCode官方示例\n";
        LFUCache cache(2);
        
        cache.put(1, 1);
        cache.put(2, 2);
        printResult(cache.get(1) == 1, "get(1) = 1");
        
        cache.put(3, 3);  // 淘汰key=2
        printResult(cache.get(2) == -1, "get(2) = -1");
        printResult(cache.get(3) == 3, "get(3) = 3");
        
        cache.put(4, 4);  // 淘汰key=1
        printResult(cache.get(1) == -1, "get(1) = -1");
        printResult(cache.get(3) == 3, "get(3) = 3");
        printResult(cache.get(4) == 4, "get(4) = 4");
        std::cout << "\n";
    }
    
    // 测试6：容量为1
    {
        std::cout << "测试6：容量为1\n";
        LFUCache cache(1);
        
        cache.put(1, 1);
        printResult(cache.get(1) == 1, "容量1时put");
        
        cache.put(2, 2);
        printResult(cache.get(1) == -1, "key=1被淘汰");
        printResult(cache.get(2) == 2, "key=2存在");
        
        cache.get(2);
        cache.put(3, 3);
        printResult(cache.get(2) == -1, "key=2被淘汰");
        printResult(cache.get(3) == 3, "key=3存在");
        std::cout << "\n";
    }
    
    // 测试7：容量为0
    {
        std::cout << "测试7：容量为0\n";
        LFUCache cache(0);
        
        cache.put(1, 1);
        printResult(cache.get(1) == -1, "容量0时无法存储");
        std::cout << "\n";
    }
    
    // 测试8：复杂场景
    {
        std::cout << "测试8：复杂场景\n";
        LFUCache cache(3);
        
        // 插入3个元素
        cache.put(1, 1);
        cache.put(2, 2);
        cache.put(3, 3);
        
        // key=1访问3次
        cache.get(1); cache.get(1); cache.get(1);  // freq=4
        // key=2访问2次
        cache.get(2); cache.get(2);  // freq=3
        // key=3访问1次
        cache.get(3);  // freq=2
        
        // 插入新元素，淘汰key=3（频率最低）
        cache.put(4, 4);
        printResult(cache.get(1) == 1, "key=1存在（频率最高）");
        printResult(cache.get(2) == 2, "key=2存在");
        printResult(cache.get(3) == -1, "key=3被淘汰（频率最低）");
        printResult(cache.get(4) == 4, "key=4存在");
        std::cout << "\n";
    }
    
    // 测试9：边界条件
    {
        std::cout << "测试9：边界条件\n";
        LFUCache cache(2);
        
        printResult(cache.get(0) == -1, "空缓存get返回-1");
        
        cache.put(1, 1);
        cache.put(1, 1);  // 重复put，增加频率
        cache.put(1, 100); // 更新值
        printResult(cache.get(1) == 100, "重复put后值正确");
        printResult(cache.size() == 1, "重复put不增加大小");
        std::cout << "\n";
    }
    
    std::cout << "所有LFU缓存测试完成！\n";
}

} // namespace lfu_cache_test

// 如果直接运行此文件
int main() {
    lfu_cache_test::test();
    return 0;
}
