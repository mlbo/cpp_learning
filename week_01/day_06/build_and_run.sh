#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

echo "Day 6: 配置 Release 构建"
cmake -S "${SCRIPT_DIR}" -B "${BUILD_DIR}" -DCMAKE_BUILD_TYPE=Release

echo "Day 6: 编译全部目标"
cmake --build "${BUILD_DIR}" --parallel

echo "Day 6: 运行半开区间演示和两道二分测试"
ctest --test-dir "${BUILD_DIR}" --output-on-failure
