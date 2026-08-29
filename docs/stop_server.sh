#!/usr/bin/env bash
set -euo pipefail

DOCS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${DOCS_DIR}/_build"
OUTPUT_DIR="${BUILD_DIR}/html"
PID_FILE="${DOCS_DIR}/.preview-server.pid"

if [[ ! -f "${PID_FILE}" ]]; then
  echo "本地文档服务未运行。"
  exit 0
fi

server_pid="$(<"${PID_FILE}")"
if [[ ! "${server_pid}" =~ ^[0-9]+$ ]]; then
  rm -f -- "${PID_FILE}"
  echo "PID 文件无效，已清理；没有终止任何进程。" >&2
  exit 1
fi

if ! kill -0 "${server_pid}" 2>/dev/null; then
  rm -f -- "${PID_FILE}"
  echo "本地文档服务已经停止，已清理过期 PID 文件。"
  exit 0
fi

command_line="$(ps -p "${server_pid}" -o args= 2>/dev/null || true)"
# 校验 PID 对应的命令行确实是本预览服务（含 -m http.server 和输出目录），避免误杀。
# 残余风险：校验与 kill 间有极小的 TOCTOU 窗口，若原进程此时退出且 PID 被复用给
# 带相同命令行的进程会误杀——教学脚本可接受；生产级可改进程组(setsid+kill -- -PID)
# 或存 starttime 用 /proc/[pid]/stat 二次校验。
if [[ "${command_line}" != *"-m http.server"* || "${command_line}" != *"${OUTPUT_DIR}"* ]]; then
  echo "PID ${server_pid} 不是本教程的预览服务，没有终止该进程。" >&2
  echo "实际命令：${command_line}" >&2
  exit 1
fi

kill "${server_pid}"
for _ in {1..50}; do
  if ! kill -0 "${server_pid}" 2>/dev/null; then
    rm -f -- "${PID_FILE}"
    echo "本地文档服务已关闭。"
    exit 0
  fi
  sleep 0.1
done

kill -KILL "${server_pid}"
rm -f -- "${PID_FILE}"
echo "本地文档服务未及时退出，已强制关闭。"
