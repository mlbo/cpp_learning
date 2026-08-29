#!/bin/bash

# Day 11 构建脚本
# 使用方法: ./build_and_run.sh [target]

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${DAY11_BUILD_DIR:-${SCRIPT_DIR}/build}"
BUILD_TYPE="${BUILD_TYPE:-Debug}"
TARGET="${1:-all}"

echo "========================================="
echo "  Day 11: Pimpl模式与链表专题"
echo "========================================="
echo ""

# 创建构建目录
if [ ! -d "$BUILD_DIR" ]; then
    echo "[INFO] 创建构建目录..."
    mkdir -p "$BUILD_DIR"
fi

# 配置CMake
echo "[INFO] 配置CMake..."
cmake -S "$SCRIPT_DIR" -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE="$BUILD_TYPE"

# 编译
echo "[INFO] 编译项目..."
cmake --build "$BUILD_DIR" -j"$(nproc 2>/dev/null || echo 4)"

echo ""
echo "========================================="
echo "  编译完成！"
echo "========================================="
echo ""

run_program() {
    local prog=$1
    local name=$2
    shift 2
    echo ""
    echo ">>> 运行 $name <<<"
    echo "-----------------------------------------"
    if [ -x "$BUILD_DIR/$prog" ]; then
        "$BUILD_DIR/$prog" "$@"
    else
        echo "[ERROR] 程序不存在: $prog"
        exit 1
    fi
    echo "-----------------------------------------"
}

run_ctest() {
    echo "[INFO] 运行CTest..."
    ctest --test-dir "$BUILD_DIR" --output-on-failure
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
        run_program "day_11_main" "主程序非交互演示" --all
        run_ctest
        ;;
    test)
        run_ctest
        ;;
    *)
        echo "用法: $0 [main|item22|leetcode23|leetcode61|all|test]"
        echo ""
        echo "可用目标:"
        echo "  main       - 运行主程序"
        echo "  item22     - 运行EMC++条款22示例"
        echo "  leetcode23 - 运行LeetCode 23测试"
        echo "  leetcode61 - 运行LeetCode 61测试"
        echo "  all        - 非交互运行主演示和全部CTest"
        echo "  test       - 运行CTest测试"
        return 1
        ;;
esac

echo ""
echo "[完成] Day 11 练习结束"
