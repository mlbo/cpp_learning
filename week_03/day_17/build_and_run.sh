#!/bin/bash

set -euo pipefail

GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${1:-${SCRIPT_DIR}/build}"
if (( $# > 0 )); then
    shift
fi

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}  Day 17: 单调栈与function/bind${NC}"
echo -e "${BLUE}========================================${NC}"

echo -e "${YELLOW}[1/4] 准备构建目录: ${BUILD_DIR}${NC}"

echo -e "${YELLOW}[2/4] CMake 配置...${NC}"
cmake -S "${SCRIPT_DIR}" -B "${BUILD_DIR}" -DCMAKE_BUILD_TYPE=Release "$@"

echo -e "${YELLOW}[3/4] 编译项目...${NC}"
cmake --build "${BUILD_DIR}" --parallel

echo -e "${YELLOW}[4/4] 运行测试...${NC}"
echo -e "${GREEN}----------------------------------------${NC}"
ctest --test-dir "${BUILD_DIR}" --output-on-failure

echo -e "${GREEN}========================================${NC}"
echo -e "${GREEN}  Day 17 学习完成！${NC}"
echo -e "${GREEN}========================================${NC}"
