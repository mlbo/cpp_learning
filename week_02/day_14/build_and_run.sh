#!/bin/bash

# Day 14: 第二周复习与综合练习 - 构建运行脚本

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${DAY14_BUILD_DIR:-${SCRIPT_DIR}/build}"
BUILD_TYPE="${BUILD_TYPE:-Debug}"

echo "=========================================="
echo "  Day 14: 第二周复习与综合练习"
echo "=========================================="
echo ""

# CMake配置
echo "[1/3] 配置CMake..."
cmake -S "$SCRIPT_DIR" -B "$BUILD_DIR" \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DENABLE_SANITIZERS="${ENABLE_SANITIZERS:-OFF}"

# 编译
echo "[2/3] 编译项目..."
cmake --build "$BUILD_DIR" --parallel "$(nproc 2>/dev/null || echo 4)"

echo ""
echo "=========================================="
echo "  编译成功！"
echo "=========================================="
echo ""

# 可直接传入 all/test/main，便于自动化运行；不传参数时显示菜单。
mode="${1:-}"
if [ "$mode" = "all" ]; then
    echo "[3/3] 运行 CTest 和全部演示..."
    ctest --test-dir "$BUILD_DIR" --output-on-failure
    "$BUILD_DIR/day14_main" --all
    exit 0
elif [ "$mode" = "test" ]; then
    echo "[3/3] 运行 CTest..."
    ctest --test-dir "$BUILD_DIR" --output-on-failure
    exit 0
elif [ "$mode" = "main" ]; then
    echo "[3/3] 运行主演示..."
    "$BUILD_DIR/day14_main" --all
    exit 0
fi

# 运行选项
echo "可运行的程序:"
echo "  1. day14_main            - 主程序（运行所有演示）"
echo "  2. test_thread_safe_list - 线程安全链表测试"
echo "  3. test_leetcode234      - 回文链表测试"
echo "  4. test_leetcode138      - 随机链表复制测试"
echo "  5. 运行所有测试"
echo "  6. 退出"
echo ""

read -p "请选择要运行的程序 [1-6]: " choice

case $choice in
    1)
        echo ""
        echo ">>> 运行主程序..."
        echo ""
        "$BUILD_DIR/day14_main"
        ;;
    2)
        echo ""
        echo ">>> 运行线程安全链表测试..."
        echo ""
        "$BUILD_DIR/test_thread_safe_list"
        ;;
    3)
        echo ""
        echo ">>> 运行回文链表测试..."
        echo ""
        "$BUILD_DIR/test_leetcode234"
        ;;
    4)
        echo ""
        echo ">>> 运行随机链表复制测试..."
        echo ""
        "$BUILD_DIR/test_leetcode138"
        ;;
    5)
        echo ""
        echo ">>> 运行所有测试..."
        echo ""
        ctest --test-dir "$BUILD_DIR" --output-on-failure
        ;;
    6)
        echo "退出。"
        exit 0
        ;;
    *)
        echo "无效选择，运行主程序..."
        "$BUILD_DIR/day14_main"
        ;;
esac

echo ""
echo "=========================================="
echo "  执行完毕！"
echo "=========================================="
