/**
 * @file hash_table_review.cpp
 * @brief 哈希表综合复习示例
 * 
 * 涵盖内容：
 * 1. 哈希表基本操作
 * 2. 哈希函数设计
 * 3. 自定义类型的哈希支持
 * 4. 常见问题与最佳实践
 */

#include "simple_hash_table.h"

#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <chrono>

namespace {

using BenchmarkClock = std::chrono::steady_clock;
using BenchmarkDuration = std::chrono::nanoseconds;

volatile std::int64_t hashBenchmarkSink = 0;

template<typename Function>
BenchmarkDuration measureMedian(Function&& function, std::size_t repeats = 5) {
    hashBenchmarkSink = function();
    std::vector<BenchmarkDuration> samples;
    samples.reserve(repeats);
    for (std::size_t repeat = 0; repeat < repeats; ++repeat) {
        const auto start = BenchmarkClock::now();
        hashBenchmarkSink = function();
        samples.push_back(std::chrono::duration_cast<BenchmarkDuration>(
            BenchmarkClock::now() - start));
    }
    std::sort(samples.begin(), samples.end());
    return samples[samples.size() / 2];
}

void printDuration(const char* label, BenchmarkDuration duration) {
    std::cout << "  " << label << ": " << std::fixed << std::setprecision(3)
              << std::chrono::duration<double, std::milli>(duration).count()
              << " ms\n";
}

} // namespace

namespace hash_table_review {

// ========================================
// 自定义类型的哈希支持
// ========================================

/**
 * @brief 二维点结构体
 * 演示如何为自定义类型提供哈希支持
 */
struct Point {
    int x;
    int y;
    
    Point(int x_, int y_) : x(x_), y(y_) {}
    
    // 必须提供相等比较运算符
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
    
    // 用于打印
    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        return os << "(" << p.x << ", " << p.y << ")";
    }
};

/**
 * @brief Point的哈希函数
 * 使用标准的哈希组合技术
 */
struct PointHash {
    std::size_t operator()(const Point& p) const noexcept {
        // 使用异或和移位组合两个坐标的哈希值
        // 这是一个简单但有效的哈希组合方法
        std::size_t h1 = std::hash<int>{}(p.x);
        std::size_t h2 = std::hash<int>{}(p.y);
        return h1 ^ (h2 << 1);  // 移位避免对称冲突
    }
};

// ========================================
// 演示函数
// ========================================

void demonstrateBasicOperations() {
    std::cout << "=== 基本操作演示 ===\n\n";
    
    // 创建哈希表
    std::unordered_map<std::string, int> scores;
    
    // 插入元素
    std::cout << "插入元素:\n";
    scores["Alice"] = 95;
    scores["Bob"] = 87;
    scores["Charlie"] = 92;
    scores.emplace("David", 88);  // 使用emplace原地构造
    
    std::cout << "  当前元素数量: " << scores.size() << "\n";
    std::cout << "  桶数量: " << scores.bucket_count() << "\n";
    std::cout << "  装载因子: " << scores.load_factor() << "\n";
    
    // 查找元素
    std::cout << "\n查找元素:\n";
    std::string name = "Bob";
    auto it = scores.find(name);
    if (it != scores.end()) {
        std::cout << "  找到 " << name << ": " << it->second << "\n";
    }
    
    // 使用at()访问（会检查边界）
    try {
        std::cout << "  Alice的分数: " << scores.at("Alice") << "\n";
    } catch (const std::out_of_range& e) {
        std::cout << "  键不存在\n";
    }
    
    // 删除元素
    std::cout << "\n删除元素:\n";
    scores.erase("Bob");
    std::cout << "  删除Bob后，元素数量: " << scores.size() << "\n";
    
    // 遍历（注意：顺序不确定）
    std::cout << "\n遍历所有元素（顺序不确定）:\n";
    for (const auto& pair : scores) {
        std::cout << "  " << pair.first << ": " << pair.second << "\n";
    }
}

void demonstrateCustomTypeHash() {
    std::cout << "\n=== 自定义类型的哈希支持 ===\n\n";
    
    // 使用自定义哈希函数
    std::unordered_map<Point, std::string, PointHash> pointNames;
    
    pointNames[Point(0, 0)] = "原点";
    pointNames[Point(1, 0)] = "单位点X";
    pointNames[Point(0, 1)] = "单位点Y";
    pointNames[Point(3, 4)] = "特殊点";
    
    std::cout << "存储的点:\n";
    for (const auto& pair : pointNames) {
        std::cout << "  " << pair.first << " -> " << pair.second << "\n";
    }
    
    // 查找测试
    Point searchPoint(3, 4);
    auto it = pointNames.find(searchPoint);
    if (it != pointNames.end()) {
        std::cout << "\n找到 " << searchPoint << ": " << it->second << "\n";
    }
}

