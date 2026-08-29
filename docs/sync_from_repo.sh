#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
DOCS_DIR="${ROOT_DIR}/docs"
CONTENT_DIR="${DOCS_DIR}/content"

if [[ "${CONTENT_DIR}" != "${DOCS_DIR}/content" ]]; then
  echo "Refusing to replace unexpected content directory: ${CONTENT_DIR}" >&2
  exit 1
fi

rm -rf -- "${CONTENT_DIR}"
mkdir -p "${CONTENT_DIR}"

sync_filtered_dir() {
  local src="$1"
  local dst="$2"
  rsync -a --delete --delete-excluded --prune-empty-dirs \
    --exclude '*REPORT*.md' --exclude '*report*.md' \
    --exclude 'build*/' --exclude 'build*/**' \
    --exclude 'CMakeFiles/' --exclude 'CMakeFiles/**' \
    --include '*/' \
    --include '*.md' \
    --exclude '*' \
    "${src}" "${dst}"
}

cp -f "${ROOT_DIR}/CPP_35天科学学习规划_最终版.md" "${CONTENT_DIR}/CPP_35天科学学习规划_最终版.md"
cp -f "${ROOT_DIR}/Modern_CPP_Features_Deep_Dive.md" "${CONTENT_DIR}/Modern_CPP_Features_Deep_Dive.md"

sync_filtered_dir "${ROOT_DIR}/tutorials/" "${CONTENT_DIR}/tutorials/"

for w in week_01 week_02 week_03 week_04 week_05; do
  sync_filtered_dir "${ROOT_DIR}/${w}/" "${CONTENT_DIR}/${w}/"
done

sync_filtered_dir "${ROOT_DIR}/model_inference_project/" "${CONTENT_DIR}/model_inference_project/"

find "${CONTENT_DIR}" -type f \( -name '*REPORT*.md' -o -name '*report*.md' \) -delete

echo "Docs copied into: ${CONTENT_DIR}"

python3 "${DOCS_DIR}/normalize_markdown.py"
python3 "${DOCS_DIR}/generate_navigation.py"
echo "Sphinx-compatible Markdown and navigation generated"
