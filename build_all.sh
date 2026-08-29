#!/usr/bin/env bash

# C++ 35 天教程根验证入口：逐项目 configure -> build -> test。
# 每次运行使用全新的临时构建树，完整诊断保存在该临时树并同步输出到终端。

set -uo pipefail
export LC_ALL=C

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
OUTPUT_NAME="CPP_Learning_35Days"
EXPECTED_DAYS=35
EXPECTED_ALL_PROJECTS=36

STAGE_TIMEOUT_SECONDS="${CPP_LEARNING_STAGE_TIMEOUT:-300}"
TEST_TIMEOUT_SECONDS="${CPP_LEARNING_TEST_TIMEOUT:-30}"
VERIFY_PARENT="${CPP_LEARNING_VERIFY_PARENT:-${TMPDIR:-/tmp}}"
VERIFY_ROOT=""

TOTAL_PROJECTS=0
SUCCESSFUL_PROJECTS=0
FAILED_PROJECTS=0
CONFIGURE_PASSED=0
BUILD_PASSED=0
TEST_PASSED=0
FAILED_STAGES=()
NOT_RUN_STAGES=()

print_info() { echo -e "${BLUE}[INFO]${NC} $*"; }
print_success() { echo -e "${GREEN}[PASS]${NC} $*"; }
print_warning() { echo -e "${YELLOW}[WARN]${NC} $*"; }
print_error() { echo -e "${RED}[FAIL]${NC} $*"; }
print_header() {
    echo
    echo -e "${GREEN}========================================${NC}"
    echo "$*"
    echo -e "${GREEN}========================================${NC}"
}

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

DETECTED_JOBS="$(detect_jobs)"
if ((DETECTED_JOBS > 8)); then
    DETECTED_JOBS=8
fi
JOBS="${CPP_LEARNING_JOBS:-${DETECTED_JOBS}}"
TIMEOUT_BINARY=""

show_help() {
    cat <<EOF
C++ 35 天学习教程 - 根验证入口

用法: $0 [选项]

选项:
  --clean       兼容选项；验证本来就始终使用全新的临时构建树
  --model-only  仅验证 model_inference_project
  --pack-only   原子创建完整 ${OUTPUT_NAME}.zip，不执行验证
  --help, -h    显示帮助

环境变量:
  CPP_LEARNING_STAGE_TIMEOUT  configure/build/test 阶段上限秒数，默认 300
  CPP_LEARNING_TEST_TIMEOUT   单个 CTest 用例上限秒数，默认 30
  CPP_LEARNING_JOBS           并行构建任务数，默认自动检测且最多 8
  CPP_LEARNING_VERIFY_PARENT  临时验证树父目录，默认 TMPDIR 或 /tmp

默认行为会对 35 个每日项目和模型综合项目分别执行：
  cmake configure -> cmake --build -> CTest
任何阶段失败、超时、没有发现测试，或因前序失败未运行，最终退出码均为非零。
EOF
}

validate_positive_integer() {
    local name="$1"
    local value="$2"
    if [[ ! "$value" =~ ^[1-9][0-9]*$ ]]; then
        print_error "${name} 必须是正整数，当前值: ${value}"
        return 1
    fi
}

configure_timeout_command() {
    if command -v timeout >/dev/null 2>&1; then
        TIMEOUT_BINARY="$(command -v timeout)"
    elif command -v gtimeout >/dev/null 2>&1; then
        TIMEOUT_BINARY="$(command -v gtimeout)"
    else
        print_error "缺少 timeout（macOS 可安装 coreutils 提供 gtimeout）"
        return 1
    fi
}

# GNU timeout 默认创建独立进程组。命令结束后再次清理该组，既覆盖正常超时，
# 也覆盖父进程提前退出但遗留子进程的情况。
run_timed_command() {
    local duration="$1"
    shift

    "$TIMEOUT_BINARY" --kill-after=1s "$duration" "$@" &
    local supervisor_pid=$!
    local status=0
    wait "$supervisor_pid" || status=$?
    kill -KILL -- "-${supervisor_pid}" 2>/dev/null || true
    return "$status"
}

