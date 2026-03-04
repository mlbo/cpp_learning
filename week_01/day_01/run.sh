#!/bin/bash
# Day 1 简易编译运行脚本
# 用法: ./run.sh

echo "=========================================="
echo "  Day 1 代码编译运行"
echo "=========================================="

cd /home/z/my-project/download/week_01/day_01/code

# 编译
echo "正在编译..."
g++ -std=c++17 -Wall test_all.cpp -o day1_test

if [ $? -eq 0 ]; then
    echo "✅ 编译成功!"
    echo ""
    echo "运行测试:"
    ./day1_test
else
    echo "❌ 编译失败，请检查代码"
fi
