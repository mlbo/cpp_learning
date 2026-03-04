#!/bin/bash

# Day 23: 移动语义 - 构建和运行脚本
# 本脚本用于编译和运行 Day 23 的所有示例代码

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
echo "=============================================="
echo "  Day 23: 移动语义 - 构建和运行"
echo "=============================================="
echo -e "${NC}"

# 创建构建目录
echo -e "${BLUE}[1/3] 创建构建目录...${NC}"
mkdir -p "${BUILD_DIR}"
cd "${BUILD_DIR}"

# 配置 CMake
echo -e "${BLUE}[2/3] 配置 CMake 项目...${NC}"
cmake .. -DCMAKE_BUILD_TYPE=Debug

# 编译
echo -e "${BLUE}[3/3] 编译项目...${NC}"
make -j$(nproc 2>/dev/null || echo 4)

echo -e "${GREEN}"
echo "=============================================="
echo "  编译成功！正在运行程序..."
echo "=============================================="
echo -e "${NC}"

# 运行程序
echo -e "${YELLOW}>>> 开始执行程序 <<<${NC}"
echo ""
./day_23_main

echo ""
echo -e "${GREEN}"
echo "=============================================="
echo "  Day 23 学习完成！"
echo "=============================================="
echo -e "${NC}"
echo -e "今日要点回顾:"
echo -e "  ${CYAN}✓${NC} 移动语义的基本概念"
echo -e "  ${CYAN}✓${NC} 移动构造函数和移动赋值运算符"
echo -e "  ${CYAN}✓${NC} std::move 和 std::forward"
echo -e "  ${CYAN}✓${NC} 通用引用与右值引用的区别"
echo -e "  ${CYAN}✓${NC} LeetCode 两数之和和四数相加 II"
echo ""
