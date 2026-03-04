#!/bin/bash

# Day 8 构建脚本
# 主题：链表数据结构与unique_ptr智能指针

set -e  # 遇到错误立即退出

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

echo "========================================"
echo "  Day 8: 链表数据结构与unique_ptr"
echo "========================================"
echo ""

# 创建构建目录
echo "[1/3] 创建构建目录..."
mkdir -p "${BUILD_DIR}"

# 配置CMake
echo "[2/3] 配置CMake..."
cd "${BUILD_DIR}"
cmake .. \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# 编译
echo "[3/3] 编译项目..."
cmake --build . -j$(nproc)

echo ""
echo "========================================"
echo "  编译完成！"
echo "========================================"
echo ""
echo "可执行文件列表："
echo "  - unique_ptr_demo      : unique_ptr基本用法演示"
echo "  - unique_ptr_advanced  : unique_ptr高级特性"
echo "  - make_unique_demo     : make_unique用法"
echo "  - item17_special_members: EMC++条款17演示"
echo "  - item18_unique_ptr    : EMC++条款18演示"
echo "  - leetcode_0203        : LeetCode 203题测试"
echo "  - leetcode_0206        : LeetCode 206题测试"
echo "  - day_08_demo          : 主程序演示"
echo ""
echo "运行方式："
echo "  cd ${BUILD_DIR} && ./day_08_demo"
echo "  或运行所有程序: cd ${BUILD_DIR} && make run_all"
echo ""

# 询问是否运行
read -p "是否运行所有演示程序？(y/n) " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
    echo ""
    cd "${BUILD_DIR}"
    make run_all
fi
