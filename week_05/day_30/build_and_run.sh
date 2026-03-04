#!/bin/bash
# Day 30: 树遍历 - 构建与运行脚本

set -e  # 遇到错误立即退出

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 项目根目录
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}   Day 30: 树遍历 - 构建与运行${NC}"
echo -e "${BLUE}========================================${NC}"

# 清理旧的构建目录
if [ -d "$BUILD_DIR" ]; then
    echo -e "${YELLOW}[清理]${NC} 删除旧的构建目录..."
    rm -rf "$BUILD_DIR"
fi

# 创建构建目录
echo -e "${GREEN}[创建]${NC} 创建构建目录..."
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# 配置 CMake
echo -e "${GREEN}[配置]${NC} 运行 CMake 配置..."
cmake ..

# 编译
echo -e "${GREEN}[编译]${NC} 编译所有目标..."
make -j$(nproc)

echo -e "${BLUE}========================================${NC}"
echo -e "${GREEN}[完成]${NC} 编译成功！"
echo -e "${BLUE}========================================${NC}"

# 运行所有程序
echo ""
echo -e "${YELLOW}========================================${NC}"
echo -e "${YELLOW}         开始运行程序${NC}"
echo -e "${YELLOW}========================================${NC}"

# 函数：运行程序并显示分隔线
run_program() {
    local name=$1
    local executable=$2
    echo ""
    echo -e "${BLUE}>>> 运行: ${name}${NC}"
    echo -e "${BLUE}----------------------------------------${NC}"
    if [ -f "$BUILD_DIR/$executable" ]; then
        "$BUILD_DIR/$executable"
    else
        echo -e "${RED}错误: 找不到可执行文件 $executable${NC}"
    fi
    echo -e "${BLUE}----------------------------------------${NC}"
}

# 运行各个程序
run_program "1. 主程序入口" "main_entry"
run_program "2. 树遍历演示" "tree_traversal_demo"
run_program "3. Mutex 演示" "mutex_demo"
run_program "4. EMC++ Item 36" "item36_async"
run_program "5. EMC++ Item 37" "item37_launch_policy"
run_program "6. LeetCode 94 (中序遍历)" "leetcode_0094"
run_program "7. LeetCode 102 (层序遍历)" "leetcode_0102"

echo ""
echo -e "${GREEN}========================================${NC}"
echo -e "${GREEN}      Day 30 所有程序运行完成！${NC}"
echo -e "${GREEN}========================================${NC}"
