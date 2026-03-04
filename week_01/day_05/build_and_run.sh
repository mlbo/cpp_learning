#!/bin/bash

# Day 5: constexpr详解与滑动窗口算法
# 构建和运行脚本

set -e  # 遇到错误立即退出

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# 打印标题
print_title() {
    echo -e "${PURPLE}╔════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${PURPLE}║${NC} ${CYAN}$1${NC}"
    echo -e "${PURPLE}╚════════════════════════════════════════════════════════════╝${NC}"
}

# 打印小节
print_section() {
    echo -e "\n${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
    echo -e "${YELLOW}▶ $1${NC}"
    echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
}

# 获取脚本所在目录
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

# 清理旧构建
print_title "Day 5: constexpr详解与滑动窗口算法"
echo -e "${YELLOW}清理旧构建...${NC}"
rm -rf "${BUILD_DIR}"
mkdir -p "${BUILD_DIR}"

# CMake配置
print_section "CMake 配置"
cd "${BUILD_DIR}"
cmake .. -DCMAKE_BUILD_TYPE=Debug

# 编译
print_section "编译项目"
make -j$(nproc)

# 运行程序
print_section "运行主程序"
echo -e "${GREEN}>>> day05_main${NC}"
./day05_main

# 运行constexpr示例
print_section "constexpr 示例程序"

echo -e "${GREEN}>>> constexpr_var${NC}"
./constexpr_var

echo -e "\n${GREEN}>>> constexpr_func${NC}"
./constexpr_func

echo -e "\n${GREEN}>>> constexpr_ctor${NC}"
./constexpr_ctor

echo -e "\n${GREEN}>>> constexpr_if${NC}"
./constexpr_if

# 运行LeetCode测试
print_section "LeetCode 测试"

echo -e "${GREEN}>>> LeetCode 209: 长度最小的子数组${NC}"
./leetcode_0209

echo -e "\n${GREEN}>>> LeetCode 3: 无重复字符的最长子串${NC}"
./leetcode_0003

# 完成
print_title "✅ Day 5 构建和测试完成!"
echo -e "${CYAN}可执行文件位置: ${BUILD_DIR}${NC}"
echo ""
