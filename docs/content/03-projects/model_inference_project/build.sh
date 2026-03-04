#!/bin/bash

# ========================================
# Model Inference Project Build Script
# ========================================

set -e

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Project directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

# Detect parallel jobs for Linux/macOS
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

echo -e "${BLUE}"
echo "========================================"
echo "  C++ Model Inference Project Builder"
echo "========================================"
echo -e "${NC}"

# Clean old build
echo -e "${YELLOW}[1/3] Cleaning build directory...${NC}"
rm -rf "${BUILD_DIR}"
mkdir -p "${BUILD_DIR}"

# CMake configuration
echo -e "${YELLOW}[2/3] Running CMake configuration...${NC}"
cd "${BUILD_DIR}"
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build
echo -e "${YELLOW}[3/3] Building project...${NC}"
cmake --build . --parallel "${JOBS}"

echo ""
echo -e "${GREEN}========================================${NC}"
echo -e "${GREEN}  Build successful!${NC}"
echo -e "${GREEN}========================================${NC}"
echo ""
echo -e "Executable: ${BUILD_DIR}/inference_demo"
echo ""
echo -e "Run with: ${YELLOW}./build/inference_demo${NC}"
echo ""