check_validation_dependencies() {
    local missing=()
    command -v cmake >/dev/null 2>&1 || missing+=(cmake)
    command -v ctest >/dev/null 2>&1 || missing+=(ctest)
    command -v mktemp >/dev/null 2>&1 || missing+=(mktemp)
    if ! command -v g++ >/dev/null 2>&1 && ! command -v clang++ >/dev/null 2>&1; then
        missing+=("g++/clang++")
    fi
    if ((${#missing[@]} > 0)); then
        print_error "缺少验证依赖: ${missing[*]}"
        return 1
    fi
    configure_timeout_command || return 1
    validate_positive_integer CPP_LEARNING_STAGE_TIMEOUT "$STAGE_TIMEOUT_SECONDS" || return 1
    validate_positive_integer CPP_LEARNING_TEST_TIMEOUT "$TEST_TIMEOUT_SECONDS" || return 1
    validate_positive_integer CPP_LEARNING_JOBS "$JOBS" || return 1
    if [[ ! -d "$VERIFY_PARENT" || ! -w "$VERIFY_PARENT" ]]; then
        print_error "临时验证树父目录不可写: ${VERIFY_PARENT}"
        return 1
    fi
    print_success "cmake: $(cmake --version | head -n 1)"
    print_success "并行任务数: ${JOBS}"
    print_success "阶段超时: ${STAGE_TIMEOUT_SECONDS}s；单测试超时: ${TEST_TIMEOUT_SECONDS}s"
}

prepare_verification_root() {
    VERIFY_ROOT="$(mktemp -d "${VERIFY_PARENT%/}/cpp-learning-verify.XXXXXX")" || {
        print_error "无法创建临时验证树: ${VERIFY_PARENT}"
        return 1
    }
    mkdir -p "${VERIFY_ROOT}/build" "${VERIFY_ROOT}/logs" || return 1
    print_success "全新临时验证树: ${VERIFY_ROOT}"
}

record_stage_failure() {
    local label="$1"
    local stage="$2"
    local detail="$3"
    FAILED_STAGES+=("${label}:${stage}:${detail}")
}

record_not_run() {
    local label="$1"
    local stage="$2"
    local reason="$3"
    NOT_RUN_STAGES+=("${label}:${stage}:${reason}")
    print_error "${label}: ${stage} 未运行（${reason}）"
}

run_stage() {
    local label="$1"
    local stage="$2"
    local timeout_seconds="$3"
    local log_file="$4"
    shift 4

    print_info "${label}: ${stage}"
    {
        echo
        echo "===== ${stage} ====="
        printf 'command:'
        printf ' %q' "$@"
        echo
    } >>"$log_file"

    run_timed_command "${timeout_seconds}s" "$@" 2>&1 | tee -a "$log_file"
    local pipeline_status=("${PIPESTATUS[@]}")
    local status=${pipeline_status[0]}
    local tee_status=${pipeline_status[1]}
    if ((status == 0 && tee_status == 0)); then
        print_success "${label}: ${stage}"
        case "$stage" in
            configure) CONFIGURE_PASSED=$((CONFIGURE_PASSED + 1)) ;;
            build) BUILD_PASSED=$((BUILD_PASSED + 1)) ;;
            test) TEST_PASSED=$((TEST_PASSED + 1)) ;;
        esac
        return 0
    fi

    local detail="exit=${status}"
    if ((tee_status != 0)); then
        detail="diagnostic-write-exit=${tee_status}"
    elif ((status == 124 || status == 137)); then
        detail="timeout=${timeout_seconds}s"
    fi
    record_stage_failure "$label" "$stage" "$detail"
    print_error "${label}: ${stage} 失败（${detail}）；诊断: ${log_file}"
    return 1
}

