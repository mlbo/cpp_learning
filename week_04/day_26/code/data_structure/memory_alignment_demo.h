#ifndef MEMORY_ALIGNMENT_DEMO_H
#define MEMORY_ALIGNMENT_DEMO_H

/**
 * 内存对齐演示
 * 展示内存对齐概念、alignas/alignof关键字
 */

#include <cstddef>

// 演示默认对齐的结构体
struct DefaultAligned {
    char a;     // 1字节 + 3字节填充
    int b;      // 4字节
    char c;     // 1字节 + 3字节填充
    double d;   // 8字节
};  // 总共24字节

// 优化成员顺序后的结构体
struct OptimizedAligned {
    double d;   // 8字节
    int b;      // 4字节
    char a;     // 1字节
    char c;     // 1字节 + 2字节填充
};  // 总共16字节

// 使用alignas指定对齐的结构体
struct alignas(16) AlignedTo16 {
    int x;
    int y;
};  // 强制16字节对齐

// 缓存行对齐的结构体
struct alignas(64) CacheLineAligned {
    int data[14];  // 56字节 + 8字节填充 = 64字节
};

// 紧凑打包的结构体（不推荐，仅作演示）
#pragma pack(push, 1)
struct PackedStruct {
    char a;     // 1字节
    int b;      // 4字节
    char c;     // 1字节
    double d;   // 8字节
};  // 总共14字节，无填充
#pragma pack(pop)

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
 * 演示未对齐访问的性能影响
 */
void demoUnalignedAccess();

#endif // MEMORY_ALIGNMENT_DEMO_H
