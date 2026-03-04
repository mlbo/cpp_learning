/**
 * 内存对齐演示
 * 展示内存对齐概念、alignas/alignof关键字
 */

#include "memory_alignment_demo.h"
#include <iostream>
#include <iomanip>
#include <cstdint>
#include <cstring>
#include <chrono>

// 打印基本类型对齐要求
void printBasicAlignments() {
    std::cout << "\n--- 基本类型对齐要求 ---" << std::endl;
    
    std::cout << "┌────────────────┬──────────┬──────────┐" << std::endl;
    std::cout << "│     类型       │   大小   │   对齐   │" << std::endl;
    std::cout << "├────────────────┼──────────┼──────────┤" << std::endl;
    std::cout << "│ char           │    " << std::setw(2) << sizeof(char) 
              << "    │    " << std::setw(2) << alignof(char) << "    │" << std::endl;
    std::cout << "│ short          │    " << std::setw(2) << sizeof(short) 
              << "    │    " << std::setw(2) << alignof(short) << "    │" << std::endl;
    std::cout << "│ int            │    " << std::setw(2) << sizeof(int) 
              << "    │    " << std::setw(2) << alignof(int) << "    │" << std::endl;
    std::cout << "│ long           │    " << std::setw(2) << sizeof(long) 
              << "    │    " << std::setw(2) << alignof(long) << "    │" << std::endl;
    std::cout << "│ long long      │    " << std::setw(2) << sizeof(long long) 
              << "    │    " << std::setw(2) << alignof(long long) << "    │" << std::endl;
    std::cout << "│ float          │    " << std::setw(2) << sizeof(float) 
              << "    │    " << std::setw(2) << alignof(float) << "    │" << std::endl;
    std::cout << "│ double         │    " << std::setw(2) << sizeof(double) 
              << "    │    " << std::setw(2) << alignof(double) << "    │" << std::endl;
    std::cout << "│ void*          │    " << std::setw(2) << sizeof(void*) 
              << "    │    " << std::setw(2) << alignof(void*) << "    │" << std::endl;
    std::cout << "└────────────────┴──────────┴──────────┘" << std::endl;
    
    std::cout << "\n对齐规则说明:" << std::endl;
    std::cout << "  1. 基本类型的对齐要求通常等于其大小" << std::endl;
    std::cout << "  2. 结构体的对齐要求等于最大成员的对齐要求" << std::endl;
    std::cout << "  3. 结构体大小必须是其对齐要求的整数倍" << std::endl;
}

// 演示结构体内存布局
void demoStructLayout() {
    std::cout << "\n--- 结构体内存布局演示 ---" << std::endl;
    
    std::cout << "\n[示例] DefaultAligned 结构体:" << std::endl;
    std::cout << "struct DefaultAligned {" << std::endl;
    std::cout << "    char a;     // 1字节" << std::endl;
    std::cout << "    // 3字节填充 (为对齐int)" << std::endl;
    std::cout << "    int b;      // 4字节" << std::endl;
    std::cout << "    char c;     // 1字节" << std::endl;
    std::cout << "    // 7字节填充 (为对齐double)" << std::endl;
    std::cout << "    double d;   // 8字节" << std::endl;
    std::cout << "};" << std::endl;
    
    std::cout << "\n实际分析:" << std::endl;
    std::cout << "  sizeof(DefaultAligned) = " << sizeof(DefaultAligned) << " 字节" << std::endl;
    std::cout << "  alignof(DefaultAligned) = " << alignof(DefaultAligned) << " 字节" << std::endl;
    
    std::cout << "\n各成员偏移量:" << std::endl;
    std::cout << "  a 偏移: " << offsetof(DefaultAligned, a) << " (填充后到偏移4)" << std::endl;
    std::cout << "  b 偏移: " << offsetof(DefaultAligned, b) << " (占4-7)" << std::endl;
    std::cout << "  c 偏移: " << offsetof(DefaultAligned, c) << " (填充后到偏移16)" << std::endl;
    std::cout << "  d 偏移: " << offsetof(DefaultAligned, d) << " (占16-23)" << std::endl;
    
    // 可视化内存布局
    std::cout << "\n内存布局可视化:" << std::endl;
    std::cout << "  [+0 ] a (1B) | [填充3B] | b (4B) | c (1B) | [填充7B] | d (8B)" << std::endl;
    std::cout << "  总计: 1 + 3 + 4 + 1 + 7 + 8 = 24 字节" << std::endl;
}

