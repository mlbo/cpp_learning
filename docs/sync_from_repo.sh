#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
DOCS_DIR="${ROOT_DIR}/docs"
CONTENT_DIR="${DOCS_DIR}/content"

mkdir -p "${CONTENT_DIR}/00-overview" "${CONTENT_DIR}/01-tutorials" "${CONTENT_DIR}/02-weeks" "${CONTENT_DIR}/03-projects"

sync_filtered_dir() {
  local src="$1"
  local dst="$2"
  rsync -a --delete --prune-empty-dirs \
    --exclude '*REPORT*.md' --exclude '*report*.md' \
    --exclude 'build/' --exclude 'build/**' \
    --exclude 'CMakeFiles/' --exclude 'CMakeFiles/**' \
    --include '*/' \
    --include '*.md' \
    --include '*.c' --include '*.cc' --include '*.cpp' --include '*.cxx' \
    --include '*.h' --include '*.hh' --include '*.hpp' --include '*.hxx' \
    --include '*.ipp' --include '*.inl' \
    --include 'CMakeLists.txt' --include '*.cmake' \
    --include '*.sh' --include '*.py' --include '*.txt' \
    --exclude '*' \
    "${src}" "${dst}"
}

cp -f "${ROOT_DIR}/CPP_35天科学学习规划_最终版.md" "${CONTENT_DIR}/00-overview/CPP_35天科学学习规划_最终版.md"
cp -f "${ROOT_DIR}/Modern_CPP_Features_Deep_Dive.md" "${CONTENT_DIR}/00-overview/Modern_CPP_Features_Deep_Dive.md"

sync_filtered_dir "${ROOT_DIR}/tutorials/" "${CONTENT_DIR}/01-tutorials/"

for w in week_01 week_02 week_03 week_04 week_05; do
  sync_filtered_dir "${ROOT_DIR}/${w}/" "${CONTENT_DIR}/02-weeks/${w}/"
done

sync_filtered_dir "${ROOT_DIR}/model_inference_project/" "${CONTENT_DIR}/03-projects/model_inference_project/"

find "${CONTENT_DIR}" -type f \( -name '*REPORT*.md' -o -name '*report*.md' \) -delete

cat > "${CONTENT_DIR}/01-tutorials/README.md" <<'EOF'
# Tutorials

- [C++ 基础学习教程](CPP基础学习教程.md)
- [Effective Modern C++ 教程](Effective_Modern_CPP教程.md)
- [C++ 并发编程教程](CPP并发编程教程.md)
- [设计模式教程](设计模式教程.md)
- [操作系统面试常考知识](OS面试常考知识.md)
EOF

echo "Docs copied into: ${CONTENT_DIR}"

python3 "${DOCS_DIR}/generate_sidebar.py"
echo "Sidebar generated: ${DOCS_DIR}/_sidebar.md"
