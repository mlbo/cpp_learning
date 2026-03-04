#!/bin/bash

# Day 31: 二叉搜索树 (BST) 构建脚本
# 功能：编译并运行所有示例程序

set -e  # 遇到错误立即退出

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

echo "========================================="
echo "  Day 31: 二叉搜索树 (BST)"
echo "========================================="
echo ""

# 创建构建目录
echo "📁 创建构建目录..."
mkdir -p "${BUILD_DIR}"

# 进入构建目录
cd "${BUILD_DIR}"

# 运行 CMake 配置
echo ""
echo "⚙️  运行 CMake 配置..."
cmake .. -DCMAKE_BUILD_TYPE=Debug

# 编译所有目标
echo ""
echo "🔨 编译项目..."
make -j$(nproc)

echo ""
echo "========================================="
echo "  编译完成！开始运行程序..."
echo "========================================="

# 运行主程序
echo ""
echo "▶️  运行主程序 (day_31_demo)..."
echo "-----------------------------------------"
./day_31_demo

# 运行 BST 演示
echo ""
echo "▶️  运行 BST 演示..."
echo "-----------------------------------------"
./bst_demo

# 运行条件变量演示
echo ""
echo "▶️  运行条件变量演示..."
echo "-----------------------------------------"
./condition_variable_demo

# 运行 LC 98 测试
echo ""
echo "▶️  运行 LeetCode 98 测试..."
echo "-----------------------------------------"
./lc0098_validate_bst

# 运行 LC 700 测试
echo ""
echo "▶️  运行 LeetCode 700 测试..."
echo "-----------------------------------------"
./lc0700_search_bst

echo ""
echo "========================================="
echo "  所有程序运行完成！"
echo "========================================="
echo ""
echo "💡 提示："
echo "  - 可执行文件位于: ${BUILD_DIR}/"
echo "  - 单独运行: ./day_31_demo"
echo "  - LC 98: ./lc0098_validate_bst"
echo "  - LC 700: ./lc0700_search_bst"
echo ""
