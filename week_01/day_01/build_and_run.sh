#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

echo "Day 1: 配置 Release 构建"
cmake -S "${SCRIPT_DIR}" -B "${BUILD_DIR}" \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_EXTENDED_EXAMPLES=ON

echo "Day 1: 编译全部目标"
cmake --build "${BUILD_DIR}" --parallel

echo "Day 1: 运行主程序、算法/边界测试和扩展示例"
ctest --test-dir "${BUILD_DIR}" --output-on-failure
