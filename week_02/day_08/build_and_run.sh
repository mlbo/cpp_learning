#!/bin/bash

# Day 8 构建脚本
# 主题：链表数据结构与unique_ptr智能指针

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${1:-${SCRIPT_DIR}/build}"

echo "========================================"
echo "  Day 8: 链表数据结构与unique_ptr"
echo "========================================"
echo ""

# 配置CMake
echo "[1/3] 配置CMake..."
cmake -S "${SCRIPT_DIR}" -B "${BUILD_DIR}" \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# 编译
echo "[2/3] 编译项目..."
cmake --build "${BUILD_DIR}" --parallel "$(nproc 2>/dev/null || echo 4)"

echo "[3/3] 运行测试..."
ctest --test-dir "${BUILD_DIR}" --output-on-failure

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
echo "  ${BUILD_DIR}/day_08_demo"
echo "  或运行所有程序: cmake --build ${BUILD_DIR} --target run_all"
