/**
 * 缓存友好编程演示
 * 展示如何编写缓存友好的代码
 */

#include "cache_friendly_demo.h"
#include <iostream>
#include <chrono>
#include <random>
#include <iomanip>
#include <algorithm>

// 演示数组vs链表的缓存性能
void demoArrayVsList() {
    std::cout << "\n--- 数组 vs 链表缓存性能对比 ---" << std::endl;
    
    const int N = 1'000'000;
    
    // 准备数据
    std::vector<int> arr(N);
    std::list<int> lst;
    
    for (int i = 0; i < N; ++i) {
        arr[i] = i;
        lst.push_back(i);
    }
    
    volatile long long sum = 0;
    
    // 数组遍历（连续内存，缓存友好）
    std::cout << "数据量: " << N << " 个元素" << std::endl;
    
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        sum += arr[i];
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto arrTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    
    std::cout << "  数组遍历耗时: " << arrTime << " μs" << std::endl;
    
    // 链表遍历（离散内存，缓存不友好）
    start = std::chrono::high_resolution_clock::now();
    for (int val : lst) {
        sum += val;
    }
    end = std::chrono::high_resolution_clock::now();
    auto lstTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    
    std::cout << "  链表遍历耗时: " << lstTime << " μs" << std::endl;
    
    if (arrTime > 0) {
        std::cout << "  数组性能优势: " << std::fixed << std::setprecision(1)
                  << 100.0 * (lstTime - arrTime) / lstTime << "%" << std::endl;
    }
    
    std::cout << "\n原因分析:" << std::endl;
    std::cout << "  - 数组内存连续，缓存行预取有效" << std::endl;
    std::cout << "  - 链表节点分散，每次访问可能缓存缺失" << std::endl;
}

// 演示循环访问模式优化
void demoLoopOptimization() {
    std::cout << "\n--- 循环访问模式优化 ---" << std::endl;
    
    constexpr int SIZE = 1024;
    std::vector<std::vector<int>> matrix(SIZE, std::vector<int>(SIZE, 1));
    
    volatile long long sum = 0;
    
    std::cout << "矩阵大小: " << SIZE << "x" << SIZE << std::endl;
    
    // 按行遍历（缓存友好）
    std::cout << "\n[按行遍历] - 缓存友好" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            sum += matrix[i][j];
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto rowTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "  耗时: " << rowTime << " ms" << std::endl;
    
    // 按列遍历（缓存不友好）
    std::cout << "\n[按列遍历] - 缓存不友好" << std::endl;
    start = std::chrono::high_resolution_clock::now();
    for (int j = 0; j < SIZE; ++j) {
        for (int i = 0; i < SIZE; ++i) {
            sum += matrix[i][j];
        }
    }
    end = std::chrono::high_resolution_clock::now();
    auto colTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "  耗时: " << colTime << " ms" << std::endl;
    
    if (rowTime > 0) {
        std::cout << "\n按行遍历性能优势: " << std::fixed << std::setprecision(1)
                  << 100.0 * (colTime - rowTime) / colTime << "%" << std::endl;
    }
    
    std::cout << "\n原因: C++中二维数组按行存储" << std::endl;
    std::cout << "      按行遍历时，连续访问同一行的元素，缓存命中率高" << std::endl;
    std::cout << "      按列遍历时，跳跃访问，缓存行利用率低" << std::endl;
}

// 演示数据局部性原理
void demoDataLocality() {
    std::cout << "\n--- 数据局部性原理演示 ---" << std::endl;
    
    const int SIZE = 1'000'000;
    std::vector<int> data(SIZE);
    
    // 初始化数据
    for (int i = 0; i < SIZE; ++i) {
        data[i] = i;
    }
    
    volatile long long sum = 0;
    
    std::cout << "测试数据量: " << SIZE << " 个int元素 (" << SIZE * sizeof(int) / 1024.0 / 1024.0 << " MB)" << std::endl;
    
    // 时间局部性演示：重复访问相同数据
    std::cout << "\n[时间局部性] 重复访问相同数据" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    for (int iter = 0; iter < 10; ++iter) {
        for (int i = 0; i < SIZE / 10; ++i) {
            sum += data[i];
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto timeLocality = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "  耗时: " << timeLocality << " μs" << std::endl;
    
    // 空间局部性演示：连续访问
    std::cout << "\n[空间局部性] 连续访问" << std::endl;
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < SIZE; ++i) {
        sum += data[i];
    }
    end = std::chrono::high_resolution_clock::now();
    auto spaceLocality = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "  耗时: " << spaceLocality << " μs" << std::endl;
    
    // 随机访问（违反局部性）
    std::cout << "\n[随机访问] 违反局部性" << std::endl;
    std::vector<int> indices(SIZE);
    for (int i = 0; i < SIZE; ++i) indices[i] = i;
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(indices.begin(), indices.end(), g);
    
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < SIZE; ++i) {
        sum += data[indices[i]];
    }
    end = std::chrono::high_resolution_clock::now();
    auto randomAccess = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "  耗时: " << randomAccess << " μs" << std::endl;
    
    if (spaceLocality > 0) {
        std::cout << "\n随机访问性能损失: " << std::fixed << std::setprecision(1)
                  << 100.0 * (randomAccess - spaceLocality) / randomAccess << "%" << std::endl;
    }
    
    std::cout << "\n局部性原理总结:" << std::endl;
    std::cout << "  1. 时间局部性: 最近访问的数据很可能再次被访问" << std::endl;
    std::cout << "  2. 空间局部性: 访问某地址后很可能访问附近地址" << std::endl;
    std::cout << "  3. 优化建议: 保持访问模式的连续性和规律性" << std::endl;
}

