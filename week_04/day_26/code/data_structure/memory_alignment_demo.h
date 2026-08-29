#ifndef MEMORY_ALIGNMENT_DEMO_H
#define MEMORY_ALIGNMENT_DEMO_H

/**
 * 内存对齐演示
 * 展示内存对齐概念、alignas/alignof关键字
 */

#include <cstddef>

// 演示默认对齐的结构体
struct DefaultAligned {
    char a;
    int b;
    char c;
    double d;
};  // 偏移和总大小由目标ABI决定，演示程序会实际查询

// 优化成员顺序后的结构体
struct OptimizedAligned {
    double d;   // 8字节
    int b;      // 4字节
    char a;     // 1字节
    char c;     // 1字节 + 2字节填充
};  // 常见ABI上更紧凑，但不把具体字节数写成跨平台保证

// 使用alignas指定对齐的结构体
struct alignas(16) AlignedTo16 {
    int x;
    int y;
};  // 强制16字节对齐

// 以64字节作为实验布局参数，不声称它等于当前机器的真实缓存行大小
struct alignas(64) CacheLineAligned {
    int data[14];
};

/**
 * 运行内存对齐演示
 */
void memoryAlignmentDemo();

/**
 * 打印基本类型对齐要求
 */
void printBasicAlignments();

/**
 * 演示结构体内存布局
 */
void demoStructLayout();

/**
 * 演示alignas/alignof使用
 */
void demoAlignasAlignof();

/**
 * 演示结构体优化
 */
void demoStructOptimization();

/**
 * 用合法的字节观察和memcpy解释为什么不能随意构造未对齐T*
 */
void demoSafeByteAccess();

#endif // MEMORY_ALIGNMENT_DEMO_H
