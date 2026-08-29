#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

echo "Day 5: 配置 Release 构建"
cmake -S "${SCRIPT_DIR}" -B "${BUILD_DIR}" -DCMAKE_BUILD_TYPE=Release

echo "Day 5: 编译全部目标"
cmake --build "${BUILD_DIR}" --parallel

echo "Day 5: 运行 constexpr 边界和滑动窗口测试"
ctest --test-dir "${BUILD_DIR}" --output-on-failure