run_tests() {
    local label="$1"
    local build_dir="$2"
    local log_file="$3"
    local listing

    print_info "${label}: test discovery"
    listing="$(run_timed_command "${STAGE_TIMEOUT_SECONDS}s" \
        ctest --test-dir "$build_dir" -N 2>&1)"
    local discovery_status=$?
    {
        echo
        echo "===== test discovery ====="
        echo "$listing"
    } | tee -a "$log_file"
    local discovery_log_status=("${PIPESTATUS[@]}")

    if ((discovery_log_status[1] != 0)); then
        record_stage_failure "$label" test \
            "discovery-diagnostic-write-exit=${discovery_log_status[1]}"
        print_error "${label}: 测试发现日志写入失败"
        return 1
    fi

    if ((discovery_status != 0)); then
        local detail="discovery-exit=${discovery_status}"
        if ((discovery_status == 124 || discovery_status == 137)); then
            detail="discovery-timeout=${STAGE_TIMEOUT_SECONDS}s"
        fi
        record_stage_failure "$label" test "$detail"
        print_error "${label}: CTest 测试发现失败（${detail}）"
        return 1
    fi

    local test_count
    test_count="$(printf '%s\n' "$listing" | sed -n 's/^[[:space:]]*Total Tests: \([0-9][0-9]*\)[[:space:]]*$/\1/p' | tail -n 1)"
    if [[ -z "$test_count" || "$test_count" == "0" ]]; then
        record_stage_failure "$label" test "no-tests-discovered"
        print_error "${label}: 未发现 CTest；测试阶段视为失败"
        return 1
    fi

    print_info "${label}: 发现 ${test_count} 个 CTest"
    local test_output_file="${build_dir}/root-verification-ctest-output.log"
    if ! : >"$test_output_file"; then
        record_stage_failure "$label" test "test-output-create-failed"
        print_error "${label}: 无法创建 CTest 状态输出文件"
        return 1
    fi
    {
        echo
        echo "===== test ====="
        printf 'command: ctest --test-dir %q --output-on-failure --timeout %q\n' \
            "$build_dir" "$TEST_TIMEOUT_SECONDS"
    } >>"$log_file" || {
        record_stage_failure "$label" test "diagnostic-header-write-failed"
        print_error "${label}: 测试日志头写入失败"
        return 1
    }

    print_info "${label}: test"
    run_timed_command "${STAGE_TIMEOUT_SECONDS}s" \
        ctest --test-dir "$build_dir" --output-on-failure \
        --timeout "$TEST_TIMEOUT_SECONDS" 2>&1 | \
        tee -a "$log_file" "$test_output_file"
    local test_pipeline_status=("${PIPESTATUS[@]}")
    local test_status=${test_pipeline_status[0]}
    local test_tee_status=${test_pipeline_status[1]}
    if ((test_status != 0 || test_tee_status != 0)); then
        local detail="exit=${test_status}"
        if ((test_tee_status != 0)); then
            detail="diagnostic-write-exit=${test_tee_status}"
        elif ((test_status == 124 || test_status == 137)); then
            detail="timeout=${STAGE_TIMEOUT_SECONDS}s"
        fi
        record_stage_failure "$label" test "$detail"
        print_error "${label}: test 失败（${detail}）；诊断: ${log_file}"
        return 1
    fi

    local passed_count
    passed_count="$(awk '
        /^[[:space:]]*[0-9]+\/[0-9]+ Test[[:space:]]+#[0-9]+:/ &&
        /[[:space:]]Passed[[:space:]]/ { ++count }
        END { print count + 0 }
    ' "$test_output_file")"
    if [[ "$passed_count" != "$test_count" ]]; then
        local detail="passed=${passed_count},discovered=${test_count}"
        if grep -Eq '\*\*\*(Not Run|Skipped)|\(Disabled\)' "$test_output_file"; then
            detail="disabled-or-skipped;${detail}"
        fi
        record_stage_failure "$label" test "$detail"
        print_error "${label}: 有 CTest 未实际执行并通过（${detail}）"
        return 1
    fi

    TEST_PASSED=$((TEST_PASSED + 1))
    print_success "${label}: test（${passed_count}/${test_count} 实际执行并通过）"
}

