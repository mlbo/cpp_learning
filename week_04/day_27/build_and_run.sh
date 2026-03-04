#!/bin/bash

# Day 27: 字符串专题 - 构建和运行脚本

set -e  # 遇到错误时退出

echo "========================================="
echo "  Day 27: 字符串专题"
echo "========================================="

# 颜色定义
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# 获取脚本所在目录
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
BUILD_DIR="${SCRIPT_DIR}/build"

echo -e "${BLUE}[1/3] 清理并创建构建目录...${NC}"
rm -rf "${BUILD_DIR}"
mkdir -p "${BUILD_DIR}"

echo -e "${BLUE}[2/3] 配置CMake...${NC}"
cd "${BUILD_DIR}"
cmake ..

echo -e "${BLUE}[3/3] 编译项目...${NC}"
make -j$(nproc)

echo ""
echo -e "${GREEN}=========================================${NC}"
echo -e "${GREEN}  编译成功！${NC}"
echo -e "${GREEN}=========================================${NC}"
echo ""

# 运行主程序
echo -e "${YELLOW}[运行] 主程序演示${NC}"
echo "-----------------------------------------"
./day_27_demo

echo ""
echo -e "${YELLOW}[运行] 字符串操作演示${NC}"
echo "-----------------------------------------"
./string_operations_demo

echo ""
echo -e "${YELLOW}[运行] 滑动窗口演示${NC}"
echo "-----------------------------------------"
./sliding_window_demo

echo ""
echo -e "${YELLOW}[运行] 字符串匹配演示${NC}"
echo "-----------------------------------------"
./string_matching_demo

echo ""
echo -e "${YELLOW}[运行] LeetCode 76 题解${NC}"
echo "-----------------------------------------"
./lc0076_minimum_window

echo ""
echo -e "${YELLOW}[运行] LeetCode 567 题解${NC}"
echo "-----------------------------------------"
./lc0567_permutation_in_string

echo ""
echo -e "${GREEN}=========================================${NC}"
echo -e "${GREEN}  Day 27 学习完成！${NC}"
echo -e "${GREEN}=========================================${NC}"
