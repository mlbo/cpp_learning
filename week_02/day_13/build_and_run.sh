#!/bin/bash

# Day 13 链表综合练习 - 编译运行脚本

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${DAY13_BUILD_DIR:-${SCRIPT_DIR}/build}"
BUILD_TYPE="${BUILD_TYPE:-Debug}"

echo "========================================"
echo "Day 13: 链表综合练习"
echo "========================================"

# 配置和编译
echo ""
echo "[1/3] 配置 CMake..."
cmake -S "$SCRIPT_DIR" -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE="$BUILD_TYPE"

echo ""
echo "[2/3] 编译项目..."
cmake --build "$BUILD_DIR" --parallel "$(nproc 2>/dev/null || echo 4)"

echo ""
echo "[3/3] 运行程序..."
echo ""
echo "========================================"
echo "运行 CTest（主程序 smoke + LeetCode 测试）"
echo "========================================"
ctest --test-dir "$BUILD_DIR" --output-on-failure

echo ""
echo "========================================"
echo "所有测试通过!"
echo "========================================"
