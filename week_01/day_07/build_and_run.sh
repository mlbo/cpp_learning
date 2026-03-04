#!/bin/bash

# Day 7 构建脚本
# 使用方法: ./build_and_run.sh [target]

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

echo "========================================="
echo "  Day 7: 第一周复习与综合练习"
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
        run_program "day_07_main" "主程序"
        ;;
    leetcode42|42)
        run_program "leetcode_42" "LeetCode 42: 接雨水"
        ;;
    leetcode189|189)
        run_program "leetcode_189" "LeetCode 189: 轮转数组"
        ;;
    all)
        run_program "day_07_main" "主程序"
        run_program "leetcode_42" "LeetCode 42: 接雨水"
        run_program "leetcode_189" "LeetCode 189: 轮转数组"
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
        echo "用法: $0 [main|leetcode42|leetcode189|all|test|clean]"
        echo ""
        echo "可用目标:"
        echo "  main       - 运行主程序"
        echo "  leetcode42 - 运行LeetCode 42测试"
        echo "  leetcode189- 运行LeetCode 189测试"
        echo "  all        - 运行所有程序"
        echo "  test       - 运行CTest测试"
        echo "  clean      - 清理构建目录"
        ;;
esac

echo ""
echo "[完成] Day 7 练习结束"
