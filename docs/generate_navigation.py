#!/usr/bin/env python3
from __future__ import annotations

import os
import shutil
from pathlib import Path


DOCS_DIR = Path(__file__).resolve().parent
CONTENT_DIR = DOCS_DIR / "content"
GENERATED_DIR = DOCS_DIR / "_generated"
NAV_START = "<!-- sphinx-navigation-start -->"
NAV_END = "<!-- sphinx-navigation-end -->"
SUPPLEMENT_START = "<!-- sphinx-supplements-start -->"
SUPPLEMENT_END = "<!-- sphinx-supplements-end -->"


def read_title(path: Path) -> str:
    for line in path.read_text(encoding="utf-8").splitlines():
        stripped = line.strip()
        if stripped.startswith("# "):
            return stripped[2:].strip()
    return path.stem


def doc_target(path: Path, parent: Path) -> str:
    target = Path(os.path.relpath(path.with_suffix(""), parent))
    return target.as_posix()


def toctree(entries: list[Path], parent: Path, *, hidden: bool, maxdepth: int) -> str:
    if not entries:
        return ""

    lines = ["```{toctree}", f":maxdepth: {maxdepth}"]
    if hidden:
        lines.append(":hidden:")
    lines.append("")
    lines.extend(doc_target(entry, parent) for entry in entries)
    lines.append("```")
    return "\n".join(lines)


def write_index(path: Path, title: str, introduction: str, entries: list[Path]) -> None:
    body = [f"# {title}", "", introduction]
    tree = toctree(entries, path.parent, hidden=False, maxdepth=2)
    if tree:
        body.extend(["", tree])
    path.write_text("\n".join(body).rstrip() + "\n", encoding="utf-8")


def append_hidden_navigation(parent_doc: Path, entries: list[Path]) -> None:
    text = parent_doc.read_text(encoding="utf-8")
    if NAV_START in text:
        text = text.split(NAV_START, 1)[0].rstrip()

    tree = toctree(entries, parent_doc.parent, hidden=True, maxdepth=2)
    if not tree:
        parent_doc.write_text(text.rstrip() + "\n", encoding="utf-8")
        return

    block = f"{NAV_START}\n{tree}\n{NAV_END}"
    parent_doc.write_text(f"{text.rstrip()}\n\n{block}\n", encoding="utf-8")


def mark_orphan(path: Path) -> None:
    text = path.read_text(encoding="utf-8")
    if text.startswith("---\norphan: true\n---\n"):
        return
    path.write_text(f"---\norphan: true\n---\n\n{text}", encoding="utf-8")


def append_supplements(parent_doc: Path, entries: list[Path]) -> None:
    if not entries:
        return

    text = parent_doc.read_text(encoding="utf-8")
    if SUPPLEMENT_START in text:
        text = text.split(SUPPLEMENT_START, 1)[0].rstrip()

    lines = [SUPPLEMENT_START, "## 补充题解", ""]
    for entry in entries:
        target = Path(os.path.relpath(entry, parent_doc.parent)).as_posix()
        lines.append(f"- [{read_title(entry)}]({target})")
    lines.append(SUPPLEMENT_END)
    parent_doc.write_text(f"{text.rstrip()}\n\n" + "\n".join(lines) + "\n", encoding="utf-8")


def markdown_children(directory: Path, *, recursive: bool) -> list[Path]:
    iterator = directory.rglob("*.md") if recursive else directory.glob("*.md")
    own_readme = directory / "README.md"
    return sorted(
        path
        for path in iterator
        if path != own_readme and "REPORT" not in path.name.upper()
    )


def sorted_numbered_dirs(directory: Path, prefix: str) -> list[Path]:
    def key(path: Path) -> tuple[int, str]:
        suffix = path.name.removeprefix(prefix)
        return (int(suffix), path.name) if suffix.isdigit() else (10_000, path.name)

    return sorted(
        (path for path in directory.iterdir() if path.is_dir() and path.name.startswith(prefix)),
        key=key,
    )


def build_navigation() -> None:
    if GENERATED_DIR.exists():
        shutil.rmtree(GENERATED_DIR)
    GENERATED_DIR.mkdir(parents=True)

    referenced: set[Path] = set()

    overview_entries = [
        CONTENT_DIR / "CPP_35天科学学习规划_最终版.md",
        CONTENT_DIR / "Modern_CPP_Features_Deep_Dive.md",
    ]
    referenced.update(overview_entries)
    write_index(
        GENERATED_DIR / "overview.md",
        "课程概览",
        "先用学习规划建立全局路线，再按需查阅现代 C++ 特性手册。",
        overview_entries,
    )

    tutorial_dir = CONTENT_DIR / "tutorials"
    tutorial_entries = markdown_children(tutorial_dir, recursive=True)
    referenced.update(tutorial_entries)
    write_index(
        GENERATED_DIR / "tutorials.md",
        "系统教程",
        "这里集中基础语法、项目设计、Effective Modern C++、并发、设计模式和操作系统知识。",
        tutorial_entries,
    )

    week_entries: list[Path] = []
    for week_dir in sorted_numbered_dirs(CONTENT_DIR, "week_"):
        week_readme = week_dir / "README.md"
        if not week_readme.exists():
            continue
        week_entries.append(week_readme)
        referenced.add(week_readme)

        week_children = markdown_children(week_dir, recursive=False)
        referenced.update(week_children)
        for day_dir in sorted_numbered_dirs(week_dir, "day_"):
            day_readme = day_dir / "README.md"
            if not day_readme.exists():
                continue
            week_children.append(day_readme)
            referenced.add(day_readme)
            day_children = markdown_children(day_dir, recursive=True)
            referenced.update(day_children)
            for child in day_children:
                mark_orphan(child)
            append_supplements(day_readme, day_children)
        append_hidden_navigation(week_readme, week_children)

    write_index(
        GENERATED_DIR / "weeks.md",
        "35 天逐日课程",
        "五周课程按前置知识逐层推进；请先完成当天自测、工程动作和五句复盘，再进入下一天。",
        week_entries,
    )

    project_dir = CONTENT_DIR / "model_inference_project"
    project_readme = project_dir / "README.md"
    project_entries = [project_readme] if project_readme.exists() else []
    if project_readme.exists():
        referenced.add(project_readme)
        project_children = markdown_children(project_dir, recursive=True)
        referenced.update(project_children)
        append_hidden_navigation(project_readme, project_children)
    write_index(
        GENERATED_DIR / "projects.md",
        "综合项目",
        "综合项目用于练习接口设计、资源所有权、并发协议、测试和多文件工程组织。",
        project_entries,
    )

    all_docs = set(CONTENT_DIR.rglob("*.md"))
    unreferenced = sorted(all_docs - referenced)
    if unreferenced:
        relative = [path.relative_to(CONTENT_DIR).as_posix() for path in unreferenced]
        raise RuntimeError(f"Markdown not included in navigation: {relative}")


if __name__ == "__main__":
    build_navigation()
