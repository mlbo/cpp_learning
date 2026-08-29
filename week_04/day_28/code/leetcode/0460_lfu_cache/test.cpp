/**
 * @file test.cpp
 * @brief LeetCode 460: LFU缓存 - 测试文件
 */

#include <iostream>
#include <type_traits>

#include "solution.h"

static_assert(std::is_unsigned<LFUCache::Frequency>::value,
              "LFU frequency must use an unsigned representation");

namespace lfu_cache_test {

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

    // 测试10：负容量与频率链表反复迁移
    {
        std::cout << "测试10：容量规范化与生命周期压力\n";
        LFUCache disabled(-1);
        disabled.put(1, 1);
        printResult(disabled.capacity() == 0 && disabled.size() == 0,
                    "负容量被规范化为0");

        LFUCache cache(2);
        cache.put(1, 10);
        cache.put(2, 20);
        bool survivedPromotions = true;
        for (int i = 0; i < 100; ++i) {
            survivedPromotions = survivedPromotions && cache.get(1) == 10;
        }
        printResult(survivedPromotions, "高频节点反复迁移后仍可访问");
        cache.put(3, 30);
        printResult(cache.get(2) == -1 && cache.get(3) == 30,
                    "反复迁移后仍淘汰最低频节点");
        std::cout << "\n";
    }

    // 测试11：长序列不得留下空频率桶
    {
        std::cout << "测试11：辅助状态空间上界\n";
        LFUCache cache(1);
        cache.put(7, 70);

        bool allHits = true;
        for (int i = 0; i < 10000; ++i) {
            allHits = allHits && cache.get(7) == 70;
        }

        printResult(allHits, "容量1时连续10000次get均命中");
        printResult(cache.size() == 1 && cache.frequencyBucketCount() == 1,
                    "空频率桶及时释放，辅助桶数不超过缓存节点数");
        printResult(cache.minFreq() == 10001,
                    "长序列中的最小频率保持一致");
        std::cout << "\n";
    }

    // 测试12：在有限步内验证频率上界和桶内LRU规则
    {
        std::cout << "测试12：频率表示边界\n";
        LFUCache cache(2, 3);
        cache.put(1, 10);
        cache.put(2, 20);
        cache.get(1);
        cache.get(1);  // key=1 达到上界3
        cache.get(2);
        cache.get(2);  // key=2 也达到上界3
        cache.get(1);  // 上界上不递增，但更新桶内的LRU顺序
        cache.put(3, 30);

        printResult(cache.get(2) == -1 && cache.get(1) == 10 && cache.get(3) == 30,
                    "频率饱和后按同频LRU规则淘汰");
        printResult(cache.frequencyBucketCount() <= cache.size(),
                    "频率边界下仍满足O(capacity)辅助空间");
        std::cout << "\n";
    }
    
    std::cout << "所有LFU缓存测试完成！\n";
}

} // namespace lfu_cache_test

// 如果直接运行此文件
int main() {
    lfu_cache_test::test();
    return lfu_cache_test::failures == 0 ? 0 : 1;
}