validate_project() {
    local project_dir="$1"
    local label="${project_dir#"${SCRIPT_DIR}/"}"
    local build_dir="${VERIFY_ROOT}/build/${label}"
    local log_file="${VERIFY_ROOT}/logs/${label}.log"

    TOTAL_PROJECTS=$((TOTAL_PROJECTS + 1))
    print_header "验证 ${label}"

    mkdir -p "$build_dir" "$(dirname "$log_file")"
    printf 'Verification for %s\n' "$label" >"$log_file"

    local configure_args=(
        cmake -S "$project_dir" -B "$build_dir"
        -DCMAKE_BUILD_TYPE=Release
        -DBUILD_TESTING=ON
        -DCMAKE_CXX_STANDARD=17
        -DCMAKE_CXX_STANDARD_REQUIRED=ON
        -DCMAKE_CXX_EXTENSIONS=OFF
    )
    case "$label" in
        week_01/day_03|week_04/day_24|week_04/day_25|week_04/day_26)
            configure_args+=(-DBUILD_TESTS=ON)
            ;;
    esac
    if [[ "$label" == "model_inference_project" ]]; then
        configure_args+=(
            -DMODEL_INFERENCE_STRICT=ON
            -DMODEL_INFERENCE_SANITIZERS=OFF
        )
    fi

    if ! run_stage "$label" configure "$STAGE_TIMEOUT_SECONDS" "$log_file" \
        "${configure_args[@]}"; then
        record_not_run "$label" build "configure failed"
        record_not_run "$label" test "configure failed"
        FAILED_PROJECTS=$((FAILED_PROJECTS + 1))
        return 1
    fi

    if ! run_stage "$label" build "$STAGE_TIMEOUT_SECONDS" "$log_file" \
        cmake --build "$build_dir" --parallel "$JOBS"; then
        record_not_run "$label" test "build failed"
        FAILED_PROJECTS=$((FAILED_PROJECTS + 1))
        return 1
    fi

    if ! run_tests "$label" "$build_dir" "$log_file"; then
        FAILED_PROJECTS=$((FAILED_PROJECTS + 1))
        return 1
    fi

    SUCCESSFUL_PROJECTS=$((SUCCESSFUL_PROJECTS + 1))
    return 0
}

record_missing_project() {
    local label="$1"
    local detail="$2"
    TOTAL_PROJECTS=$((TOTAL_PROJECTS + 1))
    FAILED_PROJECTS=$((FAILED_PROJECTS + 1))
    record_stage_failure "$label" configure "$detail"
    record_not_run "$label" build "project missing"
    record_not_run "$label" test "project missing"
}

expected_day_label() {
    local day="$1"
    local week=$(((day - 1) / 7 + 1))
    printf 'week_%02d/day_%02d' "$week" "$day"
}

is_expected_day_label() {
    local candidate="$1"
    local day expected
    for ((day = 1; day <= EXPECTED_DAYS; ++day)); do
        expected="$(expected_day_label "$day")"
        if [[ "$candidate" == "$expected" ]]; then
            return 0
        fi
    done
    return 1
}

