#!/bin/bash

# Day 1 编译运行脚本

set -e

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}  Day 1: 复杂度分析 + auto类型推导${NC}"
echo -e "${BLUE}========================================${NC}"

# 清理
rm -rf "${BUILD_DIR}"
mkdir -p "${BUILD_DIR}"

# CMake
cd "${BUILD_DIR}"
cmake .. 

# 编译
make -j$(nproc)

# 运行
echo -e "${GREEN}----------------------------------------${NC}"
echo -e "${GREEN}  运行主程序${NC}"
echo -e "${GREEN}----------------------------------------${NC}"
./day01_main

echo -e "${GREEN}========================================${NC}"
echo -e "${GREEN}  Day 1 完成！${NC}"
echo -e "${GREEN}========================================${NC}"
