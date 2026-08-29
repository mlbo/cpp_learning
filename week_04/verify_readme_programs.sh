#!/bin/bash

set -euo pipefail

WEEK_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
VERIFY_DIR="$(mktemp -d /tmp/week04-readme-programs.XXXXXX)"
COMPILER="${CXX:-c++}"
TIMEOUT_COMMAND="${TIMEOUT_COMMAND:-timeout}"
PROGRAM_TIMEOUT_SECONDS="${README_PROGRAM_TIMEOUT_SECONDS:-10}"
trap 'rm -rf "${VERIFY_DIR}"' EXIT

case "${PROGRAM_TIMEOUT_SECONDS}" in
    ''|*[!0-9]*)
        echo "README_PROGRAM_TIMEOUT_SECONDS 必须是正整数" >&2
        exit 2
        ;;
    0)
        echo "README_PROGRAM_TIMEOUT_SECONDS 必须大于 0" >&2
        exit 2
        ;;
esac

if ! command -v "${TIMEOUT_COMMAND}" >/dev/null 2>&1; then
    echo "缺少可执行的超时工具: ${TIMEOUT_COMMAND}" >&2
    echo "本门禁不会在没有内部超时保护时运行教材程序。" >&2
    exit 2
fi

# 周根验证器还负责锁定整周文档与跨 Day 真实模块，
# 因此可搬迁单元是整棵 week_04，而不是某一个 day_xx。
for required_day in 22 23 24 25 26 27 28; do
    if [ ! -f "${WEEK_DIR}/day_${required_day}/README.md" ]; then
        echo "Week 4 目录不完整: 缺少 day_${required_day}/README.md" >&2
        echo "请复制整棵 week_04 后再运行 README 程序门禁。" >&2
        exit 2
    fi
done

if [ "$#" -eq 0 ]; then
    DAYS=(22 23 24 25 26 27 28)
else
    DAYS=("$@")
fi

expected_programs() {
    case "$1" in
        22) echo 4 ;;
        23) echo 1 ;;
        24) echo 2 ;;
        25) echo 3 ;;
        26|27) echo 0 ;;
        28) echo 3 ;;
        *) return 1 ;;
    esac
}

expected_non_main_snippets() {
    case "$1" in
        27) echo 1 ;;
        22|23|24|25|26|28) echo 0 ;;
        *) return 1 ;;
    esac
}

require_text() {
    file="$1"
    expected_text="$2"
    if ! grep -Fq "${expected_text}" "${file}"; then
        echo "教学契约文本漂移: ${file}" >&2
        echo "缺少: ${expected_text}" >&2
        exit 4
    fi
}

reject_text() {
    file="$1"
    stale_text="$2"
    if grep -Fq "${stale_text}" "${file}"; then
        echo "教学契约仍含陈旧表述: ${file}" >&2
        echo "命中: ${stale_text}" >&2
        exit 4
    fi
}

