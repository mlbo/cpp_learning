#!/bin/bash

# Day 12 构建运行脚本
# 使用方法: ./build_and_run.sh [--asan]

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

echo "=========================================="
echo "  Day 12: 智能指针总结 & 底层内存管理"
echo "=========================================="

# 清理旧的构建目录
if [ -d "$BUILD_DIR" ]; then
    echo "[1/4] 清理旧的构建目录..."
    rm -rf "$BUILD_DIR"
fi

# 创建构建目录
echo "[2/4] 创建构建目录..."
mkdir -p "$BUILD_DIR"

# 配置 CMake
echo "[3/4] 配置 CMake..."
cd "$BUILD_DIR"
if [ "$1" == "--asan" ]; then
    echo "    启用 AddressSanitizer..."
    cmake -DENABLE_ASAN=ON ..
else
    cmake ..
fi

# 编译
echo "[4/4] 编译项目..."
make -j$(nproc)

echo ""
echo "=========================================="
echo "  编译成功！开始运行..."
echo "=========================================="
echo ""

# 运行程序
./day_12

echo ""
echo "=========================================="
echo "  程序执行完毕"
echo "=========================================="
