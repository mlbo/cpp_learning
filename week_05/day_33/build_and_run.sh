#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

echo "[1/3] 配置 C++17 Release 严格告警构建"
rm -rf "${BUILD_DIR}"
cmake -S "${SCRIPT_DIR}" -B "${BUILD_DIR}" \
    -DCMAKE_BUILD_TYPE=Release

echo "[2/3] 构建全部 Day 33 源文件"
cmake --build "${BUILD_DIR}" --parallel "$(nproc)"

echo "[3/3] 运行全部 CTest"
ctest --test-dir "${BUILD_DIR}" --output-on-failure
