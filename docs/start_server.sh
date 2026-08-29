#!/usr/bin/env bash
set -euo pipefail

DOCS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "${DOCS_DIR}/.." && pwd)"
BUILD_DIR="${DOCS_DIR}/_build"
OUTPUT_DIR="${BUILD_DIR}/html"
PID_FILE="${DOCS_DIR}/.preview-server.pid"
LOG_FILE="${DOCS_DIR}/.preview-server.log"
VENV_DIR="${ROOT_DIR}/.venv-docs"
REQUIREMENTS_FILE="${DOCS_DIR}/requirements.txt"
REQUIREMENTS_STAMP="${VENV_DIR}/.requirements-checksum"
HOST="${DOCS_HOST:-127.0.0.1}"
PORT="${DOCS_PORT:-8000}"
BOOTSTRAP_PYTHON="${PYTHON:-python3}"

if [[ ! "${PORT}" =~ ^[0-9]+$ ]] || ((PORT < 1 || PORT > 65535)); then
  echo "DOCS_PORT 必须是 1 到 65535 之间的整数，当前值：${PORT}" >&2
  exit 2
fi

if [[ -f "${PID_FILE}" ]]; then
  server_pid="$(<"${PID_FILE}")"
  if [[ "${server_pid}" =~ ^[0-9]+$ ]] && kill -0 "${server_pid}" 2>/dev/null; then
    echo "本地文档服务已在运行（PID ${server_pid}）。"
    echo "访问地址：http://localhost:${PORT}/"
    exit 0
  fi
  rm -f -- "${PID_FILE}"
fi

if [[ -n "${SPHINXBUILD:-}" ]]; then
  SPHINX_BUILD="${SPHINXBUILD}"
  SERVER_PYTHON="${BOOTSTRAP_PYTHON}"
  if ! command -v "${SPHINX_BUILD}" >/dev/null 2>&1; then
    echo "未找到 SPHINXBUILD 指定的程序：${SPHINX_BUILD}" >&2
    exit 127
  fi
  if ! command -v "${SERVER_PYTHON}" >/dev/null 2>&1; then
    echo "未找到 Python：${SERVER_PYTHON}" >&2
    exit 127
  fi
else
  if ! command -v "${BOOTSTRAP_PYTHON}" >/dev/null 2>&1; then
    echo "未找到用于创建文档环境的 Python：${BOOTSTRAP_PYTHON}" >&2
    exit 127
  fi

  if [[ ! -x "${VENV_DIR}/bin/python" ]]; then
    echo "首次运行：正在创建文档虚拟环境 ${VENV_DIR}"
    "${BOOTSTRAP_PYTHON}" -m venv "${VENV_DIR}"
  fi

  requirements_checksum="$(cksum "${REQUIREMENTS_FILE}" | awk '{print $1 ":" $2}')"
  installed_checksum=""
  if [[ -f "${REQUIREMENTS_STAMP}" ]]; then
    installed_checksum="$(<"${REQUIREMENTS_STAMP}")"
  fi

  if [[ "${installed_checksum}" != "${requirements_checksum}" ]] || \
     ! "${VENV_DIR}/bin/python" -c \
       'import jieba, myst_parser, sphinx, sphinx_book_theme, sphinx_copybutton, sphinxcontrib.mermaid' \
       >/dev/null 2>&1; then
    echo "正在安装完整的 Sphinx 文档依赖……"
    "${VENV_DIR}/bin/python" -m pip install \
      --disable-pip-version-check \
      -r "${REQUIREMENTS_FILE}"
    printf '%s\n' "${requirements_checksum}" >"${REQUIREMENTS_STAMP}"
  fi

  SPHINX_BUILD="${VENV_DIR}/bin/sphinx-build"
  SERVER_PYTHON="${VENV_DIR}/bin/python"
fi

SPHINXBUILD="${SPHINX_BUILD}" "${DOCS_DIR}/build.sh"

nohup "${SERVER_PYTHON}" -m http.server "${PORT}" \
  --bind "${HOST}" \
  --directory "${OUTPUT_DIR}" \
  >"${LOG_FILE}" 2>&1 &
server_pid=$!
printf '%s\n' "${server_pid}" >"${PID_FILE}"

sleep 0.2
if ! kill -0 "${server_pid}" 2>/dev/null; then
  rm -f -- "${PID_FILE}"
  echo "本地文档服务启动失败，日志如下：" >&2
  sed -n '1,80p' "${LOG_FILE}" >&2
  exit 1
fi

echo "本地文档服务已启动（PID ${server_pid}）。"
echo "访问地址：http://localhost:${PORT}/"
echo "日志文件：${LOG_FILE}"
echo "关闭服务：./docs/stop_server.sh"

if [[ "${DOCS_OPEN_BROWSER:-0}" == "1" ]] && command -v xdg-open >/dev/null 2>&1; then
  xdg-open "http://localhost:${PORT}/" >/dev/null 2>&1 || true
fi
