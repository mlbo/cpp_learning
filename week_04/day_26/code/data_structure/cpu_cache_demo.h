#ifndef CPU_CACHE_DEMO_H
#define CPU_CACHE_DEMO_H

/**
 * CPU缓存演示
 * 展示缓存层次结构、缓存行、伪共享等概念
 */

#include <cstddef>

// 缓存行大小（通常为64字节）
constexpr size_t CACHE_LINE_SIZE = 64;

// 演示伪共享的结构体（存在伪共享问题）
struct CounterBad {
    volatile int count1;  // 线程1修改
    volatile int count2;  // 线程2修改
    // 两个变量可能在同一缓存行，导致伪共享
};

// 使用缓存行填充避免伪共享的结构体
struct alignas(CACHE_LINE_SIZE) CounterGood {
    volatile int count1;
    char padding[CACHE_LINE_SIZE - sizeof(int)];  // 填充到独立缓存行
    volatile int count2;
};

// 用于测试缓存行影响的数组结构
struct UnalignedArray {
    int data[1024];
};

// 缓存行对齐的数组结构
struct alignas(CACHE_LINE_SIZE) AlignedArray {
    int data[1024];
};

/**
 * 运行CPU缓存演示
 */
void cpuCacheDemo();

/**
 * 打印缓存层次信息
 */
void printCacheInfo();

/**
 * 演示缓存行概念
 */
void demoCacheLine();

/**
 * 演示伪共享问题
 */
void demoFalseSharing();

/**
 * 演示缓存命中率影响
 */
void demoCacheHitRate();

#endif // CPU_CACHE_DEMO_H
