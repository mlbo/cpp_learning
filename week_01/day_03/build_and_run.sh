#!/bin/bash

# Day 3: C++11 统一初始化与初始化列表
# 构建和运行脚本

set -e  # 遇到错误立即退出

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

echo "========================================"
echo "  Day 3: C++11 统一初始化与初始化列表"
echo "========================================"
echo ""

# 清理旧的构建目录（可选）
if [ "$1" == "clean" ]; then
    echo "清理构建目录..."
    rm -rf "${BUILD_DIR}"
fi

# 创建构建目录
mkdir -p "${BUILD_DIR}"

# 进入构建目录
cd "${BUILD_DIR}"

# 配置 CMake
echo ""
echo ">>> 配置 CMake..."
cmake -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
      ..

# 编译
echo ""
echo ">>> 编译项目..."
make -j$(nproc)

# 运行
echo ""
echo ">>> 运行程序..."
echo "========================================"
echo ""
./day03_tutorial

echo ""
echo "========================================"
echo "  构建和运行完成！"
echo "========================================"