audit_day_inventory() {
    local manifest
    if ! manifest="$(find "$SCRIPT_DIR" -mindepth 3 -maxdepth 3 \
        -path "${SCRIPT_DIR}/week_??/day_??/CMakeLists.txt" -type f -print | sort)"; then
        record_stage_failure root inventory "discovery-failed"
        print_error "无法审计 Day 01-35 项目集合"
        return 1
    fi

    local actual_paths=()
    if [[ -n "$manifest" ]]; then
        mapfile -t actual_paths <<<"$manifest"
    fi

    local failed=false
    if ((${#actual_paths[@]} != EXPECTED_DAYS)); then
        record_stage_failure root inventory \
            "expected-days=${EXPECTED_DAYS},actual-cmake-projects=${#actual_paths[@]}"
        print_error "日项目集合数量错误：期望 ${EXPECTED_DAYS}，实际 ${#actual_paths[@]}"
        failed=true
    fi

    local cmake_file label
    for cmake_file in "${actual_paths[@]}"; do
        label="${cmake_file#"${SCRIPT_DIR}/"}"
        label="${label%/CMakeLists.txt}"
        if ! is_expected_day_label "$label"; then
            record_stage_failure root inventory "unexpected=${label}"
            print_error "发现清单外日项目: ${label}"
            failed=true
        fi
    done

    local day expected
    for ((day = 1; day <= EXPECTED_DAYS; ++day)); do
        expected="$(expected_day_label "$day")"
        if [[ ! -f "${SCRIPT_DIR}/${expected}/CMakeLists.txt" ]]; then
            record_stage_failure root inventory "missing=${expected}"
            print_error "缺少固定日项目: ${expected}"
            failed=true
        fi
    done

    [[ "$failed" == false ]]
}

validate_all_days() {
    local day week label project_dir
    for ((day = 1; day <= EXPECTED_DAYS; ++day)); do
        week=$(((day - 1) / 7 + 1))
        printf -v label 'week_%02d/day_%02d' "$week" "$day"
        project_dir="${SCRIPT_DIR}/${label}"
        if [[ ! -d "$project_dir" ]]; then
            record_missing_project "$label" "missing-directory"
        elif [[ ! -f "${project_dir}/CMakeLists.txt" ]]; then
            record_missing_project "$label" "missing-CMakeLists.txt"
        else
            validate_project "$project_dir" || true
        fi
    done
}

cleanup_package_staging() {
    local staging_dir="$1"
    local expected_prefix="${SCRIPT_DIR%/}/.cpp-learning-package."
    if [[ -z "$staging_dir" || "$staging_dir" != "${expected_prefix}"* || \
          "$staging_dir" == "$SCRIPT_DIR" ]]; then
        print_error "拒绝清理无法确认的打包暂存路径: ${staging_dir}"
        return 1
    fi
    if [[ -e "$staging_dir" ]]; then
        rm -rf -- "$staging_dir"
    fi
}

create_package() {
    print_header "创建发布包"
    local missing=()
    local dependency
    for dependency in zip cp mv mktemp; do
        command -v "$dependency" >/dev/null 2>&1 || missing+=("$dependency")
    done
    if ((${#missing[@]} > 0)); then
        print_error "缺少打包依赖: ${missing[*]}"
        return 1
    fi

    local output_file="${SCRIPT_DIR}/${OUTPUT_NAME}.zip"
    local staging_dir
    # 暂存目录与最终 zip 位于同一目录/文件系统，最后的 mv 才是原子替换。
    staging_dir="$(mktemp -d "${SCRIPT_DIR%/}/.cpp-learning-package.XXXXXX")" || {
        print_error "无法在发布目录创建唯一打包暂存目录: ${SCRIPT_DIR}"
        return 1
    }
    local payload_dir="${staging_dir}/${OUTPUT_NAME}"
    local archive_tmp="${staging_dir}/${OUTPUT_NAME}.zip"
    if ! mkdir -p "$payload_dir"; then
        print_error "无法创建打包载荷目录"
        cleanup_package_staging "$staging_dir" || true
        return 1
    fi

    local required_paths=(
        README.md
        CPP_35天科学学习规划_最终版.md
        Modern_CPP_Features_Deep_Dive.md
        build_all.sh
        tutorials
        week_01 week_02 week_03 week_04 week_05
        model_inference_project
    )
    local path destination
    for path in "${required_paths[@]}"; do
        if [[ ! -e "${SCRIPT_DIR}/${path}" ]]; then
            print_error "打包必需路径缺失: ${path}"
            cleanup_package_staging "$staging_dir" || true
            return 1
        fi
        if ! cp -R "${SCRIPT_DIR}/${path}" "$payload_dir/"; then
            print_error "复制打包路径失败: ${path}"
            cleanup_package_staging "$staging_dir" || true
            return 1
        fi
        destination="${payload_dir}/${path##*/}"
        if [[ ! -e "$destination" ]]; then
            print_error "复制命令未产生预期路径: ${path}"
            cleanup_package_staging "$staging_dir" || true
            return 1
        fi
    done

    if ! (cd "$staging_dir" && zip -r -q "$archive_tmp" "$OUTPUT_NAME"); then
        print_error "创建发布包失败"
        cleanup_package_staging "$staging_dir" || true
        return 1
    fi
    if [[ ! -s "$archive_tmp" ]]; then
        print_error "发布包为空"
        cleanup_package_staging "$staging_dir" || true
        return 1
    fi
    if ! mv -f "$archive_tmp" "$output_file"; then
        print_error "原子发布压缩包失败"
        cleanup_package_staging "$staging_dir" || true
        return 1
    fi
    if [[ -e "$archive_tmp" || ! -s "$output_file" ]]; then
        print_error "压缩包发布后状态不完整"
        cleanup_package_staging "$staging_dir" || true
        return 1
    fi
    cleanup_package_staging "$staging_dir" || return 1
    print_success "发布包: ${output_file}"
}

show_summary() {
    print_header "根验证汇总"
    echo "项目总数:       ${TOTAL_PROJECTS}"
    echo "完整通过:       ${SUCCESSFUL_PROJECTS}"
    echo "存在失败:       ${FAILED_PROJECTS}"
    echo "configure 通过: ${CONFIGURE_PASSED}/${TOTAL_PROJECTS}"
    echo "build 通过:     ${BUILD_PASSED}/${TOTAL_PROJECTS}"
    echo "test 通过:      ${TEST_PASSED}/${TOTAL_PROJECTS}"
    if [[ -n "$VERIFY_ROOT" ]]; then
        echo "验证工作区:     ${VERIFY_ROOT}"
    fi

    if ((${#FAILED_STAGES[@]} > 0)); then
        echo
        print_error "失败或超时阶段:"
        printf '  - %s\n' "${FAILED_STAGES[@]}"
    fi
    if ((${#NOT_RUN_STAGES[@]} > 0)); then
        echo
        print_error "未运行阶段:"
        printf '  - %s\n' "${NOT_RUN_STAGES[@]}"
    fi
}

main() {
    local clean=false
    local model_only=false
    local pack_only=false

    while (($# > 0)); do
        case "$1" in
            --clean) clean=true ;;
            --model-only) model_only=true ;;
            --pack-only) pack_only=true ;;
            --help|-h) show_help; return 0 ;;
            *) print_error "未知选项: $1"; show_help; return 2 ;;
        esac
        shift
    done

    if [[ "$pack_only" == true ]]; then
        create_package
        return $?
    fi

    print_header "C++ 35 天教程根验证"
    check_validation_dependencies || return 2
    prepare_verification_root || return 2
    # 临时验证树清理：正常退出(码0)时清理，失败/中断时保留现场便于排查。
    trap '
      rc=$?
      if [[ $rc -eq 0 && -n "${VERIFY_ROOT}" ]]; then
        rm -rf -- "${VERIFY_ROOT}"
      fi
    ' EXIT INT TERM
    if [[ "$clean" == true ]]; then
        print_info "--clean 已确认：本次验证使用刚创建的全新临时构建树"
    fi

    if [[ "$model_only" != true ]]; then
        audit_day_inventory || true
        validate_all_days
    fi

    local model_dir="${SCRIPT_DIR}/model_inference_project"
    if [[ -d "$model_dir" && -f "$model_dir/CMakeLists.txt" ]]; then
        validate_project "$model_dir" || true
    else
        record_missing_project model_inference_project "missing-project"
    fi

    local expected_projects=1
    if [[ "$model_only" != true ]]; then
        expected_projects=$EXPECTED_ALL_PROJECTS
    fi
    if ((TOTAL_PROJECTS != expected_projects)); then
        FAILED_PROJECTS=$((FAILED_PROJECTS + 1))
        record_stage_failure root inventory \
            "expected=${expected_projects},actual=${TOTAL_PROJECTS}"
    fi

    show_summary
    if ((FAILED_PROJECTS > 0 || ${#FAILED_STAGES[@]} > 0 || ${#NOT_RUN_STAGES[@]} > 0)); then
        return 1
    fi
    print_success "所有 configure/build/test 阶段均已通过"
}

if [[ "${BASH_SOURCE[0]}" == "$0" ]]; then
    main "$@"
fi
