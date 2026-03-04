#!/bin/bash

# Day 2: 数组与decltype学习演示 - 编译运行脚本

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

echo "========================================"
echo "Day 2: 数组与decltype学习演示"
echo "========================================"
echo ""

# 创建构建目录
mkdir -p "${BUILD_DIR}"

# 配置CMake
echo ">>> 配置CMake..."
cd "${BUILD_DIR}"
cmake ..

# 编译
echo ""
echo ">>> 编译项目..."
make -j$(nproc)

# 运行
echo ""
echo ">>> 运行程序..."
echo ""
./bin/day_02_demo

echo ""
echo "========================================"
echo "Day 2 学习完成！"
echo "========================================"