total=0
non_main_total=0
for day in "${DAYS[@]}"; do
    expected="$(expected_programs "${day}")" || {
        echo "不支持的 Day: ${day}" >&2
        exit 2
    }
    expected_non_main="$(expected_non_main_snippets "${day}")"

    day_dir="${WEEK_DIR}/day_${day}"
    block_dir="${VERIFY_DIR}/day_${day}"
    mkdir -p "${block_dir}"

    awk -v out="${block_dir}" '
        /^```cpp[[:space:]]*$/ {
            inside = 1
            ++block_index
            path = sprintf("%s/block_%02d.cpp", out, block_index)
            next
        }
        /^```[[:space:]]*$/ && inside {
            close(path)
            inside = 0
            next
        }
        inside { print > path }
    ' "${day_dir}/README.md"

    count=0
    non_main_count=0
    for source in "${block_dir}"/*.cpp; do
        [ -f "${source}" ] || continue

        if grep -q '^// WEEK04_VERIFY_NON_MAIN:' "${source}"; then
            non_main_count=$((non_main_count + 1))
            non_main_total=$((non_main_total + 1))
            "${COMPILER}" \
                -std=c++17 \
                -Wall \
                -Wextra \
                -Wpedantic \
                -Wconversion \
                -Wsign-conversion \
                -Wshadow \
                -Werror \
                -pedantic-errors \
                -c "${source}" \
                -o "${source%.cpp}.o"
        fi

        grep -Eq '^int main\(' "${source}" || continue

        count=$((count + 1))
        total=$((total + 1))
        binary="${source%.cpp}"
        command=(
            "${COMPILER}"
            -std=c++17
            -Wall
            -Wextra
            -Wpedantic
            -Wconversion
            -Wsign-conversion
            -Wshadow
            -Werror
            -pedantic-errors
            "-I${day_dir}"
            "${source}"
        )

        if grep -q '0146_lru_cache/solution.h' "${source}"; then
            command+=("${day_dir}/code/leetcode/0146_lru_cache/solution.cpp")
        elif grep -q '0460_lfu_cache/solution.h' "${source}"; then
            command+=("${day_dir}/code/leetcode/0460_lfu_cache/solution.cpp")
        elif grep -q '0438_find_anagrams/solution.h' "${source}"; then
            command+=("${day_dir}/code/leetcode/0438_find_anagrams/solution.cpp")
        fi

        command+=(-o "${binary}")
        "${command[@]}"
        if "${TIMEOUT_COMMAND}" --signal=TERM --kill-after=2s \
                "${PROGRAM_TIMEOUT_SECONDS}s" "${binary}" >/dev/null; then
            :
        else
            status=$?
            if [ "${status}" -eq 124 ] || [ "${status}" -eq 137 ]; then
                echo "README 完整程序运行超时 (${PROGRAM_TIMEOUT_SECONDS}s): ${source}" >&2
            else
                echo "README 完整程序运行失败 (退出码 ${status}): ${source}" >&2
            fi
            exit "${status}"
        fi
    done

    if [ "${count}" -ne "${expected}" ]; then
        echo "Day ${day} 完整程序数量漂移: 期望 ${expected}, 实际 ${count}" >&2
        exit 3
    fi
    if [ "${non_main_count}" -ne "${expected_non_main}" ]; then
        echo "Day ${day} 关键非 main 片段数量漂移: 期望 ${expected_non_main}, 实际 ${non_main_count}" >&2
        exit 3
    fi

    if [ "${day}" -eq 23 ]; then
        require_text "${day_dir}/code/leetcode/0454_4sum_ii/solution.cpp" \
            '两阶段合计约 2 * n² = '
        reject_text "${day_dir}/code/leetcode/0454_4sum_ii/solution.cpp" \
            '差距 10,000 倍'
    elif [ "${day}" -eq 25 ]; then
        require_text "${day_dir}/code/cpp11_features/std_forward_demo.cpp" \
            '不承诺所有权转移、移动构造一定发生或操作一定便宜'
        require_text "${day_dir}/code/cpp11_features/std_forward_demo.cpp" \
            'remove_reference_t<T>&&'
        require_text "${day_dir}/code/cpp11_features/std_forward_demo.cpp" \
            '仍有效但状态未指定'
        reject_text "${day_dir}/code/cpp11_features/std_forward_demo.cpp" \
            '明确表示要移动对象的所有权'
    elif [ "${day}" -eq 27 ]; then
        guide="${WEEK_DIR}/哈希表专题形象化题解指南.md"
        require_text "${guide}" '窗口: abcabcb[b] (left从5跳到7)'
        require_text "${guide}" '哈希: {a:3, b:7, c:5}（记录全局最后位置，不会删除窗口外字符）'
        require_text "${guide}" '回文: "aba", 长度3'
        require_text "${guide}" '| 10 ('"'"'A'"'"') | `[1,10] DOBECODEBA` |'
        require_text "${guide}" '| 12 ('"'"'C'"'"') | `[6,12] ODEBANC` |'
    fi

    echo "Day ${day}: ${count} 个 README 完整程序、${non_main_count} 个关键非 main 片段严格验证通过"
done

echo "README 验证完成: ${total} 个完整程序、${non_main_total} 个关键非 main 片段" \
     "(严格告警，非 Sanitizer，单程序超时 ${PROGRAM_TIMEOUT_SECONDS}s)"
