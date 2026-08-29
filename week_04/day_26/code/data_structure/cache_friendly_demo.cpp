/**
 * 缓存友好编程微基准：等量工作、预热、重复测量、中位数和防优化。
 */

#include "cache_friendly_demo.h"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <list>
#include <numeric>
#include <random>
#include <vector>

namespace {

using Clock = std::chrono::steady_clock;
using Nanoseconds = std::chrono::nanoseconds;

volatile std::int64_t benchmarkSink = 0;

template<typename Function>
Nanoseconds measureMedian(Function&& function, std::size_t repeats = 5) {
    benchmarkSink = function();
    std::vector<Nanoseconds> samples;
    samples.reserve(repeats);
    for (std::size_t repeat = 0; repeat < repeats; ++repeat) {
        const auto start = Clock::now();
        benchmarkSink = function();
        samples.push_back(std::chrono::duration_cast<Nanoseconds>(Clock::now() - start));
    }
    std::sort(samples.begin(), samples.end());
    return samples[samples.size() / 2];
}

void printDuration(const char* label, Nanoseconds duration) {
    std::cout << "  " << label << ": " << std::fixed << std::setprecision(3)
              << std::chrono::duration<double, std::milli>(duration).count() << " ms\n";
}

} // namespace

void demoArrayVsList() {
    std::cout << "\n--- vector vs list 等量遍历 ---\n";
    constexpr std::size_t elementCount = 500'000;
    std::vector<int> contiguous(elementCount);
    std::iota(contiguous.begin(), contiguous.end(), 0);
    std::list<int> linked(contiguous.begin(), contiguous.end());

    const auto traverseVector = [&]() {
        std::int64_t sum = 0;
        for (int value : contiguous) {
            sum += value;
        }
        return sum;
    };
    const auto traverseList = [&]() {
        std::int64_t sum = 0;
        for (int value : linked) {
            sum += value;
        }
        return sum;
    };

    const auto vectorTime = measureMedian(traverseVector);
    const auto listTime = measureMedian(traverseList);
    std::cout << "  两者都读取 " << elementCount << " 个int并做相同求和。\n";
    printDuration("vector中位数", vectorTime);
    printDuration("list中位数", listTime);
    std::cout << "  差异同时包含布局、指针追踪和编译器优化效果，不能只归因于缓存。\n";
}

void demoLoopOptimization() {
    std::cout << "\n--- 行优先与列优先等量遍历 ---\n";
    constexpr std::size_t side = 1024;
    std::vector<int> matrix(side * side, 1);

    const auto rowMajor = [&]() {
        std::int64_t sum = 0;
        for (std::size_t row = 0; row < side; ++row) {
            for (std::size_t column = 0; column < side; ++column) {
                sum += matrix[row * side + column];
            }
        }
        return sum;
    };
    const auto columnMajor = [&]() {
        std::int64_t sum = 0;
        for (std::size_t column = 0; column < side; ++column) {
            for (std::size_t row = 0; row < side; ++row) {
                sum += matrix[row * side + column];
            }
        }
        return sum;
    };

    const auto rowTime = measureMedian(rowMajor);
    const auto columnTime = measureMedian(columnMajor);
    std::cout << "  两种顺序都访问 " << side * side << " 个元素一次。\n";
    printDuration("行优先中位数", rowTime);
    printDuration("列优先中位数", columnTime);
}

void demoDataLocality() {
    std::cout << "\n--- 顺序与随机等量读取 ---\n";
    constexpr std::size_t elementCount = 1'000'000;
    std::vector<int> data(elementCount);
    std::iota(data.begin(), data.end(), 0);
    std::vector<std::size_t> order(elementCount);
    std::iota(order.begin(), order.end(), 0);
    std::mt19937 generator(20260725); // 固定种子使课程实验可复现
    std::shuffle(order.begin(), order.end(), generator);

    const auto sequential = [&]() {
        std::int64_t sum = 0;
        for (int value : data) {
            sum += value;
        }
        return sum;
    };
    const auto random = [&]() {
        std::int64_t sum = 0;
        for (std::size_t index : order) {
            sum += data[index];
        }
        return sum;
    };

    const auto sequentialTime = measureMedian(sequential);
    const auto randomTime = measureMedian(random);
    std::cout << "  两者都读取每个元素恰好一次；随机索引表在计时前生成。\n";
    printDuration("顺序读取中位数", sequentialTime);
    printDuration("随机读取中位数", randomTime);
    std::cout << "  随机版本还多一次索引读取，因此结果体现的是整个访问方案，而非纯缓存延迟。\n";
}

