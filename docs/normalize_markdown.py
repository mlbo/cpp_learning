#!/usr/bin/env python3
from __future__ import annotations

import re
import os
import html
from pathlib import Path
from urllib.parse import quote


DOCS_DIR = Path(__file__).resolve().parent
CONTENT_DIR = DOCS_DIR / "content"
REPOSITORY_ROOT = DOCS_DIR.parent
GITHUB_BLOB_ROOT = "https://github.com/mlbo/cpp_learning/blob/master/"
HTML_ANCHOR = re.compile(r'^\s*<a\s+(?:id|name)="([^"]+)"\s*></a>\s*$', re.MULTILINE)
MARKDOWN_LINK = re.compile(r"(?<!!)\[([^\]]+)\]\(([^)]+)\)")
FRAGMENT_ALIASES = {
    "综合项目dynamicarray类": "dynamicarray",
}
BROKEN_GANTT = """gantt
    title 快慢指针执行过程
    dateFormat X
    axisFormat %s

    section 初始状态
    数组: [0,0,1,1,1,2,2,3,3,4] :0, 10

    section 第一步
    fast=1, nums[0]==nums[1] :1, 2

    section 第二步
    fast=2, nums[0]!=nums[2], slow++ :2, 3

    section 结果
    前slow+1个元素唯一 :5, 6"""
FIXED_GANTT = """flowchart LR
    A["初始：slow = 0，fast = 1"] --> B{"两位置上的值相同吗？"}
    B -- 是 --> C["跳过重复项"]
    B -- 否 --> D["slow 加一并写入新值"]
    C --> E{"还有未处理元素吗？"}
    D --> E
    E -- 是 --> F["fast 加一"]
    F --> B
    E -- 否 --> G["返回 slow 加一"]"""


def normalize_link(
    match: re.Match[str],
    markdown_path: Path,
    anchors_by_file: dict[Path, set[str]],
) -> str:
    label, target = match.groups()
    if target == "#":
        return label
    if target.startswith(("http://", "https://", "mailto:", "ftp://", "#")):
        return match.group(0)

    path_part, separator, fragment = target.partition("#")
    candidate = (markdown_path.parent / path_part).resolve()

    if candidate == (CONTENT_DIR / "README.md").resolve():
        relative_index = Path(os.path.relpath(DOCS_DIR / "index.md", markdown_path.parent)).as_posix()
        normalized = relative_index
        if separator:
            normalized += f"#{fragment}"
        return f"[{label}]({normalized})"

    if path_part.endswith("/"):
        candidate = (candidate / "README.md").resolve()
        try:
            candidate.relative_to(CONTENT_DIR.resolve())
        except ValueError:
            return match.group(0)
        if not candidate.is_file():
            return match.group(0)
        path_part = f"{path_part}README.md"

    resolved_target = (markdown_path.parent / path_part).resolve()

    try:
        repository_relative = resolved_target.relative_to(CONTENT_DIR.resolve())
    except ValueError:
        repository_relative = None

    if repository_relative is not None:
        repository_file = REPOSITORY_ROOT / repository_relative
        if repository_file.is_file() and repository_file.suffix.lower() != ".md":
            github_url = GITHUB_BLOB_ROOT + quote(repository_relative.as_posix())
            if separator:
                github_url += f"#{quote(fragment)}"
            return f"[{label}]({github_url})"

    use_raw_html = False
    if separator and fragment in FRAGMENT_ALIASES:
        fragment = FRAGMENT_ALIASES[fragment]
        use_raw_html = True

    if separator and resolved_target.suffix == ".md":
        explicit_anchors = anchors_by_file.get(resolved_target, set())
        if fragment in explicit_anchors:
            use_raw_html = True

    if use_raw_html and path_part.endswith(".md"):
        href = f"{path_part[:-3]}.html#{fragment}"
        return f'<a href="{html.escape(href, quote=True)}">{html.escape(label)}</a>'

    normalized = path_part
    if separator:
        normalized += f"#{fragment}"
    return f"[{label}]({normalized})"


def normalize_fences(text: str) -> str:
    lines = text.splitlines()
    index = 0
    while index < len(lines):
        if lines[index].strip() != "```cpp":
            index += 1
            continue
        end = index + 1
        while end < len(lines) and lines[end].strip() != "```":
            end += 1
        block = "\n".join(lines[index + 1 : end])
        if "↑" in block or "↓" in block:
            lines[index] = lines[index].replace("cpp", "text", 1)
        index = end + 1
    return "\n".join(lines) + ("\n" if text.endswith("\n") else "")


def normalize_mermaid(text: str) -> str:
    """Adapt one legacy diagram that Mermaid 11 can no longer parse."""
    return text.replace(BROKEN_GANTT, FIXED_GANTT)


def normalize_markdown(path: Path, anchors_by_file: dict[Path, set[str]]) -> None:
    text = path.read_text(encoding="utf-8")
    text = MARKDOWN_LINK.sub(
        lambda match: normalize_link(match, path, anchors_by_file),
        text,
    )
    text = normalize_fences(text)
    text = normalize_mermaid(text)
    path.write_text(text, encoding="utf-8")


def main() -> None:
    markdown_paths = sorted(CONTENT_DIR.rglob("*.md"))
    anchors_by_file = {
        path.resolve(): set(HTML_ANCHOR.findall(path.read_text(encoding="utf-8")))
        for path in markdown_paths
    }
    for markdown_path in markdown_paths:
        normalize_markdown(markdown_path, anchors_by_file)


if __name__ == "__main__":
    main()
