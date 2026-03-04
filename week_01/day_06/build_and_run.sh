#!/bin/bash

# Day 6: 二分查找详解 - 构建和运行脚本

set -e  # 遇到错误立即退出

echo "=========================================="
echo "  Day 6: 二分查找详解 - 构建系统"
echo "=========================================="
echo ""

# 获取脚本所在目录
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

# 创建构建目录
echo "📁 创建构建目录..."
mkdir -p "${BUILD_DIR}"

# 进入构建目录
cd "${BUILD_DIR}"

# 运行CMake配置
echo ""
echo "🔧 配置项目 (CMake)..."
cmake ..

# 编译项目
echo ""
echo "🔨 编译项目..."
make -j$(nproc)

echo ""
echo "=========================================="
echo "  编译成功！"
echo "=========================================="
echo ""

# 运行程序
echo "🚀 运行主程序..."
echo ""
echo "--- 二分查找演示 ---"
./bin/binary_search_demo

echo ""
echo "--- LeetCode 704 测试 ---"
./bin/test_0704

echo ""
echo "--- LeetCode 34 测试 ---"
./bin/test_0034

echo ""
echo "=========================================="
echo "  所有测试完成！"
echo "=========================================="
