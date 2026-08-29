#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

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

JOBS="$(detect_jobs)"
if ((JOBS > 8)); then
    JOBS=8
fi

run_build() {
    local name="$1"
    local build_type="$2"
    local enable_sanitizers="$3"
    local enable_tsan="$4"
    local build_dir="${SCRIPT_DIR}/${name}"

    echo "[${name}] Cleaning ${build_dir}"
    rm -rf "${build_dir}"

    echo "[${name}] Configuring strict C++17 build with CTest"
    cmake -S "${SCRIPT_DIR}" -B "${build_dir}" \
        -DCMAKE_BUILD_TYPE="${build_type}" \
        -DBUILD_TESTING=ON \
        -DMODEL_INFERENCE_STRICT=ON \
        -DMODEL_INFERENCE_SANITIZERS="${enable_sanitizers}" \
        -DMODEL_INFERENCE_TSAN="${enable_tsan}"

    echo "[${name}] Building demo and all contract/stress/fault-injection tests"
    cmake --build "${build_dir}" --parallel "${JOBS}"

    echo "[${name}] Running CTest"
    if [[ "$enable_sanitizers" == "ON" ]]; then
        ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 \
        UBSAN_OPTIONS=halt_on_error=1 \
            ctest --test-dir "${build_dir}" --output-on-failure --timeout 60
    elif [[ "$enable_tsan" == "ON" ]]; then
        TSAN_OPTIONS=halt_on_error=1:second_deadlock_stack=1 \
            ctest --test-dir "${build_dir}" --output-on-failure --timeout 60
    else
        ctest --test-dir "${build_dir}" --output-on-failure --timeout 60
    fi
}

run_build build Release OFF OFF
run_build build-asan Debug ON OFF
run_build build-tsan Debug OFF ON

echo "Strict C++17, stress, fault injection, ASan/UBSan and TSan all passed."
echo "Demo: ${SCRIPT_DIR}/build/inference_demo"
