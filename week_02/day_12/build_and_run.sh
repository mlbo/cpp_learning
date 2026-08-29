#!/bin/bash

# Day 12 构建运行脚本
# 使用方法: ./build_and_run.sh [all|test|main] [--asan]

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${DAY12_BUILD_DIR:-${SCRIPT_DIR}/build}"
BUILD_TYPE="${BUILD_TYPE:-Debug}"

echo "=========================================="
echo "  Day 12: 智能指针总结 & 底层内存管理"
echo "=========================================="

# 解析运行模式和 Sanitizer 选项
mode="${1:-main}"
asan_arg="${2:-}"
if [ "$mode" = "--asan" ]; then
    mode="main"
    asan_arg="--asan"
fi

echo "[1/3] 配置 CMake..."
if [ "$asan_arg" = "--asan" ]; then
    echo "    启用 AddressSanitizer 和 UndefinedBehaviorSanitizer..."
    cmake -S "$SCRIPT_DIR" -B "$BUILD_DIR" \
        -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -DENABLE_SANITIZERS=ON
else
    cmake -S "$SCRIPT_DIR" -B "$BUILD_DIR" \
        -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -DENABLE_SANITIZERS=OFF
fi

# 编译
echo "[2/3] 编译项目..."
cmake --build "$BUILD_DIR" --parallel "$(nproc 2>/dev/null || echo 4)"

echo ""
echo "=========================================="
echo "  编译成功！开始运行..."
echo "=========================================="
echo ""

# 运行程序
if [ "$mode" = "test" ]; then
    echo "[3/3] 运行 CTest..."
    ctest --test-dir "$BUILD_DIR" --output-on-failure
elif [ "$mode" = "all" ]; then
    echo "[3/3] 运行 CTest 和全部演示..."
    ctest --test-dir "$BUILD_DIR" --output-on-failure
    "$BUILD_DIR/day_12" --all
else
    echo "[3/3] 运行主演示..."
    "$BUILD_DIR/day_12"
fi

echo ""
echo "=========================================="
echo "  程序执行完毕"
echo "=========================================="
