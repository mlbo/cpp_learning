#!/bin/bash

# Day 4: nullptr详解与双指针算法
# 构建和运行脚本

set -e  # 遇到错误立即退出

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

echo "=========================================="
echo "  Day 4: nullptr详解与双指针算法"
echo "=========================================="
echo ""

# 清理旧的构建目录
if [ -d "$BUILD_DIR" ]; then
    echo "🧹 清理旧的构建目录..."
    rm -rf "$BUILD_DIR"
fi

# 创建构建目录
echo "📁 创建构建目录..."
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# 配置CMake
echo ""
echo "⚙️  配置CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Debug

# 编译
echo ""
echo "🔨 编译项目..."
cmake --build . -j$(nproc)

# 运行主程序
echo ""
echo "=========================================="
echo "🚀 运行主程序"
echo "=========================================="
echo ""
./day04_main

# 可选：运行单独的测试
echo ""
echo "=========================================="
echo "🧪 运行LeetCode测试"
echo "=========================================="
echo ""
echo ">>> LeetCode 11: 盛最多水的容器"
./day04_test_leetcode11
echo ""
echo ">>> LeetCode 15: 三数之和"
./day04_test_leetcode15

echo ""
echo "=========================================="
echo "✅ 构建和运行完成！"
echo "=========================================="
