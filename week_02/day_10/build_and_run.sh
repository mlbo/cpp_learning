#!/bin/bash

# Day 10: weak_ptr详解与链表双指针技巧
# 构建和运行脚本

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${1:-${SCRIPT_DIR}/build}"

echo "========================================="
echo "  Day 10: weak_ptr详解与链表双指针技巧"
echo "========================================="
echo ""

# 配置CMake
echo "[1/3] 配置CMake..."
cmake -S "${SCRIPT_DIR}" -B "${BUILD_DIR}" -DCMAKE_BUILD_TYPE=Debug

# 构建
echo "[2/3] 编译项目..."
cmake --build "${BUILD_DIR}" --parallel "$(nproc 2>/dev/null || echo 4)"

echo "[3/3] 运行测试..."
ctest --test-dir "${BUILD_DIR}" --output-on-failure

# 运行
echo ""
echo "========================================="
echo "  运行 Day 10 示例"
echo "========================================="
"${BUILD_DIR}/day_10_demo"

echo ""
echo "✅ Day 10 完成!"