// 演示循环分块技术
void demoLoopTiling() {
    std::cout << "\n--- 循环分块技术演示 ---" << std::endl;
    
    constexpr int SIZE = 512;
    constexpr int BLOCK_SIZE = 32;  // 块大小，适合缓存
    
    std::vector<std::vector<int>> matrixA(SIZE, std::vector<int>(SIZE, 1));
    std::vector<std::vector<int>> matrixB(SIZE, std::vector<int>(SIZE, 1));
    std::vector<std::vector<int>> result(SIZE, std::vector<int>(SIZE, 0));
    
    std::cout << "矩阵乘法: " << SIZE << "x" << SIZE << std::endl;
    
    // 普通矩阵乘法
    std::cout << "\n[普通矩阵乘法]" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            int sum = 0;
            for (int k = 0; k < SIZE; ++k) {
                sum += matrixA[i][k] * matrixB[k][j];
            }
            result[i][j] = sum;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto normalTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "  耗时: " << normalTime << " ms" << std::endl;
    
    // 分块矩阵乘法
    std::cout << "\n[分块矩阵乘法] 块大小=" << BLOCK_SIZE << std::endl;
    // 重置结果矩阵
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            result[i][j] = 0;
        }
    }
    
    start = std::chrono::high_resolution_clock::now();
    for (int i0 = 0; i0 < SIZE; i0 += BLOCK_SIZE) {
        for (int j0 = 0; j0 < SIZE; j0 += BLOCK_SIZE) {
            for (int k0 = 0; k0 < SIZE; k0 += BLOCK_SIZE) {
                // 处理块内的计算
                for (int i = i0; i < std::min(i0 + BLOCK_SIZE, SIZE); ++i) {
                    for (int j = j0; j < std::min(j0 + BLOCK_SIZE, SIZE); ++j) {
                        int sum = 0;
                        for (int k = k0; k < std::min(k0 + BLOCK_SIZE, SIZE); ++k) {
                            sum += matrixA[i][k] * matrixB[k][j];
                        }
                        result[i][j] += sum;
                    }
                }
            }
        }
    }
    end = std::chrono::high_resolution_clock::now();
    auto tiledTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "  耗时: " << tiledTime << " ms" << std::endl;
    
    if (normalTime > 0 && tiledTime > 0) {
        std::cout << "\n分块优化性能提升: " << std::fixed << std::setprecision(1)
                  << 100.0 * (normalTime - tiledTime) / normalTime << "%" << std::endl;
    }
    
    std::cout << "\n分块技术原理:" << std::endl;
    std::cout << "  - 将大数据集分成小块，使每块能放入缓存" << std::endl;
    std::cout << "  - 减少缓存缺失，提高数据复用率" << std::endl;
}

// 演示数据结构选择对缓存的影响
void demoDataStructureChoice() {
    std::cout << "\n--- 数据结构选择对缓存的影响 ---" << std::endl;
    
    std::cout << "\n缓存友好数据结构特点:" << std::endl;
    std::cout << "  1. 连续内存存储（如数组、vector）" << std::endl;
    std::cout << "  2. 紧凑的数据布局" << std::endl;
    std::cout << "  3. 可预测的访问模式" << std::endl;
    
    std::cout << "\n缓存不友好数据结构特点:" << std::endl;
    std::cout << "  1. 指针链接的节点（如链表、树）" << std::endl;
    std::cout << "  2. 分散的内存分配" << std::endl;
    std::cout << "  3. 随机访问模式" << std::endl;
    
    std::cout << "\n优化建议:" << std::endl;
    std::cout << "  1. 性能敏感场景优先使用连续内存结构" << std::endl;
    std::cout << "  2. 对于链表结构，可使用内存池集中分配" << std::endl;
    std::cout << "  3. 考虑数据的冷热分离，热数据紧凑存放" << std::endl;
    std::cout << "  4. 使用SoA（结构数组）替代AoS（数组结构）" << std::endl;
}

// 主演示函数
void cacheFriendlyDemo() {
    std::cout << "╔══════════════════════════════════════╗" << std::endl;
    std::cout << "║        缓存友好编程演示              ║" << std::endl;
    std::cout << "║     Cache-Friendly Programming       ║" << std::endl;
    std::cout << "╚══════════════════════════════════════╝" << std::endl;
    
    demoArrayVsList();
    demoLoopOptimization();
    demoDataLocality();
    demoLoopTiling();
    demoDataStructureChoice();
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "缓存友好编程要点总结:" << std::endl;
    std::cout << "  1. 数据结构选择: 优先连续内存（数组/vector）" << std::endl;
    std::cout << "  2. 访问模式优化: 顺序访问优于随机访问" << std::endl;
    std::cout << "  3. 循环优化: 内层循环访问最连续维度" << std::endl;
    std::cout << "  4. 分块技术: 大数据集分块处理" << std::endl;
    std::cout << "  5. 局部性原理: 利用时间和空间局部性" << std::endl;
    std::cout << "========================================" << std::endl;
}