void demonstrateSimpleHashTable() {
    std::cout << "\n=== 简单哈希表实现演示 ===\n\n";
    
    SimpleHashTable<std::string, int> table(8);  // 初始容量8
    
    // 插入一些数据
    std::cout << "插入数据...\n";
    for (int i = 0; i < 20; ++i) {
        table.insert("key_" + std::to_string(i), i * 10);
    }
    
    table.printStats();
    
    // 查找测试
    std::cout << "\n查找测试:\n";
    int* val = table.find("key_5");
    if (val) {
        std::cout << "  找到 key_5: " << *val << "\n";
    }
    
    val = table.find("key_100");
    if (!val) {
        std::cout << "  key_100 不存在\n";
    }
    
    // 删除测试
    std::cout << "\n删除测试:\n";
    if (table.erase("key_10")) {
        std::cout << "  删除 key_10 成功\n";
    }
    
    table.printStats();
}

void demonstrateBestPractices() {
    std::cout << "\n=== 最佳实践演示 ===\n\n";
    
    // 1. 预分配容量
    std::cout << "1. 预分配容量避免频繁rehash:\n";
    std::unordered_map<int, std::string> map1;
    map1.reserve(10000);  // 预分配足够容量
    std::cout << "  预分配后桶数量: " << map1.bucket_count() << "\n";
    
    // 2. 自定义哈希函数的注意事项
    std::cout << "\n2. 好的哈希函数应该:\n";
    std::cout << "  - 计算效率高\n";
    std::cout << "  - 分布均匀\n";
    std::cout << "  - 确定性（相同输入=相同输出）\n";
    
    // 3. 使用emplace避免临时对象
    std::cout << "\n3. 使用emplace提高效率:\n";
    std::unordered_map<std::string, std::vector<int>> map2;
    map2.emplace("key1", std::vector<int>{1, 2, 3});  // 原地构造
    std::cout << "  emplace完成，vector大小: " << map2["key1"].size() << "\n";
    
    // 4. 使用unordered_set去重
    std::cout << "\n4. 使用unordered_set快速去重:\n";
    std::vector<int> data = {1, 2, 3, 2, 4, 3, 5, 1, 6};
    std::unordered_set<int> uniqueSet(data.begin(), data.end());
    std::cout << "  原始数据: ";
    for (int x : data) std::cout << x << " ";
    std::cout << "\n  去重后: ";
    for (int x : uniqueSet) std::cout << x << " ";
    std::cout << "\n";
}

void demonstratePerformance() {
    std::cout << "\n=== unordered_map 与 map 观察性微基准 ===\n\n";

    constexpr std::size_t elementCount = 50'000;
    std::vector<int> keys(elementCount);
    std::iota(keys.begin(), keys.end(), 0);
    std::mt19937 generator(20260726);
    std::shuffle(keys.begin(), keys.end(), generator);

    const auto buildHashMap = [&keys]() {
        std::unordered_map<int, int> values;
        values.reserve(keys.size());
        for (int key : keys) {
            values.emplace(key, key);
        }
        return static_cast<std::int64_t>(values.size());
    };
    const auto buildOrderedMap = [&keys]() {
        std::map<int, int> values;
        for (int key : keys) {
            values.emplace(key, key);
        }
        return static_cast<std::int64_t>(values.size());
    };

    std::unordered_map<int, int> hashMap;
    hashMap.reserve(keys.size());
    std::map<int, int> orderedMap;
    for (int key : keys) {
        hashMap.emplace(key, key);
        orderedMap.emplace(key, key);
    }

    const auto lookupHashMap = [&]() {
        std::int64_t sum = 0;
        for (int key : keys) {
            const auto found = hashMap.find(key);
            if (found != hashMap.end()) {
                sum += found->second;
            }
        }
        return sum;
    };
    const auto lookupOrderedMap = [&]() {
        std::int64_t sum = 0;
        for (int key : keys) {
            const auto found = orderedMap.find(key);
            if (found != orderedMap.end()) {
                sum += found->second;
            }
        }
        return sum;
    };

    const BenchmarkDuration hashBuildTime = measureMedian(buildHashMap);
    const BenchmarkDuration orderedBuildTime = measureMedian(buildOrderedMap);
    const BenchmarkDuration hashLookupTime = measureMedian(lookupHashMap);
    const BenchmarkDuration orderedLookupTime = measureMedian(lookupOrderedMap);

    std::cout << "  两种容器使用同一组固定种子随机键；每项预热后重复5次取中位数。\n";
    std::cout << "  unordered_map 构建路径包含 reserve，构建测量也包含容器销毁。\n";
    printDuration("unordered_map 构建中位数", hashBuildTime);
    printDuration("map 构建中位数", orderedBuildTime);
    printDuration("unordered_map find中位数", hashLookupTime);
    printDuration("map find中位数", orderedLookupTime);
    std::cout << "  lookup 用 find 并累加真实值，benchmarkSink=" << hashBenchmarkSink
              << " 防止结果被整体删除。\n";
    std::cout << "  结果只描述当前机器、标准库、键分布与编译选项，"
                 "不能替代平均/最坏复杂度分析。\n";
}

void demonstrate() {
    std::cout << "╔════════════════════════════════════════╗\n";
    std::cout << "║        哈希表综合复习                  ║\n";
    std::cout << "╚════════════════════════════════════════╝\n\n";
    
    demonstrateBasicOperations();
    demonstrateCustomTypeHash();
    demonstrateSimpleHashTable();
    demonstrateBestPractices();
    demonstratePerformance();
    
    std::cout << "\n哈希表复习完成！\n";
}

} // namespace hash_table_review
