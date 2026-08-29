#!/bin/bash

set -e

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}  Day 26: CPU缓存与内存对齐${NC}"
echo -e "${BLUE}========================================${NC}"

echo -e "${YELLOW}[1/4] 清理构建目录...${NC}"
rm -rf "${BUILD_DIR}"
mkdir -p "${BUILD_DIR}"

echo -e "${YELLOW}[2/4] CMake 配置...${NC}"
cd "${BUILD_DIR}"
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_TESTING=ON \
    -DBUILD_TESTS=ON \
    -DENABLE_SANITIZERS="${ENABLE_SANITIZERS:-OFF}" \
    -DENABLE_ASAN="${ENABLE_ASAN:-OFF}" \
    -DENABLE_UBSAN="${ENABLE_UBSAN:-OFF}"

echo -e "${YELLOW}[3/4] 编译项目...${NC}"
make -j$(nproc)

echo -e "${YELLOW}[4/4] 运行程序...${NC}"
echo -e "${GREEN}----------------------------------------${NC}"
ctest --output-on-failure
./day26_main

echo -e "${GREEN}========================================${NC}"
echo -e "${GREEN}  Day 26 学习完成！${NC}"
echo -e "${GREEN}  下节预告：字符串操作、匹配算法与滑动窗口${NC}"
echo -e "${GREEN}========================================${NC}"
