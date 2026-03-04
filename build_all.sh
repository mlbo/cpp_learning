#!/bin/bash

# ============================================================
# C++ 35天学习教程 - 统一编译脚本
# ============================================================
# 功能：编译 Week 1-5 所有代码，并打包为英文命名的压缩包
# 使用：./build_all.sh [选项]
#   选项：
#     --clean     编译前清理构建目录
#     --pack-only 仅打包，不编译
#     --help      显示帮助信息
# ============================================================

set -e  # 遇到错误立即退出

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 配置
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build_all"
OUTPUT_NAME="CPP_Learning_35Days"
WEEKS=("week_01" "week_02" "week_03" "week_04" "week_05")

# 统计变量
TOTAL_SUCCESS=0
TOTAL_FAILED=0
FAILED_PROJECTS=()

# 打印带颜色的消息
print_info() { echo -e "${BLUE}[INFO]${NC} $1"; }
print_success() { echo -e "${GREEN}[SUCCESS]${NC} $1"; }
print_warning() { echo -e "${YELLOW}[WARNING]${NC} $1"; }
print_error() { echo -e "${RED}[ERROR]${NC} $1"; }
print_header() { echo -e "\n${GREEN}========================================${NC}"; echo -e "$1"; echo -e "${GREEN}========================================${NC}"; }

# 获取并行编译任务数（兼容 Linux/macOS）
detect_jobs() {
    if command -v nproc >/dev/null 2>&1; then
        nproc
    elif command -v getconf >/dev/null 2>&1; then
        getconf _NPROCESSORS_ONLN
    elif command -v sysctl >/dev/null 2>&1; then
        sysctl -n hw.ncpu
    else
        echo 4
    fi
}

JOBS=$(detect_jobs)

# 显示帮助
show_help() {
    echo "C++ 35天学习教程 - 统一编译脚本"
    echo ""
    echo "用法: $0 [选项]"
    echo ""
    echo "选项:"
    echo "  --clean      编译前清理所有构建目录"
    echo "  --pack-only  仅打包，不进行编译"
    echo "  --help       显示此帮助信息"
    echo ""
    echo "输出:"
    echo "  ${OUTPUT_NAME}.zip - 包含所有教程代码和文档"
    echo ""
    exit 0
}

