/**
 * 内存对齐演示：只执行标准允许的对象访问。
 */

#include "memory_alignment_demo.h"

#include <array>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <iostream>

namespace {

template<typename T>
void printTypeLayout(const char* name) {
    std::cout << "  " << std::left << std::setw(12) << name
              << " sizeof=" << std::setw(3) << sizeof(T)
              << " alignof=" << alignof(T) << '\n';
}

} // namespace

void printBasicAlignments() {
    std::cout << "\n--- 基本类型的实际大小与对齐 ---\n";
    printTypeLayout<char>("char");
    printTypeLayout<short>("short");
    printTypeLayout<int>("int");
    printTypeLayout<long>("long");
    printTypeLayout<long long>("long long");
    printTypeLayout<float>("float");
    printTypeLayout<double>("double");
    printTypeLayout<void*>("void*");
    std::cout << "  对齐要求由类型和目标ABI决定，不应写成‘总等于sizeof(T)’。\n";
}

void demoStructLayout() {
    std::cout << "\n--- 结构体内存布局演示 ---\n";
    std::cout << "  sizeof(DefaultAligned) = " << sizeof(DefaultAligned) << '\n';
    std::cout << "  alignof(DefaultAligned) = " << alignof(DefaultAligned) << '\n';
    std::cout << "  成员偏移: a=" << offsetof(DefaultAligned, a)
              << ", b=" << offsetof(DefaultAligned, b)
              << ", c=" << offsetof(DefaultAligned, c)
              << ", d=" << offsetof(DefaultAligned, d) << '\n';
    std::cout << "  填充来自各成员真实alignof要求；上述数字只描述本次编译目标。\n";
}

void demoAlignasAlignof() {
    std::cout << "\n--- alignas/alignof 使用演示 ---\n";
    alignas(16) int alignedInt = 42;
    alignas(32) double alignedDouble = 3.14;

    const auto intAddress = reinterpret_cast<std::uintptr_t>(&alignedInt);
    const auto doubleAddress = reinterpret_cast<std::uintptr_t>(&alignedDouble);
    std::cout << "  alignas(16) int 地址满足16字节边界: "
              << (intAddress % 16 == 0 ? "是" : "否") << '\n';
    std::cout << "  alignas(32) double 地址满足32字节边界: "
              << (doubleAddress % 32 == 0 ? "是" : "否") << '\n';
    std::cout << "  alignof(AlignedTo16) = " << alignof(AlignedTo16) << '\n';
    std::cout << "  alignof(CacheLineAligned) = " << alignof(CacheLineAligned)
              << "（64只是此类型选择的实验参数）\n";
}

void demoStructOptimization() {
    std::cout << "\n--- 结构体成员排序演示 ---\n";
    std::cout << "  原顺序 sizeof(DefaultAligned) = " << sizeof(DefaultAligned) << '\n';
    std::cout << "  重排后 sizeof(OptimizedAligned) = " << sizeof(OptimizedAligned) << '\n';
    if (sizeof(OptimizedAligned) < sizeof(DefaultAligned)) {
        std::cout << "  本目标ABI上，按对齐需求分组减少了填充。\n";
    } else {
        std::cout << "  本目标ABI上没有变小；成员排序是待测量的布局决策，不是定律。\n";
    }
    std::cout << "  强制pack会产生潜在未对齐成员，除非协议布局确有要求且访问方式经过验证，否则不要使用。\n";
}

void demoSafeByteAccess() {
    std::cout << "\n--- 合法的字节观察与memcpy演示 ---\n";

    const int source = 0x01020304;
    std::array<unsigned char, sizeof(source)> bytes{};
    std::memcpy(bytes.data(), &source, sizeof(source));

    std::cout << "  int的对象表示（十六进制，字节顺序由当前平台决定）:" << std::right;
    for (unsigned char byte : bytes) {
        std::cout << ' ' << std::hex << std::setw(2) << std::setfill('0')
                  << static_cast<unsigned int>(byte);
    }
    std::cout << std::dec << std::setfill(' ') << '\n';

    int restored = 0;
    std::memcpy(&restored, bytes.data(), sizeof(restored));
    std::cout << "  memcpy回一个已经存在的int对象，恢复值 = 0x"
              << std::hex << restored << std::dec << '\n';

    alignas(int) std::array<unsigned char, sizeof(int)> rawStorage{};
    std::cout << "  alignas(int)字节存储地址满足int对齐: "
              << (reinterpret_cast<std::uintptr_t>(rawStorage.data()) % alignof(int) == 0 ? "是" : "否")
              << '\n';
    std::cout << "  但只有对齐并不会自动在其中创建int对象，所以本例绝不把它reinterpret_cast成int*解引用。\n";
    std::cout << "  若要读取协议字节，优先memcpy到已存在对象；若要手工管理对象生命周期，应使用placement new等正式构造手段。\n";
}

void memoryAlignmentDemo() {
    std::cout << "╔══════════════════════════════════════╗\n";
    std::cout << "║          内存对齐安全演示            ║\n";
    std::cout << "╚══════════════════════════════════════╝\n";
    printBasicAlignments();
    demoStructLayout();
    demoAlignasAlignof();
    demoStructOptimization();
    demoSafeByteAccess();
    std::cout << "\n结论：查询真实alignof、保持对象生命周期有效，并用memcpy进行合法字节复制。\n";
}
