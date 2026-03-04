/**
 * CPU缓存演示
 * 展示缓存层次结构、缓存行、伪共享等概念
 */

#include "cpu_cache_demo.h"
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <iomanip>
#include <cstring>

// 打印缓存层次信息
void printCacheInfo() {
    std::cout << "\n--- 缓存层次结构信息 ---" << std::endl;
    
    std::cout << "┌─────────────────────────────────────────────┐" << std::endl;
    std::cout << "│  CPU缓存层次结构                             │" << std::endl;
    std::cout << "├───────────────┬───────────┬─────────────────┤" << std::endl;
    std::cout << "│   缓存级别    │   大小    │    访问延迟     │" << std::endl;
    std::cout << "├───────────────┼───────────┼─────────────────┤" << std::endl;
    std::cout << "│   L1 Cache    │  32-64KB  │    4-5 周期     │" << std::endl;
    std::cout << "│   L2 Cache    │ 256KB-1MB │   10-12 周期    │" << std::endl;
    std::cout << "│   L3 Cache    │  8-64MB   │   30-40 周期    │" << std::endl;
    std::cout << "│   主内存      │  8-128GB  │  100-300 周期   │" << std::endl;
    std::cout << "└───────────────┴───────────┴─────────────────┘" << std::endl;
    
    std::cout << "\n缓存行大小: " << CACHE_LINE_SIZE << " 字节" << std::endl;
    std::cout << "说明: 缓存行是CPU缓存与主内存交换数据的基本单位" << std::endl;
}

// 演示缓存行概念
void demoCacheLine() {
    std::cout << "\n--- 缓存行概念演示 ---" << std::endl;
    
    std::cout << "当CPU读取一个变量时，会加载整个缓存行(64字节):" << std::endl;
    
    // 创建一个数组来演示缓存行加载
    int arr[16] = {0};  // 16 * 4 = 64字节 = 1个缓存行
    
    std::cout << "int arr[16] 占用 " << sizeof(arr) << " 字节，正好一个缓存行" << std::endl;
    
    // 访问第一个元素会加载整个缓存行
    std::cout << "访问 arr[0] 会将 arr[0]~arr[15] 都加载到缓存" << std::endl;
    std::cout << "这就是空间局部性原理的体现" << std::endl;
    
    // 演示内存地址对缓存行的影响
    int* ptr1 = arr;
    int* ptr2 = arr + 8;
    
    std::cout << "\narr 地址: " << ptr1 << std::endl;
    std::cout << "arr+8 地址: " << ptr2 << std::endl;
    std::cout << "地址差: " << ((char*)ptr2 - (char*)ptr1) << " 字节" << std::endl;
    
    if (((char*)ptr2 - (char*)ptr1) < CACHE_LINE_SIZE) {
        std::cout << "两个变量在同一缓存行中！" << std::endl;
    }
}

// 演示伪共享问题
void demoFalseSharing() {
    std::cout << "\n--- 伪共享问题演示 ---" << std::endl;
    
    const int ITERATIONS = 50'000'000;
    
    std::cout << "测试场景: 两个线程分别修改不同的变量" << std::endl;
    std::cout << "迭代次数: " << ITERATIONS << std::endl;
    
    // 测试存在伪共享的情况
    CounterBad bad;
    bad.count1 = 0;
    bad.count2 = 0;
    
    std::cout << "\n[测试1] 存在伪共享的情况:" << std::endl;
    std::cout << "  CounterBad 大小: " << sizeof(CounterBad) << " 字节" << std::endl;
    std::cout << "  count1 和 count2 可能在同一缓存行" << std::endl;
    
    auto start = std::chrono::high_resolution_clock::now();
    std::thread t1([&]() {
        for (int i = 0; i < ITERATIONS; ++i) {
            bad.count1++;
        }
    });
    std::thread t2([&]() {
        for (int i = 0; i < ITERATIONS; ++i) {
            bad.count2++;
        }
    });
    t1.join();
    t2.join();
    auto end = std::chrono::high_resolution_clock::now();
    auto badTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "  耗时: " << badTime << " ms" << std::endl;
    
    // 测试避免伪共享的情况
    CounterGood good;
    good.count1 = 0;
    good.count2 = 0;
    
    std::cout << "\n[测试2] 避免伪共享的情况:" << std::endl;
    std::cout << "  CounterGood 大小: " << sizeof(CounterGood) << " 字节" << std::endl;
    std::cout << "  count1 和 count2 在不同缓存行" << std::endl;
    
    start = std::chrono::high_resolution_clock::now();
    std::thread t3([&]() {
        for (int i = 0; i < ITERATIONS; ++i) {
            good.count1++;
        }
    });
    std::thread t4([&]() {
        for (int i = 0; i < ITERATIONS; ++i) {
            good.count2++;
        }
    });
    t3.join();
    t4.join();
    end = std::chrono::high_resolution_clock::now();
    auto goodTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "  耗时: " << goodTime << " ms" << std::endl;
    
    // 性能对比
    std::cout << "\n性能对比:" << std::endl;
    if (badTime > 0 && goodTime > 0) {
        double improvement = 100.0 * (badTime - goodTime) / badTime;
        std::cout << "  避免伪共享性能提升: " << std::fixed << std::setprecision(1) 
                  << improvement << "%" << std::endl;
    }
    
    std::cout << "\n结论: 伪共享会导致缓存行在多核之间频繁传递，" << std::endl;
    std::cout << "      使用缓存行填充可以有效避免此问题" << std::endl;
}

// 演示缓存命中率影响
void demoCacheHitRate() {
    std::cout << "\n--- 缓存命中率演示 ---" << std::endl;
    
    const int SIZE = 1024 * 1024;  // 4MB数据
    std::vector<int> data(SIZE, 1);
    
    volatile long long sum = 0;
    
    // 顺序访问（缓存友好）
    std::cout << "[测试1] 顺序访问 (缓存友好)" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < SIZE; ++i) {
        sum += data[i];
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "  耗时: " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() 
              << " ms" << std::endl;
    
    // 跳跃访问（缓存不友好）
    std::cout << "\n[测试2] 跳跃访问 (缓存不友好)" << std::endl;
    const int STRIDE = 64;  // 每次跳跃64个int（256字节，跨多个缓存行）
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < SIZE; i += STRIDE) {
        sum += data[i];
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "  耗时: " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() 
              << " ms" << std::endl;
    
    std::cout << "\n说明: 顺序访问能充分利用缓存行预取，" << std::endl;
    std::cout << "      跳跃访问导致缓存利用率低" << std::endl;
}

// 主演示函数
void cpuCacheDemo() {
    std::cout << "╔══════════════════════════════════════╗" << std::endl;
    std::cout << "║          CPU缓存知识演示             ║" << std::endl;
    std::cout << "║   Cache Hierarchy & False Sharing    ║" << std::endl;
    std::cout << "╚══════════════════════════════════════╝" << std::endl;
    
    printCacheInfo();
    demoCacheLine();
    demoFalseSharing();
    demoCacheHitRate();
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "CPU缓存要点总结:" << std::endl;
    std::cout << "  1. 缓存分层: L1最快最小，L3最慢最大" << std::endl;
    std::cout << "  2. 缓存行: 数据传输的基本单位(64字节)" << std::endl;
    std::cout << "  3. 伪共享: 多线程修改同一缓存行导致性能下降" << std::endl;
    std::cout << "  4. 解决方案: 使用缓存行填充或alignas对齐" << std::endl;
    std::cout << "========================================" << std::endl;
}
