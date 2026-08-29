#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

echo "Day 4: 配置 Release 构建"
cmake -S "${SCRIPT_DIR}" -B "${BUILD_DIR}" -DCMAKE_BUILD_TYPE=Release

echo "Day 4: 编译全部目标"
cmake --build "${BUILD_DIR}" --parallel

echo "Day 4: 运行 nullptr、双指针演示和算法用例"
ctest --test-dir "${BUILD_DIR}" --output-on-failure