// 演示alignas/alignof使用
void demoAlignasAlignof() {
    std::cout << "\n--- alignas/alignof 使用演示 ---" << std::endl;
    
    // alignof查询对齐要求
    std::cout << "\n[alignof] 查询类型对齐要求:" << std::endl;
    std::cout << "  alignof(AlignedTo16) = " << alignof(AlignedTo16) << std::endl;
    std::cout << "  alignof(CacheLineAligned) = " << alignof(CacheLineAligned) << std::endl;
    
    // alignas变量示例
    alignas(16) int alignedVar = 42;
    alignas(32) double alignedDouble = 3.14;
    
    std::cout << "\n[alignas] 变量对齐示例:" << std::endl;
    std::cout << "  alignas(16) int 变量地址: " << &alignedVar << std::endl;
    std::cout << "  地址是否16字节对齐: " 
              << ((reinterpret_cast<std::uintptr_t>(&alignedVar) % 16) == 0 ? "是" : "否") 
              << std::endl;
    std::cout << "  alignas(32) double 变量地址: " << &alignedDouble << std::endl;
    std::cout << "  地址是否32字节对齐: " 
              << ((reinterpret_cast<std::uintptr_t>(&alignedDouble) % 32) == 0 ? "是" : "否") 
              << std::endl;
    
    // 结构体大小对比
    std::cout << "\n[结构体大小对比]" << std::endl;
    std::cout << "  sizeof(AlignedTo16) = " << sizeof(AlignedTo16) << " 字节" << std::endl;
    std::cout << "  sizeof(CacheLineAligned) = " << sizeof(CacheLineAligned) << " 字节" << std::endl;
    
    std::cout << "\nalignas使用场景:" << std::endl;
    std::cout << "  1. SIMD指令要求特定对齐（如16/32字节）" << std::endl;
    std::cout << "  2. 缓存行对齐避免伪共享" << std::endl;
    std::cout << "  3. 内存映射IO要求特定对齐" << std::endl;
}

// 演示结构体优化
void demoStructOptimization() {
    std::cout << "\n--- 结构体优化演示 ---" << std::endl;
    
    std::cout << "\n[优化前] DefaultAligned:" << std::endl;
    std::cout << "  成员顺序: char, int, char, double" << std::endl;
    std::cout << "  sizeof = " << sizeof(DefaultAligned) << " 字节" << std::endl;
    
    std::cout << "\n[优化后] OptimizedAligned:" << std::endl;
    std::cout << "  成员顺序: double, int, char, char" << std::endl;
    std::cout << "  sizeof = " << sizeof(OptimizedAligned) << " 字节" << std::endl;
    
    std::cout << "\n节省内存: " << sizeof(DefaultAligned) - sizeof(OptimizedAligned) 
              << " 字节 (" 
              << 100.0 * (sizeof(DefaultAligned) - sizeof(OptimizedAligned)) / sizeof(DefaultAligned)
              << "%)" << std::endl;
    
    // 打包结构体（不推荐）
    std::cout << "\n[强制打包] PackedStruct:" << std::endl;
    std::cout << "  sizeof = " << sizeof(PackedStruct) << " 字节 (无填充)" << std::endl;
    std::cout << "  警告: 未对齐访问可能导致性能下降或崩溃！" << std::endl;
    
    std::cout << "\n结构体优化建议:" << std::endl;
    std::cout << "  1. 将大对齐要求的成员（如double）放前面" << std::endl;
    std::cout << "  2. 将相同类型的成员放在一起" << std::endl;
    std::cout << "  3. 考虑使用位域压缩bool/enum成员" << std::endl;
    std::cout << "  4. 避免使用#pragma pack(1)除非必要" << std::endl;
}

// 演示未对齐访问的性能影响
void demoUnalignedAccess() {
    std::cout << "\n--- 对齐访问性能演示 ---" << std::endl;
    
    // 创建一块内存用于测试
    char buffer[1024];
    
    // 对齐的int指针
    int* alignedPtr = reinterpret_cast<int*>(buffer);
    
    // 未对齐的int指针（偏移1字节）
    int* unalignedPtr = reinterpret_cast<int*>(buffer + 1);
    
    const int ITERATIONS = 100'000'000;
    
    std::cout << "测试 " << ITERATIONS << " 次int写入操作" << std::endl;
    
    // 测试对齐访问
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) {
        *alignedPtr = i;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto alignedTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "  对齐访问耗时: " << alignedTime << " ms" << std::endl;
    
    // 测试未对齐访问
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) {
        *unalignedPtr = i;
    }
    end = std::chrono::high_resolution_clock::now();
    auto unalignedTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "  未对齐访问耗时: " << unalignedTime << " ms" << std::endl;
    
    if (alignedTime > 0) {
        std::cout << "  性能差异: " << std::fixed << std::setprecision(1)
                  << 100.0 * (unalignedTime - alignedTime) / alignedTime << "%" << std::endl;
    }
    
    std::cout << "\n说明: 对齐访问效率更高，因为CPU只需一次内存操作" << std::endl;
    std::cout << "      未对齐访问可能需要两次或更多次内存操作" << std::endl;
}

// 主演示函数
void memoryAlignmentDemo() {
    std::cout << "╔══════════════════════════════════════╗" << std::endl;
    std::cout << "║          内存对齐演示                ║" << std::endl;
    std::cout << "║      Memory Alignment Basics         ║" << std::endl;
    std::cout << "╚══════════════════════════════════════╝" << std::endl;
    
    printBasicAlignments();
    demoStructLayout();
    demoAlignasAlignof();
    demoStructOptimization();
    demoUnalignedAccess();
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "内存对齐要点总结:" << std::endl;
    std::cout << "  1. 对齐要求: 数据地址必须是其大小的整数倍" << std::endl;
    std::cout << "  2. alignof: 查询类型的对齐要求" << std::endl;
    std::cout << "  3. alignas: 指定自定义对齐要求" << std::endl;
    std::cout << "  4. 结构体优化: 合理排序成员减少填充" << std::endl;
    std::cout << "========================================" << std::endl;
}
