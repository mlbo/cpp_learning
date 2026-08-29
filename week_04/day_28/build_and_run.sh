#!/bin/bash

# ========================================
# Day 28: 第四周复习 编译运行脚本
# ========================================

set -e  # 遇错即停

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 项目目录
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}  Day 28: 第四周复习 编译运行${NC}"
echo -e "${BLUE}========================================${NC}"

# 清理旧的build目录
echo -e "${YELLOW}[1/4] 清理构建目录...${NC}"
rm -rf "${BUILD_DIR}"
mkdir -p "${BUILD_DIR}"

# CMake配置
echo -e "${YELLOW}[2/4] CMake 配置...${NC}"
cd "${BUILD_DIR}"
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_TESTING=ON \
    -DENABLE_SANITIZERS="${ENABLE_SANITIZERS:-OFF}" \
    -DENABLE_ASAN="${ENABLE_ASAN:-OFF}" \
    -DENABLE_UBSAN="${ENABLE_UBSAN:-OFF}"

# 编译
echo -e "${YELLOW}[3/4] 编译并测试项目...${NC}"
cmake --build . --parallel "$(nproc)"
ctest --output-on-failure

# 运行
echo -e "${YELLOW}[4/4] 运行程序...${NC}"
echo -e "${GREEN}========================================${NC}"
echo -e "${GREEN}  运行综合主程序${NC}"
echo -e "${GREEN}========================================${NC}"
./day_28_main

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}  LeetCode 测试结果${NC}"
echo -e "${BLUE}========================================${NC}"

echo -e "${YELLOW}LC 146 LRU缓存测试:${NC}"
./day28_lc0146

echo ""
echo -e "${YELLOW}LC 460 LFU缓存测试:${NC}"
./day28_lc0460

echo -e "${GREEN}========================================${NC}"
echo -e "${GREEN}  Day 28 所有程序运行完成！${NC}"
echo -e "${GREEN}========================================${NC}"
