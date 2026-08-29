/**
 * CPU缓存演示：输出当前程序的实验现象，不把常见硬件参数写成语言保证。
 */

#include "cpu_cache_demo.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <thread>
#include <vector>

namespace {

using Clock = std::chrono::steady_clock;
using Nanoseconds = std::chrono::nanoseconds;

volatile std::uint64_t benchmarkSink = 0;

Nanoseconds median(std::vector<Nanoseconds> samples) {
    std::sort(samples.begin(), samples.end());
    return samples[samples.size() / 2];
}

template<typename Counter>
Nanoseconds runCounterPair(Counter& counters, std::uint64_t iterations) {
    counters.count1.store(0, std::memory_order_relaxed);
    counters.count2.store(0, std::memory_order_relaxed);
    const auto start = Clock::now();
    std::thread first([&] {
        for (std::uint64_t i = 0; i < iterations; ++i) {
            counters.count1.fetch_add(1, std::memory_order_relaxed);
        }
    });
    std::thread second([&] {
        for (std::uint64_t i = 0; i < iterations; ++i) {
            counters.count2.fetch_add(1, std::memory_order_relaxed);
        }
    });
    first.join();
    second.join();
    benchmarkSink = counters.count1.load(std::memory_order_relaxed)
                  + counters.count2.load(std::memory_order_relaxed);
    return std::chrono::duration_cast<Nanoseconds>(Clock::now() - start);
}

template<typename Function>
Nanoseconds benchmarkMedian(Function&& function, std::size_t repeats = 7) {
    function(); // 预热：触发页映射、分支与代码缓存等一次性成本
    std::vector<Nanoseconds> samples;
    samples.reserve(repeats);
    for (std::size_t repeat = 0; repeat < repeats; ++repeat) {
        const auto start = Clock::now();
        benchmarkSink = function();
        samples.push_back(std::chrono::duration_cast<Nanoseconds>(Clock::now() - start));
    }
    return median(std::move(samples));
}

} // namespace

void printCacheInfo() {
    std::cout << "\n--- 缓存层次结构 ---\n";
    std::cout << "  常见机器有多级缓存，但容量、共享方式、缓存行大小和延迟都依CPU型号而变。\n";
    std::cout << "  精确参数应读取操作系统/处理器资料或使用硬件计数器，本演示不硬编码周期数。\n";
    std::cout << "  后续alignas(" << DEMO_CACHE_LINE_BYTES << ")只是一个常见布局实验参数。\n";
}

void demoCacheLine() {
    std::cout << "\n--- 缓存行概念演示 ---\n";
    std::array<int, 16> values{};
    const auto first = reinterpret_cast<std::uintptr_t>(&values[0]);
    const auto middle = reinterpret_cast<std::uintptr_t>(&values[8]);

    std::cout << "  数组对象大小 = " << sizeof(values) << " 字节。\n";
    std::cout << "  即使大小碰巧为64字节，也可能因起始地址未落在64字节边界而跨越两个64字节区间。\n";
    std::cout << "  values[0] 与 values[8] 地址差 = " << middle - first << " 字节。\n";
    const bool sameIllustrativeBlock = first / DEMO_CACHE_LINE_BYTES == middle / DEMO_CACHE_LINE_BYTES;
    std::cout << "  按‘64字节区间’这个演示模型，它们"
              << (sameIllustrativeBlock ? "落在同一区间" : "落在不同区间") << "。\n";
    std::cout << "  地址差小于64并不能证明同一缓存行；还必须知道边界和真实硬件缓存行大小。\n";
}

void demoFalseSharing() {
    std::cout << "\n--- 伪共享实验 ---\n";
    constexpr std::uint64_t iterations = 2'000'000;
    constexpr std::size_t repeats = 5;
    CounterBad adjacent;
    CounterGood separated;

    runCounterPair(adjacent, 10'000);
    runCounterPair(separated, 10'000);

    std::vector<Nanoseconds> adjacentSamples;
    std::vector<Nanoseconds> separatedSamples;
    for (std::size_t repeat = 0; repeat < repeats; ++repeat) {
        adjacentSamples.push_back(runCounterPair(adjacent, iterations));
        separatedSamples.push_back(runCounterPair(separated, iterations));
    }

    const auto adjacentMedian = median(std::move(adjacentSamples));
    const auto separatedMedian = median(std::move(separatedSamples));
    std::cout << "  每个线程执行相同的 " << iterations << " 次 relaxed 原子自增，预热后重复 "
              << repeats << " 次并取中位数。\n";
    std::cout << "  相邻原子中位数: "
              << std::chrono::duration<double, std::milli>(adjacentMedian).count() << " ms\n";
    std::cout << "  alignas(64)分隔原子中位数: "
              << std::chrono::duration<double, std::milli>(separatedMedian).count() << " ms\n";
    std::cout << "  分隔只是在常见64字节缓存行机器上的候选方案；调度、拓扑和真实缓存行大小都会改变结果。\n";
}

void demoCacheHitRate() {
    std::cout << "\n--- 顺序与跨步等量工作实验 ---\n";
    constexpr std::size_t elementCount = 4 * 1024 * 1024;
    constexpr std::size_t stride = 257;
    std::vector<std::uint32_t> data(elementCount, 1);

    const auto sequential = [&]() {
        std::uint64_t sum = 0;
        for (std::uint32_t value : data) {
            sum += value;
        }
        return sum;
    };

    const auto strided = [&]() {
        std::uint64_t sum = 0;
        for (std::size_t offset = 0; offset < stride; ++offset) {
            for (std::size_t index = offset; index < data.size(); index += stride) {
                sum += data[index];
            }
        }
        return sum;
    };

    const auto sequentialTime = benchmarkMedian(sequential);
    const auto stridedTime = benchmarkMedian(strided);
    if (sequential() != strided()) {
        std::cout << "  [错误] 两种遍历没有处理相同元素。\n";
        return;
    }

    std::cout << "  两种遍历都恰好读取 " << elementCount << " 个元素，预热后各重复7次取中位数。\n";
    std::cout << "  顺序访问中位数: "
              << std::chrono::duration<double, std::milli>(sequentialTime).count() << " ms\n";
    std::cout << "  跨步访问中位数: "
              << std::chrono::duration<double, std::milli>(stridedTime).count() << " ms\n";
    std::cout << "  benchmarkSink=" << benchmarkSink << " 防止编译器删除求和；结果仅适用于当前机器和编译选项。\n";
}

void cpuCacheDemo() {
    std::cout << "╔══════════════════════════════════════╗\n";
    std::cout << "║          CPU缓存实验                 ║\n";
    std::cout << "╚══════════════════════════════════════╝\n";
    printCacheInfo();
    demoCacheLine();
    demoFalseSharing();
    demoCacheHitRate();
    std::cout << "\n结论：局部性和伪共享是优化线索，必须在Release构建上用当前机器的重复测量验证。\n";
}