void demoLoopTiling() {
    std::cout << "\n--- 矩阵乘法分块实验 ---\n";
    constexpr std::size_t side = 192;
    constexpr std::size_t block = 32;
    const std::size_t cells = side * side;
    std::vector<int> left(cells, 1);
    std::vector<int> right(cells, 1);
    std::vector<int> plain(cells, 0);
    std::vector<int> tiled(cells, 0);

    const auto multiplyPlain = [&]() {
        std::fill(plain.begin(), plain.end(), 0);
        for (std::size_t row = 0; row < side; ++row) {
            for (std::size_t column = 0; column < side; ++column) {
                int sum = 0;
                for (std::size_t inner = 0; inner < side; ++inner) {
                    sum += left[row * side + inner] * right[inner * side + column];
                }
                plain[row * side + column] = sum;
            }
        }
        return static_cast<std::int64_t>(plain[cells / 2]);
    };

    const auto multiplyTiled = [&]() {
        std::fill(tiled.begin(), tiled.end(), 0);
        for (std::size_t rowBlock = 0; rowBlock < side; rowBlock += block) {
            for (std::size_t innerBlock = 0; innerBlock < side; innerBlock += block) {
                for (std::size_t columnBlock = 0; columnBlock < side; columnBlock += block) {
                    for (std::size_t row = rowBlock; row < std::min(rowBlock + block, side); ++row) {
                        for (std::size_t inner = innerBlock; inner < std::min(innerBlock + block, side); ++inner) {
                            const int leftValue = left[row * side + inner];
                            for (std::size_t column = columnBlock; column < std::min(columnBlock + block, side); ++column) {
                                tiled[row * side + column] += leftValue * right[inner * side + column];
                            }
                        }
                    }
                }
            }
        }
        return static_cast<std::int64_t>(tiled[cells / 2]);
    };

    const auto plainTime = measureMedian(multiplyPlain, 3U);
    const auto tiledTime = measureMedian(multiplyTiled, 3U);
    const bool sameResult = plain == tiled;
    std::cout << "  两种算法执行相同数量的乘加并包含各自结果清零，块大小32只是候选参数。\n";
    printDuration("普通循环中位数", plainTime);
    printDuration("分块循环中位数", tiledTime);
    std::cout << "  结果一致性: " << (sameResult ? "通过" : "失败") << '\n';
}

void demoDataStructureChoice() {
    std::cout << "\n--- 数据结构选择 ---\n";
    std::cout << "  连续布局常有利于遍历，但list在稳定迭代器和常数时间拼接等需求上仍有价值。\n";
    std::cout << "  先从需求与复杂度选择结构，再用剖析确认热点，最后用代表性数据做Release基准。\n";
    std::cout << "  SoA、AoS、内存池和冷热分离都是候选设计，不是无需测量的固定答案。\n";
}

void cacheFriendlyDemo() {
    std::cout << "╔══════════════════════════════════════╗\n";
    std::cout << "║        缓存友好编程微基准            ║\n";
    std::cout << "╚══════════════════════════════════════╝\n";
    std::cout << "  请使用Release构建；每项先预热，再重复测量并报告中位数。\n";
    demoArrayVsList();
    demoLoopOptimization();
    demoDataLocality();
    demoLoopTiling();
    demoDataStructureChoice();
    std::cout << "\n  benchmarkSink=" << benchmarkSink << "，用于阻止无用计算被整体删除。\n";
    std::cout << "  所有数字只对当前机器、编译器、选项、负载和本次运行有效。\n";
}
