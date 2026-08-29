#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

cmake -S "${SCRIPT_DIR}" -B "${BUILD_DIR}" \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_EXTENDED_EXAMPLES=ON
cmake --build "${BUILD_DIR}" --target day01_algorithm_tests --parallel
ctest --test-dir "${BUILD_DIR}" --output-on-failure -R '^Day01Algorithms$'