# 检查依赖
check_dependencies() {
    print_header "检查依赖环境"
    
    local missing_deps=()
    
    # 检查 cmake
    if ! command -v cmake &> /dev/null; then
        missing_deps+=("cmake")
    else
        print_success "cmake: $(cmake --version | head -1)"
    fi
    
    # 检查 C++ 编译器（g++ 或 clang++）
    if command -v g++ &> /dev/null; then
        print_success "g++: $(g++ --version | head -1)"
    elif command -v clang++ &> /dev/null; then
        print_success "clang++: $(clang++ --version | head -1)"
    else
        missing_deps+=("g++/clang++")
    fi
    
    # 检查 zip
    if ! command -v zip &> /dev/null; then
        missing_deps+=("zip")
    else
        print_success "zip: available"
    fi
    
    if [ ${#missing_deps[@]} -gt 0 ]; then
        print_error "缺少以下依赖: ${missing_deps[*]}"
        echo "请安装缺少的依赖后重试"
        exit 1
    fi
    
    print_success "所有依赖检查通过"
}

# 编译单个项目
build_project() {
    local project_dir=$1
    local project_name=$(basename "$project_dir")
    local week_name=$(basename "$(dirname "$project_dir")")
    
    local build_dir="${project_dir}/build"
    local cmake_file="${project_dir}/CMakeLists.txt"
    
    # 检查是否有 CMakeLists.txt
    if [ ! -f "$cmake_file" ]; then
        return 0  # 跳过没有 CMakeLists.txt 的目录
    fi
    
    print_info "编译: ${week_name}/${project_name}"
    
    # 创建构建目录
    mkdir -p "$build_dir"
    cd "$build_dir"
    
    # 运行 cmake
    if cmake .. -DCMAKE_BUILD_TYPE=Release > /dev/null 2>&1; then
        # 运行构建（兼容 Make/Ninja）
        if cmake --build . --parallel "${JOBS}" > /dev/null 2>&1; then
            print_success "编译成功: ${week_name}/${project_name}"
            ((TOTAL_SUCCESS++))
            return 0
        else
            print_error "编译失败: ${week_name}/${project_name} (make error)"
            ((TOTAL_FAILED++))
            FAILED_PROJECTS+=("${week_name}/${project_name}")
            return 1
        fi
    else
        print_error "编译失败: ${week_name}/${project_name} (cmake error)"
        ((TOTAL_FAILED++))
        FAILED_PROJECTS+=("${week_name}/${project_name}")
        return 1
    fi
}

# 编译一周的所有项目
build_week() {
    local week_dir=$1
    local week_name=$(basename "$week_dir")
    
    print_header "编译 ${week_name}"
    
    # 查找所有包含 CMakeLists.txt 的子目录
    local day_dirs=$(find "$week_dir" -maxdepth 2 -name "CMakeLists.txt" -exec dirname {} \; | sort)
    
    if [ -z "$day_dirs" ]; then
        print_warning "${week_name} 中没有找到可编译的项目"
        return
    fi
    
    local count=0
    for day_dir in $day_dirs; do
        if ! build_project "$day_dir"; then
            true
        fi
        ((count++))
    done
    
    print_info "${week_name} 共处理 ${count} 个项目"
}

# 清理构建目录
clean_builds() {
    print_header "清理构建目录"
    
    for week in "${WEEKS[@]}"; do
        local week_path="${SCRIPT_DIR}/${week}"
        if [ -d "$week_path" ]; then
            find "$week_path" -type d -name "build" -exec rm -rf {} + 2>/dev/null || true
            find "$week_path" -type d -name "CMakeFiles" -exec rm -rf {} + 2>/dev/null || true
            find "$week_path" -type f -name "CMakeCache.txt" -delete 2>/dev/null || true
            find "$week_path" -type f -name "Makefile" -delete 2>/dev/null || true
            find "$week_path" -type f -name "cmake_install.cmake" -delete 2>/dev/null || true
            find "$week_path" -type f -name "*.o" -delete 2>/dev/null || true
            print_info "已清理: ${week}"
        fi
    done
    
    # 清理模型推理项目
    local model_project="${SCRIPT_DIR}/model_inference_project"
    if [ -d "$model_project" ]; then
        find "$model_project" -type d -name "build" -exec rm -rf {} + 2>/dev/null || true
        find "$model_project" -type d -name "CMakeFiles" -exec rm -rf {} + 2>/dev/null || true
        find "$model_project" -type f -name "CMakeCache.txt" -delete 2>/dev/null || true
        find "$model_project" -type f -name "Makefile" -delete 2>/dev/null || true
        print_info "已清理: model_inference_project"
    fi
    
    print_success "清理完成"
}

# 打包所有内容
create_package() {
    print_header "创建发布包"
    
    local output_file="${SCRIPT_DIR}/${OUTPUT_NAME}.zip"
    local temp_dir="${SCRIPT_DIR}/${OUTPUT_NAME}_temp"
    
    # 删除旧的打包文件
    rm -f "$output_file"
    rm -rf "$temp_dir"
    
    # 创建临时目录结构
    mkdir -p "$temp_dir"
    
    print_info "复制教程文档..."
    # 复制 tutorials 目录
    if [ -d "${SCRIPT_DIR}/tutorials" ]; then
        cp -r "${SCRIPT_DIR}/tutorials" "$temp_dir/"
        print_success "已复制 tutorials 目录"
    fi
    
    print_info "复制教程代码..."
    # 复制 week_01 到 week_05
    for week in "${WEEKS[@]}"; do
        local week_path="${SCRIPT_DIR}/${week}"
        if [ -d "$week_path" ]; then
            cp -r "$week_path" "$temp_dir/"
            print_success "已复制 ${week}"
        fi
    done
    
    # 复制模型推理项目
    print_info "复制实战项目..."
    if [ -d "${SCRIPT_DIR}/model_inference_project" ]; then
        cp -r "${SCRIPT_DIR}/model_inference_project" "$temp_dir/"
        print_success "已复制 model_inference_project"
    fi
    
    # 创建 README
    cat > "${temp_dir}/README.txt" << 'EOF'
============================================================
        C++ 35天科学学习教程
============================================================

目录结构：
├── tutorials/              # 教程文档
│   ├── CPP基础学习教程.md
│   ├── Effective_Modern_CPP教程.md
│   ├── CPP并发编程教程.md
│   ├── 设计模式教程.md
│   └── OS面试常考知识.md
├── week_01/                # 第1周：C++基础 + 数组双指针
├── week_02/                # 第2周：链表 + 智能指针
├── week_03/                # 第3周：栈队列 + Lambda
├── week_04/                # 第4周：哈希表 + 移动语义
├── week_05/                # 第5周：树 + 并发编程
└── model_inference_project/ # 实战项目：模型推理引擎

编译说明：
1. 单独编译某一天：
   cd week_XX/day_XX
   mkdir build && cd build
   cmake .. && make

2. 统一编译所有（需要 build_all.sh）：
   ./build_all.sh

学习建议：
- 每天学习 2-3 小时
- 先看 README.md 了解当天主题
- 运行代码理解实践
- 完成 LeetCode 练习

============================================================
EOF
    
    # 打包
    print_info "正在压缩..."
    cd "$SCRIPT_DIR"
    zip -r -q "$output_file" "$(basename $temp_dir)"
    
    # 清理临时目录
    rm -rf "$temp_dir"
    
    # 显示结果
    local size=$(du -h "$output_file" | cut -f1)
    print_success "发布包创建成功: ${output_file} (${size})"
}

# 编译模型推理项目
build_model_project() {
    local project_dir="${SCRIPT_DIR}/model_inference_project"
    
    if [ ! -d "$project_dir" ]; then
        print_warning "未找到 model_inference_project"
        return
    fi
    
    print_header "编译实战项目: model_inference_project"
    
    local build_dir="${project_dir}/build"
    mkdir -p "$build_dir"
    cd "$build_dir"
    
    if cmake .. -DCMAKE_BUILD_TYPE=Release > /dev/null 2>&1; then
        if cmake --build . --parallel "${JOBS}" > /dev/null 2>&1; then
            print_success "编译成功: model_inference_project"
            # 测试运行
            print_info "测试运行..."
            if ./model_inference_demo > /dev/null 2>&1; then
                print_success "测试运行成功"
            else
                print_warning "测试运行失败（可能需要调整参数）"
            fi
        else
            print_error "编译失败: model_inference_project (make error)"
        fi
    else
        print_error "编译失败: model_inference_project (cmake error)"
    fi
}

# 显示统计信息
show_summary() {
    print_header "编译统计"
    
    echo ""
    echo "  编译成功: ${TOTAL_SUCCESS} 个项目"
    echo "  编译失败: ${TOTAL_FAILED} 个项目"
    echo ""
    
    if [ ${#FAILED_PROJECTS[@]} -gt 0 ]; then
        print_warning "失败的项目列表:"
        for proj in "${FAILED_PROJECTS[@]}"; do
            echo "    - $proj"
        done
        echo ""
    fi
    
    if [ $TOTAL_FAILED -eq 0 ]; then
        print_success "所有项目编译成功！"
    else
        print_warning "部分项目编译失败，请检查错误信息"
    fi
}

# 主函数
main() {
    local clean=false
    local pack_only=false
    
    # 解析参数
    while [[ $# -gt 0 ]]; do
        case $1 in
            --clean)
                clean=true
                shift
                ;;
            --pack-only)
                pack_only=true
                shift
                ;;
            --help|-h)
                show_help
                ;;
            *)
                print_error "未知选项: $1"
                echo "使用 --help 查看帮助"
                exit 1
                ;;
        esac
    done
    
    if [ -t 1 ]; then
        clear
    fi
    echo -e "${GREEN}"
    echo "============================================================"
    echo "          C++ 35天学习教程 - 统一编译脚本"
    echo "============================================================"
    echo -e "${NC}"
    
    # 检查依赖
    check_dependencies
    
    # 如果只是打包
    if [ "$pack_only" = true ]; then
        create_package
        exit 0
    fi
    
    # 清理
    if [ "$clean" = true ]; then
        clean_builds
    fi
    
    # 编译所有周
    for week in "${WEEKS[@]}"; do
        local week_path="${SCRIPT_DIR}/${week}"
        if [ -d "$week_path" ]; then
            build_week "$week_path"
        else
            print_warning "未找到 ${week}"
        fi
    done
    
    # 编译模型推理项目
    build_model_project
    
    # 显示统计
    show_summary
    
    # 打包
    # create_package
    
    echo ""
    print_success "全部完成！"
    echo ""
}

# 执行主函数
main "$@"
