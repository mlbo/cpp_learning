#ifndef CPU_CACHE_DEMO_H
#define CPU_CACHE_DEMO_H

/**
 * CPU缓存演示
 * 展示缓存层次结构、缓存行、伪共享等概念
 */

#include <atomic>
#include <cstddef>
#include <cstdint>

// 仅用于实验布局的常见值，不代表运行机器的缓存行一定是64字节。
constexpr std::size_t DEMO_CACHE_LINE_BYTES = 64;

// 演示伪共享的结构体（存在伪共享问题）
struct CounterBad {
    std::atomic<std::uint64_t> count1{0};  // 线程1修改
    std::atomic<std::uint64_t> count2{0};  // 线程2修改
    // 两个变量可能在同一缓存行，导致伪共享
};

// 使用缓存行填充避免伪共享的结构体
struct CounterGood {
    alignas(DEMO_CACHE_LINE_BYTES) std::atomic<std::uint64_t> count1{0};
    alignas(DEMO_CACHE_LINE_BYTES) std::atomic<std::uint64_t> count2{0};
};

// 用于测试缓存行影响的数组结构
struct UnalignedArray {
    int data[1024];
};

// 缓存行对齐的数组结构
struct alignas(DEMO_CACHE_LINE_BYTES) AlignedArray {
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
