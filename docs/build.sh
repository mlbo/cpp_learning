#!/usr/bin/env bash
set -euo pipefail

DOCS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "${DOCS_DIR}/.." && pwd)"
BUILD_DIR="${DOCS_DIR}/_build"
OUTPUT_DIR="${BUILD_DIR}/html"
VENV_DIR="${ROOT_DIR}/.venv-docs"
REQUIREMENTS_FILE="${DOCS_DIR}/requirements.txt"
REQUIREMENTS_STAMP="${VENV_DIR}/.requirements-checksum"
BOOTSTRAP_PYTHON="${PYTHON:-python3}"

if [[ -n "${SPHINXBUILD:-}" ]]; then
  # 用户显式指定了 sphinx-build，直接用它（不强制走 venv）。
  SPHINX_BUILD="${SPHINXBUILD}"
  if ! command -v "${SPHINX_BUILD}" >/dev/null 2>&1; then
    echo "未找到 SPHINXBUILD 指定的程序：${SPHINX_BUILD}" >&2
    exit 127
  fi
else
  # 复用 start_server.sh 的同一套 venv 准备逻辑：建 venv、按 requirements 装依赖、用 venv 的 sphinx-build。
  if ! command -v "${BOOTSTRAP_PYTHON}" >/dev/null 2>&1; then
    echo "未找到用于创建文档环境的 Python：${BOOTSTRAP_PYTHON}" >&2
    echo "可用 PYTHON=...</path/to/python3> bash docs/build.sh 指定。" >&2
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
    # 若依赖导入检测失败（不只是 checksum 变），venv 可能已损坏或 Python 版本已升级，
    # 残留的编译扩展/元数据会让增量装出怪问题。此时先删 venv 再重建，保证干净。
    if ! "${VENV_DIR}/bin/python" -c \
         'import jieba, myst_parser, sphinx, sphinx_book_theme, sphinx_copybutton, sphinxcontrib.mermaid' \
         >/dev/null 2>&1; then
      echo "依赖导入失败，重建虚拟环境 ${VENV_DIR}……"
      rm -rf -- "${VENV_DIR}"
      "${BOOTSTRAP_PYTHON}" -m venv "${VENV_DIR}"
    fi
    echo "正在安装完整的 Sphinx 文档依赖……"
    "${VENV_DIR}/bin/python" -m pip install \
      --disable-pip-version-check \
      -r "${REQUIREMENTS_FILE}"
    printf '%s\n' "${requirements_checksum}" >"${REQUIREMENTS_STAMP}"
  fi

  SPHINX_BUILD="${VENV_DIR}/bin/sphinx-build"
fi

"${DOCS_DIR}/sync_from_repo.sh"

if [[ "${BUILD_DIR}" != "${DOCS_DIR}/_build" ]]; then
  echo "Refusing to replace unexpected Sphinx build directory: ${BUILD_DIR}" >&2
  exit 1
fi
rm -rf -- "${BUILD_DIR}"

"${SPHINX_BUILD}" \
  -b html \
  -W \
  -n \
  --keep-going \
  -E \
  -a \
  "${DOCS_DIR}" \
  "${OUTPUT_DIR}"

echo "Sphinx site generated at: ${OUTPUT_DIR}/index.html"
