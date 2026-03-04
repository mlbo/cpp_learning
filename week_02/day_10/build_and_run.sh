#!/bin/bash

# Day 10: weak_ptr详解与链表双指针技巧
# 构建和运行脚本

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

echo "========================================="
echo "  Day 10: weak_ptr详解与链表双指针技巧"
echo "========================================="
echo ""

# 清理并创建构建目录
echo "[1/3] 准备构建环境..."
rm -rf "${BUILD_DIR}"
mkdir -p "${BUILD_DIR}"

# 配置CMake
echo "[2/3] 配置CMake..."
cd "${BUILD_DIR}"
cmake .. -DCMAKE_BUILD_TYPE=Debug

# 构建
echo "[3/3] 编译项目..."
cmake --build . -- -j$(nproc 2>/dev/null || echo 4)

# 运行
echo ""
echo "========================================="
echo "  运行 Day 10 示例"
echo "========================================="
./day_10_demo

echo ""
echo "✅ Day 10 完成!"
