#!/bin/bash

# Day 11 构建脚本
# 使用方法: ./build_and_run.sh [target]

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

echo "========================================="
echo "  Day 11: Pimpl模式与链表专题"
echo "========================================="
echo ""

# 创建构建目录
if [ ! -d "$BUILD_DIR" ]; then
    echo "[INFO] 创建构建目录..."
    mkdir -p "$BUILD_DIR"
fi

# 进入构建目录
cd "$BUILD_DIR"

# 配置CMake
echo "[INFO] 配置CMake..."
cmake -DCMAKE_BUILD_TYPE=Debug ..

# 编译
echo "[INFO] 编译项目..."
cmake --build . -j$(nproc 2>/dev/null || echo 4)

echo ""
echo "========================================="
echo "  编译完成！"
echo "========================================="
echo ""

# 运行目标
TARGET=${1:-all}

run_program() {
    local prog=$1
    local name=$2
    echo ""
    echo ">>> 运行 $name <<<"
    echo "-----------------------------------------"
    if [ -f "$BUILD_DIR/$prog" ]; then
        "$BUILD_DIR/$prog"
    else
        echo "[ERROR] 程序不存在: $prog"
    fi
    echo "-----------------------------------------"
}

case $TARGET in
    main)
        run_program "day_11_main" "主程序"
        ;;
    item22)
        run_program "item22_pimpl" "EMC++条款22示例"
        ;;
    leetcode23|23)
        run_program "leetcode_23" "LeetCode 23: 合并K个升序链表"
        ;;
    leetcode61|61)
        run_program "leetcode_61" "LeetCode 61: 旋转链表"
        ;;
    all)
        run_program "day_11_main" "主程序"
        run_program "item22_pimpl" "EMC++条款22示例"
        run_program "leetcode_23" "LeetCode 23: 合并K个升序链表"
        run_program "leetcode_61" "LeetCode 61: 旋转链表"
        ;;
    test)
        echo "[INFO] 运行测试..."
        ctest --output-on-failure
        ;;
    clean)
        echo "[INFO] 清理构建目录..."
        rm -rf "$BUILD_DIR"
        echo "[INFO] 清理完成"
        ;;
    *)
        echo "用法: $0 [main|item22|leetcode23|leetcode61|all|test|clean]"
        echo ""
        echo "可用目标:"
        echo "  main       - 运行主程序"
        echo "  item22     - 运行EMC++条款22示例"
        echo "  leetcode23 - 运行LeetCode 23测试"
        echo "  leetcode61 - 运行LeetCode 61测试"
        echo "  all        - 运行所有程序"
        echo "  test       - 运行CTest测试"
        echo "  clean      - 清理构建目录"
        ;;
esac

echo ""
echo "[完成] Day 11 练习结束"
