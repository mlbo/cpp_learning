#!/bin/bash

# Day 14: 第二周复习与综合练习 - 构建运行脚本

set -e  # 遇到错误即退出

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

echo "=========================================="
echo "  Day 14: 第二周复习与综合练习"
echo "=========================================="
echo ""

# 清理旧的构建目录
if [ -d "$BUILD_DIR" ]; then
    echo "[1/4] 清理旧构建目录..."
    rm -rf "$BUILD_DIR"
fi

# 创建构建目录
echo "[2/4] 创建构建目录..."
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# CMake配置
echo "[3/4] 配置CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Debug

# 编译
echo "[4/4] 编译项目..."
make -j$(nproc)

echo ""
echo "=========================================="
echo "  编译成功！"
echo "=========================================="
echo ""

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
        ./day14_main
        ;;
    2)
        echo ""
        echo ">>> 运行线程安全链表测试..."
        echo ""
        ./test_thread_safe_list
        ;;
    3)
        echo ""
        echo ">>> 运行回文链表测试..."
        echo ""
        ./test_leetcode234
        ;;
    4)
        echo ""
        echo ">>> 运行随机链表复制测试..."
        echo ""
        ./test_leetcode138
        ;;
    5)
        echo ""
        echo ">>> 运行所有测试..."
        echo ""
        ctest --output-on-failure
        ;;
    6)
        echo "退出。"
        exit 0
        ;;
    *)
        echo "无效选择，运行主程序..."
        ./day14_main
        ;;
esac

echo ""
echo "=========================================="
echo "  执行完毕！"
echo "=========================================="
