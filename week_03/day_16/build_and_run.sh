#!/bin/bash

# ========================================
# Day 16: 队列入门与Lambda进阶 编译运行脚本
# ========================================

set -e

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}  Day 16: 队列入门与Lambda进阶${NC}"
echo -e "${BLUE}========================================${NC}"

echo -e "${YELLOW}[1/4] 清理构建目录...${NC}"
rm -rf "${BUILD_DIR}"
mkdir -p "${BUILD_DIR}"

echo -e "${YELLOW}[2/4] CMake 配置...${NC}"
cd "${BUILD_DIR}"
cmake ..

echo -e "${YELLOW}[3/4] 编译项目...${NC}"
make -j$(nproc)

echo -e "${YELLOW}[4/4] 运行程序...${NC}"
echo -e "${GREEN}----------------------------------------${NC}"
echo -e "${GREEN}  运行主程序${NC}"
echo -e "${GREEN}----------------------------------------${NC}"
./day16_main

echo -e "${GREEN}========================================${NC}"
echo -e "${GREEN}  Day 16 学习完成！${NC}"
echo -e "${GREEN}========================================${NC}"
