#!/bin/bash

# Day 9 构建和运行脚本
# 主题：链表操作技巧与智能指针进阶

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${1:-${SCRIPT_DIR}/build}"

echo "========================================"
echo "  Day 9: 链表操作技巧与智能指针进阶"
echo "========================================"
echo ""

# 配置和构建
echo ""
echo "正在配置项目..."
cmake -S "${SCRIPT_DIR}" -B "${BUILD_DIR}" -DCMAKE_BUILD_TYPE=Debug

echo ""
echo "正在编译..."
cmake --build "${BUILD_DIR}" --parallel "$(nproc 2>/dev/null || echo 4)"

echo ""
echo "正在运行测试..."
ctest --test-dir "${BUILD_DIR}" --output-on-failure

# 运行
echo ""
echo "========================================"
echo "  运行结果"
echo "========================================"
"${BUILD_DIR}/day_09_demo"

echo ""
echo "========================================"
echo "  构建完成！"
echo "========================================"
