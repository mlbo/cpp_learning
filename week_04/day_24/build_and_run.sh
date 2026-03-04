#!/bin/bash
# Day 24: 通用引用 (Universal Reference)
# 构建和运行脚本

set -e  # 遇到错误立即退出

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# 获取脚本所在目录
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

echo -e "${CYAN}"
echo "========================================"
echo "    Day 24: 通用引用构建脚本"
echo "========================================"
echo -e "${NC}"

# 清理旧的构建目录
if [ -d "$BUILD_DIR" ]; then
    echo -e "${YELLOW}清理旧的构建目录...${NC}"
    rm -rf "$BUILD_DIR"
fi

# 创建构建目录
echo -e "${BLUE}创建构建目录...${NC}"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# 运行 CMake 配置
echo -e "${BLUE}配置 CMake...${NC}"
cmake .. \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# 编译
echo -e "${BLUE}编译项目...${NC}"
make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

# 检查编译是否成功
if [ $? -eq 0 ]; then
    echo -e "${GREEN}✓ 编译成功！${NC}"
    echo ""
    
    # 运行程序
    echo -e "${CYAN}"
    echo "========================================"
    echo "    运行 Day 24 演示程序"
    echo "========================================"
    echo -e "${NC}"
    
    ./day_24_demo
    
    echo ""
    echo -e "${GREEN}"
    echo "========================================"
    echo "    运行完成！"
    echo "========================================"
    echo -e "${NC}"
else
    echo -e "${RED}✗ 编译失败！${NC}"
    exit 1
fi
