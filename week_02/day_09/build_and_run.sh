#!/bin/bash

# Day 9 构建和运行脚本
# 主题：链表操作技巧与智能指针进阶

set -e

echo "========================================"
echo "  Day 9: 链表操作技巧与智能指针进阶"
echo "========================================"
echo ""

# 创建构建目录
BUILD_DIR="build"
if [ -d "$BUILD_DIR" ]; then
    echo "清理旧的构建目录..."
    rm -rf "$BUILD_DIR"
fi

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# 配置和构建
echo ""
echo "正在配置项目..."
cmake .. -DCMAKE_BUILD_TYPE=Debug

echo ""
echo "正在编译..."
cmake --build . -j$(nproc)

# 运行
echo ""
echo "========================================"
echo "  运行结果"
echo "========================================"
./day_09_demo

echo ""
echo "========================================"
echo "  构建完成！"
echo "========================================"
