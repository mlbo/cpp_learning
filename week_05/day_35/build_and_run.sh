#!/bin/bash

# ========================================
# Day 35: 35天学习总结 编译运行脚本
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
echo -e "${BLUE}  Day 35: 35天学习总结 编译运行${NC}"
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
./day35_main

echo -e "${GREEN}========================================${NC}"
echo -e "${GREEN}  恭喜完成35天C++学习之旅！${NC}"
echo -e "${GREEN}========================================${NC}"
